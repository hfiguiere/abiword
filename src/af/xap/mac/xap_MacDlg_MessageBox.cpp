/* AbiSource Application Framework
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 1999 John Brewer DBA Jera Design
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

#include "ut_assert.h"
#include "xap_MacDlg_MessageBox.h"
#include "xap_MacApp.h"
#include "xap_MacFrame.h"

/*****************************************************************/
AP_Dialog * XAP_MacDialog_MessageBox::static_constructor(AP_DialogFactory * pFactory,
														  AP_Dialog_Id id)
{
	XAP_MacDialog_MessageBox * p = new XAP_MacDialog_MessageBox(pFactory,id);
	return p;
}

XAP_MacDialog_MessageBox::XAP_MacDialog_MessageBox(AP_DialogFactory * pDlgFactory,
													 AP_Dialog_Id id)
	: XAP_Dialog_MessageBox(pDlgFactory,id)
{
}

XAP_MacDialog_MessageBox::~XAP_MacDialog_MessageBox(void)
{
}

/*****************************************************************/

void XAP_MacDialog_MessageBox::runModal(XAP_Frame * pFrame)
{
	m_pMacFrame = (XAP_MacFrame *)pFrame;
	UT_ASSERT(m_pMacFrame);
	XAP_MacApp * pApp = (XAP_MacApp *)m_pMacFrame->getApp();
	UT_ASSERT(pApp);

	const char * szCaption = pApp->getApplicationTitleForTitleBar();

	// the caller can get the answer from getAnswer().

	m_pMacFrame = NULL;
}

