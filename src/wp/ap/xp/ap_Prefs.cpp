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

#include <stdio.h>
#include "ap_Prefs.h"
#include "xap_App.h"

/*****************************************************************/

AP_Prefs::AP_Prefs(XAP_App * pApp)
	: XAP_Prefs(pApp)
{
}

AP_Prefs::~AP_Prefs(void)
{
}

const XML_Char * AP_Prefs::getBuiltinSchemeName(void) const
{
	return "_builtin_";
}


UT_Bool AP_Prefs::loadBuiltinPrefs(void)
{
	// we have a built-in table of name/value pairs
	// (see {xap,ap}_Prefs_SchemeIds.h) that we
	// load into a prefs objects.
	//
	// we then overlay any name/value pairs in the
	// system default file.  this is installed in
	// a system library directory (see the canonical
	// layout).  we let the System Administrator to
	// override our built-in values.
	
	const XML_Char * szBuiltinSchemeName = getBuiltinSchemeName();
	
	XAP_PrefsScheme * pScheme = new XAP_PrefsScheme(szBuiltinSchemeName);
	if (!pScheme)
		return UT_FALSE;

	struct _table
	{
		XML_Char *		m_szKey;
		XML_Char *		m_szValue;
	};

	struct _table _t[] =
	{
#		define dcl(basename)			{ XAP_PREF_KEY_##basename, XAP_PREF_DEFAULT_##basename },
#		include "xap_Prefs_SchemeIds.h"
#		undef dcl

#		define dcl(basename)			{ AP_PREF_KEY_##basename, AP_PREF_DEFAULT_##basename },
#		include "ap_Prefs_SchemeIds.h"
#		undef dcl
	};

	for (UT_uint32 k=0; k<NrElements(_t); k++)
		if (!pScheme->setValue(_t[k].m_szKey, _t[k].m_szValue))
			goto Failed;

	addScheme(pScheme);					// set the builtin scheme in the base class
	overlaySystemPrefs();				// so that the base class parser can overlay it.
	
	return setCurrentScheme(szBuiltinSchemeName);
	
Failed:
	DELETEP(pScheme);
	return UT_FALSE;
}

void AP_Prefs::overlaySystemPrefs(void)
{
	// read system prefs file and overlay builtin values.

	const char * szSystemDefaultPrefsDir = m_pApp->getAbiSuiteAppDir();
	char buf[1024];
	sprintf(buf,"%s/%s",szSystemDefaultPrefsDir,"system.profile");

	loadSystemDefaultPrefsFile(buf);
}
