/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
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


#include "ut_string.h"
#include "pt_Types.h"
#include "ie_exp_AbiWord_1.h"
#include "pd_Document.h"
#include "pp_AttrProp.h"
#include "px_ChangeRecord.h"
#include "px_ChangeRecord_Object.h"
#include "px_ChangeRecord_Span.h"
#include "px_ChangeRecord_Strux.h"

/*****************************************************************/
/*****************************************************************/

IE_Exp_AbiWord_1::IE_Exp_AbiWord_1(PD_Document * pDocument)
	: IE_Exp(pDocument)
{
	m_error = 0;
	m_pListener = NULL;
	m_lid = 0;
}

IE_Exp_AbiWord_1::~IE_Exp_AbiWord_1()
{
}

/*****************************************************************/
/*****************************************************************/

UT_Bool IE_Exp_AbiWord_1::RecognizeSuffix(const char * szSuffix)
{
	return (UT_stricmp(szSuffix,".abw") == 0);
}

IEStatus IE_Exp_AbiWord_1::StaticConstructor(const char * szSuffix,
											 PD_Document * pDocument,
											 IE_Exp ** ppie)
{
	UT_ASSERT(RecognizeSuffix(szSuffix));
	
	IE_Exp_AbiWord_1 * p = new IE_Exp_AbiWord_1(pDocument);
	*ppie = p;
	return IES_OK;
}

UT_Bool	IE_Exp_AbiWord_1::GetDlgLabels(const char ** pszDesc,
									   const char ** pszSuffixList)
{
	*pszDesc = "AbiWord (.abw)";
	*pszSuffixList = "*.abw";
	return UT_TRUE;
}

/*****************************************************************/
/*****************************************************************/

IEStatus IE_Exp_AbiWord_1::writeFile(const char * szFilename)
{
	UT_ASSERT(m_pDocument);
	UT_ASSERT(szFilename && *szFilename);

	if (!_openFile(szFilename))
		return IES_CouldNotOpenForWriting;

	IEStatus status = _writeDocument();
	if (status == IES_OK)
		_closeFile();
	else
		_abortFile();

	// Note: we let our caller worry about resetting the dirty bit
	// Note: on the document and possibly updating the filename.
	
	return status;
}

void IE_Exp_AbiWord_1::write(const char * sz)
{
	if (m_error)
		return;
	m_error |= ! _writeBytes((UT_Byte *)sz);
	return;
}

void IE_Exp_AbiWord_1::write(const char * sz, UT_uint32 length)
{
	if (m_error)
		return;
	if (_writeBytes((UT_Byte *)sz,length) != length)
		m_error = UT_TRUE;
	
	return;
}

/*****************************************************************/
/*****************************************************************/

class ie_Exp_Listener : public PL_Listener
{
public:
	ie_Exp_Listener(PD_Document * pDocument,
					IE_Exp_AbiWord_1 * pie);
	virtual ~ie_Exp_Listener();

	virtual UT_Bool		populate(PL_StruxFmtHandle sfh,
								 const PX_ChangeRecord * pcr);

	virtual UT_Bool		populateStrux(PL_StruxDocHandle sdh,
									  const PX_ChangeRecord * pcr,
									  PL_StruxFmtHandle * psfh);

	virtual UT_Bool		change(PL_StruxFmtHandle sfh,
							   const PX_ChangeRecord * pcr);

	virtual UT_Bool		insertStrux(PL_StruxFmtHandle sfh,
									const PX_ChangeRecord * pcr,
									PL_StruxDocHandle sdh,
									PL_StruxFmtHandle * psfh);

protected:
	void				_closeSection(void);
	void				_closeBlock(void);
	void				_closeSpan(void);
	void				_openTag(const char * szPrefix, const char * szSuffix,
								 UT_Bool bNewLineAfter, PT_AttrPropIndex api);
	void				_outputData(const UT_UCSChar * p, UT_uint32 length);
	
	PD_Document *		m_pDocument;
	IE_Exp_AbiWord_1 *	m_pie;
	UT_Bool				m_bInSection;
	UT_Bool				m_bInBlock;
	UT_Bool				m_bInSpan;
};

void ie_Exp_Listener::_closeSection(void)
{
	if (!m_bInSection)
		return;
	
	m_pie->write("</section>\n");
	m_bInSection = UT_FALSE;
	return;
}

void ie_Exp_Listener::_closeBlock(void)
{
	if (!m_bInBlock)
		return;

	m_pie->write("</p>\n");
	m_bInBlock = UT_FALSE;
	return;
}

void ie_Exp_Listener::_closeSpan(void)
{
	if (!m_bInSpan)
		return;

	m_pie->write("</c>");
	m_bInSpan = UT_FALSE;
	return;
}

void ie_Exp_Listener::_openTag(const char * szPrefix, const char * szSuffix,
							   UT_Bool bNewLineAfter, PT_AttrPropIndex api)
{
	const PP_AttrProp * pAP = NULL;
	UT_Bool bHaveProp = m_pDocument->getAttrProp(api,&pAP);
	
	m_pie->write("<");
	UT_ASSERT(szPrefix && *szPrefix);
	m_pie->write(szPrefix);
	if (bHaveProp && pAP)
	{
		const XML_Char * szName;
		const XML_Char * szValue;
		UT_uint32 k = 0;

		while (pAP->getNthAttribute(k++,szName,szValue))
		{
			// TODO we force double-quotes on all values.
			// TODO consider scanning the value to see if it has one
			// TODO in it and escaping it or using single-quotes.
			
			m_pie->write(" ");
			m_pie->write(szName);
			m_pie->write("=\"");
			m_pie->write(szValue);
			m_pie->write("\"");
		}
		if (pAP->getNthProperty(0,szName,szValue))
		{
			m_pie->write(" ");
			m_pie->write(PT_PROPS_ATTRIBUTE_NAME);
			m_pie->write("=\"");
			m_pie->write(szName);
			m_pie->write(":");
			m_pie->write(szValue);
			UT_uint32 j = 1;
			while (pAP->getNthProperty(j++,szName,szValue))
			{
				m_pie->write("; ");
				m_pie->write(szName);
				m_pie->write(":");
				m_pie->write(szValue);
			}
			m_pie->write("\"");
		}
	}

	if (szSuffix && *szSuffix)
		m_pie->write(szSuffix);
	m_pie->write(">");
	if (bNewLineAfter)
		m_pie->write("\n");
}

void ie_Exp_Listener::_outputData(const UT_UCSChar * data, UT_uint32 length)
{
	// TODO deal with unicode.
	// TODO for now, just squish it into ascii.
	
#define MY_BUFFER_SIZE		1024
#define MY_HIGHWATER_MARK	6
	char buf[MY_BUFFER_SIZE];
	char * pBuf;
	const UT_UCSChar * pData;

	for (pBuf=buf, pData=data; (pData<data+length); /**/)
	{
		if (pBuf >= (buf+MY_BUFFER_SIZE-MY_HIGHWATER_MARK))
		{
			m_pie->write(buf,(pBuf-buf));
			pBuf = buf;
		}

		UT_ASSERT(*pData < 256);
		switch (*pData)
		{
		case '<':
			*pBuf++ = '&';
			*pBuf++ = 'l';
			*pBuf++ = 't';
			*pBuf++ = ';';
			pData++;
			break;
			
		case '>':
			*pBuf++ = '&';
			*pBuf++ = 'g';
			*pBuf++ = 't';
			*pBuf++ = ';';
			pData++;
			break;
			
		case '&':
			*pBuf++ = '&';
			*pBuf++ = 'a';
			*pBuf++ = 'm';
			*pBuf++ = 'p';
			*pBuf++ = ';';
			pData++;
			break;
			
		default:
			*pBuf++ = (UT_Byte)*pData++;
			break;
		}
	}

	if (pBuf > buf)
		m_pie->write(buf,(pBuf-buf));
}

ie_Exp_Listener::ie_Exp_Listener(PD_Document * pDocument,
								 IE_Exp_AbiWord_1 * pie)
{
	m_pDocument = pDocument;
	m_pie = pie;
	m_bInSection = UT_FALSE;
	m_bInBlock = UT_FALSE;
	m_bInSpan = UT_FALSE;
	
	m_pie->write("<awml>\n");
}

ie_Exp_Listener::~ie_Exp_Listener()
{
	_closeSpan();
	_closeBlock();
	_closeSection();

	m_pie->write("</awml>\n");
}

UT_Bool ie_Exp_Listener::populate(PL_StruxFmtHandle /*sfh*/,
								  const PX_ChangeRecord * pcr)
{
	switch (pcr->getType())
	{
	case PX_ChangeRecord::PXT_InsertSpan:
		{
			const PX_ChangeRecord_Span * pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);

			PT_AttrPropIndex api = pcr->getIndexAP();
			if (api)
			{
				_openTag("c","",UT_FALSE,api);
				m_bInSpan = UT_TRUE;
			}
			
			PT_BufIndex bi = pcrs->getBufIndex();
			_outputData(m_pDocument->getPointer(bi),pcrs->getLength());

			if (api)
				_closeSpan();
			return UT_TRUE;
		}

	case PX_ChangeRecord::PXT_InsertObject:
		{
			const PX_ChangeRecord_Object * pcro = static_cast<const PX_ChangeRecord_Object *> (pcr);
			PT_AttrPropIndex api = pcr->getIndexAP();
			switch (pcro->getObjectType())
			{
			case PTO_Image:
				_openTag("i","/",UT_FALSE,api);
				return UT_TRUE;

			case PTO_Field:
				_openTag("f","/",UT_FALSE,api);
				return UT_TRUE;

			default:
				UT_ASSERT(0);
				return UT_FALSE;
			}
		}

	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}
}

UT_Bool ie_Exp_Listener::populateStrux(PL_StruxDocHandle /*sdh*/,
									   const PX_ChangeRecord * pcr,
									   PL_StruxFmtHandle * psfh)
{
	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = static_cast<const PX_ChangeRecord_Strux *> (pcr);
	*psfh = 0;							// we don't need it.

	switch (pcrx->getStruxType())
	{
	case PTX_Section:
		{
			_closeSpan();
			_closeBlock();
			_closeSection();
			_openTag("section","",UT_TRUE,pcr->getIndexAP());
			m_bInSection = UT_TRUE;
			return UT_TRUE;
		}

	case PTX_Block:
		{
			_closeSpan();
			_closeBlock();
			_openTag("p","",UT_FALSE,pcr->getIndexAP());
			m_bInBlock = UT_TRUE;
			return UT_TRUE;
		}

	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}
}

UT_Bool ie_Exp_Listener::change(PL_StruxFmtHandle /*sfh*/,
								const PX_ChangeRecord * /*pcr*/)
{
	UT_ASSERT(0);						// this function is not used.
	return UT_FALSE;
}

UT_Bool ie_Exp_Listener::insertStrux(PL_StruxFmtHandle /*sfh*/,
									 const PX_ChangeRecord * /*pcr*/,
									 PL_StruxDocHandle /*sdh*/,
									 PL_StruxFmtHandle * /*psfh*/)
{
	UT_ASSERT(0);						// this function is not used.
	return UT_FALSE;
}


/*****************************************************************/
/*****************************************************************/

IEStatus IE_Exp_AbiWord_1::_writeDocument(void)
{
	m_pListener = new ie_Exp_Listener(m_pDocument,this);
	if (!m_pListener)
		return IES_NoMemory;
	if (!m_pDocument->addListener(static_cast<PL_Listener *>(m_pListener),&m_lid))
		return IES_Error;
	m_pDocument->removeListener(m_lid);
	delete m_pListener;

	m_lid = 0;
	m_pListener = NULL;
	
	return ((m_error) ? IES_CouldNotWriteToFile : IES_OK);
}

