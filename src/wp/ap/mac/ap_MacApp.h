/* AbiWord
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

/*****************************************************************
******************************************************************
** Only one of these is created by the application.
******************************************************************
*****************************************************************/

#ifndef AP_MACAPP_H
#define AP_MACAPP_H

#include "xap_Args.h"
#include "xap_MacApp.h"
#include "ap_MacPrefs.h"

class AP_MacApp : public XAP_MacApp
{
public:
	AP_MacApp(AP_Args * pArgs, const char * szAppName);
	virtual ~AP_MacApp(void);

	virtual UT_Bool			initialize(void);
	virtual UT_Bool			shutdown(void);
	virtual UT_Bool			getPrefsValue(const XML_Char * szKey, const XML_Char ** pszValue) const;

	static int MacMain (const char * szAppName, int argc, char **argv);

protected:
	AP_MacPrefs *			m_prefs;
};

#endif /* XAP_MACAPP_H */
