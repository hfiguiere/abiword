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



#ifndef BLOCKLAYOUT_H
#define BLOCKLAYOUT_H

#include "ut_misc.h"
#include "ut_types.h"
#include "ut_vector.h"
#include "ut_dllist.h"
#include "ut_growbuf.h"
#include "pt_Types.h"
#include "fl_Layout.h"

// number of DocPositions occupied by the block strux
#define fl_BLOCK_STRUX_OFFSET	1

// TODO the following should be an enum
#define FL_ALIGN_BLOCK_LEFT		1
#define FL_ALIGN_BLOCK_RIGHT    2
#define FL_ALIGN_BLOCK_CENTER   3
#define FL_ALIGN_BLOCK_JUSTIFY  4
	
class FL_DocLayout;
class fl_SectionLayout;
class fb_LineBreaker;
class fp_Line;
class fp_Run;
class DG_Graphics;
class PD_Document;
class PP_Property;
class PX_ChangeRecord_Object;
class PX_ChangeRecord_ObjectChange;
class PX_ChangeRecord_Span;
class PX_ChangeRecord_SpanChange;
class PX_ChangeRecord_Strux;
class PX_ChangeRecord_StruxChange;

class fl_BlockLayout : public fl_Layout
{
	friend class fl_DocListener;

public:
	fl_BlockLayout(PL_StruxDocHandle sdh, fb_LineBreaker*, fl_BlockLayout*, fl_SectionLayout*, PT_AttrPropIndex indexAP);
	~fl_BlockLayout();

	int 		format();
	
	fp_Line*	getNewLine(UT_sint32 iHeight);

	const char*	getProperty(const XML_Char * pszName) const;
	void setAlignment(UT_uint32 iAlignCmd);
	UT_uint32 getAlignment();

	fl_BlockLayout* getNext(UT_Bool bKeepGoing) const;
	fl_BlockLayout* getPrev(UT_Bool bKeepGoing) const;
	fp_Line* getFirstLine();
	fp_Line* getLastLine();
	fp_Line* findPrevLineInDocument(fp_Line*);
	fp_Line* findNextLineInDocument(fp_Line*);
	fp_Run* getFirstRun();
	void findSquigglesForRun(fp_Run* pRun);
	UT_uint32 canSlurp(fp_Line* pLine) const;

	FL_DocLayout * getDocLayout();
	fl_SectionLayout * getSectionLayout();
	UT_GrowBuf * getCharWidths(void);

	PT_DocPosition getPosition(UT_Bool bActualBlockPos=UT_FALSE) const;
	fp_Run* findPointCoords(PT_DocPosition position, UT_Bool bEOL, UT_uint32& x, UT_uint32& y, UT_uint32& height);

	UT_Bool getSpanPtr(UT_uint32 offset, const UT_UCSChar ** ppSpan, UT_uint32 * pLength) const;
	UT_Bool	getBlockBuf(UT_GrowBuf * pgb) const;

	UT_Bool truncateLayout(fp_Run* pTruncRun);

	void clearScreen(DG_Graphics*);

	void dump();
	void align();
	void alignOneLine(fp_Line* pLine);

	inline UT_sint32	getTextIndent(void) const { return m_iTextIndent; }
	inline UT_sint32	getLeftMargin(void) const { return m_iLeftMargin; }
	inline UT_sint32	getRightMargin(void) const { return m_iRightMargin; }
	inline UT_sint32	getTopMargin(void) const { return m_iTopMargin; }
	inline UT_sint32	getBottomMargin(void) const { return m_iBottomMargin; }

	UT_uint32 getOrphansProperty(void) const;
	UT_uint32 getWidowsProperty(void) const;
	void checkForWidowsAndOrphans(void);

	void checkSpelling(void);

	UT_Bool doclistener_populateSpan(PT_BlockOffset blockOffset, UT_uint32 len);
	UT_Bool doclistener_populateObject(PT_BlockOffset blockOffset, const PX_ChangeRecord_Object * pcro);
	
	UT_Bool doclistener_insertSpan(const PX_ChangeRecord_Span * pcrs);
	UT_Bool doclistener_deleteSpan(const PX_ChangeRecord_Span * pcrs);
	UT_Bool doclistener_changeSpan(const PX_ChangeRecord_SpanChange * pcrsc);
	UT_Bool doclistener_deleteStrux(const PX_ChangeRecord_Strux * pcrx);
	UT_Bool doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc);
	UT_Bool doclistener_insertStrux(const PX_ChangeRecord_Strux * pcrx,
									PL_StruxDocHandle sdh,
									fl_BlockLayout ** ppNewBL);
	UT_Bool doclistener_insertObject(const PX_ChangeRecord_Object * pcro);
	UT_Bool doclistener_deleteObject(const PX_ChangeRecord_Object * pcro);
	UT_Bool doclistener_changeObject(const PX_ChangeRecord_ObjectChange * pcroc);
	
	/*
		Blocks are stored in a linked list which contains all of the blocks in
		the normal flow, in order.
	*/

protected:
	void					_lookupProperties(void);
	void			 		_fixColumns(void);
	void					_purgeLayout(void);
	void					_removeLine(fp_Line*);
	void					_removeAllEmptyLines(void);
	void					_destroySpellCheckLists(void);
	void					_addPartNotSpellChecked(UT_uint32 iOffset, UT_uint32 iLen);
	void					_addPartSpelledWrong(UT_uint32 iOffset, UT_uint32 iLen);

	UT_uint32				_getLastChar();

	UT_GrowBuf				m_gbCharWidths;

	FL_DocLayout*	       	m_pLayout;
	fb_LineBreaker*			m_pBreaker;

	fl_BlockLayout*			m_pPrev;
	fl_BlockLayout*			m_pNext;

	fp_Run*					m_pFirstRun;
	fl_SectionLayout*		m_pSectionLayout;

	fp_Line*				m_pFirstLine;
	fp_Line*				m_pLastLine;

	int						m_bFormatting;

	// read-only caches of the underlying properties
	UT_uint32				m_iOrphansProperty;
	UT_uint32				m_iWidowsProperty;
	UT_sint32				m_iTopMargin;
	UT_sint32				m_iBottomMargin;
	UT_sint32				m_iLeftMargin;
	UT_sint32				m_iRightMargin;
	UT_sint32				m_iTextIndent;

	/*
	  The following lists are used to keep track of the spell
	  check status of this block.  The lstNotSpellChecked member
	  is a list of regions which have not been spell checked.
	  The lstSpelledWrong member is a list of regions which have
	  been found to be spelled wrong.
	*/
	UT_DLList				m_lstNotSpellChecked;
	UT_DLList				m_lstSpelledWrong;
};

/*
  This struct is used to represent a part of the block.  Pointers
  to this struct are the things contained in m_lstNotSpellChecked
  and m_lstSpelledWrong.
*/
struct fl_PartOfBlock
{
	fl_PartOfBlock();

	UT_uint32	iOffset;
	UT_uint32	iLength;
};

#endif /* BLOCKLAYOUT_H */
