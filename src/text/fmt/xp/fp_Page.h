 
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


#ifndef PAGE_H
#define PAGE_H

#include "ut_types.h"
#include "ut_vector.h"

class FL_DocLayout;
class FL_SectionLayout;
class FP_SectionSlice;
class DG_Graphics;
class DG_LayoutView;
struct dg_DrawArgs;

// ----------------------------------------------------------------
/*
	FP_Page is a concrete class used to represent a page.  A FP_Page manages
	a group of children, each of which must be a FP_SectionSlice.
*/
struct fp_SectionSliceInfo
{
	fp_SectionSliceInfo(FP_SectionSlice*, UT_uint32, UT_uint32);
	FP_SectionSlice*		pSlice;
	UT_uint32 xoff;
	UT_uint32 yoff;
};

class FP_Page
{
 public:
	FP_Page(FL_DocLayout*,
			DG_LayoutView*,
			UT_uint32 iWidth,
			UT_uint32 iHeight,
			UT_uint32 iLeft,
			UT_uint32 iTop,
			UT_uint32 iRight,
			UT_uint32 iBottom);
	~FP_Page();

	UT_Bool 		requestSpace(FL_SectionLayout*, FP_SectionSlice** si);

	int				getWidth();
	int				getHeight();
	FP_Page*		getNext();
	void			setNext(FP_Page*);
	FL_DocLayout*	getLayout();
	void            setLayoutView(DG_LayoutView*);

	void			mapXYToBufferPosition(UT_sint32 xPos, UT_sint32 yPos, UT_uint32& pos, UT_Bool& bRight);
	void			getOffsets(FP_SectionSlice*, void*, UT_sint32& xoff, UT_sint32& yoff);
	void			getScreenOffsets(FP_SectionSlice*, void*, UT_sint32& xoff, UT_sint32& yoff, UT_sint32& width, UT_sint32& height);

	void			draw(dg_DrawArgs*);
	void			dump();
	
 protected:
	FL_DocLayout*		m_pLayout;
	DG_LayoutView*      m_pLayoutView;
	FP_Page*			m_pNext;

	UT_sint32			m_iWidth;
	UT_sint32			m_iHeight;
	UT_sint32			m_iLeft;
	UT_sint32			m_iTop;
	UT_sint32			m_iRight;
	UT_sint32			m_iBottom;

	UT_Vector			m_vecSliceInfos;
};

#endif /* PAGE_H */
