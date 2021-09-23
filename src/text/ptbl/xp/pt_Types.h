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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef PT_TYPES_H
#define PT_TYPES_H

#include "ut_types.h"
#include "ut_static_string.h"

//! index to actual document data
typedef UT_uint32 PT_BufIndex;
//! index to Attribute/Property Tables
typedef UT_uint32 PT_AttrPropIndex;

//! absolute document position
typedef UT_uint32 PT_DocPosition;
//! block-relative document position
typedef UT_uint32 PT_BlockOffset;

class PD_Document;
/*!
 PD_DocumentRange identifies a piece of the document, using two
 PT_DocPosition absolute document positions as limits.
*/

class ABI_EXPORT PD_DocumentRange
{
public:
	//! Dummy constructor.
	//! Resulting range is invalid until it is initialized with set
	PD_DocumentRange(void)
		: m_pDoc(NULL), m_pos1(0), m_pos2(0)
		{
		}

	//! Initializing constructor.
	PD_DocumentRange(PD_Document * pDoc, PT_DocPosition k1, PT_DocPosition k2)
		: m_pDoc(pDoc), m_pos1(k1), m_pos2(k2)
		{
		}

	//! Set range limits
	void set(PD_Document * pDoc, PT_DocPosition k1, PT_DocPosition k2)
		{
			m_pDoc = pDoc;
			m_pos1 = k1;
			m_pos2 = k2;
		}

	//! Document this range is in
	PD_Document *		m_pDoc;
	//! Lower limit of range
	PT_DocPosition		m_pos1;
	//! Upper limit of range
	PT_DocPosition		m_pos2;
};

//! PTStruxType tells the sub-type of a FragStrux.
enum PTStruxType
{
	PTX_Section = 0,       	// 0 -- maker sure that we can cast into uint
	PTX_Block,           	// 1
	PTX_SectionHdrFtr,   	// 2
	PTX_SectionEndnote,  	// 3
	PTX_SectionTable,    	// 4
	PTX_SectionCell,     	// 5
	PTX_SectionFootnote, 	// 6
	PTX_SectionMarginnote, 	// 7
	PTX_SectionAnnotation,  // 8
	PTX_SectionFrame,      	// 9
	PTX_SectionTOC,			// 10
	PTX_EndCell,          	// 11
	PTX_EndTable,
        PTX_EndFootnote,
        PTX_EndMarginnote,
        PTX_EndEndnote,
	PTX_EndAnnotation,
        PTX_EndFrame,
	PTX_EndTOC,
	PTX_StruxDummy
};

//! PTObjectType tells the sub-type of an FragObject.

enum PTObjectType
{
    PTO_Image = 0,
    PTO_Field,
    PTO_Bookmark,
    PTO_Hyperlink,
    PTO_Math,
    PTO_Embed,
    PTO_Annotation,
    PTO_RDFAnchor
};

//! PieceTable states
enum PTState { PTS_Create=0, PTS_Loading=1, PTS_Editing=2 };
//! ChangeFormat types
enum PTChangeFmt
{
  PTC_AddFmt = 0,
  PTC_RemoveFmt = 1,
  PTC_AddStyle = 2,
  PTC_SetFmt = 3,
  PTC_SetExactly = 4
};

enum PLListenerType
{
	PTL_UNKNOWN,
	PTL_DocLayout,
	PTL_CollabExport,
	PTL_CollabServiceExport
	/* add more types here ONLY as necessary */
};

//! ID of a listener - this is its location in m_vecListeners
typedef UT_uint32 PL_ListenerId;

typedef UT_StaticString PP_PropName;
#define _PN(x) PP_PropName(x)

#define PT_PROPS_ATTRIBUTE_NAME			(_PN("props"))
#define PT_STYLE_ATTRIBUTE_NAME			(_PN("style"))
#define PT_LEVEL_ATTRIBUTE_NAME			(_PN("level"))
#define PT_LISTID_ATTRIBUTE_NAME		(_PN("listid"))
#define PT_PARENTID_ATTRIBUTE_NAME		(_PN("parentid"))
#define PT_NAME_ATTRIBUTE_NAME			(_PN("name"))
#define PT_TYPE_ATTRIBUTE_NAME			(_PN("type"))
#define PT_BASEDON_ATTRIBUTE_NAME		(_PN("basedon"))
#define PT_FOLLOWEDBY_ATTRIBUTE_NAME	(_PN("followedby"))
#define PT_ID_ATTRIBUTE_NAME            (_PN("id"))
#define PT_HEADER_ATTRIBUTE_NAME	    (_PN("header"))
#define PT_HEADEREVEN_ATTRIBUTE_NAME	(_PN("header-even"))
#define PT_HEADERFIRST_ATTRIBUTE_NAME	(_PN("header-first"))
#define PT_HEADERLAST_ATTRIBUTE_NAME	(_PN("header-last"))
#define PT_FOOTER_ATTRIBUTE_NAME	    (_PN("footer"))
#define PT_FOOTEREVEN_ATTRIBUTE_NAME	(_PN("footer-even"))
#define PT_FOOTERFIRST_ATTRIBUTE_NAME	(_PN("footer-first"))
#define PT_FOOTERLAST_ATTRIBUTE_NAME	(_PN("footer-last"))
#define PT_REVISION_ATTRIBUTE_NAME      (_PN("revision"))
#define PT_REVISION_DESC_ATTRIBUTE_NAME      (_PN("revision-desc"))
#define PT_REVISION_TIME_ATTRIBUTE_NAME      (_PN("revision-time"))
#define PT_REVISION_VERSION_ATTRIBUTE_NAME      (_PN("revision-ver"))
#define PT_DOCPROP_ATTRIBUTE_NAME      (_PN("docprop"))
#define PT_STRUX_IMAGE_DATAID           (_PN("strux-image-dataid"))
#define PT_XID_ATTRIBUTE_NAME           (_PN("xid"))
#define PT_DATAITEM_ATTRIBUTE_NAME           (_PN("dataitem"))
#define PT_IMAGE_DATAID           (_PN("dataid"))
#define PT_IMAGE_TITLE           (_PN("title"))
#define PT_IMAGE_DESCRIPTION           (_PN("alt"))
#define PT_DATA_PREVIEW           (_PN("preview"))
#define PT_HYPERLINK_TARGET_NAME  (_PN("xlink:href"))
#define PT_AUTHOR_NAME          (_PN("author"))
#define PT_ANNOTATION_NUMBER     (_PN("annotation"))
#define PT_RDF_XMLID             (_PN("xml:id"))
#define PT_XMLID                 (_PN("xml:id"))
#define PT_RDF_END               (_PN("rdf:end"))
#define PT_CHANGETRACKING_SPLIT_ID     (_PN("ct:split-id"))
#define PT_CHANGETRACKING_SPLIT_ID_REF (_PN("ct:split-id-ref"))
#define PT_CHANGETRACKING_SPLIT_IS_NEW (_PN("ct:split-is-new"))


#define ABIATTR_PARA_START_DELETED_REVISION (_PN("abi-para-start-deleted-revision"))
#define ABIATTR_PARA_END_DELETED_REVISION   (_PN("abi-para-end-deleted-revision"))
#define ABIATTR_PARA_DELETED_REVISION   (_PN("abi-para-deleted-revision"))

#endif /* PT_TYPES_H */
