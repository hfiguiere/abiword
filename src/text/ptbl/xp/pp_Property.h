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



#ifndef PP_PROPERTY_H
#define PP_PROPERTY_h

#include "ut_types.h"
#include "xmlparse.h"
class PP_AttrProp;

// PP_Property captures knowledge of the various CSS properties,
// such as their initial/default values and whether they are
// inherited.

class PP_Property
{
public:
	XML_Char *	m_pszName;
	XML_Char *	m_pszInitial;
	UT_Bool		m_bInherit;

	const XML_Char *	getName() const;
	const XML_Char *	getInitial() const;
	UT_Bool				canInherit() const;
};

const PP_Property * PP_lookupProperty(const XML_Char * pszName);
const XML_Char * PP_evalProperty(const XML_Char * pszName,
								 const PP_AttrProp * pSpanAttrProp,
								 const PP_AttrProp * pBlockAttrProp,
								 const PP_AttrProp * pSectionAttrProp);

#endif /* PP_PROPERTY_H */
