/* AbiWord
 * Copyright (C) 2004 Tomas Frydrych <tomasfrydrych@yahoo.co.uk>
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

#ifndef GR_WIN32_UNISCRIBE_H
#define GR_WIN32_UNISCRIBE_H

#include "ut_types.h"
#include <windows.h>

// This file contains definitions of Uniscribe structures and function
// prototypes.

/* structures */
#define GRWIN32_DEFINE_USP_STRUCTURES

#ifdef GRWIN32_DEFINE_USP_STRUCTURES
// these are suppossed to be defined in usp10.h, which might be
// included with newer versions of MSVC (not 6.0) in that case we will
// have to undefine the above for those versions of MSVC

typedef struct tagGOFFSET {
  LONG  du;
  LONG  dv;
} GOFFSET;

typedef void *SCRIPT_CACHE;

typedef struct tag_SCRIPT_DIGITSUBSTITUTE {
  DWORD NationalDigitLanguage    : 16;
  DWORD TraditionalDigitLanguage : 16;
  DWORD DigitSubstitute          :8;
  DWORD dwReserved;
} SCRIPT_DIGITSUBSTITUTE;

typedef struct {
  int   cBytes; 
  WORD  wgBlank; 
  WORD  wgDefault; 
  WORD  wgInvalid; 
  WORD  wgKashida; 
  int   iKashidaWidth; 
} SCRIPT_FONTPROPERTIES;

typedef struct tag_SCRIPT_LOGATTR { 
  BYTE fSoftBreak :1; 
  BYTE fWhiteSpace :1; 
  BYTE fCharStop :1; 
  BYTE fWordStop :1; 
  BYTE fInvalid :1; 
  BYTE fReserved :3; 
} SCRIPT_LOGATTR;

typedef struct {
  DWORD   langid              :16;  
  DWORD   fNumeric            :1;
  DWORD   fComplex            :1;
  DWORD   fNeedsWordBreaking  :1;   
  DWORD   fNeedsCaretInfo     :1;
  DWORD   bCharSet            :8;   
  DWORD   fControl            :1;   
  DWORD   fPrivateUseArea     :1;   
  DWORD   fNeedsCharacterJustify :1;
  DWORD   fInvalidGlyph       :1;
  DWORD   fInvalidLogAttr     :1;
  DWORD   fCDM                :1;
  DWORD   fAmbiguousCharSet   :1;
  DWORD   fClusterSizeVaries  :1;
  DWORD   fRejectInvalid      :1;
} SCRIPT_PROPERTIES;

typedef void *SCRIPT_STRING_ANALYSIS;

typedef struct tag_SCRIPT_TABDEF {
  int cTabStops;
  int iScale;
  int *pTabStops;
  int iTabOrigin;
} SCRIPT_TABDEF;

typedef enum tag_SCRIPT_JUSTIFY { 
  SCRIPT_JUSTIFY_NONE = 0, 
  SCRIPT_JUSTIFY_ARABIC_BLANK = 1, 
  SCRIPT_JUSTIFY_CHARACTER = 2, 
  SCRIPT_JUSTIFY_RESERVED1 = 3, 
  SCRIPT_JUSTIFY_BLANK = 4, 
  SCRIPT_JUSTIFY_RESERVED2 = 5, 
  SCRIPT_JUSTIFY_RESERVED3 = 6, 
  SCRIPT_JUSTIFY_ARABIC_NORMAL = 7, 
  SCRIPT_JUSTIFY_ARABIC_KASHIDA = 8, 
  SCRIPT_JUSTIFY_ARABIC_ALEF = 9, 
  SCRIPT_JUSTIFY_ARABIC_HA = 10, 
  SCRIPT_JUSTIFY_ARABIC_RA = 11, 
  SCRIPT_JUSTIFY_ARABIC_BA = 12, 
  SCRIPT_JUSTIFY_ARABIC_BARA = 13, 
  SCRIPT_JUSTIFY_ARABIC_SEEN = 14, 
  SCRIPT_JUSTIFY_ARABIC_SEEN_M = 15, 
} SCRIPT_JUSTIFY; 

typedef struct tag_SCRIPT_VISATTR { 
  WORD uJustification :4; 
  WORD fClusterStart :1; 
  WORD fDiacritic :1; 
  WORD fZeroWidth :1; 
  WORD fReserved :1; 
  WORD fShapeReserved :8; 
} SCRIPT_VISATTR;

typedef struct tag_SCRIPT_CONTROL { 
  DWORD uDefaultLanguage :16; 
  DWORD fContextDigits :1; 
  DWORD fInvertPreBoundDir :1; 
  DWORD fInvertPostBoundDir :1; 
  DWORD fLinkStringBefore :1; 
  DWORD fLinkStringAfter :1; 
  DWORD fNeutralOverride :1; 
  DWORD fNumericOverride :1; 
  DWORD fLegacyBidiClass :1; 
  DWORD fReserved :8; 
} SCRIPT_CONTROL;

typedef struct tag_SCRIPT_STATE { 
  WORD uBidiLevel :5; 
  WORD fOverrideDirection :1; 
  WORD fInhibitSymSwap :1; 
  WORD fCharShape :1; 
  WORD fDigitSubstitute :1; 
  WORD fInhibitLigate :1; 
  WORD fDisplayZWG :1; 
  WORD fArabicNumContext :1; 
  WORD fGcpClusters :1; 
  WORD fReserved :1; 
  WORD fEngineReserved :2; 
} SCRIPT_STATE;

typedef struct tag_SCRIPT_ANALYSIS {
  WORD eScript      :10; 
  WORD fRTL          :1; 
  WORD fLayoutRTL    :1; 
  WORD fLinkBefore   :1; 
  WORD fLinkAfter    :1; 
  WORD fLogicalOrder :1; 
  WORD fNoGlyphIndex :1; 
  SCRIPT_STATE s ; 
} SCRIPT_ANALYSIS;

typedef struct tag_SCRIPT_ITEM { 
  int iCharPos; 
  SCRIPT_ANALYSIS a; 
} SCRIPT_ITEM;

#endif // GRWIN32_DEFINE_USP_STRUCTURES

/* functions -- we are loading these dynamically, so we define them as
   pointers, and prefix t for type */

typedef HRESULT (WINAPI * tScriptApplyDigitSubstitution) (const SCRIPT_DIGITSUBSTITUTE* psds, 
														  SCRIPT_CONTROL* psc, 
														  SCRIPT_STATE* pss);

typedef HRESULT (WINAPI * tScriptApplyLogicalWidth) (const int *piDx,
													int cChars,
													int cGlyphs,
													const WORD *pwLogClust,
													const SCRIPT_VISATTR *psva,
													const int *piAdvance,
													const SCRIPT_ANALYSIS *psa,
													ABC *pABC,
													int *piJustify);

typedef HRESULT (WINAPI * tScriptBreak) (const WCHAR *pwcChars, 
										 int cChars, 
										 const SCRIPT_ANALYSIS *psa, 
										 SCRIPT_LOGATTR *psla);

typedef HRESULT (WINAPI * tScriptCacheGetHeight) (HDC hdc, 
												  SCRIPT_CACHE *psc, 
												  long *tmHeight);

typedef HRESULT (WINAPI * tScriptCPtoX) (int iCP, 
										 BOOL fTrailing, 
										 int cChars, 
										 int cGlyphs, 
										 const WORD *pwLogClust, 
										 const SCRIPT_VISATTR *psva, 
										 const int *piAdvance, 
										 const SCRIPT_ANALYSIS *psa, 
										 int *piX);

typedef HRESULT (WINAPI * tScriptFreeCache) (SCRIPT_CACHE *psc);

typedef HRESULT (WINAPI * tScriptGetCMap) (HDC hdc, 
										   SCRIPT_CACHE *psc, 
										   const WCHAR *pwcInChars, 
										   int cChars, 
										   DWORD dwFlags, 
										   WORD *pwOutGlyphs);

typedef HRESULT (WINAPI * tScriptGetFontProperties) (HDC hdc, 
													 SCRIPT_CACHE *psc, 
													 SCRIPT_FONTPROPERTIES *sfp);

typedef HRESULT (WINAPI * tScriptGetGlyphABCWidth) (HDC hdc, 
													SCRIPT_CACHE *psc,
													WORD wGlyph,
													ABC *pABC);

typedef HRESULT (WINAPI * tScriptGetLogicalWidths) (const SCRIPT_ANALYSIS *psa,
													int cChars,
													int cGlyphs,
													const int *piGlyphWidth,
													const WORD *pwLogClust,
													const SCRIPT_VISATTR *psva,
													int *piDx);

typedef HRESULT (WINAPI * tScriptGetProperties) (const SCRIPT_PROPERTIES ***ppSp, 
												 int *piNumScripts);

typedef HRESULT (WINAPI * tScriptIsComplex) (const WCHAR *pwcInChars,
											 int cInChars,
											 DWORD dwFlags);

typedef HRESULT (WINAPI * tScriptItemize) (const WCHAR *pwcInChars, 
										   int cInChars, 
										   int cMaxItems, 
										   const SCRIPT_CONTROL *psControl, 
										   const SCRIPT_STATE *psState, 
										   SCRIPT_ITEM *pItems, 
										   int *pcItems);


typedef HRESULT (WINAPI * tScriptJustify) (const SCRIPT_VISATTR *psva, 
										   const int *piAdvance, 
										   int cGlyphs, 
										   int iDx, 
										   int iMinKashida, 
										   int *piJustify);

typedef HRESULT (WINAPI * tScriptLayout) (int cRuns, 
										  const BYTE *pbLevel, 
										  int *piVisualToLogical, 
										  int *piLogicalToVisual);

typedef HRESULT (WINAPI * tScriptPlace) (HDC hdc, 
										 SCRIPT_CACHE *psc, 
										 const WORD *pwGlyphs, 
										 int cGlyphs, 
										 const SCRIPT_VISATTR *psva, 
										 SCRIPT_ANALYSIS *psa, 
										 int *piAdvance, 
										 GOFFSET *pGoffset, 
										 ABC *pABC);

typedef HRESULT (WINAPI * tScriptRecordDigitSubstitution) (LCID Locale,
														   SCRIPT_DIGITSUBSTITUTE *psds);

typedef HRESULT (WINAPI * tScriptShape) (HDC hdc, 
										 SCRIPT_CACHE *psc, 
										 const WCHAR *pwcChars, 
										 int cChars, 
										 int cMaxGlyphs, 
										 SCRIPT_ANALYSIS *psa, 
										 WORD *pwOutGlyphs, 
										 WORD *pwLogClust, 
										 SCRIPT_VISATTR *psva, 
										 int *pcGlyphs);

typedef HRESULT (WINAPI * tScriptStringAnalyse) (HDC hdc,
												 const void *pString,
												 int cString,
												 int cGlyphs,
												 int iCharset,
												 DWORD dwFlags,
												 int iReqWidth,
												 SCRIPT_CONTROL *psControl,
												 SCRIPT_STATE *psState,
												 const int *piDx,
												 SCRIPT_TABDEF *pTabdef,
												 const BYTE *pbInClass,
												 SCRIPT_STRING_ANALYSIS *pssa);

typedef HRESULT (WINAPI *tScriptStringCPtoX) (SCRIPT_STRING_ANALYSIS ssa, 
											  int icp, 
											  BOOL fTrailing, 
											  int *pX);

typedef HRESULT (WINAPI * tScriptStringFree) (SCRIPT_STRING_ANALYSIS *pssa);

typedef HRESULT (WINAPI * tScriptStringGetLogicalWidths) (SCRIPT_STRING_ANALYSIS ssa, int *piDx);

typedef HRESULT (WINAPI * tScriptStringGetOrder) (SCRIPT_STRING_ANALYSIS ssa, UINT *puOrder);

typedef HRESULT (WINAPI * tScriptStringOut) (SCRIPT_STRING_ANALYSIS ssa, 
											 int iX, 
											 int iY, 
											 UINT uOptions, 
											 const RECT *prc, 
											 int iMinSel, 
											 int iMaxSel, 
											 BOOL fDisabled);

typedef const int* (WINAPI * tScriptString_pcOutChars) (SCRIPT_STRING_ANALYSIS ssa);

typedef const SCRIPT_LOGATTR* (WINAPI * tScriptString_pLogAttr) (SCRIPT_STRING_ANALYSIS ssa);

typedef const SIZE* (WINAPI * tScriptString_pSize) (SCRIPT_STRING_ANALYSIS ssa);

typedef HRESULT (WINAPI * tScriptStringValidate) (SCRIPT_STRING_ANALYSIS ssa);

typedef HRESULT (WINAPI * tScriptStringXtoCP) (SCRIPT_STRING_ANALYSIS  ssa, 
											   int iX, 
											   int *piCh, 
											   int *piTrailing);

typedef HRESULT (WINAPI * tScriptTextOut) (const HDC hdc, 
										   SCRIPT_CACHE *psc, 
										   int x, 
										   int y, 
										   UINT fuOptions, 
										   const RECT *lprc, 
										   const SCRIPT_ANALYSIS *psa, 
										   const WCHAR *pwcReserved, 
										   int iReserved, 
										   const WORD *pwGlyphs, 
										   int cGlyphs, 
										   const int *piAdvance, 
										   const int *piJustify, 
										   const GOFFSET *pGoffset);

typedef HRESULT (WINAPI * tScriptXtoCP) (int iX, 
										 int cChars, 
										 int cGlyphs, 
										 const WORD *pwLogClust, 
										 const SCRIPT_VISATTR *psva, 
										 const int *piAdvance, 
										 const SCRIPT_ANALYSIS *psa, 
										 int *piCP, 
										 int *piTrailing);

#endif
