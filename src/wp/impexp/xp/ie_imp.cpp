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


#include <string.h>

#include "ut_string.h"
#include "ut_vector.h"
#include "ut_assert.h"
#include "ut_misc.h"

#include "ie_imp.h"
#include "ie_imp_AbiWord_1.h"
#include "ie_imp_GraphicAsDocument.h"
#include "pd_Document.h"

#define IEFT_Text ((IEFileType)(IE_Imp::fileTypeForSuffix(".txt")))

static UT_Vector m_sniffers (20);

/*****************************************************************/
/*****************************************************************/

IE_Imp::IE_Imp(PD_Document * pDocument)
	: m_pDocument(pDocument)
{
}

IE_Imp::~IE_Imp()
{
}
	
/*****************************************************************/
/*****************************************************************/

IE_ImpSniffer::IE_ImpSniffer()
	: m_type (IEFT_Bogus)
{
}

IE_ImpSniffer::~IE_ImpSniffer()
{
}

/*****************************************************************/
/*****************************************************************/

void IE_Imp::registerImporter (IE_ImpSniffer * s)
{
	UT_uint32 ndx = 0;
	UT_Error err = m_sniffers.addItem (s, &ndx);

	UT_ASSERT(err == UT_OK);
	UT_ASSERT(ndx >= 0);

	s->setFileType(ndx+1);
}

void IE_Imp::unregisterImporter (IE_ImpSniffer * s)
{
	UT_uint32 ndx = 0;

	ndx = s->getFileType(); // 1:1 mapping

	UT_ASSERT(ndx >= 0);

	m_sniffers.deleteNthItem (ndx-1);
}

/*****************************************************************/
/*****************************************************************/

IEFileType IE_Imp::fileTypeForContents(const char * szBuf, UT_uint32 iNumbytes)
{
	// we have to construct the loop this way because a
	// given filter could support more than one file type,
	// so we must query a match for all file types
	UT_uint32 nrElements = getImporterCount();

	for (UT_uint32 k=0; k < nrElements; k++)
	{
		IE_ImpSniffer * s = (IE_ImpSniffer *)m_sniffers.getNthItem (k);
		if (s->recognizeContents(szBuf, iNumbytes))
		{
			for (UT_sint32 a = 0; a < (int) nrElements; a++)
			{
				if (s->supportsFileType((IEFileType) (a+1)))
					return (IEFileType) (a+1);
			}

			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			// Hm... an importer recognizes the given data
			// but refuses to support any file type we request.
			return IEFT_Unknown;
		}
	}

	// No filter recognizes this data
	return IEFT_Unknown;
	
}

IEFileType IE_Imp::fileTypeForSuffix(const char * szSuffix)
{
	if (!szSuffix)
		return IEFT_Unknown;
	
	// we have to construct the loop this way because a
	// given filter could support more than one file type,
	// so we must query a suffix match for all file types
	UT_uint32 nrElements = getImporterCount();

	for (UT_uint32 k=0; k < nrElements; k++)
	{
		IE_ImpSniffer * s = (IE_ImpSniffer *)m_sniffers.getNthItem (k);
		if (s->recognizeSuffix(szSuffix))
		{
			for (UT_sint32 a = 0; a < (int) nrElements; a++)
			{
				if (s->supportsFileType((IEFileType) (a+1)))
					return (IEFileType) (a+1);
			}

			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			// Hm... an importer has registered for the given suffix,
			// but refuses to support any file type we request.
			return IEFT_Unknown;
		}
	}

	// No filter is registered for that extension
	return IEFT_Unknown;
	
}

UT_Error IE_Imp::constructImporter(PD_Document * pDocument,
								   const char * szFilename,
								   IEFileType ieft,
								   IE_Imp ** ppie,
								   IEFileType * pieft)
{
	// construct an importer of the right type.
	// caller is responsible for deleting the importer object
	// when finished with it.
	
	UT_ASSERT(pDocument);
	UT_ASSERT(szFilename && *szFilename);
	UT_ASSERT(ppie);

	// no filter will support IEFT_Unknown, so we try to detect
	// from the contents of the file or the filename suffix
	// the importer to use and assign that back to ieft.
	// Give precedence to the file suffix.
	if (ieft == IEFT_Unknown)
	{
		ieft = IE_Imp::fileTypeForSuffix(UT_pathSuffix(szFilename));
	}
	if (ieft == IEFT_Unknown)
	{
		char szBuf[4096];  // 4096 ought to be enough
		int iNumbytes;
		FILE *f;
		if ( ( f = fopen( szFilename, "r" ) ) != (FILE *)0 )
		{
			iNumbytes = fread(szBuf, 1, sizeof(szBuf), f);
			fclose(f);
			ieft = IE_Imp::fileTypeForContents(szBuf, iNumbytes);
		}
	}
	if (ieft == IEFT_Unknown)
	{
	   	// maybe they're trying to open an image directly?
	   	IE_ImpGraphic *pIEG;
 		UT_Error errorCode = IE_ImpGraphic::constructImporter(szFilename, IEGFT_Unknown, &pIEG);
		if (!errorCode && pIEG) 
 		{
			// tell the caller the type of importer they got
		   	if (pieft != NULL) *pieft = IEFT_Unknown; // to force a save-as

		   	// create the importer 
			*ppie = new IE_Imp_GraphicAsDocument(pDocument);
		   	if (*ppie) {
			   	// tell the importer where to get the graphic
			   	((IE_Imp_GraphicAsDocument*)(*ppie))->setGraphicImporter(pIEG);
			   	return UT_OK;
			} else {
			   	delete pIEG;
				return UT_IE_NOMEMORY;
			}
		}
		else
 		{
	   		// as a last resort, just try importing it as text  :(
			ieft = IEFT_Text ;
		}
	}

	UT_ASSERT(ieft != IEFT_Unknown);

	// tell the caller the type of importer they got
	if (pieft != NULL) 
		*pieft = ieft;

	// use the importer for the specified file type
	UT_uint32 nrElements = getImporterCount();

	for (UT_uint32 k=0; k < nrElements; k++)
	{
		IE_ImpSniffer * s = (IE_ImpSniffer *)m_sniffers.getNthItem (k);
		if (s->supportsFileType(ieft))
			return s->constructImporter(pDocument,ppie);
	}

	// if we got here, no registered importer handles the
	// type of file we're supposed to be reading.
	// assume it is our format and try to read it.
	// if that fails, just give up.
	*ppie = new IE_Imp_AbiWord_1(pDocument);
	return ((*ppie) ? UT_OK : UT_IE_NOMEMORY);
}

bool IE_Imp::enumerateDlgLabels(UT_uint32 ndx,
								const char ** pszDesc,
								const char ** pszSuffixList,
								IEFileType * ft)
{
	UT_uint32 nrElements = getImporterCount();
	if (ndx < nrElements)
	{
		IE_ImpSniffer * s = (IE_ImpSniffer *) m_sniffers.getNthItem (ndx);
		return s->getDlgLabels(pszDesc,pszSuffixList,ft);
	}

	return false;
}

UT_uint32 IE_Imp::getImporterCount(void)
{
	return m_sniffers.size();
}
