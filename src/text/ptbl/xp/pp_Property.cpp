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
#include <stdlib.h>

#include "ut_types.h"
#include "ut_assert.h"
#include "ut_string.h"
#include "ut_debugmsg.h"
#include "pp_Property.h"
#include "pp_AttrProp.h"
#include "pd_Document.h"
#include "pd_Style.h"

/*****************************************************************/

/*
  TODO do we want this list of last-resort default settings to be here?
  It seems out of place... --EWS
*/
/*
	We need to be able to change the BiDi relevant dafault properties at runtime
	in response to the user changing the default direction in preferences.
	Therefore cannot use constants for these three, since those are stored in
	read-only segment.
*/
#ifdef BIDI_ENABLED
	#ifndef BIDI_RTL_DOMINANT
	XML_Char default_dominant_direction[]="ltr";
	XML_Char default_direction[]="ltr";
	XML_Char text_align[]="left\0";		//the '\0' is needed so that we can copy
										//the word 'right' here
	#else
	XML_Char default_dominant_direction[]="rtl";
	XML_Char default_direction[]="rtl";
	XML_Char text_align[]="right";
	#endif
	
#endif

// KEEP THIS ALPHABETICALLY ORDERED UNDER PENALTY OF DEATH!

static PP_Property _props[] =
{
	{ "background-color", "transparent", 0},
	{ "bgcolor", "ffffff", 1},

	{ "color",   "000000", 1},
	{ "column-gap",	"0.25in", 0},
	{ "column-line", "off",	0},
	{ "columns", "1", 0},

	{ "default-tab-interval",  "0.5in", 0},

#ifdef BIDI_ENABLED
	/*	these two stand for "direction" and "dominant direction"; they were
		intentionally abreviated, because the direction property has to be
		set basically for each word and each chunk of whitespace, inflating
		the ABW file
	*/
	{ "dir", default_direction,              1},  //the direction of the present text, prossible values ltr, rtl,ntrl	
	{ "dir-override", "off",              1},  //the direction of the present text, prossible values ltr, rtl,ntrl	
	{ "dom-dir", default_dominant_direction,              0},  //added by #TF, dominant direction of writing in a paragraph, can be either ltr or rtl (i.e., left-to-right, right-to-left)
#endif

	{ "field-color", "dcdcdc", 1},
	{ "field-font",	"NULL",	1},	
	{ "font-family", "Times New Roman", 1},	// TODO this is Win32-specific.  must fix!
	{ "font-size",	"12pt",	1},	// MS word defaults to 10pt, but it just seems too small
	{ "font-stretch", "normal", 1},
	{ "font-style",	"normal", 1},
	{ "font-variant", "normal", 1},
	{ "font-weight", "normal", 1},
	{ "format","%*%d.", 1},

	{ "height", "",	0},

	{ "keep-together", "", 0},
	{ "keep-with-next", "",	0},

	{ "lang", "en-US", 1},
	
	{ "line-height", "1.0", 0},
	{ "list-decimal", ".", 1},
	{ "list-delim", "%L", 1},

	{ "margin-bottom", "0in", 0},
	{ "margin-left", "0in",	0},
	{ "margin-right", "0in", 0},
	{ "margin-top",	"0in", 0}, // zero to be consistent with other WPs

	{ "orphans", "2", 0},

	{ "page-margin-bottom",		"1in",				0},
	{ "page-margin-footer",         "0in",                          0},
	{ "page-margin-header",         "0in",                          0},
	{ "page-margin-left",		"1in",				0},
	{ "page-margin-right",		"1in",				0},
	{ "page-margin-top",		"1in",				0},

	{ "section-space-after",	"0.25in",			0},
   	{ "start-value",			"1",				1},

	{ "tabstops", "", 0},
#ifdef BIDI_ENABLED
	{ "text-align", text_align,	1},
#else
	{ "text-align", "left",	1},
#endif	
	{ "text-decoration", "none", 1},
	{ "text-indent", "0in", 0},
	{ "text-position", "normal", 1},	
	
	{ "widows", "2", 0},
	{ "width", "", 0},

};

#ifdef __MRC__
extern "C"
#endif
static int s_compare (const void * a, const void * b)
{
  const PP_Property * prop;
  const char * name;

  name = (const char *)a;
  prop = (const PP_Property *)b;

  return UT_strcmp (name, prop->getName());
}

/*****************************************************************/

inline const XML_Char * PP_Property::getName() const
{
	return m_pszName;
}

inline const XML_Char * PP_Property::getInitial() const
{
	return m_pszInitial;
}

inline bool PP_Property::canInherit() const
{
	return m_bInherit;
}

const PP_Property * PP_lookupProperty(const XML_Char * name)
{
#if 0
	int i;
	int count = NrElements(_props);

	for (i=0; i<count; i++)
	{
		if (0 == UT_strcmp(_props[i].m_pszName, name))
		{
			return _props + i;
		}
	}

	return NULL;
#else
	PP_Property * prop = NULL;

	prop = (PP_Property *)bsearch (name, _props, NrElements(_props), sizeof (_props[0]), s_compare);
	return prop;
#endif
}

#ifdef BIDI_ENABLED
//allows us to reset the default value for the direction settings;
void PP_resetInitialBiDiValues(const XML_Char * pszValue)
{
	int i;
	int count = NrElements(_props);

	for (i=0; i<count; i++)
	{
		if ((0 == UT_stricmp(_props[i].m_pszName, "dir"))||(0 == UT_stricmp(_props[i].m_pszName, "dom-dir")))
		{
			UT_XML_strncpy(_props[i].m_pszInitial, 3,pszValue);
		}
		else if ((0 == UT_stricmp(_props[i].m_pszName, "text-align")))
		{
			UT_DEBUGMSG(("reseting text-align (%s)\n", pszValue));
			if(pszValue[0] == (XML_Char)'r')
				UT_XML_strncpy(_props[i].m_pszInitial, 5,"right");
			else
				UT_XML_strncpy(_props[i].m_pszInitial, 4,"left");
			break; //since the list is alphabetical, this is always the last one
		}
	}
}
#endif

static PD_Style * _getStyle(const PP_AttrProp * pAttrProp, PD_Document * pDoc)
{
	PD_Style * pStyle = NULL;

	const XML_Char * szValue = NULL;
	if (pAttrProp->getAttribute(PT_STYLE_ATTRIBUTE_NAME, szValue))
	{
		UT_ASSERT(szValue && szValue[0]);
		if (pDoc)
			pDoc->getStyle((char*)szValue, &pStyle);

		// NOTE: we silently fail if style is referenced, but not defined
	}

	return pStyle;
}

// make sure we don't get caught in a BASEDON loop
#define pp_BASEDON_DEPTH_LIMIT	10

const XML_Char * PP_evalProperty(const XML_Char *  pszName,
								 const PP_AttrProp * pSpanAttrProp,
								 const PP_AttrProp * pBlockAttrProp,
								 const PP_AttrProp * pSectionAttrProp,
								 PD_Document * pDoc,
								 bool bExpandStyles)
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
		UT_DEBUGMSG(("PP_evalProperty: unknown property \'%s\'\n",pszName));
		return NULL;
	}
	
	PD_Style * pStyle = NULL;

	// TODO: make lookup more efficient by tagging each property with scope (block, char, section)
		
	// see if the property is on the Span item.
	
	if (pSpanAttrProp)
	{
		if (pSpanAttrProp->getProperty(pProp->getName(),szValue))
			return szValue;

		if (bExpandStyles)
		{
			pStyle = _getStyle(pSpanAttrProp, pDoc);

			int i = 0;
			while (pStyle && (i < pp_BASEDON_DEPTH_LIMIT))
			{
				if (pStyle->getProperty(pProp->getName(), szValue))
					return szValue;

				pStyle = pStyle->getBasedOn();
				i++;
			}
		}
	}

	// otherwise, see if we can inherit it from the containing block or the section.

	if (!pSpanAttrProp || pProp->canInherit())
	{
		if (pBlockAttrProp)
		{
			if (pBlockAttrProp->getProperty(pProp->getName(),szValue))
				return szValue;
			
			if (bExpandStyles)
			{
				pStyle = _getStyle(pBlockAttrProp, pDoc);

				int i = 0;
				while (pStyle && (i < pp_BASEDON_DEPTH_LIMIT))
				{
					if (pStyle->getProperty(pProp->getName(), szValue))
						return szValue;

					pStyle = pStyle->getBasedOn();
					i++;
				}
			}
		}

		if (!pBlockAttrProp || pProp->canInherit())
		{
			if (pSectionAttrProp && pSectionAttrProp->getProperty(pProp->getName(),szValue))
				return szValue;
		}
	}

	if (pDoc->getStyle("Normal", &pStyle))
	{
		// next to last resort -- check for this property in the Normal style
		if (pStyle->getProperty(pProp->getName(), szValue))
			return szValue;
	}

	// if no inheritance allowed for it or there is no
	// value set in containing block or section, we return
	// the default value for this property.
	
	return pProp->getInitial();
}

