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

#include <gtk/gtk.h>
#include "ap_Args.h"
#include "ap_UnixApp.h"
#include "ap_UnixFrame.h"
#include "ap_UnixToolbar_Icons.h"

#define DELETEP(p)	do { if (p) delete p; } while (0)

/*****************************************************************/

AP_UnixApp::AP_UnixApp(AP_Args * pArgs, const char * szAppName)
	: AP_App(pArgs, szAppName), m_dialogFactory(this)
{
	m_pUnixToolbarIcons = 0;
}

AP_UnixApp::~AP_UnixApp(void)
{
	DELETEP(m_pUnixToolbarIcons);
}

UT_Bool AP_UnixApp::initialize(void)
{
	// initialize GTK first.
	
	gtk_set_locale();
	gtk_init(&m_pArgs->m_argc,&m_pArgs->m_argv);

	// let our base class do it's thing.
	
	AP_App::initialize();

	// load only one copy of the platform-specific icons.
	
	m_pUnixToolbarIcons = new AP_UnixToolbar_Icons();
	
	// do any thing we need here...
	
	return UT_TRUE;
}

AP_Frame * AP_UnixApp::newFrame(void)
{
	AP_UnixFrame * pUnixFrame = new AP_UnixFrame(this);

	if (pUnixFrame)
		pUnixFrame->initialize();

	return pUnixFrame;
}

AP_DialogFactory * AP_UnixApp::getDialogFactory(void)
{
	return &m_dialogFactory;
}
