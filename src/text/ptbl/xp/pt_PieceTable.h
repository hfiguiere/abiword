
#ifndef PT_PIECETABLE_H
#define PT_PIECETABLE_H

#include <stdio.h>
#include "ut_types.h"
#include "ut_growbuf.h"
#include "pt_Types.h"
#include "pp_TableAttrProp.h"
#include "px_ChangeHistory.h"
#include "pf_Fragments.h"
class pf_Frag_Text;
class pf_Frag_Strux;


// pt_PieceTable implements a "Piece Table" as described/suggested
// by .../dev/design/PieceTable.html

class pt_PieceTable
{
public:
	pt_PieceTable(PD_Document * pDocument);
	~pt_PieceTable();

	typedef enum _PTState { PTS_Invalid=-1, PTS_Loading=0, PTS_Editing=1 } PTState;
	void					setPieceTableState(PTState pts);
	UT_GrowBuf *			getBuffer(PT_VarSetIndex vsIndex);

	UT_Bool					insertSpan(PT_DocPosition dpos,
									   UT_Bool bLeftSide,
									   UT_UCSChar * p,
									   UT_uint32 length);
	UT_Bool					deleteSpan(PT_DocPosition dpos,
									   UT_uint32 length);

	UT_Bool					insertFmt(PT_DocPosition dpos1,
									  PT_DocPosition dpos2,
									  const XML_Char ** attributes,
									  const XML_Char ** properties);
	UT_Bool					deleteFmt(PT_DocPosition dpos1,
									  PT_DocPosition dpos2,
									  const XML_Char ** attributes,
									  const XML_Char ** properties);

	UT_Bool					insertStrux(PT_DocPosition dpos,
										PTStruxType pts,
										const XML_Char ** attributes,
										const XML_Char ** properties);
	UT_Bool					deleteStrux(PT_DocPosition dpos);

	// the append- methods are only available while importing
	// the document.

	UT_Bool					appendStrux(PTStruxType pts, const XML_Char ** attributes);
	UT_Bool					appendFmt(const XML_Char ** attributes);
	UT_Bool					appendFmt(const UT_Vector * pVecAttributes);
	UT_Bool					appendSpan(UT_UCSChar * p, UT_uint32 length);

	UT_Bool					addListener(PL_Listener * pListener,
										PL_ListenerId listenerId);
	
	UT_Bool					getAttrProp(PT_VarSetIndex vsIndex, PT_AttrPropIndex indexAP,
										const PP_AttrProp ** ppAP) const;
	UT_Bool					getSpanAttrProp(PL_StruxDocHandle sdh, UT_uint32 offset,
											const PP_AttrProp ** ppAP) const;
	const UT_UCSChar *		getPointer(PT_VarSetIndex vsIndex, pt_BufPosition bufPos) const;
	UT_Bool					getSpanPtr(PL_StruxDocHandle sdh, UT_uint32 offset,
									   const UT_UCSChar ** ppSpan, UT_uint32 * pLength) const;
	PT_DocPosition			getStruxPosition(PL_StruxDocHandle sdh) const;
	PT_DocPosition			getFragPosition(const pf_Frag * pfToFind) const;
	
	UT_Bool					getStruxFromPosition(PL_ListenerId listenerId,
												 PT_DocPosition docPos,
												 PL_StruxFmtHandle * psfh) const;
	UT_Bool					getTextFragFromPosition(PT_DocPosition docPos,
													UT_Bool bLeftSide,
													pf_Frag_Strux ** ppfs,
													pf_Frag_Text ** ppft,
													PT_BlockOffset * pOffset) const;

	// TODO add stuff for objects like in-line images.

	void					dump(FILE * fp) const;
	
protected:
	typedef struct _VarSet
	{
		UT_GrowBuf			m_buffer;
		pp_TableAttrProp	m_tableAttrProp;
	} VarSet;

	// m_pts keeps track of whether we are loading the document
	//       or editing it.  this lets us know conceptually which
	//       VarSet [initial,change] to use.  we seperate this
	//       from m_vsIndex so that we can later do multiple
	//       VarSet's per state -- so that we can better handle
	//       things like Win16 where we might want to limit the
	//       arbitrary growth of an array.
	//
	// m_vsIndex keeps track of the index into m_vs[] that we
	//           are currently writing (appending) to.
	
	PTState					m_pts;
	PT_VarSetIndex			m_vsIndex;		/* vs[] that we are writing to */
	VarSet					m_vs[2];		/* [0] is initial, [1] is change */

	px_ChangeHistory		m_history;
	pf_Fragments			m_fragments;

	struct {
		PT_AttrPropIndex	m_indexCurrentInlineAP;
	} loading;							/* stuff only valid while m_pts==PTS_Loading */

	PD_Document *			m_pDocument; /* back pointer to our document */
};

#endif /* PT_PIECETABLE_H */
