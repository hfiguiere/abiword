 
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

#ifndef PF_FRAG_OBJECT_H
#define PF_FRAG_OBJECT_H

#include "ut_types.h"
#include "pf_Frag.h"

// pf_Frag_Object represents an in-line object (such as
// an image) in the document.

class pf_Frag_Object : public pf_Frag
{
public:
	pf_Frag_Object();
	virtual ~pf_Frag_Object();
	
	virtual UT_Bool			createSpecialChangeRecord(PX_ChangeRecord ** ppcr) const;

	virtual void			dump(FILE * fp) const;

protected:
	
};

#endif /* PF_FRAG_OBJECT_H */
