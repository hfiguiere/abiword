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
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "xap_Args.h"
#include "xap_Win32App.h"
#include "xap_Win32Clipboard.h"
#include "xap_Win32Frame.h"
#include "xap_Win32Toolbar_Icons.h"
#include "xap_Win32Toolbar_ControlFactory.h"
#include "sp_spell.h"

#define DELETEP(p)	do { if (p) delete p; } while (0)

/*****************************************************************/

AP_Win32App::AP_Win32App(HINSTANCE hInstance, AP_Args * pArgs, const char * szAppName)
	: AP_App(pArgs, szAppName), m_dialogFactory(this), m_controlFactory()
{
	UT_ASSERT(hInstance);

	m_hInstance = hInstance;
	m_pWin32ToolbarIcons = 0;
}

AP_Win32App::~AP_Win32App(void)
{
	SpellCheckCleanup();

	DELETEP(m_pWin32ToolbarIcons);
	DELETEP(_pClipboard);
}

HINSTANCE AP_Win32App::getInstance() const
{
	return m_hInstance;
}

UT_Bool AP_Win32App::initialize(void)
{
	// let our base class do it's thing.
	
	AP_App::initialize();

	// load only one copy of the platform-specific icons.

	m_pWin32ToolbarIcons = new AP_Win32Toolbar_Icons();
	
	// let various window types register themselves

	if (!AP_Win32Frame::RegisterClass(this))
	{
		UT_DEBUGMSG(("couldn't register class\n"));
		return UT_FALSE;
	}

	// do anything else we need here...

	_pClipboard = new AP_Win32Clipboard();
	
	/*
	  The following call initializes the spell checker.
	  It does NOT belong here.  However, right now, it's
	  not clear where it does belong.
	  HACK TODO fix this

	  Furthermore, it currently initializes the dictionary
	  to a hard-coded path -- the dictionary must be in the
	  same directory as the one you started the app in.
	  TODO fix this
	*/

	SpellCheckInit("c:/american.hash");		// NB: yes, the slash should be backwards
	
	return UT_TRUE;
}

AP_Frame * AP_Win32App::newFrame(void)
{
	AP_Win32Frame * pWin32Frame = new AP_Win32Frame(this);

	if (pWin32Frame)
		pWin32Frame->initialize();

	return pWin32Frame;
}

void AP_Win32App::reallyExit(void)
{
	PostQuitMessage (0);
}

AP_DialogFactory * AP_Win32App::getDialogFactory(void)
{
	return &m_dialogFactory;
}

AP_Toolbar_ControlFactory * AP_Win32App::getControlFactory(void)
{
	return &m_controlFactory;
}
