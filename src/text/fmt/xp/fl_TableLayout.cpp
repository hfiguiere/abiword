/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2002 Martin Sevior (msevior@physics.unimelb.edu.au>
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

#include <string.h>
#include <stdlib.h>

#include "ut_types.h"
#include "ut_string.h"

#include "ap_Prefs.h"
#include "fl_TableLayout.h"
#include "fl_SectionLayout.h"
#include "fl_Layout.h"
#include "fl_DocLayout.h"
#include "fl_BlockLayout.h"
#include "fb_LineBreaker.h"
#include "fp_Page.h"
#include "fp_Line.h"
#include "fp_Column.h"
#include "fp_TableContainer.h"
#include "fp_ContainerObject.h"
#include "pd_Document.h"
#include "pp_AttrProp.h"
#include "gr_Graphics.h"
#include "pp_Property.h"
#include "px_ChangeRecord.h"
#include "px_CR_Object.h"
#include "px_CR_ObjectChange.h"
#include "px_CR_Span.h"
#include "px_CR_SpanChange.h"
#include "px_CR_Strux.h"
#include "px_CR_StruxChange.h"
#include "px_CR_Glob.h"
#include "fv_View.h"
#include "fp_Run.h"
#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_units.h"

fl_TableLayout::fl_TableLayout(FL_DocLayout* pLayout, PL_StruxDocHandle sdh, 
			       PT_AttrPropIndex indexAP, fl_ContainerLayout * pMyContainerLayout)
	: fl_SectionLayout(pLayout, sdh, indexAP, FL_SECTION_TABLE,FL_CONTAINER_TABLE,PTX_SectionTable, pMyContainerLayout),
	  m_bNeedsRebuild(false),
	  m_iJustification(FL_TABLE_FULL),
	  m_iLeftOffset(0),
	  m_dLeftOffsetUserUnits(0.0),
	  m_iRightOffset(0),
	  m_dRightOffsetUserUnits(0.0),
	  m_iTopOffset(0),
	  m_dTopOffsetUserUnits(0.0),
	  m_iBottomOffset(0),
	  m_dBottomOffsetUserUnits(0.0),
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	  m_iLeftOffsetLayoutUnits(0),
	  m_iRightOffsetLayoutUnits(0),
	  m_iTopOffsetLayoutUnits(0),
	  m_iBottomOffsetLayoutUnits(0),
#endif
	  m_bIsHomogeneous(true),
	  m_bSameRowOnTopOfPage(false),
	  m_iRowNumberForTop(0),
	  m_iNumberOfRows(0),
	  m_iNumberOfColumns(0),
	  m_bColumnsPositionedOnPage(false),
	  m_bRowsPositionedOnPage(false),
	  m_bIsDirty(true),
	  m_iLineThickness(0),
	  m_iColSpacing(0),
	  m_iRowSpacing(0),
	  m_iLeftColPos(0),
	  m_bRecursiveFormat(false)

{
	UT_ASSERT(pLayout);
	m_vecColProps.clear();
	m_vecRowProps.clear();
	createTableContainer();
}

fl_TableLayout::~fl_TableLayout()
{
	// NB: be careful about the order of these
	UT_DEBUGMSG(("SEVIOR: !!!!!!!! Deleting tableLayout  %x !! \n",this));
	_purgeLayout();
	fp_TableContainer * pTC = (fp_TableContainer *) getFirstContainer();
	if (pTC)
	{
		delete pTC;
	}
	setFirstContainer(NULL);
	setLastContainer(NULL);
	UT_VECTOR_PURGEALL(fl_ColProps *, m_vecColProps);
	UT_VECTOR_PURGEALL(fl_RowProps *, m_vecRowProps);
}

/*!
 * Only one Master Table container per Table Layout. Create it here.
 */
void fl_TableLayout::createTableContainer(void)
{
	_lookupProperties();
	fp_TableContainer * pTableContainer = new fp_TableContainer((fl_SectionLayout *) this);
	setFirstContainer(pTableContainer);
	setLastContainer(pTableContainer);
	setTableContainerProperties(pTableContainer);
	fl_ContainerLayout * pCL = myContainingLayout();
	fp_Container * pCon = pCL->getLastContainer();
	UT_ASSERT(pCon);
	UT_sint32 iWidth = pCon->getWidth();
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	UT_sint32 iWidthLayout = pCon->getWidthInLayoutUnits();
#endif
	if(iWidth == 0)
	{
		iWidth = pCon->getPage()->getWidth();
		pCon->setWidth(iWidth);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		iWidthLayout = pCon->getPage()->getWidthInLayoutUnits();
		pCon->setWidthInLayoutUnits(iWidthLayout);
#endif
	}
	pTableContainer->setWidth(iWidth);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	pTableContainer->setWidthInLayoutUnits(iWidthLayout);
#endif
//
// The container of the tbale is set in getNewContainer()
//
}

/*!
 * This method sets all the parameters of the table container from
 * properties of this section.
 */
void fl_TableLayout::setTableContainerProperties(fp_TableContainer * pTab)
{
	pTab->setHomogeneous(m_bIsHomogeneous);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	UT_sint32 borderWidth = m_iLeftOffsetLayoutUnits + m_iRightOffsetLayoutUnits;
#else
	UT_sint32 borderWidth = m_iLeftOffset + m_iRightOffset;
#endif
	pTab->setBorderWidth(borderWidth);
	pTab->setColSpacings(m_iColSpacing);
	pTab->setRowSpacings(m_iRowSpacing);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	pTab->setLeftOffset(m_iLeftOffsetLayoutUnits);
	pTab->setRightOffset(m_iRightOffsetLayoutUnits);
#else
	pTab->setLeftOffset(m_iLeftOffset);
	pTab->setRightOffset(m_iRightOffset);
#endif
	pTab->setTopOffset(m_iTopOffset);
	pTab->setBottomOffset(m_iBottomOffset);
	pTab->setLineThickness(m_iLineThickness);
}


/*!
  Create a new Table container and plug it into the linked list of Table 
  containers.
  \params If pPrevTab is non-null place the new cell after this in the linked
          list, otherwise just append it to the end.
  \return The newly created Table container
*/
fp_Container* fl_TableLayout::getNewContainer(fp_Container * pPrevTab)
{
	UT_ASSERT(getFirstContainer() == NULL);
	createTableContainer();
	fp_TableContainer * pNewTab = (fp_TableContainer *) getFirstContainer();
//
// Master Tables do not get linked into the container linked list.
//
	pNewTab->setPrev(NULL);
	pNewTab->setNext(NULL);
//
// Now find the right place to put our new Table container within the arrangement
// of it's own container.
//
	insertTableContainer(pNewTab);
	return (fp_Container *) pNewTab;
}

/*!
 * This method inserts the given TableContainer into its correct place in the
 * Vertical container.
 */
void fl_TableLayout::insertTableContainer( fp_TableContainer * pNewTab)
{
	fl_ContainerLayout * pUPCL = myContainingLayout();
	fl_ContainerLayout * pPrevL = (fl_ContainerLayout *) getPrev();
	fp_Container * pPrevCon = NULL;
	fp_Container * pUpCon = NULL;
	if(pPrevL != NULL)
	{
		pPrevCon = pPrevL->getLastContainer();
		pUpCon = pPrevCon->getContainer();
	}
	else
	{
		pUpCon = pUPCL->getLastContainer();
	}
	if(pPrevL == NULL)
	{
		UT_DEBUGMSG(("SEVIOR!!!!!!!!!! New Table %x added into %x \n",pNewTab,pUpCon));
		pUpCon->addCon(pNewTab);
		pNewTab->setContainer(pUpCon);
;
	}
	else
	{
		UT_sint32 i = pUpCon->findCon(pPrevCon);
		UT_DEBUGMSG(("SEVIOR!!!!!!!!!! New Table %x inserted into %x \n",pNewTab,pUpCon));
		if(i >= 0 && (i+1) < (UT_sint32) pUpCon->countCons())
		{
			pUpCon->insertConAt(pNewTab,i+1);
			pNewTab->setContainer(pUpCon);
		}
		else if( i >=0 &&  (i+ 1) == (UT_sint32) pUpCon->countCons())
		{
			pUpCon->addCon(pNewTab);
			pNewTab->setContainer(pUpCon);
		}
		else
		{
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		}
	}
}

void fl_TableLayout::format(void)
{
	if(m_bRecursiveFormat)
	{
		return;
	}
	m_bRecursiveFormat = true;
	bool bRebuild = false;
//
// All this code is to find the right page to do a sectionBreak from.
//
	fl_ContainerLayout * pPrevCL = getPrev();
	fp_Page * pPrevP = NULL;
#if 0
	while(pPrevCL && (pPrevCL->myContainingLayout()->getContainerType() != FL_CONTAINER_DOCSECTION))
	{
		pPrevCL = pPrevCL->myContainingLayout();
	}
	if(pPrevCL)
	{
		fp_Container * pPrevCon = pPrevCL->getFirstContainer();
		if(pPrevCon)
		{
			pPrevP = pPrevCon->getPage();
		}
		static_cast<fl_DocSectionLayout *>(pPrevCL)->setNeedsSectionBreak(true,pPrevP);
	}
	else
	{
		getDocLayout()->getFirstSection()->setNeedsSectionBreak(true,NULL);
		pPrevCL = myContainingLayout();
		while(pPrevCL && (pPrevCL->myContainingLayout()->getContainerType() != FL_CONTAINER_DOCSECTION))
		{
			pPrevCL = pPrevCL->myContainingLayout();
		}
		if(pPrevCL)
		{
			static_cast<fl_DocSectionLayout *>(pPrevCL)->setNeedsSectionBreak(true,pPrevP);
		}
	}
#else
	if(pPrevCL)
	{
		fp_Container * pPrevCon = pPrevCL->getFirstContainer();
		if(pPrevCon)
		{
			pPrevP = pPrevCon->getPage();
		}
	}
	static_cast<fl_DocSectionLayout *>(getSectionLayout())->setNeedsSectionBreak(true,pPrevP);
#endif
//
// OK on with the formatting
//
	if(getFirstContainer() == NULL)
	{
		getNewContainer(NULL);
		bRebuild = true;
	}
	if(isDirty())
	{
		markAllRunsDirty();
	}
	fl_ContainerLayout*	pCell = getFirstLayout();
	while (pCell)
	{
		pCell->format();
		if(bRebuild)
		{
			attachCell(pCell);
		}
		pCell = pCell->getNext();
	}
	xxx_UT_DEBUGMSG(("SEVIOR: Finished Formatting %x isDirty %d \n",this,isDirty()));

	if(isDirty() && !getDocument()->isDontImmediateLayout())
	{
		m_bIsDirty = false;
		UT_DEBUGMSG(("SEVIOR: Layout pass 1 \n"));
		static_cast<fp_TableContainer *>(getFirstContainer())->layout();
		setNeedsRedraw();
		markAllRunsDirty();
	}
//
// The layout process can trigger a width change on a cell which requires
// a second layout pass
//
	if(isDirty() && !getDocument()->isDontImmediateLayout())
	{
		static_cast<fp_TableContainer *>(getFirstContainer())->layout();
		UT_DEBUGMSG(("SEVIOR: Layout pass 2 \n"));
		setNeedsRedraw();
		markAllRunsDirty();
		m_bIsDirty = false;
	}
//	m_bNeedsReformat = m_bIsDirty;
	m_bNeedsReformat = false;
	if(m_bNeedsReformat)
	{
		UT_DEBUGMSG(("SEVIOR: After format in TableLayout need another format \n"));
	}
	m_bRecursiveFormat = false;
}

void fl_TableLayout::markAllRunsDirty(void)
{
	fl_ContainerLayout*	pCL = getFirstLayout();
	while (pCL)
	{
		pCL->markAllRunsDirty();
		pCL = pCL->getNext();
	}
}

void fl_TableLayout::updateLayout(void)
{			
	xxx_UT_DEBUGMSG(("updateTableLayout  \n"));
	if(getDocument()->isDontImmediateLayout())
	{
		xxx_UT_DEBUGMSG(("updateTableLayout not allowed updates  \n"));
		return;
	}
	xxx_UT_DEBUGMSG(("updateTableLayout Doing updates  \n"));
	fl_ContainerLayout*	pBL = getFirstLayout();
	bool bNeedsFormat = false;
	while (pBL)
	{
		if (pBL->needsReformat())
		{
			pBL->updateLayout();
			bNeedsFormat = true;
		}

		pBL = pBL->getNext();
	}
	if(bNeedsFormat || isDirty())
	{
		format();
	}
}

void fl_TableLayout::redrawUpdate(void)
{
	if(getDocument()->isDontImmediateLayout())
	{
		return;
	}
	fl_ContainerLayout*	pBL = getFirstLayout();
	while (pBL)
	{
		if (pBL->needsRedraw())
		{
			xxx_UT_DEBUGMSG(("SEVIOR: Doing redraw in table \n"));
			pBL->redrawUpdate();
		}
		pBL = pBL->getNext();
	}
	fp_TableContainer * pTab = (fp_TableContainer *) getFirstContainer();
	if(pTab->doRedrawLines())
	{
		pTab->drawLines();
	}
	m_bNeedsRedraw = false;
}

bool fl_TableLayout::doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc)
{
	UT_ASSERT(pcrxc->getType()==PX_ChangeRecord::PXT_ChangeStrux);
	xxx_UT_DEBUGMSG(("changeStrux: getNext() %x getPrev() %x \n",getNext(),getPrev()));
	if(getPrev())
	{
		UT_ASSERT(getPrev()->getNext() == this);
	}
	if(getNext())
	{
		UT_ASSERT(getNext()->getPrev() == this);
	}

	setAttrPropIndex(pcrxc->getIndexAP());
	collapse();
	updateTable();
	UT_DEBUGMSG(("SEVIOR: getNext() %x getPrev() %x \n",getNext(),getPrev()));
	if(getPrev())
	{
		UT_ASSERT(getPrev()->getNext() == this);
	}
	if(getNext())
	{
		UT_ASSERT(getNext()->getPrev() == this);
	}
	return true;
}

fl_SectionLayout * fl_TableLayout::getSectionLayout(void) const
{
	fl_ContainerLayout * pDSL = myContainingLayout();
	while(pDSL)
	{
		if(pDSL->getContainerType() == FL_CONTAINER_DOCSECTION)
		{
			return (fl_SectionLayout *) pDSL;
		}
		pDSL = pDSL->myContainingLayout();
	}
	return NULL;
}
void fl_TableLayout::updateTable(void)
{

	const PP_AttrProp* pAP = NULL;
	bool bres = m_pDoc->getAttrProp(m_apIndex, &pAP);
	UT_ASSERT(bres);

	_lookupProperties();

	// clear all the columns
    // Assume that all formatting have already been removed via a 
    // collapse()
    //

	/*
	  TODO to more closely mirror the architecture we're using for BlockLayout, this code
	  should probably just set a flag, indicating the need to reformat this section.  Then,
	  when it's time to update everything, we'll actually do the format.
	*/

	FV_View * pView = m_pLayout->getView();
	if(pView)
	{
		pView->setScreenUpdateOnGeneralUpdate(false);
	}

	format();
	markAllRunsDirty();

	if(pView)
	{
		pView->setScreenUpdateOnGeneralUpdate(true);
	}

	return;
}


bool fl_TableLayout::bl_doclistener_insertBlock(fl_ContainerLayout* pLBlock,
											  const PX_ChangeRecord_Strux * pcrx,
											  PL_StruxDocHandle sdh,
											  PL_ListenerId lid,
											  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
																	  PL_ListenerId lid,
																	  PL_StruxFmtHandle sfhNew))
{

	UT_ASSERT(pcrx->getType()==PX_ChangeRecord::PXT_InsertStrux);
	UT_ASSERT(pcrx->getStruxType()==PTX_Block);

	fl_ContainerLayout * pNewCL = NULL;
	pNewCL = insert(sdh,this,pcrx->getIndexAP(), FL_CONTAINER_BLOCK);
	fl_BlockLayout * pBlock = (fl_BlockLayout *) pNewCL;
//
// Set the sectionlayout of this table to that of the block since it is that scope
//
	pBlock->setSectionLayout((fl_SectionLayout *) myContainingLayout());
	pNewCL->setContainingLayout(myContainingLayout());

		// Must call the bind function to complete the exchange of handles
		// with the document (piece table) *** before *** anything tries
		// to call down into the document (like all of the view
		// listeners).
		
	PL_StruxFmtHandle sfhNew = (PL_StruxFmtHandle)pNewCL;
	pfnBindHandles(sdh,lid,sfhNew);
//
// increment the insertion point in the view.
//
	FV_View* pView = m_pLayout->getView();
	if (pView && (pView->isActive() || pView->isPreview()))
	{
		pView->setPoint(pcrx->getPosition() + fl_BLOCK_STRUX_OFFSET);
	}
	else if(pView && pView->getPoint() > pcrx->getPosition())
	{
		pView->setPoint(pView->getPoint() +  fl_BLOCK_STRUX_OFFSET);
	}
	return true;
}


bool fl_TableLayout::bl_doclistener_insertTable( const PX_ChangeRecord_Strux * pcrx,
											   SectionType iType,
											   PL_StruxDocHandle sdh,
											   PL_ListenerId lid,
											   void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
																	   PL_ListenerId lid,
																	   PL_StruxFmtHandle sfhNew))
{
	UT_ASSERT(iType == FL_SECTION_TABLE);
	UT_ASSERT(pcrx->getType()==PX_ChangeRecord::PXT_InsertStrux);
	UT_ASSERT(pcrx->getStruxType()==PTX_SectionTable);
	PT_DocPosition pos1;
//
// This is to clean the fragments
//
	m_pDoc->getBounds(true,pos1);

	fl_SectionLayout* pSL = NULL;

	pSL = (fl_SectionLayout *) static_cast<fl_ContainerLayout *>(getSectionLayout())->insert(sdh,this,pcrx->getIndexAP(), FL_CONTAINER_TABLE);

		// Must call the bind function to complete the exchange of handles
		// with the document (piece table) *** before *** anything tries
		// to call down into the document (like all of the view
		// listeners).

	PL_StruxFmtHandle sfhNew = (PL_StruxFmtHandle)pSL;
	pfnBindHandles(sdh,lid,sfhNew);

//
// increment the insertion point in the view.
//
	FV_View* pView = m_pLayout->getView();
	if (pView && (pView->isActive() || pView->isPreview()))
	{
		pView->setPoint(pcrx->getPosition() + fl_BLOCK_STRUX_OFFSET);
	}
	else if(pView && pView->getPoint() > pcrx->getPosition())
	{
		pView->setPoint(pView->getPoint() + fl_BLOCK_STRUX_OFFSET);
	}
//
// OK that's it!
//
	return true;
}


bool fl_TableLayout::bl_doclistener_insertCell(fl_ContainerLayout* pCell,
											  const PX_ChangeRecord_Strux * pcrx,
											  PL_StruxDocHandle sdh,
											  PL_ListenerId lid,
											  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
																	  PL_ListenerId lid,
																	  PL_StruxFmtHandle sfhNew))
{
	fl_ContainerLayout * pNewCL = NULL;
	if(pCell == NULL)
	{
		pNewCL = append(sdh, pcrx->getIndexAP(),FL_CONTAINER_CELL);
	}
	else
	{
		pNewCL = insert(sdh,pCell,pcrx->getIndexAP(), FL_CONTAINER_CELL);
	}
	
		// Must call the bind function to complete the exchange of handles
		// with the document (piece table) *** before *** anything tries
		// to call down into the document (like all of the view
		// listeners).
		
	PL_StruxFmtHandle sfhNew = (PL_StruxFmtHandle)pNewCL;
	pfnBindHandles(sdh,lid,sfhNew);

	fl_CellLayout * pCL = (fl_CellLayout *) pNewCL;
	attachCell(pCL);
//
// increment the insertion point in the view.
//
	FV_View* pView = m_pLayout->getView();
	if (pView && (pView->isActive() || pView->isPreview()))
	{
		pView->setPoint(pcrx->getPosition() + fl_BLOCK_STRUX_OFFSET);
	}
	else if(pView && pView->getPoint() > pcrx->getPosition())
	{
		pView->setPoint(pView->getPoint() +  fl_BLOCK_STRUX_OFFSET);
	}
	return true;
}



bool fl_TableLayout::bl_doclistener_insertEndTable(fl_ContainerLayout*,
												   const PX_ChangeRecord_Strux * pcrx,
												   PL_StruxDocHandle sdh,
												   PL_ListenerId lid,
												   void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
																	  PL_ListenerId lid,
																	  PL_StruxFmtHandle sfhNew))
{
	// The endTable strux actually has a format handle to to the this table layout.
	// so we bind to this layout.

		
	PL_StruxFmtHandle sfhNew = (PL_StruxFmtHandle) this;
	pfnBindHandles(sdh,lid,sfhNew);

//
// increment the insertion point in the view.
//
	FV_View* pView = m_pLayout->getView();
	if (pView && (pView->isActive() || pView->isPreview()))
	{
		pView->setPoint(pcrx->getPosition() +  fl_BLOCK_STRUX_OFFSET);
	}
	else if(pView && pView->getPoint() > pcrx->getPosition())
	{
		pView->setPoint(pView->getPoint() +  fl_BLOCK_STRUX_OFFSET);
	}
	return true;
}

bool fl_TableLayout::recalculateFields(UT_uint32 iUpdateCount)
{
	fl_ContainerLayout*	pBL = getFirstLayout();
	while (pBL)
	{
		pBL->recalculateFields(iUpdateCount);
		pBL = pBL->getNext();
	}
	return true;
}

void fl_TableLayout::_lookupProperties(void)
{
	const PP_AttrProp* pSectionAP = NULL;

	m_pLayout->getDocument()->getAttrProp(m_apIndex, &pSectionAP);

	/*
	  TODO shouldn't we be using PP_evalProperty like
	  the blockLayout does?

	  Yes, since PP_evalProperty does a fallback to the
	  last-chance defaults, whereas the code below is
	  hard-coding its own defaults.  Bad idea.
	*/

	const char* pszHomogeneous = NULL;
	pSectionAP->getProperty("homogeneuos", (const XML_Char *&)pszHomogeneous);
	if (pszHomogeneous && pszHomogeneous[0])
	{
		if(atoi(pszHomogeneous) == 1)
		{
			m_bIsHomogeneous = true;;
		}
	}
	else
	{
		m_bIsHomogeneous = false;
	}
	const char* pszLeftOffset = NULL;
	const char* pszTopOffset = NULL;
	const char* pszRightOffset = NULL;
	const char* pszBottomOffset = NULL;
	pSectionAP->getProperty("table-margin-left", (const XML_Char *&)pszLeftOffset);
	pSectionAP->getProperty("table-margin-top", (const XML_Char *&)pszTopOffset);
	pSectionAP->getProperty("table-margin-right", (const XML_Char *&)pszRightOffset);
	pSectionAP->getProperty("table-margin-bottom", (const XML_Char *&)pszBottomOffset);

	const XML_Char * szRulerUnits;
	UT_Dimension dim;
	if (XAP_App::getApp()->getPrefsValue(AP_PREF_KEY_RulerUnits,&szRulerUnits))
		dim = UT_determineDimension(szRulerUnits);
	else
		dim = DIM_IN;

	UT_String defaultOffset;
	switch(dim)
	{
	case DIM_IN:
		defaultOffset = "0.0in"; //was 0.02in
		break;

	case DIM_CM:
		defaultOffset = "0.0cm";
		break;

	case DIM_PI:
		defaultOffset = "0.0pi";
		break;

	case DIM_PT:
		defaultOffset= "0.0pt";
		break;

	case DIM_MM:
		defaultOffset= "0.0mm"; //was 0.508
		break;

		// TODO: PX, and PERCENT
		// let them fall through to the default now
		// and we don't use them anyway
#if 0
	case DIM_PX:
	case DIM_PERCENT:
#endif
	case DIM_none:
	default:
		defaultOffset = "0.0in";	// TODO: what to do with this. was 0.01in
		break;

	}
	defaultOffset = "0.005in";	// TODO: what to do with this. was 0.01in
	if(pszLeftOffset && pszLeftOffset[0])
	{
		m_iLeftOffset = m_pLayout->getGraphics()->convertDimension(pszLeftOffset);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iLeftOffsetLayoutUnits = UT_convertToLayoutUnits(pszLeftOffset);
#endif
		m_dLeftOffsetUserUnits = UT_convertDimensionless(pszLeftOffset);
	}
	else
	{
		m_iLeftOffset = m_pLayout->getGraphics()->convertDimension(defaultOffset.c_str());
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iLeftOffsetLayoutUnits = UT_convertToLayoutUnits(defaultOffset.c_str());
#endif
		m_dLeftOffsetUserUnits = UT_convertDimensionless(defaultOffset.c_str());
	}

	if(pszTopOffset && pszTopOffset[0])
	{
		m_iTopOffset = m_pLayout->getGraphics()->convertDimension(pszTopOffset);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iTopOffsetLayoutUnits = UT_convertToLayoutUnits(pszTopOffset);
#endif
		m_dTopOffsetUserUnits = UT_convertDimensionless(pszTopOffset);
	}
	else
	{
		m_iTopOffset = m_pLayout->getGraphics()->convertDimension(defaultOffset.c_str());
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iTopOffsetLayoutUnits = UT_convertToLayoutUnits(defaultOffset.c_str());
#endif
		m_dTopOffsetUserUnits = UT_convertDimensionless(defaultOffset.c_str());
	}

	if(pszRightOffset && pszRightOffset[0])
	{
		m_iRightOffset = m_pLayout->getGraphics()->convertDimension(pszRightOffset);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iRightOffsetLayoutUnits = UT_convertToLayoutUnits(pszRightOffset);
#endif
		m_dRightOffsetUserUnits = UT_convertDimensionless(pszRightOffset);
	}
	else
	{
		m_iRightOffset = m_pLayout->getGraphics()->convertDimension(defaultOffset.c_str());
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iRightOffsetLayoutUnits = UT_convertToLayoutUnits(defaultOffset.c_str());
#endif
		m_dRightOffsetUserUnits = UT_convertDimensionless(defaultOffset.c_str());
	}

	if(pszBottomOffset && pszBottomOffset[0])
	{
		m_iBottomOffset = m_pLayout->getGraphics()->convertDimension(pszBottomOffset);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iBottomOffsetLayoutUnits = UT_convertToLayoutUnits(pszBottomOffset);
#endif
		m_dBottomOffsetUserUnits = UT_convertDimensionless(pszBottomOffset);
	}
	else
	{
		m_iBottomOffset = m_pLayout->getGraphics()->convertDimension(defaultOffset.c_str());
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iBottomOffsetLayoutUnits = UT_convertToLayoutUnits(defaultOffset.c_str());
#endif
		m_dBottomOffsetUserUnits = UT_convertDimensionless(defaultOffset.c_str());
	}
	const char * pszLineThick = NULL;
	pSectionAP->getProperty("table-line-thickness", (const XML_Char *&)pszLineThick);
	if(pszLineThick && *pszLineThick)
	{
		m_iLineThickness = m_pLayout->getGraphics()->convertDimension(pszLineThick);
	}
	else
	{
		m_iLineThickness = m_pLayout->getGraphics()->convertDimension("0.8pt");
		if(m_iLineThickness < 1)
		{
			m_iLineThickness = 1;
		}
	}
	UT_DEBUGMSG(("SEVIOR: TableLayout::_lookup lineThickness %d \n",m_iLineThickness));
	const char * pszTableColSpacing = NULL;
	const char * pszTableRowSpacing = NULL;
	pSectionAP->getProperty("table-col-spacing", (const XML_Char *&)pszTableColSpacing);
	pSectionAP->getProperty("table-row-spacing", (const XML_Char *&)pszTableRowSpacing);
	if(pszTableColSpacing && *pszTableColSpacing)
	{
//
// Note to TF from MS. It was too hard to propagate 3 different sets of layout units
// into the Table Layout code so what I did instead was the worst of all worlds.
//
// Structures are layed horizontally in LAYOUT units and vertically in screen units.
// That is what all those SCALE_TO_SCREEN macros are about. We should think through
// how to transition to pango layout.
//
// Anyway column spacing being horizontal is layout units.
//
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iColSpacing = UT_convertToLayoutUnits(pszTableColSpacing);
#else
		m_iColSpacing = m_pLayout->getGraphics()->convertDimension(pszTableColSpacing);
#endif
	}
	else
	{
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iColSpacing =  UT_convertToLayoutUnits("0.02in"); // was 0.05in
#else
		m_iColSpacing = m_pLayout->getGraphics()->convertDimension("0.02in");
#endif
	}
	if(pszTableRowSpacing && *pszTableRowSpacing)
	{
//
// Row spacing being vertical is screen units
//
		m_iRowSpacing = m_pLayout->getGraphics()->convertDimension(pszTableRowSpacing);
	}
	else
	{
		m_iRowSpacing = m_pLayout->getGraphics()->convertDimension("0.01in");
	}
//
// Positioned columns controls
//
	const char * pszLeftColPos = NULL;
	const char * pszColumnProps = NULL;
	pSectionAP->getProperty("table-column-leftpos", (const XML_Char *&)pszLeftColPos);
	pSectionAP->getProperty("table-column-props", (const XML_Char *&)pszColumnProps);
	if(pszLeftColPos && *pszLeftColPos)
	{
//
// Anyway column positioning being horizontal is layout units.
//
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iLeftColPos = UT_convertToLayoutUnits(pszLeftColPos);
#else
		m_iLeftColPos = m_pLayout->getGraphics()->convertDimension(pszLeftColPos);
#endif
	}
	else
	{
		m_iLeftColPos =  0;
	}
	if(pszColumnProps && *pszColumnProps)
	{
/*
   These will be properties applied to all columns. To start with, just the 
    widths of each column are specifed. These are translated to layout units.
 
   The format of the string of properties is:

   table-column-props:1.2in/3.0in/1.3in/;

   So we read back in pszColumnProps
   1.2in/3.0in/1.3in/

   The "/" characters will be used to delineate different column entries.
   As new properties for each column are defined these will be delineated with "_"
   characters. But we'll cross that bridge later.
*/
		UT_DEBUGMSG(("Processing Column width string %s \n",pszColumnProps));
		UT_VECTOR_PURGEALL(fl_ColProps *,m_vecColProps);
		m_vecColProps.clear();
		UT_String sProps = pszColumnProps;
		UT_sint32 sizes = sProps.size();
		UT_sint32 i =0;
		UT_sint32 j =0;
		while(i < sizes)
		{
			for (j=i; (j<sizes) && (sProps[j] != '/') ; j++) {}
			if((j+1)>i && sProps[j] == '/')
			{
				UT_String sSub = sProps.substr(i,(j-i));
				i = j + 1;
				fl_ColProps * pColP = new fl_ColProps;
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
				pColP->m_iColWidth = UT_convertToLayoutUnits(sSub.c_str());
#else
				pColP->m_iColWidth = m_pLayout->getGraphics()->convertDimension(sSub.c_str());
#endif
				m_vecColProps.addItem((void *) pColP);
				UT_DEBUGMSG(("SEVIOR: width char %s width layout %d \n",sSub.c_str(),pColP->m_iColWidth));
			}
		}
 	}
	else
	{
		UT_VECTOR_PURGEALL(fl_ColProps *,m_vecColProps);
		m_vecColProps.clear();
	}
}

UT_sint32 fl_TableLayout::getColSpacing(void) const
{
	return m_iColSpacing;
}


UT_sint32 fl_TableLayout::getRowSpacing(void) const
{
	return m_iRowSpacing;
}

UT_sint32 fl_TableLayout::getLineThickness(void) const
{
	return m_iLineThickness;
}

UT_sint32 fl_TableLayout::getTopOffset(void) const
{
	return m_iTopOffset;
}

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32 fl_TableLayout::getTopOffsetInLayoutUnits(void) const
{
	return m_iTopOffsetLayoutUnits;
}
#endif

UT_sint32 fl_TableLayout::getBottomOffset(void) const
{
	return m_iBottomOffset;
}

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32 fl_TableLayout::getBottomOffsetInLayoutUnits(void) const
{
	return m_iBottomOffsetLayoutUnits;
}
#endif

UT_sint32   fl_TableLayout::getLeftOffset(void) const
{
	return m_iLeftOffset;
}

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32  fl_TableLayout::getLeftOffsetInLayoutUnits(void) const
{
	return m_iLeftOffsetLayoutUnits;
}
#endif
UT_sint32   fl_TableLayout::getRightOffset(void) const
{
	return m_iRightOffset;
}

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32   fl_TableLayout::getRightOffsetInLayoutUnits(void) const
{
	return m_iRightOffsetLayoutUnits;
}
#endif


void fl_TableLayout::collapse(void)
{
	// Clear all our Tables
	fp_TableContainer *pTab = (fp_TableContainer *) getFirstContainer();
	if (pTab)
	{
		pTab->clearScreen();
	}

	// get rid of all the layout information for every containerLayout
	fl_ContainerLayout*	pCL = getFirstLayout();
	while (pCL)
	{
		pCL->collapse();
		pCL = pCL->getNext();
	}

	if(pTab)
	{
//
// Remove from the container it comes from
//
		fp_VerticalContainer * pUpCon = (fp_VerticalContainer *)pTab->getContainer();
		pUpCon->removeContainer(pTab);
		delete pTab;
	}
	setFirstContainer(NULL);
	setLastContainer(NULL);
}

bool fl_TableLayout::doclistener_deleteStrux(const PX_ChangeRecord_Strux * pcrx)
{
	UT_ASSERT(pcrx->getType()==PX_ChangeRecord::PXT_DeleteStrux);
	UT_ASSERT(pcrx->getStruxType()== PTX_SectionTable);

	UT_DEBUGMSG(("SEVIOR: !!!!!!!! Doing table delete strux!! \n"));
	fl_ContainerLayout * pPrev = getPrev();
	fl_ContainerLayout * pNext = getNext();

	collapse();

	if(pPrev != NULL)
	{
		pPrev->setNext(pNext);
	}
	else
	{
		myContainingLayout()->setFirstLayout(pNext);
	}
	if(pNext != NULL)
	{
		pNext->setPrev(pPrev);
	}
	else
	{
		myContainingLayout()->setLastLayout(pPrev);
	}

	delete this;			// TODO whoa!  this construct is VERY dangerous.

	return true;
}

/*!
 * Return the position of the table strux. 
 */
PT_DocPosition fl_TableLayout::getPosition(bool bActualBlockPosition /* = false */) const
{
	return fl_ContainerLayout::getPosition(bActualBlockPosition);
}

/*!
 * This method attaches pCell to the current tablecontainer.
 */
void fl_TableLayout::attachCell(fl_ContainerLayout * pCell)
{
	//
	// Verify the cell layout is in the table.
    //
	fl_ContainerLayout * pCur = getFirstLayout();
	while(pCur && pCur !=  pCell)
	{
		xxx_UT_DEBUGMSG(("SEVIOR: Looking for %x found %x \n",pCell,pCur));
		pCur = pCur->getNext();
	}
	if(pCur == NULL)
	{
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return;
	}
	fp_TableContainer * pTab = (fp_TableContainer *) getLastContainer();
	UT_ASSERT(pTab);
	pTab->tableAttach((fp_CellContainer *) pCell->getLastContainer());
	setDirty();
}

/*!
 * This method removes all layout structures contained by this layout
 * structure.
 */
void fl_TableLayout::_purgeLayout(void)
{
	fl_ContainerLayout * pCL = getFirstLayout();
	while(pCL)
	{
		fl_ContainerLayout * pNext = pCL->getNext();
		delete pCL;
		pCL = pNext;
	}
}

//------------------------------------------------------------------

fl_CellLayout::fl_CellLayout(FL_DocLayout* pLayout, PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP, fl_ContainerLayout * pMyContainerLayout)
	: fl_SectionLayout(pLayout, sdh, indexAP, FL_SECTION_CELL,FL_CONTAINER_CELL,PTX_SectionCell,pMyContainerLayout),
	  m_bNeedsRebuild(false),
	  m_iLeftOffset(0),
	  m_dLeftOffsetUserUnits(0.0),
	  m_iRightOffset(0),
	  m_dRightOffsetUserUnits(0.0),
	  m_iTopOffset(0),
	  m_dTopOffsetUserUnits(0.0),
	  m_iBottomOffset(0),
	  m_dBottomOffsetUserUnits(0.0),
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	  m_iLeftOffsetLayoutUnits(0),
	  m_iRightOffsetLayoutUnits(0),
	  m_iTopOffsetLayoutUnits(0),
	  m_iBottomOffsetLayoutUnits(0),
#endif
	  m_iLeftAttach(0),
	  m_iRightAttach(1),
	  m_iTopAttach(0),
	  m_iBottomAttach(1),
	  m_bCellPositionedOnPage(false),
	  m_iCellHeight(0),
	  m_cLeftColor(UT_RGBColor(0,0,0)),
	  m_cRightColor(UT_RGBColor(0,0,0)),
	  m_cTopColor(UT_RGBColor(0,0,0)),
	  m_cBottomColor(UT_RGBColor(0,0,0)),
	  m_iLeftStyle(LS_NORMAL),
	  m_iRightStyle(LS_NORMAL),
	  m_iTopStyle(LS_NORMAL),
	  m_iBottomStyle(LS_NORMAL),
	  m_iBgStyle(FS_OFF),
	  m_iLeftLineThickness(-1),
	  m_iTopLineThickness(-1),
	  m_iRightLineThickness(-1),
	  m_iBottomLineThickness(-1)
{
	createCellContainer();
}

fl_CellLayout::~fl_CellLayout()
{
	// NB: be careful about the order of these
	_purgeLayout();
	fp_CellContainer * pTC = (fp_CellContainer *) getFirstContainer();
	while(pTC)
	{
		fp_CellContainer * pNext = (fp_CellContainer *) pTC->getNext();
		if(pTC == (fp_CellContainer *) getLastContainer())
		{
			pNext = NULL;
		}
		delete pTC;
		pTC = pNext;
	}

	setFirstContainer(NULL);
	setLastContainer(NULL);
}

/*!
 * This method creates a new cell with it's properties initially set
 * from the Attributes/properties of this Layout
 */
void fl_CellLayout::createCellContainer(void)
{
	_lookupProperties();
	fp_CellContainer * pCellContainer = new fp_CellContainer((fl_SectionLayout *) this);
	setFirstContainer(pCellContainer);
	setLastContainer(pCellContainer);
	setCellContainerProperties(pCellContainer);
	fl_ContainerLayout * pCL = myContainingLayout();
	while(pCL!= NULL && pCL->getContainerType() != FL_CONTAINER_DOCSECTION)
	{
		pCL = pCL->myContainingLayout();
	}
	fl_DocSectionLayout * pDSL = static_cast<fl_DocSectionLayout *>(pCL);
	UT_ASSERT(pDSL != NULL);
	UT_sint32 iWidth = pDSL->getFirstContainer()->getPage()->getWidth();
	pCellContainer->setWidth(iWidth);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	UT_sint32 iWidthLayout = pDSL->getFirstContainer()->getPage()->getWidthInLayoutUnits() - pDSL->getLeftMarginInLayoutUnits() - pDSL->getRightMarginInLayoutUnits();
	xxx_UT_DEBUGMSG(("SEVIOR: Setting initial width of cell %x to %d \n",pCellContainer,iWidthLayout));
	pCellContainer->setWidthInLayoutUnits(iWidthLayout);
#endif
}


/*!
 * This method sets all the parameters of the cell container from
 * properties of this section 
 */
void fl_CellLayout::setCellContainerProperties(fp_CellContainer * pCell)
{
	pCell->setLeftAttach(m_iLeftAttach);
	pCell->setRightAttach(m_iRightAttach);
	pCell->setTopAttach(m_iTopAttach);
	pCell->setBottomAttach(m_iBottomAttach);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	pCell->setLeftPad(m_iLeftOffsetLayoutUnits);
	pCell->setRightPad(m_iRightOffsetLayoutUnits);
#else
	pCell->setLeftPad(m_iLeftOffset);
	pCell->setRightPad(m_iRightOffset);
#endif
	pCell->setTopPad(m_iTopOffset);
	pCell->setBotPad(m_iBottomOffset);
	pCell->setLeftColor(m_cLeftColor);
	pCell->setRightColor(m_cRightColor);
	pCell->setTopColor(m_cTopColor);
	pCell->setBottomColor(m_cBottomColor);
	pCell->setBgColor(m_cBgColor);
	pCell->setLeftStyle(m_iLeftStyle);
	pCell->setRightStyle(m_iRightStyle);
	pCell->setTopStyle(m_iTopStyle);
	pCell->setBottomStyle(m_iBottomStyle);
	pCell->setBgStyle(m_iBgStyle);
	
	pCell->setLeftThickness(m_iLeftLineThickness);
	pCell->setTopThickness(m_iTopLineThickness);
	pCell->setRightThickness(m_iRightLineThickness);
	pCell->setBottomThickness(m_iBottomLineThickness);
}

/*!
 * This method measures the cell height and compares it to the previously
 * measured height. If they disagree update the layout of the table.
 */
void fl_CellLayout::checkAndAdjustCellSize(void)
{
	fp_CellContainer * pCell = (fp_CellContainer *) getFirstContainer();
	if(pCell == NULL)
	{
		return;
	}
	fp_Requisition Req;
	pCell->sizeRequest(&Req);
	if(Req.height == m_iCellHeight)
	{
		xxx_UT_DEBUGMSG(("checkandadjustcellheight: Heights identical return %d \n",m_iCellHeight));
		return;
	}
	m_iCellHeight = Req.height;
	xxx_UT_DEBUGMSG(("checkandadjustcellheight: Heights differ format %d %d \n",m_iCellHeight,Req.height));
	static_cast<fl_TableLayout *>(myContainingLayout())->setDirty();
	myContainingLayout()->format();
}
	
bool fl_CellLayout::bl_doclistener_insertCell(fl_ContainerLayout* pCell,
											  const PX_ChangeRecord_Strux * pcrx,
											  PL_StruxDocHandle sdh,
											  PL_ListenerId lid,
											  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
																	  PL_ListenerId lid,
																	  PL_StruxFmtHandle sfhNew))
{
	fl_ContainerLayout * pNewCL = NULL;
	fl_TableLayout * pTL = (fl_TableLayout *) myContainingLayout();
	pNewCL = pTL->insert(sdh,pCell,pcrx->getIndexAP(), FL_CONTAINER_CELL);
	
		// Must call the bind function to complete the exchange of handles
		// with the document (piece table) *** before *** anything tries
		// to call down into the document (like all of the view
		// listeners).
		
	PL_StruxFmtHandle sfhNew = (PL_StruxFmtHandle)pNewCL;
	pfnBindHandles(sdh,lid,sfhNew);

	fl_CellLayout * pCL = (fl_CellLayout *) pNewCL;
	pTL->attachCell(pCL);

//
// increment the insertion point in the view.
//
	FV_View* pView = m_pLayout->getView();
	if (pView && (pView->isActive() || pView->isPreview()))
	{
		pView->setPoint(pcrx->getPosition() +  fl_BLOCK_STRUX_OFFSET);
	}
	else if(pView && pView->getPoint() > pcrx->getPosition())
	{
		pView->setPoint(pView->getPoint() +  fl_BLOCK_STRUX_OFFSET);
	}
	return true;
}

	
bool fl_CellLayout::bl_doclistener_insertEndCell(fl_ContainerLayout*,
											  const PX_ChangeRecord_Strux * pcrx,
											  PL_StruxDocHandle sdh,
											  PL_ListenerId lid,
											  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
																	  PL_ListenerId lid,
																	  PL_StruxFmtHandle sfhNew))
{
	// The endCell strux actually needs a format handle to to this cell layout.
	// so we bind to this layout.

		
	PL_StruxFmtHandle sfhNew = (PL_StruxFmtHandle) this;
	pfnBindHandles(sdh,lid,sfhNew);

//
// increment the insertion point in the view.
//
	FV_View* pView = m_pLayout->getView();
	if (pView && (pView->isActive() || pView->isPreview()))
	{
		pView->setPoint(pcrx->getPosition() +  fl_BLOCK_STRUX_OFFSET);
	}
	else if(pView && pView->getPoint() > pcrx->getPosition())
	{
		pView->setPoint(pView->getPoint() +  fl_BLOCK_STRUX_OFFSET);
	}
	return true;
}

fl_SectionLayout * fl_CellLayout::getSectionLayout(void) const
{
	fl_ContainerLayout * pDSL = myContainingLayout();
	while(pDSL)
	{
		if(pDSL->getContainerType() == FL_CONTAINER_DOCSECTION)
		{
			return (fl_SectionLayout *) pDSL;
		}
		pDSL = pDSL->myContainingLayout();
	}
	return NULL;
}

/*!
  Create a new Cell containers and plug it into the linked list of Cell 
  containers.
  \params If pPrevCell is non-null place the new cell after this in the linked
          list, otherwise just append it to the end.
  \return The newly created Cell container
*/
fp_Container* fl_CellLayout::getNewContainer(fp_Container * pPrev)
{
//
// One cell container per cell layout
//
	UT_ASSERT(pPrev == NULL);
	UT_ASSERT((getFirstContainer() == NULL) && (getLastContainer()==NULL));
	createCellContainer();
	setCellContainerProperties((fp_CellContainer * ) getLastContainer());
	return (fp_Container *) getLastContainer();
}


void fl_CellLayout::format(void)
{
	xxx_UT_DEBUGMSG(("SEVIOR: Formatting first container is %x \n",getFirstContainer()));
	if(getFirstContainer() == NULL)
	{
		getNewContainer(NULL);
	}
	fl_ContainerLayout * pPrevCL = myContainingLayout()->getPrev();
	fp_Page * pPrevP = NULL;
	if(pPrevCL)
	{
		fp_Container * pPrevCon = pPrevCL->getFirstContainer();
		if(pPrevCon)
		{
			pPrevP = pPrevCon->getPage();
		}
	}
	static_cast<fl_DocSectionLayout *>(getSectionLayout())->setNeedsSectionBreak(true,pPrevP);
	fl_ContainerLayout*	pBL = getFirstLayout();
	while (pBL)
	{
		pBL->format();
		UT_sint32 count = 0;
		while(pBL->getLastContainer() == NULL || pBL->getFirstContainer()==NULL)
		{
			UT_DEBUGMSG(("Error formatting a block try again \n"));
			count = count + 1;
			pBL->format();
			if(count > 3)
			{
				UT_DEBUGMSG(("Give up trying to format. Hope for the best :-( \n"));
				break;
			}
		}
		pBL = pBL->getNext();
	}
	static_cast<fp_CellContainer *>(getFirstContainer())->layout();
	m_bNeedsReformat = false;
	checkAndAdjustCellSize();
}

void fl_CellLayout::markAllRunsDirty(void)
{
	fl_ContainerLayout*	pCL = getFirstLayout();
	while (pCL)
	{
		pCL->markAllRunsDirty();
		pCL = pCL->getNext();
	}
}

void fl_CellLayout::updateLayout(void)
{
	fl_ContainerLayout*	pBL = getFirstLayout();
	bool bNeedsFormat = false;
	xxx_UT_DEBUGMSG(("updateCellLayout \n"));
	while (pBL)
	{
		if (pBL->needsReformat())
		{
			pBL->format();
			bNeedsFormat = true;
			xxx_UT_DEBUGMSG(("updateCellLayout formatting sub block \n"));
		}

		pBL = pBL->getNext();
	}
	if(bNeedsFormat)
	{
		xxx_UT_DEBUGMSG(("updateCellLayout formatting whole cell \n"));
		format();
	}
}

void fl_CellLayout::redrawUpdate(void)
{
	fl_ContainerLayout*	pBL = getFirstLayout();
	while (pBL)
	{
		if (pBL->needsRedraw())
		{
			pBL->redrawUpdate();
		}

		pBL = pBL->getNext();
	}
	m_bNeedsRedraw = false;
}

bool fl_CellLayout::doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc)
{
	UT_ASSERT(pcrxc->getType()==PX_ChangeRecord::PXT_ChangeStrux);


	setAttrPropIndex(pcrxc->getIndexAP());
//	fl_TableLayout * pTL = (fl_TableLayout *) myContainingLayout();
	collapse();
//	pTL->collapse();
	_updateCell();
//	pTL->updateTable(); // may not need this
	return true;
}


void fl_CellLayout::_updateCell(void)
{

	const PP_AttrProp* pAP = NULL;
	bool bres = m_pDoc->getAttrProp(m_apIndex, &pAP);
	UT_ASSERT(bres);

	_lookupProperties();

	// clear all the columns
    // Assume that all formatting have already been removed via a 
    // collapse()
    //

	/*
	  TODO to more closely mirror the architecture we're using for BlockLayout, this code
	  should probably just set a flag, indicating the need to reformat this section.  Then,
	  when it's time to update everything, we'll actually do the format.
	*/

	FV_View * pView = m_pLayout->getView();
	if(pView)
	{
		pView->setScreenUpdateOnGeneralUpdate(false);
	}

	format();
	markAllRunsDirty();

	if(pView)
	{
		pView->setScreenUpdateOnGeneralUpdate(true);
	}

	return;
}

bool fl_CellLayout::recalculateFields(UT_uint32 iUpdateCount)
{
	fl_ContainerLayout*	pBL = getFirstLayout();
	while (pBL)
	{
		pBL->recalculateFields(iUpdateCount);
		pBL = pBL->getNext();
	}
	return true;
}

void fl_CellLayout::_lookupProperties(void)
{
	const PP_AttrProp* pSectionAP = NULL;

	m_pLayout->getDocument()->getAttrProp(m_apIndex, &pSectionAP);
	xxx_UT_DEBUGMSG(("SEVIOR: indexAp in Cell Layout %d \n",m_apIndex));
	/*
	  TODO shouldn't we be using PP_evalProperty like
	  the blockLayout does?

	  Yes, since PP_evalProperty does a fallback to the
	  last-chance defaults, whereas the code below is
	  hard-coding its own defaults.  Bad idea.
	*/

	const char* pszLeftOffset = NULL;
	const char* pszTopOffset = NULL;
	const char* pszRightOffset = NULL;
	const char* pszBottomOffset = NULL;
	pSectionAP->getProperty("cell-margin-left", (const XML_Char *&)pszLeftOffset);
	pSectionAP->getProperty("cell-margin-top", (const XML_Char *&)pszTopOffset);
	pSectionAP->getProperty("cell-margin-right", (const XML_Char *&)pszRightOffset);
	pSectionAP->getProperty("cell-margin-bottom", (const XML_Char *&)pszBottomOffset);
	const XML_Char * szRulerUnits;
	UT_Dimension dim;
	if (XAP_App::getApp()->getPrefsValue(AP_PREF_KEY_RulerUnits,&szRulerUnits))
		dim = UT_determineDimension(szRulerUnits);
	else
		dim = DIM_IN;

	UT_String defaultOffset;
	switch(dim)
	{
	case DIM_IN:
		defaultOffset = "0.0in"; //was 0.05in
		break;

	case DIM_CM:
		defaultOffset = "0.0cm";
		break;

	case DIM_PI:
		defaultOffset = "0.0pi";
		break;

	case DIM_PT:
		defaultOffset= "0.0pt";
		break;

	case DIM_MM:
		defaultOffset= "0.0mm"; //was 1.27
		break;

		// TODO: PX, and PERCENT
		// let them fall through to the default now
		// and we don't use them anyway
#if 0
	case DIM_PX:
	case DIM_PERCENT:
#endif
	case DIM_none:
	default:
		defaultOffset = "0.0in";	// TODO: what to do with this. was 0.05in
		break;

	}
	defaultOffset = "0.002in";
	if(pszLeftOffset && pszLeftOffset[0])
	{
		m_iLeftOffset = m_pLayout->getGraphics()->convertDimension(pszLeftOffset);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iLeftOffsetLayoutUnits = UT_convertToLayoutUnits(pszLeftOffset);
#endif
		m_dLeftOffsetUserUnits = UT_convertDimensionless(pszLeftOffset);
	}
	else
	{
		m_iLeftOffset = m_pLayout->getGraphics()->convertDimension(defaultOffset.c_str());
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iLeftOffsetLayoutUnits = UT_convertToLayoutUnits(defaultOffset.c_str());
#endif
		m_dLeftOffsetUserUnits = UT_convertDimensionless(defaultOffset.c_str());
	}

	if(pszTopOffset && pszTopOffset[0])
	{
		m_iTopOffset = m_pLayout->getGraphics()->convertDimension(pszTopOffset);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iTopOffsetLayoutUnits = UT_convertToLayoutUnits(pszTopOffset);
#endif
		m_dTopOffsetUserUnits = UT_convertDimensionless(pszTopOffset);
	}
	else
	{
		m_iTopOffset = m_pLayout->getGraphics()->convertDimension(defaultOffset.c_str());
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iTopOffsetLayoutUnits = UT_convertToLayoutUnits(defaultOffset.c_str());
#endif
		m_dTopOffsetUserUnits = UT_convertDimensionless(defaultOffset.c_str());
	}

	if(pszRightOffset && pszRightOffset[0])
	{
		m_iRightOffset = m_pLayout->getGraphics()->convertDimension(pszRightOffset);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iRightOffsetLayoutUnits = UT_convertToLayoutUnits(pszRightOffset);
#endif
		m_dRightOffsetUserUnits = UT_convertDimensionless(pszRightOffset);
	}
	else
	{
		m_iRightOffset = m_pLayout->getGraphics()->convertDimension(defaultOffset.c_str());
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iRightOffsetLayoutUnits = UT_convertToLayoutUnits(defaultOffset.c_str());
#endif
		m_dRightOffsetUserUnits = UT_convertDimensionless(defaultOffset.c_str());
	}

	if(pszBottomOffset && pszBottomOffset[0])
	{
		m_iBottomOffset = m_pLayout->getGraphics()->convertDimension(pszBottomOffset);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iBottomOffsetLayoutUnits = UT_convertToLayoutUnits(pszBottomOffset);
#endif
		m_dBottomOffsetUserUnits = UT_convertDimensionless(pszBottomOffset);
	}
	else
	{
		m_iBottomOffset = m_pLayout->getGraphics()->convertDimension(defaultOffset.c_str());
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_iBottomOffsetLayoutUnits = UT_convertToLayoutUnits(defaultOffset.c_str());
#endif
		m_dBottomOffsetUserUnits = UT_convertDimensionless(defaultOffset.c_str());
	}
	const char* pszLeftAttach = NULL;
	const char* pszRightAttach = NULL;
	const char* pszTopAttach = NULL;
	const char* pszBottomAttach = NULL;
	pSectionAP->getProperty("left-attach", (const XML_Char *&)pszLeftAttach);
	pSectionAP->getProperty("right-attach", (const XML_Char *&)pszRightAttach);
	pSectionAP->getProperty("top-attach", (const XML_Char *&)pszTopAttach);
	pSectionAP->getProperty("bot-attach", (const XML_Char *&)pszBottomAttach);
	xxx_UT_DEBUGMSG(("CellLayout _lookupProps top %s bot %s left %s right %s \n",pszTopAttach,pszBottomAttach,pszLeftAttach,pszRightAttach)); 
	if(pszLeftAttach && pszLeftAttach[0])
	{
		m_iLeftAttach = atoi(pszLeftAttach);
	}
	else
	{
		m_iLeftAttach = 0;
	}
	if(pszRightAttach && pszRightAttach[0])
	{
		m_iRightAttach = atoi(pszRightAttach);
	}
	else
	{
		m_iRightAttach = m_iLeftAttach + 1;
	}
	if(pszTopAttach && pszTopAttach[0])
	{
		m_iTopAttach = atoi(pszTopAttach);
	}
	else
	{
		m_iTopAttach = 0;
	}
	if(pszBottomAttach && pszBottomAttach[0])
	{
		m_iBottomAttach = atoi(pszBottomAttach);
	}
	else
	{
		m_iBottomAttach = m_iTopAttach+1;
	}
	UT_RGBColor clrBlack(0,0,0);
	const char* pszLeftColor = NULL;
	const char* pszRightColor = NULL;
	const char* pszTopColor = NULL;
	const char* pszBottomColor = NULL;
	pSectionAP->getProperty("left-color", (const XML_Char *&)pszLeftColor);
	pSectionAP->getProperty("right-color", (const XML_Char *&)pszRightColor);
	pSectionAP->getProperty("top-color", (const XML_Char *&)pszTopColor);
	pSectionAP->getProperty("bot-color", (const XML_Char *&)pszBottomColor);
	if(pszLeftColor && pszLeftColor[0])
	{
		UT_parseColor(pszLeftColor, m_cLeftColor);
	}
	else
	{
		m_cLeftColor = clrBlack;
	}
	if(pszRightColor && pszRightColor[0])
	{
		UT_parseColor(pszRightColor, m_cRightColor);
	}
	else
	{
		m_cRightColor = clrBlack;
	}
	if(pszTopColor && pszTopColor[0])
	{
		UT_parseColor(pszTopColor, m_cTopColor);
	}
	else
	{
		m_cTopColor = clrBlack;
	}
	if(pszBottomColor && pszBottomColor[0])
	{
		UT_parseColor(pszBottomColor, m_cBottomColor);
	}
	else
	{
		m_cBottomColor = clrBlack;
	}
	const char* pszLeftStyle = NULL;
	const char* pszRightStyle = NULL;
	const char* pszTopStyle = NULL;
	const char* pszBottomStyle = NULL;
	pSectionAP->getProperty("left-style", (const XML_Char *&)pszLeftStyle);
	pSectionAP->getProperty("right-style", (const XML_Char *&)pszRightStyle);
	pSectionAP->getProperty("top-style", (const XML_Char *&)pszTopStyle);
	pSectionAP->getProperty("bot-style", (const XML_Char *&)pszBottomStyle);
	if(pszLeftStyle && pszLeftStyle[0])
	{
		m_iLeftStyle = atoi(pszLeftStyle);
	}	
	if(pszRightStyle && pszRightStyle[0])
	{
		m_iRightStyle = atoi(pszRightStyle);
	}
	if(pszTopStyle && pszTopStyle[0])
	{
		m_iTopStyle = atoi(pszTopStyle);
	}
	if(pszBottomStyle && pszBottomStyle[0])
	{
		m_iBottomStyle = atoi(pszBottomStyle);
	}
	const char* pszBgColor = NULL;
	pSectionAP->getProperty("bgcolor", (const XML_Char *&)pszBgColor);
	if(pszBgColor && pszBgColor[0])
	{
		UT_parseColor(pszBgColor, m_cBgColor);
	}
	else
	{
		m_cBgColor = UT_RGBColor(255,255,255); // fallback to something: it's not used anyway, cause bg-style would (most likely) be "off"
	}
	const char* pszBgStyle = NULL;
	pSectionAP->getProperty("bg-style", (const XML_Char *&)pszBgStyle);
	if(pszBgStyle && pszBgStyle[0])
	{
		m_iBgStyle = atoi(pszBgStyle);
	}
	const char *pszLeftLineThickness = NULL;
	const char *pszTopLineThickness = NULL;
	const char *pszRightLineThickness = NULL;
	const char *pszBottomLineThickness = NULL;
	pSectionAP->getProperty("left-thickness", (const XML_Char *&)pszLeftLineThickness);
	pSectionAP->getProperty("top-thickness", (const XML_Char *&)pszTopLineThickness);
	pSectionAP->getProperty("right-thickness", (const XML_Char *&)pszRightLineThickness);
	pSectionAP->getProperty("bot-thickness", (const XML_Char *&)pszBottomLineThickness);
	GR_Graphics * pG = m_pLayout->getGraphics();
	if (pszLeftLineThickness && pszLeftLineThickness[0]) 
	{
		m_iLeftLineThickness = pG->convertDimension(pszLeftLineThickness);
		double res = UT_convertToInches(pszLeftLineThickness);
		if(m_iLeftLineThickness == 0 && (res > 0.0001))
		{
			m_iLeftLineThickness = 1;
		}
	}
	else
	{
		m_iLeftLineThickness = -1;
	}
	if (pszTopLineThickness && pszTopLineThickness[0]) 
	{
		m_iTopLineThickness = pG->convertDimension(pszTopLineThickness);
		double res = UT_convertToInches(pszTopLineThickness);
		if(m_iTopLineThickness == 0 && (res > 0.0001))
		{
			m_iTopLineThickness = 1;
		}
	}
	else
	{
		m_iTopLineThickness = -1;
	}
	if (pszRightLineThickness && pszRightLineThickness[0]) 
	{
		m_iRightLineThickness = pG->convertDimension(pszRightLineThickness);
		double res = UT_convertToInches(pszRightLineThickness);
		if(m_iRightLineThickness == 0 && (res > 0.0001))
		{
			m_iRightLineThickness = 1;
		}
	}
	else
	{
		m_iRightLineThickness = -1;
	}
	if (pszBottomLineThickness && pszBottomLineThickness[0]) 
	{
		m_iBottomLineThickness = pG->convertDimension(pszBottomLineThickness);
		double res = UT_convertToInches(pszBottomLineThickness);
		if(m_iBottomLineThickness == 0 && (res > 0.0001))
		{
			m_iBottomLineThickness = 1;
		}
	}
	else
	{
		m_iBottomLineThickness = -1;
	}
}

UT_sint32   fl_CellLayout::getLeftOffset(void) const
{
	return m_iLeftOffset;
}

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32  fl_CellLayout::getLeftOffsetInLayoutUnits(void) const
{
	return m_iLeftOffsetLayoutUnits;
}
#endif
UT_sint32   fl_CellLayout::getRightOffset(void) const
{
	return m_iRightOffset;
}
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32   fl_CellLayout::getRightOffsetInLayoutUnits(void) const
{
	return m_iRightOffsetLayoutUnits;
}
#endif

UT_sint32 fl_CellLayout::getTopOffset(void) const
{
	return m_iTopOffset;
}


#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32 fl_CellLayout::getTopOffsetInLayoutUnits(void) const
{
	return m_iTopOffsetLayoutUnits;
}
#endif

UT_sint32 fl_CellLayout::getBottomOffset(void) const
{
	return m_iBottomOffset;
}

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32 fl_CellLayout::getBottomOffsetInLayoutUnits(void) const
{
	return m_iBottomOffsetLayoutUnits;
}
#endif


void fl_CellLayout::_localCollapse(void)
{

	// ClearScreen on our Cell. One Cell per layout.

	fp_CellContainer *pCell = (fp_CellContainer *) getFirstContainer();
	xxx_UT_DEBUGMSG(("SEVIOR: Local collapse of CellLayout %x CellContainer %x \n",this,pCell));
	if (pCell)
	{
		pCell->clearScreen();
	}

	// get rid of all the layout information for every containerLayout
	fl_ContainerLayout*	pCL = getFirstLayout();
	while (pCL != NULL)
	{
		xxx_UT_DEBUGMSG(("SEVIOR: Local collapse of CellLayout %x Contained Layout %x \n",this,pCL));
		pCL->collapse();
		pCL = pCL->getNext();
	}
}

void fl_CellLayout::collapse(void)
{
	_localCollapse();

	// Delete our Cell. One Cell per layout.

	fp_CellContainer *pCell = (fp_CellContainer *) getFirstContainer();
//
// Remove it from the table container
//
	if (pCell)
	{
		fp_TableContainer * pTabCon = (fp_TableContainer *) pCell->getContainer();
		if(pTabCon)
		{
			pTabCon->removeContainer(pCell);
		}
//
// remove it from the linked list.
//
		fp_CellContainer * pPrev = (fp_CellContainer *) pCell->getPrev();
		if(pPrev)
		{
			pPrev->setNext(pCell->getNext());
		}
		if(pCell->getNext())
		{
			pCell->getNext()->setPrev(pPrev);
		}
		delete pCell;
	}
	setFirstContainer(NULL);
	setLastContainer(NULL);
}

bool fl_CellLayout::doclistener_deleteStrux(const PX_ChangeRecord_Strux * pcrx)
{
	UT_ASSERT(pcrx->getType()==PX_ChangeRecord::PXT_DeleteStrux);
	UT_ASSERT(pcrx->getStruxType()== PTX_SectionCell);


	fl_ContainerLayout * pPrev = getPrev();
	fl_ContainerLayout * pNext = getNext();

	collapse();
//	fl_TableLayout * pTL = (fl_TableLayout *) myContainingLayout();
//	pTL->collapse();
	if(pPrev != NULL)
	{
		pPrev->setNext(pNext);
	}
	else
	{
		myContainingLayout()->setFirstLayout(pNext);
	}
	if(pNext != NULL)
	{
		pNext->setPrev(pPrev);
	}
	else
	{
		myContainingLayout()->setLastLayout(pPrev);
	}
//	pTL->updateTable(); // may not need this. FIXME check if we do!
	delete this;			// TODO whoa!  this construct is VERY dangerous.

	return true;
}

/*!
 * This method removes all layout structures contained by this layout
 * structure.
 */
void fl_CellLayout::_purgeLayout(void)
{
	fl_ContainerLayout * pCL = getFirstLayout();
	while(pCL)
	{
		fl_ContainerLayout * pNext = pCL->getNext();
		delete pCL;
		pCL = pNext;
	}
}

