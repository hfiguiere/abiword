
#ifndef PX_CHANGERECORD_H
#define PX_CHANGERECORD_H

#include "ut_types.h"

// PX_ChangeRecord describes a change made to the document.
// This description should be sufficient to allow undo to
// work and sufficient to allow the formatter to do a
// partial format and screen update (if appropriate).
// The change record must be free of pointers, since it
// represents what was done to the document -- and not
// how it was done (that is, not what was done to various
// intermediate data structures).  this also lets it be
// cached to disk (for autosave and maybe multi-session
// undo).
//
// PX_ChangeRecord is an abstract base class.
// We use an enum to remember type, rather than use any of
// the run-time stuff.

class PX_ChangeRecord
{
public:
	PX_ChangeRecord();
	virtual ~PX_ChangeRecord();
	
	typedef enum _PXType { PXT_InsertSpan, PXT_DeleteSpan,
						   PXT_InsertFmt, PXT_DeleteFmt,
						   PXT_InsertStrux, PXT_DeleteStrux,
						   PXT_InsertObject, PXT_DeleteObject } PXType;

protected:
	UT_Bool					m_bMultiStepStart;
	UT_Bool					m_bMultiStepEnd;
};

#endif /* PX_CHANGERECORD_H */
