 
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


#include <stdio.h>
#include <stdlib.h>

#include "ut_types.h"
#include "pt_Types.h"
#include "px_ChangeRecord.h"
#include "px_ChangeRecord_Span.h"
#include "px_ChangeRecord_SpanChange.h"
#include "px_ChangeRecord_Strux.h"
#include "px_ChangeRecord_StruxChange.h"
#include "fv_View.h"
#include "fl_DocListener.h"
#include "fl_DocLayout.h"
#include "fl_SectionLayout.h"
#include "fl_ColumnSetLayout.h"
#include "fl_ColumnLayout.h"
#include "fl_BlockLayout.h"
#include "fp_Line.h"
#include "fp_Run.h"
#include "pd_Document.h"

#include "ut_debugmsg.h"
#include "ut_assert.h"

fl_DocListener::fl_DocListener(PD_Document* doc, FL_DocLayout *pLayout)
{
	m_pDoc = doc;
	m_pLayout = pLayout;
}

fl_DocListener::~fl_DocListener()
{
}

UT_Bool fl_DocListener::populate(PL_StruxFmtHandle sfh,
								 const PX_ChangeRecord * pcr)
{
	UT_ASSERT(m_pLayout);
	UT_DEBUGMSG(("fl_DocListener::populate\n"));
	pcr->dump();

	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertSpan);
	const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

	fl_Layout * pL = (fl_Layout *)sfh;
	switch (pL->getType())
	{
	case PTX_Block:
		{
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			PT_DocPosition docPosBlock = m_pDoc->getStruxPosition(pBL->m_sdh);
			PT_BlockOffset blockOffset = (pcr->getPosition() - docPosBlock);

			fp_Run * pRun = pBL->m_pFirstRun;
			fp_Run * pLastRun = NULL;
			UT_uint32 offset = 1;		// TODO PAUL, check/macro this -- first text in Paragraph is 1 from block.

			while (pRun)
			{
				pLastRun = pRun;
				offset += pRun->m_iLen;
				pRun = pRun->getNext();
			}

			UT_ASSERT(offset==blockOffset);
			UT_uint32 len = pcrs->getLength();
			fp_Run * pNewRun = new fp_Run(pBL, m_pLayout->getGraphics(), offset, len);
			if (!pNewRun)
			{
				UT_DEBUGMSG(("Could not allocate run\n"));
				return UT_FALSE;
			}
			
			pBL->m_gbCharWidths.ins(offset, len);
			pNewRun->calcWidths(&pBL->m_gbCharWidths);
			
			if (pLastRun)
			{
				pLastRun->setNext(pNewRun);
				pNewRun->setPrev(pLastRun);
			}
			else
			{
				pBL->m_pFirstRun = pNewRun;
			}
		}
		return UT_TRUE;
			
	case PTX_Section:
	case PTX_ColumnSet:
	case PTX_Column:
	default:
		UT_ASSERT((0));
		return UT_FALSE;
	}
}

UT_Bool fl_DocListener::populateStrux(PL_StruxDocHandle sdh,
									  const PX_ChangeRecord * pcr,
									  PL_StruxFmtHandle * psfh)
{
	UT_ASSERT(m_pLayout);
	UT_DEBUGMSG(("fl_DocListener::populateStrux\n"));
	pcr->dump();

	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

	switch (pcrx->getStruxType())
	{
	case PTX_Section:
		{
			// append a SectionLayout to this DocLayout
			fl_SectionLayout* pSL = new fl_SectionLayout(m_pLayout, sdh);
			if (!pSL)
			{
				UT_DEBUGMSG(("no memory for SectionLayout"));
				return UT_FALSE;
			}
			pSL->setAttrPropIndex(pcr->getIndexAP());
			m_pLayout->m_vecSectionLayouts.addItem(pSL);

			*psfh = (PL_StruxFmtHandle)pSL;
		}
		break;

	case PTX_ColumnSet:
		{
			// locate the last SectionLayout
			int countSections = m_pLayout->m_vecSectionLayouts.getItemCount();
			UT_ASSERT(countSections > 0);
			fl_SectionLayout* pSL = (fl_SectionLayout*) m_pLayout->m_vecSectionLayouts.getNthItem(countSections - 1);
			UT_ASSERT(pSL);
			fl_ColumnSetLayout * pCSL = new fl_ColumnSetLayout(pSL,sdh);
			if (!pCSL)
			{
				UT_DEBUGMSG(("no memory for ColumnSetLayout"));
				return UT_FALSE;
			}
			pCSL->setAttrPropIndex(pcr->getIndexAP());
			UT_ASSERT(pSL->getColumnSetLayout()==NULL);
			pSL->setColumnSetLayout(pCSL);

			*psfh = (PL_StruxFmtHandle)pCSL;
		}
		break;
			
	case PTX_Column:
		{
			// locate the last SectionLayout
			int countSections = m_pLayout->m_vecSectionLayouts.getItemCount();
			UT_ASSERT(countSections > 0);
			fl_SectionLayout* pSL = (fl_SectionLayout*) m_pLayout->m_vecSectionLayouts.getNthItem(countSections - 1);
			UT_ASSERT(pSL);
			fl_ColumnSetLayout * pCSL =	pSL->getColumnSetLayout();
			UT_ASSERT(pCSL);
			fl_ColumnLayout * pCL = new fl_ColumnLayout(pCSL,sdh);
			if (!pCL)
			{
				UT_DEBUGMSG(("no memory for ColumnLayout"));
				return UT_FALSE;
			}
			pCL->setAttrPropIndex(pcr->getIndexAP());
			pCSL->appendColumnLayout(pCL);

			*psfh = (PL_StruxFmtHandle)pCL;
		}
		break;
			
	case PTX_Block:
		{
			// locate the last SectionLayout
			int countSections = m_pLayout->m_vecSectionLayouts.getItemCount();
			UT_ASSERT(countSections > 0);
			fl_SectionLayout* pSL = (fl_SectionLayout*) m_pLayout->m_vecSectionLayouts.getNthItem(countSections - 1);
			UT_ASSERT(pSL);

			// append a new BlockLayout to that SectionLayout
			fl_BlockLayout*	pBL = pSL->appendBlock(sdh);
			if (!pBL)
			{
				UT_DEBUGMSG(("no memory for BlockLayout"));
				return UT_FALSE;
			}
			pBL->setAttrPropIndex(pcr->getIndexAP());

			*psfh = (PL_StruxFmtHandle)pBL;
		}
		break;
			
	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}

	return UT_TRUE;
}

UT_Bool fl_DocListener::change(PL_StruxFmtHandle sfh,
							   const PX_ChangeRecord * pcr)
{
	UT_DEBUGMSG(("fl_DocListener::change\n"));
	pcr->dump();

	switch (pcr->getType())
	{
	case PX_ChangeRecord::PXT_InsertSpan:
		{
			const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

			fl_Layout * pL = (fl_Layout *)sfh;
			switch (pL->getType())
			{
			case PTX_Block:
				{
					fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
					FV_View* pView = m_pLayout->m_pView;
					PT_DocPosition docPosBlock = m_pDoc->getStruxPosition(pBL->m_sdh);
					PT_BlockOffset blockOffset = (pcr->getPosition() - docPosBlock);
					UT_uint32 len = pcrs->getLength();
					UT_ASSERT(len>0);

					pBL->m_gbCharWidths.ins(blockOffset, len);

					UT_Bool bFormat = UT_FALSE;
	
					fp_Run* pRun = pBL->m_pFirstRun;
					/*
						Having fixed the char widths array, we need to update 
						all the run offsets.  We call each run individually to 
						update its offsets.  It returns true if its size changed, 
						thus requiring us to remeasure it.
					*/
					while (pRun)
					{					
						if (pRun->ins(blockOffset, len, pcrs->getIndexAP()))
						{
							if (pcrs->getIndexAP() != pcrs->getOldIndexAP())
							{
								/*
									The format changed too, so this needs to 
									wind up in its own run.

									Since the pRun->ins() already widened it, 
									we just need to split it up.
									
									Note that split() calls calcWidths for us.
								*/
								bFormat = UT_TRUE;

								// first split off the right part
								pRun->split(blockOffset+len);

								if (blockOffset > pRun->m_iOffsetFirst)
								{
									// split off the left part
									pRun->split(blockOffset);

									// skip over the left part
									pRun = pRun->getNext();
								}

								// pick up new formatting for this run
								pRun->clearScreen();
								pRun->lookupProperties();
								pRun->calcWidths(&pBL->m_gbCharWidths);

								// skip over the right part
								pRun = pRun->getNext();

								// all done, so clear any temp formatting
								if (pView && pView->_isPointAP())
								{
									UT_ASSERT(pcrs->getIndexAP()==pView->_getPointAP());
									pView->_clearPointAP(UT_FALSE);
								}
							}
							else
							{
								pRun->calcWidths(&pBL->m_gbCharWidths);
							}
						}

						pRun = pRun->getNext();
					}

					if (bFormat)
					{
						pBL->format();
						pBL->draw(m_pLayout->getGraphics());
					}
					else
						pBL->reformat();

					if (pView)
						pView->_setPoint(pcr->getPosition()+len);
				}
				return UT_TRUE;
					
			case PTX_Section:
			case PTX_ColumnSet:
			case PTX_Column:
			default:
				UT_ASSERT((0));
				return UT_FALSE;
			}
		}
		break;

	case PX_ChangeRecord::PXT_DeleteSpan:
		{
			const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

			fl_Layout * pL = (fl_Layout *)sfh;
			switch (pL->getType())
			{
			case PTX_Block:
				{
					fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
					PT_DocPosition docPosBlock = m_pDoc->getStruxPosition(pBL->m_sdh);
					PT_BlockOffset blockOffset = (pcr->getPosition() - docPosBlock);
					UT_uint32 len = pcrs->getLength();
					UT_ASSERT(len>0);

					pBL->m_gbCharWidths.del(blockOffset, len);
	
					fp_Run* pRun = pBL->m_pFirstRun;
					/*
						Having fixed the char widths array, we need to update 
						all the run offsets.  We call each run individually to 
						update its offsets.  It returns true if its size changed, 
						thus requiring us to remeasure it.
					*/
					while (pRun)
					{					
						if (pRun->del(blockOffset, len))
							pRun->calcWidths(&pBL->m_gbCharWidths);

						if (pRun->getLength() == 0)
						{
							// remove empty runs from their line
							fp_Line* pLine = pRun->getLine();
							UT_ASSERT(pLine);

							pLine->removeRun(pRun);

							fp_Run* pNuke = pRun;
							fp_Run* pPrev = pNuke->getPrev();

							// sneak in our iterator here  :-)
							pRun = pNuke->getNext();

							// detach from run sequence
							if (pRun)
								pRun->m_pPrev = pPrev;

							if (pPrev)
								pPrev->m_pNext = pRun;

							if (pBL->m_pFirstRun == pNuke)
								pBL->m_pFirstRun = pRun;
							
							// delete it
							delete pNuke;
						}
						else
						{
							pRun = pRun->getNext();
						}
					}

					pBL->format();
					pBL->draw(m_pLayout->getGraphics());

					FV_View* pView = m_pLayout->m_pView;
					if (pView)
						pView->_setPoint(pcr->getPosition());
				}
				return UT_TRUE;
					
			case PTX_Section:
			case PTX_ColumnSet:
			case PTX_Column:
			default:
				UT_ASSERT((0));
				return UT_FALSE;
			}
		}
		break;

	case PX_ChangeRecord::PXT_ChangeSpan:
		{
			const PX_ChangeRecord_SpanChange * pcrsc = static_cast<const PX_ChangeRecord_SpanChange *>(pcr);

			fl_Layout * pL = (fl_Layout *)sfh;
			switch (pL->getType())
			{
			case PTX_Block:
				{
					fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
					PT_DocPosition docPosBlock = m_pDoc->getStruxPosition(pBL->m_sdh);
					PT_BlockOffset blockOffset = (pcr->getPosition() - docPosBlock);
					UT_uint32 len = pcrsc->getLength();

					if (len > 0)
					{
						/*
							The idea here is to invalidate the charwidths for 
							the entire span whose formatting has changed.
							
							We may need to split runs at one or both ends.
						*/
						fp_Run* pRun = pBL->m_pFirstRun;
						while (pRun)
						{
							UT_uint32 iWhere = pRun->containsOffset(blockOffset+len);
							if ((iWhere == FP_RUN_INSIDE) && 
								((blockOffset+len) > pRun->m_iOffsetFirst))
							{
								// split at right end of span
								pRun->split(blockOffset+len);
							}

							iWhere = pRun->containsOffset(blockOffset);
							if ((iWhere == FP_RUN_INSIDE) && 
								(blockOffset > pRun->m_iOffsetFirst))
							{
								// split at left end of span
								pRun->split(blockOffset);
							}

							if ((pRun->m_iOffsetFirst >= blockOffset) && 
								(pRun->m_iOffsetFirst < blockOffset + len))
							{
								pRun->clearScreen();
								pRun->lookupProperties();
								pRun->calcWidths(&pBL->m_gbCharWidths);
							}

							pRun = pRun->getNext();
						}

						pBL->format();
						pBL->draw(m_pLayout->getGraphics());

						// in case anything else moved
						m_pLayout->reformat();
					}
					else
					{
						/*
							This is just a temporary change at the insertion 
							point.  It won't take effect unless something's 
							typed. 
						*/
						FV_View* pView = m_pLayout->m_pView;
						if (pView)
						{
							UT_ASSERT(pView->_isSelectionEmpty());
							pView->_setPoint(pcr->getPosition());
							pView->_setPointAP(pcr->getIndexAP());
						}
					}
				}
				return UT_TRUE;
					
			case PTX_Section:
			case PTX_ColumnSet:
			case PTX_Column:
			default:
				UT_ASSERT((0));
				return UT_FALSE;
			}
		}
		break;
		
	case PX_ChangeRecord::PXT_DeleteStrux:
		{
			const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

			switch (pcrx->getStruxType())
			{
			case PTX_Section:
			case PTX_ColumnSet:
			case PTX_Column:
				UT_ASSERT(UT_TODO);
				return UT_FALSE;
					
			case PTX_Block:
				{
					fl_Layout * pL = (fl_Layout *)sfh;
					switch (pL->getType())
					{
					case PTX_Block:
						{
							fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
							fl_BlockLayout*	pPrevBL = pBL->m_pPrev;
							if (!pPrevBL)
							{
								UT_DEBUGMSG(("no prior BlockLayout"));
								return UT_FALSE;
							}

							/*
								The idea here is to append the runs of the deleted block,
								if any, at the end of the previous block.
							*/
							if (pBL->m_pFirstRun)
							{
								// figure out where the merge point is
								fp_Run * pRun = pPrevBL->m_pFirstRun;
								fp_Run * pLastRun = NULL;
								UT_uint32 offset = 0;

								while (pRun)
								{
									pLastRun = pRun;
									offset += pRun->m_iLen;
									pRun = pRun->getNext();
								}

								// link them together
								if (pLastRun)
								{
									pLastRun->setNext(pBL->m_pFirstRun);
									pBL->m_pFirstRun->setPrev(pLastRun);
								}
								else
								{
									pPrevBL->m_pFirstRun = pBL->m_pFirstRun;
								}

								// tell all the new runs where they live
								pRun = pBL->m_pFirstRun;
								while (pRun)
								{
									pRun->m_iOffsetFirst += offset;
									pRun->m_pBL = pPrevBL;

									// detach from their line
									fp_Line* pLine = pRun->getLine();
									UT_ASSERT(pLine);
									
									pLine->removeRun(pRun);
									
									pRun = pRun->getNext();
								}

								// merge charwidths
								UT_uint32 lenNew = pBL->m_gbCharWidths.getLength();

								pPrevBL->m_gbCharWidths.ins(offset, pBL->m_gbCharWidths.getPointer(0), lenNew);

								// runs are no longer attached to this block
								pBL->m_pFirstRun = NULL;
							}

							// get rid of everything else about the block
							pBL->_purgeLayout(UT_TRUE);

							pPrevBL->m_pNext = pBL->m_pNext;
							
							if (pBL->m_pNext)
								pBL->m_pNext->m_pPrev = pPrevBL;

							fl_SectionLayout* pSL = pBL->m_pSectionLayout;
							UT_ASSERT(pSL);
							pSL->removeBlock(pBL);
							delete pBL;

							// update the display
							pPrevBL->format();
							pPrevBL->draw(m_pLayout->getGraphics());

							FV_View* pView = m_pLayout->m_pView;
							if (pView)
								pView->_setPoint(pcr->getPosition());
						}
						return UT_TRUE;
							
					case PTX_Section:
					case PTX_ColumnSet:
					case PTX_Column:
					default:
						UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
						return UT_FALSE;
					}
				}
				break;

			default:
				UT_ASSERT(0);
				return UT_FALSE;
			}
		}
		break;
		
					
	case PX_ChangeRecord::PXT_ChangeStrux:
		{
			const PX_ChangeRecord_StruxChange * pcrxc = static_cast<const PX_ChangeRecord_StruxChange *> (pcr);

			fl_Layout * pL = (fl_Layout *)sfh;

			UT_ASSERT(pL->getAttrPropIndex() == pcrxc->getOldIndexAP());
			UT_ASSERT(pL->getAttrPropIndex() != pcr->getIndexAP());

			switch (pL->getType())
			{
			case PTX_Section:
			case PTX_ColumnSet:
			case PTX_Column:
				pL->setAttrPropIndex(pcr->getIndexAP());
				UT_ASSERT(UT_TODO);
				return UT_FALSE;
					
			case PTX_Block:
				{
					fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);

					// erase the old version
					pBL->clearScreen(m_pLayout->getGraphics());

					pL->setAttrPropIndex(pcr->getIndexAP());

					// TODO: may want to figure out the specific change and do less work
					pBL->format();
					pBL->draw(m_pLayout->getGraphics());

					// in case anything else moved
					m_pLayout->reformat();
				}
				return UT_TRUE;
					
			default:
				UT_ASSERT(0);
				return UT_FALSE;
			}
		}
		break;


	case PX_ChangeRecord::PXT_InsertStrux:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return UT_FALSE;

	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}

	return UT_TRUE;
}

UT_Bool fl_DocListener::insertStrux(PL_StruxFmtHandle sfh,
									const PX_ChangeRecord * pcr,
									PL_StruxDocHandle sdh,
									PL_StruxFmtHandle * psfh)
{
	UT_DEBUGMSG(("fl_DocListener::insertStrux\n"));
	pcr->dump();

	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

	switch (pcrx->getStruxType())
	{
	case PTX_Section:
	case PTX_ColumnSet:
	case PTX_Column:
		UT_ASSERT(UT_TODO);
		return UT_FALSE;
			
	case PTX_Block:
		{
			fl_Layout * pL = (fl_Layout *)sfh;
			switch (pL->getType())
			{
			case PTX_Block:
				{
					fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
					fl_SectionLayout* pSL = pBL->m_pSectionLayout;
					UT_ASSERT(pSL);
					fl_BlockLayout*	pNewBL = pSL->insertBlock(sdh, pBL);
					if (!pNewBL)
					{
						UT_DEBUGMSG(("no memory for BlockLayout"));
						return UT_FALSE;
					}

					pNewBL->setAttrPropIndex(pcr->getIndexAP());

					*psfh = (PL_StruxFmtHandle)pNewBL;

					/*
						The idea here is to divide the runs of the existing block 
						into two equivalence classes.  This may involve 
						splitting an existing run.  

						All runs and lines remaining in the existing block are
						fine, although the last run should be redrawn.

						All runs in the new block need their offsets fixed, and 
						that entire block needs to be formatted from scratch. 
					*/

					// figure out where the breakpoint is
					PT_DocPosition docPosBlock = m_pDoc->getStruxPosition(pBL->m_sdh);
					PT_BlockOffset blockOffset = (pcr->getPosition() - docPosBlock);

					fp_Run* pRun = pBL->m_pFirstRun;
					while (pRun)
					{			
						UT_uint32 iWhere = pRun->containsOffset(blockOffset);

						if (iWhere == FP_RUN_INSIDE)
						{
							if ((blockOffset > pRun->m_iOffsetFirst) ||
								(blockOffset == 0))
							{
								// split here
								pRun->split(blockOffset);
							}
							break;
						}
						else if (iWhere == FP_RUN_JUSTAFTER)
						{
							// no split needed
							break;
						}
						
						pRun = pRun->getNext();
					}

					fp_Run* pFirstNewRun = NULL;
					if (pRun)
					{
						pFirstNewRun = pRun->getNext();

						// break run sequence
						pRun->m_pNext = NULL;
						if (pFirstNewRun)
							pFirstNewRun->m_pPrev = NULL;
					}
					else if (blockOffset == 0)
					{
						// everything goes in new block
						pFirstNewRun = pBL->m_pFirstRun;
					}

					// explicitly truncate rest of this block's layout
					pBL->truncateLayout(pFirstNewRun);

					// split charwidths across the two blocks
					UT_uint32 lenNew = pBL->m_gbCharWidths.getLength() - blockOffset;

					if (lenNew > 0)
					{
						// NOTE: we do the length check on the outside for speed
						pNewBL->m_gbCharWidths.ins(0, pBL->m_gbCharWidths.getPointer(blockOffset), lenNew);
						pBL->m_gbCharWidths.truncate(blockOffset);
					}

					// move remaining runs to new block
					pNewBL->m_pFirstRun = pFirstNewRun;

					pRun = pFirstNewRun;
					while (pRun)
					{
						pRun->m_iOffsetFirst -= blockOffset;
						pRun->m_pBL = pNewBL;
						
						pRun = pRun->getNext();
					}

					// update the display
					pBL->reformat();
					pBL->draw(m_pLayout->getGraphics());

					pNewBL->format();
					pNewBL->draw(m_pLayout->getGraphics());
				}
				return UT_TRUE;
					
			case PTX_Section:
			case PTX_ColumnSet:
			case PTX_Column:
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
				return UT_FALSE;
			}
		}
		break;
			
	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}

	return UT_TRUE;
}

