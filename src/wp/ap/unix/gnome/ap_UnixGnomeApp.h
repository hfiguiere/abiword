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
******************************************************************
** Only one of these is created by the application.
******************************************************************
*****************************************************************/

#ifndef AP_UNIXGNOMEAPP_H
#define AP_UNIXGNOMEAPP_H

#include "ap_UnixApp.h"

class XAP_Args;

class AP_UnixGnomeApp : public AP_UnixApp
{
public:
	AP_UnixGnomeApp(XAP_Args * pArgs, const char * szAppName);
	virtual ~AP_UnixGnomeApp();

	virtual void initPopt (AP_Args *);

	static int				main (const char * szAppName, int argc, const char ** argv);
};

#endif /* AP_UNIXGNOMEAPP_H */
