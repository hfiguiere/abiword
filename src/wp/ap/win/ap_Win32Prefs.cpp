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

#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "ut_debugmsg.h"
#include "ap_Win32Prefs.h"

/*****************************************************************/

AP_Win32Prefs::AP_Win32Prefs(XAP_App * pApp)
	: AP_Prefs(pApp)
{
}

const char * AP_Win32Prefs::getPrefsPathname(void) const
{
	/* return a pointer to a static buffer */
	
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

	static char buf[PATH_MAX];
	memset(buf,0,sizeof(buf));

	char * szFile = "abiword.txt";
	int lenFile = strlen(szFile);
	
	DWORD len = GetEnvironmentVariable("HOMEPATH",buf,PATH_MAX-lenFile-2);
	if (len == 0)
	{
		UT_DEBUGMSG(("HOMEPATH not set\n"));
		strcpy(buf,"C:\\");
		strcat(buf,szFile);
	}
	else
	{
		if (buf[len-1] != '\\')
			strcat(buf,"\\");
		strcat(buf,szFile);
	}

	UT_DEBUGMSG(("Constructed preference file name [%s]\n",buf));
	
	return buf;
}
