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

/*****************************************************************
** Only one of these is created by the application.
*****************************************************************/

#ifdef ABI_OPT_JS
#include <js.h>
#endif /* ABI_OPT_JS */

#include <stdio.h>
#include <string.h>

#include "xap_Args.h"
#include "ap_MacFrame.h"
#include "ap_MacApp.h"

#define NrElements(a)		(sizeof(a) / sizeof(a[0]))
#define FREEP(p)	do { if (p) free(p); (p)=NULL; } while (0)
#define DELETEP(p)	do { if (p) delete(p); (p)=NULL; } while (0)

/*****************************************************************/

AP_MacApp::AP_MacApp(AP_Args * pArgs, const char * szAppName)
	: XAP_MacApp(pArgs,szAppName)
{
	m_prefs = NULL;
}

AP_MacApp::~AP_MacApp(void)
{
	DELETEP(m_prefs);
}

UT_Bool AP_MacApp::initialize(void)
{
	// load preferences, first the builtin set and then any on disk.
	
	m_prefs = new AP_MacPrefs(this);
	m_prefs->loadBuiltinPrefs();
	m_prefs->loadPrefsFile();

	// TODO overlay command line arguments onto preferences...
		   
	// now that preferences are established, let the xap init
		   
	return XAP_MacApp::initialize();
}

UT_Bool AP_MacApp::shutdown(void)
{
	if (m_prefs->getAutoSavePrefs())
		m_prefs->savePrefsFile();

	return UT_TRUE;
}

XAP_Prefs * AP_MacApp::getPrefs(void) const
{
	return m_prefs;
}

UT_Bool AP_MacApp::getPrefsValue(const XML_Char * szKey, const XML_Char ** pszValue) const
{
	if (!m_prefs)
		return UT_FALSE;

	return m_prefs->getPrefsValue(szKey,pszValue);
}

/*****************************************************************/

int AP_MacApp::MacMain(const char * szAppName, int argc, char **argv)
{
	// initialize our application.

	AP_Args Args = AP_Args(argc,argv);
	
	AP_MacApp * pMyMacApp = new AP_MacApp(&Args, szAppName);
	pMyMacApp->initialize();

	// create the first window.

	AP_MacFrame * pFirstMacFrame = new AP_MacFrame(pMyMacApp);
	pFirstMacFrame->initialize();
//	hwnd = pFirstMacFrame->getTopLevelWindow();

	// turn over control to windows

	unsigned short mask = 0;
	struct EventRecord theEvent;
	unsigned long delay = 0;
	while (WaitNextEvent(mask, &theEvent, delay, NULL))
	{
		// Note: we do not call TranslateMessage() because
		// Note: the keybinding mechanism is responsible
		// Note: for deciding if/when to do this.

	}

	// destroy the App.  It should take care of deleting all frames.

	pMyMacApp->shutdown();
	delete pMyMacApp;

	return 0;
}
