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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ut_misc.h"
#include "xap_BeOSDlg_FontChooser.h"
#include "xap_BeOSApp.h"
#include "xap_BeOSFrame.h"

/*****************************************************************/
AP_Dialog * XAP_BeOSDialog_FontChooser::static_constructor(AP_DialogFactory * pFactory,
														 AP_Dialog_Id id)
{
	XAP_BeOSDialog_FontChooser * p = new XAP_BeOSDialog_FontChooser(pFactory,id);
	return p;
}

XAP_BeOSDialog_FontChooser::XAP_BeOSDialog_FontChooser(AP_DialogFactory * pDlgFactory,
												   AP_Dialog_Id id)
	: XAP_Dialog_FontChooser(pDlgFactory,id)
{
	bAbusingTheFontSize = UT_FALSE;
}

XAP_BeOSDialog_FontChooser::~XAP_BeOSDialog_FontChooser(void)
{
}

void XAP_BeOSDialog_FontChooser::runModal(XAP_Frame * pFrame)
{
}
