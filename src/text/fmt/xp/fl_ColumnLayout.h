
#ifndef FL_COLUMNLAYOUT_H
#define FL_COLUMNLAYOUT_H

#include "ut_types.h"
#include "pt_Types.h"
class FL_ColumnSetLayout;
class FP_Column;
class PP_AttrProp;


class FL_ColumnLayout
{
public:
	FL_ColumnLayout(FL_ColumnSetLayout * pCSL, PL_StruxDocHandle sdh);
	~FL_ColumnLayout();

	FL_ColumnLayout *		setNext(FL_ColumnLayout * pCL);
	FL_ColumnLayout *		setPrev(FL_ColumnLayout * pCL);
	FL_ColumnLayout *		getNext(void) const;
	FL_ColumnLayout *		getPrev(void) const;

	void					setColumnSetLayout(FL_ColumnSetLayout * pcsl);
	FL_ColumnSetLayout *	getColumnSetLayout(void) const;
	UT_Bool					getNewColumn(UT_uint32 iWidthGiven, UT_uint32 iHeightGiven,
										 FP_Column ** ppCol,
										 UT_sint32 * piXoff, UT_sint32 * piYoff) const;

	void				setPTvars(PT_VarSetIndex vsIndex, PT_AttrPropIndex apIndex);
	UT_Bool				getAttrProp(const PP_AttrProp ** ppAP) const;
	
protected:
	FL_ColumnLayout *		m_prev;
	FL_ColumnLayout *		m_next;
	FL_ColumnSetLayout *	m_pColumnSetLayout;
	PL_StruxDocHandle		m_sdh;
	PT_VarSetIndex			m_vsIndex;
	PT_AttrPropIndex		m_apIndex;
};

#endif /* FL_COLUMNLAYOUT_H */
