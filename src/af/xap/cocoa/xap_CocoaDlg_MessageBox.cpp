/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:t -*- */

/* AbiSource Application Framework
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2001-2016 Hubert Figuiere
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301 USA.
 */

#import <Cocoa/Cocoa.h>

#include "ut_assert.h"
#include "xap_CocoaDlg_MessageBox.h"
#include "xap_CocoaApp.h"
#include "xap_CocoaFrame.h"

#include "xap_CocoaDialog_Utilities.h"


/*****************************************************************/
XAP_Dialog * XAP_CocoaDialog_MessageBox::static_constructor(XAP_DialogFactory * pFactory,
														 XAP_Dialog_Id dlgid)
{
	XAP_CocoaDialog_MessageBox * p = new XAP_CocoaDialog_MessageBox(pFactory, dlgid);
	return p;
}

XAP_CocoaDialog_MessageBox::XAP_CocoaDialog_MessageBox(XAP_DialogFactory * pDlgFactory,
												   XAP_Dialog_Id dlgid)
	: XAP_Dialog_MessageBox(pDlgFactory, dlgid)
{
}

XAP_CocoaDialog_MessageBox::~XAP_CocoaDialog_MessageBox(void)
{
}

/*****************************************************************/

void XAP_CocoaDialog_MessageBox::runModal(XAP_Frame * /*pFrame*/)
{
	const XAP_App* pApp = XAP_App::getApp();
	const XAP_StringSet* pSS = pApp->getStringSet();

	NSAlert* alert = [[NSAlert alloc] init];
	alert.alertStyle = NSAlertStyleWarning;
	alert.messageText = [NSString stringWithUTF8String:m_message.c_str()];
	alert.informativeText = [NSString stringWithFormat:@"%s", m_secondaryMessage.c_str()];

	switch (m_buttons) {
	case b_O:
		[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_OK)];
		[alert runModal];

		m_answer = a_OK;
		break;

	case b_OC:
		if (m_defaultAnswer == XAP_Dialog_MessageBox::a_OK)	{
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_OK)];
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_Cancel)];
			NSModalResponse btn = [alert runModal];

			switch (btn) {
			case NSAlertFirstButtonReturn:
				m_answer = a_OK;
				break;
			case NSAlertSecondButtonReturn:
				m_answer = a_CANCEL;
				break;
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
				break;
			}
		} else {
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_Cancel)];
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_OK)];
			NSModalResponse btn = [alert runModal];

			switch (btn) {
			case NSAlertFirstButtonReturn:
				m_answer = a_CANCEL;
				break;
			case NSAlertSecondButtonReturn:
				m_answer = a_OK;
				break;
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
				break;
			}
		}
		break;

	case b_YN:
		if (m_defaultAnswer == XAP_Dialog_MessageBox::a_YES) {
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_MB_Yes)];
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_MB_No)];
			NSModalResponse btn = [alert runModal];

			switch (btn) {
			case NSAlertFirstButtonReturn:
				m_answer = a_YES;
				break;
			case NSAlertSecondButtonReturn:
				m_answer = a_NO;
				break;
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
				break;
			}
		} else {
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_MB_No)];
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_MB_Yes)];
			NSModalResponse btn = [alert runModal];

			switch (btn) {
			case NSAlertFirstButtonReturn:
				m_answer = a_NO;
				break;
			case NSAlertSecondButtonReturn:
				m_answer = a_YES;
				break;
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
				break;
			}
		}
		break;

	case b_YNC:
		if (m_defaultAnswer == XAP_Dialog_MessageBox::a_YES) {
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_MB_Yes)];
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_MB_No)];
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_Cancel)];
			NSModalResponse btn = [alert runModal];

			switch (btn) {
			case NSAlertFirstButtonReturn:
				m_answer = a_YES;
				break;
			case NSAlertSecondButtonReturn:
				m_answer = a_NO;
				break;
			case NSAlertThirdButtonReturn:
				m_answer = a_CANCEL;
				break;
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
				break;
			}
		} else {
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_Cancel)];
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_MB_No)];
			[alert addButtonWithTitle:LocalizedString(pSS, XAP_STRING_ID_DLG_MB_Yes)];
			NSModalResponse btn = [alert runModal];

			switch (btn) {
			case NSAlertFirstButtonReturn:
				m_answer = a_CANCEL;
				break;
			case NSAlertSecondButtonReturn:
				m_answer = a_NO;
				break;
			case NSAlertThirdButtonReturn:
				m_answer = a_YES;
				break;
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
				break;
			}
		}
		break;

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		break;
	}
	[alert release];
}

