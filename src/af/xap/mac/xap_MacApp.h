/* AbiSource Application Framework
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 1999 John Brewer DBA Jera Design
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


#ifndef XAP_MACAPP_H
#define XAP_MACAPP_H

#include <Events.h>

#include "xap_App.h"
#include "xap_MacDialogFactory.h"
#include "xap_MacTlbr_ControlFactory.h"


class XAP_Args;
class XAP_MacFontManager;
class XAP_MacToolbar_Control;
class AP_MacToolbar_Icons;

/*****************************************************************
******************************************************************
** Only one of these is created by the application.
******************************************************************
*****************************************************************/

class XAP_MacApp : public XAP_App
{
public:
	XAP_MacApp(XAP_Args * pArgs, const char * szAppName);
	virtual ~XAP_MacApp(void);

	virtual bool			initialize(void);
	virtual XAP_Frame *		newFrame(void) = 0;
	virtual void			reallyExit(void);

	virtual XAP_DialogFactory *				getDialogFactory(void);
	virtual XAP_Toolbar_ControlFactory *	getControlFactory(void);
	virtual const char *					getUserPrivateDirectory(void);
	XAP_MacFontManager * 					getFontManager (void)
											{return m_pMacFontManager; };
        virtual UT_sint32					makeDirectory(const char * szPath, const UT_sint32 mode ) const;

   	void run ();
   	void terminate ()		{ m_finished = true; };		// simply set the finished flag to true

	XAP_MacToolbar_Control * getToolbarControl () { return m_pToolbarControl; };
	static bool m_NotInitialized;

/* 
   WARNING: ugly hack because MrCPP is really broken. I can't imagine how people can work 
   with this compiler. I really wish gcc get ported to MPW 
*/
#ifndef __MRC__
protected:
#endif
	static void InitializeMacToolbox ();

#ifdef __MRC__
protected:
#endif
	/* Mac Event handling */
#if defined(USE_CARBON_EVENTS)
	static pascal OSStatus HandleCarbonMenus (EventHandlerCallRef nextHandler, 
	                                          EventRef theEvent, void* userData);
#else
	void DispatchEvent (const EventRecord & theEvent);
	void HandleMenus (long menuSelection);
#endif

	bool		_loadFonts (void);
	UT_uint32	_getExeDir(char* pDirBuf, UT_uint32 iBufLen);
	bool					m_finished;
	XAP_MacFontManager *		m_pMacFontManager;
	XAP_MacToolbar_Control *	m_pToolbarControl;
	
	AP_MacToolbar_Icons *		m_pMacToolbarIcons;
	AP_MacDialogFactory			m_dialogFactory;
	AP_MacToolbar_ControlFactory	m_controlFactory;
};

#endif /* XAP_MACAPP_H */
