/* AbiSource Application Framework
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

#ifndef AP_WIN32DIALOG_ABOUT_H
#define AP_WIN32DIALOG_ABOUT_H

#include "ap_Dialog_About.h"

/*****************************************************************/

class AP_Win32Dialog_About: public AP_Dialog_About
{
public:
	AP_Win32Dialog_About(AP_DialogFactory * pDlgFactory, AP_Dialog_Id id);
	virtual ~AP_Win32Dialog_About(void);

	static AP_Dialog *		static_constructor(AP_DialogFactory *, AP_Dialog_Id id);

	virtual void			runModal(XAP_Frame * pFrame);

 protected:
	
};

#endif /* AP_WIN32DIALOG_ABOUT_H */
