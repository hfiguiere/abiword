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
#include "pt_Types.h"
#include "px_ChangeRecord.h"
#include "px_ChangeRecord_Object.h"
#include "px_ChangeRecord_ObjectChange.h"
#include "px_ChangeRecord_Span.h"
#include "px_ChangeRecord_SpanChange.h"
#include "px_ChangeRecord_Strux.h"
#include "px_ChangeRecord_StruxChange.h"
#include "px_ChangeRecord_Glob.h"
#include "px_ChangeRecord_TempSpanFmt.h"
#include "fv_View.h"
#include "fl_DocListener.h"
#include "fl_DocLayout.h"
#include "fl_SectionLayout.h"
#include "fl_BlockLayout.h"
#include "fp_Line.h"
#include "fp_Run.h"
#include "pd_Document.h"
#include "pp_AttrProp.h"


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

	switch (pcr->getType())
	{
	case PX_ChangeRecord::PXT_InsertSpan:
		{
			const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

			fl_Layout * pL = (fl_Layout *)sfh;
			UT_ASSERT(pL->getType() == PTX_Block);
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			PT_BlockOffset blockOffset = (pcr->getPosition() - pBL->getPosition());
			UT_uint32 len = pcrs->getLength();
			return pBL->doclistener_populateSpan(blockOffset, len);
		}

	case PX_ChangeRecord::PXT_InsertObject:
		{
			const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *>(pcr);

			fl_Layout * pL = (fl_Layout *)sfh;
			UT_ASSERT(pL->getType() == PTX_Block);
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			PT_BlockOffset blockOffset = (pcr->getPosition() - pBL->getPosition());

			return pBL->doclistener_populateObject(blockOffset,pcro);
		}

	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}
}

UT_Bool fl_DocListener::populateStrux(PL_StruxDocHandle sdh,
									  const PX_ChangeRecord * pcr,
									  PL_StruxFmtHandle * psfh)
{
	UT_ASSERT(m_pLayout);
	UT_DEBUGMSG(("fl_DocListener::populateStrux\n"));

	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

	switch (pcrx->getStruxType())
	{
	case PTX_Section:
		{
			// append a SectionLayout to this DocLayout
			fl_SectionLayout* pSL = new fl_SectionLayout(m_pLayout, sdh, pcr->getIndexAP());
			if (!pSL)
			{
				UT_DEBUGMSG(("no memory for SectionLayout"));
				return UT_FALSE;
			}
			m_pLayout->m_vecSectionLayouts.addItem(pSL);

			*psfh = (PL_StruxFmtHandle)pSL;
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
			fl_BlockLayout*	pBL = pSL->appendBlock(sdh, pcr->getIndexAP());
			if (!pBL)
			{
				UT_DEBUGMSG(("no memory for BlockLayout"));
				return UT_FALSE;
			}

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
			return UT_FALSE;
				
		case PX_ChangeRecord_Glob::PXF_MultiStepStart:	// TODO add code to inhibit screen updates
		case PX_ChangeRecord_Glob::PXF_MultiStepEnd:	// TODO until we get the end.
			return UT_TRUE;
				
		case PX_ChangeRecord_Glob::PXF_UserAtomicStart:	// TODO decide what (if anything) we need
		case PX_ChangeRecord_Glob::PXF_UserAtomicEnd:	// TODO to do here.
			return UT_TRUE;
		}
	}
			
	case PX_ChangeRecord::PXT_InsertSpan:
	{
		const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		return pBL->doclistener_insertSpan(pcrs);
	}

	case PX_ChangeRecord::PXT_DeleteSpan:
	{
		const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		return pBL->doclistener_deleteSpan(pcrs);
	}

	case PX_ChangeRecord::PXT_ChangeSpan:
	{
		const PX_ChangeRecord_SpanChange * pcrsc = static_cast<const PX_ChangeRecord_SpanChange *>(pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		return pBL->doclistener_changeSpan(pcrsc);
	}

	case PX_ChangeRecord::PXT_TempSpanFmt:
	{
		const PX_ChangeRecord_TempSpanFmt * pcrTSF = static_cast<const PX_ChangeRecord_TempSpanFmt *>(pcr);
		if (pcrTSF->getEnabled())
		{
			/*
			  This is just a temporary change at the insertion 
			  point.  It won't take effect unless something's 
			  typed -- but it will cause the toolbars and etc.
			  to be updated.
			*/

			FV_View* pView = m_pLayout->m_pView;
			if (pView)
			{
				UT_ASSERT(pView->isSelectionEmpty());
				pView->_setPoint(pcrTSF->getPosition());
				pView->_setPointAP(pcrTSF->getIndexAP());
				pView->notifyListeners(AV_CHG_TYPING | AV_CHG_FMTCHAR);
			}
		}
		else
		{
			// we have been asked to turn off the temporary change at the
			// insertion point.  we need to update any toolbars.

			FV_View* pView = m_pLayout->m_pView;
			if (pView)
			{
				UT_ASSERT(pView->isSelectionEmpty());
				// TODO decide if we need to call "pView->_setPoint(pcrTSF->getPosition());"
				// TODO and if so, add AV_CHG_TYPING to the following notifyListeners().
				pView->_clearPointAP(UT_FALSE);
				pView->notifyListeners(AV_CHG_FMTCHAR);
			}
		}

		return UT_TRUE;
	}

	case PX_ChangeRecord::PXT_DeleteStrux:
	{
		const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

		switch (pcrx->getStruxType())
		{
		case PTX_Section:
			UT_ASSERT(UT_TODO);
			return UT_FALSE;
					
		case PTX_Block:
		{
			fl_Layout * pL = (fl_Layout *)sfh;
			UT_ASSERT(pL->getType() == PTX_Block);
			fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
			return pBL->doclistener_deleteStrux(pcrx);
		}

		default:
			UT_ASSERT(0);
			return UT_FALSE;
		}
	}
					
	case PX_ChangeRecord::PXT_ChangeStrux:
	{
		const PX_ChangeRecord_StruxChange * pcrxc = static_cast<const PX_ChangeRecord_StruxChange *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;

		// TODO getOldIndexAP() is only intended for use by the document.
		// TODO this assert is probably wrong. --- BUT EVERYTIME IT HAS
		// TODO GONE OFF, I'VE FOUND A BUG, SO MAYBE WE SHOULD KEEP IT :-)
		UT_ASSERT(pL->getAttrPropIndex() == pcrxc->getOldIndexAP());
		UT_ASSERT(pL->getAttrPropIndex() != pcr->getIndexAP());

		switch (pL->getType())
		{
		case PTX_Section:
			pL->setAttrPropIndex(pcr->getIndexAP());
			UT_ASSERT(UT_TODO);
			return UT_FALSE;
					
		case PTX_Block:
			{
				fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
				return pBL->doclistener_changeStrux(pcrxc);
			}
					
		default:
			UT_ASSERT(0);
			return UT_FALSE;
		}
	}

	case PX_ChangeRecord::PXT_InsertStrux:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return UT_FALSE;

	case PX_ChangeRecord::PXT_InsertObject:
	{
		const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		return pBL->doclistener_insertObject(pcro);
	}
	case PX_ChangeRecord::PXT_DeleteObject:
	{
		const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		return pBL->doclistener_deleteObject(pcro);
	}

	case PX_ChangeRecord::PXT_ChangeObject:
	{
		const PX_ChangeRecord_ObjectChange * pcroc = static_cast<const PX_ChangeRecord_ObjectChange *> (pcr);

		fl_Layout * pL = (fl_Layout *)sfh;
		UT_ASSERT(pL->getType() == PTX_Block);
		fl_BlockLayout * pBL = static_cast<fl_BlockLayout *>(pL);
		return pBL->doclistener_changeObject(pcroc);
	}
		
	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}
}

UT_Bool fl_DocListener::insertStrux(PL_StruxFmtHandle sfh,
									const PX_ChangeRecord * pcr,
									PL_StruxDocHandle sdh,
									PL_StruxFmtHandle * psfh)
{
	UT_DEBUGMSG(("fl_DocListener::insertStrux\n"));

	// TODO coordinate screen updating with the MultiStepStart/End noted in the previous function.
	
	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);

	switch (pcrx->getStruxType())
	{
	case PTX_Section:
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
					fl_BlockLayout * pNewBL = NULL;
					UT_Bool bResult = pBL->doclistener_insertStrux(pcrx,sdh,&pNewBL);
					*psfh = (PL_StruxFmtHandle)pNewBL;
					return bResult;
				}
				
			case PTX_Section:
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

