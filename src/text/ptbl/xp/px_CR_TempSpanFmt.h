/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */ 


#ifndef PX_CHANGERECORD_TEMPSPANFMT_H
#define PX_CHANGERECORD_TEMPSPANFMT_H

#include "ut_types.h"
#include "px_ChangeRecord.h"

// PX_ChangeRecord_TempSpanFmt describes a temporary
// span format applied to the document.  this is used
// for toggling on a format style (like hitting the BOLD
// button on the tool bar) without a selection and then
// start typing.
//
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
// m_position contains the absolute document position of
// the text span at the time the change was made.
// m_bufIndex,m_length describe the actual contents
// of the text span.


class PX_ChangeRecord_TempSpanFmt : public PX_ChangeRecord
{
public:
	PX_ChangeRecord_TempSpanFmt(PXType type,
								PT_DocPosition position,
								PT_AttrPropIndex indexAP,
								UT_Bool bEnableTempSpanFmt);

	~PX_ChangeRecord_TempSpanFmt();

	virtual PX_ChangeRecord * reverse(void) const;

	UT_Bool	getEnabled(void) const;

protected:
	UT_Bool m_bEnableTempSpanFmt;
};

#endif /* PX_CHANGERECORD_TEMPSPANFMT_H */
