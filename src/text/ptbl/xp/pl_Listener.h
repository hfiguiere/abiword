
#ifndef PL_LISTENER_H
#define PL_LISTENER_H

#include "ut_types.h"
#include "pt_types.h"

// PL_Listener -- A layout registers a listener with the
//                PD_Document in order to be notified of
//                any changes to the document as they
//                occur.  The document will notify each
//                registered listener (in an undefined
//                order).  When the listener registers,
//                it is provided an ID which may be used
//                later to refer to it.

class PL_Listener
{
public:
	virtual ~PL_Listener();

	// when a listener is installed, the document calls the listener
	// for each fragment in the document.  this allows the layout to
	// fully populate its representation of the formatting/layout of
	// the document.
	//
	// when the document changes (due to editing), the document
	// will call each listener and notify them.  the (fmt) structure
	// handle passed represents the either the structure being
	// modified or the containing structure block.  if the change
	// is an insertStrux, the listener can return a handle for the
	// new structure.
	//
	// the change notification occurs after the document has
	// been updated and in a stable state.  (for an editing operation
	// that breaks down into multiple change records, the listeners
	// will be called after each step.)
	//
	// sdh represents a handle to either the structure or the
	// containing structure block (opaque document instance data).
	//
	// pcr contains a change record to indicate what was done to
	// the document (in the case of editing).  pcr contains a faked-up
	// change record describing a fragment of the document (when part
	// of the listener installation sequence).
	//
	// sfh,psfh refer to a layout handle (opaque layout instance data)
	// to correspond with sdh.
	//
	// insertStrux() is a special form of change() which allows
	// instance data to be exchanged for a new strux.  in this case,
	// sfh refers to the strux preceeding the new one.

	virtual UT_Bool		populate(PL_StruxDocHandle sdh,
								 PX_ChangeRecord * pcr,
								 PL_StruxFmtHandle * psfh) = 0;

	virtual UT_Bool		change(PL_StruxFmtHandle sfh,
							   PX_ChangeRecord * pcr) = 0;

	virtual UT_Bool		insertStrux(PL_StruxFmtHandle sfh,
									PX_ChangeRecord * pcr,
									PL_StruxDocHandle sdh,
									PL_StruxFmtHandle * psfh) = 0;
};
	
#endif /* PL_LISTENER_H */
