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

#include "ap_MacFrame.h"

AP_MacFrame::AP_MacFrame(XAP_MacApp * app)
	: XAP_MacFrame(app)
{
}

AP_MacFrame::AP_MacFrame(AP_MacFrame * f)
	: XAP_MacFrame(static_cast<XAP_MacFrame *>(f))
{
}

AP_MacFrame::~AP_MacFrame(void)
{
}

XAP_Frame *	AP_MacFrame::cloneFrame(void)
{
	return 0;
}

UT_Bool	AP_MacFrame::loadDocument(const char * szFilename)
{
	return UT_TRUE;
}

UT_Bool	AP_MacFrame::close(void)
{
	return UT_TRUE;
}

UT_Bool	AP_MacFrame::raise(void)
{
	return UT_TRUE;
}

UT_Bool	AP_MacFrame::show(void)
{
	return UT_TRUE;
}

XAP_DialogFactory *AP_MacFrame::getDialogFactory(void)
{
	return 0;
}

void AP_MacFrame::setXScrollRange(void)
{
}

void AP_MacFrame::setYScrollRange(void)
{
}
