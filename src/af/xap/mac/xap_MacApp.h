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


#ifndef AP_MACAPP_H
#define AP_MACAPP_H

#include "xap_App.h"
#include "xap_MacDialogFactory.h"
#include "xap_MacTlbr_ControlFactory.h"
class AP_Args;
class AP_MacToolbar_Icons;

/*****************************************************************
******************************************************************
** This file defines the Mac-platform-specific class for the
** cross-platform application.  This is used to hold all of the
** platform-specific, application-specific data.  Only one of these
** is created by the application.
******************************************************************
*****************************************************************/

class AP_MacApp : public AP_App
{
public:
	AP_MacApp(AP_Args * pArgs, const char * szAppName);
	virtual ~AP_MacApp(void);

	virtual UT_Bool			initialize(void);
	virtual XAP_Frame *		newFrame(void);
	virtual void			reallyExit(void);

	virtual AP_DialogFactory *				getDialogFactory(void);
	virtual AP_Toolbar_ControlFactory *		getControlFactory(void);

	static int MacMain (const char * szAppName, int argc, char **argv);

protected:
	UT_uint32	_getExeDir(char* pDirBuf, UT_uint32 iBufLen);
	
	AP_MacToolbar_Icons *		m_pMacToolbarIcons;
	AP_MacDialogFactory			m_dialogFactory;
	AP_MacToolbar_ControlFactory	m_controlFactory;
};

#endif /* AP_MACAPP_H */
