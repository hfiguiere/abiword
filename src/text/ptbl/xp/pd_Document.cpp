/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (c) 2001,2002 Tomas Frydrych
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ut_types.h"
#include "ut_string.h"
#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_bytebuf.h"
#include "ut_base64.h"
#include "ut_misc.h"
#include "pd_Document.h"
#include "xad_Document.h"
#include "pt_PieceTable.h"
#include "pl_Listener.h"
#include "ie_imp.h"
#include "ie_exp.h"
#include "pf_Frag_Strux.h"
#include "pp_Property.h"
#include "pd_Style.h"
#include "pf_Frag_Object.h"
#include "pf_Frag_FmtMark.h"
#include "px_CR_Span.h"
#include "px_CR_SpanChange.h"
#include "px_CR_Strux.h"
#include "pf_Frag.h"
#include "fd_Field.h"
#include "po_Bookmark.h"
#include "fl_AutoNum.h"
#include "xap_Frame.h"
#include "xap_App.h"
#include "xap_Prefs.h"
#include "ap_Prefs.h"
#include "ut_units.h"
#include "ut_string_class.h"
#include "ut_sleep.h"
#include "ut_path.h"

// our currently used DTD
#define ABIWORD_FILEFORMAT_VERSION "1.1"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

struct _dataItemPair
{
	UT_ByteBuf* pBuf;
	void*		pToken;
};

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

// perhaps this should be a magic "unknown" or "NULL" value,
// but now we just depend on save() never being called without
// a previous saveAs() (which specifies a type)
PD_Document::PD_Document(XAP_App *pApp)
	: AD_Document(),
	m_docPageSize(getDefaultPageSize()),
	m_ballowListUpdates(false),
	m_pPieceTable(0),
	m_hashDataItems(11),
	m_lastOpenedType(IEFT_Bogus), // used to be: IE_Imp::fileTypeForSuffix(".abw"))
    m_lastSavedAsType(IEFT_Bogus), // used to be: IE_Exp::fileTypeForSuffix(".abw")
	m_bPieceTableChanging(false),
	m_bDoingPaste(false),
	m_bAllowInsertPointChange(true),
	m_bRedrawHappenning(false),
    m_bLoading(false),
    m_bForcedDirty(false),
    m_bLockedStyles(false),        // same as lockStyles(false)
    m_bMarkRevisions(false),
	m_iRevisionID(1),
    m_bDontImmediatelyLayout(false)
{
	m_pApp = pApp;

#ifdef PT_TEST
	m_pDoc = this;
#endif
}

PD_Document::~PD_Document()
{
	if (m_szFilename)
		free((void *)m_szFilename);
	if (m_pPieceTable)
		delete m_pPieceTable;

	_destroyDataItemData();

	UT_VECTOR_PURGEALL(fl_AutoNum*, m_vecLists);
	UT_VECTOR_PURGEALL(PD_Revision*, m_vRevisions);
	// remove the meta data
	UT_HASH_PURGEDATA(UT_String*, &m_metaDataMap, delete) ;

	// we do not purge the contents of m_vecListeners
	// since these are not owned by us.

	// TODO: delete the key/data pairs
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

UT_uint32 PD_Document::getHighestRevisionId() const
{
	UT_uint32 iId = 0;

	for(UT_uint32 i = 0; i < m_vRevisions.getItemCount(); i++)
	{
		iId = UT_MAX(iId, ((const PD_Revision *)m_vRevisions.getNthItem(i))->getId());
	}

	return iId;
}

const PD_Revision * PD_Document::getHighestRevision() const
{
	UT_uint32 iId = 0;
	const PD_Revision * r = NULL;

	for(UT_uint32 i = 0; i < m_vRevisions.getItemCount(); i++)
	{
		const PD_Revision * t = (const PD_Revision *)m_vRevisions.getNthItem(i);
		UT_uint32 t_id = t->getId();

		if(t_id > iId)
		{
			iId = t_id;
			r = t;
		}
	}

	return r;
}


bool PD_Document::addRevision(UT_uint32 iId, UT_UCS4Char * pDesc)
{
	for(UT_uint32 i = 0; i < m_vRevisions.getItemCount(); i++)
	{
		const PD_Revision * r = (const PD_Revision*) m_vRevisions.getNthItem(i);
		if(r->getId() == iId)
			return false;
	}

	PD_Revision * pRev = new PD_Revision(iId, pDesc);

	m_vRevisions.addItem((void*)pRev);
	m_bForcedDirty = true;
	m_iRevisionID = iId;
	return true;
}

bool PD_Document::addRevision(UT_uint32 iId, const UT_UCS4Char * pDesc, UT_uint32 iLen)
{
	for(UT_uint32 i = 0; i < m_vRevisions.getItemCount(); i++)
	{
		const PD_Revision * r = (const PD_Revision*) m_vRevisions.getNthItem(i);
		if(r->getId() == iId)
			return false;
	}

	UT_UCS4Char * pD = new UT_UCS4Char [iLen + 1];
	UT_UCS4_strncpy(pD,pDesc,iLen);
	pD[iLen] = 0;
	PD_Revision * pRev = new PD_Revision(iId, pD);

	m_vRevisions.addItem((void*)pRev);
	m_bForcedDirty = true;
	m_iRevisionID = iId;
	return true;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void PD_Document::setMetaDataProp ( const UT_String & key,
				    const UT_String & value )
{
  UT_String * ptrvalue = new UT_String ( value ) ;
  m_metaDataMap.set ( key, ptrvalue ) ;
}

bool PD_Document::getMetaDataProp ( const UT_String & key, UT_String & outProp ) const
{
  bool found = false ;
  outProp = "" ;

  const UT_String * val = NULL ;

  val = (UT_String *) m_metaDataMap.pick ( key ) ;
  found = ( val != NULL ) ;

  if ( val && val->size() )
    outProp = *val ;

  return found ;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

UT_Error PD_Document::importFile(const char * szFilename, int ieft,
				 bool markClean)
{
	if (!szFilename || !*szFilename)
	{
		UT_DEBUGMSG(("PD_Document::importFile -- invalid filename\n"));
		return UT_INVALIDFILENAME;
	}

	m_pPieceTable = new pt_PieceTable(this);
	if (!m_pPieceTable)
	{
		UT_DEBUGMSG(("PD_Document::importFile -- could not construct piece table\n"));
		return UT_NOPIECETABLE;
	}

	IE_Imp * pie = NULL;
	UT_Error errorCode;

	IEFileType savedAsType;

	errorCode = IE_Imp::constructImporter(this, szFilename, (IEFileType) ieft, &pie, &savedAsType);
	if (errorCode)
	{
		UT_DEBUGMSG(("PD_Document::importFile -- could not construct importer\n"));
		DELETEP(m_pPieceTable);
		return errorCode;
	}
	m_bLoading = true;
	m_pPieceTable->setPieceTableState(PTS_Loading);
	errorCode = pie->importFile(szFilename);
	delete pie;

	if (errorCode)
	{
		UT_DEBUGMSG(("PD_Document::importFile -- could not import file\n"));
		DELETEP(m_pPieceTable);
		return errorCode;
	}

	m_pPieceTable->setPieceTableState(PTS_Editing);
	m_bLoading = false;
	updateFields();

	if(markClean)
		_setClean();
	else
	  m_bForcedDirty = true; // force this to be dirty

	return UT_OK;
}

UT_Error PD_Document::readFromFile(const char * szFilename, int ieft)
{
	if (!szFilename || !*szFilename)
	{
		UT_DEBUGMSG(("PD_Document::readFromFile -- invalid filename\n"));
		return UT_INVALIDFILENAME;
	}

	if ( !UT_isRegularFile(szFilename) )
	{
	  UT_DEBUGMSG (("PD_Document::readFromFile -- file is not plain file\n"));
	  return UT_INVALIDFILENAME;
	}

	m_pPieceTable = new pt_PieceTable(this);
	if (!m_pPieceTable)
	{
		UT_DEBUGMSG(("PD_Document::readFromFile -- could not construct piece table\n"));
		return UT_NOPIECETABLE;
	}

	IE_Imp * pie = NULL;
	UT_Error errorCode;

	errorCode = IE_Imp::constructImporter(this, szFilename, (IEFileType) ieft, &pie, &m_lastOpenedType);
	if (errorCode)
	{
		UT_DEBUGMSG(("PD_Document::readFromFile -- could not construct importer\n"));
		return errorCode;
	}

	_syncFileTypes(false);

	m_pPieceTable->setPieceTableState(PTS_Loading);
	errorCode = pie->importFile(szFilename);
	delete pie;

	if (errorCode)
	{
		UT_DEBUGMSG(("PD_Document::readFromFile -- could not import file\n"));
		return errorCode;
	}

	UT_ASSERT(!m_szFilename);
	if (!UT_cloneString((char *&)m_szFilename, szFilename))
	{
		UT_DEBUGMSG(("PD_Document::readFromFile -- no memory\n"));
		return UT_IE_NOMEMORY;
	}

	m_pPieceTable->setPieceTableState(PTS_Editing);
	updateFields();
	_setClean();							// mark the document as not-dirty
	return UT_OK;
}

UT_Error PD_Document::newDocument(void)
{
	// the locally installed normal.awt (per-user basis)
	UT_String users_normal_awt (XAP_App::getApp()->getUserPrivateDirectory());
	users_normal_awt += "/templates/normal.awt";
	// the globally installed normal.awt file
	UT_String global_normal_awt (XAP_App::getApp()->getAbiSuiteLibDir());
	global_normal_awt += "/templates/normal.awt";

	if ( UT_OK != importFile ( users_normal_awt.c_str(), IEFT_Unknown, true ) )
    {
		if (UT_OK != importFile ( global_normal_awt.c_str(), IEFT_Unknown, true ) )
		{
			m_pPieceTable = new pt_PieceTable(this);
			if (!m_pPieceTable)
			{
				return UT_NOPIECETABLE;
			}

			m_pPieceTable->setPieceTableState(PTS_Loading);

			// add just enough structure to empty document so we can edit
			appendStrux(PTX_Section,NULL);
			appendStrux(PTX_Block, NULL);

			// set standard document properties, such as dtd, lang,
			// dom-dir, etc. (some of the code that used to be here is
			// now in the setAttrProp() function, since it is shared
			// both by new documents and documents being loaded from disk
			// this also initializes m_indexAP
			setAttrProp(NULL);

			m_pPieceTable->setPieceTableState(PTS_Editing);
		}
		else
		{
		}
    }

	// set the default page size from preferences, regardless of template values
	setDefaultPageSize();

	// mark the document as not-dirty
	_setClean();

	return UT_OK;
}

UT_Error PD_Document::saveAs(const char * szFilename, int ieft)
{
  return saveAs(szFilename, ieft, true);
}

UT_Error PD_Document::saveAs(const char * szFilename, int ieft, bool cpy)
{
	if (!szFilename)
		return UT_SAVE_NAMEERROR;

	IE_Exp * pie = NULL;
	UT_Error errorCode;
	IEFileType newFileType;

	errorCode = IE_Exp::constructExporter(this, szFilename, (IEFileType) ieft, &pie, &newFileType);
	if (errorCode)
	{
		UT_DEBUGMSG(("PD_Document::Save -- could not construct exporter\n"));
		return UT_SAVE_EXPORTERROR;
	}

	if (cpy)
	{
		m_lastSavedAsType = newFileType;
		_syncFileTypes(true);
	}

	errorCode = pie->writeFile(szFilename);
	delete pie;

	if (errorCode)
	{
		UT_DEBUGMSG(("PD_Document::Save -- could not write file\n"));
		return UT_SAVE_WRITEERROR;
	}

	if (cpy) // we want to make the current settings persistent
	{
	    // no file name currently set - make this filename the filename
	    // stored for the doc
	    FREEP(m_szFilename);
	    char * szFilenameCopy = NULL;
	    if (!UT_cloneString(szFilenameCopy,szFilename))
			return UT_SAVE_OTHERERROR;
	    m_szFilename = szFilenameCopy;
	    _setClean(); // only mark as clean if we're saving under a new name
	}

	return UT_OK;
}

UT_Error PD_Document::save(void)
{
	if (!m_szFilename || !*m_szFilename)
		return UT_SAVE_NAMEERROR;
	if (m_lastSavedAsType == IEFT_Unknown)
		return UT_EXTENSIONERROR;

	IE_Exp * pie = NULL;
	UT_Error errorCode;

	errorCode = IE_Exp::constructExporter(this,m_szFilename,m_lastSavedAsType,&pie);
	if (errorCode)
	{
		UT_DEBUGMSG(("PD_Document::Save -- could not construct exporter\n"));
		return UT_SAVE_EXPORTERROR;
	}

	_syncFileTypes(true);

	errorCode = pie->writeFile(m_szFilename);
	delete pie;

	if (errorCode)
	{
		UT_DEBUGMSG(("PD_Document::Save -- could not write file\n"));
		return UT_SAVE_WRITEERROR;
	}

	_setClean();
	return UT_OK;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

bool PD_Document::isDirty(void) const
{
	return m_pPieceTable->isDirty() || m_bForcedDirty;
}

void PD_Document::_setClean(void)
{
	m_pPieceTable->setClean();
	m_pPieceTable->getFragments().cleanFrags();
	m_bForcedDirty = false;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

bool	PD_Document::insertObject(PT_DocPosition dpos,
								  PTObjectType pto,
								  const XML_Char ** attributes,
								  const XML_Char ** properties)
{
	bool res = m_pPieceTable->insertObject(dpos, pto, attributes, properties);
	return res;
}



bool	PD_Document::insertObject(PT_DocPosition dpos,
								  PTObjectType pto,
								  const XML_Char ** attributes,
								  const XML_Char ** properties, fd_Field ** pField)
{
	pf_Frag_Object * pfo = NULL;
	bool bres =  m_pPieceTable->insertObject(dpos, pto, attributes, properties, &pfo);
	*pField = pfo->getField();
	return bres;
}


bool PD_Document::insertSpan(PT_DocPosition dpos,
							 const UT_UCSChar * p,
							 UT_uint32 length,
							 PP_AttrProp *p_AttrProp)
{
	if(p_AttrProp)
	{
		m_pPieceTable->insertFmtMark(PTC_AddFmt, dpos, p_AttrProp);
	}

	return m_pPieceTable->insertSpan(dpos,p, length);
}

bool PD_Document::deleteSpan(PT_DocPosition dpos1,
							 PT_DocPosition dpos2,
							 PP_AttrProp *p_AttrProp_Before,
							 UT_uint32 &iRealDeleteCount,
							 bool bDeleteTableStruxes)
{
	return m_pPieceTable->deleteSpanWithTable(dpos1, dpos2, p_AttrProp_Before,iRealDeleteCount, bDeleteTableStruxes );
}

bool PD_Document::changeSpanFmt(PTChangeFmt ptc,
								PT_DocPosition dpos1,
								PT_DocPosition dpos2,
								const XML_Char ** attributes,
								const XML_Char ** properties)
{
	return m_pPieceTable->changeSpanFmt(ptc,dpos1,dpos2,attributes,properties);
}

bool PD_Document::insertStrux(PT_DocPosition dpos,
							  PTStruxType pts)
{
	return m_pPieceTable->insertStrux(dpos,pts);
}


bool PD_Document::insertStrux(PT_DocPosition dpos,
							  PTStruxType pts,
							  const XML_Char ** attributes,
							  const XML_Char ** properties)
{
	return m_pPieceTable->insertStrux(dpos,pts, attributes,properties);
}


/*!
 * This method deletes the HdrFtr strux pointed to by sdh
 */
void PD_Document::deleteHdrFtrStrux(PL_StruxDocHandle sdh)
{
	pf_Frag_Strux * pfs_hdrftr = (pf_Frag_Strux *) sdh;
	UT_ASSERT(pfs_hdrftr->getType()  == pf_Frag::PFT_Strux);
	m_pPieceTable->deleteHdrFtrStrux(pfs_hdrftr);
}

bool PD_Document::changeStruxFmt(PTChangeFmt ptc,
								 PT_DocPosition dpos1,
								 PT_DocPosition dpos2,
								 const XML_Char ** attributes,
								 const XML_Char ** properties,
								 PTStruxType pts)
{
	return m_pPieceTable->changeStruxFmt(ptc,dpos1,dpos2,attributes,properties,pts);
}

/*!
 * This Method is used to change just the parentID of each strux in a list
 * without updating the fl_Layouts.
 */
bool PD_Document::changeStruxForLists(PL_StruxDocHandle sdh, const char * pszParentID)
{
	return m_pPieceTable->changeStruxForLists(sdh, pszParentID);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

bool PD_Document::appendStrux(PTStruxType pts, const XML_Char ** attributes)
{
	UT_ASSERT(m_pPieceTable);

	// can only be used while loading the document
//
// Update frames during load.
//
	XAP_Frame * pFrame = m_pApp->getLastFocussedFrame();
	if(pFrame)
	{
		pFrame->nullUpdate();
	}
	return m_pPieceTable->appendStrux(pts,attributes);
}

bool PD_Document::appendFmt(const XML_Char ** attributes)
{
	UT_ASSERT(m_pPieceTable);

	// can only be used while loading the document
	return m_pPieceTable->appendFmt(attributes);
}

bool PD_Document::appendFmt(const UT_Vector * pVecAttributes)
{
	UT_ASSERT(m_pPieceTable);

	// can only be used while loading the document

	return m_pPieceTable->appendFmt(pVecAttributes);
}

bool PD_Document::appendSpan(const UT_UCSChar * pbuf, UT_uint32 length)
{
	UT_ASSERT(m_pPieceTable);

	// can only be used while loading the document

	return m_pPieceTable->appendSpan(pbuf,length);
}

bool PD_Document::appendObject(PTObjectType pto, const XML_Char ** attributes)
{
	UT_ASSERT(m_pPieceTable);

	// can only be used while loading the document

	return m_pPieceTable->appendObject(pto,attributes);
}

bool PD_Document::appendFmtMark(void)
{
	UT_ASSERT(m_pPieceTable);

	// can only be used while loading the document

	return m_pPieceTable->appendFmtMark();
}

/*!
 * This method returns the value associated with attribute szAttribute
 * at picetable strux given by sdh.
 \param  PL_StruxDocHandle sdh (pf_Frag_Strux) where we want to find the value
\param const char * szAttribute the attribute we're looking for.
\param const char ** pszValue the value of the attribute.
\returns true if the attribute was present at the sdh
Don't FREEP *pszRetValue!!!
*/
bool PD_Document::getAttributeFromSDH(PL_StruxDocHandle sdh, const char * szAttribute, const char ** pszRetValue)
{
	pf_Frag_Strux * pfStrux = (pf_Frag_Strux *)sdh;
	PT_AttrPropIndex indexAP = pfStrux->getIndexAP();
	const PP_AttrProp * pAP = NULL;
	m_pPieceTable->getAttrProp(indexAP,&pAP);
	UT_ASSERT(pAP);
	const XML_Char * pszValue = NULL;
	(pAP)->getAttribute(szAttribute, pszValue);
	if(pszValue == NULL)
	{
		*pszRetValue = NULL;
		return false;
	}
	*pszRetValue = pszValue;
	return true;
}

/*!
 * This method returns the value associated with attribute szProperty
 * at picetable strux given by sdh.
 \param  PL_StruxDocHandle sdh (pf_Frag_Strux) where we want to find the value
\param const char * szProperty the Property we're looking for.
\param const char ** pszValue the value of the property.
\returns true if the property was present at the sdh
Don't FREEP *pszRetValue!!!
*/
bool PD_Document::getPropertyFromSDH(PL_StruxDocHandle sdh, const char * szProperty, const char ** pszRetValue)
{
	pf_Frag_Strux * pfStrux = (pf_Frag_Strux *)sdh;
	PT_AttrPropIndex indexAP = pfStrux->getIndexAP();
	const PP_AttrProp * pAP = NULL;
	m_pPieceTable->getAttrProp(indexAP,&pAP);
	UT_ASSERT(pAP);
	const XML_Char * pszValue = NULL;
	(pAP)->getProperty(szProperty, pszValue);
	if(pszValue == NULL)
	{
		*pszRetValue = NULL;
		return false;
	}
	*pszRetValue = pszValue;
	return true;
}

/*!
 * This medthod modifies the attributes of a section strux without
 * generating a change record. Use with extreme care!!
 */
bool  PD_Document::changeSectionAttsNoUpdate(PL_StruxDocHandle sdh, const char * attr, const char * attvalue)
{
	pf_Frag_Strux * pfStrux = (pf_Frag_Strux *)sdh;
	UT_ASSERT(pfStrux);
	UT_ASSERT(pfStrux->getStruxType() == PTX_Section);
	bool bres = m_pPieceTable->changeSectionAttsNoUpdate(pfStrux, attr, attvalue);
	return bres;
}

/*!
 * This method returns the last pf_Frag_Strux as a PL_StruxDocHandle before the end of the piecetable.
 */
PL_StruxDocHandle  PD_Document::getLastSectionSDH(void)
{
	pf_Frag * currentFrag = m_pPieceTable->getFragments().getFirst();
	pf_Frag_Strux * pfSecLast = NULL;
	while (currentFrag!=m_pPieceTable->getFragments().getLast())
	{
		UT_ASSERT(currentFrag);
		if(currentFrag->getType()  == pf_Frag::PFT_Strux)
		{
		     pf_Frag_Strux * pfSec = static_cast<pf_Frag_Strux *>(currentFrag);
		     if(pfSec->getStruxType() == PTX_Section)
		     {
				 pfSecLast = pfSec;
			 }
		}
		currentFrag = currentFrag->getNext();
	}
	return (PL_StruxDocHandle *) pfSecLast;
}


/*!
 * This method scans the document to check that the id of a header/footer
 *  section actually exists in a section somewhere in the document.
 */
bool PD_Document::verifySectionID(const XML_Char * pszId)
{
	pf_Frag * currentFrag = m_pPieceTable->getFragments().getFirst();
	while (currentFrag!=m_pPieceTable->getFragments().getLast())
	{
		UT_ASSERT(currentFrag);
		PT_AttrPropIndex indexAP = 0;
		if(currentFrag->getType()  == pf_Frag::PFT_Strux)
		{
		     pf_Frag_Strux * pfSec = static_cast<pf_Frag_Strux *>(currentFrag);
		     if(pfSec->getStruxType() == PTX_Section)
		     {
				 indexAP = static_cast<pf_Frag_Text *>(currentFrag)->getIndexAP();
				 const PP_AttrProp * pAP = NULL;
				 m_pPieceTable->getAttrProp(indexAP,&pAP);
				 UT_ASSERT(pAP);
				 const XML_Char * pszIDName = NULL;
				 (pAP)->getAttribute("header", pszIDName);
				 if(pszIDName && UT_XML_stricmp(pszIDName,pszId) == 0)
				 {
					 return true;
				 }
				 (pAP)->getAttribute("header-first", pszIDName);
				 if(pszIDName && UT_XML_stricmp(pszIDName,pszId) == 0)
				 {
					 return true;
				 }
				 (pAP)->getAttribute("header-last", pszIDName);
				 if(pszIDName && UT_XML_stricmp(pszIDName,pszId) == 0)
				 {
					 return true;
				 }
				 (pAP)->getAttribute("header-even", pszIDName);
				 if(pszIDName && UT_XML_stricmp(pszIDName,pszId) == 0)
				 {
					 return true;
				 }
				 (pAP)->getAttribute("footer", pszIDName);
				 if(pszIDName && UT_XML_stricmp(pszIDName,pszId) == 0)
				 {
					 return true;
				 }
				 (pAP)->getAttribute("footer-first", pszIDName);
				 if(pszIDName && UT_XML_stricmp(pszIDName,pszId) == 0)
				 {
					 return true;
				 }
				 (pAP)->getAttribute("footer-last", pszIDName);
				 if(pszIDName && UT_XML_stricmp(pszIDName,pszId) == 0)
				 {
					 return true;
				 }
				 (pAP)->getAttribute("footer-even", pszIDName);
				 if(pszIDName && UT_XML_stricmp(pszIDName,pszId) == 0)
				 {
					 return true;
				 }

		     }
		}
//
// Get Next frag in the table.
//
		currentFrag = currentFrag->getNext();
	}
	return false;
}



/*!
 * This method scans the document to look for a HdrFtr strux.
\params const char * pszHdrFtr The particular attribute that identifies the
                               strux as "header" "footer" "header-even" etc.
\params const char * pszHdrFtrID the unique string to match with Docsection.
\returns a PL_StruxDocHandle of the matching frag or NULL if none found.
 */
PL_StruxDocHandle PD_Document::findHdrFtrStrux(const XML_Char * pszHdrFtr,
											const XML_Char * pszHdrFtrID)
{
	pf_Frag * currentFrag = m_pPieceTable->getFragments().getFirst();
	while (currentFrag!=m_pPieceTable->getFragments().getLast())
	{
		UT_ASSERT(currentFrag);
		PT_AttrPropIndex indexAP = 0;
		if(currentFrag->getType()  == pf_Frag::PFT_Strux)
		{
		     pf_Frag_Strux * pfSec = static_cast<pf_Frag_Strux *>(currentFrag);
		     if(pfSec->getStruxType() == PTX_SectionHdrFtr)
		     {
				 indexAP = pfSec->getIndexAP();
				 const PP_AttrProp * pAP = NULL;
				 m_pPieceTable->getAttrProp(indexAP,&pAP);
				 UT_ASSERT(pAP);
				 const XML_Char * pszIDName = NULL;
				 const XML_Char * pszHeaderName = NULL;
				 (pAP)->getAttribute(PT_TYPE_ATTRIBUTE_NAME, pszHeaderName);
				 (pAP)->getAttribute(PT_ID_ATTRIBUTE_NAME, pszIDName);
				 if(pszIDName && pszHeaderName && (UT_XML_stricmp(pszIDName,pszHdrFtrID) == 0) && (UT_XML_stricmp(pszHeaderName,pszHdrFtr) == 0))
				 {
					 return (PL_StruxDocHandle) pfSec ;
				 }
			 }
		}
//
// Get Next frag in the table.
//
		currentFrag = currentFrag->getNext();
	}
	return NULL;
}



/*!
 * This method scans the document for all styles used in the document, including
 * styles in the basedon heiracy and the followedby list
 *
 */
void PD_Document::getAllUsedStyles(UT_Vector * pVecStyles)
{
	UT_sint32 i = 0;
	pf_Frag * currentFrag = m_pPieceTable->getFragments().getFirst();
	PD_Style * pStyle = NULL;
	while (currentFrag!=m_pPieceTable->getFragments().getLast())
	{
		UT_ASSERT(currentFrag);
//
// get indexAP
// get PT_STYLE_ATTRIBUTE_NAME
// if it matches style name or is contained in a basedon name or followedby
//
//
// All this code is used to find if this frag has a style in it.
//
		PT_AttrPropIndex indexAP = 0;
		if(currentFrag->getType()  == pf_Frag::PFT_Strux)
		{
			indexAP = static_cast<pf_Frag_Strux *>(currentFrag)->getIndexAP();
		}
		else if(currentFrag->getType()  == pf_Frag::PFT_Text)
		{
			indexAP = static_cast<pf_Frag_Text *>(currentFrag)->getIndexAP();
		}
		else if(currentFrag->getType()  == pf_Frag::PFT_Object)
		{
			indexAP = static_cast<pf_Frag_Object *>(currentFrag)->getIndexAP();
		}
		else if(currentFrag->getType()  == pf_Frag::PFT_FmtMark)
		{
			indexAP = static_cast<pf_Frag_FmtMark *>(currentFrag)->getIndexAP();
		}
		const PP_AttrProp * pAP = NULL;
		m_pPieceTable->getAttrProp(indexAP,&pAP);
		UT_ASSERT(pAP);
		const XML_Char * pszStyleName = NULL;
		(pAP)->getAttribute(PT_STYLE_ATTRIBUTE_NAME, pszStyleName);
//
// We've found a style...
//
		if(pszStyleName != NULL)
		{
			m_pPieceTable->getStyle(pszStyleName,&pStyle);
			UT_ASSERT(pStyle);
			if(pStyle)
			{
				if(pVecStyles->findItem((void *) pStyle) < 0)
				{
					pVecStyles->addItem((void *) pStyle);
				}
				PD_Style * pBasedOn = pStyle->getBasedOn();
				i = 0;
				while(pBasedOn != NULL && i <  pp_BASEDON_DEPTH_LIMIT)
				{
					if(pVecStyles->findItem((void *) pBasedOn) < 0)
					{
						pVecStyles->addItem((void *) pBasedOn);
					}
					i++;
					pBasedOn = pBasedOn->getBasedOn();
				}
				PD_Style * pFollowedBy = pStyle->getFollowedBy();
				if(pFollowedBy && (pVecStyles->findItem((void *) pFollowedBy) < 0))
				{
					pVecStyles->addItem((void *) pFollowedBy);
				}
			}
		}
//
// Get Next frag in the table.
//
		currentFrag = currentFrag->getNext();
	}
//
// Done!
//
}

/*!
 * This method removes the style of name pszName from the styles definition and removes
 * all instances of it from the document including the basedon heiracy and the
 * followed-by sequences.
 */
bool PD_Document::removeStyle(const XML_Char * pszName)
{
	UT_ASSERT(m_pPieceTable);
//
// First replace all occurances of pszName with "Normal"
//
	PD_Style * pNormal = NULL;
	PD_Style * pNuke = NULL;
	m_pPieceTable->getStyle(pszName,&pNuke);
	UT_ASSERT(pNuke);
	pNormal = pNuke->getBasedOn();
	const XML_Char * szBack = NULL;
	if(pNormal == NULL)
	{
		m_pPieceTable->getStyle("Normal",&pNormal);
		szBack = "None";
	}
	else
	{
//
// The name of the style is stored in the PT_NAME_ATTRIBUTE_NAME attribute within the
// style
//
		pNormal->getAttribute(PT_NAME_ATTRIBUTE_NAME, szBack);
	}
	UT_ASSERT(szBack);
	UT_ASSERT(pNormal);
	PT_AttrPropIndex indexNormal = pNormal->getIndexAP();

	struct prevStuff
	{
		pf_Frag::PFType fragType;
		pf_Frag_Strux * lastFragStrux;
		PT_AttrPropIndex indexAPFrag;
		pf_Frag * thisFrag;
		PT_DocPosition thisPos;
		PT_DocPosition thisStruxPos;
		UT_uint32 fragLength;
		bool bChangeIndexAP;
	};
//
// Now scan through the document finding all instances of pszName as either
// the style or the basedon style or the followed by style. Replace these with
// "normal"
//
	UT_Vector vFrag;

	PT_DocPosition pos = 0;
	PT_DocPosition posLastStrux = 0;
	pf_Frag_Strux * pfs = NULL;
	pf_Frag * currentFrag = m_pPieceTable->getFragments().getFirst();
	UT_ASSERT(currentFrag);
	while (currentFrag!=m_pPieceTable->getFragments().getLast())
	{
//
// get indexAP
// get PT_STYLE_ATTRIBUTE_NAME
// if it matches style name or is contained in a basedon name or followedby
//
//
// All this code is used to find if this strux has our style in it
//
		PT_AttrPropIndex indexAP = 0;
		if(currentFrag->getType()  == pf_Frag::PFT_Strux)
		{
			pfs = static_cast<pf_Frag_Strux *>(currentFrag);
			indexAP = static_cast<pf_Frag_Strux *>(currentFrag)->getIndexAP();
			posLastStrux = pos;
		}
		else if(currentFrag->getType()  == pf_Frag::PFT_Text)
		{
			indexAP = static_cast<pf_Frag_Text *>(currentFrag)->getIndexAP();
		}
		else if(currentFrag->getType()  == pf_Frag::PFT_Object)
		{
			indexAP = static_cast<pf_Frag_Object *>(currentFrag)->getIndexAP();
		}
		else if(currentFrag->getType()  == pf_Frag::PFT_FmtMark)
		{
			indexAP = static_cast<pf_Frag_FmtMark *>(currentFrag)->getIndexAP();
		}
		const PP_AttrProp * pAP = NULL;
		m_pPieceTable->getAttrProp(indexAP,&pAP);
		UT_ASSERT(pAP);
		const XML_Char * pszStyleName = NULL;
		(pAP)->getAttribute(PT_STYLE_ATTRIBUTE_NAME, pszStyleName);
//
// It does so remember this frag and set the old indexAP to Normal
//
		if(pszStyleName != NULL && strcmp(pszStyleName,pszName)==0)
		{
			prevStuff *  pStuff = new prevStuff;
			pf_Frag::PFType cType = currentFrag->getType();
			pStuff->fragType = cType;
			pStuff->thisFrag = currentFrag;
			pStuff->indexAPFrag = indexAP;
			pStuff->lastFragStrux = pfs;
			pStuff->thisPos = pos;
			pStuff->thisStruxPos = pos;
			pStuff->fragLength = currentFrag->getLength();
			pStuff->bChangeIndexAP = true;
			vFrag.addItem((void *) pStuff);
//
// OK set this frag's indexAP to that of basedon of our deleted style or
// Normal.
//
			if(pf_Frag::PFT_Strux == cType)
				static_cast<pf_Frag_Strux *>(currentFrag)->setIndexAP(indexNormal);
			else if(pf_Frag::PFT_Text == cType)
				static_cast<pf_Frag_Text *>(currentFrag)->setIndexAP(indexNormal);
			else if(pf_Frag::PFT_Object == cType)
				static_cast<pf_Frag_Object *>(currentFrag)->setIndexAP(indexNormal);
			else if(pf_Frag::PFT_FmtMark == cType)
				static_cast<pf_Frag_FmtMark *>(currentFrag)->setIndexAP(indexNormal);
		}
//
// Now recursively search to see if has our style in the basedon list
//
		else if(pszStyleName != NULL)
		{
			PD_Style * cStyle = NULL;
			m_pPieceTable->getStyle(pszStyleName,&cStyle);
			UT_ASSERT(cStyle);
			if(!cStyle)
				break;
			PD_Style * pBasedOn = cStyle->getBasedOn();
			PD_Style * pFollowedBy = cStyle->getFollowedBy();
			UT_uint32 i =0;
			for(i=0; (i < pp_BASEDON_DEPTH_LIMIT) && (pBasedOn != NULL) && (pBasedOn!= pNuke); i++)
			{
				pBasedOn = pBasedOn->getBasedOn();
			}
			if(pBasedOn == pNuke)
			{
				prevStuff *  pStuff = new prevStuff;
				pStuff->fragType = currentFrag->getType();
				pStuff->thisFrag = currentFrag;
				pStuff->indexAPFrag = indexAP;
				pStuff->lastFragStrux = pfs;
				pStuff->thisPos = pos;
				pStuff->thisStruxPos = pos;
				pStuff->fragLength = currentFrag->getLength();
				pStuff->bChangeIndexAP = false;
				vFrag.addItem((void *) pStuff);
			}
//
// Look if followedBy points to our style
//
			else if(pFollowedBy == pNuke)
			{
				prevStuff *  pStuff = new prevStuff;
				pStuff->fragType = currentFrag->getType();
				pStuff->thisFrag = currentFrag;
				pStuff->indexAPFrag = indexAP;
				pStuff->lastFragStrux = pfs;
				pStuff->thisPos = pos;
				pStuff->thisStruxPos = pos;
				pStuff->fragLength = currentFrag->getLength();
				pStuff->bChangeIndexAP = false;
				vFrag.addItem((void *) pStuff);
			}
		}
		pos = pos + currentFrag->getLength();
		currentFrag = currentFrag->getNext();
	}
//
// Now replace all pointers to this style in basedon or followedby
// with Normal
//
	UT_uint32 nstyles = getStyleCount();
	const PD_Style * cStyle = NULL;
	const char * szCstyle = NULL;
	UT_uint32 i;
	for(i=0; i< nstyles;i++)
	{
		enumStyles(i, &szCstyle,&cStyle);
		bool bDoBasedOn = false;
		bool bDoFollowedby = false;
		if(const_cast<PD_Style *>(cStyle)->getBasedOn() == pNuke)
		{
			bDoBasedOn = true;
		}
		if(const_cast<PD_Style *>(cStyle)->getFollowedBy() == pNuke)
		{
			bDoFollowedby = true;
		}
		const XML_Char * nAtts[5] ={NULL,NULL,NULL,NULL,NULL};
		if( bDoBasedOn && bDoFollowedby)
		{
			nAtts[0] = "basedon"; nAtts[1] =  szBack;
			nAtts[2]= "followedby";	nAtts[3] = "Current Settings";
			nAtts[4] = NULL;
		}
		else if ( bDoBasedOn && ! bDoFollowedby)
		{
			nAtts[0] = "basedon"; nAtts[1] =  szBack;
			nAtts[2] = NULL;
		}
		else if ( !bDoBasedOn && bDoFollowedby)
		{
			nAtts[0]= "followedby";	nAtts[1] = "Current Settings";
			nAtts[2] = NULL;
		}
		if( bDoBasedOn || bDoFollowedby)
		{
			UT_uint32 i =0;
			for(i=0; nAtts[i] != NULL; i+=2)
			{
				xxx_UT_DEBUGMSG(("SEVIOR New Style Name %s, Value %s \n",nAtts[i],nAtts[i+1]));
			}
			const_cast<PD_Style *>(cStyle)->addAttributes( (const XML_Char **) nAtts);
		}
	}
//
// OK Now remove the style
//
	m_pPieceTable->removeStyle(pszName);
//
// Alright now we replace all the instances of fragSrux using the style to be
// deleted.
//
	UT_sint32 countChanges = vFrag.getItemCount();
	UT_sint32 j;
	pf_Frag * pfsLast = NULL;
	PX_ChangeRecord * pcr = NULL;
	for(j = 0; j<countChanges; j++)
	{
		prevStuff * pStuff = (prevStuff *) vFrag.getNthItem(j);
		if(pStuff->fragType == pf_Frag::PFT_Strux)
		{
			pfsLast = pStuff->thisFrag;
			if(pStuff->bChangeIndexAP)
			{
				pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_ChangeStrux,pStuff->thisPos,indexNormal);
				notifyListeners(pStuff->lastFragStrux, pcr);
				delete pcr;
			}
			else
			{
				pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_ChangeStrux,pStuff->thisPos,pStuff->indexAPFrag);
				notifyListeners(pStuff->lastFragStrux, pcr);
				delete pcr;
			}
		}
		else
		{
			if(pStuff->lastFragStrux != pfsLast)
			{
				pfsLast = pStuff->lastFragStrux;
				if(pStuff->bChangeIndexAP)
				{
					pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_ChangeStrux,pStuff->thisPos,indexNormal);
					notifyListeners(pStuff->lastFragStrux, pcr);
					delete pcr;
				}
				else
				{
					PT_AttrPropIndex indexLastAP = static_cast<pf_Frag_Strux *>(pfsLast)->getIndexAP();
					pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_ChangeStrux,pStuff->thisPos,indexLastAP);
					notifyListeners(pStuff->lastFragStrux, pcr);
					delete pcr;
				}
			}
		}
	}
//  		else if(bisCharStyle)
//  		{
//  			UT_uint32 blockoffset = (UT_uint32) (pStuff->thisPos - pStuff->thisStruxPos -1);
//  			pf_Frag_Text * pft = (pf_Frag_Text *) pStuff->thisFrag;
//  			PX_ChangeRecord_SpanChange * pcr =
//  				new PX_ChangeRecord_SpanChange(PX_ChangeRecord::PXT_ChangeSpan,
//  											   pStuff->thisPos,
//  											   pStuff->indexAPFrag,
//  											   indexNormal,
//  											   m_pPieceTable->getVarSet().getBufIndex(pft->getBufIndex(),0),
//  											   pStuff->fragLength,
//  											   blockoffset);
//  			notifyListeners(pStuff->lastFragStrux, pcr);
//  			delete pcr;
//  		}
//  	}
	if(countChanges > 0)
	{
		UT_VECTOR_PURGEALL(prevStuff *,vFrag);
	}
//
// Now reformat the entire document
//
//	signalListeners(PD_SIGNAL_REFORMAT_LAYOUT);
	return true;
}

bool PD_Document::appendStyle(const XML_Char ** attributes)
{
	UT_ASSERT(m_pPieceTable);

	// can only be used while loading the document

	return m_pPieceTable->appendStyle(attributes);
}

size_t PD_Document::getStyleCount(void)
{
  UT_ASSERT(m_pPieceTable);

  return m_pPieceTable->getStyleCount();
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

bool PD_Document::tellListener(PL_Listener* pListener)
{
	UT_ASSERT(pListener);
	UT_ASSERT(m_pPieceTable);

	return m_pPieceTable->tellListener(pListener);
}

bool PD_Document::tellListenerSubset(PL_Listener* pListener, PD_DocumentRange * pDocRange)
{
	UT_ASSERT(pListener);
	UT_ASSERT(m_pPieceTable);
	UT_ASSERT(pDocRange && pDocRange->m_pDoc==this);

	return m_pPieceTable->tellListenerSubset(pListener,pDocRange);
}

bool PD_Document::addListener(PL_Listener * pListener,
								 PL_ListenerId * pListenerId)
{
	UT_uint32 kLimit = m_vecListeners.getItemCount();
	UT_uint32 k=0;

	// see if we can recycle a cell in the vector.

	for (k=0; k<kLimit; k++)
		if (m_vecListeners.getNthItem(k) == 0)
		{
			(void)m_vecListeners.setNthItem(k,pListener,NULL);
			goto ClaimThisK;
		}

	// otherwise, extend the vector for it.

	if (m_vecListeners.addItem(pListener,&k) != 0)
	{
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return false;				// could not add item to vector
	}
  ClaimThisK:

	// propagate the listener to the PieceTable and
	// let it do its thing.
	UT_ASSERT(m_pPieceTable);

	// give our vector index back to the caller as a "Listener Id".

	*pListenerId = k;
	UT_ASSERT(pListener);
	m_pPieceTable->addListener(pListener,k);
	return true;
}

bool PD_Document::removeListener(PL_ListenerId listenerId)
{
	return (m_vecListeners.setNthItem(listenerId,NULL,NULL) == 0);
}

bool PD_Document::signalListeners(UT_uint32 iSignal) const
{
	PL_ListenerId lid;
	PL_ListenerId lidCount = m_vecListeners.getItemCount();

	// for each listener in our vector, we send a notification.
	// we step over null listners (for listeners which have been
	// removed (views that went away)).

	for (lid=0; lid<lidCount; lid++)
	{
		PL_Listener * pListener = (PL_Listener *)m_vecListeners.getNthItem(lid);
		if (pListener)
		{
			pListener->signal(iSignal);
		}
	}

	return true;
}

bool PD_Document::notifyListeners(pf_Frag_Strux * pfs, const PX_ChangeRecord * pcr) const
{
	// notify listeners of a change.

#ifdef PT_TEST
	//pcr->__dump();
#endif

	PL_ListenerId lid;
	PL_ListenerId lidCount = m_vecListeners.getItemCount();

	// for each listener in our vector, we send a notification.
	// we step over null listners (for listeners which have been
	// removed (views that went away)).

	for (lid=0; lid<lidCount; lid++)
	{
		PL_Listener * pListener = (PL_Listener *)m_vecListeners.getNthItem(lid);
		if (pListener)
		{
			PL_StruxFmtHandle sfh = 0;
			if (pfs)
				sfh = pfs->getFmtHandle(lid);
			pListener->change(sfh,pcr);
		}
	}

	return true;
}

PL_StruxFmtHandle PD_Document::getNthFmtHandle(PL_StruxDocHandle sdh, UT_uint32 n)
{
	pf_Frag_Strux * pfs = (pf_Frag_Strux *) sdh;
	UT_uint32 nListen = m_vecListeners.getItemCount();
	if(n >= nListen)
		return NULL;
	PL_ListenerId lid = (PL_ListenerId) n;
	PL_StruxFmtHandle sfh = pfs->getFmtHandle(lid);
	return sfh;
}

static void s_BindHandles(PL_StruxDocHandle sdhNew,
						  PL_ListenerId lid,
						  PL_StruxFmtHandle sfhNew)
{
	UT_ASSERT(sdhNew);
	UT_ASSERT(sfhNew);

	pf_Frag_Strux * pfsNew = (pf_Frag_Strux *)sdhNew;
	pfsNew->setFmtHandle(lid,sfhNew);
}

bool PD_Document::notifyListeners(pf_Frag_Strux * pfs,
									 pf_Frag_Strux * pfsNew,
									 const PX_ChangeRecord * pcr) const
{
	// notify listeners of a new strux.  this is slightly
	// different from the other one because we need to exchange
	// handles with the listener for the new strux.

#ifdef PT_TEST
	//pcr->__dump();
#endif

	PL_ListenerId lid;
	PL_ListenerId lidCount = m_vecListeners.getItemCount();

	// for each listener in our vector, we send a notification.
	// we step over null listeners (for listeners which have been
	// removed (views that went away)).

	for (lid=0; lid<lidCount; lid++)
	{
		PL_Listener * pListener = (PL_Listener *)m_vecListeners.getNthItem(lid);
		if (pListener)
		{
			PL_StruxDocHandle sdhNew = (PL_StruxDocHandle)pfsNew;
			PL_StruxFmtHandle sfh = pfs->getFmtHandle(lid);
			if (pListener->insertStrux(sfh,pcr,sdhNew,lid,s_BindHandles))
			{
				// verify that the listener used our callback
				UT_ASSERT(pfsNew->getFmtHandle(lid));
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

bool PD_Document::getAttrProp(PT_AttrPropIndex indexAP, const PP_AttrProp ** ppAP) const
{
	return m_pPieceTable->getAttrProp(indexAP,ppAP);
}

const UT_UCSChar * PD_Document::getPointer(PT_BufIndex bi) const
{
	// the pointer that we return is NOT a zero-terminated
	// string.  the caller is responsible for knowing how
	// long the data is within the span/fragment.

	return m_pPieceTable->getPointer(bi);
}

bool PD_Document::getSpanPtr(PL_StruxDocHandle sdh, UT_uint32 offset,
								const UT_UCSChar ** ppSpan, UT_uint32 * pLength) const
{
	return m_pPieceTable->getSpanPtr(sdh,offset,ppSpan,pLength);
}

bool PD_Document::getBlockBuf(PL_StruxDocHandle sdh, UT_GrowBuf * pgb) const
{
	return m_pPieceTable->getBlockBuf(sdh,pgb);
}

bool PD_Document::getBounds(bool bEnd, PT_DocPosition & docPos) const
{
	return m_pPieceTable->getBounds(bEnd,docPos);
}

PT_DocPosition PD_Document::getStruxPosition(PL_StruxDocHandle sdh) const
{
	return m_pPieceTable->getStruxPosition(sdh);
}

bool PD_Document::getSpanAttrProp(PL_StruxDocHandle sdh, UT_uint32 offset, bool bLeftSide,
									 const PP_AttrProp ** ppAP) const
{
	return m_pPieceTable->getSpanAttrProp(sdh,offset,bLeftSide,ppAP);
}

/*!
 * Return strux type of the StruxDocHandle
 */
PTStruxType PD_Document::getStruxType(PL_StruxDocHandle sdh) const
{
	pf_Frag * pf = (pf_Frag *)sdh;
	UT_ASSERT(pf->getType() == pf_Frag::PFT_Strux);
	pf_Frag_Strux * pfs = static_cast<pf_Frag_Strux *> (pf);
	return pfs->getStruxType();
}

po_Bookmark * PD_Document::getBookmark(PL_StruxDocHandle sdh, UT_uint32 offset)
{
	pf_Frag * pf = (pf_Frag *)sdh;
	UT_ASSERT(pf->getType() == pf_Frag::PFT_Strux);
	pf_Frag_Strux * pfsBlock = static_cast<pf_Frag_Strux *> (pf);
	UT_ASSERT(pfsBlock->getStruxType() == PTX_Block);

	UT_uint32 cumOffset = 0;
	pf_Frag_Object * pfo = NULL;
	for (pf_Frag * pfTemp=pfsBlock->getNext(); (pfTemp); pfTemp=pfTemp->getNext())
	{
		cumOffset += pfTemp->getLength();
		if (offset < cumOffset)
		{
			switch (pfTemp->getType())
			{
				case pf_Frag::PFT_Object:
					pfo = static_cast<pf_Frag_Object *> (pfTemp);
					return pfo->getBookmark();
				default:
					return NULL;
			}
		}

	}
	return NULL;
}

bool PD_Document::getField(PL_StruxDocHandle sdh, UT_uint32 offset,
                               fd_Field * & pField)
{

	pf_Frag * pf = (pf_Frag *)sdh;
	UT_ASSERT(pf->getType() == pf_Frag::PFT_Strux);
	pf_Frag_Strux * pfsBlock = static_cast<pf_Frag_Strux *> (pf);
	UT_ASSERT(pfsBlock->getStruxType() == PTX_Block);

	UT_uint32 cumOffset = 0;
	pf_Frag_Text * pft = NULL;
	for (pf_Frag * pfTemp=pfsBlock->getNext(); (pfTemp); pfTemp=pfTemp->getNext())
	{
		cumOffset += pfTemp->getLength();
		if (offset < cumOffset)
		{
			switch (pfTemp->getType())
			{
			case pf_Frag::PFT_Text:
			case pf_Frag::PFT_Object:
				pft = static_cast<pf_Frag_Text *> (pfTemp);
				pField = pft->getField();
				return true; // break out of loop
				break;
			default:
				return false;
				break;
			}
		}

	}
	return false;
}

bool PD_Document::getStruxFromPosition(PL_ListenerId listenerId,
										  PT_DocPosition docPos,
										  PL_StruxFmtHandle * psfh) const
{
	return m_pPieceTable->getStruxFromPosition(listenerId,docPos,psfh);
}

bool PD_Document::getStruxOfTypeFromPosition(PL_ListenerId listenerId,
												PT_DocPosition docPos,
												PTStruxType pts,
												PL_StruxFmtHandle * psfh) const
{
	return m_pPieceTable->getStruxOfTypeFromPosition(listenerId,docPos,pts,psfh);
}


///
///  return the SDH of the last strux of the given type
/// immediately prior to the given absolute document position.
/// This sdh is actually a (void *) pointer to a pf_Frag_Strux
///
bool PD_Document::getStruxOfTypeFromPosition(PT_DocPosition docPos,
												PTStruxType pts,
												PL_StruxDocHandle * sdh) const
{
	return m_pPieceTable->getStruxOfTypeFromPosition(docPos,pts, sdh);
}

///
/// Return the sdh of type pts immediately prior to sdh
///
bool PD_Document::getPrevStruxOfType(PL_StruxDocHandle sdh,PTStruxType pts,
					PL_StruxDocHandle * prevsdh)
{
	pf_Frag_Strux * pfs = (pf_Frag_Strux *) sdh;
	UT_ASSERT(pfs);
	pfs = (pf_Frag_Strux *) pfs->getPrev();
	for (pf_Frag * pf=pfs; (pf); pf=pf->getPrev())
		if (pf->getType() == pf_Frag::PFT_Strux)
		{
			pf_Frag_Strux * pfsTemp = static_cast<pf_Frag_Strux *>(pf);
			if (pfsTemp->getStruxType() == pts)	// did we find it
			{
				*prevsdh = pfsTemp;
				return true;
			}
		}

	// did not find it.

	return false;
}


///
/// Return the sdh of type pts immediately after sdh
///
bool PD_Document::getNextStruxOfType(PL_StruxDocHandle sdh,PTStruxType pts,
					PL_StruxDocHandle * nextsdh)
{
	pf_Frag_Strux * pfs = (pf_Frag_Strux *) sdh;
	UT_ASSERT(pfs);
	pfs = (pf_Frag_Strux *) pfs->getNext();
	for (pf_Frag * pf=pfs; (pf); pf=pf->getNext())
		if (pf->getType() == pf_Frag::PFT_Strux)
		{
			pf_Frag_Strux * pfsTemp = static_cast<pf_Frag_Strux *>(pf);
			if (pfsTemp->getStruxType() == pts)	// did we find it
			{
				*nextsdh = pfsTemp;
				return true;
			}
		}

	// did not find it.

	return false;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void PD_Document::beginUserAtomicGlob(void)
{
	m_pPieceTable->beginUserAtomicGlob();
}

void PD_Document::endUserAtomicGlob(void)
{
	m_pPieceTable->endUserAtomicGlob();
}

UT_uint32 PD_Document::undoCount(bool bUndo) const
{
  return m_pPieceTable->undoCount(bUndo);
}

bool PD_Document::canDo(bool bUndo) const
{
	return m_pPieceTable->canDo(bUndo);
}

bool PD_Document::undoCmd(UT_uint32 repeatCount)
{
	UT_sint32 sRepeatCount = (UT_uint32)repeatCount;
	while (sRepeatCount > 0)
	{
		UT_uint32 inCount = undoCount(true);
		if (!m_pPieceTable->undoCmd())
			return false;
		sRepeatCount -= inCount - undoCount(true);
	}
	return true;
}

bool PD_Document::redoCmd(UT_uint32 repeatCount)
{
	while (repeatCount--)
		if (!m_pPieceTable->redoCmd())
			return false;
	return true;
}

///////////////////////////////////////////////////////////////////
// DataItems represent opaque (and probably binary) data found in
// the data-section of the document.  These are used, for example,
// to store the actual data of an image.  The inline image tag has
// a reference to a DataItem.

bool PD_Document::createDataItem(const char * szName, bool bBase64, const UT_ByteBuf * pByteBuf,
									void* pToken,
									void ** ppHandle)
{
	// verify unique name

	if (getDataItemDataByName(szName,NULL,NULL,NULL) == true)
		return false;				// invalid or duplicate name

	// set the actual DataItem's data using the contents of the ByteBuf.
	// we must copy it if we want to keep it.  bBase64 is TRUE if the
	// data is Base64 encoded.

	UT_ASSERT(pByteBuf);

	struct _dataItemPair* pPair = NULL;

	UT_ByteBuf * pNew = new UT_ByteBuf();
	if (!pNew)
		return false;

	if (bBase64)
	{
		if (!UT_Base64Decode(pNew,pByteBuf))
			goto Failed;
	}
	else
	{
		if (!pNew->ins(0,pByteBuf->getPointer(0),pByteBuf->getLength()))
			goto Failed;
	}

	pPair = new _dataItemPair();
	if (!pPair)
	{
		goto Failed;
	}

	pPair->pBuf = pNew;
	pPair->pToken = pToken;
	m_hashDataItems.insert(szName, (void *)pPair);

	// give them back a handle if they want one

	if (ppHandle)
	{
		const void *pHashEntry = m_hashDataItems.pick(szName);
		UT_ASSERT(pHashEntry);
		*ppHandle = (void *)pHashEntry;
	}

	return true;

Failed:
	if (pNew)
	{
		delete pNew;
	}

	// we also have to free the pToken, which was created by UT_strdup
	FREEP(pToken);

	return false;
}

bool PD_Document::getDataItemDataByName(const char * szName,
										   const UT_ByteBuf ** ppByteBuf,
										   void** ppToken,
										   void ** ppHandle) const
{
	UT_ASSERT(szName && *szName);

	const void *pHashEntry = m_hashDataItems.pick(szName);
	if (!pHashEntry)
		return false;

	struct _dataItemPair* pPair = (struct _dataItemPair*) pHashEntry;
	UT_ASSERT(pPair);

	if (ppByteBuf)
	{
		*ppByteBuf = pPair->pBuf;
	}

	if (ppToken)
	{
		*ppToken = pPair->pToken;
	}

	if (ppHandle)
	{
		*ppHandle = (void *)pHashEntry;
	}

	return true;
}

bool PD_Document::setDataItemToken(void * pHandle,
									  void* pToken)
{
	UT_ASSERT(pHandle);

	struct _dataItemPair* pPair = (struct _dataItemPair*) pHandle;
	UT_ASSERT(pPair);

	pPair->pToken = pToken;

	return true;
}

bool PD_Document::getDataItemData(void * pHandle,
									 const char ** pszName,
									 const UT_ByteBuf ** ppByteBuf,
									 void** ppToken) const
{
	UT_ASSERT(pHandle);

	struct _dataItemPair* pPair = (struct _dataItemPair*) pHandle;
	UT_ASSERT(pPair);

	if (ppByteBuf)
	{
		*ppByteBuf = pPair->pBuf;
	}

	if (ppToken)
	{
		*ppToken = pPair->pToken;
	}

	if (pszName)
	{
		UT_ASSERT(UT_TODO);
		*pszName = 0;
		//*pszName = pHashEntry->pszLeft;
	}

	return true;
}

bool PD_Document::enumDataItems(UT_uint32 k,
								   void ** ppHandle, const char ** pszName, const UT_ByteBuf ** ppByteBuf, void** ppToken) const
{
	// return the kth data item.

	UT_uint32 kLimit = m_hashDataItems.size();
	if (k >= kLimit)
		return false;

	UT_StringPtrMap::UT_Cursor c(&m_hashDataItems);
	const void *pHashEntry = NULL;
	UT_uint32 i;

	for (i = 0, pHashEntry = c.first();
	     c.is_valid() && i < k; i++, pHashEntry = c.next())
	  {
	    // noop
	  }

	if (ppHandle && c.is_valid())
		*ppHandle = (void *)pHashEntry;

	struct _dataItemPair* pPair = (struct _dataItemPair*)pHashEntry;
	UT_ASSERT(pPair);

	if (ppByteBuf)
	{
		*ppByteBuf = pPair->pBuf;
	}

	if (ppToken)
	{
		*ppToken = pPair->pToken;
	}

	if (pszName)
	{
		*pszName = c.key().c_str();
	}

	return true;
}

void PD_Document::_destroyDataItemData(void)
{
	if (m_hashDataItems.size() == 0)
		return;

	UT_StringPtrMap::UT_Cursor c(&m_hashDataItems);
	const void *val = NULL;

	for (val = c.first(); c.is_valid(); val = c.next())
	  {
		xxx_UT_DEBUGMSG(("DOM: destroying data item\n"));
		struct _dataItemPair* pPair = (struct _dataItemPair*) val;
		UT_ASSERT(pPair);
		UT_String key = c.key();
		m_hashDataItems.remove (key, NULL);
		delete pPair->pBuf;
		FREEP(pPair->pToken);
		delete pPair;
	}
}

/*!
  Synchronize the last opened/last saves filetypes.
 \param bReadLastSavedAsType True to write last opened and read last
           saved type; otherwise, write last saved type from last opened type.

 There are actually two filetypes - one for importers and one for
 exporters.  This function tries to synchronize the one to the other.
*/
bool PD_Document::_syncFileTypes(bool bReadSaveWriteOpen)
{
	const char *szSuffixes;

	if (bReadSaveWriteOpen)
	  szSuffixes = IE_Exp::suffixesForFileType(m_lastSavedAsType);
	else
	  szSuffixes = IE_Imp::suffixesForFileType(m_lastOpenedType);

	if (!szSuffixes)
	  return false;

	// Pull first suffix from the file dialog pattern string
	UT_String suffix;
	for (const char *p = szSuffixes; *p && *p != ';'; ++p)
		if (*p != '*')
			suffix += *p;

	IEFileType ieft;
	if (bReadSaveWriteOpen)
	{
		ieft = IE_Imp::fileTypeForSuffix(suffix.c_str());
		m_lastOpenedType = ieft;
	}
	else
	{
		ieft = IE_Exp::fileTypeForSuffix(suffix.c_str());
		m_lastSavedAsType = ieft;
	}

	if (ieft == IEFT_Unknown || ieft == IEFT_Bogus)
	{
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// Styles represent named collections of formatting properties.

bool PD_Document::getStyle(const char * szName, PD_Style ** ppStyle) const
{
	return m_pPieceTable->getStyle(szName, ppStyle);
}

bool PD_Document::enumStyles(UT_uint32 k,
								const char ** pszName, const PD_Style ** ppStyle) const
{
	return m_pPieceTable->enumStyles(k, pszName, ppStyle);
}

bool	PD_Document::addStyleProperty(const char * szStyleName, const char * szPropertyName, const char * szPropertyValue)
{
	PD_Style * pS;
	PD_Style ** ppS = &pS;
	if(!m_pPieceTable->getStyle(szStyleName, ppS))
		return false;

	return (*ppS)->addProperty(szPropertyName, szPropertyValue);
}

bool	PD_Document::addStyleProperties(const XML_Char * szStyleName, const XML_Char ** pProperties)
{
	PD_Style * pS;
	PD_Style ** ppS = &pS;
	if(!m_pPieceTable->getStyle(szStyleName, ppS))
		return false;
	if(!(*ppS)->addProperties(pProperties))
		return false;
	return updateDocForStyleChange(szStyleName,!(*ppS)->isCharStyle());
}

/*!
 * This methods changes the attributes of a style (basedon,followedby)
 *
\param szStyleName the const XML_Char * name of the style
\param pAttribs The list of attributes of the updated style.
*/
bool	PD_Document::addStyleAttributes(const XML_Char * szStyleName, const XML_Char ** pAttribs)
{
	PD_Style * pS;
	PD_Style ** ppS = &pS;
	if(!m_pPieceTable->getStyle(szStyleName, ppS))
		return false;
	if(!(*ppS)->addAttributes(pAttribs))
		return false;
//
// These functions just set the new member variable pointers in the class
//
	(*ppS)->getBasedOn();
	(*ppS)->getFollowedBy();
	return updateDocForStyleChange(szStyleName,!(*ppS)->isCharStyle());
}

/*!
 * The method returns the style defined in a sdh. If there is no style it returns
 * NULL
 */
PD_Style * PD_Document::getStyleFromSDH( PL_StruxDocHandle sdh)
{
	pf_Frag_Strux * pfs = (pf_Frag_Strux *) sdh;
	PT_AttrPropIndex indexAP = pfs->getIndexAP();
	const PP_AttrProp * pAP = NULL;
	m_pPieceTable->getAttrProp(indexAP,&pAP);
	UT_ASSERT(pAP);
	const XML_Char * pszStyleName = NULL;
	(pAP)->getAttribute(PT_STYLE_ATTRIBUTE_NAME, pszStyleName);
	if(pszStyleName == NULL  || UT_strcmp(pszStyleName,"Current Settings") == 0 || UT_strcmp(pszStyleName,"None") == 0)
	{
		return NULL;
	}
	PD_Style * pStyle = NULL;
	if(!m_pPieceTable->getStyle(pszStyleName, &pStyle))
	{
		return NULL;
	}
	return pStyle;
}

/*!
 * Find previous style of type numbered heading or basedon numbered heading
\params sdh The StruxDocHandle of the fragment where we start to look from.
\returns PD_Style of the first Numbered Heading, otherwise NULL
*/
PL_StruxDocHandle PD_Document::getPrevNumberedHeadingStyle(PL_StruxDocHandle sdh)
{
	pf_Frag * pf = (pf_Frag_Strux *) sdh;
	bool bFound = false;
	pf = pf->getPrev();
	PD_Style * pStyle = NULL;
	PL_StruxDocHandle foundSDH = NULL;
	PD_Style * pBasedOn = NULL;
	const char * szStyleName = NULL;
	while(pf && !bFound)
	{
		if(pf->getType() == pf_Frag::PFT_Strux)
		{
			foundSDH = (PL_StruxDocHandle) pf;
			pStyle = getStyleFromSDH(foundSDH);
			if(pStyle != NULL)
			{
				szStyleName = pStyle->getName();
				if(strstr(szStyleName,"Numbered Heading") != 0)
				{
					bFound = true;
					break;
				}
				pBasedOn  = pStyle->getBasedOn();
				UT_uint32 i = 0;
				while(pBasedOn != NULL && i < 10 && !bFound)
				{
					if(strstr(pBasedOn->getName(),"Numbered Heading") != 0)
					{
						bFound = true;
					}
					else
					{
						pBasedOn = pBasedOn->getBasedOn();
					}
				}
				if(bFound)
				{
					break;
				}
			}
		}
//
// Should not need the if. It's in for defensive programming.
//
		if(!bFound)
		{
			pf = pf->getPrev();
		}
	}
	if(!bFound)
	{
		return NULL;
	}
	return foundSDH;
}



//
/*!
 * This methods changes the attributes /properties of a style (basedon,followedby)
 * plus the properties. We have to save the indexAP of the pre-existing style
 * and broadcast it out witht e change records.
 *
\param szStyleName the const XML_Char * name of the style
\param pAttribs The list of attributes/properties of the updated style.
*/
bool	PD_Document::setAllStyleAttributes(const XML_Char * szStyleName, const XML_Char ** pAttribs)
{
	PD_Style * pS;
	PD_Style ** ppS = &pS;
	if(!m_pPieceTable->getStyle(szStyleName, ppS))
		return false;
//
// Sevior May need this code
//	PT_AttrPropIndex oldindexAp = (*pss)->getIndexAP();
	if(!(*ppS)->setAllAttributes(pAttribs))
		return false;
//
// These functions just set the new member variable pointers in the class
//
	(*ppS)->getBasedOn();
	(*ppS)->getFollowedBy();
	return updateDocForStyleChange(szStyleName,!(*ppS)->isCharStyle());
}

/*!
 * This method scans the document backwards for a struc with the style name szStyle in it.
\params pStyle a pointer to style to be scanned for.
\params pos the document position to start from.
\return the sdh of the strux found.
*/
PL_StruxDocHandle PD_Document::findPreviousStyleStrux(const XML_Char * szStyle, PT_DocPosition pos)
{
	PL_StruxDocHandle sdh = NULL;
	getStruxOfTypeFromPosition(pos,PTX_Block, &sdh);
	pf_Frag_Strux * pfs = NULL;
	pf_Frag * currentFrag = (pf_Frag *) sdh;
	bool bFound = false;
    while (currentFrag != m_pPieceTable->getFragments().getFirst() && !bFound)
	{
		if (currentFrag->getType()==pf_Frag::PFT_Strux)
		{
//
// All this code is used to find if this strux has our style in it
//
			pfs = static_cast<pf_Frag_Strux *> (currentFrag);
			PT_AttrPropIndex indexAP = pfs->getIndexAP();
			const PP_AttrProp * pAP = NULL;
			m_pPieceTable->getAttrProp(indexAP,&pAP);
			UT_ASSERT(pAP);
			const XML_Char * pszStyleName = NULL;
			(pAP)->getAttribute(PT_STYLE_ATTRIBUTE_NAME, pszStyleName);
			if(pszStyleName != NULL && strcmp(pszStyleName,szStyle)==0)
			{
				bFound = true;
			}
		}
		if(!bFound)
		{
			currentFrag = currentFrag->getPrev();
		}
	}
	if(bFound)
	{
		sdh = (PL_StruxDocHandle) currentFrag;
	}
	else
	{
		sdh = NULL;
	}
	return sdh;
}

/*!
 * This method scans the document forwards for a strux with the style name
 * szStyle in it.
\params pStyle a pointer to style to be scanned for.
\params pos the document position to start from.
\return the sdh of the strux found.
*/
PL_StruxDocHandle PD_Document::findForwardStyleStrux(const XML_Char * szStyle, PT_DocPosition pos)
{
	PL_StruxDocHandle sdh = NULL;
	getStruxOfTypeFromPosition(pos,PTX_Block, &sdh);
	pf_Frag_Strux * pfs = NULL;
	pf_Frag * currentFrag = (pf_Frag *) sdh;
	bool bFound = false;
    while (currentFrag != m_pPieceTable->getFragments().getLast() && !bFound)
	{
		if (currentFrag->getType()==pf_Frag::PFT_Strux)
		{
//
// All this code is used to find if this strux has our style in it
//
			pfs = static_cast<pf_Frag_Strux *> (currentFrag);
			PT_AttrPropIndex indexAP = pfs->getIndexAP();
			const PP_AttrProp * pAP = NULL;
			m_pPieceTable->getAttrProp(indexAP,&pAP);
			UT_ASSERT(pAP);
			const XML_Char * pszStyleName = NULL;
			(pAP)->getAttribute(PT_STYLE_ATTRIBUTE_NAME, pszStyleName);
			if(pszStyleName != NULL && strcmp(pszStyleName,szStyle)==0)
			{
				bFound = true;
			}
		}
		if(!bFound)
		{
			currentFrag = currentFrag->getNext();
		}
	}
	if(bFound)
	{
		sdh = (PL_StruxDocHandle) currentFrag;
	}
	else
	{
		sdh = NULL;
	}
	return sdh;
}


/*!
 * This method loops through the entire document updating each location
 * where the style exists.
\param szStyle the name of style that has changed.
\param isParaStyle true if the style is a paragraph type.
*/
bool   PD_Document::updateDocForStyleChange(const XML_Char * szStyle,
											bool isParaStyle)
{
	PT_DocPosition pos = 0;
	PT_DocPosition posLastStrux = 0;
	pf_Frag_Strux * pfs = NULL;
	PD_Style * pStyle = NULL;
	m_pPieceTable->getStyle(szStyle,&pStyle);
	UT_ASSERT(pStyle);
	pf_Frag * currentFrag = m_pPieceTable->getFragments().getFirst();
	UT_ASSERT(currentFrag);
	while (currentFrag!=m_pPieceTable->getFragments().getLast())
	{
//
// get indexAP
// get PT_STYLE_ATTRIBUTE_NAME
// if it matches style name do a notify listeners call.
		if(isParaStyle)
		{
			if (currentFrag->getType()==pf_Frag::PFT_Strux)
			{
//
// All this code is used to find if this strux has our style in it
//
				pfs = static_cast<pf_Frag_Strux *> (currentFrag);
				PT_AttrPropIndex indexAP = pfs->getIndexAP();
				const PP_AttrProp * pAP = NULL;
				m_pPieceTable->getAttrProp(indexAP,&pAP);
				UT_ASSERT(pAP);
				const XML_Char * pszStyleName = NULL;
				(pAP)->getAttribute(PT_STYLE_ATTRIBUTE_NAME, pszStyleName);
				bool bUpdate = false;
//
// It does so signal all the layouts to update themselves for the new definition
// of the style.
//
				if(pszStyleName != NULL && strcmp(pszStyleName,szStyle)==0)
				{
					bUpdate = true;
				}
//
// Look if the style in the basedon ancestory is our style
//
				else if (pszStyleName != NULL)
				{
					PD_Style * cStyle = NULL;
					m_pPieceTable->getStyle(pszStyleName,&cStyle);
					UT_ASSERT(cStyle);
					if(cStyle)
					{
						PD_Style * pBasedOn = cStyle->getBasedOn();
						UT_uint32 i =0;
						for(i=0; (i < pp_BASEDON_DEPTH_LIMIT) && (pBasedOn != NULL) && (pBasedOn!= pStyle); i++)
						{
							pBasedOn = pBasedOn->getBasedOn();
						}
						if(pBasedOn == pStyle)
						{
							bUpdate = true;
						}
					}
				}
				if(bUpdate)
				{
					PX_ChangeRecord * pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_ChangeStrux,pos,indexAP);
					notifyListeners(pfs, pcr);
					delete pcr;
				}
			}
		}
//
// Character type
//
		else
		{
//
// Need the most recent frag_strux to find the block containing our text span
//
			if (currentFrag->getType()==pf_Frag::PFT_Strux)
			{
				pfs = static_cast<pf_Frag_Strux *> (currentFrag);
				posLastStrux = pos;
			}
			if (currentFrag->getType()==pf_Frag::PFT_Text)
			{
//
// All this code is used to find if this Text Frag has our style in it
//
				pf_Frag_Text * pft = static_cast<pf_Frag_Text *> (currentFrag);
				PT_AttrPropIndex indexAP = pft->getIndexAP();
				const PP_AttrProp * pAP = NULL;
				m_pPieceTable->getAttrProp(indexAP,&pAP);
				UT_ASSERT(pAP);
				const XML_Char * pszStyleName = NULL;
				(pAP)->getAttribute(PT_STYLE_ATTRIBUTE_NAME, pszStyleName);

//
// It does so signal all the layouts to update themselves for the new definition
// of the style.
//
				if(pszStyleName != NULL && strcmp(pszStyleName,szStyle)==0)
				{
					UT_uint32 blockoffset = (UT_uint32) (pos - posLastStrux -1);
					PX_ChangeRecord_SpanChange * pcr = new PX_ChangeRecord_SpanChange(PX_ChangeRecord::PXT_ChangeSpan,pos,indexAP,indexAP, m_pPieceTable->getVarSet().getBufIndex(pft->getBufIndex(),0) ,currentFrag->getLength(),blockoffset);
					notifyListeners(pfs, pcr);
					delete pcr;
				}
			}
		}
		pos += currentFrag->getLength();
		currentFrag = currentFrag->getNext();
	}
	return true;
}


/*!
 * This method updates all the layouts associated with the document.
*/
void  PD_Document::updateAllLayoutsInDoc( PL_StruxDocHandle sdh)
{
	pf_Frag_Strux * pfs = (pf_Frag_Strux *) sdh;
	PT_AttrPropIndex indexAP = pfs->getIndexAP();
	PT_DocPosition pos = getStruxPosition(sdh);
	PX_ChangeRecord * pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_ChangeStrux,
												pos,indexAP);
	notifyListeners(pfs, pcr);
	delete pcr;
}

//////////////////////////////////////////////////////////////////

void PD_Document::clearIfAtFmtMark (PT_DocPosition dpos)
{
	m_pPieceTable->clearIfAtFmtMark(dpos);
}

bool PD_Document::updateFields(void)
{
	//
	// Turn off Insertion point motion during this general update
	//
	setDontChangeInsPoint();
	pf_Frag * currentFrag = m_pPieceTable->getFragments().getFirst();
	UT_ASSERT(currentFrag);
	while (currentFrag!=m_pPieceTable->getFragments().getLast())
	{
		if (currentFrag->getType()==pf_Frag::PFT_Object)
		{
			pf_Frag_Object * pfo = static_cast<pf_Frag_Object *>
				(currentFrag);
			if (pfo->getObjectType()==PTO_Field)
			{
				UT_ASSERT (pfo->getField());
				pfo->getField()->update();
			}
		}
		currentFrag = currentFrag->getNext();
	}
	//
	// Restore insertion point motion
	//
	allowChangeInsPoint();
	return true;
}

void PD_Document::setDontChangeInsPoint(void)
{
	if(m_bLoading)
	{
		UT_DEBUGMSG(("Illegal request to not change insertion Point!!! \n"));
        m_bAllowInsertPointChange = true;
		return;
	}
	m_bAllowInsertPointChange = false;
}

void PD_Document::allowChangeInsPoint(void)
{
        m_bAllowInsertPointChange = true;
}

bool PD_Document::getAllowChangeInsPoint(void) const
{
        return m_bAllowInsertPointChange;
}

////////////////////////////////////////////////////////////////////////////////
// Step towards full thread safety

void PD_Document::notifyPieceTableChangeStart(void)
{
//
// Wait for all redraws to finish before starting.
//
	UT_uint32 i = 0;
	while(m_bRedrawHappenning && i < 10000)
	{
		UT_usleep(100); // wait 100 microseonds
		i++;
	}
	if(i>0)
	{
		UT_DEBUGMSG(("!!!!Waited %d microseconds for redraw to finish \n",i*100));
	}
	m_bRedrawHappenning = false;
	m_bPieceTableChanging = true;
}


void PD_Document::notifyPieceTableChangeEnd(void)
{
        m_bPieceTableChanging = false;
}

bool PD_Document::isPieceTableChanging(void)
{
        return m_bPieceTableChanging;
}

////////////////////////////////////////////////////////////////
// List Vector Functions



fl_AutoNum * PD_Document::getListByID(UT_uint32 id) const
{
	UT_uint16 i = 0;
	UT_sint32 cnt = 0;
	fl_AutoNum * pAutoNum;

	cnt = m_vecLists.getItemCount();
	if ( cnt <= 0)
		return (fl_AutoNum *) NULL;
	UT_ASSERT(m_vecLists.getFirstItem());

	while (i<cnt)
	{
		pAutoNum = (fl_AutoNum *)m_vecLists[i];
		if (pAutoNum->getID() == id)
			return pAutoNum;
		i++;
	}

	return (fl_AutoNum *) NULL;
}

bool PD_Document::enumLists(UT_uint32 k, fl_AutoNum ** pAutoNum)
{
	UT_uint32 kLimit = m_vecLists.getItemCount();
	if (k >= kLimit)
		return false;

	if (pAutoNum)
		*pAutoNum = (fl_AutoNum *)m_vecLists[k];

	return true;
}

fl_AutoNum * PD_Document::getNthList(UT_uint32 i) const
{
	UT_ASSERT(i >= 0);
	return (fl_AutoNum *)m_vecLists[i];
}

UT_uint32 PD_Document::getListsCount(void) const
{
	return m_vecLists.getItemCount();
}

void PD_Document::addList(fl_AutoNum * pAutoNum)
{
	UT_uint32 id = pAutoNum->getID();
	UT_uint32 i;
	UT_uint32 numlists = m_vecLists.getItemCount();
	for(i=0; i < numlists; i++)
	{
		fl_AutoNum * pAuto = (fl_AutoNum *) m_vecLists.getNthItem(i);
		if(pAuto->getID() == id)
			break;
	}
	if(i >= numlists)
		m_vecLists.addItem(pAutoNum);
}

void PD_Document::listUpdate(PL_StruxDocHandle sdh )
{
	//
	// Notify all views of a listupdate
	//
	UT_ASSERT(sdh);
	pf_Frag_Strux * pfs = (pf_Frag_Strux *) sdh;
	PT_AttrPropIndex pAppIndex = pfs->getIndexAP();
	PT_DocPosition pos = getStruxPosition(sdh);
#ifndef __MRC__
	const PX_ChangeRecord * pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_ListUpdate,pos,pAppIndex);
#else
	PX_ChangeRecord * pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_ListUpdate,pos,pAppIndex);
#endif
	notifyListeners(pfs, pcr);
	delete pcr;
}


void PD_Document::StopList(PL_StruxDocHandle sdh )
{
	//
	// Notify all views of a stoplist
	//
	setHasListStopped(false);
	pf_Frag_Strux * pfs = (pf_Frag_Strux *) sdh;
	PT_AttrPropIndex pAppIndex = pfs->getIndexAP();
	PT_DocPosition pos = getStruxPosition(sdh);
#ifndef __MRC__
	const PX_ChangeRecord * pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_StopList,pos,pAppIndex);
#else
	PX_ChangeRecord * pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_StopList,pos,pAppIndex);
#endif
	notifyListeners(pfs, pcr);
	delete pcr;
	setHasListStopped(false);
}


bool PD_Document::appendList(const XML_Char ** attributes)
{
	const XML_Char * szID=NULL, * szPid=NULL, * szType=NULL, * szStart=NULL, * szDelim=NULL, *szDec=NULL;
	UT_uint32 id, parent_id, start;
	List_Type type;

	for (const XML_Char ** a = attributes; (*a); a++)
	{
		if (UT_XML_stricmp(a[0],"id") == 0)
			szID = a[1];
		else if (UT_XML_stricmp(a[0], "parentid") == 0)
			szPid = a[1];
		else if (UT_XML_stricmp(a[0], "type") == 0)
			szType = a[1];
		else if (UT_XML_stricmp(a[0], "start-value") == 0)
			szStart = a[1];
		else if (UT_XML_stricmp(a[0], "list-delim") == 0)
			szDelim = a[1];
		else if (UT_XML_stricmp(a[0], "list-decimal") == 0)
			szDec = a[1];
	}

	if(!szID)
		return false;
	if(!szPid)
		return false;
	if(!szType)
		return false;
	if(!szStart)
		return false;
	if(!szDelim)
		return false;
	if(!szDec)
		szDec = (const XML_Char *) ".";
	id = atoi(szID);
	UT_uint32 i;
	UT_uint32 numlists = m_vecLists.getItemCount();
	for(i=0; i < numlists; i++)
	{
		fl_AutoNum * pAuto = (fl_AutoNum *) m_vecLists.getNthItem(i);
		if(pAuto->getID() == id)
			break;
	}
	if(i < numlists)
		return true; // List is already present
	parent_id = atoi(szPid);
	type = (List_Type)atoi(szType);
	start = atoi(szStart);

	fl_AutoNum * pAutoNum = new fl_AutoNum(id, parent_id, type, start, szDelim,szDec,this);
	addList(pAutoNum);

	return true;
}

bool PD_Document::areListUpdatesAllowed(void)
{
        return m_ballowListUpdates;
}

void PD_Document::disableListUpdates(void)
{
        m_ballowListUpdates = false;
}

void PD_Document::enableListUpdates(void)
{
        m_ballowListUpdates = true;
}

void PD_Document::updateDirtyLists(void)
{
	UT_uint32 iNumLists = m_vecLists.getItemCount();
	UT_uint32 i;
	fl_AutoNum * pAutoNum;
	bool bDirtyList = false;
	for(i=0; i< iNumLists; i++)
	{
		pAutoNum = (fl_AutoNum *) m_vecLists.getNthItem(i);
		if(pAutoNum->isEmpty())
		{
			delete pAutoNum;
			m_vecLists.deleteNthItem(i);
			iNumLists--;
			i--;
		}
	}
	for(i=0; i< iNumLists; i++)
	{
		pAutoNum = (fl_AutoNum *) m_vecLists.getNthItem(i);
		if(pAutoNum->isDirty() == true)
		{
			pAutoNum->update(0);
			bDirtyList = true;
		}
	}
	if(bDirtyList)
	{
		for(i=0; i< iNumLists; i++)
		{
			pAutoNum = (fl_AutoNum *) m_vecLists.getNthItem(i);
			pAutoNum->fixHierarchy();
			pAutoNum->findAndSetParentItem();
		}
	}
}


bool PD_Document::fixListHierarchy(void)
{
	UT_uint32 iNumLists = m_vecLists.getItemCount();
	fl_AutoNum * pAutoNum;

	if (iNumLists == 0)
	{
		return false;
	}
	else
	{
		for (UT_uint32 i = 0; i < iNumLists; i++)
		{
			pAutoNum = (fl_AutoNum *)m_vecLists.getNthItem(i);
			pAutoNum->fixHierarchy();
		}
		return true;
	}
}

void PD_Document::removeList(fl_AutoNum * pAutoNum, PL_StruxDocHandle sdh )
{
	UT_ASSERT(pAutoNum);
	UT_sint32 ndx = m_vecLists.findItem(pAutoNum);
	UT_ASSERT(ndx >= 0);
	if (ndx != -1)
	{
		//
		// Notify all views of a remove List
		//
		pf_Frag_Strux * pfs = (pf_Frag_Strux *) sdh;
		PT_AttrPropIndex pAppIndex = pfs->getIndexAP();
		PT_DocPosition pos = getStruxPosition(sdh);
#ifndef __MRC__
		const PX_ChangeRecord * pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_RemoveList,pos,pAppIndex);
#else
		PX_ChangeRecord * pcr = new PX_ChangeRecord(PX_ChangeRecord::PXT_RemoveList,pos,pAppIndex);
#endif
		notifyListeners(pfs, pcr);
		delete pcr;
		m_vecLists.deleteNthItem(ndx);
	}
}

void  PD_Document::setDoingPaste(void)
{
         m_bDoingPaste = true;
}


void  PD_Document::clearDoingPaste(void)
{
         m_bDoingPaste = false;
}


bool  PD_Document::isDoingPaste(void)
{
         return m_bDoingPaste;
}

void     PD_Document::setDefaultPageSize(void)
{
	XAP_App *pApp = XAP_App::getApp();
	UT_ASSERT(pApp);

	const XML_Char * szDefaultPageSize = NULL;
	pApp->getPrefsValue(XAP_PREF_KEY_DefaultPageSize,
	                      &szDefaultPageSize);
	UT_ASSERT((szDefaultPageSize) && (*szDefaultPageSize));
	UT_ASSERT(sizeof(char) == sizeof(XML_Char));
	m_docPageSize.Set(  (const char *) szDefaultPageSize);
}

const char *  PD_Document::getDefaultPageSize(void)
{
	XAP_App *pApp = XAP_App::getApp();
	UT_ASSERT(pApp);

	const XML_Char * szDefaultPageSize = NULL;
	pApp->getPrefsValue(XAP_PREF_KEY_DefaultPageSize,
	                      &szDefaultPageSize);
	UT_ASSERT((szDefaultPageSize) && (*szDefaultPageSize));
	UT_ASSERT(sizeof(char) == sizeof(XML_Char));
	return (const char *) szDefaultPageSize;
}

bool PD_Document:: setPageSizeFromFile(const XML_Char ** attributes)
{
	const XML_Char * szPageSize=NULL, * szOrientation=NULL, * szWidth=NULL, * szHeight=NULL, * szUnits=NULL, * szPageScale=NULL;
	double width=0.0;
	double height=0.0;
	double scale =1.0;
	UT_Dimension u = DIM_IN;

	for (const XML_Char ** a = attributes; (*a); a++)
	{
		if (UT_XML_stricmp(a[0],"pagetype") == 0)
		        szPageSize = a[1];
		else if (UT_XML_stricmp(a[0], "orientation") == 0)
			szOrientation = a[1];
		else if (UT_XML_stricmp(a[0], "width") == 0)
			szWidth = a[1];
		else if (UT_XML_stricmp(a[0], "height") == 0)
			szHeight = a[1];
		else if (UT_XML_stricmp(a[0], "units") == 0)
			szUnits = a[1];
		else if (UT_XML_stricmp(a[0], "page-scale") == 0)
			szPageScale = a[1];
	}

	if(!szPageSize)
		return false;
	if(!szOrientation)
		return false;
	m_docPageSize.Set((const char *)szPageSize);

	if( szWidth && szHeight && szUnits && szPageScale)
	  {
		if(UT_XML_stricmp(szPageSize,"Custom") == 0)
		  {
		    width = UT_convertDimensionless(szWidth);
		    height = UT_convertDimensionless(szHeight);
		    if(UT_XML_stricmp(szUnits,"cm") == 0)
		      u = DIM_CM;
		    else if(UT_XML_stricmp(szUnits,"mm") == 0)
		      u = DIM_MM;
		    else if(UT_XML_stricmp(szUnits,"inch") == 0)
		      u = DIM_IN;
		    m_docPageSize.Set(width,height,u);
		  }

		scale =  UT_convertDimensionless(szPageScale);
		m_docPageSize.setScale(scale);
	  }

	// set portrait by default
	m_docPageSize.setPortrait();

	// custom page sizes are always in "Portrait" mode
	if ( UT_XML_stricmp(szPageSize,"Custom") != 0 )
	  {
	    if( UT_XML_stricmp(szOrientation,"landscape") == 0 )
	      {
		m_docPageSize.setLandscape();
	      }
	  }

	return true;
}

void PD_Document::addBookmark(const XML_Char * pName)
{
	m_vBookmarkNames.addItem((void*)pName);
}

void PD_Document::removeBookmark(const XML_Char * pName)
{
	for(UT_uint32 i = 0; i < m_vBookmarkNames.getItemCount(); i++)
	{
		const XML_Char * pBM =  (const XML_Char *) m_vBookmarkNames.getNthItem(i);
		if(!UT_XML_strcmp(pName, pBM))
		{
			m_vBookmarkNames.deleteNthItem(i);
			break;
		}
	}
}

/*! Returns true if pName doesn't correspond to a
 *  currently existing bookmark. */
bool PD_Document::isBookmarkUnique(const XML_Char * pName) const
{
	for(UT_uint32 i = 0; i < m_vBookmarkNames.getItemCount(); i++)
	{
		const XML_Char * pBM =  (const XML_Char *) m_vBookmarkNames.getNthItem(i);
		if(!UT_XML_strcmp(pName, pBM))
			return false;
	}
	return true;
}

/*! Returns true if pName looks like a relative link, rather than a
 *  bookmark.

 *  Current heuristic: if pName contains a ., then it's a rel link;
 * otherwise it's a bookmark. */
bool PD_Document::isBookmarkRelativeLink(const XML_Char * pName) const
{
	UT_ASSERT(sizeof(char) == sizeof(XML_Char));
	return strchr((char *)pName, '.') != NULL;
}

//////////////////////////////////////////////////////////////////
// document-level properties

#define VARSET m_pPieceTable->getVarSet()

const PP_AttrProp * PD_Document::getAttrProp() const
{
	return VARSET.getAP(m_indexAP);
}

bool PD_Document::setAttrProp(const XML_Char ** ppAttr)
{
	// this method can only be used while loading  ...
	if(m_pPieceTable->getPieceTableState() != PTS_Loading)
	{
		UT_return_val_if_fail(0,false);
	}

	bool bRet = VARSET.storeAP(ppAttr, &m_indexAP);

	if(!bRet)
		return false;

	const XML_Char * attr[21];
	attr[20] = NULL;

	attr[0] = "xmlns";
	attr[1] = "http://www.abisource.com/awml.dtd";

	attr[2] = "xml:space";
	attr[3] = "preserve";

	attr[4] = "xmlns:awml";
	attr[5] = "http://www.abisource.com/awml.dtd";

	attr[6] = "xmlns:xlink";
	attr[7] = "http://www.w3.org/1999/xlink";

	attr[8] = "xmlns:svg";
	attr[9] = "http://www.w3.org/2000/svg";

	attr[10] = "xmlns:fo";
	attr[11] = "http://www.w3.org/1999/XSL/Format";

	attr[12] = "xmlns:math";
	attr[13] = "http://www.w3.org/1998/Math/MathML";

	attr[14] = "xmlns:dc";
	attr[15] = "http://purl.org/dc/elements/1.1/";

	attr[16] = "fileformat";
	attr[17] = ABIWORD_FILEFORMAT_VERSION;

	if (XAP_App::s_szBuild_Version && XAP_App::s_szBuild_Version[0])
	{
		attr[18] = "version";
		attr[19] = XAP_App::s_szBuild_Version;
	}
	else
		attr[18] = NULL;

	bRet =  setAttributes(attr);
	if(!bRet)
		return false;


	// see if the document has dominant direction set and if not, set
	// dominant direction from preferences
	const PP_AttrProp * docAP =  getAttrProp();
	const XML_Char * doc_dir;

	if(docAP && !docAP->getProperty("dom-dir", doc_dir))
	{
		const XML_Char r[] = "rtl";
		const XML_Char l[] = "ltr";
		const XML_Char p[] = "dom-dir";
		const XML_Char * props[3] = {p,l,NULL};

		bool bRTL = false;
		XAP_App::getApp()->getPrefs()->getPrefsValueBool(AP_PREF_KEY_DefaultDirectionRtl,&bRTL);

		if(bRTL)
			props[1] = r;

		UT_DEBUGMSG(( "pd_Document::setAttrProp: setting dom-dir to %s\n", props[1]));

		bRet = setProperties(props);


		if(!bRet)
			return false;
	}
	else
	{
		UT_DEBUGMSG(( "pd_Document::setAttrProp: document has default direction %s\n", doc_dir));
	}

	// see if the document we are loading has a default language;
	// if not, and there is a default language in the preferences, set
	// it
	const XML_Char * doc_lang;

	if(docAP && !docAP->getProperty("lang", doc_lang))
	{
		const XML_Char * doc_locale = NULL;
		if (XAP_App::getApp()->getPrefs()->getPrefsValue(XAP_PREF_KEY_DocumentLocale,&doc_locale) && doc_locale)
		{
			const XML_Char * props[3];
			props[0] = "lang";
			props[1] = doc_locale;
			props[2] = 0;
			UT_DEBUGMSG(( "pd_Document::setAttrProp: setting lang to %s\n", doc_locale ));

			bRet = setProperties(props);
		}
	}
	else
	{
		UT_DEBUGMSG(( "pd_Document::setAttrProp: document has default language %s\n", doc_lang));
	}


	return bRet;
}

bool PD_Document::setAttributes(const XML_Char ** ppAttr)
{
	return VARSET.mergeAP(PTC_AddFmt, m_indexAP, ppAttr, NULL, &m_indexAP, this);
}


bool PD_Document::setProperties(const XML_Char ** ppProps)
{
	return VARSET.mergeAP(PTC_AddFmt, m_indexAP, NULL, ppProps, &m_indexAP, this);
}

#undef VARSET

void PD_Document::lockStyles(bool b)
{
	const XML_Char *attr[3];
	const XML_Char n[] = "styles";
	const XML_Char v1[] = "locked";
	const XML_Char v2[] = "unlocked";

	attr[0] = n;
	attr[2] = NULL;

	if(b)
		attr[1] = v1;
	else
		attr[1] = v2;

	setAttributes(attr);
	m_bLockedStyles = b;
}


