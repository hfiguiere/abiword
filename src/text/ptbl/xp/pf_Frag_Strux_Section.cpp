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


#include "pf_Frag_Strux_Section.h"
#include "px_ChangeRecord.h"
#include "px_CR_Strux.h"

#define pf_FRAG_STRUX_SECTION_LENGTH 1

/*****************************************************************/
/*****************************************************************/

pf_Frag_Strux_Section::pf_Frag_Strux_Section(pt_PieceTable * pPT,
											 PT_AttrPropIndex indexAP)
	: pf_Frag_Strux(pPT,PTX_Section,pf_FRAG_STRUX_SECTION_LENGTH,indexAP)
{
}

pf_Frag_Strux_Section::~pf_Frag_Strux_Section()
{
}


pf_Frag_Strux_SectionHdrFtr::pf_Frag_Strux_SectionHdrFtr(pt_PieceTable * pPT,
											 PT_AttrPropIndex indexAP)
	: pf_Frag_Strux(pPT,PTX_SectionHdrFtr,pf_FRAG_STRUX_SECTION_LENGTH,indexAP)
{
}

pf_Frag_Strux_SectionHdrFtr::~pf_Frag_Strux_SectionHdrFtr()
{
}






