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

#include <windows.h>

#include "ut_types.h"
#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "xap_ViewListener.h"
#include "ap_FrameData.h"
#include "xap_Win32Frame.h"
#include "ev_Win32Toolbar.h"
#include "xav_View.h"
#include "xad_Document.h"
#include "fv_View.h"
#include "fl_DocLayout.h"
#include "pd_Document.h"
#include "gr_Win32Graphics.h"
#include "xap_Scrollbar_ViewListener.h"

#define DELETEP(p)		do { if (p) delete p; } while (0)
#define REPLACEP(p,q)	do { if (p) delete p; p = q; } while (0)
#define ENSUREP(p)		do { UT_ASSERT(p); if (!p) goto Cleanup; } while (0)

/*****************************************************************/
	
UT_Bool AP_Win32Frame::_showDocument(void)
{
	if (!m_pDoc)
	{
		UT_DEBUGMSG(("Can't show a non-existent document\n"));
		return UT_FALSE;
	}

	if (!m_pData)
	{
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return UT_FALSE;
	}

	Win32Graphics * pG = NULL;
	FL_DocLayout * pDocLayout = NULL;
	AV_View * pView = NULL;
	AV_ScrollObj * pScrollObj = NULL;
	ap_ViewListener * pViewListener = NULL;
	AD_Document * pOldDoc = NULL;
	ap_Scrollbar_ViewListener * pScrollbarViewListener = NULL;
	
	AV_ListenerId lidScrollbarViewListener;
	
	UT_uint32 iWindowHeight, iHeight;
	UT_uint32 nrToolbars, k;
	HWND hwnd = m_hwndDocument;

	// TODO fix prefix on class Win32Graphics

	pG = new Win32Graphics(GetDC(hwnd), hwnd);
	ENSUREP(pG);
	pDocLayout = new FL_DocLayout(static_cast<PD_Document *>(m_pDoc), pG);
	ENSUREP(pDocLayout);
  
	pDocLayout->formatAll();
	
	pView = new FV_View(this,pDocLayout);
	ENSUREP(pView);
	pScrollObj = new AV_ScrollObj(this,_scrollFunc);
	ENSUREP(pScrollObj);
	pViewListener = new ap_ViewListener(this);
	ENSUREP(pViewListener);

	AV_ListenerId lid;
	if (!pView->addListener(static_cast<AV_Listener *>(pViewListener),&lid))
		goto Cleanup;

	nrToolbars = m_vecToolbarLayoutNames.getItemCount();
	for (k=0; k < nrToolbars; k++)
	{
		// TODO Toolbars are a frame-level item, but a view-listener is
		// TODO a view-level item.  I've bound the toolbar-view-listeners
		// TODO to the current view within this frame and have code in the
		// TODO toolbar to allow the view-listener to be rebound to a different
		// TODO view.  in the future, when we have support for multiple views
		// TODO in the frame (think splitter windows), we will need to have
		// TODO a loop like this to help change the focus when the current
		// TODO view changes.
		
		EV_Win32Toolbar * pWin32Toolbar = (EV_Win32Toolbar *)m_vecWin32Toolbars.getNthItem(k);
		pWin32Toolbar->bindListenerToView(pView);
	}

	// add a Scrollbar-View-Listener to help up keep the scrollbar up-to-date.
	//
	// TODO we ***really*** need to re-do the whole scrollbar thing.
	// TODO we have an addScrollListener() using an m_pScrollObj
	// TODO and a View-Listener, and a bunch of other widget stuff.
	// TODO and its very confusing.

	pScrollbarViewListener = new ap_Scrollbar_ViewListener(this,pView);
	ENSUREP(pScrollbarViewListener);
	if (!pView->addListener(static_cast<AV_Listener *>(pScrollbarViewListener),
							&lidScrollbarViewListener))
		goto Cleanup;

	/****************************************************************
	*****************************************************************
	** If we reach this point, everything for the new document has
	** been created.  We can now safely replace the various fields
	** within the structure.  Nothing below this point should fail.
	*****************************************************************
	****************************************************************/
	
	// switch to new view, cleaning up previous settings
	if (m_pData->m_pDocLayout)
	{
		pOldDoc = m_pData->m_pDocLayout->getDocument();
	}

	REPLACEP(m_pData->m_pG, pG);
	REPLACEP(m_pData->m_pDocLayout, pDocLayout);
	DELETEP(pOldDoc);
	REPLACEP(m_pView, pView);
	REPLACEP(m_pScrollObj, pScrollObj);
	REPLACEP(m_pViewListener, pViewListener);
	m_lid = lid;
	REPLACEP(m_pScrollbarViewListener,pScrollbarViewListener);
	m_lidScrollbarViewListener = lidScrollbarViewListener;
	
	RECT r;
	GetClientRect(hwnd, &r);
	iWindowHeight = r.bottom - r.top;

	iHeight = m_pData->m_pDocLayout->getHeight();

	m_pView->addScrollListener(m_pScrollObj);
//	m_pMouse->reset();
	
	// enough HACKs to get a clean redisplay?
	m_pView->setWindowSize(r.right - r.left, iWindowHeight);
	InvalidateRect(hwnd, NULL, true);

	setXScrollRange();
	setYScrollRange();
	
	updateTitle();

	return UT_TRUE;

Cleanup:
	// clean up anything we created here
	DELETEP(pG);
	DELETEP(pDocLayout);
	DELETEP(pView);
	DELETEP(pViewListener);
	DELETEP(pScrollObj);
	DELETEP(pScrollbarViewListener);
	
	// change back to prior document
	DELETEP(m_pDoc);
	m_pDoc = m_pData->m_pDocLayout->getDocument();

	return UT_FALSE;
}

void AP_Win32Frame::setXScrollRange(void)
{
	UT_uint32 iWindowWidth, iWidth;

	RECT r;
	GetClientRect(m_hwndDocument, &r);
	iWindowWidth = r.right - r.left;

	iWidth = m_pData->m_pDocLayout->getWidth();

	SCROLLINFO si;
	memset(&si, 0, sizeof(si));

	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
	si.nMin = 0;
	si.nMax = iWidth;
	si.nPos = ((m_pView) ? m_pView->getXScrollOffset() : 0);
	si.nPage = iWindowWidth;
	SetScrollInfo(m_hwndHScroll, SB_CTL, &si, TRUE);
}

void AP_Win32Frame::setYScrollRange(void)
{
	UT_uint32 iWindowHeight, iHeight;

	RECT r;
	GetClientRect(m_hwndDocument, &r);
	iWindowHeight = r.bottom - r.top;

	iHeight = m_pData->m_pDocLayout->getHeight();

	SCROLLINFO si;
	memset(&si, 0, sizeof(si));

	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
	si.nMin = 0;
	si.nMax = iHeight;
	si.nPos = ((m_pView) ? m_pView->getYScrollOffset() : 0);
	si.nPage = iWindowHeight;
	SetScrollInfo(m_hwndVScroll, SB_CTL, &si, TRUE);
}
