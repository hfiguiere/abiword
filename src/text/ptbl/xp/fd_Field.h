/* AbiWord
 * Copyright (C) 2000 AbiSource, Inc.
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
#ifndef FIELD_H
#define FIELD_H

#include "ut_types.h"
#include "pf_Frag_Object.h"
#include "pf_Frag_Text.h"
#include "pt_Types.h"
#include "fl_BlockLayout.h"

class pf_Frag_Object;

/*!
 \note This class will eventually have subclasses to implement the different
 types of fields.
*/

class fd_Field
{
 public:
    // TBD: convention for naming 
    typedef enum _FieldType 
	{
		FD_Test,
		FD_MartinTest,
		FD_Time, 
		FD_PageNumber, 
		FD_PageCount,
		FD_ListLabel
	} FieldType;
    fd_Field(pf_Frag_Object& fO, pt_PieceTable * pt, FieldType fieldType);
    virtual                              ~fd_Field(void);
    bool                              update(void);
    void                                 setBlock(fl_BlockLayout * pBlock);
    fl_BlockLayout *                     getBlock( void);
    // probably need different types of update
    // which are overridden in the appropriate subclass
    // eg positionChangeUpdate
    //    referenceChangeUpdate
 protected:
    bool                              _deleteSpan(void);
    void                                 _throwChangeRec(PT_DocPosition docPos);
    fl_BlockLayout * m_pBlock;
    // will need some more helper functions in here eg. to test 
    // whether text has changed to avoid unnecessary updates
 private:
    pf_Frag_Object& m_fragObject;
    pt_PieceTable *	m_pPieceTable;
    UT_uint32 m_updateCount;
    FieldType m_iFieldType;
};

#endif




