 
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

#ifndef PF_FRAG_H
#define PF_FRAG_H

#include <stdio.h>
#include "ut_types.h"
#include "pt_Types.h"
class pt_PieceTable;
class PX_ChangeRecord;


// pf_Frag represents a fragment of the document.  This may
// be text, an inline object (such as an image), or structure
// information (such as a paragraph or section).
//
// pf_Frag is an abstract base class.
// We use an enum to remember type, rather than use any of the
// run-time stuff.

class pf_Frag
{
public:
	typedef enum _PFType { PFT_Text, PFT_Object, PFT_Strux, PFT_EndOfDoc } PFType;

	pf_Frag(pt_PieceTable * pPT, PFType type, UT_uint32 length);
	virtual ~pf_Frag();

	PFType					getType(void) const;
	pf_Frag *				getNext(void) const;
	pf_Frag *				getPrev(void) const;
	pf_Frag *				setNext(pf_Frag * pNext);
	pf_Frag *				setPrev(pf_Frag * pPrev);

	UT_uint32				getLength(void) const;
	
	// createSpecialChangeRecord() constructs a change
	// record which describes the fragment itself and
	// not an actual change (editing) operation.  the
	// is used to initialize the listeners.
	
	virtual UT_Bool			createSpecialChangeRecord(PX_ChangeRecord ** ppcr,
													  PT_DocPosition dpos) const;
	
	virtual void			dump(FILE * fp) const;

protected:
	PFType					m_type;
	UT_uint32				m_length;	/* in PT_DocPosition-space */
	
	pf_Frag *				m_next;
	pf_Frag *				m_prev;

	pt_PieceTable *			m_pPieceTable;
};

#endif /* PF_FRAG_H */
