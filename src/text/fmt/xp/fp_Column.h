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

#ifndef COLUMN_H
#define COLUMN_H

#ifdef FMT_TEST
#include <stdio.h>
#endif

#include "ut_misc.h"
#include "ut_types.h"
#include "ut_vector.h"
#include "pt_Types.h"
#include "fp_Page.h"
#include "fl_DocLayout.h"
#include "fp_ContainerObject.h"

class fl_EndnoteSectionLayout;
class fl_HdrFtrSectionLayout;
class fl_DocSectionLayout;
class fl_SectionLayout;
class fl_HdrFtrShadow;
class fp_Page;
class PP_AttrProp;
class GR_Graphics;
class fp_TableContainer;
struct dg_DrawArgs;
struct fp_Sliver;

class ABI_EXPORT fp_VerticalContainer : public fp_Container
{
public:
	fp_VerticalContainer(FP_ContainerType iType, fl_SectionLayout* pSectionLayout);
	virtual ~fp_VerticalContainer();

	virtual void		setWidth(UT_sint32);
	virtual void		setHeight(UT_sint32);
	virtual void		setMaxHeight(UT_sint32);
	virtual void		setX(UT_sint32, bool bDontClearIfNeeded=false);
	virtual void		setY(UT_sint32);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	virtual void		setWidthInLayoutUnits(UT_sint32);
	virtual void		setMaxHeightInLayoutUnits(UT_sint32);
	virtual void        setHeightLayoutUnits(UT_sint32 ihLayout) {m_iHeightLayoutUnits = ihLayout;}
	virtual void        setYInLayoutUnits(UT_sint32) {}
#endif
	/*!
	  Get container's max height
	  \return Max height
	*/
	inline UT_sint32	getMaxHeight(void) const
 		{ return m_iMaxHeight; }
	
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	/*!
	  Get container's max height in layout units
	  \return Max height in layout units
	*/
	inline UT_sint32	getMaxHeightInLayoutUnits(void) const
		{ return m_iMaxHeightLayoutUnits; }
	/*!
	  Get container's width
	  \return Width
	*/
#endif

	virtual UT_sint32	getWidth(void) const
		{ return m_iWidth; }
	
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	/*!
	  Get container's width in layout units
	  \return Width in layout units
	*/
	virtual UT_sint32	getWidthInLayoutUnits(void) const
		{ UT_ASSERT(m_iWidthLayoutUnits); return m_iWidthLayoutUnits; }
#endif

	virtual UT_sint32	getX(void) const;

	void        _setX( UT_sint32 iX) { m_iX = iX;}

	virtual UT_sint32	getY(void) const;

	void        _setY( UT_sint32 iY) { m_iY = iY;}
	/*!
	  Get container's height
	  \return Height
	*/
	virtual UT_sint32	getHeight(void) const
		{ return m_iHeight; }
	
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	/*!
	  Get container's height in layout units
	  \return Height in layout units
	*/
	virtual UT_sint32	getHeightInLayoutUnits(void) const
		{ return m_iHeightLayoutUnits; }
#endif

	UT_sint32	getColumnGap(void) const;

	/*!
	  Get container's intentionally empty flag
	  \return Empty
	*/
	bool			getIntentionallyEmpty(void) const
		{ return m_bIntentionallyEmpty; }
	/*!
	  Set container's intentionally empty flag
	*/
	void			setIntentionallyEmpty(bool b)
		{ m_bIntentionallyEmpty = b; }

	fp_Container*			getFirstContainer(void) const;
	fp_Container*			getLastContainer(void) const;
	UT_sint32               getYoffsetFromTable(fp_Container * pT,
												fp_Container* pCell,
												fp_ContainerObject * pCon);
	bool				insertContainerAfter(fp_Container* pNewContainer, fp_Container*	pAfterContainer);
	bool				insertContainer(fp_Container*);
	bool				addContainer(fp_Container*);
	void				removeContainer(fp_Container*);

	virtual UT_uint32 	distanceFromPoint(UT_sint32 x, UT_sint32 y);

	virtual void		mapXYToPosition(UT_sint32 xPos,
										UT_sint32 yPos,
										PT_DocPosition& pos,
										bool& bBOL, bool& bEOL);

	void		 		getOffsets(fp_ContainerObject* pContainer,
								   UT_sint32& xoff,
								   UT_sint32& yoff);
	fp_TableContainer * getCorrectBrokenTable(fp_Container * pLine);
	void		 		getScreenOffsets(fp_ContainerObject* pContainer,
										 UT_sint32& xoff,
										 UT_sint32& yoff);

	virtual void		draw(dg_DrawArgs*);
	virtual void		draw(GR_Graphics*) {}

	void				clearScreen(void);
	void 				bumpContainers(fp_ContainerObject* pLastContainerToKeep);
	virtual bool        isVBreakable(void) {return true;}
	virtual bool        isHBreakable(void) {return false;}
	virtual UT_sint32   wantVBreakAt(UT_sint32) {return 0;}
	virtual UT_sint32   wantHBreakAt(UT_sint32) {return 0;}
	virtual fp_ContainerObject * VBreakAt(UT_sint32) {return NULL;}
	virtual fp_ContainerObject * HBreakAt(UT_sint32) {return NULL;}
	void                recalcMaxWidth(bool bDontClearIfNeeded = false) {}
	virtual UT_sint32   getMarginBefore(void) const { return 0;}
	virtual UT_sint32   getMarginAfter(void) const { return 0;}
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	virtual UT_sint32   getMarginBeforeInLayoutUnits(void) const { return 0;}
	virtual UT_sint32   getMarginAfterInLayoutUnits(void) const { return 0;}
#endif
	virtual void        setAssignedScreenHeight(UT_sint32) {}
	virtual fp_Container * getNextContainerInSection(void) const
		{return NULL;}
	virtual fp_Container * getPrevContainerInSection(void) const
		{return NULL;}

	FV_View*			getView(void) const
		{ return getPage()->getDocLayout()->getView(); }

protected:
    void                _setMaxContainerHeight(UT_sint32 iContainerHeight);
	UT_sint32           _getMaxContainerHeight(void) const;

	virtual void			_drawBoundaries(dg_DrawArgs* pDA);

private:

	/*!
	  Width of the container
	*/
	UT_sint32 				m_iWidth;
	
	/*!
	  Height of the container
	*/
	UT_sint32 				m_iHeight;
	/*!
	  Maximum height of the container
	*/
	UT_sint32				m_iMaxHeight;

	/*!
	  X coordinate of container
	*/
	UT_sint32				m_iX;
	/*!
	  Y coordinate of container
	*/
	UT_sint32				m_iY;

	/*!
	  Set if this container is intentionally left empty

	  The breakSection function that does page layout sometimes
	  decides to leave sections empty for one reason or another. This
	  needs to be flagged, or fl_DocSectionLayout::deleteEmptyColumns
	  will delete the container.
	 */
	bool					m_bIntentionallyEmpty;
	UT_sint32               m_imaxContainerHeight;

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	/*!
	  Width in layout units of the container
	*/
	UT_sint32 				m_iWidthLayoutUnits;
	/*!
	  Height in layout units of the container
	*/
	UT_sint32 				m_iHeightLayoutUnits;
	/*!
	  Maximum height in layout units of the container
	*/
	UT_sint32				m_iMaxHeightLayoutUnits;
#endif
};

class ABI_EXPORT fp_Column : public fp_VerticalContainer
{
public:
	fp_Column(fl_SectionLayout* pSectionLayout);
	~fp_Column();

	fl_DocSectionLayout*	getDocSectionLayout(void) const;

	void			setLeader(fp_Column* p) { m_pLeader = p; }
	void			setFollower(fp_Column* p) { m_pFollower = p; }
	fp_Column*	getLeader(void) const  { return m_pLeader; }
	fp_Column*	getFollower(void) const 		{ return m_pFollower; }
	/*!
	  Get page container is located on
	  \return Page
	*/

	void				setPage(fp_Page* pPage) {m_pPage = pPage ;}
	virtual fp_Page*	getPage(void) const
		{ return m_pPage; }
	/*!
	  Get container's max height
	  \return Max height
	*/
	UT_sint32	        getMaxHeight(void) const;
 	
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	/*!
	  Get container's max height in layout units
	  \return Max height in layout units
	*/
	UT_sint32	        getMaxHeightInLayoutUnits(void) const;
#endif

	void				layout(void);


#ifdef FMT_TEST
	void				__dump(FILE * fp) const;
#endif

protected:
	UT_uint32 				_getBottomOfLastContainer(void) const;

	void					_drawBoundaries(dg_DrawArgs* pDA);

private:

	fp_Column*				m_pLeader;
	fp_Column*				m_pFollower;
	fp_Page*				m_pPage;
};

class ABI_EXPORT fp_ShadowContainer : public fp_VerticalContainer
{
public:
	fp_ShadowContainer(UT_sint32 iX, UT_sint32 iY,
					   UT_sint32 iWidth, UT_sint32 iHeight,
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
					   UT_sint32 iWidthLayout, UT_sint32 iHeightLayout,
#endif
					   fl_SectionLayout* pSL);
	~fp_ShadowContainer();

	fl_HdrFtrSectionLayout*	getHdrFtrSectionLayout(void) const;
	fl_HdrFtrShadow *   getShadow();
 	virtual void		draw(dg_DrawArgs*);
 	virtual void		draw(GR_Graphics*) {};
  	virtual void		layout(void);
 	virtual void		clearScreen(void);
	void                clearHdrFtrBoundaries(void);
	void				setPage(fp_Page* pPage) {m_pPage = pPage ;}
	virtual inline fp_Page*		getPage(void) const
		{ return m_pPage; }
protected:
	void                _drawHdrFtrBoundaries(dg_DrawArgs * pDA);
private:
	bool                m_bHdrFtrBoxDrawn;
	UT_sint32           m_ixoffBegin;
	UT_sint32           m_iyoffBegin;
	UT_sint32           m_ixoffEnd;
	UT_sint32           m_iyoffEnd;
	fp_Page*			m_pPage;

};


class ABI_EXPORT fp_HdrFtrContainer : public fp_VerticalContainer
{
public:
	fp_HdrFtrContainer( UT_sint32 iWidth,
					   UT_sint32 iWidthLayout,
					   fl_SectionLayout* pSL);
	~fp_HdrFtrContainer();

	fl_HdrFtrSectionLayout*	getHdrFtrSectionLayout(void) const;
 	virtual void		draw(dg_DrawArgs*);
  	virtual void		layout(void);
 	virtual void		clearScreen(void);
	void		 		getScreenOffsets(fp_ContainerObject* pContainer, UT_sint32& xoff,
										 UT_sint32& yoff);

protected:
};

class fp_EndnoteSectionContainer : public fp_VerticalContainer
{
public:
	fp_EndnoteSectionContainer(fl_SectionLayout* pSectionLayout);
	~fp_EndnoteSectionContainer();

	fl_EndnoteSectionLayout*	getEndnoteSectionLayout(void) const;

 	virtual void		draw(dg_DrawArgs*);
  	virtual void		layout(void);
 	virtual void		clearScreen(void);
	virtual inline fp_Page*		getPage(void) const
		{ return m_pPage; }
protected:
private:
	fp_Page*				m_pPage;
};

#endif /* COLUMN_H */
