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
	m_iPageViewTopMargin = 0;
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
	setOffsetPageViewTopMargin(pView->getPageViewTopMargin());
	
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

void AP_LeftRuler::setOffsetPageViewTopMargin(UT_uint32 iPageViewTopMargin)
{
	// This gives us the amount of gray-space that the DocumentWindow
	// draws to the above the paper in the "Page View".  We set the
	// origin of our ruler at this offset.  For "Normal View" this
	// should be zero.

	m_iPageViewTopMargin = iPageViewTopMargin;
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
	AP_FrameData * pFrameData = m_pFrame->getFrameData();
	if (pFrameData && pFrameData->m_pTopRuler)
		pFrameData->m_pTopRuler->setOffsetLeftRuler(m_iWidth);
}

UT_uint32 AP_LeftRuler::getWidth(void) const
{
	return s_iFixedWidth;
}

/*****************************************************************/

UT_Bool AP_LeftRuler::notify(AV_View * pView, const AV_ChangeMask /* mask */)
{
	// Handle AV_Listener events on the view.

	UT_ASSERT(pView==m_pView);
	//UT_DEBUGMSG(("AP_LeftRuler::notify [view %p][mask %p]\n",pView,mask));
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

/*
  This function is taken from AP_TopRuler, and if _draw() gets similar treatment
  (broken into functions) plug this in with the appropriate h (height) and watch
  it go.

  One could also stick this in AP_Ruler:: and cast down from AP_TopRulerInfo or
  AP_LeftRulerInfo for specifics.
*/

#if 0
void AP_LeftRuler::_draw3DFrame(const UT_Rect * pClipRect, AP_TopRulerInfo * pInfo,
							   UT_sint32 x, UT_sint32 h)
{
	// Draw ruler bar (white or dark-gray) over [x,x+w)
	// where x is in page-relative coordinates.  we need
	// to compensate for fixed portion, the page-view margin,
	// and the scroll.
	
	UT_uint32 yTop = s_iFixedHeight/4;
	UT_uint32 yBar = s_iFixedHeight/2;
	UT_sint32 xFixed = (UT_sint32)MyMax(m_iLeftRulerWidth,s_iFixedWidth);

	// convert page-relative coordinates into absolute coordinates.
	
	UT_sint32 xAbsLeft = xFixed + pInfo->m_xPageViewMargin + x - m_xScrollOffset;
	UT_sint32 xAbsRight = xAbsLeft + w;

	// we need to do our own clipping for the fixed area
	
	if (xAbsLeft < xFixed)			// need to shorten what we draw
		xAbsLeft = xFixed;

	// draw whatever is left

	UT_RGBColor clrWhite(255,255,255);
	UT_RGBColor clrDarkGray(127,127,127);
	UT_RGBColor clrBlack(0,0,0);

	if (xAbsRight > xAbsLeft)
	{
		// draw black lines above and left of area
		m_pG->setColor(clrBlack);
		m_pG->drawLine(xAbsLeft - 1, yTop - 1, xAbsLeft + w + 2, yTop - 1);		// above
		m_pG->drawLine(xAbsLeft - 1, yTop - 1, xAbsLeft - 1, yTop + yBar + 1);	// left

		// draw dark gray lines above and left of black lines
		m_pG->setColor(clrDarkGray);
		m_pG->drawLine(xAbsLeft - 2, yTop - 2, xAbsLeft + w + 3, yTop - 2);		// above
		m_pG->drawLine(xAbsLeft - 2, yTop - 2, xAbsLeft - 2, yTop + yBar + 2);	// left

		// draw light gray lines down and right of area, seperated by 1 pixel all the way
		m_pG->setColor(clrWhite);
		m_pG->drawLine(xAbsLeft - 1, yTop + yBar + 1, xAbsLeft + w + 2, yTop + yBar + 1);	// below
		m_pG->drawLine(xAbsLeft + w + 2, yTop + yBar + 1, xAbsLeft + w + 2, yTop - 2);		// right
	}
	
	return;
}
#endif

void AP_LeftRuler::draw(const UT_Rect * pClipRect)
{
	if (!m_pG)
		return;
	
	if (pClipRect)
		m_pG->setClipRect(pClipRect);

	UT_RGBColor clrDarkGray(127,127,127);
	UT_RGBColor clrLiteGray(192,192,192);
	UT_RGBColor clrBlack(0,0,0);
	UT_RGBColor clrWhite(255,255,255);

	// draw the background
	
	m_pG->fillRect(clrLiteGray,0,0,m_iWidth,m_iHeight);

	// draw a dark-gray and white bar lined up with the paper

	UT_uint32 xLeft = s_iFixedWidth/4;
	UT_uint32 xBar  = s_iFixedWidth/2;

	// TODO get these from the document at the current cursor position.
	
	UT_uint32 pageHeight = m_pG->convertDimension("11.0in");
	UT_uint32 docTopMarginHeight = m_pG->convertDimension("1.0in");
	UT_uint32 docBottomMarginHeight = m_pG->convertDimension("1.0in");
	UT_uint32 docWithinMarginHeight = pageHeight - docTopMarginHeight - docBottomMarginHeight;

	// top edge of the paper depends upon the height of the
	// gray-space we draw for PageView (when in PageView mode).

	UT_sint32 yOrigin = (UT_sint32)m_iPageViewTopMargin;
	UT_sint32 yScrolledOrigin = yOrigin - m_yScrollOffset;
	UT_sint32 y,h;

	if ((yScrolledOrigin + docTopMarginHeight) > 0)
	{
		// top margin of paper is on-screen.  draw dark-gray bar.
		// we need to clip it ourselves -- since the expose/paint
		// clip rects don't know anything about this distinction.

		y = yScrolledOrigin;
		h = docTopMarginHeight - 1;
		m_pG->fillRect(clrDarkGray,xLeft,y,xBar,h);
	}

	yScrolledOrigin += docTopMarginHeight + 1;
	if ((yScrolledOrigin + docWithinMarginHeight) > 0)
	{
		// area within the page margins is on-screen.
		// draw a main white bar over the area.

		y = yScrolledOrigin;
		h = docWithinMarginHeight - 1;
		m_pG->fillRect(clrWhite,xLeft,y,xBar,h);
	}

	yScrolledOrigin += docWithinMarginHeight + 1;
	if ((yScrolledOrigin + docBottomMarginHeight) > 0)
	{
		// bottom margin of paper is on-screen.
		// draw another dark-gray bar, like we
		// did at the top.

		y = yScrolledOrigin;
		h = docBottomMarginHeight - 1;
		m_pG->fillRect(clrDarkGray,xLeft,y,xBar,h);
	}

	// draw 3D frame around top margin + document + bottom margin rects

// not quite implemented for left ruler	
//	_draw3DFrame(pClipRect,pInfo,1,sum + pInfo->u.c.m_xaRightMargin - 2);
	
	// now draw tick marks on the bar, using the selected system of units.

	ap_RulerTicks tick(m_pG,m_dim);

	UT_uint32 k, iFontHeight;

	m_pG->setColor(clrBlack);

	GR_Font * pFont = m_pG->getGUIFont();
	if (pFont)
	{
		m_pG->setFont(pFont);
		iFontHeight = m_pG->getFontHeight();
	}

	// first draw the top margin
	for (k=1; (k*tick.tickUnit/tick.tickUnitScale < docTopMarginHeight); k++)
	{
		y = yOrigin + docTopMarginHeight - k*tick.tickUnit/tick.tickUnitScale - m_yScrollOffset;
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
	for (k=1; (k*tick.tickUnit/tick.tickUnitScale < (pageHeight - docTopMarginHeight)); k++)
	{
		y = yOrigin + docTopMarginHeight + k*tick.tickUnit/tick.tickUnitScale - m_yScrollOffset;
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
}

