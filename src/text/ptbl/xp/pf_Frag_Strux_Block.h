
#ifndef PF_FRAG_STRUX_BLOCK_H
#define PF_FRAG_STRUX_BLOCK_H

#include "ut_types.h"
#include "pf_Frag.h"
#include "pf_Frag_Strux.h"

// pf_Frag_Strux_Block represents structure information for a 
// block (paragraph) in the document.

class pf_Frag_Strux_Block : public pf_Frag_Strux
{
public:
	pf_Frag_Strux_Block(pt_PieceTable * pPT,
						PT_AttrPropIndex indexAP);
	virtual ~pf_Frag_Strux_Block();

	virtual void			dump(FILE * fp) const;
};

#endif /* PF_FRAG_STRUX_BLOCK_H */
