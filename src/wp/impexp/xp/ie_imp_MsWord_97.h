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


#ifndef IE_IMP_WV_H
#define IE_IMP_WV_H

//#include <stdlib.h>
#include <stdio.h>
#include "ut_xml.h"
#include "ut_vector.h"
#include "ut_stack.h"
#include "ie_imp.h"
#include "ut_bytebuf.h"
class PD_Document;

// The importer/reader for Microsoft Word 97, 95, and 6.0

typedef struct _wvParseStruct wvParseStruct;
typedef struct _Blip Blip;

class IE_Imp_MsWord_97_Sniffer : public IE_ImpSniffer
{
	friend class IE_Imp;

public:
	IE_Imp_MsWord_97_Sniffer() {}

	virtual ~IE_Imp_MsWord_97_Sniffer() {}

	virtual bool recognizeContents (const char * szBuf, 
									UT_uint32 iNumbytes);
	virtual bool recognizeSuffix (const char * szSuffix);
	virtual bool getDlgLabels (const char ** szDesc,
							   const char ** szSuffixList,
							   IEFileType * ft);
	virtual UT_Error constructImporter (PD_Document * pDocument,
										IE_Imp ** ppie);

};

class IE_Imp_MsWord_97 : public IE_Imp
{
public:
	IE_Imp_MsWord_97(PD_Document * pDocument);
	~IE_Imp_MsWord_97();

	UT_Error			importFile(const char * szFilename);
	virtual void	       	pasteFromBuffer(PD_DocumentRange * pDocRange,
										unsigned char * pData, UT_uint32 lenData);

	static bool		RecognizeContents(const char * szBuf, UT_uint32 iNumbytes);
	static bool		RecognizeSuffix(const char * szSuffix);
	static UT_Error		StaticConstructor(PD_Document * pDocument,
										  IE_Imp ** ppie);
	static bool		GetDlgLabels(const char ** pszDesc,
									 const char ** pszSuffixList,
									 IEFileType * ft);
	static bool 		SupportsFileType(IEFileType ft);

   
   	// the callbacks need access to these, so they have to be public
   	int				_charData(UT_UCSChar *, int);
	int 				_docProc(wvParseStruct *ps, UT_uint32 tag);
	int 				_eleProc(wvParseStruct *ps, UT_uint32 tag,void *props,int dirty);
	int                             _fieldProc(wvParseStruct *ps, UT_uint16 eachchar, UT_Byte chartype, UT_uint16 lid);
   	UT_Error _handleImage(Blip *, long, long);
   	

	UT_UCSChar * m_pTextRun;
   	UT_uint16 m_iTextRunLength;
   	UT_uint16 m_iTextRunMaxLength;
   
protected:

	int _handleCommandField(char *command);

   	UT_Error			m_error;
   	int m_iImageCount;
};

#endif /* IE_IMP_WV_H */
