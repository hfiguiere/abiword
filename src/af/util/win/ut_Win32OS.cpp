/* AbiSource Program Utilities
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
#include "ut_Win32OS.h"

/*!
 Returns Windows's OSVERSIONINFO structure
 */
OSVERSIONINFO& UT_GetWinVersion(void)
{
	static bool bInitialized = false;
	static OSVERSIONINFO os;

	if (!bInitialized)
	{
		os.dwOSVersionInfoSize = sizeof(os);
		BOOL bSuccess = GetVersionEx(&os);
		UT_ASSERT(bSuccess);
		bInitialized = true;
	}

	return os;
}

/*!
 Return true if we're running on Windows NT, false otherwise
 */
bool UT_IsWinNT(void)
{
	return UT_GetWinVersion().dwPlatformId == VER_PLATFORM_WIN32_NT;
}

/*!
 Return true if we're running on Windows 2000, false otherwise
 */
bool UT_IsWin2K(void)
{
	return (UT_GetWinVersion().dwPlatformId == VER_PLATFORM_WIN32_NT
		 && UT_GetWinVersion().dwMajorVersion >= 5);
}

/*****************************************************************/

/*!
 This function loads and locks a dialog template resource. 

 \param hinst
 \param lpszResName Name of the resource

 Returns the address of the locked resource.
 The caller is responsible for any unlocking/releasing necessary.
 This function is used by the various tabbed dialogs to load
 the sub-dialogs.
 */
DLGTEMPLATE * WINAPI UT_LockDlgRes(HINSTANCE hinst, LPCSTR lpszResName)
{ 
    HRSRC hrsrc = FindResource(NULL, lpszResName, RT_DIALOG); 
    HGLOBAL hglb = LoadResource(hinst, hrsrc); 
    return (DLGTEMPLATE *) LockResource(hglb); 	
} 

