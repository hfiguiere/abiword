/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2002 Tomas Frydrych, <tomas@frydrych.uklinux.net>
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

#include <ctype.h>
#include <math.h>
#include <string.h>

#include "xap_App.h"
#include "xap_Prefs.h"
#include "gr_Graphics.h"
#include "gr_CharWidths.h"
#include "ut_assert.h"
#include "ut_string.h"
#include "ut_units.h"
#include "ut_sleep.h"
#include "ut_growbuf.h"
#include "ut_debugmsg.h"
#include "ut_OverstrikingChars.h"
#include "gr_Caret.h"

// static class member initializations
#ifndef WITH_PANGO
bool GR_Graphics::m_bRemapGlyphsMasterSwitch = false;
bool GR_Graphics::m_bRemapGlyphsNoMatterWhat = false;
UT_UCSChar GR_Graphics::m_ucRemapGlyphsDefault = 0xB0;
UT_UCSChar *GR_Graphics::m_pRemapGlyphsTableSrc = 0;
UT_UCSChar *GR_Graphics::m_pRemapGlyphsTableDst = 0;
UT_uint32 GR_Graphics::m_iRemapGlyphsTableLen = 0;
#else
PangoContext * GR_Graphics::s_pPangoContext = NULL;
XAP_PangoFontManager * GR_Graphics::s_pPangoFontManager = NULL;
#endif

XAP_PrefsScheme *GR_Graphics::m_pPrefsScheme = 0;
UT_uint32 GR_Graphics::m_uTick = 0;

UT_uint32 GR_Graphics::m_instanceCount = 0;
UT_uint32 GR_Graphics::s_iScreenResolution = 100;

UT_uint32 GR_Font::s_iAllocCount = 0;

#ifndef WITH_PANGO
GR_Font::GR_Font()
{
	s_iAllocCount++;
	m_iAllocNo = s_iAllocCount;
}

GR_Font::~GR_Font()
{
	// need this so children can clean up
}
#endif //WITH_PANGO

GR_Graphics::GR_Graphics()
	: m_pCaret(NULL)
{
	m_pApp = 0;
	m_iZoomPercentage = 100;
#ifdef USE_LAYOUT_UNITS
	m_bLayoutResolutionModeEnabled = false;
#endif
	m_bIsPortrait = true;

	m_pRect = NULL;
	m_bSpawnedRedraw = false;
	m_bExposePending = false;
	m_bIsExposedAreaAccessed = false;
	m_bDontRedraw = false;
	m_bDoMerge = false;
#ifdef WITH_PANGO
	m_pLanguage = NULL;
	// the order of these calls is important !!!
	_initPangoContext();
	_initFontManager();
#endif

	m_iFontAllocNo = 0;
	
	// init the prefs ...
	if(!m_instanceCount)
	{
		
		m_pApp = XAP_App::getApp();
		UT_ASSERT( m_pApp );
		XAP_Prefs *p = m_pApp->getPrefs();
		UT_ASSERT(m_pApp->getPrefs());
		UT_ASSERT(p->getCurrentScheme(false));
		m_pPrefsScheme = p->getCurrentScheme(false);
		m_uTick = m_pPrefsScheme->getTickCount();

		const XML_Char *table_utf8, *default_utf8;
		UT_GrowBuf gb;
		const UT_UCSChar *tbl;
		bool bNoErr = p->getPrefsValueBool((XML_Char*)XAP_PREF_KEY_RemapGlyphsMasterSwitch, &m_bRemapGlyphsMasterSwitch);
		p->getPrefsValueBool((XML_Char*)XAP_PREF_KEY_RemapGlyphsNoMatterWhat, &m_bRemapGlyphsNoMatterWhat);

		bNoErr = p->getPrefsValue((XML_Char*)XAP_PREF_KEY_RemapGlyphsDefault, &default_utf8);
		UT_ASSERT( bNoErr );
		
		UT_decodeUTF8string(default_utf8, UT_XML_strlen(default_utf8), &gb);
		m_ucRemapGlyphsDefault = *(gb.getPointer(0));  // might be null

		bNoErr = p->getPrefsValue((XML_Char*)XAP_PREF_KEY_RemapGlyphsTable, &table_utf8);
		UT_ASSERT( bNoErr );
		
		gb.truncate(0);
		UT_decodeUTF8string(table_utf8, UT_XML_strlen(table_utf8), &gb);

		UT_uint32 doublelength;
		doublelength = gb.getLength();
		UT_uint32 m_iRemapGlyphsTableLen = doublelength / 2;


		if (m_iRemapGlyphsTableLen)
		{
			m_pRemapGlyphsTableSrc = new UT_UCSChar[m_iRemapGlyphsTableLen];
			m_pRemapGlyphsTableDst = new UT_UCSChar[m_iRemapGlyphsTableLen];
			
			tbl = (UT_UCSChar*)gb.getPointer(0);
			for (UT_uint32 tdex=0; tdex<m_iRemapGlyphsTableLen; ++tdex)
			{
				m_pRemapGlyphsTableSrc[tdex] = tbl[2 * tdex];
				m_pRemapGlyphsTableDst[tdex] = tbl[2 * tdex + 1];
				UT_DEBUGMSG(("RemapGlyphsTable[%d]  0x%04x -> 0x%04x\n", tdex, m_pRemapGlyphsTableSrc[tdex], m_pRemapGlyphsTableDst[tdex]));
			}
		}
	}
	
	m_instanceCount++;
}


GR_Graphics::~GR_Graphics()
{
	// need this so children can clean up
	m_instanceCount--;
	DELETEP(m_pCaret);

	if(m_instanceCount == 0)
	{
#ifndef WITH_PANGO
		delete[] m_pRemapGlyphsTableSrc;
		delete[] m_pRemapGlyphsTableDst;
#else
		delete s_pPangoFontManager;
		g_free(s_pPangoContext); // not sure about the g_free here
#endif
	}
}

UT_uint32 GR_Graphics::s_getScreenResolution()
{
  return s_iScreenResolution ;
}

void GR_Graphics::setStaticScreenResolution(UT_uint32 iRes)
{
  s_iScreenResolution = iRes;
}

void GR_Graphics::setLineProperties ( double    inWidthPixels, 
				      JoinStyle inJoinStyle,
				      CapStyle  inCapStyle,
				      LineStyle inLineStyle )
{
  UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

#ifndef WITH_PANGO
UT_uint32 GR_Graphics::getMaxCharacterWidth(const UT_UCSChar*s, UT_uint32 Length)
{
	UT_GrowBufElement *pWidths = new UT_GrowBufElement[Length];

	measureString(s, 0, Length, pWidths);

	UT_sint32 MaxWidth = 0;

	for(UT_uint32 i = 0; i < Length; i++)
	{
		if(pWidths[i] > MaxWidth)
			MaxWidth = pWidths[i];
	}

	DELETEPV(pWidths);

	return MaxWidth;
}

UT_uint32 GR_Graphics::measureString(const UT_UCSChar* s, int iOffset,
										 int num,  UT_GrowBufElement* pWidths)
{
	// Generic base class version defined in terms of measureUnRemappedChar().
	// Platform versions can roll their own if it makes a performance difference.
	UT_ASSERT(s);

	int stringWidth = 0, charWidth;
	for (int i = 0; i < num; i++)
    {
		UT_UCSChar currentChar = remapGlyph(s[i + iOffset], false);

		{
			charWidth = measureUnRemappedChar(currentChar);

			if(charWidth == GR_CW_UNKNOWN)
				charWidth = 0;
			else if(UT_isOverstrikingChar(currentChar) != UT_NOT_OVERSTRIKING && charWidth > 0)
				charWidth = -charWidth;
			
			// if the widths is < 0 we are dealing with an
			// overstriking character, which does not count for
			// the overall width
			if(charWidth > 0)
				stringWidth += charWidth;
		}

		if (pWidths) pWidths[i] = charWidth;
    }
	return stringWidth;
}

UT_UCSChar GR_Graphics::remapGlyph(const UT_UCSChar actual_, bool noMatterWhat)
{
	UT_UCSChar actual = actual_;

	// Here is how the remapGlyph works.

	// * If preference value RemapGlyphsMasterSwitch is not true, no
	// remapping is done.

	// * RemapGlyphsTable is a list of pairs of characters, with the
	// first of each pair being the actual character and the second of
	// each pair being the replacement.  Normally, a character will
	// not be remapped unless it is zero-width.  If it is listed as an
	// actual character in RemapGlyphsTable, then the affiliated
	// replacement character is returned.  If it is not listed in
	// RemapGlyphsTable, then the value of RemapGlyphsDefault is used,
	// though if the default value is null, it is not used (it is
	// tricky to specify null due to XML parsing).

	// * Remapping is not recursive/iterative/whatever.  So, if you
	// remap from a zero-width character to another zero-width
	// character, that's what you get.

	// * If this function is called with noMatterWhat true, or if the
	// preference value RemapGlyphsNoMatterWhat is true, the above algorithm
	// is changed a bit.  Characters listed as actual characters in the table
	// are given replacement values even if they are not zero-width, however
	// the default replacement character is used only if the actual character
	// really has zero-width.  (The intent of the function parameter is that
	// the calling function may have already determined that the character is
	// zero-width, and it may be arbitrarily expensive to check, so a
	// redundant check can be avoided.)  (The intent of the user preference
	// value is to just give a way to force the algorithm's hand "just in
	// case".)

	// * Because we are constantly checking those user preference values
	// (sometimes it might be 2-3 times per character rendered), we cache
	// their values as static members of the GR_Graphics class.  The table of
	// actual and replacement characters is split into two arrays of "src"
	// and "dst" characters, respectively.  If the table had an odd number of
	// characters, the extra dangling character is ignored.

	// * Linear searching is done looking for the actual character, so the
	// implementation is assuming that the RemapGlyphsTable is not too big.
	// Also, because null-terminated UTF8 and Unicode strings are all over
	// the place, you can remap a null character nor remap to a null
	// character.  In office productivity apps, this probably won't be a
	// problem.

	UT_UCSChar remap = actual;
	if (!m_pApp)
	{
		UT_DEBUGMSG(("GR_Graphics::remapGlyph() has no XAP_App*, glyphs not remapped\n"));
		return actual;
	}
#if 0	
	// the prefs scheme and remaping prefs cannot change during the life of the
	// application, can it (i.e., we do not have an ui implemented for
	// that)? In fact, we could insist even on restart
	// if the user wants to change preferences to do with remaping,
	// because this whole thing is extremely costly
	
	XAP_Prefs *p = m_pApp->getPrefs();
	UT_ASSERT(m_pApp->getPrefs());
	XAP_PrefsScheme *s = p->getCurrentScheme(false);
	UT_ASSERT(p->getCurrentScheme(false));
	UT_uint32 t = s->getTickCount();
	if (m_pPrefsScheme != s  ||  m_uTick != t)
	{
		// refresh cached preference values
		UT_DEBUGMSG(("GR_Graphics::remapGlyph() refreshing cached values\n"));
		m_pPrefsScheme = s;
		m_uTick = t;

		p->getPrefsValueBool((XML_Char*)XAP_PREF_KEY_RemapGlyphsMasterSwitch, &m_bRemapGlyphsMasterSwitch);
		p->getPrefsValueBool((XML_Char*)XAP_PREF_KEY_RemapGlyphsNoMatterWhat, &m_bRemapGlyphsNoMatterWhat);

		// we have no mechanism through which we could change the
		// tables at runtime, so this is entirely unnecessary.
		const XML_Char *table_utf8, *default_utf8;
		UT_GrowBuf gb;
		const UT_UCSChar *tbl;

		p->getPrefsValue((XML_Char*)XAP_PREF_KEY_RemapGlyphsDefault, &default_utf8);
		UT_ASSERT(p->getPrefsValue((XML_Char*)XAP_PREF_KEY_RemapGlyphsDefault, &default_utf8));
		UT_decodeUTF8string(default_utf8, UT_XML_strlen(default_utf8), &gb);
		m_ucRemapGlyphsDefault = *(gb.getPointer(0));  // might be null

		p->getPrefsValue((XML_Char*)XAP_PREF_KEY_RemapGlyphsTable, &table_utf8);
		UT_ASSERT(p->getPrefsValue((XML_Char*)XAP_PREF_KEY_RemapGlyphsTable, &table_utf8));
		gb.truncate(0);
		UT_decodeUTF8string(table_utf8, UT_XML_strlen(table_utf8), &gb);

		UT_uint32 doublelength;
		doublelength = gb.getLength();
		UT_uint32 iNewTableLen = doublelength / 2;

		// free these resources -- only if the table is too small
		if(m_iRemapGlyphsTableLen < iNewTableLen)
		{
			DELETEPV(m_pRemapGlyphsTableSrc);
			DELETEPV(m_pRemapGlyphsTableDst);
			m_iRemapGlyphsTableLen = iNewTableLen;
			
			m_pRemapGlyphsTableSrc = new UT_UCSChar[m_iRemapGlyphsTableLen];
			m_pRemapGlyphsTableDst = new UT_UCSChar[m_iRemapGlyphsTableLen];
		}
		
		if (m_iRemapGlyphsTableLen)
		{
			tbl = (UT_UCSChar*)gb.getPointer(0);
			for (UT_uint32 tdex=0; tdex<m_iRemapGlyphsTableLen; ++tdex)
			{
				m_pRemapGlyphsTableSrc[tdex] = tbl[2 * tdex];
				m_pRemapGlyphsTableDst[tdex] = tbl[2 * tdex + 1];
				UT_DEBUGMSG(("RemapGlyphsTable[%d]  0x%04x -> 0x%04x\n", tdex, m_pRemapGlyphsTableSrc[tdex], m_pRemapGlyphsTableDst[tdex]));
			}
		}
	}
#endif		
	if (!m_bRemapGlyphsMasterSwitch
		|| actual == 0x200B  // zero width space
		|| actual == 0xFEFF) // zero width non-breaking space
	{
		return (actual);
	}

	UT_sint32 width = 0xFFFF;
	if (noMatterWhat  ||  m_bRemapGlyphsNoMatterWhat  ||  (width = measureUnRemappedChar(actual))== GR_CW_UNKNOWN)
	{
		bool try_default = true;
		for (UT_uint32 tdex=0; tdex<m_iRemapGlyphsTableLen; ++tdex)
		{
			/*compare with character in unicode, not in current locale*/
			if (actual_ == m_pRemapGlyphsTableSrc[tdex])
			{
				remap = m_pRemapGlyphsTableDst[tdex];
				try_default = false;
				break;
			}
		}
		if (try_default  &&  m_ucRemapGlyphsDefault)
		{
			if (width == 0xFFFF) width = measureUnRemappedChar(actual);
			if (width == GR_CW_UNKNOWN) remap = m_ucRemapGlyphsDefault;
		}
	}
#if 0	
	if (remap != actual)
	{
		UT_DEBUGMSG(("GR_Graphics::remapGlyph( 0x%04X ) -> 0x%04X    tick: %d [%d]\n", actual, remap, t, s));
	}
#endif 
	return remap;
}

#else
UT_sint32 GR_Graphics::getApproxCharWidth()
{
	PangoFontMetrics * pMetrics = pango_font_get_metrics(m_pPangoFont, m_pLanguage);
	UT_sint32 iWidth = pango_font_metrics_get_approximate_char_width(pMetrics);
	pango_font_metrics_unref(pMetrics);
	return iWidth;
}
#endif //#ifndef WITH_PANGO



void GR_Graphics::setZoomPercentage(UT_uint32 iZoom)
{
	UT_ASSERT(iZoom > 0);

	m_iZoomPercentage = iZoom;
#ifndef USE_LAYOUT_UNITS
	_setTransform(m_Transform);
#endif
}

UT_uint32 GR_Graphics::getZoomPercentage(void) const
{
	return m_iZoomPercentage;
}

/*!
 * This method converts a const * string to the appropriate Type 1 Font size.
 * This method should guarentee we get the same sized font for Layout in both
 * Print and Screen contexts.
 * It takes into account if we are in layout resolution mode and in either print
 * or screen contexts.
\params const char *pszFontSize
\returns UT_uint32 the correct font number
*/
UT_uint32 GR_Graphics::getAppropriateFontSizeFromString(const char * pszFontSize)
{
	UT_uint32  iSize;
	UT_uint32  iSizeLayout;
	//double dSize;
	if(queryProperties(DGP_SCREEN))
	{
#ifdef USE_LAYOUT_UNITS
		bool curRes = m_bLayoutResolutionModeEnabled;
		m_bLayoutResolutionModeEnabled = false;
#endif
		iSize = convertDimension(pszFontSize);
#ifdef USE_LAYOUT_UNITS		
		m_bLayoutResolutionModeEnabled = curRes;
		if( m_bLayoutResolutionModeEnabled)
		{
			iSizeLayout = (UT_uint32) ((double ) UT_LAYOUT_UNITS * UT_convertToInches(pszFontSize) +0.05);
			xxx_UT_DEBUGMSG(("SEVIOR: iSizeLayout in gr_graphics = %d \n",iSizeLayout));
			return iSizeLayout;
		}
#endif
		return iSize;
	}
//
// For printing we ignore all zoom stuff and calculate the screen font based on
// inch size.
//
	else
	{
		double dPaperSize = UT_convertToInches(pszFontSize) * (double) getResolution();
		iSize = (UT_sint32) (dPaperSize + 0.05);
#ifdef USE_LAYOUT_UNITS		
		if( m_bLayoutResolutionModeEnabled)
		{
			iSizeLayout = (UT_uint32) ((double ) UT_LAYOUT_UNITS * UT_convertToInches(pszFontSize) +0.05);
			return iSizeLayout;
		}
#endif
		return iSize;
	}
}

UT_uint32 GR_Graphics::getResolution(void) const
{
	return _getResolution() * m_iZoomPercentage / 100;
}

UT_sint32 GR_Graphics::convertDimension(const char * s) const
{
	double dInches = UT_convertToInches(s);
	double dResolution;
#ifdef USE_LAYOUT_UNITS
	if(m_bLayoutResolutionModeEnabled)
	{
		dResolution = UT_LAYOUT_UNITS;
	}
	else
#endif
	{
		dResolution = getResolution();		// NOTE: assumes square pixels/dpi/etc.
	}

	/*
	 We do this because the convert Dimension code is actually
	 lossy in that when we put a conversion of 12pt in at 72DPI
	 then we end up getting 11pt back which really kind of licks.
	 We have to round down to fix bug 1521.
	*/
	return (UT_sint32) ((dInches * dResolution) + 0.05);
}

UT_sint32 GR_Graphics::convertDimension(double Value, UT_Dimension dim) const
{
	double dInches = UT_convertDimToInches(Value, dim);
	double dResolution;
#ifdef USE_LAYOUT_UNITS
	if(m_bLayoutResolutionModeEnabled)
		{
		dResolution = UT_LAYOUT_UNITS;
		}
	else
#endif
		{
		dResolution = getResolution();		// NOTE: assumes square pixels/dpi/etc.
		}

	/*
	 We do this because the convert Dimension code is actually
	 lossy in that when we put a conversion of 12pt in at 72DPI
	 then we end up getting 11pt back which really kind of licks.
	*/
	return (UT_sint32) ((dInches * dResolution) + 0.5);
}

const char * GR_Graphics::invertDimension(UT_Dimension dim, double dValue) const
{
	// return pointer to static buffer -- use it quickly.

	double dResolution;
#ifdef USE_LAYOUT_UNITS	
	if(m_bLayoutResolutionModeEnabled)
		{
		dResolution = UT_LAYOUT_UNITS;
		}
	else
#endif
		{
		dResolution = getResolution();		// NOTE: assumes square pixels/dpi/etc.
		}

	double dInches = dValue / dResolution;

	return UT_convertInchesToDimensionString( dim, dInches);
}

bool GR_Graphics::scaleDimensions(const char * szLeftIn, const char * szWidthIn,
									 UT_uint32 iWidthAvail,
									 UT_sint32 * piLeft, UT_uint32 * piWidth) const
{
	/* Scale the given left-offset and width using the width available.
	** Compute the actual left-offset and actual width used.
	** We allow the given left-offset to be a number.
	** We allow the given width to be a number or "*"; where "*" indicates
	** we take all remaining space available.
	**
	** NOTE: This routine can also be used for vertical calculations.
	*/

	UT_ASSERT(szLeftIn);
	UT_ASSERT(szWidthIn);

	UT_sint32 iLeft = convertDimension(szLeftIn);
	UT_uint32 iWidth;

	if (szWidthIn[0] == '*')
		iWidth = iWidthAvail - iLeft;
	else
		iWidth = convertDimension(szWidthIn);

	if (piLeft)
		*piLeft = iLeft;
	if (piWidth)
		*piWidth = iWidth;

	return true;
}

void GR_Graphics::flush(void)
{
	// default implementation does nothing
}

void GR_Graphics::drawImage(GR_Image* pImg, UT_sint32 xDest, UT_sint32 yDest)
{
   if (pImg)
     pImg->render(this, xDest, yDest);
}

GR_Image* GR_Graphics::createNewImage(const char* pszName, const UT_ByteBuf* pBB, UT_sint32 iDisplayWidth, UT_sint32 iDisplayHeight, GR_Image::GRType iType)
{
   GR_VectorImage * vectorImage = NULL;

   if (iType == GR_Image::GRT_Unknown) {
      if (GR_Image::getBufferType(pBB) == GR_Image::GRT_Vector)
	vectorImage = new GR_VectorImage(pszName);
   }
   else if (iType == GR_Image::GRT_Vector) {
      vectorImage = new GR_VectorImage(pszName);
   }

   if (vectorImage) {
      vectorImage->convertFromBuffer(pBB, iDisplayWidth, iDisplayHeight);
   }

   return vectorImage;
}

bool GR_Graphics::_PtInPolygon(UT_Point * pts,UT_uint32 nPoints,UT_sint32 x,UT_sint32 y)
{
    UT_uint32 i,j;
    bool bResult = false;
    for (i = 0,j = nPoints - 1;i < nPoints;j = i++){
        if ((((pts[i].y <= y) && (y < pts[j].y)) || ((pts[j].y <= y) && (y < pts[i].y))) &&
            (x < (pts[j].x - pts[i].x) * (y - pts[i].y) / (pts[j].y - pts[i].y) + pts[i].x))
        {
            bResult = !bResult;
        }
    }
    return (bResult);
}

void GR_Graphics::polygon(UT_RGBColor& c,UT_Point *pts,UT_uint32 nPoints)
{
    UT_sint32 minX,maxX,minY,maxY,x,y;
    minX = maxX = pts[0].x;
    minY = maxY = pts[0].y;
    for(UT_uint32 i = 0;i < nPoints - 1;i++){
        minX = UT_MIN(minX,pts[i].x);
        maxX = UT_MAX(maxX,pts[i].x);
        minY = UT_MIN(minY,pts[i].y);
        maxY = UT_MAX(maxY,pts[i].y);
    }
    for(x = minX;x <= maxX;x++){
        for(y = minY;y <= maxY;y++){
            if(_PtInPolygon(pts,nPoints,x,y)){
                fillRect(c,x,y,1,1);
            }
        }
    }
 }




/*!
 * Hand shaking variables to let the App know when expose events are being
 * handled.
 * Returns true if the exposed redraw method is running.
 */
const bool  GR_Graphics::isSpawnedRedraw(void) const
{
	return m_bSpawnedRedraw;
}

/*!
 * sets/clears the redraw running variable
 */
void  GR_Graphics::setSpawnedRedraw( bool exposeState)
{
	m_bSpawnedRedraw = exposeState;
}

/*!
 * Informs if there are unprocessed expose information present
 * Returns true is so.
 */
const bool GR_Graphics::isExposePending(void) const
{
	return m_bExposePending;
}

/*!
 * sets the exposed Pending state.
 */
void GR_Graphics::setExposePending(bool exposeState)
{
	m_bExposePending = exposeState;
	if(!exposeState)
	{
		m_PendingExposeArea.left = m_RecentExposeArea.left;
		m_PendingExposeArea.top = m_RecentExposeArea.top;
		m_PendingExposeArea.width = m_RecentExposeArea.width;
		m_PendingExposeArea.height = m_RecentExposeArea.height;
	}
}

/*!
 * Sets the Most recent expose rectangle to something sane after repaint.
 *
 */
void GR_Graphics::setRecentRect(UT_Rect * pRect)
{
		m_RecentExposeArea.left = pRect->left;
		m_RecentExposeArea.top = pRect->top;
	    m_RecentExposeArea.width = pRect->width;
		m_RecentExposeArea.height = pRect->height;
}


/*!
 * Informs if a process is accessing the global merged expose area.
 * Returns true is so.
 */
const bool GR_Graphics::isExposedAreaAccessed(void) const
{
	return m_bIsExposedAreaAccessed;
}

/*!
 * sets the varaible explaining the state of the Merged area
 */
void GR_Graphics::setExposedAreaAccessed(bool exposeState)
{
	m_bIsExposedAreaAccessed = exposeState;
}

/*!
 * Methods to manipulate the expose rectangle.
 */

/*!
 * Set values inside the PendingArea Rectangle
\param x the x-coord of the upper left corner.
\param y the y-coord of the upper left corner.
\param width the width of the rectangle.
\param height the height of the rectangle.
*/
void  GR_Graphics::setPendingRect( UT_sint32 x, UT_sint32 y, UT_sint32 width, UT_sint32 height)
{
	m_PendingExposeArea.set(x,y,width,height);
}

/*!
 * Do a union of the current rectangle with the one presented in the
 * parameter list. The makes the new rectangle the smallest possible that
 * covers both rectangles.
\param  UT_Rect * pRect pointer to the rectangle to merge with.
*/
void  GR_Graphics::unionPendingRect( UT_Rect * pRect)
{
	m_PendingExposeArea.unionRect(pRect);
}

/*!
\returns a const pointer to the PendingExposeArea.
*/
const UT_Rect * GR_Graphics::getPendingRect(void) const
{
	return & m_PendingExposeArea;
}

/*
 * Hand shaking fail safe variable sto make sure we don'tr repaint till
 * we're absolueltely ready.
 */
void  GR_Graphics::setDontRedraw(bool bDontRedraw)
{
	m_bDontRedraw = bDontRedraw;
}


/*
 * Hand shaking fail safe variable sto make sure we don'tr repaint till
 * we're absolueltely ready.
 */
bool GR_Graphics::isDontRedraw(void)
{
	return m_bDontRedraw;
}
/*!
 * Alternate method to tell the doRepaint to merge the next expose so that
 * expands of exposed area due to scrolls can be merged without doing a
 * display update.
 \returns the doMerge boolean (set from scroll)
*/
bool GR_Graphics::doMerge(void) const
{
	return m_bDoMerge;
}

/*!
 * Sets the do merge boolean
 * Set for scroll, clear from doRepaint
 */
void GR_Graphics::setDoMerge( bool bMergeState)
{
	m_bDoMerge = bMergeState;
}

/*!
 * Method to handle expose events with a background repainter. Events that
 * Pass through here are rapidly delt with by either expanding an
 * already existing expose rectangle to cover the expose rectangle of
 *  the current event or if there is no pending expose rectangle, because
 *  the background repainter has cleared it, set a new expose rectangle.
\params UT_Rect *rClip the rectangle of the expose event.
*/
void GR_Graphics::doRepaint( UT_Rect * rClip)
{
//
// Look if we have a pending expose left over.
//
	xxx_UT_DEBUGMSG(("SEVIOR: Starting doRepaint \n"));
	while(isSpawnedRedraw())
	{
		UT_usleep(100);
	}
//
// Stop the repainter
//
	setDontRedraw(true);
//
// Get a lock on the expose rectangle
//
	while(isExposedAreaAccessed())
	{
		UT_usleep(10); // 10 microseconds
	}
	setExposedAreaAccessed(true);
	if(isExposePending() || doMerge())
	{
		//
        // If so merge in the current expose area
        //
		xxx_UT_DEBUGMSG(("Doing a union in expose handler\n"));
		unionPendingRect( rClip);
		setRecentRect(rClip);
		setDoMerge(false);
	}
	else
	{
//
// Otherwise Load the current expose area into the redraw area.
//
		xxx_UT_DEBUGMSG(("Setting Exposed Area in expose handler \n"));
		setPendingRect(rClip->left,rClip->top,rClip->width,rClip->height);
		setRecentRect(rClip);
  	}
//
// Release expose rectangle lock
//
	setExposedAreaAccessed(false);
//
// Tell the repainter there is something to repaint.
//
	setExposePending(true);
//
// Allow the repainter to paint
//
	setDontRedraw(false);
	xxx_UT_DEBUGMSG(("SEVIOR: Finished doRepaint \n"));
//
// OK this event is handled.
//
}

void GR_Graphics::fillRect(const UT_RGBColor& c, const UT_Rect &r)
{
	fillRect(c, r.left, r.top, r.width, r.height);
}

void xorRect(GR_Graphics* pG, UT_sint32 x, UT_sint32 y, UT_sint32 w, UT_sint32 h)
{
	pG->xorLine(x,     y,     x + w, y);
	pG->xorLine(x + w, y,     x + w, y + h);
	pG->xorLine(x + w, y + h, x,     y + h);
	pG->xorLine(x,     y + h, x,     y);
}

void xorRect(GR_Graphics* pG, const UT_Rect& r)
{
	xorRect(pG, r.left, r.top, r.width, r.height);
}

#ifdef DEBUG // XXX
void flash(GR_Graphics* pG, const UT_Rect& r, const UT_RGBColor& c)
{
/*
	UT_RGBColor black(0, 0, 0);

	pG->setColor(c);
	xorRect(pG, r);
	flush();
	UT_usleep(10000);
	xorRect(pG, r);
	flush();
	UT_usleep(1000);
	pG->setColor(black);
*/
}
#endif

#ifdef WITH_PANGO
/*!
  Draws text represented by a GList of PangoGlyphString's
*/
void GR_Graphics::drawPangoGlyphString(GList * pGlyphString,
									   UT_sint32 xoff,
									   UT_sint32 yoff) const
{
	// first, convert the glyphs string to FT_Bitmap
	FT_Bitmap FTBitmap;
	UT_sint32 iWidth = 0;
	UT_sint32 iHeight = 0;

	PangoRectangle inkRect;

	// iterate over the list calculating the total width of the bitmap, keeping track of the
	// intermediate cumulative widths

	GList * pListItem = g_list_first(pGlyphString);
	GList * pWidthList = NULL;

	while (pListItem)
	{
		PangoGlyphString * pGString = (PangoGlyphString *) pListItem->data;
		pango_glyph_string_extents(pGString, m_pPangoFont, &inkRect, NULL);

		// remember the present cumulative width
		pWidthList = g_list_append(pWidthList, (gpointer)iWidth);

		iWidth += inkRect.width;
		iHeight = UT_MAX(iHeight, inkRect.height);

		pListItem = pListItem->next;
	}


	FTBitmap.rows       = PANGO_PIXELS(iHeight);
	FTBitmap.width      = PANGO_PIXELS(iWidth);
	FTBitmap.pixel_mode = ft_pixel_mode_grays;
	FTBitmap.num_grays  = 256;
	FTBitmap.pitch      = 1*FTBitmap.width;
	FTBitmap.buffer     = (unsigned char*)UT_calloc(FTBitmap.rows * FTBitmap.width, 1);

	// now itterate again, creating composite bitmap
	pListItem = g_list_first(pGlyphString);
	GList * pWidthItem = g_list_first(pWidthList);

	while (pListItem)
	{
		UT_ASSERT(pWidthItem);

		PangoGlyphString * pGString = (PangoGlyphString *) pListItem->data;
		UT_sint32 iCumWidth = (UT_sint32) pWidthItem->data;

		pango_ft2_render(&FTBitmap, m_pPangoFont, pGString, iCumWidth , 0 /*??*/);

		pListItem = pListItem->next;
		pWidthItem = pWidthItem->next;
	}


	// now draw it
	_drawFT2Bitmap(xoff, yoff, &FTBitmap);

	FREEP(FTBitmap.buffer);
	g_list_free(pWidthList);
}

/*!
  Draws a single glyph represented by a PangoGlyphString
 */
void GR_Graphics::drawPangoGlyphString(PangoGlyphString * pGlyphString,
									   UT_sint32 xoff,
									   UT_sint32 yoff) const
{
	// first, convert the glyph to FT_Bitmap
	FT_Bitmap FTBitmap;
	PangoRectangle inkRect;

	pango_glyph_string_extents(pGlyphString, m_pPangoFont, &inkRect, NULL);

	FTBitmap.rows       = PANGO_PIXELS(inkRect.height);
	FTBitmap.width      = PANGO_PIXELS(inkRect.width);
	FTBitmap.pixel_mode = ft_pixel_mode_grays;
	FTBitmap.num_grays  = 256;
	FTBitmap.pitch      = 1*FTBitmap.width;
	FTBitmap.buffer     = (unsigned char*)UT_calloc(FTBitmap.rows * FTBitmap.width, 1);

	pango_ft2_render(&FTBitmap, m_pPangoFont, pGlyphString, 0 , 0 /*??*/);

	// now draw it
	_drawFT2Bitmap(xoff, yoff, &FTBitmap);

	FREEP(FTBitmap.buffer);
}


void GR_Graphics::setFont(PangoFont* pFont)
{
	m_pPangoFont = pFont;
}

// note that the font metrics are often language dependent
// at the moment we will ignore this, but one day we might
// want to enable the #if 0 branches below -- this will require
// adding the language as a parameter to these

UT_uint32 GR_Graphics::getFontAscent()
{
	return getFontAscent(m_pPangoFont);
}

UT_uint32 GR_Graphics::getFontAscent(PangoFont * pFont)
{
	PangoFontMetrics * pMetrics = pango_font_get_metrics(pFont, m_pLanguage);
	UT_uint32 iAscent = pMetrics->ascent;
	pango_font_metrics_unref(pMetrics);

	return iAscent;
}

UT_uint32 GR_Graphics::getFontDescent()
{
	return getFontDescent(m_pPangoFont);
}

UT_uint32 GR_Graphics::getFontDescent(PangoFont * pFont)
{
	PangoFontMetrics * pMetrics = pango_font_get_metrics(pFont, m_pLanguage);
	UT_uint32 iDescent = pMetrics->descent;
	pango_font_metrics_unref(pMetrics);

	return iDescent;
}

UT_uint32 GR_Graphics::getFontHeight()
{
	return getFontHeight(m_pPangoFont);
}

UT_uint32 GR_Graphics::getFontHeight(PangoFont * pFont)
{
	PangoFontMetrics * pMetrics = pango_font_get_metrics(pFont, m_pLanguage);
	UT_uint32 iHeight = pMetrics->ascent + pMetrics->descent;
	pango_font_metrics_unref(pMetrics);

	return iHeight;
}

PangoFont* GR_Graphics::findFont(const char* pszFontFamily,
								 const char* pszFontStyle,
								 const char* pszFontVariant,
								 const char* pszFontWeight,
								 const char* pszFontStretch,
								 const char* pszFontSize)
{
	return s_pPangoFontManager->findFont(pszFontFamily, pszFontStyle, pszFontVariant,
										 pszFontWeight, pszFontStretch, pszFontSize);
}


void GR_Graphics::_initPangoContext()
{
	if(!s_pPangoContext)
	{

		s_pPangoContext = _createPangoContext();

		// here we need to do some extra processing, such as setting the base direction
		// default font, etc.
	}

}

void  GR_Graphics::_initFontManager()
{
	if(!s_pPangoFontManager)
		s_pPangoFontManager = _createFontManager();
}

PangoContext * GR_Graphics::_createPangoContext()
{
	double x_dpi = (double)_getResolution();
	double y_dpi = (double)_getResolution();

	return pango_ft2_get_context(x_dpi, y_dpi);
}

XAP_PangoFontManager * GR_Graphics::_createFontManager()
{
	UT_ASSERT(s_pPangoContext);
	return new XAP_PangoFontManager(s_pPangoContext);
}

GList *  GR_Graphics::getPangoGlyphString(const UT_UCS4Char * pChars, UT_uint32 iLen) const
{
	UT_UTF8String text(pChars);

	GList * pGStringList = NULL;

	GList * pItems = pango_itemize(s_pPangoContext, text.utf8_str(), 0, text.byteLength(), NULL, NULL);

	GList * pListItem = g_list_first(pItems);

	while(pListItem)
	{
		PangoItem * pItem = (PangoItem*) pListItem->data;
		PangoGlyphString * pGlyphString = pango_glyph_string_new();
		pGStringList = g_list_append(pGStringList, (gpointer)pGlyphString);

		pango_shape(text.utf8_str() + pItem->offset,pItem->length, &pItem->analysis, pGlyphString);

		pango_item_free(pItem);

		pListItem = pListItem->next;
	}

	g_list_free(pItems);

	return pGStringList;
}


PangoGlyphString * GR_Graphics::getPangoGlyphString(const UT_UCS4Char iChar) const
{
	UT_UTF8String text(&iChar,1);

	GList * pItems = pango_itemize(s_pPangoContext, text.utf8_str(), 0, text.byteLength(), NULL, NULL);

    // since there is only one char, we can use pItem directly

	PangoItem * pItem = (PangoItem*) pItems->data;
	PangoGlyphString * pGlyphString = pango_glyph_string_new();
	pango_shape(text.utf8_str(),pItem->length, &pItem->analysis, pGlyphString);

	pango_item_free(pItem);
	g_list_free(pItems);

	return pGlyphString;
}



void GR_Graphics::setLanguage(const char * lang)
{
	m_pLanguage = pango_language_from_string(lang);
}

/*!
  This function will draw a Unicode string; however, it is very inefficient
  and should be used only for drawing into the GUI; it must not be used for
  drawing the text in the main editing window !!!
*/
void GR_Graphics::drawCharsDirectly(const UT_UCS4Char* pChars,
									UT_uint32 iCharOffset,
									UT_uint32 iLength,
									UT_sint32 xoff,
									UT_sint32 yoff) const
{
	GList * pGlyphs = getPangoGlyphString(pChars + iCharOffset, iLength);
	drawPangoGlyphString(pGlyphs, xoff, yoff);

	g_list_foreach(pGlyphs, UT_free1PangoGlyphString, NULL);
	g_list_free(pGlyphs);
}


#endif

