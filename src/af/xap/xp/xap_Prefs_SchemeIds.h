/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */

/* AbiSource Application Framework
 * Copyright (C) 1998-2002 AbiSource, Inc.
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

/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.
******************************************************************
*****************************************************************/

#ifndef XAP_PREFS_SCHEMEIDS_H
#define XAP_PREFS_SCHEMEIDS_H

//////////////////////////////////////////////////////////////////////////////////////
// The following are the set of scheme-based application-independent preference keys
// and the set of default values for them.	Each item must have the XAP_PREF_KEY_ prefix
// and each value must have the XAP_PREF_DEFAULT_ prefix.  Default values *must* obey
// XML encoding rules if they contain any double quote (&quot;), ampersand (&amp;),
// or angle bracket (&lt; and &gt;) characters.
//
// ***FOR EACH PAIR DEFINED, ADD A 'dcl(basename)' TO THE BOTTOM HALF OF THIS FILE***
//
// Note: Additional keys may be defined by the application.
//////////////////////////////////////////////////////////////////////////////////////

#define XAP_PREF_KEY_ToolbarAppearance		"ToolbarAppearance"
#define XAP_PREF_DEFAULT_ToolbarAppearance	"icon"						/* {icon,text,both} */

#define XAP_PREF_KEY_UnixFontPath			"UnixFontPath"
#define XAP_PREF_DEFAULT_UnixFontPath		"fonts" 					/* if relative path, prepend "getAbiSuiteLibDir()" */

#define XAP_PREF_KEY_RemapGlyphsMasterSwitch			"RemapGlyphsMasterSwitch"
 /* completely ignore glyph remapping if false */
#define XAP_PREF_DEFAULT_RemapGlyphsMasterSwitch		"1"

#define XAP_PREF_KEY_RemapGlyphsNoMatterWhat			"RemapGlyphsNoMatterWhat"
 /* if true, do remap even if originals aren't zero-width */
#define XAP_PREF_DEFAULT_RemapGlyphsNoMatterWhat	"0"

#define XAP_PREF_KEY_RemapGlyphsDefault 			"RemapGlyphsDefault"
 /* default replacement glyph for originals not mentioned in the table */
#define XAP_PREF_DEFAULT_RemapGlyphsDefault 		"&#x00B0;"
/* degree symbol */

#define XAP_PREF_KEY_RemapGlyphsTable				"RemapGlyphsTable"
/* pairwise table of originals and replacements, arbitrarily many pairs */
#	define XAP_PREF_DEFAULT_RemapGlyphsTable			"&#x2018;`&#x2019;'&#x201c;&quot;&#x201d;&quot;"
/* smart quotes */

#define XAP_PREF_KEY_SmartQuotesEnable				"SmartQuotesEnable"
/* substitute curly smart quotes on the fly */
#define XAP_PREF_DEFAULT_SmartQuotesEnable			"0"

#define XAP_PREF_KEY_UseSuffix						"UseSuffix"
/* append suffixes to saved files */
#define XAP_PREF_DEFAULT_UseSuffix					"1"

#define XAP_PREF_KEY_DefaultPageSize				"DefaultPageSize"
#define XAP_PREF_DEFAULT_DefaultPageSize			 "Letter"

#define XAP_PREF_KEY_DefaultDirectionRtl	"DefaultDirectionRtl"

#ifndef BIDI_RTL_DOMINANT
#define XAP_PREF_DEFAULT_DefaultDirectionRtl		 "0"
#else
#define XAP_PREF_DEFAULT_DefaultDirectionRtl		 "1"
#endif

#define XAP_PREF_KEY_UseContextGlyphs				 "UseContextGlyphs"
#define XAP_PREF_DEFAULT_UseContextGlyphs			 "1"
#define XAP_PREF_KEY_SaveContextGlyphs				 "SaveContextGlyphs"
#define XAP_PREF_DEFAULT_SaveContextGlyphs			 "0"
#define XAP_PREF_KEY_UseHebrewContextGlyphs 		 "UseGlyphShapingForHebrew"
#define XAP_PREF_DEFAULT_UseHebrewContextGlyphs 	 "0"

#define XAP_PREF_KEY_AutoSaveFile					"AutoSaveFile"
#define XAP_PREF_DEFAULT_AutoSaveFile				"0" 						/* Auto save files by default */

#define XAP_PREF_KEY_AutoSaveFilePeriod 				"AutoSaveFilePeriod"
#define XAP_PREF_DEFAULT_AutoSaveFilePeriod 			"5" 						/* Auto save files by default */

#define XAP_PREF_KEY_AutoSaveFileExt					"AutoSaveFileExt"
#ifdef WIN32
#define XAP_PREF_DEFAULT_AutoSaveFileExt			".bak"
#else
#define XAP_PREF_DEFAULT_AutoSaveFileExt			".bak~"
#endif

#define XAP_PREF_KEY_EmbedFontsInPS 				"EmbedFontsInPs"
#define XAP_PREF_DEFAULT_EmbedFontsInPS 			"1"

#define XAP_PREF_KEY_ColorForTransparent			"TransparentColor"
#define XAP_PREF_DEFAULT_ColorForTransparent		"ffffff"

#define XAP_PREF_KEY_ColorForShowPara				"ColorShowPara"
#define XAP_PREF_DEFAULT_ColorForShowPara			"7f7f7f"
#define XAP_PREF_KEY_ColorForSquiggle				"ColorSquiggle"
#define XAP_PREF_DEFAULT_ColorForSquiggle			"ff0000"
#define XAP_PREF_KEY_ColorForMargin					"ColorMargin"
#define XAP_PREF_DEFAULT_ColorForMargin				"7f7f7f"
#define XAP_PREF_KEY_ColorForSelBackground			"ColorSelBackground"
#define XAP_PREF_DEFAULT_ColorForSelBackground		"c0c0c0"
#define XAP_PREF_KEY_ColorForFieldOffset			"ColorFieldOffset"
#define XAP_PREF_DEFAULT_ColorForFieldOffset		"0c0c0c"
#define XAP_PREF_KEY_ColorForImage					"ColorImage"
#define XAP_PREF_DEFAULT_ColorForImage				"0000ff"
#define XAP_PREF_KEY_ColorForHyperLink				"ColorHyperLink"
#define XAP_PREF_DEFAULT_ColorForHyperLink			"0000ff"
#define XAP_PREF_KEY_ColorForHdrFtr					"ColorHdrFtr"
#define XAP_PREF_DEFAULT_ColorForHdrFtr				"000000"
#define XAP_PREF_KEY_ColorForColumnLine				"ColorColumnLine"
#define XAP_PREF_DEFAULT_ColorForColumnLine			"000000"

#define XAP_PREF_KEY_ColorForRevision1				"ColorRevision1"
#define XAP_PREF_DEFAULT_ColorForRevision1			"ab04fe"
#define XAP_PREF_KEY_ColorForRevision2				"ColorRevision2"
#define XAP_PREF_DEFAULT_ColorForRevision2			"ab1477"
#define XAP_PREF_KEY_ColorForRevision3				"ColorRevision3"
#define XAP_PREF_DEFAULT_ColorForRevision3			"ff9708"
#define XAP_PREF_KEY_ColorForRevision4				"ColorRevision4"
#define XAP_PREF_DEFAULT_ColorForRevision4			"9eb345"
#define XAP_PREF_KEY_ColorForRevision5				"ColorRevision5"
#define XAP_PREF_DEFAULT_ColorForRevision5			"0fb305"
#define XAP_PREF_KEY_ColorForRevision6				"ColorRevision6"
#define XAP_PREF_DEFAULT_ColorForRevision6			"08b3f8"
#define XAP_PREF_KEY_ColorForRevision7				"ColorRevision7"
#define XAP_PREF_DEFAULT_ColorForRevision7			"04cec3"
#define XAP_PREF_KEY_ColorForRevision8				"ColorRevision8"
#define XAP_PREF_DEFAULT_ColorForRevision8			"0485c3"
#define XAP_PREF_KEY_ColorForRevision9				"ColorRevision9"
#define XAP_PREF_DEFAULT_ColorForRevision9			"0712c3"
#define XAP_PREF_KEY_ColorForRevision10				"ColorRevision10"
#define XAP_PREF_DEFAULT_ColorForRevision10			"ff0000"

#define XAP_PREF_KEY_DocumentLocale "DocumentLocale"
#define XAP_PREF_DEFAULT_DocumentLocale "en-US"

#define XAP_PREF_KEY_DefaultSaveDirectory "DefaultSaveDirectory"
#define XAP_PREF_DEFAULT_DefaultSaveDirectory "" /* empty for $PWD basically */

#define XAP_PREF_KEY_ZoomType		"ZoomType"
#define XAP_PREF_DEFAULT_ZoomType	"Width" /* 100, Width, Page */

#define XAP_PREF_KEY_ShowUnixFontWarning "ShowUnixFontWarning"
#define XAP_PREF_DEFAULT_ShowUnixFontWarning "1"

#define XAP_PREF_KEY_AllowCustomToolbars "AllowCustomToolbars"
#define XAP_PREF_DEFAULT_AllowCustomToolbars		 "1"

#define XAP_PREF_KEY_AutoLoadPlugins				 "AutoLoadPlugins"
#define XAP_PREF_DEFAULT_AutoLoadPlugins			 "1"

#define XAP_PREF_KEY_ToolbarNumEntries "Toolbar_NumEntries_"
#define XAP_PREF_KEY_ToolbarID "Toolbar_ID_"
#define XAP_PREF_KEY_ToolbarFlag "Toolbar_Flag_"

#define XAP_PREF_KEY_ModifyUnixFontPath  "ModifyUnixFontPath"
#define XAP_PREF_DEFAULT_ModifyUnixFontPath "1"

#define XAP_PREF_KEY_HTMLExportOptions  "HTML_Export_Options"
#define XAP_PREF_DEFAULT_HTMLExportOptions "?xml,xmlns:awml,+CSS"

#else /* XAP_PREFS_SCHEMEID_H */
#ifdef dcl

dcl(ToolbarAppearance)
dcl(UnixFontPath)
dcl(RemapGlyphsMasterSwitch)
dcl(RemapGlyphsNoMatterWhat)
dcl(RemapGlyphsDefault)
dcl(RemapGlyphsTable)
dcl(SmartQuotesEnable)
dcl(UseSuffix)
dcl(DefaultPageSize)
dcl(DefaultDirectionRtl)
dcl(UseContextGlyphs)
dcl(SaveContextGlyphs)
dcl(UseHebrewContextGlyphs)
dcl(AutoSaveFile)
dcl(AutoSaveFilePeriod)
dcl(AutoSaveFileExt)
dcl(EmbedFontsInPS)
dcl(ColorForTransparent)
dcl(ColorForShowPara)
dcl(ColorForSquiggle)
dcl(ColorForMargin)
dcl(ColorForSelBackground)
dcl(ColorForFieldOffset)
dcl(ColorForImage)
dcl(ColorForHyperLink)
dcl(ColorForHdrFtr)
dcl(ColorForColumnLine)
dcl(ColorForRevision1)
dcl(ColorForRevision2)
dcl(ColorForRevision3)
dcl(ColorForRevision4)
dcl(ColorForRevision5)
dcl(ColorForRevision6)
dcl(ColorForRevision7)
dcl(ColorForRevision8)
dcl(ColorForRevision9)
dcl(ColorForRevision10)
dcl(DocumentLocale)
dcl(AllowCustomToolbars)
dcl(AutoLoadPlugins)
dcl(ZoomType)
dcl(DefaultSaveDirectory)
dcl(ShowUnixFontWarning)
dcl(ModifyUnixFontPath)
dcl(HTMLExportOptions)

#endif /* dcl */
#endif /* XAP_PREFS_SCHEMEID_H */
