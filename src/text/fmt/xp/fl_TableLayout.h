/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2002 Martin Sevior <msevior@physics.unimelb.edu.au>
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

#ifndef TABLELAYOUT_H
#define TABLELAYOUT_H

#ifdef FMT_TEST
#include <stdio.h>
#endif

#include "ut_types.h"
#include "ut_vector.h"
#include "pt_Types.h"
#include "fl_Layout.h"
#include "fl_ContainerLayout.h"
#include "fl_SectionLayout.h"
#include "pl_Listener.h"
#include "ut_debugmsg.h"
#include "ut_misc.h" // for UT_RGBColor

enum  FL_TableJustification
{
    FL_TABLE_LEFT,
    FL_TABLE_CENTER,
    FL_TABLE_RIGHT,
    FL_TABLE_FULL
};

class fp_Page;
class FL_DocLayout;
class fl_Layout;
class fl_ContainerLayout;
class fl_BlockLayout;
class fl_SectionLayout;
class fl_DocSectionLayout;
class fl_HdrFtrSectionLayout;
class fl_HdrFtrShadow;
class fb_LineBreaker;
class fp_ShadowContainer;
class fp_Column;
class fp_Run;
class fp_Line;
class fp_Container;
class fp_HdrFtrContainer;
class PD_Document;
class PP_AttrProp;
class PX_ChangeRecord_FmtMark;
class PX_ChangeRecord_FmtMarkChange;
class PX_ChangeRecord_Object;
class PX_ChangeRecord_ObjectChange;
class PX_ChangeRecord_Span;
class PX_ChangeRecord_SpanChange;
class PX_ChangeRecord_Strux;
class PX_ChangeRecord_StruxChange;

class ABI_EXPORT fl_TableLayout : public fl_SectionLayout
{
	friend class fl_DocListener;

public:
	fl_TableLayout(FL_DocLayout* pLayout, PL_StruxDocHandle sdh, PT_AttrPropIndex ap, fl_ContainerLayout * pMyContainerLayout);
	virtual ~fl_TableLayout();

	SectionType     	        getType(void) const { return m_iType; }

	virtual bool		        recalculateFields(UT_uint32 iUpdateCount);
	virtual bool 	            doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc);
	virtual bool				doclistener_deleteStrux(const PX_ChangeRecord_Strux * pcrx);
	virtual bool                bl_doclistener_insertSection(fl_ContainerLayout*,
											  SectionType iType,
											  const PX_ChangeRecord_Strux * pcrx,
											  PL_StruxDocHandle sdh,
											  PL_ListenerId lid,
											  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
																	  PL_ListenerId lid,
																	  PL_StruxFmtHandle sfhNew));


#ifdef FMT_TEST
	virtual void		__dump(FILE * fp) const;
#endif
	virtual void		        format(void);
	virtual void		        updateLayout(void);
	void		                updateTable(void);
	virtual void                collapse(void);
	virtual void                markAllRunsDirty(void);
	virtual fp_Container *      getFirstContainer(void) const;
	virtual fp_Container *      getLastContainer(void) const;
	virtual PT_DocPosition      getPosition(bool bActualBlockPosition = false) const;
	virtual void		        redrawUpdate(void);
	virtual fp_Container*		getNewContainer(fp_Container * pFirstContainer = NULL);

	void                        markForRebuild(void) { m_bNeedsRebuild = true;}
	void                        clearRebuild(void) { m_bNeedsRebuild = false;}
	bool                        needsRebuild(void) const { return m_bNeedsRebuild;}
    void                        markForReformat(void) { m_bNeedsFormat = true;}
    bool                        needsReFormat(void) const { return m_bNeedsFormat;}

	UT_sint32                getLeftOffset(void) const;
#ifndef WITH_PANGO
 UT_sint32                   getLeftOffsetInLayoutUnits(void) const;
#endif
UT_sint32                    getRightOffset(void) const;
#ifndef WITH_PANGO
UT_sint32                    getRightOffsetInLayoutUnits(void) const;
#endif
	UT_sint32                getTopOffset(void) const;
#ifndef WITH_PANGO
 UT_sint32                   getTopOffsetInLayoutUnits(void) const;
#endif
UT_sint32                    getBottomOffset(void) const;
#ifndef WITH_PANGO
UT_sint32                    getBottomOffsetInLayoutUnits(void) const;
#endif

protected:
	virtual void		        _lookupProperties(void);
	void				        _purgeLayout();
private:
	bool                   m_bNeedsFormat;
	bool                   m_bNeedsRebuild;
	FL_TableJustification  m_iJustification;
	UT_sint32              m_iLeftOffset;
	UT_sint32              m_iLeftOffsetLayoutUnits;
	double                 m_dLeftOffsetUserUnits;
	UT_sint32              m_iRightOffset;
	UT_sint32              m_iRightOffsetLayoutUnits;
	double                 m_dRightOffsetUserUnits;
	UT_sint32              m_iTopOffset;
	UT_sint32              m_iTopOffsetLayoutUnits;
	double                 m_dTopOffsetUserUnits;
	UT_sint32              m_iBottomOffset;
	UT_sint32              m_iBottomOffsetLayoutUnits;
	double                 m_dBottomOffsetUserUnits;
	bool                   m_bIsHomogeneous;
	bool                   m_bSameRowOnTopOfPage;
	UT_sint32              m_iRowNumberForTop;
	UT_sint32              m_iNumberOfRows;
	UT_sint32              m_iNumberOfColumns;
	bool                   m_bColumnsPositionedOnPage;
	bool                   m_bRowsPositionedOnPage;
};


class ABI_EXPORT fl_CellLayout : public fl_SectionLayout
{
	friend class fl_DocListener;
public:
	fl_CellLayout(FL_DocLayout* pLayout, PL_StruxDocHandle sdh, PT_AttrPropIndex ap, fl_ContainerLayout * pMyContainerLayout);
	virtual ~fl_CellLayout();


	virtual bool 	doclistener_changeStrux(const PX_ChangeRecord_StruxChange * pcrxc);
	virtual bool    doclistener_deleteStrux(const PX_ChangeRecord_Strux * pcrx);
	virtual bool bl_doclistener_insertSection(fl_ContainerLayout*,
											  SectionType iType,
											  const PX_ChangeRecord_Strux * pcrx,
											  PL_StruxDocHandle sdh,
											  PL_ListenerId lid,
											  void (* pfnBindHandles)(PL_StruxDocHandle sdhNew,
																	  PL_ListenerId lid,
																	  PL_StruxFmtHandle sfhNew));
	virtual void		     format(void);
	virtual void		     updateLayout(void);
	void                     updateCell(void);
	virtual void             collapse(void);
	virtual void             markAllRunsDirty(void);
	virtual fl_SectionLayout *  getSectionLayout(void)  const;
	bool                     recalculateFields(UT_uint32 iUpdateCount);
	virtual void		     redrawUpdate(void);
	virtual fp_Container*	 getNewContainer(fp_Container * pFirstContainer = NULL);

	UT_sint32                getLeftOffset(void) const;
#ifndef WITH_PANGO
 UT_sint32                   getLeftOffsetInLayoutUnits(void) const;
#endif
UT_sint32                    getRightOffset(void) const;
#ifndef WITH_PANGO
UT_sint32                    getRightOffsetInLayoutUnits(void) const;
#endif
	UT_sint32                getTopOffset(void) const;
#ifndef WITH_PANGO
 UT_sint32                   getTopOffsetInLayoutUnits(void) const;
#endif
UT_sint32                    getBottomOffset(void) const;
#ifndef WITH_PANGO
UT_sint32                    getBottomOffsetInLayoutUnits(void) const;
#endif
protected:
	virtual void		     _lookupProperties(void);
	virtual void             _purgeLayout(void);
private:
	bool                   m_bNeedsFormat;
	bool                   m_bNeedsRebuild;
	UT_sint32              m_iLeftOffset;
	UT_sint32              m_iLeftOffsetLayoutUnits;
	double                 m_dLeftOffsetUserUnits;
	UT_sint32              m_iRightOffset;
	UT_sint32              m_iRightOffsetLayoutUnits;
	double                 m_dRightOffsetUserUnits;
	UT_sint32              m_iTopOffset;
	UT_sint32              m_iTopOffsetLayoutUnits;
	double                 m_dTopOffsetUserUnits;
	UT_sint32              m_iBottomOffset;
	UT_sint32              m_iBottomOffsetLayoutUnits;
	double                 m_dBottomOffsetUserUnits;
	UT_sint32              m_iLeftAttach;
	UT_sint32              m_iRightAttach;
	UT_sint32              m_iTopAttach;
	UT_sint32              m_iBotAttach;
	bool                   m_bCellPositionedOnPage;
};

#endif /* TABLELAYOUT_H */








