/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*- */

/* AbiWord
 * Copyright (C) 1998-2000 AbiSource, Inc.
 * Copyright (c) 2016 Hubert Figuiere
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


#include <string.h>
#include <stdlib.h>

#include "ut_types.h"
#include "ut_assert.h"
#include "ut_string.h"
#include "ut_hash.h"
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
	Response: I agree that it seems out of place.  It's inconsistent with
				the per-class definition and organization of the properties.
				IE, the pd_Document use of dom-dir.  We don't even have a
				PP_LEVEL_DOC represented here.		-MG
*/

/*
	We need to be able to change the BiDi relevant dafault properties at runtime
	in response to the user changing the default direction in preferences.
	Therefore cannot use constants for these three, since those are stored in
	read-only segment.
*/
#ifndef BIDI_RTL_DOMINANT
	gchar def_dom_dir[]="ltr";
	gchar default_direction[]="ltr";
	gchar text_align[]="left\0";		//the '\0' is needed so that we can copy
										//the word 'right' here
#else
	gchar def_dom_dir[]="rtl";
	gchar default_direction[]="rtl";
	gchar text_align[]="right";
#endif

// KEEP THIS ALPHABETICALLY ORDERED UNDER PENALTY OF DEATH!


/*!
 * Definitions are: Property Nme: Initial Value: Can Inherit: Pointer
 * to class : tPropLevel
 * tPropLevel should be set by or-ing the values defined in PP_Property.h
 */
static PP_Property _props[] =
{
	{ _PN("background-color"),      "transparent",     false, PP_LEVEL_SECT},
	{ _PN("bgcolor"),               "transparent",     true,  PP_LEVEL_CHAR},
	{ _PN("border-merge"),           "0",               true,  PP_LEVEL_BLOCK},
	{ _PN("border-shadow-merge"),    "0",               true,  PP_LEVEL_BLOCK},
	{ _PN("bot-attach"),            "",               false,  PP_LEVEL_TABLE},
	{ _PN("bot-color"),             "000000",          false, PP_LEVEL_TABLE},
	{ _PN("bot-shadow"),            "0",               false, PP_LEVEL_BLOCK},
	{ _PN("bot-shadow-color"),      "grey",            false, PP_LEVEL_BLOCK},
	{ _PN("bot-space"),             "0.02in",          false, PP_LEVEL_BLOCK},
	{ _PN("bot-style"),             "1",           false, PP_LEVEL_TABLE},
	{ _PN("bot-thickness"),         "1px",             false, PP_LEVEL_TABLE},

	{ _PN("bounding-space"),        "0.05in",          false, PP_LEVEL_FRAME},

	{ _PN("cell-margin-bottom"),   "0.002in",         false,  PP_LEVEL_TABLE},
	{ _PN("cell-margin-left"),     "0.002in",         false,  PP_LEVEL_TABLE},
	{ _PN("cell-margin-right"),    "0.002in",         false,  PP_LEVEL_TABLE},
	{ _PN("cell-margin-top"),      "0.002in",         false,  PP_LEVEL_TABLE},

	{ _PN("color"),                 "000000",          true,  PP_LEVEL_CHAR},
	{ _PN("column-gap"),	           "0.25in",          false, PP_LEVEL_SECT},
	{ _PN("column-line"),           "off",	          false, PP_LEVEL_SECT},
	{ _PN("columns"),               "1",               false, PP_LEVEL_SECT},

	{ _PN("default-tab-interval"),  "0.5in",           false, PP_LEVEL_BLOCK},
	{ _PN("dir-override"),          NULL,              true,  PP_LEVEL_CHAR},
	{ _PN("display"),               "inline",          true,  PP_LEVEL_CHAR},
	{ _PN("dom-dir"),               def_dom_dir,       true,  PP_LEVEL_BLOCK | PP_LEVEL_SECT},

	{ _PN("field-color"),           "dcdcdc",          true,  PP_LEVEL_FIELD},
	{ _PN("field-font"),	           "NULL",	          true,  PP_LEVEL_FIELD},
	{ _PN("font-family"),           "Times New Roman", true,  PP_LEVEL_CHAR},
	{ _PN("font-size"),	           "12pt",	          true,  PP_LEVEL_CHAR},	// MS word defaults to 10pt, but it just seems too small
	{ _PN("font-stretch"),          "normal",          true,  PP_LEVEL_CHAR},
	{ _PN("font-style"),	           "normal",          true,  PP_LEVEL_CHAR},
	{ _PN("font-variant"),          "normal",          true,  PP_LEVEL_CHAR},
	{ _PN("font-weight"),           "normal",          true,  PP_LEVEL_CHAR},
	{ _PN("footer"),                "",                false, PP_LEVEL_SECT},
	{ _PN("footer-even"),           "",                false, PP_LEVEL_SECT},
	{ _PN("footer-first"),          "",                false, PP_LEVEL_SECT},
	{ _PN("footer-last"),           "",                false, PP_LEVEL_SECT},
	{ _PN("format"),                "%*%d.",           true,  PP_LEVEL_BLOCK},

	{ _PN("frame-col-xpos"),         "0.0in",           false, PP_LEVEL_FRAME},
	{ _PN("frame-col-ypos"),         "0.0in",           false, PP_LEVEL_FRAME},
	{ _PN("frame-expand-height"),    "0.0in",           false, PP_LEVEL_FRAME},
	{ _PN("frame-height"),           "0.0in",           false, PP_LEVEL_FRAME},
	{ _PN("frame-horiz-align"),      "left",            false, PP_LEVEL_FRAME},
	{ _PN("frame-min-height"),       "0.0in",           false, PP_LEVEL_FRAME},
	{ _PN("frame-page-xpos"),        "0.0in",           false, PP_LEVEL_FRAME},
	{ _PN("frame-page-ypos"),        "0.0in",           false, PP_LEVEL_FRAME},
	{ _PN("frame-pref-column"),      "0",               false, PP_LEVEL_FRAME},
	{ _PN("frame-pref-page"),        "0",               false, PP_LEVEL_FRAME},
	{ _PN("frame-rel-width"),        "0.5",             false, PP_LEVEL_FRAME},
	{ _PN("frame-type"),             "textbox",         false, PP_LEVEL_FRAME},
	{ _PN("frame-width"),            "0.0in",           false, PP_LEVEL_FRAME},

	{ _PN("header"),                "",                false, PP_LEVEL_SECT},
	{ _PN("header-even"),           "",                false, PP_LEVEL_SECT},
	{ _PN("header-first"),          "",                false, PP_LEVEL_SECT},
	{ _PN("header-last"),           "",                false, PP_LEVEL_SECT},
	{ _PN("height"),                "0in",             false, PP_LEVEL_CHAR},
	{ _PN("homogeneous"),           "1",               false, PP_LEVEL_CHAR},

	{ _PN("keep-together"),         "no",              false, PP_LEVEL_BLOCK},
	{ _PN("keep-with-next"),        "no",              false, PP_LEVEL_BLOCK},

	{ _PN("lang"),                  "en-US",           true,  PP_LEVEL_CHAR},
	{ _PN("left-attach"),           "",               false,  PP_LEVEL_TABLE},
	{ _PN("left-color"),            "000000",          false, PP_LEVEL_TABLE},
	{ _PN("left-shadow"),           "0",               false, PP_LEVEL_BLOCK},
	{ _PN("left-shadow-color"),     "grey",            false, PP_LEVEL_BLOCK},
	{ _PN("left-space"),            "0.02in",          false, PP_LEVEL_BLOCK},
	{ _PN("left-style"),            "1",           false, PP_LEVEL_TABLE},
	{ _PN("left-thickness"),        "1px",             false, PP_LEVEL_TABLE},

	{ _PN("line-height"),           "1.0",             false, PP_LEVEL_BLOCK},
	{ _PN("list-decimal"),          ".",               true,  PP_LEVEL_BLOCK},
	{ _PN("list-delim"),            "%L",              true,  PP_LEVEL_BLOCK},
	{ _PN("list-style"),            "None",            true,  PP_LEVEL_CHAR},
	{ _PN("list-tag"),              "0",               false, PP_LEVEL_BLOCK},

	{ _PN("margin-bottom"),         "0in",             false, PP_LEVEL_BLOCK},
	{ _PN("margin-left"),           "0in",	          false, PP_LEVEL_BLOCK},
	{ _PN("margin-right"),          "0in",             false, PP_LEVEL_BLOCK},
	{ _PN("margin-top"),	           "0in",             false, PP_LEVEL_BLOCK}, // zero to be consistent with other WPs

	{ _PN("orphans"),               "2",               false, PP_LEVEL_BLOCK}, // 2 to be consistent with widows & CSS

	{ _PN("page-margin-bottom"),	   "1in",             false, PP_LEVEL_SECT},
	{ _PN("page-margin-footer"),    "0.0in",           false, PP_LEVEL_SECT},
	{ _PN("page-margin-header"),    "0.0in",           false, PP_LEVEL_SECT},
	{ _PN("page-margin-left"),	   "1in",             false, PP_LEVEL_SECT},
	{ _PN("page-margin-right"),     "1in",             false, PP_LEVEL_SECT},
	{ _PN("page-margin-top"),       "1in",             false, PP_LEVEL_SECT},
	{ _PN("position-to"),           "block-above-text",false, PP_LEVEL_FRAME},
	{ _PN("right-attach"),          "",                false, PP_LEVEL_TABLE},
	{ _PN("right-color"),           "000000",          false, PP_LEVEL_TABLE},
	{ _PN("right-shadow"),          "0",               false, PP_LEVEL_BLOCK},
	{ _PN("right-shadow-color"),    "grey",            false, PP_LEVEL_BLOCK},
	{ _PN("right-space"),           "0.02in",          false, PP_LEVEL_BLOCK},
	{ _PN("right-style"),           "1",           false, PP_LEVEL_TABLE},
	{ _PN("right-thickness"),       "1px",             false, PP_LEVEL_TABLE},

	{ _PN("section-footnote-line-thickness"),"0.005in",false, PP_LEVEL_SECT},
	{ _PN("section-footnote-yoff"), "0.01in",          false, PP_LEVEL_SECT},
	{ _PN("section-max-column-height"), "0in",         false, PP_LEVEL_SECT},
	{ _PN("section-restart"),       "",                false, PP_LEVEL_SECT},
	{ _PN("section-restart-value"), "",                false, PP_LEVEL_SECT},
	{ _PN("section-space-after"),   "0.25in",          false, PP_LEVEL_SECT},
	{ _PN("shading-background-color"), "white",         false, PP_LEVEL_BLOCK},
	{ _PN("shading-foreground-color"), "white",         false, PP_LEVEL_BLOCK},
	{ _PN("shading-pattern"),          "0",             false, PP_LEVEL_BLOCK},
	{ _PN("start-value"),           "1",               true,  PP_LEVEL_BLOCK},

	{ _PN("table-border"),          "0.1in",           false, PP_LEVEL_TABLE},
	{ _PN("table-col-spacing"),     "0.03in",          false, PP_LEVEL_TABLE},
	{ _PN("table-column-leftpos"),  "0.0in",           false, PP_LEVEL_TABLE},
	{ _PN("table-column-props"),    "",                false, PP_LEVEL_TABLE},
	{ _PN("table-line-thickness"),  "0.8pt",           false, PP_LEVEL_TABLE},
	{ _PN("table-line-type"),       "1",               false, PP_LEVEL_TABLE},
	{ _PN("table-margin-bottom"),   "0.01in",          false, PP_LEVEL_TABLE},
	{ _PN("table-margin-left"),     "0.005in",         false, PP_LEVEL_TABLE},
	{ _PN("table-margin-right"),    "0.005in",         false, PP_LEVEL_TABLE},
	{ _PN("table-margin-top"),      "0.01in",          false, PP_LEVEL_TABLE},
	{ _PN("table-max-extra-margin"),"0.05",            false, PP_LEVEL_TABLE},
	{ _PN("table-row-props"),       "",                false, PP_LEVEL_TABLE},
	{ _PN("table-row-spacing"),     "0.01in",          false, PP_LEVEL_TABLE},
	{ _PN("tabstops"),              "",                false, PP_LEVEL_BLOCK},
	{ _PN("text-align"),            text_align,	      true,  PP_LEVEL_BLOCK},
	{ _PN("text-decoration"),       "none",            true,  PP_LEVEL_CHAR},
	{ _PN("text-folded"),           "0",               false, PP_LEVEL_BLOCK},
	{ _PN("text-folded-id"),        "0",               false, PP_LEVEL_BLOCK},
	{ _PN("text-indent"),           "0in",             false, PP_LEVEL_BLOCK},
	{ _PN("text-position"),         "normal",          true,  PP_LEVEL_CHAR},
	{ _PN("text-transform"),         "none",          true,  PP_LEVEL_CHAR},
	{ _PN("toc-dest-style1"),      "Contents 1"   ,   false, PP_LEVEL_BLOCK},
	{ _PN("toc-dest-style2"),      "Contents 2",      false, PP_LEVEL_BLOCK},
	{ _PN("toc-dest-style3"),      "Contents 3",      false, PP_LEVEL_BLOCK},
	{ _PN("toc-dest-style4"),      "Contents 4",      false, PP_LEVEL_BLOCK},
	{ _PN("toc-has-heading"),       "1",               false, PP_LEVEL_BLOCK},
	{ _PN("toc-has-label1"),       "1",                false, PP_LEVEL_BLOCK},
	{ _PN("toc-has-label2"),       "1",                false, PP_LEVEL_BLOCK},
	{ _PN("toc-has-label3"),       "1",                false, PP_LEVEL_BLOCK},
	{ _PN("toc-has-label4"),       "1",                false, PP_LEVEL_BLOCK},
	{ _PN("toc-heading"),       "Contents",   false, PP_LEVEL_BLOCK},
	{ _PN("toc-heading-style"),  "Contents Header",    false, PP_LEVEL_BLOCK},
	{ _PN("toc-id"),                "0",               false, PP_LEVEL_SECT},
	{ _PN("toc-indent1"),           "0.5in",           false, PP_LEVEL_BLOCK},
	{ _PN("toc-indent2"),           "0.5in",           false, PP_LEVEL_BLOCK},
	{ _PN("toc-indent3"),           "0.5in",           false, PP_LEVEL_BLOCK},
	{ _PN("toc-indent4"),           "0.5in",           false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-after1"),       "",               false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-after2"),       "",               false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-after3"),       "",               false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-after4"),       "",               false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-before1"),       "",              false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-before2"),       "",              false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-before3"),       "",              false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-before4"),       "",              false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-inherits1"),       "1",           false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-inherits2"),       "1",           false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-inherits3"),       "1",           false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-inherits4"),       "1",           false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-start1"),       "1",              false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-start2"),       "1",              false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-start3"),       "1",              false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-start4"),       "1",              false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-type1"),       "numeric",         false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-type2"),       "numeric",         false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-type3"),       "numeric",         false, PP_LEVEL_BLOCK},
	{ _PN("toc-label-type4"),       "numeric",         false, PP_LEVEL_BLOCK},
	{ _PN("toc-page-type1"),        "numeric",         false, PP_LEVEL_BLOCK},
	{ _PN("toc-page-type2"),        "numeric",         false, PP_LEVEL_BLOCK},
	{ _PN("toc-page-type3"),        "numeric",         false, PP_LEVEL_BLOCK},
	{ _PN("toc-page-type4"),        "numeric",         false, PP_LEVEL_BLOCK},
	{ _PN("toc-range-bookmark"),    "",                false, PP_LEVEL_BLOCK},
	{ _PN("toc-source-style1"),     "Heading 1",       false, PP_LEVEL_BLOCK},
	{ _PN("toc-source-style2"),     "Heading 2",       false, PP_LEVEL_BLOCK},
	{ _PN("toc-source-style3"),     "Heading 3",       false, PP_LEVEL_BLOCK},
	{ _PN("toc-source-style4"),     "Heading 4",       false, PP_LEVEL_BLOCK},
	{ _PN("toc-tab-leader1"),       "dot",             false, PP_LEVEL_BLOCK},
	{ _PN("toc-tab-leader2"),       "dot",             false, PP_LEVEL_BLOCK},
	{ _PN("toc-tab-leader3"),       "dot",             false, PP_LEVEL_BLOCK},
	{ _PN("toc-tab-leader4"),       "dot",             false, PP_LEVEL_BLOCK},

	{ _PN("top-attach"),             "",               false, PP_LEVEL_TABLE},
	{ _PN("top-color"),             "000000",          false, PP_LEVEL_TABLE},
	{ _PN("top-shadow"),            "0",               false, PP_LEVEL_BLOCK},
	{ _PN("top-shadow-color"),      "grey",            false, PP_LEVEL_BLOCK},
	{ _PN("top-space"),             "0.02in",          false, PP_LEVEL_BLOCK},
	{ _PN("top-style"),             "1",               false, PP_LEVEL_TABLE},
	{ _PN("top-thickness"),         "1px",             false, PP_LEVEL_TABLE},


	{ _PN("vert-align"),            "0",               false, PP_LEVEL_TABLE},

	{ _PN("widows"),                "2",               false, PP_LEVEL_BLOCK},
	{ _PN("width"),                 "0in",             false, PP_LEVEL_CHAR},
	{ _PN("wrap-mode"),             "above-text",      false, PP_LEVEL_FRAME},
	{ _PN("xpad"),                  "0.03in",          false, PP_LEVEL_FRAME},
	{ _PN("xpos"),                  "0.0in",           false, PP_LEVEL_FRAME},
	{ _PN("ypad"),                  "0.03in",          false, PP_LEVEL_FRAME},
	{ _PN("ypos"),                  "0.0in",           false, PP_LEVEL_FRAME}
};

static int s_compare (const void * a, const void * b)
{
  const PP_Property * prop;
  const char* name;

  name = static_cast<const char *>(a);
  prop = static_cast<const PP_Property *>(b);

  return strcmp (name, prop->getName().c_str());
}

/*****************************************************************/

const PP_Property * PP_lookupProperty(PP_PropName name)
{
	PP_Property * prop = NULL;

	prop = static_cast<PP_Property *>(bsearch (name.c_str(), _props, G_N_ELEMENTS(_props), sizeof (_props[0]), s_compare));

	return prop;
}

//allows us to reset the default value for the direction settings;
void PP_resetInitialBiDiValues(const gchar * pszValue)
{
	int i;
	int count = G_N_ELEMENTS(_props);

	for (i=0; i<count; i++)
	{
		if (/*(0 == strcmp(_props[i].m_pszName, "dir"))
		  ||*/(_props[i].m_pszName == "dom-dir")
		  /*||(0 == strcmp(_props[i].m_pszName, "column-order"))*/)
		  //this last one is not necessary since dom-dir and column-order
		  //share the same physical string
		{
			_props[i].m_pszInitial = pszValue;
		}
		else if (_props[i].m_pszName == "text-align") {
			UT_DEBUGMSG(("reseting text-align (%s)\n", pszValue));
			if (pszValue[0] == 'r') {
				_props[i].m_pszInitial = "right";
			}
			else {
				_props[i].m_pszInitial = "left";
			}
			break; //since the list is alphabetical, this is always the last one
		}
	}
}

void PP_setDefaultFontFamily(const char* pszFamily)
{
	static std::string family(pszFamily ? pszFamily : "");
	PP_Property* prop = static_cast<PP_Property*>(bsearch ("font-family", _props, G_N_ELEMENTS(_props), sizeof(_props[0]), s_compare));
	prop->m_pszInitial = family.c_str();
}

static PD_Style * _getStyle(const PP_AttrProp * pAttrProp, const PD_Document * pDoc)
{
	PD_Style * pStyle = NULL;

	const gchar * szValue = NULL;
//
// SHIT. This is where the style/name split gets really hairy. This index AP MIGHT be
// from a style definition in which case the name of the style is PT_NAME_ATTRIBUTE_NAME
// or it might be from the document in which case the attribute is
// PT_STYLE_ATTRIBUTE_NAME. Fuck it, try both. - MES.
//
	if (pAttrProp->getAttribute(PT_NAME_ATTRIBUTE_NAME, szValue))
	{
		UT_return_val_if_fail (szValue && szValue[0], NULL);
		if (pDoc)
			pDoc->getStyle(szValue, &pStyle);

		// NOTE: we silently fail if style is referenced, but not defined
	}
    else if(pAttrProp->getAttribute(PT_STYLE_ATTRIBUTE_NAME, szValue))
	{
		UT_return_val_if_fail (szValue && szValue[0], NULL);
		if (pDoc)
			pDoc->getStyle(szValue, &pStyle);

		// NOTE: we silently fail if style is referenced, but not defined
	}

	return pStyle;
}

static const gchar * s_evalProperty (const PP_Property * pProp,
										const PP_AttrProp * pAttrProp,
										const PD_Document * pDoc,
										bool bExpandStyles)
{
	const gchar * szValue = NULL;

	if (pAttrProp->getProperty (pProp->getName(), szValue))
		{
			return szValue;
		}
	if (!bExpandStyles) return NULL;

	PD_Style * pStyle = _getStyle (pAttrProp, pDoc);

	int i = 0;
	while (pStyle && (i < pp_BASEDON_DEPTH_LIMIT))
		{
			if (pStyle->getProperty (pProp->getName (), szValue))
				{
					return szValue;
				}
			pStyle = pStyle->getBasedOn ();
			i++;
		}
	return NULL;
}

const gchar * PP_evalProperty (PP_PropName pszName,
								  const PP_AttrProp * pSpanAttrProp,
								  const PP_AttrProp * pBlockAttrProp,
								  const PP_AttrProp * pSectionAttrProp,
								  const PD_Document * pDoc,
								  bool bExpandStyles)
{
	// find the value for the given property
	// by evaluating it in the contexts given.
	// use the CSS inheritance as necessary.

	if (!pszName || !*pszName.c_str()) {
		UT_DEBUGMSG(("PP_evalProperty: null property given\n"));
		return NULL;
	}

	if (pDoc == nullptr)
		bExpandStyles = false;

	const PP_Property * pProp = PP_lookupProperty(pszName);
	if (!pProp)
	{
		UT_DEBUGMSG(("PP_evalProperty: unknown property \'%s\'\n", pszName.c_str()));
		return NULL;
	}

	/* Not all properties can have a value of inherit, but we're not validating here.
	 * This is not to be confused with automatic inheritance - the difference is whether
	 * to take the default value (for when no value is specified).
	 */
	bool bInherit = false;

	// see if the property is on the Span item.

	const gchar * szValue = NULL;

	// TODO: ?? make lookup more efficient by tagging each property with scope (block, char, section)

	if (pSpanAttrProp)
	{
		szValue = s_evalProperty (pProp, pSpanAttrProp, pDoc, bExpandStyles);

		if (szValue)
			if (strcmp (szValue, "inherit") == 0)
			{
				szValue = NULL;
				bInherit = true;
			}
		if ((szValue == NULL) && (bInherit || pProp->canInherit ()))
		{
			bInherit = false;

			if (pBlockAttrProp)
			{
				szValue = s_evalProperty (pProp, pBlockAttrProp, pDoc, bExpandStyles);

				if (szValue)
					if (strcmp (szValue, "inherit") == 0)
					{
						szValue = NULL;
						bInherit = true;
					}
				if ((szValue == NULL) && (bInherit || pProp->canInherit ()))
				{
					bInherit = false;

					if (pSectionAttrProp)
					{
						szValue = s_evalProperty (pProp, pSectionAttrProp, pDoc, bExpandStyles);

						if (szValue)
							if (strcmp (szValue, "inherit") == 0)
							{
								szValue = NULL;
								bInherit = true;
							}
						if ((szValue == NULL) && (bInherit || pProp->canInherit ()))
						{
							const PP_AttrProp * pDocAP = pDoc->getAttrProp ();
							if (pDocAP)
								pDocAP->getProperty (pszName, szValue);
						}
					}
				}
			}
		}
	}
	else if (pBlockAttrProp)
	{
		szValue = s_evalProperty (pProp, pBlockAttrProp, pDoc, bExpandStyles);

		if (szValue)
			if (strcmp (szValue, "inherit") == 0)
			{
				szValue = NULL;
				bInherit = true;
			}
		if ((szValue == NULL) && (bInherit || pProp->canInherit ()))
		{
			bInherit = false;

			if (pSectionAttrProp)
			{
				szValue = s_evalProperty (pProp, pSectionAttrProp, pDoc, bExpandStyles);

				if (szValue)
					if (strcmp (szValue, "inherit") == 0)
					{
						szValue = NULL;
						bInherit = true;
					}
				if ((szValue == NULL) && (bInherit || pProp->canInherit ()))
				{
					const PP_AttrProp * pDocAP = pDoc->getAttrProp ();
					if (pDocAP)
						pDocAP->getProperty (pszName, szValue);
				}
			}
		}
	}
	else if (pSectionAttrProp)
	{
		szValue = s_evalProperty (pProp, pSectionAttrProp, pDoc, bExpandStyles);

		if (szValue)
			if (strcmp (szValue, "inherit") == 0)
			{
				szValue = NULL;
				bInherit = true;
			}
		if ((szValue == NULL) && (bInherit || pProp->canInherit ()))
		{
			const PP_AttrProp * pDocAP = pDoc->getAttrProp ();
			if (pDocAP)
				pDocAP->getProperty (pszName, szValue);
		}
	}
	else
	{
		const PP_AttrProp * pDocAP = pDoc->getAttrProp ();
		if (pDocAP)
		{
			pDocAP->getProperty (pszName, szValue);

			// dom-dir requires special treatment at document level
			if (szValue && pszName == "dom-dir") {
				if(   strcmp(szValue, "logical-ltr") == 0
				   || strcmp(szValue, "logical-rtl") == 0)
					szValue += 8;
			}
		}
	}
	if (szValue)
		if (strcmp (szValue, "inherit") == 0) // shouldn't happen, but doesn't hurt to check
			szValue = NULL;

	if (szValue == NULL)
		if (bExpandStyles)
		{
			PD_Style * pStyle = nullptr;

			if (pDoc->getStyle ("Normal", &pStyle))
			{
				/* next to last resort -- check for this property in the Normal style
				 */
				pStyle->getProperty (pszName, szValue);

				if (szValue)
					if (strcmp (szValue, "inherit") == 0)
						szValue = NULL;
			}
		}

	if(szValue == NULL && pDoc && (bInherit || pProp->canInherit ()))
	{
		// see if the doc has a value for this prop
		const PP_AttrProp *  pAP = pDoc->getAttrProp();
		if(pAP)
		{
			pAP->getProperty(pszName, szValue);
		}
	}
	
	if (szValue == NULL)
		szValue = pProp->getInitial (); // which may itself be NULL, but that is a bad thing - FIXME!!

	return szValue;
}

std::unique_ptr<PP_PropertyType> PP_evalPropertyType(PP_PropName  pszName,
								 const PP_AttrProp * pSpanAttrProp,
								 const PP_AttrProp * pBlockAttrProp,
								 const PP_AttrProp * pSectionAttrProp,
								 tProperty_type Type,
								 const PD_Document * pDoc,
								 bool bExpandStyles)
{
	// find the value for the given property
	// by evaluating it in the contexts given.
	// use the CSS inheritance as necessary.

	if (!pszName || !*pszName.c_str()) {
		UT_DEBUGMSG(("PP_evalProperty: null property given\n"));
		return NULL;
	}

	std::unique_ptr<PP_PropertyType> p_property;
	const PP_Property * pProp = PP_lookupProperty(pszName);
	if (!pProp)
	{
		UT_DEBUGMSG(("PP_evalProperty: unknown property \'%s\'\n", pszName.c_str()));
		return NULL;
	}

	PD_Style * pStyle = NULL;

	// TODO: make lookup more efficient by tagging each property with scope (block, char, section)

	// see if the property is on the Span item.

	if (pSpanAttrProp)
	{
		p_property = pSpanAttrProp->getPropertyType(pProp->getName(), Type);
		if(p_property)
			return p_property;

		if (bExpandStyles)
		{
			pStyle = _getStyle(pSpanAttrProp, pDoc);

			int i = 0;
			while (pStyle && (i < pp_BASEDON_DEPTH_LIMIT))
			{
				p_property = pStyle->getPropertyType(pProp->getName(), Type);
				if(p_property)
					return p_property;

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
			p_property = pBlockAttrProp->getPropertyType(pProp->getName(), Type);
			if(p_property)
				return p_property;

			if (bExpandStyles)
			{
				pStyle = _getStyle(pBlockAttrProp, pDoc);

				int i = 0;
				while (pStyle && (i < pp_BASEDON_DEPTH_LIMIT))
				{
					p_property = pStyle->getPropertyType(pProp->getName(),  Type);
					if(p_property)
						return p_property;

					pStyle = pStyle->getBasedOn();
					i++;
				}
			}
		}

		if (!pBlockAttrProp || pProp->canInherit())
		{
			if (pSectionAttrProp)
			{
				p_property =  pSectionAttrProp->getPropertyType(pProp->getName(), Type);
				if(p_property)
					return p_property;
			}
		}
	}

	if (pDoc->getStyle("Normal", &pStyle))
	{
		// next to last resort -- check for this property in the Normal style
		p_property = pStyle->getPropertyType(pProp->getName(),  Type);
		if(p_property)
			return p_property;
	}

	// if no inheritance allowed for it or there is no
	// value set in containing block or section, we return
	// the default value for this property.

	return pProp->getInitialType(Type);
}

UT_uint32        PP_getPropertyCount()
{
	return (sizeof(_props)/sizeof(PP_Property));
}

PP_PropName PP_getNthPropertyName(UT_uint32 n)
{
	return _props[n].getName();
}

tPropLevel   PP_getNthPropertyLevel(UT_uint32 n)
{
	return _props[n].getLevel();
}



PP_Property::~PP_Property()
{
}


std::unique_ptr<PP_PropertyType> PP_Property::getInitialType(tProperty_type Type) const
{
	// XXX this used to be cached....
	return PP_PropertyType::createPropertyType(Type, m_pszInitial);
}

std::unique_ptr<PP_PropertyType> PP_PropertyType::createPropertyType(tProperty_type Type, const gchar *p_init)
{
	switch(Type)
	{
	case Property_type_color:
		return std::unique_ptr<PP_PropertyType>(new PP_PropertyTypeColor(p_init));
		break;

	case Property_type_bool:
		return std::unique_ptr<PP_PropertyType>(new PP_PropertyTypeBool(p_init));
		break;

	case Property_type_int:
		return std::unique_ptr<PP_PropertyType>(new PP_PropertyTypeInt(p_init));
		break;

	case Property_type_size:
		return std::unique_ptr<PP_PropertyType>(new PP_PropertyTypeSize(p_init));
		break;

	default:
		UT_ASSERT_HARMLESS(UT_SHOULD_NOT_HAPPEN);
		break;
	}

	return std::unique_ptr<PP_PropertyType>();
}

PP_PropertyTypeColor::PP_PropertyTypeColor(const gchar *p_init)
{
	UT_parseColor(p_init, Color);
}

PP_PropertyTypeBool::PP_PropertyTypeBool(const gchar *p_init)
{
	State = (strcmp("yes", p_init) != 0);
}

PP_PropertyTypeInt::PP_PropertyTypeInt(const gchar *p_init)
{
	Value = atoi(p_init);
}

PP_PropertyTypeSize::PP_PropertyTypeSize(const gchar *p_init)
{
	Value = UT_convertDimensionless(p_init);
	Dim = UT_determineDimension(p_init);
}
