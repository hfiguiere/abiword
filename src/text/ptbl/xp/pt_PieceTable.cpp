
#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_growbuf.h"
#include "pt_PieceTable.h"
#include "pf_Frag.h"
#include "pf_Frag_Strux.h"
#include "pf_Frag_Strux_Block.h"
#include "pf_Frag_Strux_Column.h"
#include "pf_Frag_Strux_ColumnSet.h"
#include "pf_Frag_Strux_Section.h"
#include "pf_Frag_Text.h"
#include "pf_Fragments.h"
#include "px_ChangeRecord.h"
#include "px_ChangeRecord_Span.h"


#define NrElements(a)		(sizeof(a)/sizeof(a[0]))

/*****************************************************************/
/*****************************************************************/

pt_PieceTable::pt_PieceTable(PD_Document * pDocument)
{
	setPieceTableState(PTS_Invalid);
	m_vsIndex = 0;
	m_pDocument = pDocument;
}

pt_PieceTable::~pt_PieceTable()
{
}

void pt_PieceTable::setPieceTableState(PTState pts)
{
	UT_uint32 k;
	PT_AttrPropIndex ndx;
	
	m_pts = pts;

	switch (m_pts)
	{
	case PTS_Loading:
		for (k=0; k<NrElements(m_vs); k++)					// create a default A/P
			if (!m_vs[k].m_tableAttrProp.createAP(&ndx))	// as entry zero in the
				return;										// AP table in each VarSet.
		loading.m_indexCurrentInlineAP = 0;
		break;

	case PTS_Editing:
		m_vsIndex = 1;
		break;

	case PTS_Invalid:
		break;
	}
}

UT_GrowBuf * pt_PieceTable::getBuffer(PT_VarSetIndex vsIndex)
{
	UT_ASSERT(vsIndex < NrElements(m_vs));
	
	return &m_vs[vsIndex].m_buffer;
}


UT_Bool pt_PieceTable::insertSpan(PT_DocPosition dpos,
								  UT_Bool bLeftSide,
								  UT_UCSChar * p,
								  UT_uint32 length)
{
	// insert character data into the document at the given position.

	UT_ASSERT(m_pts==PTS_Editing);

	// append the text data to the end of the current buffer.

	VarSet * pvs = &m_vs[m_vsIndex];
	pt_BufPosition bufOffset = pvs->m_buffer.getLength();
	if (!pvs->m_buffer.ins(bufOffset,p,length))
		return UT_FALSE;

	pf_Frag_Strux * pfs = NULL;
	pf_Frag_Text * pft = NULL;
	PT_BlockOffset fragOffset = 0;
	if (!getTextFragFromPosition(dpos,bLeftSide,&pfs,&pft,&fragOffset))
		return UT_FALSE;

	// see if the character data is contiguous with the
	// fragment we found (very likely during data entry).

	UT_uint32 fragLen = pft->getLength();
	
	if (   (fragOffset == fragLen)						// at right-end of fragment
		&& (m_vsIndex == pft->getVSindex())				// in the same buffer
		&& (pft->getOffset()+fragLen == bufOffset))		// contiguous in buffer
	{
		// new text is contiguous, we just update the length of this fragment.

		pft->changeLength(fragLen+length);
	}
	else
	{
		// new text is not contiguous, we need to insert one or two new text
		// fragment(s) into the list.  first we construct a new text fragment
		// for the data that we inserted.

		pf_Frag_Text * pftNew = new pf_Frag_Text(this,m_vsIndex,bufOffset,length,pft->getIndexAP());
		if (!pftNew)
			return UT_FALSE;

		
		if (fragOffset == 0)
		{
			// if change is at the beginning of the fragment, we insert a
			// single new text fragment before the one we found.

			m_fragments.insertFrag(pft->getPrev(),pftNew);
		}
		else if (fragLen==fragOffset)
		{
			// if the change is after this fragment, we insert a single
			// new text fragment after the one we found.

			m_fragments.insertFrag(pft,pftNew);
		}
		else
		{
			// if the change is in the middle of the fragment, we construct
			// a second new text fragment for the portion after the insert.

			pf_Frag_Text * pftTail = new pf_Frag_Text(this,pft->getVSindex(),pft->getOffset()+fragOffset,
													  fragLen-fragOffset,pft->getIndexAP());
			if (!pftTail)
				return UT_FALSE;
			
			pft->changeLength(fragOffset);
			m_fragments.insertFrag(pft,pftNew);
			m_fragments.insertFrag(pftNew,pftTail);
		}
	}

	// now we notify all listeners who have subscribed.
	
	PX_ChangeRecord_Span * pcr = new PX_ChangeRecord_Span(PX_ChangeRecord::PXT_InsertSpan,
														  UT_FALSE,UT_FALSE,dpos,m_vsIndex,
														  bLeftSide,pft->getIndexAP(),
														  bufOffset,length);
	if (!pcr)
		return UT_FALSE;
	m_pDocument->notifyListeners(pfs,pcr);
	
	return UT_TRUE;
}

UT_Bool pt_PieceTable::deleteSpan(PT_DocPosition dpos,
								  UT_uint32 length)
{
	return UT_TRUE;
}

UT_Bool pt_PieceTable::insertFmt(PT_DocPosition dpos1,
								 PT_DocPosition dpos2,
								 const XML_Char ** attributes,
								 const XML_Char ** properties)
{
	return UT_TRUE;
}

UT_Bool pt_PieceTable::deleteFmt(PT_DocPosition dpos1,
								 PT_DocPosition dpos2,
								 const XML_Char ** attributes,
								 const XML_Char ** properties)
{
	return UT_TRUE;
}

UT_Bool pt_PieceTable::insertStrux(PT_DocPosition dpos,
								   PTStruxType pts,
								   const XML_Char ** attributes,
								   const XML_Char ** properties)
{
	return UT_TRUE;
}

UT_Bool pt_PieceTable::deleteStrux(PT_DocPosition dpos)
{
	return UT_TRUE;
}

UT_Bool pt_PieceTable::appendStrux(PTStruxType pts, const XML_Char ** attributes)
{
	// can only be used while loading the document
	UT_ASSERT(m_pts==PTS_Loading);

	// create a new structure fragment at the current end of the document.
	
	PT_AttrPropIndex indexAP = 0;

	if (attributes && *attributes)
		if (!m_vs[m_vsIndex].m_tableAttrProp.createAP(attributes,NULL,&indexAP))
			return UT_FALSE;

	pf_Frag_Strux * pfs = 0;
	switch (pts)
	{
	case PTX_Section:
		pfs = new pf_Frag_Strux_Section(this,m_vsIndex,indexAP);
		break;
		
	case PTX_ColumnSet:
		pfs = new pf_Frag_Strux_ColumnSet(this,m_vsIndex,indexAP);
		break;
		
	case PTX_Column:
		pfs = new pf_Frag_Strux_Column(this,m_vsIndex,indexAP);
		break;
		
	case PTX_Block:
		pfs = new pf_Frag_Strux_Block(this,m_vsIndex,indexAP);
		break;
	}
	if (!pfs)
	{
		UT_DEBUGMSG(("Could not create structure fragment.\n"));
		// we forget about the AP that we created
		return UT_FALSE;
	}

	m_fragments.appendFrag(pfs);
	return UT_TRUE;
}

UT_Bool pt_PieceTable::appendFmt(const XML_Char ** attributes)
{
	// can only be used while loading the document
	UT_ASSERT(m_pts==PTS_Loading);

	// create a new Attribute/Property structure in the table
	// and set the current index to it.  the next span of text
	// (in this block) that comes in will then be set to these
	// attributes/properties.  becase we are loading, we do not
	// create a Fragment or a ChangeRecord.  (Formatting changes
	// are implicit at this point in time.)

	loading.m_indexCurrentInlineAP = 0;
	if (attributes && *attributes)
		if (!m_vs[m_vsIndex].m_tableAttrProp.createAP(attributes,NULL,
													  &loading.m_indexCurrentInlineAP))
			return UT_FALSE;

	return UT_TRUE;
}

UT_Bool pt_PieceTable::appendFmt(const UT_Vector * pVecAttributes)
{
	// can only be used while loading the document
	UT_ASSERT(m_pts==PTS_Loading);

	loading.m_indexCurrentInlineAP = 0;
	if (pVecAttributes && (pVecAttributes->getItemCount() > 0))
		if (!m_vs[m_vsIndex].m_tableAttrProp.createAP(pVecAttributes,
													  &loading.m_indexCurrentInlineAP))
			return UT_FALSE;

	return UT_TRUE;
}

UT_Bool pt_PieceTable::appendSpan(UT_UCSChar * pbuf, UT_uint32 length)
{
	// can only be used while loading the document
	UT_ASSERT(m_pts==PTS_Loading);

	// create a new fragment for this text span.
	// append the text data to the end of the buffer.
	// set the formatting Attributes/Properties to that
	// of the last fmt set in this paragraph.
	// becauase we are loading, we do not create change
	// records or any of the other stuff that an insertSpan
	// would do.

	VarSet * pvs = &m_vs[m_vsIndex];
	pt_BufPosition offset = pvs->m_buffer.getLength();
	if (!pvs->m_buffer.ins(offset,pbuf,length))
		return UT_FALSE;
	
	pf_Frag_Text * pft = new pf_Frag_Text(this,m_vsIndex,offset,length,
										  loading.m_indexCurrentInlineAP);
	if (!pft)
		return UT_FALSE;

	m_fragments.appendFrag(pft);
	return UT_TRUE;
}

UT_Bool pt_PieceTable::addListener(PL_Listener * pListener,
								   PL_ListenerId listenerId)
{
	// walk document and for each fragment, send a notification
	// to each layout.

	PL_StruxFmtHandle sfh = 0;
	
	for (pf_Frag * pf = m_fragments.getFirst(); (pf); pf=pf->getNext())
	{
		switch (pf->getType())
		{
		case pf_Frag::PFT_Text:
			{
				pf_Frag_Text * pft = static_cast<pf_Frag_Text *> (pf);
				PX_ChangeRecord * pcr = 0;
				UT_Bool bStatus = (   pft->createSpecialChangeRecord(&pcr)
								   && pListener->populate(sfh,pcr));
				if (pcr)
					delete pcr;
				if (!bStatus)
					return UT_FALSE;
			}
			break;
			
		case pf_Frag::PFT_Strux:
			{
				pf_Frag_Strux * pfs = static_cast<pf_Frag_Strux *> (pf);
				PL_StruxDocHandle sdh = (PL_StruxDocHandle)pf;
				sfh = 0;
				PX_ChangeRecord * pcr = 0;
				UT_Bool bStatus = (   pfs->createSpecialChangeRecord(&pcr)
								   && pListener->populateStrux(sdh,pcr,&sfh)
								   && pfs->setFmtHandle(listenerId,sfh));
				if (pcr)
					delete pcr;
				if (!bStatus)
					return UT_FALSE;
			}
			break;
			
		default:
			UT_ASSERT(0);
			return UT_FALSE;
		}
	}

	return UT_TRUE;
}

UT_Bool pt_PieceTable::getAttrProp(PT_VarSetIndex vsIndex, PT_AttrPropIndex indexAP,
								   const PP_AttrProp ** ppAP) const
{
	UT_ASSERT(vsIndex < NrElements(m_vs));
	UT_ASSERT(ppAP);

	const PP_AttrProp * pAP = m_vs[vsIndex].m_tableAttrProp.getAP(indexAP);
	if (!pAP)
		return UT_FALSE;

	*ppAP = pAP;
	return UT_TRUE;
}

UT_Bool pt_PieceTable::getSpanAttrProp(PL_StruxDocHandle sdh, UT_uint32 offset,
									   const PP_AttrProp ** ppAP) const
{
	UT_ASSERT(sdh);
	UT_ASSERT(ppAP);

	pf_Frag * pf = (pf_Frag *)sdh;
	UT_ASSERT(pf->getType() == pf_Frag::PFT_Strux);
	pf_Frag_Strux * pfsBlock = static_cast<pf_Frag_Strux *> (pf);
	UT_ASSERT(pfsBlock->getStruxType() == PTX_Block);

	UT_uint32 cumOffset = 0;
	for (pf_Frag * pfTemp=pfsBlock->getNext(); (pfTemp); pfTemp=pfTemp->getNext())
	{
		if (pfTemp->getType() != pf_Frag::PFT_Text)
			continue;

		pf_Frag_Text * pfText = static_cast<pf_Frag_Text *> (pfTemp);
		
		// TODO consider moving the guts of this loop to pf_Frag_Text.cpp
		
		if ((offset >= cumOffset) && (offset < cumOffset+pfText->getLength()))
		{
			const PP_AttrProp * pAP = m_vs[pfText->getVSindex()].m_tableAttrProp.getAP(pfText->getIndexAP());
			if (!pAP)
				return UT_FALSE;

			*ppAP = pAP;
			return UT_TRUE;
		}

		cumOffset += pfText->getLength();
	}
	return UT_FALSE;
}

const UT_UCSChar * pt_PieceTable::getPointer(PT_VarSetIndex vsIndex, pt_BufPosition bufPos) const
{
	UT_ASSERT(vsIndex < NrElements(m_vs));
	
	return m_vs[vsIndex].m_buffer.getPointer(bufPos);
}

UT_Bool pt_PieceTable::getSpanPtr(PL_StruxDocHandle sdh, UT_uint32 offset,
								  const UT_UCSChar ** ppSpan, UT_uint32 * pLength) const
{
	pf_Frag * pf = (pf_Frag *)sdh;
	UT_ASSERT(pf->getType() == pf_Frag::PFT_Strux);
	pf_Frag_Strux * pfsBlock = static_cast<pf_Frag_Strux *> (pf);
	UT_ASSERT(pfsBlock->getStruxType() == PTX_Block);

	UT_uint32 cumOffset = 0;
	for (pf_Frag * pfTemp=pfsBlock->getNext(); (pfTemp); pfTemp=pfTemp->getNext())
	{
		if (pfTemp->getType() != pf_Frag::PFT_Text)
			continue;

		pf_Frag_Text * pfText = static_cast<pf_Frag_Text *> (pfTemp);
		
		// TODO consider moving the guts of this loop to pf_Frag_Text.cpp
		
		if (offset == cumOffset)
		{
			*ppSpan = getPointer(pfText->getVSindex(),pfText->getOffset());
			*pLength = pfText->getLength();
			return UT_TRUE;
		}
		if (offset < cumOffset+pfText->getLength())
		{
			const UT_UCSChar * p = getPointer(pfText->getVSindex(),pfText->getOffset());
			UT_uint32 delta = offset - cumOffset;
			*ppSpan = p + delta;
			*pLength = pfText->getLength() - delta;
			return UT_TRUE;
		}

		cumOffset += pfText->getLength();
	}
	return UT_FALSE;
}

PT_DocPosition pt_PieceTable::getStruxPosition(PL_StruxDocHandle sdh) const
{
	// return absolute document position of the given handle.

	pf_Frag * pfToFind = (pf_Frag *)sdh;

	return getFragPosition(pfToFind);
}

PT_DocPosition pt_PieceTable::getFragPosition(const pf_Frag * pfToFind) const
{
	PT_DocPosition sum = 0;

	for (pf_Frag * pf = m_fragments.getFirst(); (pf); pf=pf->getNext())
	{
		if (pf == pfToFind)
			return sum;
		if (pf->getType() == pf_Frag::PFT_Text)
		{
			pf_Frag_Text * pfText = static_cast<pf_Frag_Text *>(pf);
			sum += pfText->getLength();
		}
	}
	UT_ASSERT(0);
	return 0;
}

UT_Bool pt_PieceTable::getStruxFromPosition(PL_ListenerId listenerId,
											PT_DocPosition docPos,
											PL_StruxFmtHandle * psfh) const
{
	// return the SFH of the last strux immediately prior to
	// the given absolute document position.
	
	PT_DocPosition sum = 0;
	pf_Frag * pfLastStrux = NULL;

	for (pf_Frag * pf = m_fragments.getFirst(); (pf); pf=pf->getNext())
	{
		if (pf->getType() == pf_Frag::PFT_Text)
		{
			pf_Frag_Text * pfText = static_cast<pf_Frag_Text *>(pf);
			sum += pfText->getLength();

			if (sum >= docPos)
				goto FoundIt;
		}
		else if (pf->getType() == pf_Frag::PFT_Strux)
		{
			pfLastStrux = pf;
		}
	}

	// if we fall out of the loop, we didn't have a text node
	// at or around the document position requested.
	// return the last strux in the document.

 FoundIt:

	pf_Frag_Strux * pfs = static_cast<pf_Frag_Strux *> (pfLastStrux);
	*psfh = pfs->getFmtHandle(listenerId);
	return UT_TRUE;
}

UT_Bool pt_PieceTable::getTextFragFromPosition(PT_DocPosition docPos,
											   UT_Bool bLeftSide,
											   pf_Frag_Strux ** ppfs,
											   pf_Frag_Text ** ppft,
											   PT_BlockOffset * pOffset) const
{
	UT_ASSERT(ppft);
	UT_ASSERT(pOffset);
	
	// return the text fragment containing the given position.
	// return the strux containing the text fragment we find.
	// return the offset from the start of the fragment.
	// if the position is between 2 fragments, return the
	// one w/r/t bLeftSide.
	
	PT_DocPosition sum = 0;
	pf_Frag_Strux * pfLastStrux = NULL;
	
	for (pf_Frag * pf = m_fragments.getFirst(); (pf); pf=pf->getNext())
	{
		if (pf->getType() == pf_Frag::PFT_Strux)
			pfLastStrux = static_cast<pf_Frag_Strux *>(pf);
		
		if (pf->getType() != pf_Frag::PFT_Text)
			continue;

		pf_Frag_Text * pfText = static_cast<pf_Frag_Text *>(pf);
		if (docPos == sum)				// if on left-edge of this fragment
		{
			*pOffset = 0;
			*ppft = pfText;
			*ppfs = pfLastStrux;
			return UT_TRUE;
		}
		
		UT_uint32 len = pfText->getLength();

		if (docPos < sum+len)			// if position inside this fragment
		{
			*pOffset = (docPos - sum);
			*ppft = pfText;
			*ppfs = pfLastStrux;
			return UT_TRUE;
		}

		sum += len;
		if ((docPos == sum) && bLeftSide)	// if on right-edge of this fragment 
		{									// (aka the left side of this position).
			*pOffset = len;
			*ppft = pfText;
			*ppfs = pfLastStrux;
			return UT_TRUE;
		}
	}

	// if we fall out of the loop, we didn't have a text node
	// at or around the document position requested.
	// TODO this looks like it should be an error, for now we bail
	// TODO and see if it ever goes off.  later we can just return
	// TODO the last node in the list.
	
	UT_ASSERT(0);
	return UT_FALSE;
}

void pt_PieceTable::dump(FILE * fp) const
{
	fprintf(fp,"  PieceTable: State %d\n",(int)m_pts);
	fprintf(fp,"  PieceTable: vsIndex %d\n",m_vsIndex);
	fprintf(fp,"  PieceTable: Fragments:\n");

	m_fragments.dump(fp);
}
