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
#include "ap_Win32Frame.h"
#include "xap_Win32App.h"
#include "ev_Win32Mouse.h"
#include "ap_Win32TopRuler.h"
#include "ap_Win32LeftRuler.h"

/*****************************************************************/

#define GWL(hwnd)		(AP_Win32Frame*)GetWindowLong((hwnd), GWL_USERDATA)
#define SWL(hwnd, f)	(AP_Win32Frame*)SetWindowLong((hwnd), GWL_USERDATA,(LONG)(f))

#define DELETEP(p)		do { if (p) delete p; } while (0)
#define REPLACEP(p,q)	do { if (p) delete p; p = q; } while (0)
#define ENSUREP(p)		do { UT_ASSERT(p); if (!p) goto Cleanup; } while (0)

/*****************************************************************/

static char s_ContainerWndClassName[256];
static char s_DocumentWndClassName[256];
static char s_LeftRulerWndClassName[256];

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

	GR_Win32Graphics * pG = NULL;
	FL_DocLayout * pDocLayout = NULL;
	AV_View * pView = NULL;
	AV_ScrollObj * pScrollObj = NULL;
	ap_ViewListener * pViewListener = NULL;
	AD_Document * pOldDoc = NULL;
	ap_Scrollbar_ViewListener * pScrollbarViewListener = NULL;
	AV_ListenerId lid;
	AV_ListenerId lidScrollbarViewListener;
	
	UT_uint32 nrToolbars, k;
	HWND hwnd = m_hwndDocument;

	pG = new GR_Win32Graphics(GetDC(hwnd), hwnd);
	ENSUREP(pG);
	pDocLayout = new FL_DocLayout(static_cast<PD_Document *>(m_pDoc), pG);
	ENSUREP(pDocLayout);
  
//	pDocLayout->formatAll();
	
	pView = new FV_View(this,pDocLayout);
	ENSUREP(pView);

	// The "AV_ScrollObj pScrollObj" receives
	// send{Vertical,Horizontal}ScrollEvents
	// from both the scroll-related edit methods
	// and from the UI callbacks.
	// 
	// The "ap_ViewListener pViewListener" receives
	// change notifications as the document changes.
	// This ViewListener is responsible for keeping
	// the title-bar up to date (primarily title
	// changes, dirty indicator, and window number).
	//
	// The "ap_Scrollbar_ViewListener pScrollbarViewListener"
	// receives change notifications as the doucment changes.
	// This ViewListener is responsible for recalibrating the
	// scrollbars as pages are added/removed from the document.
	//
	// Each Toolbar will also get a ViewListener so that
	// it can update toggle buttons, and other state-indicating
	// controls on it.
	//
	// TODO we ***really*** need to re-do the whole scrollbar thing.
	// TODO we have an addScrollListener() using an m_pScrollObj
	// TODO and a View-Listener, and a bunch of other widget stuff.
	// TODO and its very confusing.
	
	pScrollObj = new AV_ScrollObj(this,_scrollFuncX,_scrollFuncY);
	ENSUREP(pScrollObj);
	pViewListener = new ap_ViewListener(this);
	ENSUREP(pViewListener);
	pScrollbarViewListener = new ap_Scrollbar_ViewListener(this,pView);
	ENSUREP(pScrollbarViewListener);

	if (!pView->addListener(static_cast<AV_Listener *>(pViewListener),&lid))
		goto Cleanup;
	if (!pView->addListener(static_cast<AV_Listener *>(pScrollbarViewListener),
							&lidScrollbarViewListener))
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
	
	m_pView->addScrollListener(m_pScrollObj);

	// Associate the new view with the existing TopRuler, LeftRuler.
	// Because of the binding to the actual on-screen widgets
	// we do not destroy and recreate the TopRuler,LeftRuler when we change
	// views, like we do for all the other objects.  We also do not
	// allocate the TopRuler, LeftRuler here; that is done as the frame is
	// created.
	m_pData->m_pTopRuler->setView(pView);
	m_pData->m_pLeftRuler->setView(pView);

	RECT r;
	GetClientRect(hwnd, &r);
	m_pView->setWindowSize(r.right - r.left, r.bottom - r.top);
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

	m_pView->sendHorizontalScrollEvent(si.nPos,si.nMax-si.nPage);
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

	m_pView->sendVerticalScrollEvent(si.nPos,si.nMax-si.nPage);
}

UT_Bool AP_Win32Frame::RegisterClass(AP_Win32App * app)
{
	// NB: can't access 'this' members from a static member function

	if (!XAP_Win32Frame::RegisterClass(app))
		return UT_FALSE;

	WNDCLASSEX  wndclass;
	ATOM a;

	// register class for the container window (this will contain the document
	// and the rulers and the scroll bars)
	
	sprintf(s_ContainerWndClassName, "%sContainer", app->getApplicationName());

	memset(&wndclass, 0, sizeof(wndclass));
	wndclass.cbSize        = sizeof(wndclass);
	wndclass.style         = CS_DBLCLKS | CS_OWNDC;
	wndclass.lpfnWndProc   = AP_Win32Frame::_ContainerWndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = app->getInstance();
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = s_ContainerWndClassName;
	wndclass.hIconSm       = NULL;

	a = RegisterClassEx(&wndclass);
	UT_ASSERT(a);
	
	// register class for the actual document window
	sprintf(s_DocumentWndClassName, "%sDocument", app->getApplicationName());

	memset(&wndclass, 0, sizeof(wndclass));
	wndclass.cbSize        = sizeof(wndclass);
	wndclass.style         = CS_DBLCLKS | CS_OWNDC;
	wndclass.lpfnWndProc   = AP_Win32Frame::_DocumentWndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = app->getInstance();
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = s_DocumentWndClassName;
	wndclass.hIconSm       = NULL;

	a = RegisterClassEx(&wndclass);
	UT_ASSERT(a);
	
	if (!AP_Win32TopRuler::RegisterClass(app))
		return UT_FALSE;
	if (!AP_Win32LeftRuler::RegisterClass(app))
		return UT_FALSE;

	return UT_TRUE;
}

AP_Win32Frame::AP_Win32Frame(AP_Win32App * app)
	: XAP_Win32Frame(app)
{
	m_hwndTopRuler = NULL;
	m_hwndLeftRuler = NULL;
	m_hwndDeadLowerRight = NULL;
	m_hwndVScroll = NULL;
	m_hwndHScroll = NULL;
	m_hwndDocument = NULL;
}

AP_Win32Frame::AP_Win32Frame(AP_Win32Frame * f)
	: XAP_Win32Frame(static_cast<XAP_Win32Frame *>(f))
{
	m_hwndTopRuler = NULL;
	m_hwndLeftRuler = NULL;
	m_hwndDeadLowerRight = NULL;
	m_hwndVScroll = NULL;
	m_hwndHScroll = NULL;
	m_hwndDocument = NULL;
}

AP_Win32Frame::~AP_Win32Frame(void)
{
	killFrameData();
}

UT_Bool AP_Win32Frame::initialize(void)
{
	if (!initFrameData())
		return UT_FALSE;

	if (!XAP_Win32Frame::initialize())
		return UT_FALSE;

	_createTopLevelWindow();
	return UT_TRUE;
}

XAP_Frame * AP_Win32Frame::cloneFrame(void)
{
	AP_Win32Frame * pClone = new AP_Win32Frame(this);
	ENSUREP(pClone);

	if (!pClone->initialize())
		goto Cleanup;

	if (!pClone->_showDocument())
		goto Cleanup;

	pClone->show();

	return pClone;

Cleanup:
	// clean up anything we created here
	if (pClone)
	{
		m_pWin32App->forgetFrame(pClone);
		delete pClone;
	}

	return NULL;
}

HWND AP_Win32Frame::_createDocumentWindow(HWND hwndParent,
										  UT_uint32 iLeft, UT_uint32 iTop,
										  UT_uint32 iWidth, UT_uint32 iHeight)
{
	// create the window(s) that the user will consider to be the
	// document window -- the thing between the bottom of the toolbars
	// and the top of the status bar.  return the handle to it.
		
	// create a child window for us -- this will be the 'container'.
	// the 'container' will in turn contain the document window, the
	// rulers, and the various scroll bars and other dead space.

	RECT r;
	int cxVScroll, cyHScroll;
	
	HWND hwndContainer = CreateWindowEx(WS_EX_CLIENTEDGE, s_ContainerWndClassName, NULL,
										WS_CHILD | WS_VISIBLE,
										iLeft, iTop, iWidth, iHeight,
										hwndParent, NULL, m_pWin32App->getInstance(), NULL);
	UT_ASSERT(hwndContainer);
	SWL(hwndContainer, this);
	
	// now create all the various windows inside the container window.

	GetClientRect(hwndContainer,&r);
	cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
	cyHScroll = GetSystemMetrics(SM_CYHSCROLL);

	m_hwndVScroll = CreateWindowEx(0,"ScrollBar",NULL,
								   WS_CHILD | WS_VISIBLE | SBS_VERT,
								   r.right-cxVScroll, 0, cxVScroll, r.bottom-cyHScroll,
								   hwndContainer, NULL, m_pWin32App->getInstance(), NULL);
	UT_ASSERT(m_hwndVScroll);
	SWL(m_hwndVScroll, this);

	m_hwndHScroll = CreateWindowEx(0,"ScrollBar",NULL,
								   WS_CHILD | WS_VISIBLE | SBS_HORZ,
								   0, r.bottom-cyHScroll, r.right-cxVScroll, cyHScroll,
								   hwndContainer, NULL, m_pWin32App->getInstance(), NULL);
	UT_ASSERT(m_hwndHScroll);
	SWL(m_hwndHScroll, this);

	m_hwndDeadLowerRight = CreateWindowEx(0,"ScrollBar",NULL,
										  WS_CHILD | WS_VISIBLE | WS_DISABLED | SBS_SIZEBOX,
										  r.right-cxVScroll, r.bottom-cyHScroll, cxVScroll, cyHScroll,
										  hwndContainer, NULL, m_pWin32App->getInstance(), NULL);
	UT_ASSERT(m_hwndHScroll);
	SWL(m_hwndHScroll, this);

	// create the top ruler
	
	AP_Win32TopRuler * pWin32TopRuler = new AP_Win32TopRuler(this);
	UT_ASSERT(pWin32TopRuler);
	m_hwndTopRuler = pWin32TopRuler->createWindow(hwndContainer,
												  0,0, (r.right - cxVScroll));
	m_pData->m_pTopRuler = pWin32TopRuler;
	UT_uint32 yTopRulerHeight = pWin32TopRuler->getHeight();

	// create the left ruler
	
	AP_Win32LeftRuler * pWin32LeftRuler = new AP_Win32LeftRuler(this);
	UT_ASSERT(pWin32LeftRuler);
	m_hwndLeftRuler = pWin32LeftRuler->createWindow(hwndContainer,0,yTopRulerHeight,
													r.bottom - yTopRulerHeight - cyHScroll);
	m_pData->m_pLeftRuler = pWin32LeftRuler;
	UT_uint32 xLeftRulerWidth = pWin32LeftRuler->getWidth();

	// get the width from the left ruler and stuff it into the top ruler.
	
	pWin32TopRuler->setOffsetLeftRuler(xLeftRulerWidth);

	// create a child window for us.
	m_hwndDocument = CreateWindowEx(0, s_DocumentWndClassName, NULL,
									WS_CHILD | WS_VISIBLE,
									xLeftRulerWidth, yTopRulerHeight,
									r.right - xLeftRulerWidth - cxVScroll,
									r.bottom - yTopRulerHeight - cyHScroll,
									hwndContainer, NULL, m_pWin32App->getInstance(), NULL);
	UT_ASSERT(m_hwndDocument);
	SWL(m_hwndDocument, this);

	return hwndContainer;
}

UT_Bool AP_Win32Frame::loadDocument(const char * szFilename)
{
	if (! _loadDocument(szFilename))
	{
		// we could not load the document.
		// we cannot complain to the user here, we don't know
		// if the app is fully up yet.  we force our caller
		// to deal with the problem.
		return UT_FALSE;
	}

	return _showDocument();
}
	
void AP_Win32Frame::_scrollFuncY(void* pData, UT_sint32 yoff, UT_sint32 /*ylimit*/)
{
	// this is a static callback function and doesn't have a 'this' pointer.

	// scroll event came in (probably from an EditMethod (like a PageDown
	// or insertData or something).  update the on-screen scrollbar and
	// then warp the document window contents.
	
	AP_Win32Frame * pWin32Frame = static_cast<AP_Win32Frame *>(pData);
	UT_ASSERT(pWin32Frame);

	SCROLLINFO si;
	memset(&si, 0, sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;

	HWND hwndV = pWin32Frame->m_hwndVScroll;
	GetScrollInfo(hwndV, SB_CTL, &si);

	si.nPos = yoff;
	SetScrollInfo(hwndV, SB_CTL, &si, TRUE);

	GetScrollInfo(hwndV, SB_CTL, &si);	// may have been clamped
	pWin32Frame->m_pView->setYScrollOffset(si.nPos);
}

void AP_Win32Frame::_scrollFuncX(void* pData, UT_sint32 xoff, UT_sint32 /*xlimit*/)
{
	// this is a static callback function and doesn't have a 'this' pointer.

	AP_Win32Frame * pWin32Frame = static_cast<AP_Win32Frame *>(pData);
	UT_ASSERT(pWin32Frame);

	SCROLLINFO si;
	memset(&si, 0, sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;

	HWND hwndH = pWin32Frame->m_hwndHScroll;
	GetScrollInfo(hwndH, SB_CTL, &si);

	si.nPos = xoff;
	SetScrollInfo(hwndH, SB_CTL, &si, TRUE);

	GetScrollInfo(hwndH, SB_CTL, &si);	// may have been clamped
	pWin32Frame->m_pView->setXScrollOffset(si.nPos);
}

/*****************************************************************/

#define SCROLL_LINE_SIZE 20

LRESULT CALLBACK AP_Win32Frame::_ContainerWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	AP_Win32Frame * f = GWL(hwnd);
	AV_View * pView = NULL;

	if (f)
		pView = f->m_pView;

	switch (iMsg)
	{
	case WM_SIZE:
	{
		if (f)
		{
			int nWidth = LOWORD(lParam);
			int nHeight = HIWORD(lParam);
			int cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
			int cyHScroll = GetSystemMetrics(SM_CYHSCROLL);

			int yTopRulerHeight = f->m_pData->m_pTopRuler->getHeight();
			int xLeftRulerWidth = f->m_pData->m_pLeftRuler->getWidth();
			
			MoveWindow(f->m_hwndVScroll, nWidth-cxVScroll, 0, cxVScroll, nHeight-cyHScroll, TRUE);
			MoveWindow(f->m_hwndHScroll, 0, nHeight-cyHScroll, nWidth - cxVScroll, cyHScroll, TRUE);
			MoveWindow(f->m_hwndDeadLowerRight, nWidth-cxVScroll, nHeight-cyHScroll, cxVScroll, cyHScroll, TRUE);
			MoveWindow(f->m_hwndTopRuler, 0, 0, nWidth-cxVScroll, yTopRulerHeight, TRUE);
			MoveWindow(f->m_hwndLeftRuler, 0, yTopRulerHeight,
					   xLeftRulerWidth, nHeight - yTopRulerHeight - cyHScroll, TRUE);
			MoveWindow(f->m_hwndDocument, xLeftRulerWidth, yTopRulerHeight,
					   nWidth - xLeftRulerWidth - cxVScroll, nHeight - yTopRulerHeight - cyHScroll, TRUE);
		}
		return 0;
	}

	case WM_VSCROLL:
	{
		int nScrollCode = (int) LOWORD(wParam); // scroll bar value 
		int nPos = (int) HIWORD(wParam);  // scroll box position 

		SCROLLINFO si;
		memset(&si, 0, sizeof(si));

		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(f->m_hwndVScroll, SB_CTL, &si);

		switch (nScrollCode)
		{
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			if (si.nPos < 0)
			{
				si.nPos = 0;
			}
			SetScrollInfo(f->m_hwndVScroll, SB_CTL, &si, TRUE);
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			SetScrollInfo(f->m_hwndVScroll, SB_CTL, &si, TRUE);
			break;
		case SB_LINEDOWN:
			si.nPos += SCROLL_LINE_SIZE;
			SetScrollInfo(f->m_hwndVScroll, SB_CTL, &si, TRUE);
			break;
		case SB_LINEUP:
			si.nPos -= SCROLL_LINE_SIZE;
			if (si.nPos < 0)
			{
				si.nPos = 0;
			}
			SetScrollInfo(f->m_hwndVScroll, SB_CTL, &si, TRUE);
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			si.nPos = nPos;
			SetScrollInfo(f->m_hwndVScroll, SB_CTL, &si, TRUE);
			break;
		}

		if (nScrollCode != SB_ENDSCROLL)
		{
			// in case we got clamped
			GetScrollInfo(f->m_hwndVScroll, SB_CTL, &si);

			// now tell the view
			pView->sendVerticalScrollEvent(si.nPos);
		}
	}
	return 0;

	case WM_HSCROLL:
	{
		int nScrollCode = (int) LOWORD(wParam); // scroll bar value 
		int nPos = (int) HIWORD(wParam);  // scroll box position 

		SCROLLINFO si;
		memset(&si, 0, sizeof(si));

		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(f->m_hwndHScroll, SB_CTL, &si);

		switch (nScrollCode)
		{
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			if (si.nPos < 0)
			{
				si.nPos = 0;
			}
			SetScrollInfo(f->m_hwndHScroll, SB_CTL, &si, TRUE);
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			SetScrollInfo(f->m_hwndHScroll, SB_CTL, &si, TRUE);
			break;
		case SB_LINEDOWN:
			si.nPos += SCROLL_LINE_SIZE;
			SetScrollInfo(f->m_hwndHScroll, SB_CTL, &si, TRUE);
			break;
		case SB_LINEUP:
			si.nPos -= SCROLL_LINE_SIZE;
			if (si.nPos < 0)
			{
				si.nPos = 0;
			}
			SetScrollInfo(f->m_hwndHScroll, SB_CTL, &si, TRUE);
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			si.nPos = nPos;
			SetScrollInfo(f->m_hwndHScroll, SB_CTL, &si, TRUE);
			break;
		}

		if (nScrollCode != SB_ENDSCROLL)
		{
			// in case we got clamped
			GetScrollInfo(f->m_hwndHScroll, SB_CTL, &si);

			// now tell the view
			pView->sendHorizontalScrollEvent(si.nPos);
		}
	}
	return 0;

	default:
		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	}
}

LRESULT CALLBACK AP_Win32Frame::_LeftRulerWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK AP_Win32Frame::_DocumentWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;

	AP_Win32Frame * f = GWL(hwnd);
	AV_View * pView = NULL;
	EV_Win32Mouse * pMouse = NULL;

	if (f)
	{
		pView = f->m_pView;
		pMouse = f->m_pWin32Mouse;
	}

	switch (iMsg)
	{
	case WM_CREATE:
		return 0;

	case WM_LBUTTONDOWN:
		pMouse->onButtonDown(pView,hwnd,EV_EMB_BUTTON1,wParam,LOWORD(lParam),HIWORD(lParam));
		return 0;
	case WM_MBUTTONDOWN:
		pMouse->onButtonDown(pView,hwnd,EV_EMB_BUTTON2,wParam,LOWORD(lParam),HIWORD(lParam));
		return 0;
	case WM_RBUTTONDOWN:
		pMouse->onButtonDown(pView,hwnd,EV_EMB_BUTTON3,wParam,LOWORD(lParam),HIWORD(lParam));
		return 0;

	case WM_LBUTTONDBLCLK:
		pMouse->onDoubleClick(pView,hwnd,EV_EMB_BUTTON1,wParam,LOWORD(lParam),HIWORD(lParam));
		return 0;
	case WM_MBUTTONDBLCLK:
		pMouse->onDoubleClick(pView,hwnd,EV_EMB_BUTTON2,wParam,LOWORD(lParam),HIWORD(lParam));
		return 0;
	case WM_RBUTTONDBLCLK:
		pMouse->onDoubleClick(pView,hwnd,EV_EMB_BUTTON3,wParam,LOWORD(lParam),HIWORD(lParam));
		return 0;

	case WM_MOUSEMOVE:
		pMouse->onButtonMove(pView,hwnd,wParam,LOWORD(lParam),HIWORD(lParam));
		return 0;

	case WM_LBUTTONUP:
		pMouse->onButtonUp(pView,hwnd,EV_EMB_BUTTON1,wParam,LOWORD(lParam),HIWORD(lParam));
		return 0;
	case WM_MBUTTONUP:
		pMouse->onButtonUp(pView,hwnd,EV_EMB_BUTTON2,wParam,LOWORD(lParam),HIWORD(lParam));
		return 0;
	case WM_RBUTTONUP:
		pMouse->onButtonUp(pView,hwnd,EV_EMB_BUTTON3,wParam,LOWORD(lParam),HIWORD(lParam));
		return 0;

	case WM_SIZE:
	{
		if (pView)
		{
			int nWidth = LOWORD(lParam);
			int nHeight = HIWORD(lParam);

			pView->setWindowSize(nWidth, nHeight);

			// may need to scroll to keep everything in sync

			// TODO decide if all of the following is necessary
			// TODO the above call to setWindowSize() causes alot
			// TODO of this to happen already.
			
			SCROLLINFO si;
			memset(&si, 0, sizeof(si));
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;

			GetScrollInfo(f->m_hwndVScroll, SB_CTL, &si);
			pView->sendVerticalScrollEvent(si.nPos,si.nMax-si.nPage);

			GetScrollInfo(f->m_hwndHScroll, SB_CTL, &si);
			pView->sendHorizontalScrollEvent(si.nPos,si.nMax-si.nPage);
		}
		return 0;
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);

		UT_Rect r(ps.rcPaint.left,ps.rcPaint.top,
				  ps.rcPaint.right-ps.rcPaint.left,
				  ps.rcPaint.bottom-ps.rcPaint.top);
		pView->draw(&r);

		EndPaint(hwnd, &ps);

		return 0;
	}

	case WM_DESTROY:
		return 0;
	} /* switch (iMsg) */

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

/*****************************************************************/

UT_Bool AP_Win32Frame::initFrameData(void)
{
	UT_ASSERT(!m_pData);

	m_pData = new AP_FrameData();
	
	return (m_pData ? UT_TRUE : UT_FALSE);
}

void AP_Win32Frame::killFrameData(void)
{
	DELETEP(m_pData);
	m_pData = NULL;
}

UT_Bool AP_Win32Frame::_loadDocument(const char * szFilename)
{
	// are we replacing another document?
	if (m_pDoc)
	{
		// yep.  first make sure it's OK to discard it, 
		// TODO: query user if dirty...
	}

	// load a document into the current frame.
	// if no filename, create a new document.

	AD_Document * pNewDoc = new PD_Document();
	UT_ASSERT(pNewDoc);
	
	if (!szFilename || !*szFilename)
	{
		pNewDoc->newDocument();
		m_iUntitled = _getNextUntitledNumber();
		goto ReplaceDocument;
	}

	if (pNewDoc->readFromFile(szFilename))
		goto ReplaceDocument;
	
	UT_DEBUGMSG(("ap_Frame: could not open the file [%s]\n",szFilename));
	delete pNewDoc;
	return UT_FALSE;

ReplaceDocument:
	// NOTE: prior document is bound to m_pData->m_pDocLayout, which gets discarded by subclass
	m_pDoc = pNewDoc;
	return UT_TRUE;
}
