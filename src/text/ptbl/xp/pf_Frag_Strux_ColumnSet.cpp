/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */ 


#include "pf_Frag_Strux_ColumnSet.h"
#include "px_ChangeRecord.h"
#include "px_ChangeRecord_Strux.h"

#define pf_FRAG_STRUX_COLUMNSET_LENGTH 0

/*****************************************************************/
/*****************************************************************/

pf_Frag_Strux_ColumnSet::pf_Frag_Strux_ColumnSet(pt_PieceTable * pPT,
												 PT_AttrPropIndex indexAP)
	: pf_Frag_Strux(pPT,PTX_ColumnSet,pf_FRAG_STRUX_COLUMNSET_LENGTH,indexAP)
{
}

pf_Frag_Strux_ColumnSet::~pf_Frag_Strux_ColumnSet()
{
}
