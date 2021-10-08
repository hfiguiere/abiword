/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */

/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2021 Hubert Figuière
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

#include "ut_string.h"
#include "pd_Style.h"
#include "pt_PieceTable.h"
#include "pp_Property.h"
#include "ut_debugmsg.h"

PD_Style::PD_Style(pt_PieceTable * pPT, PT_AttrPropIndex indexAP, const char * szName, bool bDisplayed) :
  m_pPT(pPT), m_indexAP(indexAP), m_szName(NULL), m_bDisplayed(bDisplayed), m_iUsed(0),
  m_pBasedOn(NULL), m_pFollowedBy(NULL)
{
  if (szName)
    m_szName = g_strdup (szName);
}

PD_Style::~PD_Style()
{
  FREEP(m_szName);
}

bool PD_Style::setIndexAP(PT_AttrPropIndex indexAP)
{
	UT_ASSERT_HARMLESS(indexAP != m_indexAP);

	// TODO: may need to rebind, handle undo, clear caches, etc.

	m_indexAP = indexAP;

	return true;
}

bool PD_Style::getProperty(PP_PropName name, const gchar *& szValue) const
{
	const PP_AttrProp * pAP = NULL;
	
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
		return false;
	else
		return pAP->getProperty(name, szValue);
}


bool PD_Style::getPropertyExpand(PP_PropName name, const gchar *& szValue) const
{
	const PP_AttrProp * pAP = NULL;
	
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
	{
		return false;
	}
	else
	{
		if( pAP->getProperty(name, szValue))
		{
			return true;
		}
		else
		{
			const PD_Style * pStyle = getBasedOn();
			if(pStyle != NULL)
			{
				return pStyle->_getPropertyExpand(name, szValue, 0);
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}


bool PD_Style::_getPropertyExpand(PP_PropName name, const gchar *& szValue, UT_sint32 iDepth) const
{
	const PP_AttrProp * pAP = NULL;
	
	
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
	{
		return false;
	}
	else
	{
		if( pAP->getProperty(name, szValue))
		{
			return true;
		}
		else
		{
			const PD_Style * pStyle = getBasedOn();
			if((pStyle != NULL) && (iDepth < pp_BASEDON_DEPTH_LIMIT ))
			{
				return pStyle->_getPropertyExpand(name, szValue, iDepth+1);
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}


bool PD_Style::getAttributeExpand(PP_PropName szName, const gchar *& szValue) const
{
	const PP_AttrProp * pAP = NULL;
	
	
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
	{
		return false;
	}
	else
	{
		if( pAP->getAttribute(szName, szValue))
		{
			return true;
		}
		else
		{
			const PD_Style * pStyle = getBasedOn();
			if(pStyle != NULL )
			{
				return pStyle->_getAttributeExpand(szName,szValue, 0);
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}


bool PD_Style::_getAttributeExpand(PP_PropName szName, const gchar *& szValue, UT_sint32 iDepth) const
{
	const PP_AttrProp * pAP = NULL;
	
	
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
	{
		return false;
	}
	else
	{
		if( pAP->getAttribute(szName, szValue))
		{
			return true;
		}
		else
		{
			const PD_Style * pStyle = getBasedOn();
			if((pStyle != NULL) && (iDepth < pp_BASEDON_DEPTH_LIMIT ) )
			{
				return pStyle->_getAttributeExpand(szName,szValue, iDepth+1);
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

std::unique_ptr<PP_PropertyType> PD_Style::getPropertyType(PP_PropName szName, tProperty_type Type) const
{
	const PP_AttrProp * pAP = NULL;
	
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
		return std::unique_ptr<PP_PropertyType>();
	else
		return pAP->getPropertyType(szName, Type);
}


bool PD_Style::getAttribute(PP_PropName name, const gchar *& szValue) const
{
	const PP_AttrProp * pAP = NULL;
	
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
		return false;
	else
		return pAP->getAttribute(name, szValue);
}

void PD_Style::used(UT_sint32 count)
{
	m_iUsed += count;
	if(m_iUsed < 0) m_iUsed = 0;
}

bool PD_Style::isUsed(void) const
{
	// TODO: we need some way of refcounting
	// TODO: what if this document is a template
	// NOTE: Re: the above - 
	//	Not sure how to know when a style that was
	// 	being used isn't anymore.  For the second
	//	point, I think a m_bTemplateStyle member
	//	might suffice.

	// marginally better
	return m_iUsed > 0;
}

bool PD_Style::isCharStyle(void) const
{
	// TODO: cache this too  

	const gchar * szValue = NULL;
	if (getAttribute(PT_TYPE_ATTRIBUTE_NAME, szValue))
		if (szValue && szValue[0])
			return g_ascii_strcasecmp(szValue, "C") == 0; // *PLEASE LEAVE THIS AS CASE
                                               // *INSESITIVE. IF YOU DON'T
                                               // * I'LL PUT YOUR EMAIL ADDRESS
                                               // * ON SPAM BLACK LIST!
                                               // I'm sick and tired of fixing
                                               // this regression - MES

	// default: no
	return false;
}

bool PD_Style::isList(void) const
{
	const char *szListStyle = NULL;
	if (getPropertyExpand(_PN("list-style"), szListStyle)) {
		return (g_ascii_strcasecmp(szListStyle, "None") != 0);
	}
	else {
		return FALSE;
	}
}

PD_Style * PD_Style::getBasedOn(void) const
{
	if (m_pBasedOn)
		return m_pBasedOn;

	const gchar * szStyle;

	if (getAttribute(PT_BASEDON_ATTRIBUTE_NAME, szStyle))
		if (szStyle && szStyle[0])
			m_pPT->getStyle(szStyle, &m_pBasedOn);

	// NOTE: we silently fail if style is referenced, but not defined

	return m_pBasedOn;
}

PD_Style * PD_Style::getFollowedBy(void) const
{
	if (m_pFollowedBy)
		return m_pFollowedBy;

	const gchar * szStyle;

	if (getAttribute(PT_FOLLOWEDBY_ATTRIBUTE_NAME, szStyle))
		if (szStyle && szStyle[0])
			m_pPT->getStyle((char*)szStyle, &m_pFollowedBy);

	// NOTE: we silently fail if style is referenced, but not defined

	return m_pFollowedBy;
}

/*!
 * Add a property to the style definition.
 */
bool PD_Style::addProperty(const gchar * szName, const gchar * szValue)
{
	const PP_AttrProp * pAP = NULL;
	bool bres= true;
	
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
		return false;
	else
	{
		const PP_PropertyVector pProps = {
			{ szName, szValue }
		};
		PP_AttrProp * pNewAP = pAP->cloneWithReplacements(PP_NOPROPS,
                                                          pProps, false);
		pNewAP->markReadOnly();
		bres =	m_pPT->getVarSet().addIfUniqueAP(pNewAP, &m_indexAP);
	}
	return bres;
}

/*!
 * This method adds the properties defined in pProperties to the already
 * defined set. If a property already exists in the definition, its value is
 * replaced.
\param const gchar ** pProperties string of properties
*/
bool PD_Style::addProperties(const PP_PropertyVector & pProperties)
{
	const PP_AttrProp * pAP = NULL;
	bool bres= true;
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
		return false;
	else
	{
		PP_AttrProp * pNewAP =
			pAP->cloneWithReplacements(PP_NOPROPS, pProperties, false);
		pNewAP->markReadOnly();
		bres =	m_pPT->getVarSet().addIfUniqueAP(pNewAP, &m_indexAP);
	}
	return bres;
}

/*!
 * This method replaces the previous set of attributes/properties with
 * a new one defined in %atts. It is imperitive that updateDocForStyleChange
 * be called after this method.
 * \param atts list of attributes with an extended properties
 *                        string
 */
bool PD_Style::setAllAttributes(const PP_PropertyVector & atts)
{
	bool bres =	m_pPT->getVarSet().storeAP(atts, &m_indexAP);
	m_pFollowedBy = NULL;
	m_pBasedOn = NULL;
	return bres;
}


bool PD_Style::addAttributes(const PP_PropertyVector & pAtts)
{
	const PP_AttrProp * pAP = NULL;
	bool bres = false;
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
		return false;
	else
	{
		if(pAP->areAlreadyPresent(pAtts, PP_NOPROPS))
		{
			return true;
		}
		PP_AttrProp * pNewAP = pAP->cloneWithReplacements(pAtts, PP_NOPROPS, false);
		UT_return_val_if_fail( pNewAP, false );

		pNewAP->markReadOnly();
		bres =	m_pPT->getVarSet().addIfUniqueAP(pNewAP, &m_indexAP);
	}
	m_pFollowedBy = NULL;
	m_pBasedOn = NULL;
	return bres;
}

size_t PD_Style::getAttributeCount(void) const
{
  	const PP_AttrProp * pAP = NULL;
	
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
		return 0;
	else
	        return pAP->getAttributeCount();
}


size_t PD_Style::getPropertyCount(void) const
{
  	const PP_AttrProp * pAP = NULL;
	
	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
		return 0;
	else
	        return pAP->getPropertyCount();
}

bool PD_Style::getNthAttribute (int ndx, PP_PropName &name, const gchar *&szValue) const
{
  	const PP_AttrProp * pAP = NULL;

	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
		return false;
	else
	  {
	        return pAP->getNthAttribute(ndx, name, szValue);
	  }
}

bool PD_Style::getNthProperty (int ndx, PP_PropName &name, const gchar *&szValue) const
{
  	const PP_AttrProp * pAP = NULL;

	if (!m_pPT->getAttrProp(m_indexAP, &pAP))
		return false;
	else
	  {
	        return pAP->getNthProperty(ndx, name, szValue);
	  }
}

/*!
 * This method fills a vector structure with all the attributes defined
 * in this style, including the basedon style.
\param vProps the vector containing const gchar * (name,value) pairs
*/

bool PD_Style::getAllAttributes(PP_PropertyVector & vAttribs, UT_sint32 depth) const
{
//
// This method will be recursively called to basedon style
//
	UT_sint32 count = getAttributeCount();
	PP_PropName szName;
	const gchar * szValue = NULL;
	for (UT_sint32 i = 0; i < count; i++) {
		getNthAttribute(i, szName, szValue);
		bool bfound = false;
//
// Only keep the most recently defined properties
//
		for (PP_PropertyVector::size_type j = 0; (j < vAttribs.size()) && !bfound ; j ++) {
			bfound = (vAttribs[j].name == szName);
		}
		if (!bfound) {
			vAttribs.push_back({ szName, szValue });
		}
	}
	if (depth < pp_BASEDON_DEPTH_LIMIT && getBasedOn() != NULL)	{
		getBasedOn()->getAllAttributes(vAttribs,depth +1);
	}
	return true;
}

/*!
 * This method fills a vector structure with all the properties defined
 * in this style, including the basedon style.
\param vProps the vector containing const gchar * (name,value) pairs
*/

bool PD_Style::getAllProperties(PP_PropertyVector & vProps, UT_sint32 depth) const
{
//
// This method will be recursively called to basedon style
//
	UT_sint32 count = getPropertyCount();
	PP_PropName szName;
	const gchar * szValue = NULL;

	for (UT_sint32 i = 0; i < count; i++) {
		getNthProperty(i, szName, szValue);
		bool bfound = false;
//
// Only keep the most recently defined properties
//
		for (PP_PropertyVector::size_type j = 0; (j < vProps.size()) && !bfound ; j++) {
			bfound = (vProps[j].name == szName);
		}
		if (!bfound) {
			vProps.push_back({ szName, szValue });
		}
	}
	if (depth < pp_BASEDON_DEPTH_LIMIT && getBasedOn() != NULL) {
		getBasedOn()->getAllProperties(vProps,depth +1);
	}
	return true;
}

//////////////////////////////////////////////////////////////////
// a sub-class to wrap the compiled-in styles
//////////////////////////////////////////////////////////////////

PD_BuiltinStyle::PD_BuiltinStyle(pt_PieceTable * pPT, PT_AttrPropIndex indexAP, const char * szName, bool bDisplayed)
  : PD_Style(pPT, indexAP, szName, bDisplayed), m_indexAPOrig(indexAP)
{
}

PD_BuiltinStyle::~PD_BuiltinStyle()
{
}



