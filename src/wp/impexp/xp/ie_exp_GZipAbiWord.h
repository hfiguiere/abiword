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


#ifndef IE_EXP_GZIPABIWORD_1_H
#define IE_EXP_GZIPABIWORD_1_H

#include <zlib.h>
#include "ie_exp_AbiWord_1.h"

// The exporter/writer for GZipped AbiWord file format.

class IE_Exp_GZipAbiWord : public IE_Exp_AbiWord_1
{
public:
	IE_Exp_GZipAbiWord(PD_Document * pDocument);
	virtual ~IE_Exp_GZipAbiWord();

	static UT_Bool		RecognizeSuffix(const char * szSuffix);
	static IEStatus		StaticConstructor(PD_Document * pDocument,
										  IE_Exp ** ppie);
	static UT_Bool		GetDlgLabels(const char ** pszDesc,
									 const char ** pszSuffixList,
									 IEFileType * ft);
	static UT_Bool 		SupportsFileType(IEFileType ft);
	
protected:
	UT_Bool				_openFile(const char * szFilename);
	UT_uint32			_writeBytes(const UT_Byte * pBytes, UT_uint32 length);
	UT_Bool				_writeBytes(const UT_Byte * sz);
	UT_Bool				_closeFile(void);

private:
    
    gzFile m_gzfp;

};

#endif /* IE_EXP_GZIPABIWORD_1_H */
