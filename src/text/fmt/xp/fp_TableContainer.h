/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2002 Martin Sevior <msevior@physics.unimelb.edu.au>
 *
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
 *
 */
/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-1999.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
 */

#ifndef TABLECONTAINER_H
#define TABLECONTAINER_H

#ifdef FMT_TEST
#include <stdio.h>
#endif

#include "ut_misc.h"
#include "ut_types.h"
#include "ut_vector.h"
#include "pt_Types.h"
#include "fp_Page.h"
#include "fp_ContainerObject.h"
#include "fp_Column.h"


class fp_TableRowColumn
{
public:
	fp_TableRowColumn(void);
	virtual ~fp_TableRowColumn(void);
	UT_sint32 requisition;
	UT_sint32 allocation;
	UT_sint32 spacing;
	bool need_expand;
	bool need_shrink;
	bool expand;
	bool shrink;
	bool empty;
};

class fp_VerticalContainer;
class fp_Column;
class fl_EndnoteSectionLayout;
class fl_HdrFtrSectionLayout;
class fl_DocSectionLayout;
class fl_SectionLayout;
class fl_HdrFtrShadow;
class fp_Page;
class PP_AttrProp;
class GR_Graphics;
struct dg_DrawArgs;
struct fp_Sliver;

class ABI_EXPORT fp_CellContainer : public fp_VerticalContainer
{
public:
	fp_CellContainer(fl_SectionLayout* pSectionLayout);
	virtual ~fp_CellContainer();

	void                sizeRequest(fp_Requisition * pRequest);
	void                sizeAllocate(fp_Allocation * pAllocate);

	virtual void		draw(dg_DrawArgs*);
	virtual void		draw(GR_Graphics*) {}
	virtual void        setWidth(UT_sint32 iWidth);
	        void        _drawBoundaries(dg_DrawArgs* pDA);
	virtual bool        isVBreakable(void);
	virtual bool        isHBreakable(void) {return false;}
	virtual UT_sint32   wantVBreakAt(UT_sint32);
	virtual UT_sint32   wantHBreakAt(UT_sint32) {return 0;}
	virtual fp_ContainerObject * VBreakAt(UT_sint32);
	virtual fp_ContainerObject * HBreakAt(UT_sint32) {return NULL;}
	void                recalcMaxWidth(bool bDontClearIfNeeded = false) {}
	virtual void        setAssignedScreenHeight(UT_sint32) {}
	virtual fp_Container * getNextContainerInSection(void) const;
	virtual fp_Container * getPrevContainerInSection(void) const;
    UT_sint32           getLeftAttach(void) const
		{ return m_iLeftAttach;}
    UT_sint32           getRightAttach(void) const
		{ return m_iRightAttach;}
    UT_sint32           getTopAttach(void) const
		{ return m_iTopAttach;}
    UT_sint32           getBottomAttach(void) const
		{ return m_iBottomAttach;}
    void                setLeftAttach(UT_sint32 i)
		{ m_iLeftAttach = i;}
    void                setRightAttach(UT_sint32 i)
		{ m_iRightAttach = i;}
    void                setTopAttach(UT_sint32 i)
		{ m_iTopAttach = i;}
    void                setBottomAttach(UT_sint32 i)
		{ m_iBottomAttach = i;}
	void                setToAllocation(void);
	UT_sint32           getLeftPad(void) const
		{ return m_iLeftPad;}
	UT_sint32           getRightPad(void) const
		{ return m_iRightPad;}
	UT_sint32           getTopPad(void) const
		{ return m_iTopPad;}
	UT_sint32           getBotPad(void) const
		{ return m_iBotPad;}
	void                setLeftPad(UT_sint32 i)
		{ m_iLeftPad = i;}
	void                setRightPad(UT_sint32 i)
		{ m_iRightPad = i;}
	void                setTopPad(UT_sint32 i)
		{ m_iTopPad = i;}
	void                setBotPad(UT_sint32 i)
		{ m_iBotPad = i;}
	bool                getXexpand(void) const
		{ return m_bXexpand;}
	bool                getYexpand(void) const
		{ return m_bYexpand;}
	bool                getXshrink(void) const
		{ return m_bXshrink;}
	bool                getYshrink(void) const
		{ return m_bYshrink;}
	void                setXexpand(bool b)
		{ m_bXexpand = b;}
	void                setYexpand(bool b)
		{ m_bYexpand = b;}
	void                setXshrink(bool b)
		{ m_bXshrink = b;}
	void                setYshrink(bool b)
		{ m_bYshrink = b;}
	bool                getXfill(void) const
		{return m_bXfill;}
	bool                getYfill(void)const
		{return m_bYfill;}
	void                setXfill(bool b)
		{ m_bXfill = b;}
	void                setYfill(bool b)
		{ m_bYfill = b;}

private:
//
// These variables describe where the cell is attached to the table.
// The first cell in the Table is at (0,0)
//
// m_iLeftAttach is the leftmost column containing the cell.
//  
	UT_sint32           m_iLeftAttach;

// m_iRightAttach is the first column to the right of the cell.

	UT_sint32           m_iRightAttach;

// m_iTopAttach is the topmost row containing the cell.

	UT_sint32           m_iTopAttach;

// m_iBottomAttach is the row immediately below the cell.

	UT_sint32           m_iBottomAttach;

// Local size request and allocation.

	fp_Allocation       m_MyAllocation;
	fp_Requisition      m_MyRequest;
//
// Padding left,right, top and bottom
//
	UT_sint32           m_iLeftPad;
	UT_sint32           m_iRightPad;
	UT_sint32           m_iTopPad;
	UT_sint32           m_iBotPad;
//
// Needed since a cell can span multiple pages.
//
	fp_CellContainer *  m_pNextInTable;
	fp_CellContainer *  m_pPrevInTable;
//
// Should the cell expand or shrink in the x and y directitions.
//
	bool                m_bXexpand;
	bool                m_bYexpand;
	bool                m_bXshrink;
	bool                m_bYshrink;
//
// Should we fill the container in x and y?
//
	bool                m_bXfill;
	bool                m_bYfill;
};

class ABI_EXPORT fp_TableContainer : public fp_Container
{
public:
	fp_TableContainer(fl_SectionLayout* pSectionLayout);
	~fp_TableContainer();

	void                sizeRequest(fp_Requisition * pRequest);
	void                sizeAllocate(fp_Allocation * pAllocate);

	void				layout(void);
	virtual void		draw(dg_DrawArgs*);
	virtual void		draw(GR_Graphics*) {}
    virtual void        clearScreen(void);
	virtual bool        isVBreakable(void);
	virtual bool        isHBreakable(void) {return false;}
	virtual UT_sint32   wantVBreakAt(UT_sint32);
	virtual UT_sint32   wantHBreakAt(UT_sint32) {return 0;}
	virtual fp_ContainerObject * VBreakAt(UT_sint32);
	virtual fp_ContainerObject * HBreakAt(UT_sint32) {return NULL;}
	void                setToAllocation(void);
	void                tableAttach(fp_CellContainer * pCell, 
									UT_sint32 leftAttach,
									UT_sint32 rightAttach,
									UT_sint32 topAttach,
									UT_sint32 bottomAttach,
									UT_uint32 attachOptions,
									UT_sint32 leftPad,
									UT_sint32 rightPad,
									UT_sint32 topPad,
									UT_sint32 botPad);

#ifdef FMT_TEST
	void				__dump(FILE * fp) const;
#endif

private:
	fp_TableRowColumn *     getNthCol(UT_sint32 i);
	fp_TableRowColumn *     getNthRow(UT_sint32 i);
	void                    _size_request_init(void);
	void                    _size_request_pass1(void);
	void                    _size_request_pass2(void);
	void                    _size_request_pass3(void);

    void                    _size_allocate_init(void);
    void                    _size_allocate_pass1(void);
	void                    _size_allocate_pass2(void);
	UT_uint32 				_getBottomOfLastContainer(void) const;
	void					_drawBoundaries(dg_DrawArgs* pDA);

	UT_Vector               m_vecRows;
	UT_Vector               m_vecColumns;
	UT_sint32               m_iRows;
	UT_sint32               m_iCols;

// Local size request and allocation.

	fp_Allocation           m_MyAllocation;
	fp_Requisition          m_MyRequest;
	UT_sint32               m_iBorderwidth;
	bool                    m_bIsHomogeneous;
};

#endif /* TABLECONTAINER_H */







