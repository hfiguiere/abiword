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

#include <windows.h>
#include <commctrl.h>   // includes the common control header
#include <crtdbg.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>

#include "ut_debugmsg.h"
#include "xap_Args.h"
#include "ap_Win32Frame.h"
#include "ap_Win32App.h"
#include "sp_spell.h"
#include "ap_Strings.h"
#include "xap_EditMethods.h"
#include "ap_LoadBindings.h"
#include "xap_Menu_ActionSet.h"
#include "xap_Toolbar_ActionSet.h"

#include "ap_Win32Resources.rc2"

#include "ie_types.h"

/*****************************************************************/

AP_Win32App::AP_Win32App(HINSTANCE hInstance, XAP_Args * pArgs, const char * szAppName)
	: XAP_Win32App(hInstance, pArgs,szAppName)
{
	m_prefs = NULL;
	m_pStringSet = NULL;
}

AP_Win32App::~AP_Win32App(void)
{
	SpellCheckCleanup();

	DELETEP(m_prefs);
	DELETEP(m_pStringSet);
}

static UT_Bool s_createDirectoryIfNecessary(const char * szDir)
{
	struct _stat statbuf;
	
	if (_stat(szDir,&statbuf) == 0)								// if it exists
	{
		if ( (statbuf.st_mode & _S_IFDIR) == _S_IFDIR )			// and is a directory
			return UT_TRUE;

		UT_DEBUGMSG(("Pathname [%s] is not a directory.\n",szDir));
		return UT_FALSE;
	}

	if (CreateDirectory(szDir,NULL))
		return UT_TRUE;

	UT_DEBUGMSG(("Could not create Directory [%s].\n",szDir));
	return UT_FALSE;
}	

UT_Bool AP_Win32App::initialize(void)
{
	const char * szUserPrivateDirectory = getUserPrivateDirectory();
	UT_Bool bVerified = s_createDirectoryIfNecessary(szUserPrivateDirectory);
	UT_ASSERT(bVerified);

	// load preferences, first the builtin set and then any on disk.
	
	m_prefs = new AP_Win32Prefs(this);
	m_prefs->loadBuiltinPrefs();
	m_prefs->loadPrefsFile();

	// TODO overlay command line arguments onto preferences...
		   
	// now that preferences are established, let the xap init
		   
	m_pEMC = AP_GetEditMethods();
	UT_ASSERT(m_pEMC);

	m_pBindingSet = new AP_BindingSet(m_pEMC);
	UT_ASSERT(m_pBindingSet);
	
	m_pMenuActionSet = AP_CreateMenuActionSet();
	UT_ASSERT(m_pMenuActionSet);

	m_pToolbarActionSet = AP_CreateToolbarActionSet();
	UT_ASSERT(m_pToolbarActionSet);

	if (! XAP_Win32App::initialize())
		return UT_FALSE;

	// let various window types register themselves

	if (!AP_Win32Frame::RegisterClass(this))
	{
		UT_DEBUGMSG(("couldn't register class\n"));
		return UT_FALSE;
	}

	//////////////////////////////////////////////////////////////////
	// initializes the spell checker.
	//////////////////////////////////////////////////////////////////
	
	{
		const char * szISpellDirectory = NULL;
		getPrefsValueDirectory(AP_PREF_KEY_SpellDirectory,&szISpellDirectory);
		UT_ASSERT((szISpellDirectory) && (*szISpellDirectory));

		const char * szSpellCheckWordList = NULL;
		if ((getPrefsValue(AP_PREF_KEY_SpellCheckWordList,&szSpellCheckWordList)) && (szSpellCheckWordList) && (*szSpellCheckWordList))
			;
		else
			szSpellCheckWordList = AP_PREF_DEFAULT_SpellCheckWordList;
		
		char * szPathname = (char *)calloc(sizeof(char),strlen(szISpellDirectory)+strlen(szSpellCheckWordList)+2);
		UT_ASSERT(szPathname);
		
		sprintf(szPathname,"%s%s%s",
				szISpellDirectory,
				((szISpellDirectory[strlen(szISpellDirectory)-1]=='\\') ? "" : "\\"),
				szSpellCheckWordList);

		UT_DEBUGMSG(("Loading SpellCheckWordList [%s]\n",szPathname));
		SpellCheckInit(szPathname);
		free(szPathname);
		
		// we silently go on if we cannot load it....
	}
	
	//////////////////////////////////////////////////////////////////
	// load the dialog and message box strings
	//////////////////////////////////////////////////////////////////
	
	{
		// assume we will be using the builtin set (either as the main
		// set or as the fallback set).
		
		AP_BuiltinStringSet * pBuiltinStringSet = new AP_BuiltinStringSet(this,AP_PREF_DEFAULT_StringSet);
		UT_ASSERT(pBuiltinStringSet);
		m_pStringSet = pBuiltinStringSet;

		// see if we should load an alternate set from the disk
		
		const char * szDirectory = NULL;
		const char * szStringSet = NULL;

		if (   (getPrefsValue(AP_PREF_KEY_StringSet,&szStringSet))
			&& (szStringSet)
			&& (*szStringSet)
			&& (UT_stricmp(szStringSet,AP_PREF_DEFAULT_StringSet) != 0))
		{
			getPrefsValueDirectory(AP_PREF_KEY_StringSetDirectory,&szDirectory);
			UT_ASSERT((szDirectory) && (*szDirectory));

			char * szPathname = (char *)calloc(sizeof(char),strlen(szDirectory)+strlen(szStringSet)+100);
			UT_ASSERT(szPathname);

			sprintf(szPathname,"%s%s%s.strings",
					szDirectory,
					((szDirectory[strlen(szDirectory)-1]=='\\') ? "" : "\\"),
					szStringSet);

			AP_DiskStringSet * pDiskStringSet = new AP_DiskStringSet(this);
			UT_ASSERT(pDiskStringSet);

			if (pDiskStringSet->loadStringsFromDisk(szPathname))
			{
				pDiskStringSet->setFallbackStringSet(m_pStringSet);
				m_pStringSet = pDiskStringSet;
				UT_DEBUGMSG(("Using StringSet [%s]\n",szPathname));
			}
			else
			{
				DELETEP(pDiskStringSet);
				UT_DEBUGMSG(("Unable to load StringSet [%s] -- using builtin strings instead.\n",szPathname));
			}
				
			free(szPathname);
		}
	}

	//////////////////////////////////////////////////////////////////

	return UT_TRUE;
}

XAP_Frame * AP_Win32App::newFrame(void)
{
	AP_Win32Frame * pWin32Frame = new AP_Win32Frame(this);

	if (pWin32Frame)
		pWin32Frame->initialize();

	return pWin32Frame;
}

UT_Bool AP_Win32App::shutdown(void)
{
	if (m_prefs->getAutoSavePrefs())
		m_prefs->savePrefsFile();

	return UT_TRUE;
}

XAP_Prefs * AP_Win32App::getPrefs(void) const
{
	return m_prefs;
}

UT_Bool AP_Win32App::getPrefsValue(const XML_Char * szKey, const XML_Char ** pszValue) const
{
	if (!m_prefs)
		return UT_FALSE;

	return m_prefs->getPrefsValue(szKey,pszValue);
}

UT_Bool AP_Win32App::getPrefsValueDirectory(const XML_Char * szKey, const XML_Char ** pszValue) const
{
	if (!m_prefs)
		return UT_FALSE;

	const XML_Char * psz = NULL;
	if (!m_prefs->getPrefsValue(szKey,&psz))
		return UT_FALSE;

	if ((*psz == '/') || (*psz == '\\'))
	{
		*pszValue = psz;
		return UT_TRUE;
	}

	static XML_Char buf[1024];
	UT_ASSERT((strlen(getAbiSuiteLibDir()) + strlen(psz) + 2) < sizeof(buf));
	
	sprintf(buf,"%s\\%s",getAbiSuiteLibDir(),psz);
	*pszValue = buf;
	return UT_TRUE;
}

HICON AP_Win32App::getIcon(void)
{

	int sy = GetSystemMetrics(SM_CYICON);
	int sx = GetSystemMetrics(SM_CXICON);
	UT_DEBUGMSG(("GetIcon(): system metrics [%d %d]\n",sx,sy));
	
	if ((sx==32) && (sy==32))
		return LoadIcon(getInstance(), MAKEINTRESOURCE(AP_RID_ICON_APPLICATION_32));
	else
		return LoadImage(getInstance(), MAKEINTRESOURCE(AP_RID_ICON_APPLICATION_32), IMAGE_ICON, 0,0,0);
}

HICON AP_Win32App::getSmallIcon(void)
{

	int sy = GetSystemMetrics(SM_CYICON);
	int sx = GetSystemMetrics(SM_CXICON);
	UT_DEBUGMSG(("GetIcon(): system metrics [%d %d]\n",sx,sy));

	if ((sx==16) && (sy==16))
		return LoadIcon(getInstance(), MAKEINTRESOURCE(AP_RID_ICON_APPLICATION_16));
	else
		return LoadImage(getInstance(), MAKEINTRESOURCE(AP_RID_ICON_APPLICATION_16), IMAGE_ICON, 0,0,0);
}

const XAP_StringSet * AP_Win32App::getStringSet(void) const
{
	return m_pStringSet;
}

/*****************************************************************/

#ifdef   _DEBUG
#define  SET_CRT_DEBUG_FIELD(a) \
            _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a) \
            _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif

/*****************************************************************/

#define SPLASH 1

#if SPLASH
#include "gr_Graphics.h"
#include "gr_Win32Graphics.h"
#include "gr_Image.h"
#include "ut_bytebuf.h"
#include "ut_png.h"

static HWND hwndSplash = NULL;
static GR_Image * pSplash = NULL;
static char s_SplashWndClassName[256];

static void _hideSplash(void)
{
	if (hwndSplash)
	{
		DestroyWindow(hwndSplash);
		hwndSplash = NULL;
	}
	
	DELETEP(pSplash);
}

static LRESULT CALLBACK _SplashWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    
    switch (message) 
	{
    case WM_CREATE:
        // Set the timer for the specified number of ms
        SetTimer(hWnd, 0, 2000, NULL);  
        break;

#if 0		
	// Handle the palette messages in case 
	// another app takes over the palette
    case WM_PALETTECHANGED:
        if ((HWND) wParam == hWnd)
            return 0;
    case WM_QUERYNEWPALETTE:
        InvalidateRect(hWnd, NULL, FALSE);
		UpdateWindow(hWnd);
		return TRUE;
#endif     

    // Destroy the window if... 
    case WM_LBUTTONDOWN:      // ...the user pressed the left mouse button
    case WM_RBUTTONDOWN:      // ...the user pressed the right mouse button
    case WM_TIMER:            // ...the timer timed out
        _hideSplash();		  // Close the window
        break;
        
        // Draw the window
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
		{
			GR_Graphics * pG = new GR_Win32Graphics(hdc, hwndSplash);
			pG->drawImage(pSplash, 0, 0);
			DELETEP(pG);
		}
        EndPaint(hWnd, &ps);
        break;
        
    default:
        return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (0);
}

static GR_Image * _showSplash(HINSTANCE hInstance, XAP_Args * pArgs, const char * szAppName)
{
	hwndSplash = NULL;
	pSplash = NULL;

	UT_ByteBuf* pBB = NULL;
	UT_Bool bShowSplash = UT_TRUE;
	const char * szFile = NULL;

	// Win32 does not put the program name in argv[0], so [0] is the first argument
	int nFirstArg = 0;
	int k;
	
	// scan args for splash-related stuff
	for (k=nFirstArg; (k<pArgs->m_argc); k++)
	{
		if (*pArgs->m_argv[k] == '-')
		{
			if (UT_stricmp(pArgs->m_argv[k],"-nosplash") == 0)
			{
				bShowSplash = UT_FALSE;
				break;
			}
#if DEBUG
			else if (UT_stricmp(pArgs->m_argv[k],"-splash") == 0)
			{
				// [-splash filename]
				szFile = pArgs->m_argv[k+1];
				break;

				// NOTE: this switch is just for debugging artwork, so 
				// it's OK that the filename also gets opened as a document
			}
#endif
		}

		// TODO: platform-specific reasons to not show splash?
		// TODO: for example, if being launched via DDE or OLE??
	}

	if (!bShowSplash)
		goto Done;

	extern unsigned char g_pngSplash[];		// see ap_wp_Splash.cpp
	extern unsigned long g_pngSplash_sizeof;	// see ap_wp_Splash.cpp

	pBB = new UT_ByteBuf();
	if (
		(szFile && szFile[0] && (pBB->insertFromFile(0, szFile)))
		|| (pBB->ins(0, g_pngSplash, g_pngSplash_sizeof))
		)
	{
		// NB: can't access 'this' members from a static member function
		WNDCLASSEX  wndclass;
		ATOM a;
	
		sprintf(s_SplashWndClassName, "%sSplash", szAppName /* app->getApplicationName() */);

		// register class for the splash window
		wndclass.cbSize        = sizeof(wndclass);
		wndclass.style         = 0;
		wndclass.lpfnWndProc   = _SplashWndProc;
		wndclass.cbClsExtra    = 0;
		wndclass.cbWndExtra    = 0;
		wndclass.hInstance     = hInstance /* app->getInstance() */;
		wndclass.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(AP_RID_ICON_APPLICATION_32)) /* app->getIcon() */;
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = GetStockObject(NULL_BRUSH);
		wndclass.lpszMenuName  = NULL;
		wndclass.lpszClassName = s_SplashWndClassName;
		wndclass.hIconSm       = LoadIcon(hInstance, MAKEINTRESOURCE(AP_RID_ICON_APPLICATION_16)) /* app->getSmallIcon() */;

		a = RegisterClassEx(&wndclass);
		UT_ASSERT(a);

		// get the extents of the desktop window
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);

		// get splash size
		UT_sint32 iSplashWidth;
		UT_sint32 iSplashHeight;
		UT_PNG_getDimensions(pBB, iSplashWidth, iSplashHeight);

		// create a centered window the size of our bitmap
		hwndSplash = CreateWindow(s_SplashWndClassName, 
								  NULL, WS_POPUP | WS_BORDER,
								  (rect.right  / 2) - (iSplashWidth  / 2),
								  (rect.bottom / 2) - (iSplashHeight / 2),
								  iSplashWidth,
								  iSplashHeight,
								  NULL, NULL, hInstance, NULL);
		UT_ASSERT(hwndSplash);
    
		if (hwndSplash) 
		{
			// create image first
			GR_Graphics * pG = new GR_Win32Graphics(GetDC(hwndSplash), hwndSplash);
			pSplash = pG->createNewImage("splash", pBB, iSplashWidth, iSplashHeight);
			DELETEP(pG);

			// now bring the window up front & center
			SetWindowPos(hwndSplash, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE); 
			ShowWindow(hwndSplash, SW_SHOWNORMAL);
			UpdateWindow(hwndSplash);
		}
	}

	DELETEP(pBB);

Done:
	return pSplash;
}
#endif

/*****************************************************************/

int AP_Win32App::WinMain(const char * szAppName, HINSTANCE hInstance, 
						 HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	// this is a static function and doesn't have a 'this' pointer.
	MSG msg;

	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG );

	// Ensure that common control DLL is loaded

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;	// load the rebar and toolbar
	InitCommonControlsEx(&icex);

	// initialize our application.

	XAP_Args Args = XAP_Args(szCmdLine);

#if SPLASH
	_showSplash(hInstance, &Args, szAppName);
#endif
	
	AP_Win32App * pMyWin32App = new AP_Win32App(hInstance, &Args, szAppName);
	pMyWin32App->initialize();

	pMyWin32App->ParseCommandLine(iCmdShow);

#if 0 /* SPLASH */
	_hideSplash();
#endif
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// Note: we do not call TranslateMessage() because
		// Note: the keybinding mechanism is responsible
		// Note: for deciding if/when to do this.
			
		DispatchMessage(&msg);
	}
	
	// destroy the App.  It should take care of deleting all frames.

	pMyWin32App->shutdown();
	delete pMyWin32App;

	SET_CRT_DEBUG_FIELD( _CRTDBG_LEAK_CHECK_DF );

	return msg.wParam;
}

void AP_Win32App::ParseCommandLine(int iCmdShow)
{
	// parse the command line
	// <app> [-script <scriptname>]* [-dumpstrings] [-lib <AbiSuiteLibDirectory>] [<documentname>]*
	
	// TODO when we refactor the App classes, consider moving
	// TODO this to app-specific, cross-platform.

	// TODO replace this with getopt or something similar.
	
	// Win32 does not put the program name in argv[0], so [0] is the first argument.

	int nFirstArg = 0;
	int k;
	int kWindowsOpened = 0;
	
	for (k=nFirstArg; (k<m_pArgs->m_argc); k++)
	{
		if (*m_pArgs->m_argv[k] == '-')
		{
			if (UT_stricmp(m_pArgs->m_argv[k],"-script") == 0)
			{
				// [-script scriptname]
				k++;
			}
			else if (UT_stricmp(m_pArgs->m_argv[k],"-lib") == 0)
			{
				// [-lib <AbiSuiteLibDirectory>]
				// we've already processed this when we initialized the App class
				k++;
			}
			else if (UT_stricmp(m_pArgs->m_argv[k],"-dumpstrings") == 0)
			{
				// [-dumpstrings]
#ifdef DEBUG
				// dump the string table in english as a template for translators.
				// see abi/docs/AbiSource_Localization.abw for details.
				AP_BuiltinStringSet * pBuiltinStringSet = new AP_BuiltinStringSet(this,AP_PREF_DEFAULT_StringSet);
				pBuiltinStringSet->dumpBuiltinSet("EnUS.strings");
				delete pBuiltinStringSet;
#endif
			}
			else
			{
				UT_DEBUGMSG(("Unknown command line option [%s]\n",m_pArgs->m_argv[k]));
				// TODO don't know if it has a following argument or not -- assume not
			}
		}
		else
		{
			// [filename]
			
			AP_Win32Frame * pFirstWin32Frame = new AP_Win32Frame(this);
			pFirstWin32Frame->initialize();
			if (pFirstWin32Frame->loadDocument(m_pArgs->m_argv[k], IEFT_Unknown))
			{
				kWindowsOpened++;

				HWND hwnd = pFirstWin32Frame->getTopLevelWindow();
				ShowWindow(hwnd, iCmdShow);
				UpdateWindow(hwnd);
			}
			else
			{
				// TODO: warn user that we couldn't open that file

#if 1
				// TODO we crash if we just delete this without putting something
				// TODO in it, so let's go ahead and open an untitled document
				// TODO for now.  this would cause us to get 2 untitled documents
				// TODO if the user gave us 2 bogus pathnames....
				kWindowsOpened++;
				pFirstWin32Frame->loadDocument(NULL, IEFT_Unknown);
				HWND hwnd = pFirstWin32Frame->getTopLevelWindow();
				ShowWindow(hwnd, iCmdShow);
				UpdateWindow(hwnd);
#else
				delete pFirstWin32Frame;
#endif
			}
		}
	}

	if (kWindowsOpened == 0)
	{
		// no documents specified or were able to be opened, open an untitled one

		AP_Win32Frame * pFirstWin32Frame = new AP_Win32Frame(this);
		pFirstWin32Frame->initialize();

		pFirstWin32Frame->loadDocument(NULL, IEFT_Unknown);

		HWND hwnd = pFirstWin32Frame->getTopLevelWindow();
		ShowWindow(hwnd, iCmdShow);
		UpdateWindow(hwnd);
	}

	return;
}
