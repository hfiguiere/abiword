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


#include <string.h>

#include "ut_types.h"
#include "ut_assert.h"
#include "ut_string.h"
#include "ut_debugmsg.h"
#include "xmlparse.h"
#include "pp_Property.h"
#include "pp_AttrProp.h"

#define NrElements(a)		(sizeof(a) / sizeof(a[0]))

/*****************************************************************/

static PP_Property _props[] =
{
	{ "color",					"000000",			1},
	{ "font-family",			"Times New Roman",	1},	// TODO this is Win32-specific.  must fix!
	{ "font-size",				"14pt",				1},
	{ "font-stretch",			"normal",			1},
	{ "font-style",				"normal",			1},
	{ "font-variant",			"normal",			1},
	{ "font-weight",			"normal",			1},
	{ "margin-bottom",			"0.25in",			0},
	{ "margin-top",				"0.25in",			0},
	{ "text-align",				"left",				1},
	{ "text-decoration",		"none",				1},

	{ "background-color",		"transparent",		0},
	{ "line-break-after",		"auto",				0},
	{ "line-break-before",		"auto",				0},
	{ "line-break-inside",		"auto",				0},
	{ "column-break-after",		"auto",				0},
	{ "column-break-before",	"auto",				0},
	{ "column-break-inside",	"auto",				0},
	{ "page-break-after",		"auto",				0},
	{ "page-break-before",		"auto",				0},
	{ "page-break-inside",		"auto",				0},
};

/*****************************************************************/

const XML_Char * PP_Property::getName() const
{
	return m_pszName;
}

const XML_Char * PP_Property::getInitial() const
{
	return m_pszInitial;
}

UT_Bool PP_Property::canInherit() const
{
	return m_bInherit;
}

const PP_Property * PP_lookupProperty(const XML_Char * name)
{
	/*
		TODO we can make this faster later by storing all the property names
		in alphabetical order and doing a binary search.
	*/

	int i;
	int count = NrElements(_props);

	for (i=0; i<count; i++)
	{
		if (0 == UT_stricmp(_props[i].m_pszName, name))
		{
			return _props + i;
		}
	}

	return NULL;
}

const XML_Char * PP_evalProperty(const XML_Char *  pszName,
								 const PP_AttrProp * pSpanAttrProp,
								 const PP_AttrProp * pBlockAttrProp,
								 const PP_AttrProp * pSectionAttrProp)
{
	// find the value for the given property
	// by evaluating it in the contexts given.
	// use the CSS inheritance as necessary.

	if (!pszName || !*pszName)
	{
		UT_DEBUGMSG(("PP_evalProperty: null property given\n"));
		return NULL;
	}

	const XML_Char * szValue;
	const PP_Property * pProp = PP_lookupProperty(pszName);
	if (!pProp)
	{
		UT_DEBUGMSG(("PP_evalProperty: unknown property\n"));
		return NULL;
	}
	
	// see if the property is on the Span item.
	
	if (pSpanAttrProp && pSpanAttrProp->getProperty(pProp->getName(),szValue))
		return szValue;

	// otherwise, see if we can inherit it from the containing block or the section.

	if (pProp->canInherit())
	{
		if (pBlockAttrProp && pBlockAttrProp->getProperty(pProp->getName(),szValue))
			return szValue;
		if (pSectionAttrProp && pSectionAttrProp->getProperty(pProp->getName(),szValue))
			return szValue;
	}

	// if no inheritance allowed for it or there is no
	// value set in containing block or section, we return
	// the default value for this property.
	
	return pProp->getInitial();
}

