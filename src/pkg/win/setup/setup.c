/* AbiWord
 * Copyright (C) 1999 AbiSource, Inc.
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

#include "ask.h"

#include "setup.rc2"

extern int ASK_Win32_Init(HINSTANCE hInstance, long iIconId);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   PSTR szCmdLine, int iCmdShow)
{
	int result;
	
	if (0 != ASK_Win32_Init(hInstance, AP_ICON_SETUP))
	{
		return -1;
	}

	if (!ASK_DoScreen_welcome("AbiWord Setup",
							  "\r\nWelcome to the AbiWord setup program.\r\n\r\nThis program will install AbiWord on your computer.  The setup program will guide you at each step of the process.\r\n\r\nYou may cancel the setup at any time by pressing the Cancel button below.",
							  "This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.  You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.\r\n"
		))
	{
		goto cancelled;
	}

	if (g_pReadMeFile)
	{
		char buf[256];
		
		unsigned char* pBytes = ASK_decompressFile(g_pReadMeFile);

		sprintf(buf, "README: %s", g_pReadMeFile->pszFileName);
		
		result = ASK_DoScreen_readme(buf, (char*) pBytes);
		free(pBytes);

		if (!result)
		{
			goto cancelled;
		}
	}
	
	if (g_pLicenseFile)
	{
		unsigned char* pBytes = ASK_decompressFile(g_pLicenseFile);
		
		result = ASK_DoScreen_license((char*) pBytes);
		free(pBytes);

		if (!result)
		{
			goto cancelled;
		}
	}

	{
		int i;
		
		for (i=0; i<g_iNumFileSets; i++)
		{
			result = ASK_DoScreen_chooseDirForFileSet(g_aFileSets[i]);

			if (!result)
			{
				goto cancelled;
			}
		}
	}

	if (!ASK_DoScreen_readyToCopy(g_iNumFileSets, g_aFileSets))
	{
		goto cancelled;
	}

	// TODO note that we currently don't allow the user to select which file sets are optional

	// Fileset 1 (index 0) is the Program Files fileset
	ASK_createRemoveFile("AbiWord", g_aFileSets[ 0 ]->szInstallPath);
	
	{
		int err = ASK_DoScreen_copy(g_iNumFileSets, g_aFileSets);

		if (err != 0)
		{
			MessageBox(NULL, "Installation is not complete", "AbiSetup", MB_OK);
			goto all_done;
		}
	}

	ASK_CreateDesktopShortcuts(g_iNumFileSets, g_aFileSets);

	ASK_PopulateStartMenu("AbiWord", g_iNumFileSets, g_aFileSets);

	ASK_DoScreen_copyComplete(g_iNumFileSets, g_aFileSets);

	// Fileset 1 (index 0) is the Program Files fileset
	ASK_registerForRemove( g_aFileSets[ 0 ]->szInstallPath );
	
	goto all_done;

 cancelled:
	MessageBox(NULL, "Installation was cancelled", "AbiSetup", MB_OK);
	
 all_done:
	return 0;
}
