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

#include <stdio.h>
#include <stdlib.h>

#include "ut_types.h"
#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_string.h"
#include "pt_Types.h"
#include "px_ChangeRecord.h"
#include "px_CR_Object.h"
#include "px_CR_ObjectChange.h"
#include "px_CR_Span.h"
#include "px_CR_SpanChange.h"
#include "px_CR_Strux.h"
#include "px_CR_StruxChange.h"
#include "px_CR_FmtMark.h"
//###TF#include "px_CR_Bookmark.h"
#include "px_CR_FmtMarkChange.h"
#include "px_CR_Glob.h"
#include "fv_View.h"
#include "fl_DocListener.h"
#include "fl_DocLayout.h"
#include "fl_SectionLayout.h"
#include "fl_BlockLayout.h"
#include "fp_Line.h"
#include "fp_Run.h"
#include "gr_Graphics.h"
#include "pd_Document.h"
#include "pp_AttrProp.h"
#include "xap_Frame.h"

#define UPDATE_LAYOUT_ON_SIGNAL

/*!
 Create DocListener
 \param doc Client of this DocListener
 \param pLayout Layout notified by this DocListener
*/
fl_DocListener::fl_DocListener(PD_Document* doc, FL_DocLayout *pLayout)
{
	m_pDoc = doc;
	m_pLayout = pLayout;
	m_bScreen = pLayout->getGraphics()->queryProperties(GR_Graphics::DGP_SCREEN);
	m_iGlobCounter = 0;
	m_pCurrentSL = NULL;
}

/*!
 Destruct DocListener
*/
fl_DocListener::~fl_DocListener()
{
}

/*!
 */
bool fl_DocListener::populate(PL_StruxFmtHandle sfh,
							  const PX_ChangeRecord * pcr)
{
	UT_ASSERT(m_pLayout);
	//UT_DEBUGMSG(("fl_DocListener::populate\n"));

	bool bResult = false;

	switch (pcr->getType())
	{
	case PX_ChangeRecord::PXT_InsertSpan:
	{
		const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		if(pBL->getPrev()!= NULL && pBL->getPrev()->getLastLine()==NULL)
		{
			UT_DEBUGMSG(("In DocListner no LastLine in Previous Block Fixing this now \n"));
			UT_DEBUGMSG(("getPrev = %d this = %d \n",pBL->getPrev(),pBL));
			if( pBL->getSectionLayout()->getType() != FL_SECTION_HDRFTR)
				pBL->getPrev()->format();
		}

		PT_BlockOffset blockOffset = pcrs->getBlockOffset();
		UT_uint32 len = pcrs->getLength();
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_populateSpan(pBL, pcrs, blockOffset, len);
		}
		else
			bResult = pBLSL->bl_doclistener_populateSpan(pBL, pcrs, blockOffset, len);
		if(pBL->getLastLine()==NULL)
		{
			UT_DEBUGMSG(("In  DocListner no LastLine in this block fixing this now \n"));
			UT_DEBUGMSG(("getPrev = %d this = %d \n",pBL->getPrev(),pBL));
			if(pBL->getSectionLayout()->getType() != FL_SECTION_HDRFTR && pBL->getPrev()!= NULL)
				pBL->format();
			//UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		}

		goto finish_up;
	}

	case PX_ChangeRecord::PXT_InsertObject:
	{
		const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *>(pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		PT_BlockOffset blockOffset = pcro->getBlockOffset();

		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_populateObject(pBL, blockOffset,pcro);
		}
		else
			bResult = pBLSL->bl_doclistener_populateObject(pBL, blockOffset,pcro);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_InsertFmtMark:
	{
		const PX_ChangeRecord_FmtMark * pcrfm = static_cast<const PX_ChangeRecord_FmtMark *>(pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_insertFmtMark(pBL, pcrfm);
		}
		else
			bResult = pBLSL->bl_doclistener_insertFmtMark(pBL, pcrfm);
		goto finish_up;
	}
	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return false;
	}

 finish_up:
	if (0 == m_iGlobCounter)
	{
#ifndef UPDATE_LAYOUT_ON_SIGNAL
		m_pLayout->updateLayout();
#endif
	}
	
	return bResult;
}

/*!
 */
bool fl_DocListener::populateStrux(PL_StruxDocHandle sdh,
								   const PX_ChangeRecord * pcr,
								   PL_StruxFmtHandle * psfh)
{
	UT_ASSERT(m_pLayout);

	XAP_Frame * pFrame = XAP_App::getApp()->getLastFocussedFrame();
	if(pFrame)
	{
		pFrame->nullUpdate();
		UT_DEBUGMSG(("SEVIOR: Null Update in Populate Strux \n"));
	}
//
// This piece of code detects if there is enough document to notify
// listeners and other things.
//
// If so it moves the insertion point from 0.
//
	if(m_pLayout->getView() && (m_pLayout->getView()->getPoint() == 0))
	{
		fl_DocSectionLayout * pDSL = m_pLayout->getFirstSection();
		if(pDSL)
		{
			fl_BlockLayout * pBL = pDSL->getFirstBlock();
			UT_uint32 i = 0;
			while(pBL && i< 2)
			{
				i++;
				pBL = pBL->getNext();
			}
			if(i >= 2)
			{
				m_pLayout->getView()->moveInsPtTo(FV_DOCPOS_BOD);
			}
		}
	}

	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

	switch (pcrx->getStruxType())
	{
	case PTX_Section:
	case PTX_SectionEndnote:
	{
		PT_AttrPropIndex indexAP = pcr->getIndexAP();
		const PP_AttrProp* pAP = NULL;
		xxx_UT_DEBUGMSG(("SEVIOR: Doing Populate Section in DocListener \n"));
		if (m_pDoc->getAttrProp(indexAP, &pAP) && pAP)
		{
			const XML_Char* pszSectionType = NULL;
			pAP->getAttribute("type", pszSectionType);
#ifndef NDEBUG
			if (pszSectionType) {
				UT_DEBUGMSG(("fl_DocListener::populateStrux for '%s'\n",pszSectionType));
			}
			else {
				UT_DEBUGMSG(("fl_DocListener::populateStrux for '%s'\n","(null)"));
			}
#endif
			if (!pszSectionType	|| (0 == UT_strcmp(pszSectionType, "doc")))
			{
				// Append a SectionLayout to this DocLayout
				//
				// Format Previous section is it exists to get page mapping sane.
				//
				xxx_UT_DEBUGMSG(("SEVIOR: Doing Populate DocSection in DocListener \n"));
				fl_DocSectionLayout * pPDSL = m_pLayout->getLastSection();
				if(pPDSL != NULL)
				{
					pPDSL->format();
				}
				
				fl_DocSectionLayout* pSL = new fl_DocSectionLayout(m_pLayout, sdh, pcr->getIndexAP(), FL_SECTION_DOC);
				if (!pSL)
				{
					UT_DEBUGMSG(("no memory for SectionLayout"));
					return false;
				}
				
				m_pLayout->addSection(pSL);
				
				*psfh = (PL_StruxFmtHandle)pSL;
				
				m_pCurrentSL = pSL;
			}
			else
			{
				HdrFtrType hfType = FL_HDRFTR_NONE;
				if(pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header") == 0)
				{
					hfType = FL_HDRFTR_HEADER;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header-even") == 0)
				{
					hfType = FL_HDRFTR_HEADER_EVEN;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header-first") == 0)
				{
					hfType = FL_HDRFTR_HEADER_FIRST;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header-last") == 0)
				{
					hfType = FL_HDRFTR_HEADER_LAST;
				}
				if(pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer") == 0)
				{
					hfType = FL_HDRFTR_FOOTER;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer-even") == 0)
				{
					hfType = FL_HDRFTR_FOOTER_EVEN;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer-first") == 0)
				{
					hfType = FL_HDRFTR_FOOTER_FIRST;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer-last") == 0)
				{
					hfType = FL_HDRFTR_FOOTER_LAST;
				}
				if(hfType != FL_HDRFTR_NONE)
				{
					const XML_Char* pszID = NULL;
					pAP->getAttribute("id", pszID);
					UT_DEBUGMSG(("Populating header/footer header strux \n"));
					fl_DocSectionLayout* pDocSL = m_pLayout->findSectionForHdrFtr((char*)pszID);
					UT_ASSERT(pDocSL);
			
					// Append a HdrFtrSectionLayout to this DocLayout
					fl_HdrFtrSectionLayout* pSL = new fl_HdrFtrSectionLayout(hfType, m_pLayout, pDocSL, sdh, pcr->getIndexAP());
					if (!pSL)
					{
						UT_DEBUGMSG(("no memory for SectionLayout"));
						return false;
					}
					//
					// Add the hdrFtr section to the linked list of SectionLayouts
					//
					m_pLayout->addHdrFtrSection(pSL);
					pDocSL->setHdrFtr(hfType, pSL);
					*psfh = (PL_StruxFmtHandle)pSL;
					
					m_pCurrentSL = pSL;
				}
				else if (0 == UT_strcmp(pszSectionType, "endnote"))
				{
					const XML_Char* pszID = NULL;
					pAP->getAttribute("id", pszID);
					fl_DocSectionLayout* pDocSL = m_pLayout->findSectionForEndnotes((char*)pszID);
					UT_ASSERT(pDocSL);
			
					// Append an EndnoteSectionLayout to this DocLayout
					fl_DocSectionLayout* pSL;
					if (pDocSL->getEndnote() == NULL)
					{
						pSL = new fl_DocSectionLayout(m_pLayout, sdh, pcr->getIndexAP(), FL_SECTION_ENDNOTE);
	
						if (!pSL)
						{
							UT_DEBUGMSG(("no memory for SectionLayout"));
							return false;
						}
						//
						// Add the endnote section to the linked list of SectionLayouts
						//
						m_pLayout->addEndnoteSection(pSL);
	
						pDocSL->setEndnote(pSL);
						pSL->setEndnoteOwner(pDocSL);
					}
					else
						pSL = pDocSL->getEndnote();

					*psfh = (PL_StruxFmtHandle)pSL;
					
					m_pCurrentSL = pSL;
				}
				else
				{
					UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
					return false;
				}
			}
		}
		else
		{
			// TODO fail?
		   UT_DEBUGMSG(("fl_DocListener::populateStrux - m_doc->getAttrProp() failed for structure %d\n",indexAP));
			return false;
		}
	}
	break;

	case PTX_SectionHdrFtr:
		// This path is taken on a change of page type. Eg A4 => letter.
	{
	   UT_DEBUGMSG(("fl_DocListener::populateStrux for '%s'\n","SectionHdrFtr"));
		PT_AttrPropIndex indexAP = pcr->getIndexAP();
		const PP_AttrProp* pAP = NULL;
		if (m_pDoc->getAttrProp(indexAP, &pAP) && pAP)
		{
			const XML_Char* pszSectionType = NULL;
			pAP->getAttribute("type", pszSectionType);
			if (
				!pszSectionType
				|| (0 == UT_strcmp(pszSectionType, "doc"))
				)
			{
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			}
			else
			{
				HdrFtrType hfType = FL_HDRFTR_NONE;
				if(pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header") == 0)
				{
					hfType = FL_HDRFTR_HEADER;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header-even") == 0)
				{
					hfType = FL_HDRFTR_HEADER_EVEN;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header-first") == 0)
				{
					hfType = FL_HDRFTR_HEADER_FIRST;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header-last") == 0)
				{
					hfType = FL_HDRFTR_HEADER_LAST;
				}
				if(pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer") == 0)
				{
					hfType = FL_HDRFTR_FOOTER;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer-even") == 0)
				{
					hfType = FL_HDRFTR_FOOTER_EVEN;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer-first") == 0)
				{
					hfType = FL_HDRFTR_FOOTER_FIRST;
				}
				else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer-last") == 0)
				{
					hfType = FL_HDRFTR_FOOTER_LAST;
				}
				if(hfType != FL_HDRFTR_NONE)
				{
					const XML_Char* pszID = NULL;
					pAP->getAttribute("id", pszID);

					fl_DocSectionLayout* pDocSL = m_pLayout->findSectionForHdrFtr((char*)pszID);
					if (pDocSL == NULL)
					{
						UT_DEBUGMSG(("Could not find HeaderFooter %s\n",(char*)pszID));
						return false;
					}
			
					// Append a HdrFtrSectionLayout to this DocLayout
					fl_HdrFtrSectionLayout* pSL = new fl_HdrFtrSectionLayout(hfType, m_pLayout, pDocSL, sdh, pcr->getIndexAP());
					if (!pSL)
					{
						UT_DEBUGMSG(("no memory for SectionLayout"));
						return false;
					}
					//
					// Add the hdrFtr section to the linked list of SectionLayouts
					//
					m_pLayout->addHdrFtrSection(pSL);
					pDocSL->setHdrFtr(hfType, pSL);
					*psfh = (PL_StruxFmtHandle)pSL;
					
					m_pCurrentSL = pSL;
				}
				else
				{
					return false;
				}
				break;
			}
		}
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}
	case PTX_Block:
	{
		UT_ASSERT(m_pCurrentSL);
		
		// Append a new BlockLayout to that SectionLayout
		fl_BlockLayout*	pBL = m_pCurrentSL->appendBlock(sdh, pcr->getIndexAP());
		if (!pBL)
		{
			UT_DEBUGMSG(("no memory for BlockLayout"));
			return false;
		}

		// BUGBUG: this is *not* thread-safe, but should work for now
		if (m_bScreen)
		{
			UT_uint32 reason =  0;
			if( m_pLayout->getAutoSpellCheck())
			{
				reason = (UT_uint32) FL_DocLayout::bgcrSpelling;
			}
			m_pLayout->queueBlockForBackgroundCheck(reason, pBL,true);
		}
		*psfh = (PL_StruxFmtHandle)pBL;
		if(pBL->getLastLine()==NULL)
		{
			if(pBL->getSectionLayout()->getType() != FL_SECTION_HDRFTR && pBL->getPrev() != NULL)
			{
				UT_DEBUGMSG(("In DocListner no LastLine in block append. Fixing this now \n"));
				UT_DEBUGMSG(("getPrev = %d this = %d \n",pBL->getPrev(),pBL));
				pBL->format();
			}
		}

	}
	break;
			
	default:
		UT_ASSERT(0);
		return false;
	}

	if (0 == m_iGlobCounter)
	{
#ifndef UPDATE_LAYOUT_ON_SIGNAL
		m_pLayout->updateLayout();
#endif
	}
	
	return true;
}

/*!
 */
bool fl_DocListener::change(PL_StruxFmtHandle sfh,
							const PX_ChangeRecord * pcr)
{
	//UT_DEBUGMSG(("fl_DocListener::change\n"));
	bool bResult = false;

	switch (pcr->getType())
	{
	case PX_ChangeRecord::PXT_GlobMarker:
	{
		UT_ASSERT(sfh == 0);							// globs are not strux-relative
		const PX_ChangeRecord_Glob * pcrg = static_cast<const PX_ChangeRecord_Glob *> (pcr);
		switch (pcrg->getFlags())
		{
		default:
		case PX_ChangeRecord_Glob::PXF_Null:			// not a valid glob type
			UT_ASSERT(0);
			bResult = false;
			goto finish_up;
				
		case PX_ChangeRecord_Glob::PXF_MultiStepStart:
			m_iGlobCounter++;
			bResult = true;
			goto finish_up;
			
		case PX_ChangeRecord_Glob::PXF_MultiStepEnd:
			m_iGlobCounter--;
			bResult = true;
			goto finish_up;
				
		case PX_ChangeRecord_Glob::PXF_UserAtomicStart:	// TODO decide what (if anything) we need
		case PX_ChangeRecord_Glob::PXF_UserAtomicEnd:	// TODO to do here.
			bResult = true;
			goto finish_up;
		}
	}
			
	case PX_ChangeRecord::PXT_InsertSpan:
	{
		const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			pHdr->bl_doclistener_insertSpan(pBL, pcrs);
		}
		else
		{
			bResult = pBLSL->bl_doclistener_insertSpan(pBL, pcrs);
		}
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_DeleteSpan:
	{
		const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_deleteSpan(pBL, pcrs);
		}
		else
			bResult = pBLSL->bl_doclistener_deleteSpan(pBL, pcrs);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_ChangeSpan:
	{
		const PX_ChangeRecord_SpanChange * pcrsc = static_cast<const PX_ChangeRecord_SpanChange *>(pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_changeSpan(pBL, pcrsc);
		}
		else
			bResult = pBLSL->bl_doclistener_changeSpan(pBL, pcrsc);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_InsertFmtMark:
	{
		const PX_ChangeRecord_FmtMark * pcrfm = static_cast<const PX_ChangeRecord_FmtMark *>(pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		xxx_UT_DEBUGMSG(("DocListener: InsertFmtMark strux type = %d \n",pL->getType()));
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_insertFmtMark(pBL, pcrfm);
		}
		else
			bResult = pBLSL->bl_doclistener_insertFmtMark(pBL, pcrfm);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_DeleteFmtMark:
	{
		const PX_ChangeRecord_FmtMark * pcrfm = static_cast<const PX_ChangeRecord_FmtMark *>(pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_deleteFmtMark(pBL, pcrfm);
		}
		else
			bResult = pBLSL->bl_doclistener_deleteFmtMark(pBL, pcrfm);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_ChangeFmtMark:
	{
		const PX_ChangeRecord_FmtMarkChange * pcrfmc = static_cast<const PX_ChangeRecord_FmtMarkChange *>(pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_changeFmtMark(pBL, pcrfmc);
		}
		else
			bResult = pBLSL->bl_doclistener_changeFmtMark(pBL, pcrfmc);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_DeleteStrux:
	{
		const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

		switch (pcrx->getStruxType())
		{
		case PTX_Section:
		case PTX_SectionEndnote:
		{
			fl_Layout * pL = (fl_Layout *)sfh;
			UT_ASSERT(pL->getType() == PTX_Section || pL->getType() == PTX_SectionEndnote);
			fl_DocSectionLayout * pSL = static_cast<fl_DocSectionLayout *>(pL);
			bResult = pSL->doclistener_deleteStrux(pcrx);
			goto finish_up;
		}
		case PTX_Block:
		{
			fl_Layout * pL = (fl_Layout *)sfh;
			UT_ASSERT(pL->getType() == PTX_Block);
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			if(pBLSL->getType() == FL_SECTION_SHADOW)
			{
				fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
				bResult = pHdr->bl_doclistener_deleteStrux(pBL, pcrx);
			}
			else
				bResult = pBLSL->bl_doclistener_deleteStrux(pBL, pcrx);
			goto finish_up;
		}
		case PTX_SectionHdrFtr:
		{
			fl_Layout * pL = (fl_Layout *)sfh;
			UT_ASSERT(pL->getType() == PTX_SectionHdrFtr);
			fl_HdrFtrSectionLayout * pSL = static_cast<fl_HdrFtrSectionLayout *>(pL);
//
// Nuke the HdrFtrSectionLayout and the shadows associated with it.
//
			pSL->doclistener_deleteStrux(pcrx); 
			m_pLayout->updateLayout();
			goto finish_up;
		}

		default:
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			bResult = false;
			goto finish_up;
		}
	}
					
	case PX_ChangeRecord::PXT_ChangeStrux:
	{
		const PX_ChangeRecord_StruxChange * pcrxc = static_cast<const PX_ChangeRecord_StruxChange *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;

		// TODO getOldIndexAP() is only intended for use by the document.
		// TODO this assert is probably wrong. --- BUT EVERYTIME IT HAS
		// TODO GONE OFF, I'VE FOUND A BUG, SO MAYBE WE SHOULD KEEP IT :-)
		// UT_ASSERT(pL->getAttrPropIndex() == pcrxc->getOldIndexAP());
		// UT_ASSERT(pL->getAttrPropIndex() != pcr->getIndexAP());

		switch (pL->getType())
		{
		case PTX_Section:
		case PTX_SectionEndnote:
		{
			fl_DocSectionLayout* pSL = static_cast<fl_DocSectionLayout*>(pL);
			
			PT_AttrPropIndex indexAP = pcr->getIndexAP();
			const PP_AttrProp* pAP = NULL;
			
			bool bres = (m_pDoc->getAttrProp(indexAP, &pAP) && pAP);
			UT_ASSERT(bres);
			PL_StruxDocHandle sdh = pL->getStruxDocHandle();
	
			const XML_Char* pszSectionType = NULL;
			pAP->getAttribute("type", pszSectionType);
			//
			// OK Sevior adds code to actually change a 
			// sectionlayout to
			// a header/footer layout
			//
			// Strategy: Collapse all the blocks in this section.
			// create a header/footer sectionlayout ala populate_strux
			// transfer the blocks in this sectionlayout to the
			// new header/footer and format just the shadows
			//
			HdrFtrType hfType = FL_HDRFTR_NONE;
			if(pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header") == 0)
			{
				hfType = FL_HDRFTR_HEADER;
			}
			else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header-even") == 0)
			{
				hfType = FL_HDRFTR_HEADER_EVEN;
			}
			else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header-first") == 0)
			{
				hfType = FL_HDRFTR_HEADER_FIRST;
			}
			else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"header-last") == 0)
			{
				hfType = FL_HDRFTR_HEADER_LAST;
			}
			if(pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer") == 0)
			{
				hfType = FL_HDRFTR_FOOTER;
			}
			else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer-even") == 0)
			{
				hfType = FL_HDRFTR_FOOTER_EVEN;
			}
			else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer-first") == 0)
			{
				hfType = FL_HDRFTR_FOOTER_FIRST;
			}
			else if (pszSectionType && *pszSectionType && UT_strcmp(pszSectionType,"footer-last") == 0)
			{
				hfType = FL_HDRFTR_FOOTER_LAST;
			}
			if(hfType != FL_HDRFTR_NONE)
			{
				//
				//  OK first we need a previous section with a
				//  matching ID
				//
				const XML_Char* pszID = NULL;
				pAP->getAttribute("id", pszID);

				fl_DocSectionLayout* pDocSL = m_pLayout->findSectionForHdrFtr((char*)pszID);
				UT_ASSERT(pDocSL); 
			        
				// Append a HdrFtrSectionLayout to this DocLayout
				fl_HdrFtrSectionLayout* pHeadSL = new fl_HdrFtrSectionLayout(hfType, m_pLayout, pDocSL, sdh, pcr->getIndexAP());
				if (!pHeadSL)
				{
					UT_DEBUGMSG(("no memory for SectionLayout"));
					return false;
				}
				//
				// Add the hdrFtr section to the linked list of SectionLayouts
				//
				m_pLayout->addHdrFtrSection(pHeadSL);
				//
				// Set the pointers to this header/footer
				//
				pDocSL->setHdrFtr(hfType, pHeadSL);

				// OK now clean up the old section and transfer
				// blocks into this header section.
				
				pHeadSL->changeIntoHdrFtrSection(pSL);

				bResult = true;
				goto finish_up;
			}
			else if(pszSectionType && UT_strcmp(pszSectionType,"endnote") == 0)
			{
				//
				//  OK first we need a previous section with a
				//  matching ID
				//
				const XML_Char* pszID = NULL;
				pAP->getAttribute("id", pszID);

				fl_DocSectionLayout* pDocSL = m_pLayout->findSectionForEndnotes((char*)pszID);
				UT_ASSERT(pDocSL); 

				// You know, maybe we already have an endnote section!
				if (pDocSL->getEndnote() != NULL)
				{
					bResult = pSL->doclistener_changeStrux(pcrxc);
					goto finish_up;
				}
			    
				UT_ASSERT((UT_SHOULD_NOT_HAPPEN));
				// Append an endnote DocSectionLayout to this DocLayout
				fl_DocSectionLayout* pEndSL = new fl_DocSectionLayout(m_pLayout, sdh, pcr->getIndexAP(), FL_SECTION_ENDNOTE);
				if (!pEndSL)
				{
					UT_DEBUGMSG(("no memory for SectionLayout"));
					return false;
				}
				//
				// Add the endnote section to the linked list of SectionLayouts
				//
				m_pLayout->addEndnoteSection(pEndSL);
				//
				// Set the pointers to this endnote
				//
				pDocSL->setEndnote(pEndSL);
				pEndSL->setEndnoteOwner(pDocSL);

				// OK Now clean up the old section and (don't) transfer
				// blocks into this endnote section.
				// Maybe this does the trick, though.  I don't know. - PL
  				bResult = pEndSL->doclistener_changeStrux(pcrxc);

				if (!bResult)
				{
					UT_DEBUGMSG(("couldn't changeStrux"));
					return false;
				}
			        
				bResult = true;
				goto finish_up;
			}
			if(pSL->getType() == FL_SECTION_DOC)
			{
				fl_DocSectionLayout * pDSL = (fl_DocSectionLayout *) pSL;
				m_pLayout->changeDocSections(pcrxc,pDSL);
				bResult = true;
				goto finish_up;
			}
 			bResult = pSL->doclistener_changeStrux(pcrxc);
			goto finish_up;
		}
		
		case PTX_Block:
		{
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			if(pBLSL->getType() == FL_SECTION_SHADOW)
			{
				fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
				bResult = pHdr->bl_doclistener_changeStrux(pBL, pcrxc);
			}
			else 
				bResult = pBLSL->bl_doclistener_changeStrux(pBL, pcrxc);
			goto finish_up;
		}
		case PTX_SectionHdrFtr:
		{
//
// OK A previous "insertStrux" has created a HdrFtrSectionLayout but it
// Doesn't know if it's a header or a footer or the DocSection and hences pages
// It associated with. Tell it now.
//
			fl_HdrFtrSectionLayout* pHFSL = static_cast<fl_HdrFtrSectionLayout*>(pL);
			
			PT_AttrPropIndex indexAP = pcr->getIndexAP();
//
// Save this new index to the Attributes/Properties of this section in the
// HdrFtrSection Class
//
			pHFSL->setAttrPropIndex(pcrxc->getIndexAP());
			const PP_AttrProp* pHFAP = NULL;
			
			bool bres = (m_pDoc->getAttrProp(indexAP, &pHFAP) && pHFAP);
			UT_ASSERT(bres);
	
			const XML_Char* pszHFSectionType = NULL;
			pHFAP->getAttribute("type", pszHFSectionType);
			//
            // Look for type of Hdr/Ftr
			//
			HdrFtrType hfType = FL_HDRFTR_NONE;
			if(pszHFSectionType && *pszHFSectionType && UT_strcmp(pszHFSectionType,"header") == 0)
			{
				hfType = FL_HDRFTR_HEADER;
			}
			else if (pszHFSectionType && *pszHFSectionType && UT_strcmp(pszHFSectionType,"header-even") == 0)
			{
				hfType = FL_HDRFTR_HEADER_EVEN;
			}
			else if (pszHFSectionType && *pszHFSectionType && UT_strcmp(pszHFSectionType,"header-first") == 0)
			{
				hfType = FL_HDRFTR_HEADER_FIRST;
			}
			else if (pszHFSectionType && *pszHFSectionType && UT_strcmp(pszHFSectionType,"header-last") == 0)
			{
				hfType = FL_HDRFTR_HEADER_LAST;
			}
			if(pszHFSectionType && *pszHFSectionType && UT_strcmp(pszHFSectionType,"footer") == 0)
			{
				hfType = FL_HDRFTR_FOOTER;
			}
			else if (pszHFSectionType && *pszHFSectionType && UT_strcmp(pszHFSectionType,"footer-even") == 0)
			{
				hfType = FL_HDRFTR_FOOTER_EVEN;
			}
			else if (pszHFSectionType && *pszHFSectionType && UT_strcmp(pszHFSectionType,"footer-first") == 0)
			{
				hfType = FL_HDRFTR_FOOTER_FIRST;
			}
			else if (pszHFSectionType && *pszHFSectionType && UT_strcmp(pszHFSectionType,"footer-last") == 0)
			{
				hfType = FL_HDRFTR_FOOTER_LAST;
			}

			if(hfType !=  FL_HDRFTR_NONE )
			{
				//
				//  OK now we need a previous section with a
				//  matching ID
				//
				const XML_Char* pszHFID = NULL;
				pHFAP->getAttribute("id", pszHFID);

				fl_DocSectionLayout* pDocSL = m_pLayout->findSectionForHdrFtr((char*)pszHFID);
				
				UT_ASSERT(pDocSL); 
			        
				// Set the Headerness and overall docSectionLayout
				//
				pHFSL->setDocSectionLayout(pDocSL);
				pHFSL->setHdrFtr(hfType);
				//
				// Set the pointers to this header/footer
				//
				pDocSL->setHdrFtr(hfType, pHFSL);

				// OK now Format this and attach it to it's pages
				
				pHFSL->format();

				bResult = true;
				goto finish_up;
			}
			UT_DEBUGMSG(("SEVIOR: Unknown change record on a SectionHdrFtr strux \n"));
			UT_DEBUGMSG(("SEVIOR: Most like we're undoing an Insert HdrFtr. Carry on! \n"));
			bResult = true;
			goto finish_up;
		}
		default:
		{
			UT_ASSERT(0);
			bResult = false;
			goto finish_up;
		}
		}
	}

	case PX_ChangeRecord::PXT_InsertStrux:
	{
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		bResult = false;
		goto finish_up;
	}
	case PX_ChangeRecord::PXT_InsertObject:
	{
		const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_insertObject(pBL, pcro);
		}
		else
			bResult = pBLSL->bl_doclistener_insertObject(pBL, pcro);
		goto finish_up;
	}
	case PX_ChangeRecord::PXT_DeleteObject:
	{
		const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_deleteObject(pBL, pcro);
		}
		else
			bResult = pBLSL->bl_doclistener_deleteObject(pBL, pcro);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_ChangeObject:
	{
		const PX_ChangeRecord_ObjectChange * pcroc = static_cast<const PX_ChangeRecord_ObjectChange *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		if(pBLSL->getType() == FL_SECTION_SHADOW)
		{
			fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
			bResult = pHdr->bl_doclistener_changeObject(pBL, pcroc);
		}
		else
			bResult = pBLSL->bl_doclistener_changeObject(pBL, pcroc);
		goto finish_up;
	}

	case PX_ChangeRecord::PXT_ChangePoint:
	{
		FV_View* pView = m_pLayout->getView();
		if (pView && pView->isActive())
			pView->_setPoint(pcr->getPosition());
		goto finish_up;
	}
	case PX_ChangeRecord::PXT_ListUpdate:
	{
		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		pBL->listUpdate();
		goto finish_up;

	}
	case PX_ChangeRecord::PXT_StopList:
	{
		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		pBL->StopListInBlock();
		goto finish_up;
	}
	case PX_ChangeRecord::PXT_UpdateField:
	{
		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		pBL->format();
		FV_View* pView = m_pLayout->getView();
		pView->updateScreen();
		goto finish_up;
	}
	case PX_ChangeRecord::PXT_RemoveList:
	{
		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		pBL->m_bStopList = true;
		pBL->_deleteListLabel();
		pBL->m_pAutoNum = NULL;
		pBL->m_bListItem = false;
		pBL->m_bStopList = false;
		goto finish_up;
	}
	case PX_ChangeRecord::PXT_UpdateLayout:
	{
		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		pBL->getDocLayout()->updateLayout();
		FV_View * pView = pBL->getDocLayout()->getView();
		if(pView)
		{
		// remember the state of the cursor.
			bool bCursorErased = false;
			if (pView->isCursorOn() == true)
			{
				pView->eraseInsertionPoint();
				bCursorErased = true;
			}
//			pView->updateScreen(false);
			pView->notifyListeners(AV_CHG_HDRFTR);
//
// Redraw the cursor if needed
//
			if (bCursorErased == true)
			{
				pView->drawInsertionPoint();
			}
		}

		goto finish_up;
	}
#if 0 //###TF
	case PX_ChangeRecord::PXT_InsertBookmark:
	{
		const PX_ChangeRecord_Bookmark * pcrfm = static_cast<const PX_ChangeRecord_Bookmark *>(pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_DEBUGMSG(("DocListener: InsertBookmark strux type = %d \n",pL->getType()));
		//UT_ASSERT(pL->getType() == PTX_Block);
		//fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		//fl_SectionLayout* pBLSL = pBL->getSectionLayout();
		//if(pBLSL->getType() == FL_SECTION_SHADOW)
		//{
		//	fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
		//	bResult = pHdr->bl_doclistener_insertFmtMark(pBL, pcrfm);
		//}
		//else
		//	bResult = pBLSL->bl_doclistener_insertFmtMark(pBL, pcrfm);
		goto finish_up;
	}
#endif
	
	default:
	{
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		bResult = false;
		goto finish_up;
	}
	}

 finish_up:
	if (0 == m_iGlobCounter)
	{
#ifndef UPDATE_LAYOUT_ON_SIGNAL
		m_pLayout->updateLayout();
#endif
	}
	
	return bResult;
}

/*!
 */
bool fl_DocListener::insertStrux(PL_StruxFmtHandle sfh,
								 const PX_ChangeRecord * pcr,
								 PL_StruxDocHandle sdh,
								 PL_ListenerId lid,
								 void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
														 PL_ListenerId lid,
														 PL_StruxFmtHandle sfhNew))
{
	UT_DEBUGMSG(("fl_DocListener::insertStrux\n"));

	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);
	fl_Layout * pL = (fl_Layout *)sfh;
	xxx_UT_DEBUGMSG(("Previous strux type %d \n",pL->getType()));
	xxx_UT_DEBUGMSG(("Insert strux type %d \n",pcrx->getStruxType()));
	switch (pL->getType())				// see what the immediately prior strux is.
	{
	case PTX_Section:					// the immediately prior strux is a section.
    {
		switch (pcrx->getStruxType())	// see what we are inserting.
		{
		case PTX_Section:				// we are inserting a section.
		   {	// We are inserting a section immediately after a section (with no
			   // intervening block).  This is probably a bug, because there should
			   // at least be an empty block between them (so that the user can set
			   // the cursor there and start typing, if nothing else).
			   UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			   return false;
		   }		
		case PTX_Block:					// we are inserting a block.
		   {
			   // The immediately prior strux is a section.  So, this
			   // will become the first block of the section and have no
			   // text.

			   fl_SectionLayout * pSL = static_cast<fl_SectionLayout *>(pL);
			   bool bResult = pSL->bl_doclistener_insertBlock(NULL, pcrx,sdh,lid,pfnBindHandles);
			   return bResult;
		   }
		case PTX_SectionHdrFtr:				// we are inserting a HdrFtr section.
		   {
			   // We are inserting a HdrFtr section immediately after a section 
			   // (with no
			   // intervening block).  This is probably a bug, because there should
			   // at least be an empty block between them (so that the user can set
			   // the cursor there and start typing, if nothing else).
			   UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			   return false;
		   }
		case PTX_SectionEndnote:
		   {
			   UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			   return false;
		   }
		default:						// unknown strux.
		   {
			   UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			   return false;
		   }
		}
	}	
	case PTX_SectionHdrFtr:	   // the immediately prior strux is a HdrFtr.
	{	
		xxx_UT_DEBUGMSG(("Immediately prior strux is a HdrFtr \n"));
		switch (pcrx->getStruxType())	// see what we are inserting.
		{
		case PTX_Section:				// we are inserting a section.
		   {
			   // The immediately prior strux is a hdrftr. This should not
			   // happen.
			   UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			   return false;
		   }
		case PTX_Block:					// we are inserting a block.
		   {
			   xxx_UT_DEBUGMSG(("Inserting a block after hdrftr \n"));
			   // The immediately prior strux is a hdrftr.  Insert the new
			   // block.
			   fl_SectionLayout* pBLSL = static_cast<fl_SectionLayout *>(pL);
			   bool bResult = pBLSL->bl_doclistener_insertBlock(NULL, pcrx,sdh,lid,pfnBindHandles);
			   return bResult;
		   }
	    case PTX_SectionHdrFtr:
		   {
			   // The immediately prior strux is a HdrFtr.  
			   // This should not happen.

			   UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			   return false;
		   }
		case PTX_SectionEndnote:
		   {	
			   // The immediately prior strux is a HdrFtr.  
			   // This should not happen.

			   UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			   return false;
		   }
		default:
		   {
			   UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			   return false;
		   }
		}
	}
    case PTX_Block:						// the immediately prior strux is a block.
    {
		switch (pcrx->getStruxType())	// see what we are inserting.
		{
		case PTX_Section:				// we are inserting a section.
		   {
			   // The immediately prior strux is a block.  Everything
			   // from this point forward (to the next section) needs to
			   // be re-parented to this new section.  We also need to
			   // verify that there is a block immediately after this new
			   // section -- a section must be followed by a block
			   // because a section cannot contain content.
			
			   fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			   UT_DEBUGMSG(("SEVIOR: Doing Insert Section Correctly \n"));
			   fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			   bool bResult = pBLSL->bl_doclistener_insertSection(pBL, FL_SECTION_DOC, pcrx,sdh,lid,pfnBindHandles);
	
			   return bResult;
		   }
		
		case PTX_Block:					// we are inserting a block.
		   {
			   // The immediately prior strux is also a block.  Insert the new
			   // block and split the content between the two blocks.
			   fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			   fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			   bool bResult = true;
			   if(pBLSL->getType() == FL_SECTION_SHADOW)
			   {
				   fl_HdrFtrSectionLayout * pHdr = pBLSL->getHdrFtrSectionLayout();
				   bResult = pHdr->bl_doclistener_insertBlock(pBL, pcrx,sdh,lid,pfnBindHandles);
			   }
			   else
			   {
				   bResult = pBLSL->bl_doclistener_insertBlock(pBL, pcrx,sdh,lid,pfnBindHandles);
			   }
			   return bResult;
		   }
	    case PTX_SectionHdrFtr:
		   {
			   // The immediately prior strux is a block.  Everything
			   // from this point forward (to the next section) needs to
			   // be re-parented to this new HdrFtr section.  We also need to
			   // verify that there is a block immediately after this new
			   // section -- a section must be followed by a block
			   // because a section cannot contain content.
			
			   fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			   fl_SectionLayout* pBLSL = pBL->getSectionLayout();
			   bool bResult = pBLSL->bl_doclistener_insertSection(pBL, FL_SECTION_HDRFTR, pcrx,sdh,lid,pfnBindHandles);
			   return bResult;
		   }
		case PTX_SectionEndnote:
		   {
			   fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			   fl_SectionLayout* pBLSL = pBL->getSectionLayout();

			   bool bResult = pBLSL->bl_doclistener_insertSection(pBL, FL_SECTION_ENDNOTE, pcrx,sdh,lid,pfnBindHandles);
			   return bResult;
		   }
		default:
		   {
			   UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			   return false;
		   }
		}
	}
	default:
	{	
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return false;
	}

	} // finish the overall switch

	/*NOTREACHED*/
	UT_ASSERT(0);
	return false;
}

/*!
 */
bool fl_DocListener::signal(UT_uint32 iSignal)
{
	bool bCursorErased = false;
	FV_View* pView = m_pLayout->getView();

	switch (iSignal)
	{
	case PD_SIGNAL_UPDATE_LAYOUT:
#ifdef UPDATE_LAYOUT_ON_SIGNAL
		m_pLayout->updateLayout();
#endif
		if(pView->isCursorOn()== true)
		{
			pView->eraseInsertionPoint();
			bCursorErased = true;
		}
		pView->updateScreen();
		if(bCursorErased == true)
		{
			pView->drawInsertionPoint();
		}

		break;
	case PD_SIGNAL_REFORMAT_LAYOUT:
		if(pView->isCursorOn()== true)
		{
			pView->eraseInsertionPoint();
			bCursorErased = true;
		}
		m_pLayout->formatAll();
		if(bCursorErased == true)
		{
			pView->drawInsertionPoint();
		}

		break;
		
	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		break;
	}

	return true;
}
