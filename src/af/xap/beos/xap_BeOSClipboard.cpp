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

#include <string.h>

#include "ut_string.h"
#include "ut_assert.h"

#include "xap_BeOSClipboard.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

XAP_BeOSClipboard::XAP_BeOSClipboard()
	: XAP_FakeClipboard()
{
}

XAP_BeOSClipboard::~XAP_BeOSClipboard()
{
	clear();
}

UT_Bool XAP_BeOSClipboard::open(void)
{
	return XAP_FakeClipboard::open();
}

UT_Bool XAP_BeOSClipboard::close(void)
{
	return XAP_FakeClipboard::close();
}

UT_Bool XAP_BeOSClipboard::addData(const char* format, void* pData, UT_sint32 iNumBytes)
{
	return XAP_FakeClipboard::addData(format, pData, iNumBytes);
}

UT_Bool XAP_BeOSClipboard::hasFormat(const char* format)
{
	return XAP_FakeClipboard::hasFormat(format);
}

UT_sint32 XAP_BeOSClipboard::getDataLen(const char * format)
{
	return XAP_FakeClipboard::getDataLen(format);
}

UT_Bool XAP_BeOSClipboard::getData(const char * format, void* pData)
{
	return XAP_FakeClipboard::getData(format, pData);
}

UT_sint32 XAP_BeOSClipboard::countFormats(void)
{
	return XAP_FakeClipboard::countFormats();
}

const char * XAP_BeOSClipboard::getNthFormat(UT_sint32 n)
{
	return XAP_FakeClipboard::getNthFormat(n);
}

UT_Bool XAP_BeOSClipboard::clear(void)
{
	return XAP_FakeClipboard::clear();
}

GR_Image * XAP_BeOSClipboard::getImage(void)
{
	return XAP_FakeClipboard::getImage();
}

UT_Bool XAP_BeOSClipboard::addImage(GR_Image*)
{
	return XAP_FakeClipboard::addImage(NULL);
}

