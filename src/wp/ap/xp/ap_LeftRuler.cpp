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

#include <string.h>
#include <stdio.h>

#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ap_LeftRuler.h"
#include "xav_View.h"
#include "gr_Graphics.h"
#include "ap_FrameData.h"
#include "ap_TopRuler.h"
#include "xap_Frame.h"
#include "ap_Ruler.h"
#include "ap_Prefs.h"
#include "fv_View.h"

#define MyMax(a,b)		(((a)>(b)) ? (a) : (b))
#define DELETEP(p)		do { if (p) delete p; p = NULL; } while (0)

/*****************************************************************/

AP_LeftRuler::AP_LeftRuler(XAP_Frame * pFrame)
{
	m_pFrame = pFrame;
	m_pView = NULL;
	m_pScrollObj = NULL;
	m_pG = NULL;
	m_iHeight = 0;
	m_iWidth = 0;
	m_yScrollOffset = 0;
	m_yScrollLimit = 0;
	
	const XML_Char * szRulerUnits;
	if (pFrame->getApp()->getPrefsValue(AP_PREF_KEY_RulerUnits,&szRulerUnits))
		m_dim = UT_determineDimension(szRulerUnits);
	else
		m_dim = DIM_IN;

	// i wanted these to be "static const x = 32;" in the
	// class declaration, but MSVC5 can't handle it....
	// (GCC can :-)
	
	s_iFixedWidth = 32;

	// Initialize colors.  Derived classes can change these, but they should
	// probably be set as soon as possible (in the constructor if possible),
	// so the drawing is consistent.

 	UT_setColor(m_clrWhite, 255, 255, 255);
	UT_setColor(m_clrBlack, 0, 0, 0);
	UT_setColor(m_clrDarkGray, 127, 127, 127);
	UT_setColor(m_clrLiteGray, 192, 192, 192);

	UT_setColor(m_clrForeground, 0, 0, 0);
	UT_setColor(m_clrBackground, 192, 192, 192);

	UT_setColor(m_clrMarginArea, 127, 127, 127);
	UT_setColor(m_clrDocumentArea, 255, 255, 255);

	memset(&m_lfi,0,sizeof(m_lfi));
}

AP_LeftRuler::~AP_LeftRuler(void)
{
	DELETEP(m_pScrollObj);
}

/*****************************************************************/

void AP_LeftRuler::setView(AV_View* pView, UT_uint32 iZoom)
{
	this->setView(pView);

	UT_ASSERT(m_pG);
	m_pG->setZoomPercentage(iZoom);
}

void AP_LeftRuler::setView(AV_View * pView)
{
	if (m_pView && (m_pView != pView))
	{
		// view is changing.  since this LeftRuler class is bound to
		// the actual on-screen widgets, we reuse it as documents
		// change in the frame rather than recreating it with each
		// view (as we do with some of the other objects).

		DELETEP(m_pScrollObj);
	}
	
	m_pView = pView;
	
	// create an AV_ScrollObj to receive send*ScrollEvents()
	
	m_pScrollObj = new AV_ScrollObj(this,_scrollFuncX,_scrollFuncY);
	UT_ASSERT(m_pScrollObj);
	m_pView->addScrollListener(m_pScrollObj);

	// Register the LeftRuler as a ViewListeners on the View.
	// This lets us receive notify events as the user interacts
	// with the document (cmdCharMotion, etc).  This will let
	// us update the display as we move from block to block and
	// from column to column.

	AV_ListenerId lidLeftRuler;
	m_pView->addListener(static_cast<AV_Listener *>(this),&lidLeftRuler);

	return;
}

void AP_LeftRuler::setHeight(UT_uint32 iHeight)
{
	m_iHeight = iHeight;
}

UT_uint32 AP_LeftRuler::getHeight(void) const
{
	return m_iHeight;
}

void AP_LeftRuler::setWidth(UT_uint32 iWidth)
{
	m_iWidth = iWidth;
	AP_FrameData * pFrameData = (AP_FrameData *)m_pFrame->getFrameData();
	if (pFrameData && pFrameData->m_pTopRuler)
		pFrameData->m_pTopRuler->setOffsetLeftRuler(m_iWidth);
}

UT_uint32 AP_LeftRuler::getWidth(void) const
{
	return s_iFixedWidth;
}

/*****************************************************************/

UT_Bool AP_LeftRuler::notify(AV_View * pView, const AV_ChangeMask mask)
{
	// Handle AV_Listener events on the view.

	UT_ASSERT(pView==m_pView);
	//UT_DEBUGMSG(("AP_LeftRuler::notify [view %p][mask %p]\n",pView,mask));

	// If the caret has moved to a different page or any of the properties
	// on the page (such as the margins) have changed, we force a redraw.

	if (mask & (AV_CHG_MOTION | AV_CHG_FMTSECTION))
	{
		AP_LeftRulerInfo lfi;
		(static_cast<FV_View *>(m_pView))->getLeftRulerInfo(&lfi);

		if (   (lfi.m_yPageStart != m_lfi.m_yPageStart)
			|| (lfi.m_yPageSize != m_lfi.m_yPageSize)
			|| (lfi.m_yTopMargin != m_lfi.m_yTopMargin)
			|| (lfi.m_yBottomMargin != m_lfi.m_yBottomMargin))
		{
			draw(NULL,lfi);
		}
	}
	
	return UT_TRUE;
}

/*****************************************************************/

void AP_LeftRuler::_scrollFuncX(void * /* pData */, UT_sint32 /* xoff */, UT_sint32 /* xlimit */)
{
	// static callback referenced by an AV_ScrollObj() for the ruler
	// we don't care about horizontal scrolling.
	return;
}

void AP_LeftRuler::_scrollFuncY(void * pData, UT_sint32 yoff, UT_sint32 ylimit)
{
	// static callback referenced by an AV_ScrollObj() for the ruler
	UT_ASSERT(pData);

	AP_LeftRuler * pLeftRuler = (AP_LeftRuler *)(pData);

	// let non-static member function do all the work.
	
	pLeftRuler->scrollRuler(yoff,ylimit);
	return;
}

/*****************************************************************/

void AP_LeftRuler::scrollRuler(UT_sint32 yoff, UT_sint32 ylimit)
{
	//UT_DEBUGMSG(("LeftRuler:: scroll [y %d]\n",yoff));

	if (ylimit > 0)
		m_yScrollLimit = ylimit;

	if (yoff > m_yScrollLimit)
		yoff = m_yScrollLimit;
	
	UT_sint32 dy = yoff - m_yScrollOffset;
	if (!dy)
		return;

	UT_Rect rClip;
	rClip.left = 0;
	rClip.width = s_iFixedWidth;

	if (dy > 0)
	{
		rClip.top = m_iHeight - dy;
		rClip.height = dy;
	}
	else
	{
		rClip.top = 0;
		rClip.height = -dy;
	}
	
	m_pG->scroll(0,dy);
	m_yScrollOffset = yoff;
	draw(&rClip);
}

/*****************************************************************/

void AP_LeftRuler::draw(const UT_Rect * pClipRect)
{
	AP_LeftRulerInfo lfi;
	(static_cast<FV_View *>(m_pView))->getLeftRulerInfo(&lfi);
	
	draw(pClipRect,lfi);
}

void AP_LeftRuler::draw(const UT_Rect * pClipRect, AP_LeftRulerInfo & lfi)
{
	if (!m_pG)
		return;
	
	if (pClipRect)
		m_pG->setClipRect(pClipRect);

	// draw the background
	
	m_pG->fillRect(m_clrBackground,0,0,m_iWidth,m_iHeight);

	// draw a dark-gray and white bar lined up with the paper

	UT_uint32 xLeft = s_iFixedWidth/4;
	UT_uint32 xBar  = s_iFixedWidth/2;

	UT_uint32 docWithinMarginHeight = lfi.m_yPageSize - lfi.m_yTopMargin - lfi.m_yBottomMargin;

	UT_sint32 yOrigin = lfi.m_yPageStart;
	UT_sint32 yScrolledOrigin = yOrigin - m_yScrollOffset;
	UT_sint32 y,h;

	if ((yScrolledOrigin + lfi.m_yTopMargin) > 0)
	{
		// top margin of paper is on-screen.  draw dark-gray bar.
		// we need to clip it ourselves -- since the expose/paint
		// clip rects don't know anything about this distinction.

		y = yScrolledOrigin;
		h = lfi.m_yTopMargin - 1;
		m_pG->fillRect(m_clrMarginArea,xLeft,y,xBar,h);
	}

	yScrolledOrigin += lfi.m_yTopMargin + 1;
	if ((yScrolledOrigin + docWithinMarginHeight) > 0)
	{
		// area within the page margins is on-screen.
		// draw a main white bar over the area.

		y = yScrolledOrigin;
		h = docWithinMarginHeight - 1;
		m_pG->fillRect(m_clrDocumentArea,xLeft,y,xBar,h);
	}

	yScrolledOrigin += docWithinMarginHeight + 1;
	if ((yScrolledOrigin + lfi.m_yBottomMargin) > 0)
	{
		// bottom margin of paper is on-screen.
		// draw another dark-gray bar, like we
		// did at the top.

		y = yScrolledOrigin;
		h = lfi.m_yBottomMargin - 1;
		m_pG->fillRect(m_clrMarginArea,xLeft,y,xBar,h);
	}

	// draw 3D frame around top margin + document + bottom margin rects

	// now draw tick marks on the bar, using the selected system of units.

	ap_RulerTicks tick(m_pG,m_dim);

	UT_uint32 k, iFontHeight;

	m_pG->setColor(m_clrForeground);

	GR_Font * pFont = m_pG->getGUIFont();
	if (pFont)
	{
		m_pG->setFont(pFont);
		iFontHeight = m_pG->getFontHeight();
	}

	// first draw the top margin
	for (k=1; (k*tick.tickUnit/tick.tickUnitScale < lfi.m_yTopMargin); k++)
	{
		y = yOrigin + lfi.m_yTopMargin - k*tick.tickUnit/tick.tickUnitScale - m_yScrollOffset;
		if (y >= 0)
		{
			if (k % tick.tickLabel)
			{
				// draw the ticks
				UT_uint32 w = ((k % tick.tickLong) ? 2 : 6);
				UT_uint32 x = xLeft + (xBar-w)/2;
				m_pG->drawLine(x,y,x+w,y);
			}
			else if (pFont)
			{
				// draw the number
				UT_uint32 n = k / tick.tickLabel * tick.tickScale;

				char buf[6];
				UT_UCSChar span[6];
				UT_uint16 charWidths[6];
				UT_ASSERT(n < 10000);

				sprintf(buf, "%ld", n);
				UT_UCS_strcpy_char(span, buf);
				UT_uint32 len = strlen(buf);

				UT_uint32 w = m_pG->measureString(span, 0, len, charWidths);
				UT_uint32 x = xLeft + (xBar-w)/2;

				m_pG->drawChars(span, 0, len, x, y - iFontHeight/2);
			}
		}
	}
	
	// then draw everything below
	for (k=1; (k*tick.tickUnit/tick.tickUnitScale < (lfi.m_yPageSize - lfi.m_yTopMargin)); k++)
	{
		y = yOrigin + lfi.m_yTopMargin + k*tick.tickUnit/tick.tickUnitScale - m_yScrollOffset;
		if (y >= 0)
		{
			if (k % tick.tickLabel)
			{
				// draw the ticks
				UT_uint32 w = ((k % tick.tickLong) ? 2 : 6);
				UT_uint32 x = xLeft + (xBar-w)/2;
				m_pG->drawLine(x,y,x+w,y);
			}
			else if (pFont)
			{
				// draw the number
				UT_uint32 n = k / tick.tickLabel * tick.tickScale;

				char buf[6];
				UT_UCSChar span[6];
				UT_uint16 charWidths[6];
				UT_ASSERT(n < 10000);

				sprintf(buf, "%ld", n);
				UT_UCS_strcpy_char(span, buf);
				UT_uint32 len = strlen(buf);

				UT_uint32 w = m_pG->measureString(span, 0, len, charWidths);
				UT_uint32 x = xLeft + (xBar-w)/2;

				m_pG->drawChars(span, 0, len, x, y - iFontHeight/2);
			}
		}
	}
	
	if (pClipRect)
		m_pG->setClipRect(NULL);

	m_lfi = lfi;
}

