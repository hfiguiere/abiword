/* AbiSource Application Framework
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2001-2002 Hubert Figuiere
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

#import <Cocoa/Cocoa.h>
#include <stdlib.h>
#include <string.h>
#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

#include "xap_App.h"
#include "xap_Dialog_Id.h"
#include "xap_CocoaApp.h"
#include "xap_CocoaFrame.h"

#include "xap_Dialog_Id.h"
#include "xap_Dlg_About.h"
#include "xap_CocoaDlg_About.h"

#include "gr_CocoaGraphics.h"

#import "xap_Cocoa_ResourceIDs.h"


/*****************************************************************/


XAP_Dialog * XAP_CocoaDialog_About::static_constructor(XAP_DialogFactory * pFactory,
													 XAP_Dialog_Id dlgid)
{
	XAP_CocoaDialog_About * p = new XAP_CocoaDialog_About(pFactory,dlgid);
	return p;
}

XAP_CocoaDialog_About::XAP_CocoaDialog_About(XAP_DialogFactory * pDlgFactory,
											 XAP_Dialog_Id dlgid)
	: XAP_Dialog_About(pDlgFactory,dlgid)
{
}

XAP_CocoaDialog_About::~XAP_CocoaDialog_About(void)
{
}

/*****************************************************************/

void XAP_CocoaDialog_About::runModal(XAP_Frame * pFrame)
{
	// stash away the frame
	m_pFrame = pFrame;

	m_dlg = [XAP_CocoaDlg_AboutController loadFromNib];
	[m_dlg setXAPOwner:this];
	NSWindow *win = [m_dlg window];		// force the window to be loaded.

	[NSApp runModalForWindow:win];

	m_pFrame = NULL;
}

void XAP_CocoaDialog_About::event_OK(void)
{
	[NSApp stopModal];
}

void XAP_CocoaDialog_About::event_URL(void)
{
	m_pFrame->openURL("http://www.abisource.com/");
}

/*****************************************************************/


@implementation XAP_CocoaDlg_AboutController
+ (XAP_CocoaDlg_AboutController *)loadFromNib
{
	XAP_CocoaDlg_AboutController * dlg = [[XAP_CocoaDlg_AboutController alloc] initWithWindowNibName:@"xap_CocoaDlg_About"];
	return [dlg autorelease];
}

- (void)windowDidLoad
{
	XAP_Frame *pFrame = m_xap->_getFrame ();
	// we get all our strings from the application string set
	const XAP_StringSet * pSS = pFrame->getApp()->getStringSet();
	[[self window] setTitle:[NSString stringWithFormat:@XAP_ABOUT_TITLE, pFrame->getApp()->getApplicationName()]];	
	[m_okBtn setTitle:[NSString stringWithCString:pSS->getValue(XAP_STRING_ID_DLG_OK)]];
	[m_appName setStringValue:[NSString stringWithCString:pFrame->getApp()->getApplicationName()]];
	[m_versionLabel setStringValue:[NSString stringWithFormat:@XAP_ABOUT_VERSION, XAP_App::s_szBuild_Version]];
	[m_licenseText setStringValue:[NSString stringWithFormat:@"%s\n\n%@", XAP_ABOUT_COPYRIGHT, 
	                               [NSString stringWithFormat:@XAP_ABOUT_GPL_LONG_LF,
	                 pFrame->getApp()->getApplicationName()]]];

	NSImage*	image = [NSImage imageNamed:XAP_COCOA_ABOUT_SIDEBAR_RESOURCE_NAME];
	[m_imageView setImage:image];
}

- (void)setXAPOwner:(XAP_CocoaDialog_About *)owner
{
	m_xap = owner;
}

- (IBAction)okBtnAction:(id)sender
{
	m_xap->event_OK();
}

- (IBAction)webBtnAction:(id)sender
{
	m_xap->event_URL();
}

@end

