/* AbiWord
 * Copyright (C) 2002 Jordi Mas i Hern�ndez <jmas@softcatala.org>
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

#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

#include "xap_App.h"
#include "xap_Win32App.h"
#include "xap_Win32Frame.h"
#include "xap_Win32PropertySheet.h"
#include "xap_Win32DialogHelper.h"

/*

	XAP_Win32PropertyPage
	
*/



int CALLBACK XAP_Win32PropertyPage::s_pageWndProc(HWND hWnd, UINT msg, WPARAM wParam,
   LPARAM lParam)
{	
	NMHDR* pNMHDR;
	
	switch(msg) 
	{
		case WM_INITDIALOG:
		{	
			PROPSHEETPAGE*	pStruct = (PROPSHEETPAGE*)lParam;  		
			XAP_Win32PropertyPage *pThis = (XAP_Win32PropertyPage *)pStruct->lParam;
			SetWindowLong(hWnd,DWL_USER,pStruct->lParam);
			pThis->m_hWnd = hWnd;
			pThis->_onInitDialog();
			return 0;
		}		

		case WM_NOTIFY:
		{
			pNMHDR = (NMHDR*)lParam;					
			if (pNMHDR->code==PSN_KILLACTIVE)
			{
				XAP_Win32PropertyPage *pThis = (XAP_Win32PropertyPage *)GetWindowLong(hWnd,DWL_USER);
				pThis->_onKillActive();
			}
		}		
		
		default:
		break;
	}  
        
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void XAP_Win32PropertyPage::createPage(XAP_Win32App* pWin32App, WORD wRscID,
	XAP_String_Id	nID /* =0 */)
{	
	m_pWin32App = pWin32App;
	LPCTSTR lpTemplate = MAKEINTRESOURCE(wRscID);	
	const XAP_StringSet * pSS = getApp()->getStringSet();									
	
	m_page.pszTitle = pSS->getValue(nID);
	
	m_page.dwSize = sizeof(PROPSHEETPAGE);
    m_page.dwFlags = PSP_DEFAULT;
    m_page.hInstance = pWin32App->getInstance();
	m_page.hIcon  = NULL;
	m_page.pszIcon  = NULL;	
    m_page.pszTemplate = lpTemplate;
    m_page.pfnDlgProc = s_pageWndProc;
    m_page.lParam = (LPARAM) this;
    m_page.pfnCallback = NULL;
	m_page.pcRefParent  = NULL;
	
	if (nID)
	{
		m_page.dwFlags = m_page.dwFlags | PSP_USETITLE;
		m_page.pszTitle = pSS->getValue(nID);
	}	
	else
		m_page.pszTitle = NULL;
    
	m_hdle = CreatePropertySheetPage(&m_page);
	
}

/*

	XAP_Win32PropertySheet
	
*/


PROPSHEETPAGE* XAP_Win32PropertySheet::_buildPageArray()
{
			
	PROPSHEETPAGE *pArPages = (PROPSHEETPAGE *) new PROPSHEETPAGE[m_vecPages.getItemCount()];
	PROPSHEETPAGE *pCurPage;
	pCurPage = pArPages;
	XAP_Win32PropertyPage* pPage;
	
	UT_sint32 count = m_vecPages.getItemCount();
	UT_sint32 i= 0;
	for(i=0; i< count; pCurPage++, i++)
	{			
		pPage = (XAP_Win32PropertyPage*)m_vecPages.getNthItem(i);		
		memcpy (pCurPage,  pPage->getStruct(), sizeof(PROPSHEETPAGE));
	}
	
	return pArPages;	
}


void XAP_Win32PropertySheet::addPage(XAP_Win32PropertyPage* pPage)
{
	m_vecPages.addItem(pPage);
}

/*
	Sheet window proc
*/
int CALLBACK SheetWndProc(
    HWND hwndDlg,
    UINT uMsg,
    LPARAM lParam)

{		
	return 0;
}


int XAP_Win32PropertySheet::runModal(XAP_Win32App* pWin32App, XAP_Frame * pFrame, XAP_String_Id nID/* = 0*/)
{		
	PROPSHEETPAGE*	pPages = _buildPageArray();
	const XAP_StringSet * pSS = pWin32App->getStringSet();
	XAP_Win32Frame * pWin32Frame = static_cast<XAP_Win32Frame *>(pFrame);
		
	memset (&m_psh, 0, sizeof(PROPSHEETHEADER));
		
    m_psh.dwSize = sizeof(PROPSHEETHEADER);
    m_psh.dwFlags = PSH_NOAPPLYNOW   |PSH_USECALLBACK| PSH_PROPSHEETPAGE;
    m_psh.hwndParent = pWin32Frame->getTopLevelWindow();
    m_psh.hInstance = pWin32App->getInstance();    
	m_psh.hIcon  = NULL;
	m_psh.pszIcon  = NULL;	
    m_psh.nPages = m_vecPages.getItemCount();
    m_psh.nStartPage = 0;
    m_psh.ppsp = (LPCPROPSHEETPAGE) pPages;
    m_psh.pfnCallback = SheetWndProc;
    
    if (nID)
    	m_psh.pszCaption  = pSS->getValue(nID);    	
    else
		m_psh.pszCaption  = NULL;
		
    int nRslt = PropertySheet(&m_psh);
    
    delete pPages;
    
    return nRslt;
}


