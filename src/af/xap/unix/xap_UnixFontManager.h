/* AbiSource Application Framework
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

#ifndef XAP_UNIXFONTMANAGER_H
#define XAP_UNIXFONTMANAGER_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>

#ifdef HAVE_FONTCONFIG_FONTCONFIG_H
#include <fontconfig/fontconfig.h>
#endif

#include "ut_types.h"
#include "ut_vector.h"
#include "ut_hash.h"
#include "xap_UnixApp.h"
#include "xap_UnixFont.h"

/*****************************************************************/

class UT_String;

class ABI_EXPORT XAP_UnixFontManager
{
public:
	XAP_UnixFontManager(void);
	~XAP_UnixFontManager(void);

#ifndef USE_XFT
	bool					setFontPath(const char * searchpath);
	XAP_UnixFont *			getDefaultFont16Bit(void);
#endif
	bool					scavengeFonts(void);

	UT_Vector *			    getAllFonts(void);
	XAP_UnixFont *			getDefaultFont(GR_Font::FontFamilyEnum f = GR_Font::FF_Roman);

	XAP_UnixFont *			getFont(const char * fontname,
									XAP_UnixFont::style s);

#ifdef USE_XFT	
	XAP_UnixFont*			searchFont(const char* pszXftName);

	XAP_UnixFont*			findNearestFont(const char* pszFontFamily,
											const char* pszFontStyle,
											const char* pszFontVariant,
											const char* pszFontWeight,
											const char* pszFontStretch,
											const char* pszFontSize);
#endif
	
	void					unregisterFont(XAP_UnixFont * pFont);
	
	// MARCM: this should point to the only instance of XAP_UnixFontManager, 
	// so we can reach our Font Manager from a static context. Without having this static
	// member, font caching can't be implemented without a whole lot of code rewriting
	static XAP_UnixFontManager* pFontManager;

private:

#ifndef USE_XFT
	void					_allocateThisFont(const char * line,
											  const char * workingdir, int iLine);
	void 					_allocateCJKFont(const char * line, int iLine);
#endif
	
	void					_addFont(XAP_UnixFont* font);

	UT_Vector				m_vecFontCache;

	// perhaps this should be a hash to avoid duplicates?
	UT_Vector				m_searchPaths;

	UT_StringPtrMap 		m_fontHash;

#ifndef USE_XFT
	char ** 				m_pExtraXFontPath;
	UT_sint32				m_iExtraXFontPathCount;
#endif

#ifdef USE_XFT
	static FcFontSet*		m_pFontSet;
	static FcConfig*		m_pConfig;
#endif
	XAP_UnixFont*           m_pDefaultFont;
};

#endif /* XAP_UNIXFONTMANAGER_H */

