/* AbiSource Application Framework
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
#include <commctrl.h>
#include <stdio.h>
#include "zmouse.h"

#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "xap_ViewListener.h"
#include "xap_Win32App.h"
#include "xap_Win32Frame.h"
#include "ev_Win32Keyboard.h"
#include "ev_Win32Mouse.h"
#include "ev_Win32Menu.h"
#include "ev_Win32Toolbar.h"
#include "ev_EditMethod.h"
#include "xav_View.h"
#include "xad_Document.h"

#include "fv_View.h"

#pragma warning(disable:4355)

/*****************************************************************/

#define GWL(hwnd)		(XAP_Win32Frame*)GetWindowLong((hwnd), GWL_USERDATA)
#define SWL(hwnd, f)	(XAP_Win32Frame*)SetWindowLong((hwnd), GWL_USERDATA,(LONG)(f))

#define ENSUREP(p)		do { UT_ASSERT(p); if (!p) goto Cleanup; } while (0)

/*****************************************************************/

UT_Bool XAP_Win32Frame::RegisterClass(XAP_Win32App * app)
{
	// NB: can't access 'this' members from a static member function
	WNDCLASSEX  wndclass;
	ATOM a;

	// register class for the frame window
	wndclass.cbSize        = sizeof(wndclass);
	wndclass.style         = CS_DBLCLKS;
	wndclass.lpfnWndProc   = XAP_Win32Frame::_FrameWndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = app->getInstance();
	wndclass.hIcon         = app->getIcon();
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = app->getApplicationName();
	wndclass.hIconSm       = app->getSmallIcon();

	a = RegisterClassEx(&wndclass);
	UT_ASSERT(a);

	return UT_TRUE;
}

/*****************************************************************/

XAP_Win32Frame::XAP_Win32Frame(XAP_Win32App * app)
:	XAP_Frame(app),
	m_dialogFactory(this, app),
	m_iBarHeight(0),
	m_iStatusBarHeight(0)
{
	m_pWin32App = app;
	m_pWin32Menu = NULL;
	m_pWin32Popup = NULL;
	m_pView = NULL;
	m_hwndFrame = NULL;
	m_hwndRebar = NULL;
	m_hwndContainer = NULL;
	m_iSizeWidth = 0;
	m_iSizeHeight = 0;
	m_mouseWheelMessage = 0;
}

// TODO when cloning a new frame from an existing one
// TODO should we also clone any frame-persistent
// TODO dialog data ??

XAP_Win32Frame::XAP_Win32Frame(XAP_Win32Frame * f)
	: XAP_Frame(static_cast<XAP_Frame *>(f)),
	m_dialogFactory(this, static_cast<XAP_App *>(f->m_pWin32App))
{
	m_pWin32App = f->m_pWin32App;
	m_pWin32Menu = NULL;
	m_pWin32Popup = NULL;
	m_pView = NULL;
	m_hwndFrame = NULL;
	m_hwndRebar = NULL;
	m_hwndContainer = NULL;
	m_iSizeWidth = 0;
	m_iSizeHeight = 0;
	m_mouseWheelMessage = 0;
}

XAP_Win32Frame::~XAP_Win32Frame(void)
{
	// only delete the things we created...
	
	DELETEP(m_pWin32Menu);
	DELETEP(m_pWin32Popup);
}

UT_Bool XAP_Win32Frame::initialize(const char * szKeyBindingsKey, const char * szKeyBindingsDefaultValue,
								   const char * szMenuLayoutKey, const char * szMenuLayoutDefaultValue,
								   const char * szMenuLabelSetKey, const char * szMenuLabelSetDefaultValue,
								   const char * szToolbarLayoutsKey, const char * szToolbarLayoutsDefaultValue,
								   const char * szToolbarLabelSetKey, const char * szToolbarLabelSetDefaultValue)
{
	UT_Bool bResult;

	// invoke our base class first.
	
	bResult = XAP_Frame::initialize(szKeyBindingsKey, szKeyBindingsDefaultValue,
									szMenuLayoutKey, szMenuLayoutDefaultValue,
									szMenuLabelSetKey, szMenuLabelSetDefaultValue,
									szToolbarLayoutsKey, szToolbarLayoutsDefaultValue,
									szToolbarLabelSetKey, szToolbarLabelSetDefaultValue);
	UT_ASSERT(bResult);

	// get a handle to our keyboard binding mechanism
	// and to our mouse binding mechanism.
	
	EV_EditEventMapper * pEEM = getEditEventMapper();
	UT_ASSERT(pEEM);

	m_pKeyboard = new ev_Win32Keyboard(pEEM);
	UT_ASSERT(m_pKeyboard);
	
	m_pMouse = new EV_Win32Mouse(pEEM);
	UT_ASSERT(m_pMouse);

	// TODO: Jeff, I'm currently showing in WinMain, to honor iCmdShow.
	// should we pass that via argv, to do it here for all frames?

	return UT_TRUE;
}

UT_sint32 XAP_Win32Frame::setInputMode(const char * szName)
{
	UT_sint32 result = XAP_Frame::setInputMode(szName);
	if (result == 1)
	{
		// if it actually changed we need to update keyboard and mouse

		EV_EditEventMapper * pEEM = getEditEventMapper();
		UT_ASSERT(pEEM);

		m_pKeyboard->setEditEventMap(pEEM);
		m_pMouse->setEditEventMap(pEEM);
	}

	return result;
}

HWND XAP_Win32Frame::getTopLevelWindow(void) const
{
	return m_hwndFrame;
}

HWND XAP_Win32Frame::getToolbarWindow(void) const
{
	return m_hwndRebar;
}

XAP_DialogFactory * XAP_Win32Frame::getDialogFactory(void)
{
	return &m_dialogFactory;
}

void XAP_Win32Frame::_createTopLevelWindow(void)
{
	RECT r;
	UT_uint32 iHeight, iWidth;

	// create a top-level window for us.
	// TODO get the default window size from preferences or something.

	m_hwndFrame = CreateWindow(m_pWin32App->getApplicationName(),
							   m_pWin32App->getApplicationTitleForTitleBar(),
							   WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
							   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
							   NULL, NULL, m_pWin32App->getInstance(), NULL);
	UT_ASSERT(m_hwndFrame);
	SWL(m_hwndFrame, this);					// bind this frame to its window

	m_mouseWheelMessage = RegisterWindowMessage(MSH_MOUSEWHEEL);

	// synthesize a menu from the info in our
	// base class and install it into the window.
	m_pWin32Menu = new EV_Win32MenuBar(m_pWin32App,
									   getEditEventMapper(),
									   m_szMenuLayoutName,
									   m_szMenuLabelSetName);
	UT_ASSERT(m_pWin32Menu);
	UT_Bool bResult = m_pWin32Menu->synthesizeMenuBar(this);
	UT_ASSERT(bResult);

	HMENU oldMenu = GetMenu(m_hwndFrame);
	if (SetMenu(m_hwndFrame, m_pWin32Menu->getMenuHandle()))
	{
		DrawMenuBar(m_hwndFrame);
		if (oldMenu)
			DestroyMenu(oldMenu);
	}

	// create a rebar container for all the toolbars
	m_hwndRebar = CreateWindowEx(0L, REBARCLASSNAME, NULL,
								 WS_VISIBLE | WS_BORDER | WS_CHILD | WS_CLIPCHILDREN |
								 WS_CLIPSIBLINGS | CCS_NODIVIDER | CCS_NOPARENTALIGN |
								 RBS_VARHEIGHT | RBS_BANDBORDERS,
								 0, 0, 0, 0,
								 m_hwndFrame, NULL, m_pWin32App->getInstance(), NULL);
	UT_ASSERT(m_hwndRebar);

	// create a toolbar instance for each toolbar listed in our base class.

	_createToolbars();

	// figure out how much room is left for the child
	GetClientRect(m_hwndFrame, &r);
	iHeight = r.bottom - r.top;
	iWidth = r.right - r.left;

	m_iSizeWidth = iWidth;
	m_iSizeHeight = iHeight;
	
	// force rebar to resize itself
	// TODO for some reason, we give REBAR the height of the FRAME
	// TODO and let it decide how much it actually needs....
	if( m_hwndRebar != NULL )
	{
		MoveWindow(m_hwndRebar, 0, 0, iWidth, iHeight, TRUE);

		GetClientRect(m_hwndRebar, &r);
		m_iBarHeight = r.bottom - r.top + 6;

		UT_ASSERT(iHeight > m_iBarHeight);
		iHeight -= m_iBarHeight;
	}
	else
		m_iBarHeight = 0;

	m_hwndContainer = _createDocumentWindow(m_hwndFrame, 0, m_iBarHeight, iWidth, iHeight);

	// Let the app-specific frame code create the status bar
	// if it wants to.  we will put it below the document
	// window (a peer with toolbars and the overall sunkenbox)
	// so that it will appear outside of the scrollbars.

	m_hwndStatusBar = _createStatusBarWindow(m_hwndFrame,0,m_iBarHeight+iHeight,iWidth);
	GetClientRect(m_hwndStatusBar,&r);
	m_iStatusBarHeight = r.bottom;

	// we let our caller decide when to show m_hwndFrame.

	return;
}

UT_Bool XAP_Win32Frame::close()
{
	// NOTE: this should only be called from the closeWindow edit method
	DestroyWindow(m_hwndFrame);

	return UT_TRUE;
}

UT_Bool XAP_Win32Frame::raise()
{
	BringWindowToTop(m_hwndFrame);

	return UT_TRUE;
}

UT_Bool XAP_Win32Frame::show()
{
	ShowWindow(m_hwndFrame, SW_SHOW);
	UpdateWindow(m_hwndFrame);

	return UT_TRUE;
}

UT_Bool XAP_Win32Frame::openURL(const char * szURL)
{
	// NOTE: could get finer control over browser window via DDE 
	// NOTE: may need to fallback to WinExec for old NSCP versions

	HWND hwnd = getTopLevelWindow();
	int res = (int) ShellExecute(hwnd, "open", szURL, NULL, NULL, SW_SHOWNORMAL);

	// TODO: more specific error messages ??
	UT_ASSERT(res>32);

	return (res>32);
}

UT_Bool XAP_Win32Frame::updateTitle()
{
	if (!XAP_Frame::updateTitle())
	{
		// no relevant change, so skip it
		return UT_FALSE;
	}

	char buf[256];
	buf[0] = 0;

	const char * szAppName = m_pWin32App->getApplicationTitleForTitleBar();

	int len = 256 - strlen(szAppName) - 4;
	
	const char * szTitle = getTitle(len);

	sprintf(buf, "%s - %s", szTitle, szAppName);
	
	SetWindowText(m_hwndFrame, buf);

	return UT_TRUE;
}

LRESULT CALLBACK XAP_Win32Frame::_FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	XAP_Win32Frame * f = GWL(hwnd);

	if (!f)
	{
		return DefWindowProc(hwnd,iMsg,wParam,lParam);
	}
	
	AV_View * pView = NULL;

	pView = f->m_pView;

	FV_View* pfView = static_cast<FV_View*>(pView);

	if(iMsg == f->m_mouseWheelMessage)
	{
		wParam = MAKEWPARAM(0, (short)(int)wParam);
		return SendMessage(hwnd, WM_MOUSEWHEEL, wParam, lParam);
	}

	switch (iMsg)
	{
	case WM_EXITMENULOOP:
	case WM_SETFOCUS:
		if (pView)
		{
			pView->focusChange(AV_FOCUS_HERE);
			SetFocus(f->m_hwndContainer);
		}
		return 0;

	case WM_ENTERMENULOOP:
	case WM_KILLFOCUS:
		if (pView)
		{
			pView->focusChange(AV_FOCUS_NONE);
		}
		return 0;

	case WM_CREATE:
		return 0;

	case WM_COMMAND:
		if (f->m_pWin32Popup)
		{
			if (f->m_pWin32Popup->onCommand(pView,hwnd,wParam))
				return 0;
		}
		else if (f->m_pWin32Menu->onCommand(pView,hwnd,wParam))
		{
			return 0;
		}
		else if (HIWORD(wParam) == 0)
		{
			// after menu passes on it, give each of the toolbars a chance
			UT_uint32 nrToolbars, k;
			nrToolbars = f->m_vecToolbars.getItemCount();
			for (k=0; k < nrToolbars; k++)
			{
				EV_Win32Toolbar * t = (EV_Win32Toolbar *)f->m_vecToolbars.getNthItem(k);
				XAP_Toolbar_Id id = t->ItemIdFromWmCommand(LOWORD(wParam));
				if (t->toolbarEvent(id))
					return 0;
			}
		}
		return DefWindowProc(hwnd,iMsg,wParam,lParam);

	case WM_INITMENU:
		if (f->m_pWin32Popup)
		{
			if (f->m_pWin32Popup->onInitMenu(f,pView,hwnd,(HMENU)wParam))
				return 0;
		}
		else if (f->m_pWin32Menu->onInitMenu(f,pView,hwnd,(HMENU)wParam))
			return 0;
		return DefWindowProc(hwnd,iMsg,wParam,lParam);

	case WM_MENUSELECT:
		if (f->m_pWin32Popup)
		{
			if (f->m_pWin32Popup->onMenuSelect(f,pView,hwnd,(HMENU)lParam,wParam))
				return 0;
		}
		else if (f->m_pWin32Menu->onMenuSelect(f,pView,hwnd,(HMENU)lParam,wParam))
			return 0;
		return DefWindowProc(hwnd,iMsg,wParam,lParam);

	case WM_NOTIFY:
		switch (((LPNMHDR) lParam)->code) 
		{
		case TTN_NEEDTEXT:
			{
				UT_uint32 nrToolbars, k;
				nrToolbars = f->m_vecToolbars.getItemCount();
				for (k=0; k < nrToolbars; k++)
				{
					EV_Win32Toolbar * t = (EV_Win32Toolbar *)f->m_vecToolbars.getNthItem(k);
					if (t->getToolTip(lParam))
						break;
				}
			}
			break;

		case RBN_HEIGHTCHANGE:
			{
				RECT r;
				GetClientRect(f->m_hwndFrame, &r);
				int nWidth = r.right - r.left;
				int nHeight = r.bottom - r.top;

				GetClientRect(f->m_hwndRebar, &r);
				f->m_iBarHeight = r.bottom - r.top + 6;

				if (f->m_hwndContainer)
				{
					// leave room for the toolbars
					nHeight -= f->m_iBarHeight;

					MoveWindow(f->m_hwndContainer, 0, f->m_iBarHeight, nWidth, nHeight, TRUE);
				}
			}
			break;

		case NM_CUSTOMDRAW:
			{
				LPNMCUSTOMDRAW  pNMcd = (LPNMCUSTOMDRAW)lParam;
				UT_uint32 nrToolbars, k;
				nrToolbars = f->m_vecToolbars.getItemCount();
				for (k=0; k < nrToolbars; k++)
				{
					EV_Win32Toolbar * t = (EV_Win32Toolbar *)f->m_vecToolbars.getNthItem(k);
					if( pNMcd->hdr.hwndFrom == t->getWindow() )
					{
						if( pNMcd->dwDrawStage == CDDS_PREPAINT )
						{
							return CDRF_NOTIFYPOSTPAINT;
						}
						if( pNMcd->dwDrawStage == CDDS_POSTPAINT )
						{
							RECT  rc;
							HBRUSH	hBr = NULL;

							rc.top = pNMcd->rc.top;
							rc.bottom = pNMcd->rc.bottom;
							hBr = GetSysColorBrush( COLOR_3DFACE );

							HWND  hWndChild = FindWindowEx( pNMcd->hdr.hwndFrom, NULL, NULL, NULL );
							while( hWndChild != NULL )
							{
								RECT   rcChild;
								POINT  pt;
								GetWindowRect( hWndChild, &rcChild );
								pt.x = rcChild.left;
								pt.y = rcChild.top;
								ScreenToClient( pNMcd->hdr.hwndFrom, &pt );
								rc.left = pt.x;
								pt.x = rcChild.right;
								pt.y = rcChild.bottom;
								ScreenToClient( pNMcd->hdr.hwndFrom, &pt );
								rc.right = pt.x;
								FillRect( pNMcd->hdc, &rc, hBr );
								hWndChild = FindWindowEx( pNMcd->hdr.hwndFrom, hWndChild, NULL, NULL );
							}

							if( hBr != NULL )
							{
								DeleteObject( hBr );
								hBr = NULL;
							}
						}
						break;
					}
				}
			}
			break;

		// Process other notifications here
		default:
			break;
		} 
		break;

	case WM_SIZE:
	{
		int nWidth = LOWORD(lParam);
		int nHeight = HIWORD(lParam);

		if (nWidth != (int) f->m_iSizeWidth && f->m_hwndRebar != NULL)
		{
			MoveWindow(f->m_hwndRebar, 0, 0, nWidth, f->m_iBarHeight, TRUE); 
		}

		// leave room for the toolbars and the status bar
		nHeight -= f->m_iBarHeight;
		nHeight -= f->m_iStatusBarHeight;

		if (f->m_hwndContainer)
			MoveWindow(f->m_hwndContainer, 0, f->m_iBarHeight, nWidth, nHeight, TRUE);

		if (f->m_hwndStatusBar)
			MoveWindow(f->m_hwndStatusBar, 0, f->m_iBarHeight+nHeight, nWidth, f->m_iStatusBarHeight, TRUE);
			
		f->m_iSizeWidth = nWidth;
		f->m_iSizeHeight = nHeight;


		UT_uint32 newZoom = 0;

		switch(f->getZoomType())
		{
//		// special cases
		case XAP_Frame::z_PAGEWIDTH:
			newZoom = pfView->calculateZoomPercentForPageWidth();
			f->setZoomPercentage(newZoom);

			break;
		case XAP_Frame::z_WHOLEPAGE:
			newZoom = pfView->calculateZoomPercentForWholePage();
			f->setZoomPercentage(newZoom);
			break;
		default:
			;
		}
		
		return 0;
	}

	case WM_CLOSE:
	{
		XAP_App * pApp = f->getApp();
		UT_ASSERT(pApp);

		const EV_EditMethodContainer * pEMC = pApp->getEditMethodContainer();
		UT_ASSERT(pEMC);

		EV_EditMethod * pEM = pEMC->findEditMethodByName("closeWindow");
		UT_ASSERT(pEM);						// make sure it's bound to something

		if (pEM)
		{
			(*pEM->getFn())(pView,NULL);
			return 0;
		}

		// let the window be destroyed
		break;
	}

	case WM_INPUTLANGCHANGE:
	{
		UT_DEBUGMSG(("Frame received input language change\n"));

		// This will remap the static tables used by all frames.
		// (see the comment in ev_Win32Keyboard.cpp.)
		ev_Win32Keyboard *pWin32Keyboard = static_cast<ev_Win32Keyboard *>(f->m_pKeyboard);
		pWin32Keyboard->remapKeyboard((HKL)lParam);

		// We must propagate this message.
		
		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	}

	case WM_MOUSEWHEEL:
	{
		return SendMessage(f->m_hwndContainer, iMsg, wParam, lParam);
	}

	case WM_SYSCOLORCHANGE:
	{
		if (f->m_hwndRebar)
		{
			SendMessage(f->m_hwndRebar,WM_SYSCOLORCHANGE,0,0);

			REBARBANDINFO rbbi;
			ZeroMemory(&rbbi, sizeof(rbbi));
			rbbi.cbSize = sizeof(REBARBANDINFO);
			rbbi.fMask = RBBIM_COLORS;
			rbbi.clrFore = GetSysColor(COLOR_BTNTEXT);
			rbbi.clrBack = GetSysColor(COLOR_BTNFACE);

			UT_uint32 nrToolbars = f->m_vecToolbars.getItemCount();
			for (UT_uint32 k=0; k < nrToolbars; k++)
				SendMessage(f->m_hwndRebar, RB_SETBANDINFO,k,(LPARAM)&rbbi);
		}

		if (f->m_hwndContainer)
			SendMessage(f->m_hwndContainer,WM_SYSCOLORCHANGE,0,0);
		if (f->m_hwndStatusBar)
			SendMessage(f->m_hwndStatusBar,WM_SYSCOLORCHANGE,0,0);
		return 0;
	}

	case WM_DESTROY:
		return 0;

	} /* switch (iMsg) */

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

/*****************************************************************/

UT_Bool XAP_Win32Frame::runModalContextMenu(AV_View * pView, const char * szMenuName,
											UT_sint32 x, UT_sint32 y)
{
	UT_Bool bResult = UT_FALSE;

	UT_ASSERT(!m_pWin32Popup);

	m_pWin32Popup = new EV_Win32MenuPopup(m_pWin32App,szMenuName,m_szMenuLabelSetName);
	if (m_pWin32Popup && m_pWin32Popup->synthesizeMenuPopup(this))
	{
		UT_DEBUGMSG(("ContextMenu: %s at [%d,%d]\n",szMenuName,x,y));

		translateDocumentToScreen(x,y);

		TrackPopupMenu(m_pWin32Popup->getMenuHandle(),
					   TPM_CENTERALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON,
					   x,y,0,m_hwndFrame,NULL);

		// the popup steals our capture, so we need to reset our counter.
		EV_Win32Mouse *pWin32Mouse = static_cast<EV_Win32Mouse *>(m_pMouse);
		pWin32Mouse->reset();
	}

	DELETEP(m_pWin32Popup);
	return bResult;
}

EV_Toolbar * XAP_Win32Frame::_newToolbar(XAP_App *app, XAP_Frame *frame,
					const char *szLayout,
					const char *szLanguage)
{
    RECT r;
	UT_uint32 iHeight;
	EV_Win32Toolbar *result = new EV_Win32Toolbar(static_cast<XAP_Win32App *>(app), 
												  static_cast<XAP_Win32Frame *>(frame), 
												  szLayout, szLanguage);
	// for now, position each one manually
	// TODO: put 'em all in a rebar instead
	HWND hwndBar = result->getWindow();
	
	GetClientRect(hwndBar, &r);
	iHeight = r.bottom - r.top;
	
	m_iBarHeight += iHeight;

	return result;
}
