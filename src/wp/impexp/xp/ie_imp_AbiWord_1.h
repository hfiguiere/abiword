
#ifndef IE_IMP_ABIWORD_1_H
#define IE_IMP_ABIWORD_1_H

#include <stdio.h>
#include "xmlparse.h"
#include "ut_vector.h"
#include "ie_imp.h"
class PD_Document;

// The importer/reader for AbiWord file format version 1.

class IE_Imp_AbiWord_1 : public IE_Imp
{
protected:
	IE_Imp_AbiWord_1(PD_Document * pDocument);
public:
	~IE_Imp_AbiWord_1();
	IEStatus			importFile(const char * szFilename);

	// the following are public only so that the
	// XML parser callback routines can access them.
	
	void _startElement(const XML_Char *name, const XML_Char **atts);
	void _endElement(const XML_Char *name);
	void _charData(const XML_Char*, int);

protected:
	void			_startElement(const XML_Char *name, const XML_Char **atts);
	void			_endElement(const XML_Char *name);
	void			_charData(const XML_Char *s, int len);

	UT_uint32		_getInlineDepth(void) const;
	UT_Bool			_pushInlineFmt(const XML_Char ** atts);
	void			_popInlineFmt(void);
	
protected:
	typedef emum _parseState { _PS_Init,
							   _PS_Doc,
							   _PS_Sec,
							   _PS_ColSet,
							   _PS_Col,
							   _PS_Block } ParseState;

	IEStatus		m_iestatus;
	ParseState		m_parseState;

	UT_vector		m_vecInlineFmt;
	UT_Stack		m_stackFmtStartIndex;
};

#endif /* IE_IMP_ABIWORD_1_H */
