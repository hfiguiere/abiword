
#ifndef PT_VARSET_H
#define PT_VARSET_H

#include "ut_types.h"
#include "ut_growbuf.h"
#include "pt_Types.h"
#include "pp_TableAttrProp.h"


class pt_VarSet
{
public:
	pt_VarSet();
	~pt_VarSet();
	
	void					setPieceTableState(PTState pts);
	UT_Bool					appendBuf(UT_UCSChar * pBuf, UT_uint32 length, PT_BufIndex * pbi);
	UT_Bool					storeAP(const XML_Char ** attributes, PT_AttrPropIndex * papi);
	UT_Bool					storeAP(const UT_Vector * pVecAttributes, PT_AttrPropIndex * papi);
	const UT_UCSChar *		getPointer(PT_BufIndex bi) const;
	PT_BufIndex				getBufIndex(PT_BufIndex bi, UT_uint32 offset) const;
	UT_Bool					isContiguous(PT_BufIndex bi, UT_uint32 length, PT_BufIndex bi2) const;
	const PP_AttrProp *		getAP(PT_AttrPropIndex api) const;
	UT_Bool					mergeAP(PTChangeFmt ptc,PT_AttrPropIndex apiOld,
									const XML_Char ** attributes, const XML_Char ** properties,
									PT_AttrPropIndex * papiNew);
	
private:
	inline UT_uint32		_subscriptFromBufIndex(PT_BufIndex bi) const;
	inline UT_uint32		_subscriptFromAPIndex(PT_AttrPropIndex api) const;
	inline UT_uint32		_varsetFromBufIndex(PT_BufIndex bi) const;
	inline UT_uint32		_varsetFromAPIndex(PT_AttrPropIndex api) const;
	inline PT_BufIndex		_makeBufIndex(UT_uint32 varset, UT_uint32 subscript) const;
	inline PT_AttrPropIndex	_makeAPIndex(UT_uint32 varset, UT_uint32 subscript) const;
	UT_Bool					_finishConstruction(void);

	UT_Bool					m_bInitialized;
	UT_uint32				m_currentVarSet;

	UT_GrowBuf				m_buffer[2];
	pp_TableAttrProp		m_tableAttrProp[2];
};

	
#endif /* PT_VARSET_H */
