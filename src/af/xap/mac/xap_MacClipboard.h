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

#ifndef XAP_MACCLIPBOARD_H
#define XAP_MACCLIPBOARD_H

#include "ut_types.h"

#include "xap_Clipboard.h"

class AP_MacClipboard : public AP_Clipboard
{
public:
	AP_MacClipboard();
	
	virtual UT_Bool		open(void);
	virtual UT_Bool		close(void);
	virtual UT_Bool		addData(char* format, void* pData, UT_sint32 iNumBytes);
	virtual UT_sint32	getDataLen(char* format);
	virtual UT_Bool		getData(char* format, void* pData);
	virtual UT_Bool		hasFormat(char* format);
	virtual UT_sint32	countFormats(void);
	virtual char*		getNthFormat(UT_sint32 n);
	virtual UT_Bool		clear(void);

	virtual GR_Image*	getImage(void);
	virtual UT_Bool		addImage(GR_Image*);

protected:
	UT_uint32	_convertFormatString(char* format);
	
	UT_uint32			m_cfRTF;
};

#endif /* XAP_MACCLIPBOARD_H */

