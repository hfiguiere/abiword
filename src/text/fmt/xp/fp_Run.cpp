/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
 * 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fp_Run.h"
#include "fl_DocLayout.h"
#include "fl_BlockLayout.h"
#include "fp_Line.h"
#include "fp_Column.h"
#include "fp_Page.h"
#include "pp_Property.h"
#include "gr_Graphics.h"
#include "pd_Document.h"
#include "gr_DrawArgs.h"
#include "fv_View.h"
#include "pp_AttrProp.h"
#include "fd_Field.h"

#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_string.h"
#include "ut_growbuf.h"

#include "ap_Prefs.h"

// TODO can we use the indexAP provided in the change records
// TODO to remember the attr/prop for each run rather than
// TODO looking it up each time we call lookupProperties() -- jeff 4/19/99


// findPointCoords:
//  Can be called to find the position and size of the point (cursor)
//  for an offset both before, inside and after the Run.
//   Before: When the Run is the first on a Line and point is a BOL.
//   Inside: When the point is inside the Run.
//   After : Point is at the start of the next Run, but insertion is
//           done with the properties of this Run so cursor size/position
//           must reflect that.
//
//  Previous implementations would assert that the offset was within the
//  Run, but that would always fail for the 'After' case.

/*****************************************************************/

/*
  TODO this file is too long -- it needs to be broken
  up into several pieces.
*/

fp_Run::fp_Run(fl_BlockLayout* pBL,
			   GR_Graphics* pG,
			   UT_uint32 iOffsetFirst,
			   UT_uint32 iLen,
			   FP_RUN_TYPE iType)
:	m_iType (iType),
	m_pLine(0),
	m_pBL(pBL),
	m_pNext(0),
	m_pPrev(0),
	m_iX(0),
	m_iY(0),
	m_iHeight(0),
	m_iHeightLayoutUnits(0),
	m_iWidth(0),
	m_iWidthLayoutUnits(0),
	m_iOffsetFirst(iOffsetFirst),
	m_iLen(iLen),
	m_iAscent(0),
	m_iDescent(0),
	m_iAscentLayoutUnits(0),
	m_iDescentLayoutUnits(0),
	m_pG(pG),
	m_bDirty(true),	// a run which has just been created is not onscreen, therefore it is dirty
	m_pField(0)
{
	// set a default background color
	UT_setColor(m_colorBG, 255, 255, 255);

#ifdef BIDI_ENABLED
	m_iDirection = -1; //by default all runs are whitespace
#endif

}


fp_Run::~fp_Run()
{
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void fp_Run::insertIntoRunListBeforeThis(fp_Run& newRun)
{
	newRun.unlinkFromRunList();
	newRun.m_pNext = this;
	if (m_pPrev)
	{
		m_pPrev->m_pNext = &newRun;
	}
	newRun.m_pPrev = m_pPrev;
	m_pPrev = &newRun;
}

void fp_Run::insertIntoRunListAfterThis(fp_Run& newRun)
{
	newRun.unlinkFromRunList();
	newRun.m_pPrev = this;
	if (m_pNext)
	{
		m_pNext->m_pPrev = &newRun;
	}
	newRun.m_pNext = m_pNext;
	m_pNext = &newRun;
}

void fp_Run::unlinkFromRunList()
{
	if (m_pPrev)
	{
		m_pPrev->m_pNext = m_pNext;
	}
	if (m_pNext)
	{
		m_pNext->m_pPrev = m_pPrev;
		m_pNext = 0;
	}
	m_pPrev = 0;
}

void	fp_Run::setX(UT_sint32 iX)
{
	if (iX == m_iX)
	{
		return;
	}

	clearScreen();
	
	m_iX = iX;
}

void	fp_Run::setY(UT_sint32 iY)
{
	if (iY == m_iY)
	{
		return;
	}
	
	clearScreen();
	
	m_iY = iY;
}
	
void fp_Run::setLine(fp_Line* pLine)
{
	if (pLine == m_pLine)
	{
		return;
	}
	
	clearScreen();
	
	m_pLine = pLine;
}

void fp_Run::setBlock(fl_BlockLayout * pBL)
{
	m_pBL = pBL;
}

void fp_Run::setNext(fp_Run* p)
{
	m_pNext = p;
}

void fp_Run::setPrev(fp_Run* p)
{
	m_pPrev = p;
}

bool fp_Run::isLastRunOnLine(void) const
{
	return (m_pLine->getLastRun() == this);
}

bool fp_Run::isFirstRunOnLine(void) const
{
	return (m_pLine->getFirstRun() == this);
}

#ifdef BIDI_ENABLED
bool fp_Run::isLastVisRunOnLine(void) const
{
	return (m_pLine->getLastVisRun() == this);
}

bool fp_Run::isFirstVisRunOnLine(void) const
{
	return (m_pLine->getFirstVisRun() == this);
}
#endif

bool fp_Run::isOnlyRunOnLine(void) const
{
	if (m_pLine->countRuns() == 1)
	{
		UT_ASSERT(isFirstRunOnLine());
		UT_ASSERT(isLastRunOnLine());

		return true;
	}

	return false;
}

void fp_Run::setLength(UT_uint32 iLen)
{
	if (iLen == m_iLen)
	{
		return;
	}

	clearScreen();
	
	m_iLen = iLen;
}

void fp_Run::setBlockOffset(UT_uint32 offset)
{
	m_iOffsetFirst = offset;
}

void fp_Run::clearScreen(bool bFullLineHeightRect)
{

	if (m_bDirty)
	{
		// no need to clear if we've already done so.
		return;
	}

	if (!m_pLine)
	{
		// nothing to clear if this run is not currently on a line
		return;
	}
	
	UT_ASSERT(m_pG->queryProperties(GR_Graphics::DGP_SCREEN));

	_clearScreen(bFullLineHeightRect);
	
	// make sure we only get erased once
	m_bDirty = true;
}

void fp_Run::draw(dg_DrawArgs* pDA)
{

	if (pDA->bDirtyRunsOnly)
	{
		if (!m_bDirty)
		{
			return;
		}
	}
	
	_draw(pDA);

	m_bDirty = false;
}

bool fp_Run::canContainPoint(void) const
{
	return true;
}

bool fp_Run::letPointPass(void) const
{
	return true;
}

void fp_Run::fetchCharWidths(fl_CharWidths * /* pgbCharWidths */)
{
	// do nothing.  subclasses may override this.
}

bool fp_Run::recalcWidth(void)
{
	// do nothing.  subclasses may override this.
	return false;
}

const PP_AttrProp* fp_Run::getAP(void) const
{
	const PP_AttrProp * pSpanAP = NULL;
	
	m_pBL->getSpanAttrProp(m_iOffsetFirst,false,&pSpanAP);

	return pSpanAP;
}

void fp_Run::_drawTextLine(UT_sint32 xoff,UT_sint32 yoff,UT_uint32 iWidth,UT_uint32 iHeight,UT_UCSChar *pText)
{

    m_pG->setFont(m_pG->getGUIFont());

    UT_uint32 iTextLen = UT_UCS_strlen(pText);
    UT_uint32 iTextWidth = m_pG->measureString(pText,0,iTextLen,NULL);
    UT_uint32 iTextHeight = m_pG->getFontHeight();

    UT_uint32 xoffText = xoff + (iWidth - iTextWidth) / 2;
    UT_uint32 yoffText = yoff - m_pG->getFontAscent() * 2 / 3;

    m_pG->drawLine(xoff,yoff,xoff + iWidth,yoff);

    //UT_RGBColor clrPaper(255,255,255);

    UT_DEBUGMSG(("DOM: drawTextLine\n"));
    if((iTextWidth < iWidth) && (iTextHeight < iHeight)){
        m_pG->fillRect(m_colorBG,xoffText,yoffText,iTextWidth,iTextHeight);
        m_pG->drawChars(pText,0,iTextLen,xoffText,yoffText);
    }
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fp_TabRun::fp_TabRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_Run(pBL, pG, iOffsetFirst, iLen, FPRUN_TAB)
{
	lookupProperties();
}

void fp_TabRun::lookupProperties(void)
{
	const PP_AttrProp * pSpanAP = NULL;
	const PP_AttrProp * pBlockAP = NULL;
	const PP_AttrProp * pSectionAP = NULL; // TODO do we care about section-level inheritance?
	
	m_pBL->getSpanAttrProp(m_iOffsetFirst,false,&pSpanAP);
	m_pBL->getAttrProp(&pBlockAP);
        m_pBL->getField(m_iOffsetFirst,m_pField);


	// look for fonts in this DocLayout's font cache
	FL_DocLayout * pLayout = m_pBL->getDocLayout();
	GR_Font* pFont = pLayout->findFont(pSpanAP,pBlockAP,pSectionAP, FL_DocLayout::FIND_FONT_AT_SCREEN_RESOLUTION);

    UT_parseColor(PP_evalProperty("color",pSpanAP,pBlockAP,pSectionAP, m_pBL->getDocument(), true), m_colorFG);

    UT_parseColor(PP_evalProperty("bgcolor",pSpanAP,pBlockAP,pSectionAP, m_pBL->getDocument(), true), m_colorBG);

	m_pG->setFont(pFont);
	m_iAscent = m_pG->getFontAscent();	
	m_iDescent = m_pG->getFontDescent();
	m_iHeight = m_pG->getFontHeight();

	pFont = pLayout->findFont(pSpanAP,pBlockAP,pSectionAP, FL_DocLayout::FIND_FONT_AT_LAYOUT_RESOLUTION);

	m_pG->setFont(pFont);
	m_iAscentLayoutUnits = m_pG->getFontAscent();	
	m_iDescentLayoutUnits = m_pG->getFontDescent();
	m_iHeightLayoutUnits = m_pG->getFontHeight();
#ifdef BIDI_ENABLED
	//#TF need to retrieve the direction of this run
	//check the preferences to see whether to use Unicode direction of text
	/*XAP_App * pApp = XAP_App::getApp();
	bool bAppDirection;
	pApp->getPrefsValueBool((XML_Char *) AP_PREF_KEY_UseUnicodeDirection, &bAppDirection);
	
	if(!bAppDirection)
	{
	    const XML_Char * pszDirection = PP_evalProperty("dir",pSpanAP,pBlockAP,pSectionAP, pDoc, true);
	    if(!UT_stricmp(pszDirection, "rtl"))  m_iDirection = 1;
	    else                                  m_iDirection = 0;
	}
	else m_iDirection = -1;      */
	m_iDirection = -1;
#endif
}

bool fp_TabRun::canBreakAfter(void) const
{
	return false;
}

bool fp_TabRun::canBreakBefore(void) const
{
	return false;
}

bool fp_TabRun::letPointPass(void) const
{
	return true;
}

bool	fp_TabRun::findMaxLeftFitSplitPointInLayoutUnits(UT_sint32 /* iMaxLeftWidth */, fp_RunSplitInfo& /* si */, bool /* bForce */)
{
	return false;
}

void fp_TabRun::mapXYToPosition(UT_sint32 x, UT_sint32 /*y*/, PT_DocPosition& pos, bool& bBOL, bool& bEOL)
{
	// If X is left of the middle, return offset to the left,
	// otherwise the offset to the right.
	if (x < (getWidth() / 2))
		pos = m_pBL->getPosition() + m_iOffsetFirst;
	else
		pos = m_pBL->getPosition() + m_iOffsetFirst + m_iLen;
		
	bBOL = false;
	bEOL = false;
}

void fp_TabRun::findPointCoords(UT_uint32 iOffset, UT_sint32& x, UT_sint32& y, UT_sint32& x2, UT_sint32& y2, UT_sint32& height, bool& bDirection)
{
	UT_sint32 xoff;
	UT_sint32 yoff;
#ifdef BIDI_ENABLED
	UT_sint32 xoff2;
	UT_sint32 yoff2;
#endif
	UT_ASSERT(m_pLine);
	
	m_pLine->getOffsets(this, xoff, yoff);
#ifdef BIDI_ENABLED
	fp_Run * pRun = 0;
	UT_sint32 iNextDir;

	if (iOffset == (m_iOffsetFirst + m_iLen))  //#TF is this the right-most logical element of the run?
	{
	    pRun = getNext();
	    if(pRun)
	    {
	        pRun->getLine()->getOffsets(pRun, xoff2, yoff2);
	        iNextDir = pRun->getVisDirection();
	    }
	}
	
	UT_sint32 iDirection = getVisDirection();

	if(iDirection == 1)
	    x = xoff - 1;      //necessary to draw the the caret properly -- I am not sure why though #TF
	else
	    x = xoff + getWidth() + 1;
	
	
	if(pRun && (iNextDir != iDirection)) //if this run precedes run of different direction, we have to split the caret
	{
	    x2 = (iNextDir == 0) ?  xoff + pRun->getWidth() + 1 : xoff2 - 1;
	    y2 = yoff2;
	}
	else
	{
	    x2 = x;
	    y2 = yoff;
	}
	bDirection = iDirection;
#else
	if (iOffset == m_iOffsetFirst)
	{
		x = xoff;
	}
	else
	{
		UT_ASSERT(iOffset == (m_iOffsetFirst + m_iLen));
		
		x = xoff + getWidth();
	}
#endif	
	y = yoff;
	height = m_iHeight;
}

void fp_TabRun::setWidth(UT_sint32 iWidth)
{
  clearScreen();
  m_iWidth = iWidth;
}

void fp_TabRun::_clearScreen(bool /* bFullLineHeightRect */)
{
	UT_ASSERT(!m_bDirty);
	UT_ASSERT(m_pG->queryProperties(GR_Graphics::DGP_SCREEN));

	UT_sint32 xoff = 0, yoff = 0;
	
	// need to clear full height of line, in case we had a selection
	m_pLine->getScreenOffsets(this, xoff, yoff);
	m_pG->clearArea(xoff, yoff, m_iWidth, m_pLine->getHeight());
}

void fp_TabRun::_drawArrow(UT_uint32 iLeft,UT_uint32 iTop,UT_uint32 iWidth, UT_uint32 iHeight)
{
    if (!(m_pG->queryProperties(GR_Graphics::DGP_SCREEN))){
        return;
    }

    #define NPOINTS 4

    UT_Point points[NPOINTS];

    UT_sint32 cur_linewidth = 1 + (UT_MAX(10,m_iAscent) - 10) / 8;
    UT_uint32 iyAxis = iTop + m_pLine->getAscent() * 2 / 3;
    UT_uint32 iMaxWidth = iWidth / 10 * 6;
    UT_uint32 ixGap = (iWidth - iMaxWidth) / 2;

    points[0].x = iLeft + ixGap + iMaxWidth - cur_linewidth * 4;
    points[0].y = iyAxis - cur_linewidth * 2;

    points[1].x = iLeft + iWidth - ixGap;
    points[1].y = iyAxis;

    points[2].x = points[0].x;
    points[2].y = iyAxis + cur_linewidth * 2;

    points[3].x = points[0].x;
    points[3].y = points[0].y;

    m_pG->polygon(m_colorFG,points,NPOINTS);
    m_pG->fillRect(m_colorFG,iLeft + ixGap,iyAxis - cur_linewidth / 2,iMaxWidth - cur_linewidth * 4,cur_linewidth);
}

void fp_TabRun::_draw(dg_DrawArgs* pDA)
{
	UT_ASSERT(pDA->pG == m_pG);

	UT_RGBColor clrSelBackground(192, 192, 192);
	UT_RGBColor clrNormalBackground(m_colorBG.m_red,m_colorBG.m_grn,m_colorBG.m_blu);

	UT_sint32 iFillHeight = m_pLine->getHeight();
	UT_sint32 iFillTop = pDA->yoff - m_pLine->getAscent();
		
	FV_View* pView = m_pBL->getDocLayout()->getView();
	UT_uint32 iSelAnchor = pView->getSelectionAnchor();
	UT_uint32 iPoint = pView->getPoint();

	UT_uint32 iSel1 = UT_MIN(iSelAnchor, iPoint);
	UT_uint32 iSel2 = UT_MAX(iSelAnchor, iPoint);
	
	UT_ASSERT(iSel1 <= iSel2);
	
#ifdef BIDI_ENABLED
	UT_uint32 iRunBase = m_pBL->getPosition() + getOffsetFirstVis(); //m_iOffsetFirst;
#else
	UT_uint32 iRunBase = m_pBL->getPosition() + m_iOffsetFirst;
#endif

	if (
		pView->getFocus()!=AV_FOCUS_NONE &&
		(iSel1 <= iRunBase)
		&& (iSel2 > iRunBase)
		)
	{
		m_pG->fillRect(clrSelBackground, pDA->xoff, iFillTop, m_iWidth, iFillHeight);
        if(pView->getShowPara()){
            _drawArrow(pDA->xoff, iFillTop, m_iWidth, iFillHeight);
        }
	}
	else
	{
		m_pG->fillRect(clrNormalBackground, pDA->xoff, iFillTop, m_iWidth, iFillHeight);
        if(pView->getShowPara()){
            _drawArrow(pDA->xoff, iFillTop, m_iWidth, iFillHeight);
        }
	}
}



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fp_ForcedLineBreakRun::fp_ForcedLineBreakRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_Run(pBL, pG, iOffsetFirst, iLen, FPRUN_FORCEDLINEBREAK)
{
        lookupProperties();
}

void fp_ForcedLineBreakRun::lookupProperties(void)
{
        m_pBL->getField(m_iOffsetFirst,m_pField);

}

bool fp_ForcedLineBreakRun::canContainPoint(void) const
{
	return false;
}

bool fp_ForcedLineBreakRun::canBreakAfter(void) const
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	return false;
}

bool fp_ForcedLineBreakRun::canBreakBefore(void) const
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	return false;
}

bool fp_ForcedLineBreakRun::letPointPass(void) const
{
	return false;
}

bool	fp_ForcedLineBreakRun::findMaxLeftFitSplitPointInLayoutUnits(UT_sint32 /* iMaxLeftWidth */, fp_RunSplitInfo& /* si */, bool /* bForce */)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	return false;
}

void fp_ForcedLineBreakRun::mapXYToPosition(UT_sint32 /* x */, UT_sint32 /*y*/, PT_DocPosition& pos, bool& bBOL, bool& bEOL)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	pos = m_pBL->getPosition() + m_iOffsetFirst;
	bBOL = false;
	bEOL = false;
}

void fp_ForcedLineBreakRun::findPointCoords(UT_uint32 iOffset, UT_sint32& x, UT_sint32& y, UT_sint32& x2, UT_sint32& y2, UT_sint32& height, bool& bDirection)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

void fp_ForcedLineBreakRun::_clearScreen(bool /* bFullLineHeightRect */)
{
	UT_ASSERT(!m_bDirty);
	UT_ASSERT(m_pG->queryProperties(GR_Graphics::DGP_SCREEN));
}

void fp_ForcedLineBreakRun::_draw(dg_DrawArgs* pDA)
{
	UT_ASSERT(pDA->pG == m_pG);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fp_FieldStartRun::fp_FieldStartRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_Run(pBL, pG, iOffsetFirst, iLen, FPRUN_FIELDSTARTRUN)
{
        lookupProperties();
}

void fp_FieldStartRun::lookupProperties(void)
{
        m_pBL->getField(m_iOffsetFirst,m_pField);
	m_iWidth = 0;
}

bool fp_FieldStartRun::canContainPoint(void) const
{
	return false;
}

bool fp_FieldStartRun::canBreakAfter(void) const
{
	return true;
}

bool fp_FieldStartRun::canBreakBefore(void) const
{
	return true;
}

bool fp_FieldStartRun::letPointPass(void) const
{
	return true;
}

bool	fp_FieldStartRun::findMaxLeftFitSplitPointInLayoutUnits(UT_sint32 /* iMaxLeftWidth */, fp_RunSplitInfo& /* si */, bool /* bForce */)
{
	return false;
}

void fp_FieldStartRun::mapXYToPosition(UT_sint32 /* x */, UT_sint32 /*y*/, PT_DocPosition& pos, bool& bBOL, bool& bEOL)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	pos = m_pBL->getPosition() + m_iOffsetFirst;
	bBOL = false;
	bEOL = false;
}


void fp_FieldStartRun::findPointCoords(UT_uint32 iOffset, UT_sint32& x, UT_sint32& y, UT_sint32& x2, UT_sint32& y2, UT_sint32& height, bool& bDirection)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

void fp_FieldStartRun::_clearScreen(bool /* bFullLineHeightRect */)
{
	UT_ASSERT(!m_bDirty);
	UT_ASSERT(m_pG->queryProperties(GR_Graphics::DGP_SCREEN));
}

void fp_FieldStartRun::_draw(dg_DrawArgs* pDA)
{
	UT_ASSERT(pDA->pG == m_pG);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fp_FieldEndRun::fp_FieldEndRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_Run(pBL, pG, iOffsetFirst, iLen, FPRUN_FIELDENDRUN)
{
        lookupProperties();
}

void fp_FieldEndRun::lookupProperties(void)
{
	m_pBL->getField(m_iOffsetFirst,m_pField);
	m_iWidth = 0;
}

bool fp_FieldEndRun::canContainPoint(void) const
{
	return false;
}

bool fp_FieldEndRun::canBreakAfter(void) const
{
	return true;
}

bool fp_FieldEndRun::canBreakBefore(void) const
{
	return true;
}

bool fp_FieldEndRun::letPointPass(void) const
{
	return true;
}

bool	fp_FieldEndRun::findMaxLeftFitSplitPointInLayoutUnits(UT_sint32 /* iMaxLeftWidth */, fp_RunSplitInfo& /* si */, bool /* bForce */)
{
	return false;
}

void fp_FieldEndRun::mapXYToPosition(UT_sint32 /* x */, UT_sint32 /*y*/, PT_DocPosition& pos, bool& bBOL, bool& bEOL)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	pos = m_pBL->getPosition() + m_iOffsetFirst;
	bBOL = false;
	bEOL = false;
}

void fp_FieldEndRun::findPointCoords(UT_uint32 iOffset, UT_sint32& x, UT_sint32& y, UT_sint32& x2, UT_sint32& y2, UT_sint32& height, bool& bDirection)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

void fp_FieldEndRun::_clearScreen(bool /* bFullLineHeightRect */)
{
	UT_ASSERT(!m_bDirty);
	UT_ASSERT(m_pG->queryProperties(GR_Graphics::DGP_SCREEN));
}

void fp_FieldEndRun::_draw(dg_DrawArgs* pDA)
{
	UT_ASSERT(pDA->pG == m_pG);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fp_ImageRun::fp_ImageRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen, GR_Image* pImage) : fp_Run(pBL, pG, iOffsetFirst, iLen, FPRUN_IMAGE)
{
#if 0	// put this back later
	UT_ASSERT(pImage);
#endif
	
	m_pImage = pImage;
	
	lookupProperties();
}

fp_ImageRun::~fp_ImageRun()
{
	if (m_pImage)
	{
		delete m_pImage;
	}
}

void fp_ImageRun::lookupProperties(void)
{
        m_pBL->getField(m_iOffsetFirst,m_pField);
	if (m_pImage)
	{
		m_iWidth = m_pImage->getDisplayWidth();
		m_iHeight = m_pImage->getDisplayHeight();
		m_iWidthLayoutUnits = m_pImage->getLayoutWidth();
		m_iHeightLayoutUnits = m_pImage->getLayoutHeight();
	}
	else
	{
		// If we have no image, we simply insert a square "slug"
			
		m_iWidth = m_pG->convertDimension("0.5in");
		m_iHeight = m_pG->convertDimension("0.5in");
		m_iWidthLayoutUnits = UT_convertToLayoutUnits("0.5in");
		m_iHeightLayoutUnits = UT_convertToLayoutUnits("0.5in");
	}
		
	UT_ASSERT(m_iWidth > 0);
	UT_ASSERT(m_iHeight > 0);

	m_iAscent = m_iHeight;
	m_iDescent = 0;
	m_iAscentLayoutUnits = m_iHeightLayoutUnits;
	m_iDescentLayoutUnits = 0;
}

bool fp_ImageRun::canBreakAfter(void) const
{
	return true;
}

bool fp_ImageRun::canBreakBefore(void) const
{
	return true;
}

bool fp_ImageRun::letPointPass(void) const
{
	return false;
}

bool	fp_ImageRun::findMaxLeftFitSplitPointInLayoutUnits(UT_sint32 /* iMaxLeftWidth */, fp_RunSplitInfo& /* si */, bool /* bForce */)
{
	return false;
}

void fp_ImageRun::mapXYToPosition(UT_sint32 x, UT_sint32 /*y*/, PT_DocPosition& pos, bool& bBOL, bool& bEOL)
{
	if (x > m_iWidth)
		pos = m_pBL->getPosition() + m_iOffsetFirst + m_iLen;
	else
		pos = m_pBL->getPosition() + m_iOffsetFirst;

	bBOL = false;
	bEOL = false;
}

void fp_ImageRun::findPointCoords(UT_uint32 iOffset, UT_sint32& x, UT_sint32& y, UT_sint32& x2, UT_sint32& y2, UT_sint32& height, bool& bDirection)
{
	UT_sint32 xoff;
	UT_sint32 yoff;

	UT_ASSERT(m_pLine);
	
	m_pLine->getOffsets(this, xoff, yoff);
	if (iOffset == (m_iOffsetFirst + m_iLen))
	{
		x = xoff + m_iWidth;
#ifdef BIDI_ENABLED
		x2 = x;
#endif
	}
	else
	{
		x = xoff;
	}
	y = yoff;
	height = m_iHeight;
#ifdef BIDI_ENABLED
	y2 = y;
	bDirection = getVisDirection();
#endif
}

void fp_ImageRun::_clearScreen(bool /* bFullLineHeightRect */)
{
	UT_ASSERT(!m_bDirty);
	
	UT_ASSERT(m_pG->queryProperties(GR_Graphics::DGP_SCREEN));

	UT_sint32 xoff = 0, yoff = 0;
	
	// need to clear full height of line, in case we had a selection
	m_pLine->getScreenOffsets(this, xoff, yoff);
	UT_sint32 iLineHeight = m_pLine->getHeight();
	m_pG->clearArea(xoff, yoff, m_iWidth, iLineHeight);
}

void fp_ImageRun::_draw(dg_DrawArgs* pDA)
{
	UT_ASSERT(pDA->pG == m_pG);

	UT_sint32 xoff = 0, yoff = 0;
	m_pLine->getScreenOffsets(this, xoff, yoff);

	yoff += m_pLine->getAscent() - m_iAscent;
	
	// clip drawing to the page
	UT_Rect pClipRect;
	pClipRect.top = yoff;
	pClipRect.left = xoff;
	pClipRect.height = m_pLine->getContainer()->getHeight();
	pClipRect.width = m_pLine->getContainer()->getWidth();
	m_pG->setClipRect(&pClipRect);

	if (m_pImage)
	{
		// draw the image (always)
		m_pG->drawImage(m_pImage, xoff, yoff);

		// if we're the selection, draw a pretty box
		if (m_pG->queryProperties(GR_Graphics::DGP_SCREEN))
		{
			UT_uint32 iRunBase = m_pBL->getPosition() + m_iOffsetFirst;

			FV_View* pView = m_pBL->getDocLayout()->getView();
			UT_uint32 iSelAnchor = pView->getSelectionAnchor();
			UT_uint32 iPoint = pView->getPoint();

			UT_uint32 iSel1 = UT_MIN(iSelAnchor, iPoint);
			UT_uint32 iSel2 = UT_MAX(iSelAnchor, iPoint);
	
			UT_ASSERT(iSel1 <= iSel2);
	
			if (
				pView->getFocus()!=AV_FOCUS_NONE &&
				(iSel1 <= iRunBase)
				&& (iSel2 > iRunBase)
				)
			{
				UT_Point pts[5];

				UT_uint32 top = yoff;
				UT_uint32 left = xoff;
				UT_uint32 right = xoff + m_iWidth - 1;
				UT_uint32 bottom = yoff + m_iHeight - 1;
									
				pts[0].x = left; 	pts[0].y = top;
				pts[1].x = right;	pts[1].y = top;
				pts[2].x = right;	pts[2].y = bottom;
				pts[3].x = left; 	pts[3].y = bottom;
				pts[4].x = left;	pts[4].y = top;
				
				// TODO : remove the hard-coded (but pretty) blue color

				UT_RGBColor clr(0, 0, 255);
				m_pG->setColor(clr);
				m_pG->polyLine(pts, 5);
				
			}
		}
		
	}
	else
	{
		UT_RGBColor clr(0, 0, 255);
		m_pG->fillRect(clr, xoff, yoff, m_iWidth, m_iHeight);
	}

	// unf*ck clipping rect
	m_pG->setClipRect(NULL);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define  _FIELD(type,desc,tag)  /*nothing*/
#define  _FIELDTYPE(type,desc)  {FPFIELDTYPE_##type, NULL, desc},

fp_FieldTypeData fp_FieldTypes[] = {

#include "fp_Fields.h"

{FPFIELDTYPE_END, NULL, 0} };

#undef  _FIELD
#undef  _FIELDTYPE

// The way to turn macro argument into string constant
#define xstr2(x) #x
#define xstr(x) xstr2(x)
#define _FIELD(type,desc,tag)  {FPFIELDTYPE_##type, FPFIELD_##tag, NULL, xstr(tag), desc},
#define _FIELDTYPE(type,desc)  /*nothing*/

fp_FieldData fp_FieldFmts[] = {

#include "fp_Fields.h"

{FPFIELDTYPE_END, FPFIELD_end, NULL, NULL, 0} };

#undef  xstr2
#undef  xstr
#undef  _FIELD
#undef  _FIELDTYPE

fp_FieldRun::fp_FieldRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen)
:	fp_Run(pBL, pG, iOffsetFirst, iLen, FPRUN_FIELD),
	m_pFont(0),
	m_pFontLayout(0),
	m_iFieldType(FPFIELD_start)
{
	m_sFieldValue[0] = 0;
}

bool fp_FieldRun::_setValue(UT_UCSChar *p_new_value)
{

	if (0 != UT_UCS_strcmp(p_new_value, m_sFieldValue))
	{
		clearScreen();

		UT_UCS_strcpy(m_sFieldValue, p_new_value);

		{
			unsigned short aCharWidths[FPFIELD_MAX_LENGTH];
			lookupProperties();
			m_pG->setFont(m_pFont);
			UT_sint32 iNewWidth = m_pG->measureString(m_sFieldValue, 0, UT_UCS_strlen(m_sFieldValue), aCharWidths);
			if (iNewWidth != m_iWidth)
			{
				clearScreen();
				m_iWidth = iNewWidth;

				m_pG->setFont(m_pFontLayout);
				m_iWidthLayoutUnits = m_pG->measureString(m_sFieldValue, 0, UT_UCS_strlen(m_sFieldValue), aCharWidths);

				return true;
			}

			return false;
		}
	}

	return false;
}

void fp_FieldRun::lookupProperties(void)
{
	const PP_AttrProp * pSpanAP = NULL;
	const PP_AttrProp * pBlockAP = NULL;
	const PP_AttrProp * pSectionAP = NULL; // TODO do we care about section-level inheritance?
	
	m_pBL->getSpanAttrProp(m_iOffsetFirst,false,&pSpanAP);
	//	UT_DEBUGMSG(("SEVIOR: Doing Lookupprops for block %x run %x  offset =%d \n ",m_pBL,this,m_iOffsetFirst));
	UT_ASSERT(pSpanAP);
	PD_Document * pDoc = m_pBL->getDocument();
	m_pBL->getAttrProp(&pBlockAP);
	m_pBL->getField(m_iOffsetFirst+1,m_pField); // Next Pos?
	if(m_pField != NULL)
	{
		m_pField->setBlock(m_pBL);
	}
	// look for fonts in this DocLayout's font cache
	FL_DocLayout * pLayout = m_pBL->getDocLayout();
	if(m_iFieldType == FPFIELD_list_label)
	{
		m_pFont = pLayout->findFont(pSpanAP,pBlockAP,pSectionAP, FL_DocLayout::FIND_FONT_AT_SCREEN_RESOLUTION,true);
		m_pFontLayout = pLayout->findFont(pSpanAP,pBlockAP,pSectionAP, FL_DocLayout::FIND_FONT_AT_LAYOUT_RESOLUTION,true);
	}
	else
	{
		m_pFont = pLayout->findFont(pSpanAP,pBlockAP,pSectionAP, FL_DocLayout::FIND_FONT_AT_SCREEN_RESOLUTION);
		m_pFontLayout = pLayout->findFont(pSpanAP,pBlockAP,pSectionAP, FL_DocLayout::FIND_FONT_AT_LAYOUT_RESOLUTION);
	}

	UT_parseColor(PP_evalProperty("color",pSpanAP,pBlockAP,pSectionAP, m_pBL->getDocument(), true), m_colorFG);
	UT_parseColor(PP_evalProperty("field-color",pSpanAP,pBlockAP,pSectionAP, m_pBL->getDocument(), true), m_colorBG);
	UT_parseColor(PP_evalProperty("bgcolor",pSpanAP,pBlockAP,pSectionAP, m_pBL->getDocument(), true), m_colorBG);

	m_pG->setFont(m_pFont);

	m_iAscent = m_pG->getFontAscent();	
	m_iDescent = m_pG->getFontDescent();
	m_iHeight = m_pG->getFontHeight();

	m_pG->setFont(m_pFontLayout);
	m_iAscentLayoutUnits = m_pG->getFontAscent();	
	m_iDescentLayoutUnits = m_pG->getFontDescent();
	m_iHeightLayoutUnits = m_pG->getFontHeight();

	m_pG->setFont(m_pFont);

	const XML_Char* pszType = NULL;

	const XML_Char * pszPosition = PP_evalProperty("text-position",pSpanAP,pBlockAP,pSectionAP, pDoc, true);

	if (0 == UT_strcmp(pszPosition, "superscript"))
	{
		m_fPosition = TEXT_POSITION_SUPERSCRIPT;
	}
	else if (0 == UT_strcmp(pszPosition, "subscript"))
	{
		m_fPosition = TEXT_POSITION_SUBSCRIPT;
	}
	else
	{
		 m_fPosition = TEXT_POSITION_NORMAL;
	}

	pSpanAP->getAttribute((XML_Char*)"type", pszType);
	UT_ASSERT(pszType);

#ifdef BIDI_ENABLED
	//#TF need to retrieve the direction of this run
	//PD_Document * pDoc = m_pBL->getDocument();
	const XML_Char * pszDirection = PP_evalProperty("dir",pSpanAP,pBlockAP,pSectionAP, pDoc, true);
	if(!UT_stricmp(pszDirection, "rtl"))
	{
		m_iDirection = 1;
	}
	else
	{
		m_iDirection = 0;
	}
#endif

	int i;
	for( i = 0; fp_FieldFmts[i].m_Tag != NULL; i++ )
	{
		if (0 == UT_stricmp(pszType, fp_FieldFmts[i].m_Tag))
		{
			m_iFieldType = fp_FieldFmts[i].m_Num;
			break;
		}
	}
	if( fp_FieldFmts[i].m_Tag == NULL )
	{
        // probably new type of field
        //		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}
}


fp_FieldsEnum fp_FieldRun::getFieldType(void) const
{
	return m_iFieldType;
}

bool fp_FieldRun::canBreakAfter(void) const
{
	return true;
}

bool fp_FieldRun::canBreakBefore(void) const
{
	return true;
}

bool fp_FieldRun::letPointPass(void) const
{
	return true;
}

bool        fp_FieldRun::findMaxLeftFitSplitPointInLayoutUnits(UT_sint32 /*
iMaxLeftWidth */, fp_RunSplitInfo& /* si */, bool /* bForce */)
{
	return false;
}

bool fp_FieldRun::isSuperscript(void) const
{
	return (m_fPosition == TEXT_POSITION_SUPERSCRIPT);
}

bool fp_FieldRun::isSubscript(void) const
{
	return (m_fPosition == TEXT_POSITION_SUBSCRIPT);
}

void fp_FieldRun::mapXYToPosition(UT_sint32 x, UT_sint32 /*y*/, PT_DocPosition& pos, bool& bBOL, bool& bEOL)
{
	// If X is left of the middle, return offset to the left,
	// otherwise the offset to the right.
	if (x < (getWidth() / 2))
		pos = m_pBL->getPosition() + m_iOffsetFirst;
	else
		pos = m_pBL->getPosition() + m_iOffsetFirst + m_iLen;
		
	bBOL = false;
	bEOL = false;
}

void fp_FieldRun::findPointCoords(UT_uint32 iOffset, UT_sint32& x,
                                  UT_sint32& y, UT_sint32& x2,
                                  UT_sint32& y2, UT_sint32& height, bool& bDirection)
{
	UT_sint32 xoff;
	UT_sint32 yoff;

	UT_ASSERT(m_pLine);

	lookupProperties();
	
	m_pLine->getOffsets(this, xoff, yoff);

	if (iOffset == (m_iOffsetFirst + m_iLen))
	{
		xoff += m_iWidth;
	}
	if (m_fPosition == TEXT_POSITION_SUPERSCRIPT)
	{
		yoff -= m_iAscent * 1/2;
	}
	else if (m_fPosition == TEXT_POSITION_SUBSCRIPT)
	{
		yoff += m_iDescent /* * 3/2 */;
	}
 	x = xoff;
	y = yoff;
	height = m_iHeight;
#ifdef BIDI_ENABLED
	x2 = x;
	y2 = y;
	bDirection = getVisDirection();
#endif
}

bool fp_FieldRun::calculateValue(void)
{
  //
  // Code for the Piece Table Fields Calculation
  // Get size of the field from the following runs
  //
  //      return m_pField->update();
  //        UT_ASSERT(m_pField);

  /*        UT_sint32 count = 0;
        fp_Run* pNext = getNext();
	while(pNext != NULL && pNext->getField() != NULL )
	{
	        if(m_pField == NULL)
		{
		        m_pField = pNext->getField();
		}
	        pNext = getNext();
		count++;
	}
	if( count == 0)
	{
	        m_iWidth = 0;
		m_iHeight = 0;
		m_iWidthLayoutUnits = 0;
		m_iHeightLayoutUnits = 0;
	}
	else
        {
	        pNext = getPrev();
	        m_iWidth = pNext->getWidth();
		m_iHeight = pNext->getHeight();
		m_iWidthLayoutUnits = pNext->getWidthInLayoutUnits();
		m_iHeightLayoutUnits = pNext->getHeightInLayoutUnits();
	}
	if(m_pField != NULL)
	m_pField->update();
*/
	return true;
}

void fp_FieldRun::_clearScreen(bool /* bFullLineHeightRect */)
{
	UT_ASSERT(!m_bDirty);

	UT_ASSERT(m_pG->queryProperties(GR_Graphics::DGP_SCREEN));
	UT_sint32 xoff = 0, yoff = 0;
	
	// need to clear full height of line, in case we had a selection
	m_pLine->getScreenOffsets(this, xoff, yoff);
	UT_sint32 iLineHeight = m_pLine->getHeight();
	m_pG->clearArea(xoff, yoff-1, m_iWidth, iLineHeight);
}

void fp_FieldRun::_defaultDraw(dg_DrawArgs* pDA)
{
	UT_ASSERT(pDA->pG == m_pG);

	lookupProperties();
	UT_sint32 iYdraw =  pDA->yoff - getAscent()-1;
	
	if (m_fPosition == TEXT_POSITION_SUPERSCRIPT)
	{
	        iYdraw -= getAscent() * 1/2;
	}
        else if (m_fPosition == TEXT_POSITION_SUBSCRIPT)
	{
	        iYdraw +=  getDescent(); // * 3/2
	}

	//if (m_pG->queryProperties(GR_Graphics::DGP_SCREEN))
	{
		UT_uint32 iRunBase = m_pBL->getPosition() + m_iOffsetFirst;

		/*
		  TODO we might want special colors for fields.  We might also
		  want the colors to be calculated on the fly instead of
		  hard-coded.  See comment above in fp_TextRun::_draw*.
		  For now, we are hard-coding a couple of shades of grey,
		  with fields always being drawn a little darker than the
		  surrounding text.
		*/
		UT_RGBColor clrSelBackground(112, 112, 112);
		//		UT_RGBColor clrNormalBackground(220, 220, 220);
		
		UT_sint32 iFillTop = iYdraw;
		UT_sint32 iFillHeight = getAscent() + getDescent();
		
		FV_View* pView = m_pBL->getDocLayout()->getView();
		UT_uint32 iSelAnchor = pView->getSelectionAnchor();
		UT_uint32 iPoint = pView->getPoint();

		UT_uint32 iSel1 = UT_MIN(iSelAnchor, iPoint);
		UT_uint32 iSel2 = UT_MAX(iSelAnchor, iPoint);
	
		UT_ASSERT(iSel1 <= iSel2);
	
		if (
			pView->getFocus()!=AV_FOCUS_NONE &&
			(iSel1 <= iRunBase)
			&& (iSel2 > iRunBase)
			)
		{
			m_pG->fillRect(clrSelBackground, pDA->xoff, iFillTop, m_iWidth, iFillHeight);
		}
		else
		{
			m_pG->fillRect(m_colorBG, pDA->xoff, iFillTop, m_iWidth, iFillHeight);
		}
	}
	m_pG->setFont(m_pFont);
	m_pG->setColor(m_colorFG);
	
	m_pG->drawChars(m_sFieldValue, 0, UT_UCS_strlen(m_sFieldValue), pDA->xoff,iYdraw);
}

// BEGIN DOM work on some new fields

static FV_View *
_getViewFromBlk(fl_BlockLayout* pBlock)
{
	FV_View *pView    = pBlock->getView();	
	return pView;
}

static XAP_App *
_getAppFromBlk(fl_BlockLayout * pBlock)
{
	XAP_App *pApp    = pBlock->getView()->getApp();	
	return pApp;
}

fp_FieldCharCountRun::fp_FieldCharCountRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldCharCountRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];
	szFieldValue[0] = 0;

	FV_View *pView = _getViewFromBlk(m_pBL);
	FV_DocCount cnt = pView->countWords();

	sprintf(szFieldValue, "%d", cnt.ch_sp);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldNonBlankCharCountRun::fp_FieldNonBlankCharCountRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldNonBlankCharCountRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];
	szFieldValue[0] = 0;

	FV_View *pView = _getViewFromBlk(m_pBL);
	FV_DocCount cnt = pView->countWords();

	sprintf(szFieldValue, "%d", cnt.ch_no);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldLineCountRun::fp_FieldLineCountRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldLineCountRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];
	szFieldValue[0] = 0;

	FV_View *pView = _getViewFromBlk(m_pBL);
	FV_DocCount cnt = pView->countWords();

	sprintf(szFieldValue, "%d", cnt.line);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldParaCountRun::fp_FieldParaCountRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldParaCountRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];
	szFieldValue[0] = 0;

	FV_View *pView = _getViewFromBlk(m_pBL);
	FV_DocCount cnt = pView->countWords();

	sprintf(szFieldValue, "%d", cnt.para);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldWordCountRun::fp_FieldWordCountRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldWordCountRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];
	szFieldValue[0] = 0;

	FV_View *pView = _getViewFromBlk(m_pBL);
	FV_DocCount cnt = pView->countWords();

	sprintf(szFieldValue, "%d", cnt.word);

	UT_DEBUGMSG(("DOM: szFieldValue is %s\n", szFieldValue));

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

// mm/dd/yy notation
fp_FieldMMDDYYRun::fp_FieldMMDDYYRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldMMDDYYRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%m/%d/%y", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

// dd/mm/yy time
fp_FieldDDMMYYRun::fp_FieldDDMMYYRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldDDMMYYRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%d/%m/%y", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

// Month Day, Year
fp_FieldMonthDayYearRun::fp_FieldMonthDayYearRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldMonthDayYearRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%B %d, %Y", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldMthDayYearRun::fp_FieldMthDayYearRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldMthDayYearRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%b %d, %Y", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldDefaultDateRun::fp_FieldDefaultDateRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldDefaultDateRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%c", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldDefaultDateNoTimeRun::fp_FieldDefaultDateNoTimeRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldDefaultDateNoTimeRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%x", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldWkdayRun::fp_FieldWkdayRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldWkdayRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%A", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldDOYRun::fp_FieldDOYRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldDOYRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%j", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldMilTimeRun::fp_FieldMilTimeRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldMilTimeRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%H:%M:%S", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldAMPMRun::fp_FieldAMPMRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldAMPMRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%p", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldTimeEpochRun::fp_FieldTimeEpochRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldTimeEpochRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	sprintf(szFieldValue, "%ld", (long)tim);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldTimeZoneRun::fp_FieldTimeZoneRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldTimeZoneRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%Z", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldBuildIdRun::fp_FieldBuildIdRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldBuildIdRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	XAP_App * pApp = _getAppFromBlk(m_pBL);
	
	UT_UCS_strcpy_char(sz_ucs_FieldValue, pApp->s_szBuild_ID);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldBuildVersionRun::fp_FieldBuildVersionRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldBuildVersionRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	XAP_App * pApp = _getAppFromBlk(m_pBL);
	
	UT_UCS_strcpy_char(sz_ucs_FieldValue, pApp->s_szBuild_Version);

	return _setValue(sz_ucs_FieldValue);
}

// END OF DOM NEW FIELDS

fp_FieldTimeRun::fp_FieldTimeRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldTimeRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%I:%M:%S %p", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldDateRun::fp_FieldDateRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldDateRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	time_t	tim = time(NULL);
	struct tm *pTime = localtime(&tim);

	strftime(szFieldValue, FPFIELD_MAX_LENGTH, "%A %B %d, %Y", pTime);

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldPageNumberRun::fp_FieldPageNumberRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldPageNumberRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];

	if (m_pLine && m_pLine->getContainer() && m_pLine->getContainer()->getPage())
	{
		fp_Page* pPage = m_pLine->getContainer()->getPage();
		FL_DocLayout* pDL = pPage->getDocLayout();

		UT_sint32 iPageNum = 0;
		UT_uint32 iNumPages = pDL->countPages();
		for (UT_uint32 i=0; i<iNumPages; i++)
		{
			fp_Page* pPg = pDL->getNthPage(i);

			if (pPg == pPage)
			{
				iPageNum = i + 1;
				break;
			}
		}

		UT_ASSERT(iPageNum > 0);

		sprintf(szFieldValue, "%d", iPageNum);
	}
	else
	{
		strcpy(szFieldValue, "?");
	}

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}

fp_FieldPageCountRun::fp_FieldPageCountRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_FieldRun(pBL, pG, iOffsetFirst, iLen)
{
}

bool fp_FieldPageCountRun::calculateValue(void)
{
	UT_UCSChar sz_ucs_FieldValue[FPFIELD_MAX_LENGTH + 1];
	sz_ucs_FieldValue[0] = 0;
	
	char szFieldValue[FPFIELD_MAX_LENGTH + 1];
	
	if (m_pLine && m_pLine->getContainer() && m_pLine->getContainer()->getPage())
	{

		fp_Page* pPage = m_pLine->getContainer()->getPage();
		FL_DocLayout* pDL = pPage->getDocLayout();

		sprintf(szFieldValue, "%d", pDL->countPages());
	}
	else
	{
		strcpy(szFieldValue, "?");
	}

	UT_UCS_strcpy_char(sz_ucs_FieldValue, szFieldValue);

	return _setValue(sz_ucs_FieldValue);
}



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fp_ForcedColumnBreakRun::fp_ForcedColumnBreakRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_Run(pBL, pG, iOffsetFirst, iLen, FPRUN_FORCEDCOLUMNBREAK)
{
}

void fp_ForcedColumnBreakRun::lookupProperties(void)
{

        m_pBL->getField(m_iOffsetFirst,m_pField);

}

bool fp_ForcedColumnBreakRun::canContainPoint(void) const
{
	return false;
}

bool fp_ForcedColumnBreakRun::canBreakAfter(void) const
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	return false;
}

bool fp_ForcedColumnBreakRun::canBreakBefore(void) const
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	return false;
}

bool fp_ForcedColumnBreakRun::letPointPass(void) const
{
	return false;
}

bool	fp_ForcedColumnBreakRun::findMaxLeftFitSplitPointInLayoutUnits(UT_sint32 /* iMaxLeftWidth */, fp_RunSplitInfo& /* si */, bool /* bForce */)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	return false;
}

void fp_ForcedColumnBreakRun::mapXYToPosition(UT_sint32 /* x */, UT_sint32 /*y*/, PT_DocPosition& pos, bool& bBOL, bool& bEOL)
{
	pos = m_pBL->getPosition() + m_iOffsetFirst;
	bBOL = false;
	bEOL = false;
}

void fp_ForcedColumnBreakRun::findPointCoords(UT_uint32 iOffset, UT_sint32& x, UT_sint32& y, UT_sint32& x2, UT_sint32& y2, UT_sint32& height, bool& bDirection)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

void fp_ForcedColumnBreakRun::_clearScreen(bool /* bFullLineHeightRect */)
{
	UT_ASSERT(!m_bDirty);
	UT_ASSERT(m_pG->queryProperties(GR_Graphics::DGP_SCREEN));

    UT_sint32 xoff = 0, yoff = 0;
    m_pLine->getScreenOffsets(this, xoff, yoff);
    UT_sint32 iWidth  = m_pLine->getMaxWidth() - m_pLine->calculateWidthOfLine();
    m_pG->clearArea(xoff,yoff,iWidth,m_pLine->getHeight());
}

void fp_ForcedColumnBreakRun::_draw(dg_DrawArgs* pDA)
{
	UT_ASSERT(pDA->pG == m_pG);

    if (!(m_pG->queryProperties(GR_Graphics::DGP_SCREEN))){
        return;
    }

    FV_View* pView = m_pBL->getDocLayout()->getView();
    UT_ASSERT(pView);
    if(!pView->getShowPara()){
        return;
    }

    UT_sint32 iLineWidth  = m_pLine->getMaxWidth() - m_pLine->calculateWidthOfLine();

    UT_UCSChar *pColumnBreak;
    UT_UCS_cloneString_char(&pColumnBreak,"Column Break");
    _drawTextLine(pDA->xoff,pDA->yoff - m_pLine->getAscent() / 3,iLineWidth,m_pLine->getHeight(),pColumnBreak);
    FREEP(pColumnBreak);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fp_ForcedPageBreakRun::fp_ForcedPageBreakRun(fl_BlockLayout* pBL, GR_Graphics* pG, UT_uint32 iOffsetFirst, UT_uint32 iLen) : fp_Run(pBL, pG, iOffsetFirst, iLen, FPRUN_FORCEDPAGEBREAK)
{
	lookupProperties();
}

void fp_ForcedPageBreakRun::lookupProperties(void)
{
        m_pBL->getField(m_iOffsetFirst,m_pField);
}

bool fp_ForcedPageBreakRun::canContainPoint(void) const
{
	return false;
}

bool fp_ForcedPageBreakRun::canBreakAfter(void) const
{
	return false;
}

bool fp_ForcedPageBreakRun::canBreakBefore(void) const
{
	return false;
}

bool fp_ForcedPageBreakRun::letPointPass(void) const
{
	return false;
}

bool	fp_ForcedPageBreakRun::findMaxLeftFitSplitPointInLayoutUnits(UT_sint32 /* iMaxLeftWidth */, fp_RunSplitInfo& /* si */, bool /* bForce */)
{
	return false;
}

void fp_ForcedPageBreakRun::mapXYToPosition(UT_sint32 /* x */, UT_sint32 /*y*/, PT_DocPosition& pos, bool& bBOL, bool& bEOL)
{
	pos = m_pBL->getPosition() + m_iOffsetFirst;
	bBOL = false;
	bEOL = false;
}

void fp_ForcedPageBreakRun::findPointCoords(UT_uint32 iOffset, UT_sint32& x, UT_sint32& y, UT_sint32& x2, UT_sint32& y2, UT_sint32& height, bool& bDirection)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

void fp_ForcedPageBreakRun::_clearScreen(bool /* bFullLineHeightRect */)
{
	UT_ASSERT(!m_bDirty);
	UT_ASSERT(m_pG->queryProperties(GR_Graphics::DGP_SCREEN));

    UT_sint32 xoff = 0, yoff = 0;
    m_pLine->getScreenOffsets(this, xoff, yoff);
    UT_sint32 iWidth  = m_pLine->getMaxWidth() - m_pLine->calculateWidthOfLine();
    m_pG->clearArea(xoff,yoff,iWidth,m_pLine->getHeight());
}

void fp_ForcedPageBreakRun::_draw(dg_DrawArgs* pDA)
{
	UT_ASSERT(pDA->pG == m_pG);

    if (!(m_pG->queryProperties(GR_Graphics::DGP_SCREEN))){
        return;
    }

    FV_View* pView = m_pBL->getDocLayout()->getView();
    UT_ASSERT(pView);
    if(!pView->getShowPara()){
        return;
    }

    UT_sint32 iLineWidth  = m_pLine->getMaxWidth() - m_pLine->calculateWidthOfLine();

    UT_UCSChar *pPageBreak;
    UT_UCS_cloneString_char(&pPageBreak,"Page Break");
    _drawTextLine(pDA->xoff,pDA->yoff - m_pLine->getAscent() / 3,iLineWidth,m_pLine->getHeight(),pPageBreak);
    FREEP(pPageBreak);
}


#ifdef BIDI_ENABLED
////////////////////////////////////////////////////////////////////////
// the following functions are used in the BiDi implementation #TF

// translates logical position in a run into visual position
// (will also translate correctly visual -> logical)
UT_uint32 fp_Run::getVisPosition(UT_uint32 iLogPos)
{
    if(getVisDirection() == 1) //rtl needs translation
    {
        return (m_iLen - iLogPos - 1);
    }
    else return (iLogPos);
}

//translates a visual position in a span of length iLen to logical pos
//or vice versa
UT_uint32 fp_Run::getVisPosition(UT_uint32 iLogPos, UT_uint32 iLen)
{
    if(getVisDirection() == 1) //rtl needs translation
    {
        return (iLen - iLogPos - 1);
    }
    else return (iLogPos);
}

//returns the logical offset of the first visual character
UT_uint32 fp_Run::getOffsetFirstVis()
{
    if(getVisDirection() == 1) //rtl, requires translation
    {
        return(m_iOffsetFirst + m_iLen - 1);
    }
    else return (m_iOffsetFirst);
}

//translates visual offset to logical one, can be also used for translation
//in the other direction
UT_uint32 fp_Run::getOffsetLog(UT_uint32 iVisOff)
{
    if(getVisDirection() == 1) //rtl needs translation
    {
        return(m_iOffsetFirst + m_iLen - iVisOff + m_iOffsetFirst - 1);
    }
    else return (iVisOff);
}

void fp_Run::setDirection(UT_sint32 iDir)
{
     //this function should be called with -1,0,1; if it is called
     //with -2, which is used in the derived classes that can handle
     //Unicode as an indication that direction should be worked out
     //from Unicode, we will treat this a whitespace

     UT_sint32 iDirection = iDir != -2 ? iDir : -1;
     if(m_iDirection != iDirection)
     {
         m_iDirection = iDirection;
         clearScreen();
		/*
			if this run belongs to a line we have to notify the line that
			that it now contains a run of this direction, if it does not belong
			to a line this will be taken care of by the fp_Line:: member function
			used to add the run to the line (generally, we set it here if this
			is a run that is being typed in and it gets set in the member
			functions when the run is loaded from a document on the disk.)
		*/
	
		if(m_pLine)
			m_pLine->addDirectionUsed(m_iDirection);

   }
}

// returns the direction with which the run is displayed, converting the direction of white
// space to either ltr or rtl depending on context
UT_sint32 fp_Run::getVisDirection()
{
    if(m_iDirection == 2) //direction not set, yet, use dominant direction;
    {
        return (m_pBL->getDominantDirection());
    }

    if(m_iDirection != -1)
    {
        //UT_DEBUGMSG(("getVisDirection (non-white): direction = %d, visDirection = %d\n", m_iDirection, m_iDirection));
        return(m_iDirection);
    }

    UT_sint32 iBlDirection = (UT_sint32) m_pBL->getDominantDirection();
    fp_Run * r = getPrev();
    UT_sint32 prevDir;

    // find first non-white run before this one
    while (r)
    {
        prevDir = r->getDirection();
        r = r->getPrev();

        if(prevDir == iBlDirection)
        {
            //UT_DEBUGMSG(("getVisDirection (prev): direction = %d, visDirection = %d\n", m_iDirection, iBlDirection));
            return(iBlDirection);
        }
        if(prevDir != -1) break; //run of foreign direction
    }

    // if there are no non-white runs, return direction of the block
    if(r == 0)
    {
        //UT_DEBUGMSG(("getVisDirection (non-white start): direction = %d, visDirection = %d\n", m_iDirection, iBlDirection));
        return(iBlDirection);
    }

    // now we know that the preceding run is of foreing direction, in which case
    // the direction of white space depends on the first non-white run that follows it
    UT_sint32 nextDir;
    r = getNext();

    while(r)
    {
        nextDir = r->getDirection();
        r = r->getNext();

        if(nextDir != -1)
        {
            //UT_DEBUGMSG(("getVisDirection (next): direction = %d, visDirection = %d\n", m_iDirection, nextDir));
            return(nextDir);
        }
    }

    // if get this far, this white space run is the last one on the line
    // and will have the direciton of the previous run
    //UT_DEBUGMSG(("getVisDirection (last on line): direction = %d, visDirection = %d\n", m_iDirection, prevDir));
    return(prevDir);
}

/*
bool fp_Run::setUnicodeDirection()
{
	//bool bSetProperty = (m_iDirection == 2) ? true : false; //do we need to call View->setCharFormat ?
	
	m_iDirection = -1; //all runs that rely on this implementatio will be treated as whitespace
	
	//if this run did not have its direction set so far we have to call View->setCharFormat
	//if(bSetProperty)
	//    setDirectionProperty(-1);
	
	return (false);
}
*/
void fp_Run::setDirectionProperty(UT_sint32 dir)
{
	//FV_View * pView = getBlock()->getDocLayout()->getView();
	//UT_ASSERT(pView);
	const XML_Char * prop[] = {NULL, NULL, 0};
	const XML_Char direction[] = "dir";
	const XML_Char rtl[] = "rtl";
	const XML_Char ltr[] = "ltr";
	const XML_Char neutral[] = "ntrl";
	
	prop[0] = (XML_Char*) &direction;
	
	switch(dir)
	{
	   case 0:  prop[1] = (XML_Char*) &ltr;     break;
	   case 1:  prop[1] = (XML_Char*) &rtl;     break;
	   case -1: prop[1] = (XML_Char*) &neutral; break;
	   default: UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	};
	
	//pView->setCharFormat(prop);
	UT_uint32 offset = m_pBL->getPosition() + m_iOffsetFirst;
	getBlock()->getDocument()->changeSpanFmt(PTC_AddFmt,offset,offset + m_iLen,NULL,prop);
	//UT_DEBUGMSG(("Run::setDirectionProperty: offset=%d, len=%d\n", offset,m_iLen));
}

#endif //BIDI_ENABLED





