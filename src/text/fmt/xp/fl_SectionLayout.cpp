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

#include <string.h>
#include <stdlib.h>

#include "ut_types.h"
#include "ut_string.h"

#include "ap_Prefs.h"
#include "fl_SectionLayout.h"
#include "fl_Layout.h"
#include "fl_DocLayout.h"
#include "fl_BlockLayout.h"
#include "fb_LineBreaker.h"
#include "fp_Page.h"
#include "fp_Line.h"
#include "fp_Column.h"
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

#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_units.h"

/*
  TODO this file is now really too long.  divide it up
  into smaller ones.
*/

fl_SectionLayout::fl_SectionLayout(FL_DocLayout* pLayout, PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP, UT_uint32 iType)
	: fl_Layout(PTX_Section, sdh)
{
	UT_ASSERT(pLayout);

	m_iType = iType;
	
	m_pLayout = pLayout;
	m_pDoc = pLayout->getDocument();
	m_pLB = NULL;
	m_pFirstBlock = NULL;
	m_pLastBlock = NULL;
	m_pNext = NULL;
	m_pPrev = NULL;

	setAttrPropIndex(indexAP);
}

fl_SectionLayout::~fl_SectionLayout()
{
	if (m_pLB)
	{
		delete m_pLB;
	}
}

const char*	fl_SectionLayout::getAttribute(const char * pszName) const
{
	const PP_AttrProp * pAP = NULL;
	getAttrProp(&pAP);
	
	const XML_Char* pszAtt = NULL;
	pAP->getAttribute((XML_Char*)pszName, pszAtt);

	return pszAtt;
}

void fl_SectionLayout::setNext(fl_SectionLayout* pSL)
{
	m_pNext = pSL;
}

void fl_SectionLayout::setPrev(fl_SectionLayout* pSL)
{
	m_pPrev = pSL;
}

FL_DocLayout* fl_SectionLayout::getDocLayout(void) const
{
	return m_pLayout;
}

fl_BlockLayout * fl_SectionLayout::getFirstBlock(void) const
{
	return m_pFirstBlock;
}

fl_BlockLayout * fl_SectionLayout::getLastBlock(void) const
{
	return m_pLastBlock;
}

bool fl_SectionLayout::recalculateFields(void)
{
	bool bResult = false;
	
	fl_BlockLayout*	pBL = m_pFirstBlock;

	while (pBL)
	{
		bResult = pBL->recalculateFields() || bResult;

		pBL = pBL->getNext();
	}

	return bResult;
}

fl_BlockLayout * fl_SectionLayout::appendBlock(PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP)
{
	return insertBlock(sdh, m_pLastBlock, indexAP);
}

void fl_SectionLayout::addBlock(fl_BlockLayout* pBL)
{
	if (m_pLastBlock)
	{
		UT_ASSERT(m_pLastBlock->getNext() == NULL);
		
		pBL->setNext(NULL);
		pBL->setPrev(m_pLastBlock);
		m_pLastBlock->setNext(pBL);
		m_pLastBlock = pBL;
	}
	else
	{
		UT_ASSERT(!m_pFirstBlock);
		
		pBL->setNext(NULL);
		pBL->setPrev(NULL);
		m_pFirstBlock = pBL;
		m_pLastBlock = m_pFirstBlock;
	}
	pBL->setSectionLayout(this);
}

fl_BlockLayout * fl_SectionLayout::insertBlock(PL_StruxDocHandle sdh, fl_BlockLayout * pPrev, PT_AttrPropIndex indexAP)
{
	fl_BlockLayout*	pBL = new fl_BlockLayout(sdh, _getLineBreaker(), pPrev, this, indexAP);
	if (!pBL)
	{
		return pBL;
	}

	if (!m_pLastBlock)
	{
		UT_ASSERT(!m_pFirstBlock);
		m_pFirstBlock = pBL;
		m_pLastBlock = pBL;
	}
	else if (m_pLastBlock == pPrev)
	{
		m_pLastBlock = pBL;
	}
	else if (!pPrev)
	{
		m_pFirstBlock = pBL;
	}

	return pBL;
}

void fl_SectionLayout::removeBlock(fl_BlockLayout * pBL)
{
	UT_ASSERT(pBL);
	UT_ASSERT(m_pFirstBlock);
	
	if (pBL->getPrev())
	{
		pBL->getPrev()->setNext(pBL->getNext());
	}

	if (pBL->getNext())
	{
		pBL->getNext()->setPrev(pBL->getPrev());
		pBL->transferListFlags();
	}
	
	if (pBL == m_pFirstBlock)
	{
		m_pFirstBlock = m_pFirstBlock->getNext();
		if (!m_pFirstBlock)
		{
			m_pLastBlock = NULL;
		}
	}

	if (pBL == m_pLastBlock)
	{
		m_pLastBlock = m_pLastBlock->getPrev();
		if (!m_pLastBlock)
		{
			m_pFirstBlock = NULL;
		}
	}

	pBL->setNext(NULL);
	pBL->setPrev(NULL);
	pBL->setSectionLayout(NULL);
}

fb_LineBreaker * fl_SectionLayout::_getLineBreaker(void)
{
	if (!m_pLB)
	{
		fb_LineBreaker* slb = new fb_LineBreaker();

		m_pLB = slb;
	}

	UT_ASSERT(m_pLB);

	return m_pLB;
}

void fl_SectionLayout::_purgeLayout()
{
	fl_BlockLayout*	pBL = m_pLastBlock;

	while (pBL)
	{
		fl_BlockLayout* pNuke = pBL;

		pBL = pBL->getPrev();

		delete pNuke;
	}

	return;
}

bool fl_SectionLayout::bl_doclistener_populateSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs, PT_BlockOffset blockOffset, UT_uint32 len)
{
        if(pBL->getPrev()!= NULL && pBL->getPrev()->getLastLine()==NULL)
        {
	        UT_DEBUGMSG(("In bl_doclistner_pop no LastLine \n"));
	        UT_DEBUGMSG(("getPrev = %d this = %d \n",pBL->getPrev(),pBL));
		//  UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}

	return pBL->doclistener_populateSpan(pcrs, blockOffset, len);
}

bool fl_SectionLayout::bl_doclistener_populateObject(fl_BlockLayout* pBL, PT_BlockOffset blockOffset, const PX_ChangeRecord_Object * pcro)
{
	return pBL->doclistener_populateObject(blockOffset, pcro);
}
	
bool fl_SectionLayout::bl_doclistener_insertSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs)
{
	return pBL->doclistener_insertSpan(pcrs);
}

bool fl_SectionLayout::bl_doclistener_deleteSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs)
{
	return pBL->doclistener_deleteSpan(pcrs);
}

bool fl_SectionLayout::bl_doclistener_changeSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_SpanChange * pcrsc)
{
	return pBL->doclistener_changeSpan(pcrsc);
}

bool fl_SectionLayout::bl_doclistener_deleteStrux(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx)
{
	return pBL->doclistener_deleteStrux(pcrx);
}

bool fl_SectionLayout::bl_doclistener_changeStrux(fl_BlockLayout* pBL, const PX_ChangeRecord_StruxChange * pcrxc)
{
	return pBL->doclistener_changeStrux(pcrxc);
}

bool fl_SectionLayout::bl_doclistener_insertBlock(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx,
									PL_StruxDocHandle sdh,
									PL_ListenerId lid,
									void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
															PL_ListenerId lid,
															PL_StruxFmtHandle sfhNew))
{
	if (pBL)
	{
		return pBL->doclistener_insertBlock(pcrx, sdh, lid, pfnBindHandles);
	}
	else
	{
		// Insert the block at the beginning of the section
		fl_BlockLayout*	pNewBL = insertBlock(sdh, NULL, pcrx->getIndexAP());
		if (!pNewBL)
		{
			UT_DEBUGMSG(("no memory for BlockLayout\n"));
			return false;
		}

		return pNewBL->doclistener_insertFirstBlock(pcrx, sdh, 
													lid, pfnBindHandles);
	}
}

bool fl_SectionLayout::bl_doclistener_insertSection(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx,
									  PL_StruxDocHandle sdh,
									  PL_ListenerId lid,
									  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
															  PL_ListenerId lid,
															  PL_StruxFmtHandle sfhNew))
{
	return pBL->doclistener_insertSection(pcrx, sdh, lid, pfnBindHandles);
}

bool fl_SectionLayout::bl_doclistener_insertObject(fl_BlockLayout* pBL, const PX_ChangeRecord_Object * pcro)
{
	return pBL->doclistener_insertObject(pcro);
}

bool fl_SectionLayout::bl_doclistener_deleteObject(fl_BlockLayout* pBL, const PX_ChangeRecord_Object * pcro)
{
	return pBL->doclistener_deleteObject(pcro);
}

bool fl_SectionLayout::bl_doclistener_changeObject(fl_BlockLayout* pBL, const PX_ChangeRecord_ObjectChange * pcroc)
{
	return pBL->doclistener_changeObject(pcroc);
}

bool fl_SectionLayout::bl_doclistener_insertFmtMark(fl_BlockLayout* pBL, const PX_ChangeRecord_FmtMark * pcrfm)
{
	return pBL->doclistener_insertFmtMark(pcrfm);
}

bool fl_SectionLayout::bl_doclistener_deleteFmtMark(fl_BlockLayout* pBL, const PX_ChangeRecord_FmtMark * pcrfm)
{
	return pBL->doclistener_deleteFmtMark(pcrfm);
}

bool fl_SectionLayout::bl_doclistener_changeFmtMark(fl_BlockLayout* pBL, const PX_ChangeRecord_FmtMarkChange * pcrfmc)
{
	return pBL->doclistener_changeFmtMark(pcrfmc);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fl_DocSectionLayout::fl_DocSectionLayout(FL_DocLayout* pLayout, PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP)
	: fl_SectionLayout(pLayout, sdh, indexAP, FL_SECTION_DOC)
{
	m_pFirstColumn = NULL;
	m_pLastColumn = NULL;

	m_pHeaderSL = NULL;
	m_pFooterSL = NULL;
	m_pFirstOwnedPage = NULL;
	_lookupProperties();
}

fl_DocSectionLayout::~fl_DocSectionLayout()
{
	// NB: be careful about the order of these
	_purgeLayout();

	DELETEP(m_pHeaderSL);
	DELETEP(m_pFooterSL);
	
	fp_Column* pCol = m_pFirstColumn;
	while (pCol)
	{
		fp_Column* pNext = pCol->getNext();

		delete pCol;

		pCol = pNext;
	}
}

void fl_DocSectionLayout::setHdrFtr(UT_uint32 /*iType*/, fl_HdrFtrSectionLayout* pHFSL)
{
	// TODO use the type
	
	const char* pszID = pHFSL->getAttribute("id");

	const char* pszAtt = NULL;

	pszAtt = getAttribute("header");
	if (
		pszAtt
		&& (0 == UT_stricmp(pszAtt, pszID))
		)
	{
		m_pHeaderSL = pHFSL;
		return;
	}
	
	pszAtt = getAttribute("footer");
	if (
		pszAtt
		&& (0 == UT_stricmp(pszAtt, pszID))
		)
	{
		m_pFooterSL = pHFSL;
		return;
	}

	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

fl_HdrFtrSectionLayout*   fl_DocSectionLayout::getHeader(void)
{
        return m_pHeaderSL;
}

fl_HdrFtrSectionLayout*   fl_DocSectionLayout::getFooter(void)
{
        return m_pFooterSL;
}

fp_Container* fl_DocSectionLayout::getFirstContainer()
{
	return m_pFirstColumn;
}

fp_Container* fl_DocSectionLayout::getLastContainer()
{
	return m_pLastColumn;
}

fp_Container* fl_DocSectionLayout::getNewContainer(void)
{
	/*
	  This is called to create a new column (or row of same).
	*/
	fp_Page* pPage = NULL;
	fp_Column* pLastColumn = (fp_Column*) getLastContainer();
	fp_Column* pAfterColumn = NULL;
	
	if (pLastColumn)
	{
		fp_Page* pTmpPage = pLastColumn->getPage();
		if (pTmpPage->getNext())
		{
			pPage = pTmpPage->getNext();
		}
		else
		{
			pPage = m_pLayout->addNewPage(this);
		}
	}
	else
	{
		/*
		  We currently have no columns.  Time to create some.
		  If there is a previous section, then we need to
		  start our section right after that one.  If not, then
		  we start our section on the first page.  If there is no
		  first page, then we need to create one.
		*/
		fl_DocSectionLayout* pPrevSL = getPrevDocSection();
		if (pPrevSL)
		{
		  //
		  // Sevior this code should not be needed!
		  //
		        fp_Column * pPrevCol = (fp_Column *) pPrevSL->getLastContainer();
			if(pPrevCol == NULL)
			{
			        UT_DEBUGMSG(("BUG! BUG! Prev section has no last container! Attempting to fix this \n"));
				pPrevSL->format();
			}
			fp_Page* pTmpPage = pPrevSL->getLastContainer()->getPage();
			if (m_bForceNewPage)
			{
				if (pTmpPage->getNext())
				{
					pPage = pTmpPage->getNext();
				}
				else
				{
					pPage = m_pLayout->addNewPage(this);
				}
			}
			else
			{
				pPage = pTmpPage;
				pAfterColumn = pPage->getNthColumnLeader(pPage->countColumnLeaders()-1);
			}
		}
		else
		{
			if (m_pLayout->countPages() > 0)
			{
				pPage = m_pLayout->getFirstPage();
			}
			else
			{
				pPage = m_pLayout->addNewPage(this);
			}
		}
	}

	UT_ASSERT(pPage);

	fp_Column* pLeaderColumn = NULL;
	fp_Column* pTail = NULL;
	for (UT_uint32 i=0; i<m_iNumColumns; i++)
	{
		fp_Column* pCol = new fp_Column(this);

		if (pTail)
		{
			pCol->setLeader(pLeaderColumn);
			pTail->setFollower(pCol);
			pTail->setNext(pCol);
			pCol->setPrev(pTail);
			
			pTail = pCol;
		}
		else
		{
			pLeaderColumn = pTail = pCol;
			pLeaderColumn->setLeader(pLeaderColumn);
		}
	}

	fp_Column* pLastNewCol = pLeaderColumn;
	while (pLastNewCol->getFollower())
	{
		pLastNewCol = pLastNewCol->getFollower();
	}

	if (m_pLastColumn)
	{
		UT_ASSERT(m_pFirstColumn);

		m_pLastColumn->setNext(pLeaderColumn);
		pLeaderColumn->setPrev(m_pLastColumn);
	}
	else
	{
		UT_ASSERT(!m_pFirstColumn);
		
		m_pFirstColumn = pLeaderColumn;
	}
	
	m_pLastColumn = pLastNewCol;
	UT_ASSERT(!(m_pLastColumn->getNext()));

	pPage->insertColumnLeader(pLeaderColumn, pAfterColumn);
	
	fp_Column* pTmpCol = pLeaderColumn;
 	while (pTmpCol)
	{
		UT_ASSERT(pTmpCol->getPage());
		
		pTmpCol = pTmpCol->getFollower();
	}

	return pLeaderColumn;
}

void fl_DocSectionLayout::format(void)
{
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		pBL->format();
		UT_sint32 count = 0;
		while(pBL->getLastLine() == NULL || pBL->getFirstLine()==NULL)
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

	breakSection();

	if (m_pHeaderSL)
	{
		m_pHeaderSL->format();
	}
	
	if (m_pFooterSL)
	{
		m_pFooterSL->format();
	}
}

void fl_DocSectionLayout::updateLayout(void)
{
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		if (pBL->needsReformat())
		{
			pBL->format();
		}
		
		pBL = pBL->getNext();
	}
	
	breakSection();

	m_pLayout->deleteEmptyColumnsAndPages();
	
	if (m_pHeaderSL)
	{
		m_pHeaderSL->recalculateFields();
		m_pHeaderSL->updateLayout();
	}
	
	if (m_pFooterSL)
	{
		m_pFooterSL->recalculateFields();
		m_pFooterSL->updateLayout();
	}
}

void fl_DocSectionLayout::redrawUpdate(void)
{
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		if (pBL->needsRedraw())
		{
			pBL->redrawUpdate();
		}
		
		pBL = pBL->getNext();
	}
	
	breakSection();

	m_pLayout->deleteEmptyColumnsAndPages();
	
	if (m_pHeaderSL)
	{
		m_pHeaderSL->recalculateFields();
		m_pHeaderSL->updateLayout();
	}
	
	if (m_pFooterSL)
	{
		m_pFooterSL->recalculateFields();
		m_pFooterSL->updateLayout();
	}
}

bool fl_DocSectionLayout::doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc)
{
	UT_ASSERT(pcrxc->getType()==PX_ChangeRecord::PXT_ChangeStrux);

	PT_AttrPropIndex indexAP = pcrxc->getIndexAP();
	const PP_AttrProp* pAP = NULL;
			
	bool bres = (m_pDoc->getAttrProp(indexAP, &pAP) && pAP);
	UT_ASSERT(bres);
	
	const XML_Char* pszSectionType = NULL;
	pAP->getAttribute((XML_Char*)"type", pszSectionType);

	setAttrPropIndex(pcrxc->getIndexAP());

	_lookupProperties();
	//
	// Clear the header/footers first
	//
  	if(m_pHeaderSL)
  	        m_pHeaderSL->collapse();
  	if(m_pFooterSL)
  	        m_pFooterSL->collapse();

	m_pFirstColumn = NULL;
	m_pLastColumn = NULL;

	// clear all the columns
	fp_Column* pCol = m_pFirstColumn;
	while (pCol)
	{
		pCol->clearScreen();

		pCol = pCol->getNext();
	}
	//
	// Clear the header/footers too
	//
	if(m_pHeaderSL)
	        m_pHeaderSL->clearScreen();
	if(m_pFooterSL)
	        m_pFooterSL->clearScreen();

	// remove all the columns from their pages
	pCol = m_pFirstColumn;
	while (pCol)
	{
		if (pCol->getLeader() == pCol)
		{
			pCol->getPage()->removeColumnLeader(pCol);
		}

		pCol = pCol->getNext();
	}

	// get rid of all the layout information for every block
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		pBL->collapse();

		pBL = pBL->getNext();
	}

	// delete all our columns
	pCol = m_pFirstColumn;
	while (pCol)
	{
		fp_Column* pNext = pCol->getNext();

		delete pCol;

		pCol = pNext;
	}

	/*
	  TODO to more closely mirror the architecture we're using for BlockLayout, this code
	  should probably just set a flag, indicating the need to reformat this section.  Then,
	  when it's time to update everything, we'll actually do the format.
	*/

	format();

	FV_View* pView = m_pLayout->getView();
	if (pView)
	{
		pView->notifyListeners(AV_CHG_TYPING | AV_CHG_FMTSECTION);
	}

	return false;
}

void fl_DocSectionLayout::_lookupProperties(void)
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
	
	const char* pszNumColumns = NULL;
	pSectionAP->getProperty((XML_Char*)"columns", (const XML_Char *&)pszNumColumns);
	if (pszNumColumns && pszNumColumns[0])
	{
		m_iNumColumns = atoi(pszNumColumns);
	}
	else
	{
		m_iNumColumns = 1;
	}

	const char* pszColumnGap = NULL;
	pSectionAP->getProperty((XML_Char*)"column-gap", (const XML_Char *&)pszColumnGap);
	if (pszColumnGap && pszColumnGap[0])
	{
		m_iColumnGap = m_pLayout->getGraphics()->convertDimension(pszColumnGap);
		m_iColumnGapLayoutUnits = UT_convertToLayoutUnits(pszColumnGap);
	}
	else
	{
		m_iColumnGap = m_pLayout->getGraphics()->convertDimension("0.25in");
		m_iColumnGapLayoutUnits = UT_convertToLayoutUnits("0.25in");
	}

	const char* pszColumnLineBetween = NULL;
	pSectionAP->getProperty((XML_Char*)"column-line", (const XML_Char *&)pszColumnLineBetween);
	if (pszColumnLineBetween && pszColumnLineBetween[0])
	{
		m_bColumnLineBetween = (strcmp(pszColumnLineBetween, "on") == 0) ? true : false;
	}
	else
	{
		m_bColumnLineBetween = false;
	}

	const char* pszSpaceAfter = NULL;
	pSectionAP->getProperty((XML_Char*)"section-space-after", (const XML_Char *&)pszSpaceAfter);
	if (pszSpaceAfter && pszSpaceAfter[0])
	{
		m_iSpaceAfter = m_pLayout->getGraphics()->convertDimension(pszSpaceAfter);
		m_iSpaceAfterLayoutUnits = UT_convertToLayoutUnits(pszSpaceAfter);
	}
	else
	{
		m_iSpaceAfter = m_pLayout->getGraphics()->convertDimension("0in");
		m_iSpaceAfterLayoutUnits = UT_convertToLayoutUnits("0in");
	}

	const char* pszLeftMargin = NULL;
	const char* pszTopMargin = NULL;
	const char* pszRightMargin = NULL;
	const char* pszBottomMargin = NULL;
	const char* pszFooterMargin = NULL;
	const char* pszHeaderMargin = NULL;
	pSectionAP->getProperty((XML_Char*)"page-margin-left", (const XML_Char *&)pszLeftMargin);
	pSectionAP->getProperty((XML_Char*)"page-margin-top", (const XML_Char *&)pszTopMargin);
	pSectionAP->getProperty((XML_Char*)"page-margin-right", (const XML_Char *&)pszRightMargin);
	pSectionAP->getProperty((XML_Char*)"page-margin-bottom", (const XML_Char *&)pszBottomMargin);
	pSectionAP->getProperty((XML_Char*)"page-margin-footer", (const XML_Char *&)pszFooterMargin);
	pSectionAP->getProperty((XML_Char*)"page-margin-header", (const XML_Char *&)pszHeaderMargin);


	const XML_Char * szRulerUnits;
	UT_Dimension dim;
	if (XAP_App::getApp()->getPrefsValue(AP_PREF_KEY_RulerUnits,&szRulerUnits))
		dim = UT_determineDimension(szRulerUnits);
	else
		dim = DIM_IN;

	char defaultMargin[30];
	switch(dim)
	{
	case DIM_IN:
		strcpy(defaultMargin, "1.0in");
		break;

	case DIM_CM:
		strcpy(defaultMargin, "2.54cm");
		break;

	case DIM_PI:
		strcpy(defaultMargin, "6.0pi");
		break;

	case DIM_PT:
		strcpy(defaultMargin, "72.0pt");
		break;

	case DIM_MM:
		strcpy(defaultMargin, "25.4mm");
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
		strcpy(defaultMargin, "1.0in");	// TODO: what to do with this.
		break;

	}

	if(pszLeftMargin && pszLeftMargin[0])
	{
		m_iLeftMargin = m_pLayout->getGraphics()->convertDimension(pszLeftMargin);
		m_iLeftMarginLayoutUnits = UT_convertToLayoutUnits(pszLeftMargin);
		m_dLeftMarginUserUnits = UT_convertDimensionless(pszLeftMargin);
	}
	else
	{
		m_iLeftMargin = m_pLayout->getGraphics()->convertDimension(defaultMargin);
		m_iLeftMarginLayoutUnits = UT_convertToLayoutUnits(defaultMargin);
		m_dLeftMarginUserUnits = UT_convertDimensionless(defaultMargin);
	}
	
	if(pszTopMargin && pszTopMargin[0])
	{
		m_iTopMargin = m_pLayout->getGraphics()->convertDimension(pszTopMargin);
		m_iTopMarginLayoutUnits = UT_convertToLayoutUnits(pszTopMargin);
		m_dTopMarginUserUnits = UT_convertDimensionless(pszTopMargin);
	}
	else
	{
		m_iTopMargin = m_pLayout->getGraphics()->convertDimension(defaultMargin);
		m_iTopMarginLayoutUnits = UT_convertToLayoutUnits(defaultMargin);
		m_dTopMarginUserUnits = UT_convertDimensionless(defaultMargin);
	}

	if(pszRightMargin && pszRightMargin[0])
	{
		m_iRightMargin = m_pLayout->getGraphics()->convertDimension(pszRightMargin);
		m_iRightMarginLayoutUnits = UT_convertToLayoutUnits(pszRightMargin);
		m_dRightMarginUserUnits = UT_convertDimensionless(pszRightMargin);
	}
	else
	{
		m_iRightMargin = m_pLayout->getGraphics()->convertDimension(defaultMargin);
		m_iRightMarginLayoutUnits = UT_convertToLayoutUnits(defaultMargin);
		m_dRightMarginUserUnits = UT_convertDimensionless(defaultMargin);
	}

	if(pszBottomMargin && pszBottomMargin[0])
	{
		m_iBottomMargin = m_pLayout->getGraphics()->convertDimension(pszBottomMargin);
		m_iBottomMarginLayoutUnits = UT_convertToLayoutUnits(pszBottomMargin);
		m_dBottomMarginUserUnits = UT_convertDimensionless(pszBottomMargin);
	}
	else
	{
		m_iBottomMargin = m_pLayout->getGraphics()->convertDimension(defaultMargin);
		m_iBottomMarginLayoutUnits = UT_convertToLayoutUnits(defaultMargin);
		m_dBottomMarginUserUnits = UT_convertDimensionless(defaultMargin);
	}

	if(pszFooterMargin && pszFooterMargin[0])
	{
		m_iFooterMargin = m_pLayout->getGraphics()->convertDimension(pszFooterMargin);
		m_iFooterMarginLayoutUnits = UT_convertToLayoutUnits(pszFooterMargin);
		m_dFooterMarginUserUnits = UT_convertDimensionless(pszFooterMargin);
	}
	else
	{
		m_iFooterMargin = m_pLayout->getGraphics()->convertDimension("0.0in");
		m_iFooterMarginLayoutUnits = UT_convertToLayoutUnits("0.0in");
		m_dFooterMarginUserUnits = UT_convertDimensionless("0.0in");
	}

	if(pszHeaderMargin && pszHeaderMargin[0])
	{
		m_iHeaderMargin = m_pLayout->getGraphics()->convertDimension(pszHeaderMargin);
		m_iHeaderMarginLayoutUnits = UT_convertToLayoutUnits(pszHeaderMargin);
		m_dHeaderMarginUserUnits = UT_convertDimensionless(pszHeaderMargin);
	}
	else
	{
		m_iHeaderMargin = m_pLayout->getGraphics()->convertDimension("0.0in");
		m_iHeaderMarginLayoutUnits = UT_convertToLayoutUnits("0.0in");
		m_dHeaderMarginUserUnits = UT_convertDimensionless("0.0in");
	}
	


	m_bForceNewPage = false;
}

void fl_DocSectionLayout::deleteEmptyColumns(void)
{
	fp_Column* pCol = m_pFirstColumn;
	while (pCol)
	{
		if (pCol->getLeader() == pCol)
		{
			fp_Column* pCol2 = pCol;
			bool bAllEmpty = true;
			fp_Column* pLastInGroup = NULL;
			
			while (pCol2)
			{
				if (!pCol2->isEmpty())
				{
					bAllEmpty = false;
				}

				if (!(pCol2->getFollower()))
				{
					pLastInGroup = pCol2;
				}
				
				pCol2 = pCol2->getFollower();
			}

			if (bAllEmpty)
			{
				UT_ASSERT(pLastInGroup);

				pCol->getPage()->removeColumnLeader(pCol);

				if (pCol == m_pFirstColumn)
				{
					m_pFirstColumn = pLastInGroup->getNext();
				}

				if (pLastInGroup == m_pLastColumn)
				{
					m_pLastColumn = pCol->getPrev();
				}
				
				if (pCol->getPrev())
				{
					pCol->getPrev()->setNext(pLastInGroup->getNext());
				}

				if (pLastInGroup->getNext())
				{
					pLastInGroup->getNext()->setPrev(pCol->getPrev());
				}

				fp_Column* pCol3 = pCol;
				pCol = pLastInGroup->getNext();
				while (pCol3)
				{
					fp_Column* pNext = pCol3->getFollower();

					delete pCol3;

					pCol3 = pNext;
				}
			}
			else
			{
				pCol = pLastInGroup->getNext();
			}
		}
		else
		{
			pCol = pCol->getNext();
		}
	}
}

UT_uint32 fl_DocSectionLayout::getNumColumns(void) const
{
	return m_iNumColumns;
}

UT_uint32 fl_DocSectionLayout::getColumnGap(void) const
{
	return m_iColumnGap;
}

UT_uint32 fl_DocSectionLayout::getColumnGapInLayoutUnits(void) const
{
	return m_iColumnGapLayoutUnits;
}

fl_DocSectionLayout* fl_DocSectionLayout::getNextDocSection(void) const
{
	UT_ASSERT(getType() == FL_SECTION_DOC);

	return (fl_DocSectionLayout*) getNext();
}

fl_DocSectionLayout* fl_DocSectionLayout::getPrevDocSection(void) const
{
	UT_ASSERT(getType() == FL_SECTION_DOC);

	return (fl_DocSectionLayout*) getPrev();
}

bool fl_DocSectionLayout::doclistener_deleteStrux(const PX_ChangeRecord_Strux * pcrx)
{
	UT_ASSERT(pcrx->getType()==PX_ChangeRecord::PXT_DeleteStrux);
	UT_ASSERT(pcrx->getStruxType()==PTX_Section);

	fl_DocSectionLayout* pPrevSL = getPrevDocSection();
	if (!pPrevSL)
	{
		// TODO shouldn't this just assert?
		UT_DEBUGMSG(("no prior SectionLayout"));
		return false;
	}
	
	// clear all the columns
	fp_Column* pCol = m_pFirstColumn;
	while (pCol)
	{
		pCol->clearScreen();

		pCol = pCol->getNext();
	}

	// remove all the columns from their pages
	pCol = m_pFirstColumn;
	while (pCol)
	{
		if (pCol->getLeader() == pCol)
		{
			pCol->getPage()->removeColumnLeader(pCol);
		}

		pCol = pCol->getNext();
	}

	// get rid of all the layout information for every block
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		pBL->collapse();

		pBL = pBL->getNext();
	}

	// delete all our columns
	pCol = m_pFirstColumn;
	while (pCol)
	{
		fp_Column* pNext = pCol->getNext();

		delete pCol;

		pCol = pNext;
	}

	m_pFirstColumn = NULL;
	m_pLastColumn = NULL;

	while (m_pFirstBlock)
	{
		pBL = m_pFirstBlock;
		removeBlock(pBL);
		pPrevSL->addBlock(pBL);
	}
	
	pPrevSL->m_pNext = m_pNext;
							
	if (m_pNext)
	{
		m_pNext->setPrev(pPrevSL);
	}

	m_pLayout->removeSection(this);

	pPrevSL->format();
	
//	FV_View* pView = m_pLayout->getView();
//      if (pView)
//  	{
//  		pView->_setPoint(pcrx->getPosition());
//  	}

	delete this;			// TODO whoa!  this construct is VERY dangerous.
	
	return true;
}

void fl_DocSectionLayout::addOwnedPage(fp_Page* pPage)
{
	// TODO do we really need the vecOwnedPages member? YES!!!

        if(m_pFirstOwnedPage == NULL)
	        m_pFirstOwnedPage = pPage;
	fp_Page * pPrev = m_pFirstOwnedPage;

	if (m_pHeaderSL)
	{
	        if(pPrev && pPrev->getOwningSection() == this && pPrev->getHeaderP() == NULL )
	                prependOwnedHeaderPage(pPrev);

		m_pHeaderSL->addPage(pPage);
	}

	if (m_pFooterSL)
	{
	        if(pPrev && pPrev->getOwningSection() == this && pPrev->getFooterP() == NULL)
	                prependOwnedFooterPage(pPrev);
		m_pFooterSL->addPage(pPage);
	}
}

void fl_DocSectionLayout::prependOwnedHeaderPage(fp_Page* pPage)
{
  //
  // Skip back through the pages until the first owned page of this section
  //
        fp_Page * pPrev = pPage->getPrev();
	if(pPrev && pPrev->getOwningSection() == this && pPrev->getHeaderP() == NULL)
	        prependOwnedHeaderPage(pPrev);
       	m_pHeaderSL->addPage(pPage);
}


void fl_DocSectionLayout::prependOwnedFooterPage(fp_Page* pPage)
{
  //
  // Skip back through the pages until the first owned page of this section
  //
        fp_Page * pPrev = pPage->getPrev();
	if(pPrev && pPrev->getOwningSection() == this && pPrev->getFooterP() == NULL)
	        prependOwnedFooterPage(pPrev);
	m_pFooterSL->addPage(pPage);
}

	
void fl_DocSectionLayout::deleteOwnedPage(fp_Page* pPage)
{
	// TODO do we really need the vecOwnedPages member?
	
	if (m_pHeaderSL)
	{
		m_pHeaderSL->deletePage(pPage);
	}

	if (m_pFooterSL)
	{
		m_pFooterSL->deletePage(pPage);
	}
}

UT_sint32 fl_DocSectionLayout::breakSection(void)
{
	fl_BlockLayout* pFirstBlock = getFirstBlock();
	if (!pFirstBlock)
	{
		return 0;
	}

	fp_Line* pCurrentLine = pFirstBlock->getFirstLine();
		
	fp_Column* pCurColumn = (fp_Column*) getFirstContainer();

	while (pCurColumn)
	{
		fp_Line* pFirstLineToKeep = pCurrentLine;
		fp_Line* pLastLineToKeep = NULL;
		fp_Line* pOffendingLine = NULL;
		
		UT_sint32 iMaxColHeight = pCurColumn->getMaxHeightInLayoutUnits();
		UT_sint32 iWorkingColHeight = 0;

		fp_Line* pCurLine = pFirstLineToKeep;
		while (pCurLine)
		{
			UT_sint32 iLineHeight = pCurLine->getHeightInLayoutUnits();
			UT_sint32 iLineMarginAfter = pCurLine->getMarginAfterInLayoutUnits();
			UT_sint32 iTotalLineSpace = iLineHeight + iLineMarginAfter;

			if ((iWorkingColHeight + iTotalLineSpace) > iMaxColHeight)
			{
				pOffendingLine = pCurLine;

				/*
				  We have found the offending line (the first one which won't fit in the
				  column) and we now need to decide whether we can break the column
				  just before it.
				*/

				if (pOffendingLine == pFirstLineToKeep)
				{
					/*
					  Wow!  The very first line in this column won't fit.
					  
					  Big line.  (or maybe a small column)
					  
					  TODO what should we do here?  For now, we force it.
					*/
					pLastLineToKeep = pFirstLineToKeep;
				}
				else
				{
					fl_BlockLayout* pBlock = pOffendingLine->getBlock();
					UT_uint32 iWidows = pBlock->getProp_Widows();
					UT_uint32 iOrphans = pBlock->getProp_Orphans();

					UT_uint32 iNumLinesBeforeOffending = 0;
					UT_uint32 iNumLinesAfterOffending = 0;
					bool bFoundOffending = false;
					
					fp_Line* pFirstLineInBlock = pBlock->getFirstLine();
					pCurLine = pFirstLineInBlock;
					while (pCurLine)
					{
						if (bFoundOffending)
						{
							iNumLinesAfterOffending++;
						}
						else
						{
							if (pCurLine == pOffendingLine)
							{
								iNumLinesAfterOffending = 1;
								bFoundOffending = true;
							}
							else
							{
								iNumLinesBeforeOffending++;
							}
						}
						
						pCurLine = pCurLine->getNext();
					}

					UT_uint32 iNumLinesInBlock = iNumLinesBeforeOffending + iNumLinesAfterOffending;

					UT_uint32 iNumBlockLinesInThisColumn = 0;
					pCurLine = pOffendingLine->getPrev();
					while (pCurLine)
					{
						iNumBlockLinesInThisColumn++;
						if (pCurLine == pFirstLineToKeep)
						{
							break;
						}

						pCurLine = pCurLine->getPrev();
					}

					if (
						pBlock->getProp_KeepTogether()
						&& (iNumLinesBeforeOffending == iNumBlockLinesInThisColumn)
						&& (pBlock->getFirstLine() != pFirstLineToKeep)
						)
					{
						/*
						  This block wants to be kept all in the same column.
						  Bump the whole block to the next column.
						*/

						/*
						  If the block is simply too big to fit in a
						  single column, then we can spawn an infinite
						  loop by continually punting it to the next
						  one.  So, we assume that if the first line
						  in the block is the first line in this
						  column, we just keep it and cope.  This will
						  be slightly incorrect in cases where pushing
						  it to the next column would allow the block
						  to try to live in a larger column, thus
						  staying all together.
						*/
						
						pLastLineToKeep = pFirstLineInBlock->getPrevLineInSection();
					}
					else if (
						(iNumLinesInBlock < (iWidows + iOrphans))
						&& (iNumLinesBeforeOffending == iNumBlockLinesInThisColumn)
						)
					{
						/*
						  There are not enough lines to divide between the
						  two columns while still satisfying both constraints.
						  Bump the whole block to the next column.
						*/
						
						pLastLineToKeep = pFirstLineInBlock->getPrevLineInSection();
					}
					else if (
						(iNumLinesBeforeOffending < iOrphans)
						&& (iNumLinesBeforeOffending == iNumBlockLinesInThisColumn)
						)
					{
						/*
						  We're leaving too few lines in the current column.
						  Bump the whole block.
						*/

						pLastLineToKeep = pFirstLineInBlock->getPrevLineInSection();
					}
					else if (
						(iNumLinesAfterOffending < iWidows)
						&& ((iWidows - iNumLinesAfterOffending) < iNumBlockLinesInThisColumn)
						)
					{
						/*
						  There aren't going to be enough lines in the next
						  column.  Bump just enough.
						*/

						UT_uint32 iNumLinesNeeded = (iWidows - iNumLinesAfterOffending);
						pLastLineToKeep = pOffendingLine->getPrevLineInSection();
						for (UT_uint32 iBump = 0; iBump < iNumLinesNeeded; iBump++)
						{
							pLastLineToKeep = pLastLineToKeep->getPrevLineInSection();
						}
					}
					else
					{
						pLastLineToKeep = pOffendingLine->getPrevLineInSection();
					}
				}
				break;
			}
			else
			{
				iWorkingColHeight += iTotalLineSpace;
				if (
					pCurLine->containsForcedColumnBreak()
					|| pCurLine->containsForcedPageBreak()
					)
				{
					pLastLineToKeep = pCurLine;
					break;
				}
			}

			pCurLine = pCurLine->getNextLineInSection();
		}

		if (pLastLineToKeep)
		{
			pCurrentLine = pLastLineToKeep->getNextLineInSection();
		}
		else
		{
			pCurrentLine = NULL;
		}
		
		pCurLine = pFirstLineToKeep;
		while (pCurLine)
		{
			if (pCurLine->getContainer() != pCurColumn)
			{
				pCurLine->getContainer()->removeLine(pCurLine);
				pCurColumn->addLine(pCurLine);
			}

			if (pCurLine == pLastLineToKeep)
			{
				break;
			}
			else
			{
				pCurLine = pCurLine->getNextLineInSection();
			}
		}

		fp_Column* pNextColumn = NULL;
		
		if (pLastLineToKeep)
		{
			UT_ASSERT(pLastLineToKeep->getContainer() == pCurColumn);
			
			if (pCurColumn->getLastLine() != pLastLineToKeep)
			{
				// make sure there is a next column
				pNextColumn = pCurColumn->getNext();
				if (!pNextColumn)
				{
					pNextColumn = (fp_Column*) getNewContainer();
				}

				pCurColumn->bumpLines(pLastLineToKeep);
			}
		}

		UT_ASSERT((!pLastLineToKeep) || (pCurColumn->getLastLine() == pLastLineToKeep));
			
		pCurColumn->layout();

		pCurColumn = pCurColumn->getNext();
	}

	return 0; // TODO return code
}

void fl_DocSectionLayout::checkAndAdjustColumnGap(UT_sint32 iLayoutWidth)
{
	// Check to make sure column gap is not to wide to fit on the page with the
	// given number of columns.

	if(m_iNumColumns > 1)
	{
		UT_sint32 minColumnWidth = m_pLayout->getGraphics()->convertDimension("0.5in");	//TODO should this dimension be hard coded.
		UT_sint32 iColWidth = (iLayoutWidth - (UT_sint32)(((m_iNumColumns - 1) * m_iColumnGap))) / (UT_sint32)m_iNumColumns;

		if(iColWidth < minColumnWidth)
		{
			m_iColumnGap = (iLayoutWidth - minColumnWidth * m_iNumColumns) / (m_iNumColumns - 1);
		}
			
	}

}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

struct _PageHdrFtrShadowPair
{
	fp_Page*			pPage;
	fl_HdrFtrShadow*	pShadow;
};

fl_HdrFtrSectionLayout::fl_HdrFtrSectionLayout(UT_uint32 iHFType, FL_DocLayout* pLayout, fl_DocSectionLayout* pDocSL, PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP)
	: fl_SectionLayout(pLayout, sdh, indexAP, FL_SECTION_HDRFTR)
{
	m_pDocSL = pDocSL;
	m_iHFType = iHFType;
}

fl_HdrFtrSectionLayout::~fl_HdrFtrSectionLayout()
{
	
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		delete pPair->pShadow;
	}
	_purgeLayout();
	
	UT_VECTOR_PURGEALL(struct _PageHdrFtrShadowPair*, m_vecPages);
}


void fl_HdrFtrSectionLayout::collapse(void)
{
	UT_uint32 iCount = m_vecPages.getItemCount();
	UT_uint32 i;
	for (i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		fp_Page * ppPage = pPair->pPage;
		delete pPair->pShadow;
	        if (getHFType() == FL_HDRFTR_HEADER)
		{
		       ppPage->removeHeader();
		}
		else 
		{
		       ppPage->removeFooter();
		}
		UT_DEBUGMSG(("SEVIOR: Removing page %x \n",ppPage));
		delete pPair;
	}
	m_vecPages.clear();
	UT_DEBUGMSG(("SEVIOR: Number of shadows left %d \n",m_vecPages.getItemCount()));
}

bool fl_HdrFtrSectionLayout::recalculateFields(void)
{
	bool bResult = false;
	
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		UT_ASSERT(pPair->pShadow);
		bResult = pPair->pShadow->recalculateFields() || bResult;
	}

	return bResult;
}


fl_HdrFtrShadow * fl_HdrFtrSectionLayout::getFirstShadow(void)
{
	UT_uint32 iCount = m_vecPages.getItemCount();
	if(iCount != 0)
	{
	        struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(0);
	        return pPair->pShadow;
	}
	return NULL;
}

fp_Container* fl_HdrFtrSectionLayout::getFirstContainer()
{
	UT_uint32 iCount = m_vecPages.getItemCount();
	if(iCount != 0)
	{
	        struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(0);
	        return pPair->pShadow->getFirstContainer();
	}
	return NULL;
}


fp_Container* fl_HdrFtrSectionLayout::getLastContainer()
{
	UT_uint32 iCount = m_vecPages.getItemCount();
	UT_ASSERT(iCount);
	struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(iCount-1);
	return pPair->pShadow->getFirstContainer();
}

fp_Container* fl_HdrFtrSectionLayout::getNewContainer(void)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return NULL;
}

UT_sint32 fl_HdrFtrSectionLayout::_findShadow(fp_Page* pPage)
{
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		if (pPair->pPage == pPage)
		{
			return i;
		}
	}

	return -1;
}
//
// This method converts a previously existing section to this header/footer.
// Code liberally stolen from fl_DocSectionLayout::doclistener_deleteStrux
//
void fl_HdrFtrSectionLayout::changeStrux( fl_DocSectionLayout * pSL)
{
        fl_DocSectionLayout* pPrevSL = pSL->getPrevDocSection();
	UT_ASSERT(pPrevSL);
	// clear all the columns
	fp_Column* pCol =NULL;

	pCol = (fp_Column *) pSL->getFirstContainer();
	while (pCol)
	{
		pCol->clearScreen();

		pCol = pCol->getNext();
	}

	// remove all the columns from their pages
	pCol = (fp_Column *) pSL->getFirstContainer();
	while (pCol)
	{
		if (pCol->getLeader() == pCol)
		{
			pCol->getPage()->removeColumnLeader(pCol);
		}

		pCol = pCol->getNext();
	}


	// get rid of all the layout information for every block
	fl_BlockLayout*	pBL = pSL->getFirstBlock();
	while (pBL)
	{
		pBL->collapse();

		pBL = pBL->getNext();
	}

	// transfer the Sections' blocks into this header/footer

	while (pSL->getFirstBlock())
	{
		pBL = pSL->getFirstBlock();
		pSL->removeBlock(pBL);
		addBlock(pBL);
	}
	
	pPrevSL->setNext(pSL->getNext());
							
	if (pSL->getNext())
	{
		pSL->getNext()->setPrev(pPrevSL);
	}

	m_pLayout->removeSection(pSL);
	DELETEP(pSL); // Old Section layout is totally gone
	//
	// Create and Format the shadows
	//
	format();

	// OK set the insertion point at the beginning of the last block
	pBL = getLastBlock();

	UT_ASSERT(pBL);

//	FV_View* pView = m_pLayout->getView();
//  	if (pView)
//  	{
//  	        UT_uint32 pos = (UT_uint32) pBL->getPosition(true);
//  		pView->setPoint(pos);
//  	}

	// Finished! we now have a header/footer
}

void fl_HdrFtrSectionLayout::addPage(fp_Page* pPage)
{
	UT_ASSERT(0 > _findShadow(pPage));
	if(_findShadow(pPage) > 0)
	       return;
	struct _PageHdrFtrShadowPair* pPair = new _PageHdrFtrShadowPair;
	// TODO outofmem

	pPair->pPage = pPage;
	pPair->pShadow = new fl_HdrFtrShadow(m_pLayout, pPage, this, m_sdh, m_apIndex);
	//
	// Make sure we register the shadow before populating it.
	//
	m_vecPages.addItem(pPair);
	//
	// Populate the shadow
	//
	fl_ShadowListener* pShadowListener = new fl_ShadowListener(this, pPair->pShadow);
	m_pDoc->tellListener(pShadowListener);

	delete pShadowListener;

	UT_DEBUGMSG(("SEVIOR: Number of shadows = %d \n",m_vecPages.getItemCount()));

	//
	// Now generate the container for this page
	//
	// fp_Container * pContainer= pPair->pShadow->getFirstContainer();
	// unused
}

void fl_HdrFtrSectionLayout::deletePage(fp_Page* pPage)
{
	UT_sint32 iShadow = _findShadow(pPage);
	UT_ASSERT(iShadow >= 0);
	if(iShadow <= 0)
	        return;
	struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(iShadow);
	UT_ASSERT(pPair);

	UT_ASSERT(pPair->pShadow);


	fp_Page * ppPage = pPair->pPage; 
	delete pPair->pShadow;
	if (getHFType() == FL_HDRFTR_HEADER)
	{
	        ppPage->removeHeader();
	}
	else 
	{
        	ppPage->removeFooter();
	}

	delete pPair;
	m_vecPages.deleteNthItem(iShadow);

	UT_DEBUGMSG(("SEVIOR: Deleting page %x Number of shadows = %d \n",ppPage,m_vecPages.getItemCount()));
}

void fl_HdrFtrSectionLayout::format(void)
{
  //
  // Just format the shadow block since the HdrFtrSectionLayout blocks 
  // have no containers for lines.
  // 
	UT_uint32 iCount = m_vecPages.getItemCount();
	//
	// See if this hdrFtr section has any pages, if not add them
	//
	if(iCount == 0)
	{
	  // OK now add all the pages this header/footer is associated with
	  // We have to extract this information from m_pDocSL
	  // Loop through all the columns in m_pDocSl and find the pages owned
	  // by m_pDocSL
	  //

	       fp_Column * pCol = (fp_Column *) m_pDocSL->getFirstContainer();
	       fp_Page * pOldPage = NULL;
	       fp_Page * pNewPage = NULL;
	       while(pCol)
	       {
	              pNewPage = pCol->getPage();
		      if(pNewPage != NULL && pNewPage != pOldPage)
		      {
		              fl_DocSectionLayout* pDocSec = pNewPage->getOwningSection();
			      if(pDocSec == m_pDocSL && _findShadow(pNewPage) < 0)
			      {
			               addPage(pNewPage);
			      }
		      }
		      pCol = pCol->getNext();
	       }
	       iCount = m_vecPages.getItemCount();
	}
	
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		pPair->pShadow->format();
	}
}

void fl_HdrFtrSectionLayout::updateLayout(void)
{
  //
  // update Just the  blocks in the shadowlayouts
  //
  	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		pPair->pShadow->updateLayout();
	}
}


void fl_HdrFtrSectionLayout::clearScreen(void)
{
  //
  // update Just the  blocks in the shadowlayouts
  //
  	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		pPair->pShadow->clearScreen();
	}
}

void fl_HdrFtrSectionLayout::redrawUpdate(void)
{
  //
  // Don't need to draw here since this is never displayed on the screen?
  // 
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		pPair->pShadow->redrawUpdate();
	}

}

bool fl_HdrFtrSectionLayout::doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc)
{
	UT_ASSERT(pcrxc->getType()==PX_ChangeRecord::PXT_ChangeStrux);

	setAttrPropIndex(pcrxc->getIndexAP());

	// TODO what happens here?

	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return false;
}

void fl_HdrFtrSectionLayout::_lookupProperties(void)
{
}

bool fl_HdrFtrSectionLayout::bl_doclistener_populateSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs, PT_BlockOffset blockOffset, UT_uint32 len)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_populateSpan(pcrs,blockOffset,len)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_populateSpan(pcrs,blockOffset,len)
			&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_populateObject(fl_BlockLayout* pBL, PT_BlockOffset blockOffset, const PX_ChangeRecord_Object * pcro)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_populateObject(blockOffset,pcro)
			&& bResult;
	}
        // Update the overall block too.
	bResult = pBL->doclistener_populateObject(blockOffset,pcro)
			&& bResult;
	return bResult;
}
	
bool fl_HdrFtrSectionLayout::bl_doclistener_insertSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_insertSpan(pcrs)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_insertSpan(pcrs)
				&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_deleteSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_Span * pcrs)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_deleteSpan(pcrs)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_deleteSpan(pcrs)
			&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_changeSpan(fl_BlockLayout* pBL, const PX_ChangeRecord_SpanChange * pcrsc)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_changeSpan(pcrsc)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_changeSpan(pcrsc)
	        && bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_deleteStrux(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_deleteStrux(pcrx)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_deleteStrux(pcrx)
			&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_changeStrux(fl_BlockLayout* pBL, const PX_ChangeRecord_StruxChange * pcrxc)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_changeStrux(pcrxc)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_changeStrux(pcrxc)
			&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_insertBlock(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx,
									PL_StruxDocHandle sdh,
									PL_ListenerId lid,
									void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
															PL_ListenerId lid,
															PL_StruxFmtHandle sfhNew))
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_insertBlock(pcrx,sdh,lid,pfnBindHandles)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_insertBlock(pcrx,sdh,lid,pfnBindHandles)
			&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_insertSection(fl_BlockLayout* pBL, const PX_ChangeRecord_Strux * pcrx,
									  PL_StruxDocHandle sdh,
									  PL_ListenerId lid,
									  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
															  PL_ListenerId lid,
															  PL_StruxFmtHandle sfhNew))
{
	// TODO this should NEVER happen, right?
	
	bool bResult = true;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);

		bResult = pPair->pShadow->bl_doclistener_insertSection(pBL, pcrx, sdh, lid, pfnBindHandles)
			&& bResult;
	}

	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_insertObject(fl_BlockLayout* pBL, const PX_ChangeRecord_Object * pcro)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_insertObject(pcro)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_insertObject(pcro)
			&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_deleteObject(fl_BlockLayout* pBL, const PX_ChangeRecord_Object * pcro)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_deleteObject(pcro)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_deleteObject(pcro)
			&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_changeObject(fl_BlockLayout* pBL, const PX_ChangeRecord_ObjectChange * pcroc)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_changeObject(pcroc)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_changeObject(pcroc)
			&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_insertFmtMark(fl_BlockLayout* pBL, const PX_ChangeRecord_FmtMark * pcrfm)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_insertFmtMark(pcrfm)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_insertFmtMark(pcrfm)
			&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_deleteFmtMark(fl_BlockLayout* pBL, const PX_ChangeRecord_FmtMark * pcrfm)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_deleteFmtMark(pcrfm)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_deleteFmtMark(pcrfm)
			&& bResult;
	return bResult;
}

bool fl_HdrFtrSectionLayout::bl_doclistener_changeFmtMark(fl_BlockLayout* pBL, const PX_ChangeRecord_FmtMarkChange * pcrfmc)
{
	bool bResult = true;
	fl_BlockLayout * pShadowBL = NULL;
	UT_uint32 iCount = m_vecPages.getItemCount();
	for (UT_uint32 i=0; i<iCount; i++)
	{
		struct _PageHdrFtrShadowPair* pPair = (struct _PageHdrFtrShadowPair*) m_vecPages.getNthItem(i);
		// Find matching block in this shadow.
		pShadowBL = pPair->pShadow->findMatchingBlock(pBL);
		bResult = pShadowBL->doclistener_changeFmtMark(pcrfmc)
			&& bResult;
	}
	// Update the overall block too.
	bResult = pBL->doclistener_changeFmtMark(pcrfmc)
			&& bResult;
	return bResult;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fl_HdrFtrShadow::fl_HdrFtrShadow(FL_DocLayout* pLayout, fp_Page* pPage, fl_HdrFtrSectionLayout* pHdrFtrSL, PL_StruxDocHandle sdh, PT_AttrPropIndex indexAP)
	: fl_SectionLayout(pLayout, sdh, indexAP, FL_SECTION_SHADOW)
{
	m_pHdrFtrSL = pHdrFtrSL;
	m_pPage = pPage;
	if (m_pHdrFtrSL->getHFType() == FL_HDRFTR_HEADER)
	{
		m_pContainer = m_pPage->getHeaderContainer(m_pHdrFtrSL);
	}
	else if (m_pHdrFtrSL->getHFType() == FL_HDRFTR_FOOTER)
	{
	        m_pContainer =  m_pPage->getFooterContainer(m_pHdrFtrSL);
	}
}

fl_HdrFtrShadow::~fl_HdrFtrShadow()
{
	_purgeLayout();
}

fp_Container* fl_HdrFtrShadow::getFirstContainer()
{
	if (m_pHdrFtrSL->getHFType() == FL_HDRFTR_HEADER)
	{
		return m_pPage->getHeaderContainer(m_pHdrFtrSL);
	}
	else if (m_pHdrFtrSL->getHFType() == FL_HDRFTR_FOOTER)
	{
		return m_pPage->getFooterContainer(m_pHdrFtrSL);
	}
	
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return NULL;
}

fp_Container* fl_HdrFtrShadow::getLastContainer()
{
	UT_ASSERT(UT_TODO);

	return NULL;
}

fp_Container* fl_HdrFtrShadow::getNewContainer(void)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return NULL;
}

fl_BlockLayout* fl_HdrFtrShadow::findMatchingBlock(fl_BlockLayout* pBL)
{
  // This routine returns the matching block within this shadow of the
  // hdrftrSectionlayout. 
  //
        fl_BlockLayout* ppBL = m_pFirstBlock;
	while(ppBL && (ppBL->getStruxDocHandle() != pBL->getStruxDocHandle()))
        {
	      ppBL = ppBL->getNext();
	}
	UT_ASSERT(ppBL);
	return ppBL;
}
	  
void fl_HdrFtrShadow::format(void)
{
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		pBL->format();
		pBL = pBL->getNext();
	}
}

void fl_HdrFtrShadow::updateLayout(void)
{
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		if (pBL->needsReformat())
		{
			pBL->format();
		}
		
		pBL = pBL->getNext();
	}
}


void fl_HdrFtrShadow::clearScreen(void)
{
        UT_ASSERT(m_pContainer);
        if(m_pContainer)
                m_pContainer->clearScreen();
}

void fl_HdrFtrShadow::redrawUpdate(void)
{
	fl_BlockLayout*	pBL = m_pFirstBlock;
	while (pBL)
	{
		pBL->redrawUpdate();
		pBL = pBL->getNext();
	}
}
bool fl_HdrFtrShadow::doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc)
{
	UT_ASSERT(pcrxc->getType()==PX_ChangeRecord::PXT_ChangeStrux);

	setAttrPropIndex(pcrxc->getIndexAP());

	// TODO

	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return false;
}


void fl_HdrFtrShadow::_lookupProperties(void)
{
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

fl_ShadowListener::fl_ShadowListener(fl_HdrFtrSectionLayout* pHFSL, fl_HdrFtrShadow* pShadow)
{
	UT_ASSERT(pHFSL);
	UT_ASSERT(pShadow);

	m_pDoc = pHFSL->getDocLayout()->getDocument();
	m_pHFSL = pHFSL;
	m_pShadow = pShadow;
	m_bListening = false;
	m_pCurrentBL = NULL;
}

fl_ShadowListener::~fl_ShadowListener()
{
}

bool fl_ShadowListener::populate(PL_StruxFmtHandle sfh,
									const PX_ChangeRecord * pcr)
{
	if (!m_bListening)
	{
		return true;
	}
	
	UT_ASSERT(m_pShadow);
	UT_DEBUGMSG(("fl_ShadowListener::populate\n"));

	bool bResult = false;
	//	bool curInsPoint = m_pDoc->getAllowChangeInsPoint();
	//	m_pDoc->setDontChangeInsPoint();
	FV_View* pView = m_pHFSL->getDocLayout()->getView();
	PT_DocPosition oldPos = pView->getPoint();
	switch (pcr->getType())
	{
	case PX_ChangeRecord::PXT_InsertSpan:
		{
			const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

			{
				fl_Layout * pL = (fl_Layout *)sfh;
				UT_ASSERT(pL->getType() == PTX_Block);
				UT_ASSERT(m_pCurrentBL == (static_cast<fl_BlockLayout *>(pL)));
			}
			PT_BlockOffset blockOffset = pcrs->getBlockOffset();
			UT_uint32 len = pcrs->getLength();

// sterwill -- is this call to getSectionLayout() needed?  pBLSL is not used.
			
//			fl_SectionLayout* pBLSL = m_pCurrentBL->getSectionLayout();
			bResult = m_pCurrentBL->doclistener_populateSpan(pcrs, blockOffset, len);
			goto finish_up;
		}

	case PX_ChangeRecord::PXT_InsertObject:
		{
			const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *>(pcr);

			{
				fl_Layout * pL = (fl_Layout *)sfh;
				UT_ASSERT(pL->getType() == PTX_Block);
				UT_ASSERT(m_pCurrentBL == (static_cast<fl_BlockLayout *>(pL)));
			}
			PT_BlockOffset blockOffset = pcro->getBlockOffset();

// sterwill -- is this call to getSectionLayout() needed?  pBLSL is not used.

//			fl_SectionLayout* pBLSL = m_pCurrentBL->getSectionLayout();
			bResult = m_pCurrentBL->doclistener_populateObject(blockOffset,pcro);
			goto finish_up;
		}
	
	case PX_ChangeRecord::PXT_InsertFmtMark:
                {
		  //	const PX_ChangeRecord_FmtMark * pcrfm = static_cast<const PX_ChangeRecord_FmtMark *>(pcr);

			{
				fl_Layout * pL = (fl_Layout *)sfh;
				UT_ASSERT(pL->getType() == PTX_Block);
				UT_ASSERT(m_pCurrentBL == (static_cast<fl_BlockLayout *>(pL)));
			}
			bResult = m_pCurrentBL->doclistener_insertFmtMark( (const PX_ChangeRecord_FmtMark *) pcr);
			goto finish_up;
		}

	default:
	        UT_DEBUGMSG(("Unknown Change record = %d \n",pcr->getType())); 
		UT_ASSERT(0);
		pView->setPoint(oldPos);
		//		if(curInsPoint)
		//        m_pDoc->allowChangeInsPoint();
		return false;
	}

finish_up:
	pView->setPoint(oldPos);
	//	if(curInsPoint)
	// 	 m_pDoc->allowChangeInsPoint();
	
	return bResult;
}

bool fl_ShadowListener::populateStrux(PL_StruxDocHandle sdh,
										 const PX_ChangeRecord * pcr,
										 PL_StruxFmtHandle * psfh)
{
	UT_ASSERT(m_pShadow);
	//	UT_DEBUGMSG(("fl_ShadowListener::populateStrux\n"));

	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

	//	bool curInsPoint = m_pDoc->getAllowChangeInsPoint();
	//m_pDoc->setDontChangeInsPoint();
	FV_View* pView = m_pHFSL->getDocLayout()->getView();
	PT_DocPosition oldPos = pView->getPoint();

	switch (pcrx->getStruxType())
	{
	case PTX_Section:
	{
		PT_AttrPropIndex indexAP = pcr->getIndexAP();
		const PP_AttrProp* pAP = NULL;
		if (m_pDoc->getAttrProp(indexAP, &pAP) && pAP)
		{
			const XML_Char* pszSectionType = NULL;
			pAP->getAttribute((XML_Char*)"type", pszSectionType);
			if (
				!pszSectionType
				|| (0 == UT_strcmp(pszSectionType, "doc"))
				)
			{
				m_bListening = false;
			}
			else
			{
				if (
					(0 == UT_strcmp(pszSectionType, "header"))
					|| (0 == UT_strcmp(pszSectionType, "footer"))
					)
				{
					// TODO verify id match
					
					m_bListening = true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			// TODO fail?
			return false;
		}
	}
	break;

	case PTX_Block:
	{
		if (m_bListening)
		{
			// append a new BlockLayout to that SectionLayout
			fl_BlockLayout*	pBL = m_pShadow->appendBlock(sdh, pcr->getIndexAP());
			if (!pBL)
			{
				UT_DEBUGMSG(("no memory for BlockLayout"));
				return false;
			}

			m_pCurrentBL = pBL;	
			*psfh = (PL_StruxFmtHandle)pBL;
		}
		
#if 0		// TODO are we spell-checking headers and footers?
		
		// BUGBUG: this is *not* thread-safe, but should work for now
		if (m_bScreen)
			m_pLayout->queueBlockForBackgroundCheck(bgcrSpelling, pBL);
#endif

	}
	break;
			
	default:
		UT_ASSERT(0);
		//     	        if(curInsPoint)
	 	//        m_pDoc->allowChangeInsPoint();
		pView->setPoint(oldPos);
		return false;
	}
	pView->setPoint(oldPos);
	//	if(curInsPoint)
	// 	 m_pDoc->allowChangeInsPoint();

	return true;
}

bool fl_ShadowListener::change(PL_StruxFmtHandle /*sfh*/,
								  const PX_ChangeRecord * /*pcr*/)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	return false;
}

bool fl_ShadowListener::insertStrux(PL_StruxFmtHandle /*sfh*/,
									   const PX_ChangeRecord * /*pcr*/,
									   PL_StruxDocHandle /*sdh*/,
									   PL_ListenerId /*lid*/,
									   void (* /*pfnBindHandles*/)(PL_StruxDocHandle sdhNew,
															   PL_ListenerId lid,
															   PL_StruxFmtHandle sfhNew))
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	
	return false;
}

bool fl_ShadowListener::signal(UT_uint32 /*iSignal*/)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return false;
}

