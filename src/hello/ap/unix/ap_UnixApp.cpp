/* AbiHello
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

#ifdef ABI_OPT_JS
#include <EXTERN.h>
#include <perl.h>
#endif /* ABI_OPT_JS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ut_debugmsg.h"
#include "ut_types.h"
#include "ut_string.h"
#include "xap_App.h"
#include "xap_Args.h"
#include "ap_UnixFrame.h"
#include "ap_UnixApp.h"
#include "xap_EditMethods.h"
#include "ap_LoadBindings.h"
#include "xap_Menu_ActionSet.h"
#include "xap_Toolbar_ActionSet.h"

#define DELETEP(p)      do { if (p) delete(p); (p)=NULL; } while (0)

PerlInterpreter *AP_UnixApp::m_pPerlInstance = 0;

static bool s_createDirectoryIfNecessary(const char * szDir);
#ifdef ABI_OPT_JS
static void s_perlEvalFile(PerlInterpreter *my_perl, const char *filename);
#endif

AP_UnixApp::AP_UnixApp(XAP_Args* pArgs, const char* szAppName)
	: XAP_UnixApp(pArgs, szAppName)
{
	m_pStringSet = NULL;
}

AP_UnixApp::~AP_UnixApp()
{
	DELETEP(m_pStringSet);
#ifdef ABI_OPT_JS
	if (m_pPerlInstance != 0)
	{
		perl_destruct(m_pPerlInstance);
		perl_free(m_pPerlInstance);
	}
#endif
}

static bool s_createDirectoryIfNecessary(const char * szDir)
{
	struct stat statbuf;

	if (stat(szDir,&statbuf) == 0)
		// if it exists
	{
		if (S_ISDIR(statbuf.st_mode))
			// and is a directory
			return true;
		
		UT_DEBUGMSG(("Pathname [%s] is not a directory.\n",szDir));
		return false;
	}
	
	if (mkdir(szDir,0700) == 0)
		return true;
	
	
	UT_DEBUGMSG(("Could not create Directory [%s].\n",szDir));
	return false;
}

#ifdef ABI_OPT_JS
static void s_perlEvalFile(PerlInterpreter *my_perl, const char *filename)
{
	char *argv[] = { "", filename };
	int argc = 2;
	perl_parse(my_perl, NULL, argc, argv, (char **)NULL);
	perl_run(my_perl);
}
#endif

bool AP_UnixApp::initialize(void)
{
	// getUserPrivateDirectory() is in xap/unix/xap_UnixApp.cpp
	const char * szUserPrivateDirectory = getUserPrivateDirectory();
	bool bVerified = s_createDirectoryIfNecessary(szUserPrivateDirectory);

	UT_ASSERT(bVerified);

	m_prefs = new AP_UnixPrefs(this);
	m_prefs->loadBuiltinPrefs();
	m_prefs->loadPrefsFile();

		   
	m_pEMC = AP_GetEditMethods();
	UT_ASSERT(m_pEMC);

	m_pBindingSet = new AP_BindingSet(m_pEMC);
	UT_ASSERT(m_pBindingSet);
	
	m_pMenuActionSet = AP_CreateMenuActionSet();
	UT_ASSERT(m_pMenuActionSet);

	m_pToolbarActionSet = AP_CreateToolbarActionSet();
	UT_ASSERT(m_pToolbarActionSet);

	if (!XAP_UnixApp::initialize())
		return false;

	// TODO  - load in strings

	return true;
}

XAP_Frame* AP_UnixApp::newFrame(void)
{
	AP_UnixFrame* pUnixFrame = new AP_UnixFrame(this);

	if (pUnixFrame)
		pUnixFrame->initialize();

	return pUnixFrame;
}

bool AP_UnixApp::shutdown(void)
{
	if (m_prefs && m_prefs->getAutoSavePrefs())
		m_prefs->savePrefsFile();

	return true;
}



const XAP_StringSet* AP_UnixApp::getStringSet(void) const
{
	return m_pStringSet;
}
	
int AP_UnixApp::main(const char* szAppName, int argc, char** argv)
{
	// TODO These printfs are not here permanently.  remove them later.
	
	printf("Build ID:\t%s\n", XAP_App::s_szBuild_ID);
	printf("Version:\t%s\n", XAP_App::s_szBuild_Version);
	printf("Build Options: \t%s\n", XAP_App::s_szBuild_Options);
	printf("Build Target: \t%s\n", XAP_App::s_szBuild_Target);
	printf("Compile Date:\t%s\n", XAP_App::s_szBuild_CompileDate);
	printf("Compile Time:\t%s\n", XAP_App::s_szBuild_CompileTime);
	
	printf("\n");
	
	// initialize our application.
	
	XAP_Args Args = XAP_Args(argc,argv);
	
	AP_UnixApp* pMyUnixApp = new AP_UnixApp(&Args, szAppName);

	// if the initialize fails, we don't have icons, fonts, etc.
	if (!pMyUnixApp->initialize())
	{
		delete pMyUnixApp;
		return -1;      // make this something standard?
	}

	// create the first window.
	AP_UnixFrame* pFirstUnixFrame = new AP_UnixFrame(pMyUnixApp);
	pFirstUnixFrame->initialize();

	int i;

	for (i=1; i<argc; i++)
	{
		if (0 == strcmp(argv[i], "-script"))
		{
			i++;
			
#ifdef ABI_OPT_JS
			s_perlEvalFile(pMyUnixApp->getPerlInterp(), argv[i]);
//			js_eval_file(pMyUnixApp->getInterp(), argv[i]);
#endif /* ABI_OPT_JS */
		}
		else
		{
			break;
		}
	}

	// let gtk have at it
	gtk_main();

	pMyUnixApp->shutdown();
	DELETEP(pMyUnixApp);

	return 0;
}

const char * AP_UnixApp::getAbiSuiteAppDir(void) const
{
}

void AP_UnixApp::copyToClipboard(PD_DocumentRange * pDocRange){}


void AP_UnixApp::pasteFromClipboard(PD_DocumentRange * pDocRange, bool bUseClipboard)
{
}

bool	AP_UnixApp::canPasteFromClipboard(void){}
	
bool	AP_UnixApp::parseCommandLine(void){}

void AP_UnixApp::setSelectionStatus(AV_View * pView){}
void AP_UnixApp::clearSelection(void){}
bool	AP_UnixApp::getCurrentSelection(const char** formatList,
					void ** ppData, UT_uint32 * pLen,
					const char **pszFormatFound){}
void AP_UnixApp::cacheCurrentSelection(AV_View *){}

#ifdef ABI_OPT_JS
PerlInterpreter *AP_UnixApp::getPerlInterp()
{
	if (m_pPerlInstance == 0)
	{
		m_pPerlInstance = perl_alloc();
		perl_construct(m_pPerlInstance);
	}

	return m_pPerlInstance;
}
#endif
