 
/*
** The contents of this file are subject to the AbiSource Public
** License Version 1.0 (the "License"); you may not use this file
** except in compliance with the License. You may obtain a copy
** of the License at http://www.abisource.com/LICENSE/ 
** 
** Software distributed under the License is distributed on an
** "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
** implied. See the License for the specific language governing
** rights and limitations under the License. 
** 
** The Original Code is AbiWord.
** 
** The Initial Developer of the Original Code is AbiSource, Inc.
** Portions created by AbiSource, Inc. are Copyright (C) 1998 AbiSource, Inc. 
** All Rights Reserved. 
** 
** Contributor(s):
**  
*/

#ifndef PD_DOCUMENT_H
#define PD_DOCUMENT_H

// TODO should the filename be UT_UCSChar rather than char ?

#include <stdio.h>
#include "ut_types.h"
#include "ut_vector.h"
#include "xmlparse.h"
#include "pt_Types.h"
#include "pl_Listener.h"
#include "ie_types.h"

class UT_GrowBuf;
class pt_PieceTable;
class PP_AttrProp;
class pf_Frag_Strux;
class PX_ChangeRecord;

// PD_Document is the representation for a document.

class PD_Document
{
public:
	PD_Document();
	~PD_Document();

	UT_Bool					readFromFile(const char * szFilename);
	UT_Bool					saveAs(const char * szFilename, IEFileType ieft);
	UT_Bool					save(IEFileType ieft);
	UT_Bool					newDocument(void);
	const char *			getFilename(void) const;
	UT_Bool					isDirty(void) const;
	void					setClean(void);

	void					beginUserAtomicGlob(void);
	void					endUserAtomicGlob(void);
	UT_Bool					undoCmd(UT_uint32 repeatCount);
	UT_Bool					redoCmd(UT_uint32 repeatCount);
	
	UT_Bool					insertSpan(PT_DocPosition dpos,
									   UT_UCSChar * p,
									   UT_uint32 length);
	UT_Bool					deleteSpan(PT_DocPosition dpos1,
									   PT_DocPosition dpos2);

	UT_Bool					changeSpanFmt(PTChangeFmt ptc,
										  PT_DocPosition dpos1,
										  PT_DocPosition dpos2,
										  const XML_Char ** attributes,
										  const XML_Char ** properties);

	UT_Bool					insertStrux(PT_DocPosition dpos,
										PTStruxType pts);

	UT_Bool					changeStruxFmt(PTChangeFmt ptc,
										   PT_DocPosition dpos1,
										   PT_DocPosition dpos2,
										   const XML_Char ** attributes,
										   const XML_Char ** properties,
										   PTStruxType pts);

	// the append- methods are only available while importing
	// the document.

	UT_Bool					appendStrux(PTStruxType pts, const XML_Char ** attributes);
	UT_Bool					appendFmt(const XML_Char ** attributes);
	UT_Bool					appendFmt(const UT_Vector * pVecAttributes);
	UT_Bool					appendSpan(UT_UCSChar * p, UT_uint32 length);

	UT_Bool					addListener(PL_Listener * pListener, PL_ListenerId * pListenerId);
	UT_Bool					removeListener(PL_ListenerId listenerId);
	UT_Bool					notifyListeners(pf_Frag_Strux * pfs, const PX_ChangeRecord * pcr) const;
	UT_Bool					notifyListeners(pf_Frag_Strux * pfs,
											pf_Frag_Strux * pfsNew,
											const PX_ChangeRecord * pcr) const;

	UT_Bool					getAttrProp(PT_AttrPropIndex indexAP, const PP_AttrProp ** ppAP) const;
	UT_Bool					getSpanAttrProp(PL_StruxDocHandle sdh, UT_uint32 offset,
											const PP_AttrProp ** ppAP) const;
	const UT_UCSChar *		getPointer(PT_BufIndex bi) const; /* see warning on this function */
	UT_Bool					getSpanPtr(PL_StruxDocHandle sdh, UT_uint32 offset,
									   const UT_UCSChar ** ppSpan, UT_uint32 * pLength) const;
	UT_Bool					getBlockBuf(PL_StruxDocHandle sdh, UT_GrowBuf * pgb) const;

	UT_Bool					getBounds(UT_Bool bEnd, PT_DocPosition & docPos) const;
	PT_DocPosition			getStruxPosition(PL_StruxDocHandle sdh) const;
	UT_Bool					getStruxFromPosition(PL_ListenerId listenerId,
												 PT_DocPosition docPos,
												 PL_StruxFmtHandle * psfh) const;
	UT_Bool					getStruxOfTypeFromPosition(PL_ListenerId listenerId,
													   PT_DocPosition docPos,
													   PTStruxType pts,
													   PL_StruxFmtHandle * psfh) const;

	void					clearTemporarySpanFmt(void);

	void					dump(FILE * fp) const;
	
protected:
	const char *			m_szFilename;
	UT_Bool					m_bDirty;
	pt_PieceTable *			m_pPieceTable;
	UT_Vector				m_vecListeners;
};


#endif /* PD_DOCUMENT_H */
