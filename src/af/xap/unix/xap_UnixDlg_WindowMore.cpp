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

#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

#include "xap_App.h"
#include "xap_UnixApp.h"
#include "xap_UnixFrame.h"

#include "ap_Dialog_Id.h"
#include "xap_Dlg_WindowMore.h"
#include "xap_UnixDlg_WindowMore.h"

#define FREEP(p)	do { if (p) free(p); (p)=NULL; } while (0)
#define DELETEP(p)	do { if (p) delete p; } while (0)

/*****************************************************************/
AP_Dialog * XAP_UnixDialog_WindowMore::static_constructor(AP_DialogFactory * pFactory,
													   AP_Dialog_Id id)
{
	XAP_UnixDialog_WindowMore * p = new XAP_UnixDialog_WindowMore(pFactory,id);
	return p;
}

XAP_UnixDialog_WindowMore::XAP_UnixDialog_WindowMore(AP_DialogFactory * pDlgFactory,
											   AP_Dialog_Id id)
	: XAP_Dialog_WindowMore(pDlgFactory,id)
{
}

XAP_UnixDialog_WindowMore::~XAP_UnixDialog_WindowMore(void)
{
}

void XAP_UnixDialog_WindowMore::runModal(XAP_Frame * pFrame)
{
	// NOTE: this work could be done in XP code
	m_pApp = pFrame->getApp();
	UT_ASSERT(m_pApp);
	m_ndxSelFrame = m_pApp->findFrame(pFrame);
	UT_ASSERT(m_ndxSelFrame >= 0);

	// raise the dialog
#if 0
	XAP_UnixApp * pUnixApp = static_cast<XAP_UnixApp *>(m_pApp);
	XAP_UnixFrame * pUnixFrame = static_cast<XAP_UnixFrame *>(pFrame);

	LPCTSTR lpTemplate = NULL;

	UT_ASSERT(m_id == XAP_DIALOG_ID_WINDOWMORE);

	lpTemplate = MAKEINTRESOURCE(XAP_RID_DIALOG_WINDOWMORE);

	int result = DialogBoxParam(pUnixApp->getInstance(),lpTemplate,
								pUnixFrame->getTopLevelWindow(),
								(DLGPROC)s_dlgProc,(LPARAM)this);
	UT_ASSERT((result != -1));
#endif 
}

#if 0
BOOL CALLBACK XAP_UnixDialog_WindowMore::s_dlgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	// This is a static function.

	XAP_UnixDialog_WindowMore * pThis;
	
	switch (msg)
	{
	case WM_INITDIALOG:
		pThis = (XAP_UnixDialog_WindowMore *)lParam;
		SetWindowLong(hWnd,DWL_USER,lParam);
		return pThis->_onInitDialog(hWnd,wParam,lParam);
		
	case WM_COMMAND:
		pThis = (XAP_UnixDialog_WindowMore *)GetWindowLong(hWnd,DWL_USER);
		return pThis->_onCommand(hWnd,wParam,lParam);
		
	default:
		return 0;
	}
}

BOOL XAP_UnixDialog_WindowMore::_onInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	{
		HWND hwndList = GetDlgItem(hWnd, XAP_RID_DIALOG_WINDOWMORE_LIST);  

		// load each frame name into the list
		for (UT_uint32 i=0; i<m_pApp->getFrameCount(); i++)
		{
			XAP_Frame * f = m_pApp->getFrame(i);
			UT_ASSERT(f);
			const char * s = f->getTitle(128);	// TODO: chop this down more? 

            SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM) s); 
            SendMessage(hwndList, LB_SETITEMDATA, i, (LPARAM) i);  
        } 

		// select the one we're in
		SendMessage(hwndList, LB_SETCURSEL, (WPARAM) m_ndxSelFrame, 0);
	}		

	return 1;							// 1 == we did not call SetFocus()
}

BOOL XAP_UnixDialog_WindowMore::_onCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);
	WORD wId = LOWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	int nItem;

	switch (wId)
	{
	case XAP_RID_DIALOG_WINDOWMORE_LIST:
		switch (HIWORD(wParam))
		{
			case LBN_SELCHANGE:
				// NOTE: we could get away with only grabbing this in IDOK case
				nItem = SendMessage(hWndCtrl, LB_GETCURSEL, 0, 0);
				m_ndxSelFrame = SendMessage(hWndCtrl, LB_GETITEMDATA, nItem, 0);
				return 1;

			case LBN_DBLCLK:
				nItem = SendMessage(hWndCtrl, LB_GETCURSEL, 0, 0);
				m_ndxSelFrame = SendMessage(hWndCtrl, LB_GETITEMDATA, nItem, 0);
				EndDialog(hWnd,0);
				return 1;

			default:
				return 0;
		}
		break;

	case IDCANCEL:						// also XAP_RID_DIALOG_WINDOWMORE_BTN_CANCEL
		m_answer = a_CANCEL;
		// fall through

	case IDOK:							// also XAP_RID_DIALOG_WINDOWMORE_BTN_OK
		EndDialog(hWnd,0);
		return 1;

	default:							// we did not handle this notification
		UT_DEBUGMSG(("WM_Command for id %ld\n",wId));
		return 0;						// return zero to let windows take care of it.
	}
}

#endif