/* AbiSource Program Utilities
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
 


#include <stdio.h>
#include <string.h>

#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"

#include "xap_UnixPSGenerate.h"
#include "xap_UnixPSGraphics.h"
#include "xap_UnixPSFont.h"

#include "xap_UnixFont.h"
#include "xap_UnixFontManager.h"

// the resolution that we report to the application (pixels per inch).
#define PS_RESOLUTION		7200

/*****************************************************************
******************************************************************
** This file contains the Interface between the application and
** the PostScript generation code.
******************************************************************
*****************************************************************/

PS_Graphics::PS_Graphics(const char * szFilename,
						 const char * szTitle,
						 const char * szSoftwareNameAndVersion,
						 AP_UnixFontManager * fontManager,						 
						 UT_Bool	  bIsFile)
{
	UT_ASSERT(szFilename && *szFilename);
	m_szFilename = szFilename;
	m_szTitle = szTitle;
	m_szSoftwareNameAndVersion = szSoftwareNameAndVersion;
	m_pCurrentFont = 0;
	m_bStartPrint = UT_FALSE;
	m_bStartPage = UT_FALSE;
	m_bNeedStroked = UT_FALSE;
	m_bIsFile = bIsFile;
	m_ps = 0;
	m_fm = fontManager;
}

PS_Graphics::~PS_Graphics()
{
	// TODO free stuff
}

UT_Bool PS_Graphics::queryProperties(DG_Graphics::Properties gp) const
{
	switch (gp)
	{
	case DGP_SCREEN:
		return UT_FALSE;
	case DGP_PAPER:
		return UT_TRUE;
	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}
}

void PS_Graphics::setFont(DG_Font* pFont)
{
	UT_ASSERT(pFont);
	PSFont * pNewFont = (static_cast<PSFont*> (pFont));

	// TODO Not always what we want, i.e., start of a new page.
	// TODO I added a call directly to _startPage to call _emit_SetFont();
	// TODO I would rather do it all here.
	if (pNewFont == m_pCurrentFont ) 
		return;
	
	m_pCurrentFont = pNewFont;
	if (m_ps)
		_emit_SetFont();
}

UT_uint32 PS_Graphics::getFontAscent()
{
	UT_ASSERT(m_pCurrentFont);

	GlobalFontInfo * gfi = m_pCurrentFont->getMetricsData()->gfi;
	UT_ASSERT(gfi);

	return _scale(gfi->ascender);
}

UT_uint32 PS_Graphics::getFontDescent()
{
	UT_ASSERT(m_pCurrentFont);

	GlobalFontInfo * gfi = m_pCurrentFont->getMetricsData()->gfi;
	UT_ASSERT(gfi);

	UT_ASSERT(gfi->descender <= 0);
	return _scale(-gfi->descender);
}

UT_uint32 PS_Graphics::getFontHeight()
{
	UT_ASSERT(m_pCurrentFont);

	GlobalFontInfo * gfi = m_pCurrentFont->getMetricsData()->gfi;
	UT_ASSERT(gfi);

	// TODO who takes care of leading ??
	
	UT_ASSERT(gfi->descender <= 0);
	return _scale(gfi->ascender - gfi->descender);
}
	
UT_uint32 PS_Graphics::measureString(const UT_UCSChar* s, int iOffset, 
									int num, unsigned short* pWidths)
{
	UT_ASSERT(m_pCurrentFont);
	UT_ASSERT(s);
	const UT_UCSChar * p = s+iOffset;
	const int * cwi = m_pCurrentFont->getMetricsData()->cwi;

	UT_uint32 iCharWidth = 0;
	for (int k=0; k<num; k++)
	{
		UT_ASSERT(p[k] < 256);			// TODO deal with Unicode
		register int x = _scale(cwi[p[k]]);
		
		iCharWidth += x;
		pWidths[k] = x;
	}
		
	return iCharWidth;
}
	
UT_uint32 PS_Graphics::getResolution() const
{
	return PS_RESOLUTION;
}

void PS_Graphics::setColor(UT_RGBColor& /*clr*/)
{
}

DG_Font* PS_Graphics::getGUIFont()
{
	// getGUIFont is only used for drawing UI widgets, which does not apply on paper.
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

	return NULL;
}

DG_Font* PS_Graphics::findFont(const char* pszFontFamily, 
							   const char* pszFontStyle, 
							   const char* pszFontVariant, 
							   const char* pszFontWeight, 
							   const char* /*pszFontStretch*/, 
							   const char* pszFontSize)
{
	UT_ASSERT(pszFontFamily);
	UT_ASSERT(pszFontStyle);
	UT_ASSERT(pszFontWeight);
	UT_ASSERT(pszFontSize);
	
	// convert styles to AP_UnixFont:: formats
	AP_UnixFont::style s;

	// this is kind of sloppy
	if (!UT_stricmp(pszFontStyle, "normal") &&
		!UT_stricmp(pszFontWeight, "normal"))
	{
		s = AP_UnixFont::STYLE_NORMAL;
	}
	else if (!UT_stricmp(pszFontStyle, "normal") &&
			 !UT_stricmp(pszFontWeight, "bold"))
	{
		s = AP_UnixFont::STYLE_BOLD;
	}
	else if (!UT_stricmp(pszFontStyle, "italic") &&
			 !UT_stricmp(pszFontWeight, "normal"))
	{
		s = AP_UnixFont::STYLE_ITALIC;
	}
	else if (!UT_stricmp(pszFontStyle, "italic") &&
			 !UT_stricmp(pszFontWeight, "bold"))
	{
		s = AP_UnixFont::STYLE_BOLD_ITALIC;
	}
	else
	{
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}

	// Request the appropriate AP_UnixFont::, and bury it in an
	// instance of a UnixFont:: with the correct size.
	AP_UnixFont * unixfont = m_fm->getFont(pszFontFamily, s);
	AP_UnixFont * item = NULL;
	if (unixfont)
	{
		// make a copy
		item = new AP_UnixFont(*unixfont);
	}
	else
	{
		// Oops!  We don't have that font here.  substitute something
		// we know we have (get smarter about this later)
		item = new AP_UnixFont(*m_fm->getFont("Times New Roman", s));
	}
	
	PSFont * pFont = new PSFont(item, convertDimension(pszFontSize));
	UT_ASSERT(pFont);

	// Here we do something different from gr_UnixGraphics::setFont().
	// The PostScript context keeps a simple vector of all its fonts,
	// so that it can run through them and dump them into a document.
	UT_uint32 k, count;
	for (k = 0, count = m_vecFontList.getItemCount(); k < count; k++)
	{
		PSFont * psf = (PSFont *) m_vecFontList.getNthItem(k);
		UT_ASSERT(psf);
		// is this good enough for a match?
		if (psf->getUnixFont() == pFont->getUnixFont() &&
			psf->getSize() == pFont->getSize())
		{
			// don't return the one in the vector, even though
			// it matches, but return the copy, since they're
			// disposable outside our realm.
			return pFont;
		}
	}

	// wasn't already there, add it
	m_vecFontList.addItem((void *) pFont);

	// it's always the last in the list
	UT_uint32 n = m_vecFontList.getItemCount() - 1;
	pFont->setIndex(n);
	
	return pFont;
}

#define OUR_LINE_LIMIT		70			/* DSC3.0 limits to 256 */

void PS_Graphics::drawChars(const UT_UCSChar* pChars, int iCharOffset, 
							int iLength, UT_sint32 xoff, UT_sint32 yoff)
{
	UT_ASSERT(m_pCurrentFont);
	
	char buf[OUR_LINE_LIMIT*2];
	const UT_UCSChar * pS = pChars+iCharOffset;
	const UT_UCSChar * pEnd = pS+iLength;
	char * pD = buf;

	*pD++ = '(';
	while (pS<pEnd)
	{
		if (pD-buf > OUR_LINE_LIMIT)
		{
			*pD++ = '\\';
			*pD++ = '\n';
			*pD++ = 0;
			m_ps->writeBytes(buf);
			pD = buf;
		}

		// TODO deal with Unicode issues.

		switch (*pS)
		{
		case 0x08:		*pD++ = '\\';	*pD++ = 'b';	break;
		case 0x09:		*pD++ = '\\';	*pD++ = 't';	break;
		case 0x0a:		*pD++ = '\\';	*pD++ = 'n';	break;
		case 0x0c:		*pD++ = '\\';	*pD++ = 'f';	break;
		case 0x0d:		*pD++ = '\\';	*pD++ = 'r';	break;
		case '\\':		*pD++ = '\\';	*pD++ = '\\';	break;
		case '(':		*pD++ = '\\';	*pD++ = '(';	break;
		case ')':		*pD++ = '\\';	*pD++ = ')';	break;
		default:		*pD++ = (char)*pS;				break;
		}
		pS++;
	}
	*pD++ = ')';
	sprintf(pD," %ld %ld MS\n",xoff,yoff);
	m_ps->writeBytes(buf);
}

void PS_Graphics::drawLine(UT_sint32 x1, UT_sint32 y1, UT_sint32 x2, UT_sint32 y2)
{
	// TODO This is used for lines in the document, as well as underlines
	// TODO and strikes.
	m_bNeedStroked = UT_TRUE;

	char buf[OUR_LINE_LIMIT*2];
	UT_sint32 nA = getFontAscent();
	sprintf(buf,"%ld %ld %ld %ld ML\n", x2, y2-nA, x1, y1-nA);
	m_ps->writeBytes(buf);
}

void PS_Graphics::xorLine(UT_sint32, UT_sint32, UT_sint32, UT_sint32)
{
}

void PS_Graphics::polyLine(UT_Point * pts, UT_uint32 nPoints)
{
	// polyLine is only used for drawing spell-check squiggles, which does not apply on paper.
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

void PS_Graphics::fillRect(UT_RGBColor& /*c*/, UT_sint32 /*x*/, UT_sint32 /*y*/, UT_sint32 /*w*/, UT_sint32 /*h*/)
{
}

void PS_Graphics::invertRect(const UT_Rect* /*pRect*/)
{
	// invertRect is only used for drawing the selection region, which does not apply on paper.
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

void PS_Graphics::setClipRect(const UT_Rect* /*pRect*/)
{
	// setClipRect is only used for handling screen exposes, which do not happen on paper.
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

void PS_Graphics::clearArea(UT_sint32 /*x*/, UT_sint32 /*y*/,
							UT_sint32 /*width*/, UT_sint32 /*height*/)
{
	// clearArea is only used for re-drawing edited regions, which does not apply on paper.
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

}

void PS_Graphics::scroll(UT_sint32, UT_sint32)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

void PS_Graphics::scroll(UT_sint32 x_dest, UT_sint32 y_dest,
						 UT_sint32 x_src, UT_sint32 y_src,
						 UT_sint32 width, UT_sint32 height)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

UT_Bool PS_Graphics::startPrint(void)
{
	UT_ASSERT(!m_bStartPrint);
	m_bStartPrint = UT_TRUE;
	m_ps = new ps_Generate(m_szFilename);
	if (!m_ps)
		return UT_FALSE;

	return _startDocument();
}

UT_Bool PS_Graphics::startPage(const char * szPageLabel, UT_uint32 pageNumber,
							   UT_Bool bPortrait, UT_uint32 iWidth, UT_uint32 iHeight)
{
	if (m_bStartPage)
		_endPage();
	m_bStartPage = UT_TRUE;
	m_bNeedStroked = UT_FALSE;
	return _startPage(szPageLabel,pageNumber,bPortrait,iWidth,iHeight);
}

UT_Bool PS_Graphics::endPrint(void)
{
	if (m_bStartPage)
		_endPage();
	return _endDocument();
}

/*****************************************************************/
/*****************************************************************/

UT_uint32 PS_Graphics::_scale(UT_uint32 units) const
{
	// we are given a value from the AFM file which are
	// expressed in 1/1000ths of the scaled font.
	// return the number of pixels at our resolution.

	return (units * m_pCurrentFont->getSize() / 1000);
}

UT_Bool PS_Graphics::_startDocument(void)
{
	// open the file and begin writing the prolog
	// return false if an error occurs.

	//////////////////////////////////////////////////////////////////
	// DSC3.0/Prolog/Header
	//////////////////////////////////////////////////////////////////
	
	if (!m_ps->openFile(m_bIsFile))
		return UT_FALSE;

	// we use the argv-version of these so that we get PS-escaping on
	// the strings (which will probably have spaces in them).
	
	if (m_szSoftwareNameAndVersion && *m_szSoftwareNameAndVersion)
		m_ps->formatComment("Creator",&m_szSoftwareNameAndVersion,1);
	if (m_szTitle && *m_szTitle)
		m_ps->formatComment("Title",&m_szTitle,1);

	_emit_DocumentNeededResources();
	
	// TODO add other header-comments here
	
	m_ps->formatComment("EndComments");

	///////////////////////////////////////////////////////////////////
	// DSC3.0/Prolog/ProcedureDefinitions
	///////////////////////////////////////////////////////////////////
		
	m_ps->formatComment("BeginProlog");

	_emit_PrologMacros();
	_emit_FontMacros();
	
	// TODO add rest of prolog

	m_ps->formatComment("EndProlog");

	///////////////////////////////////////////////////////////////////
	// DSC3.0/Script/DocumentSetup
	///////////////////////////////////////////////////////////////////

	m_ps->formatComment("BeginSetup");

	_emit_IncludeResource();
	
	// TODO add other setup stuff

	m_ps->formatComment("EndSetup");
	return UT_TRUE;
}

UT_Bool PS_Graphics::_startPage(const char * szPageLabel, UT_uint32 pageNumber,
								UT_Bool bPortrait, UT_uint32 iWidth, UT_uint32 iHeight)
{
	// emit stuff prior to each page
	
	char buf[1024];
	sprintf(buf,"%ld",pageNumber);

	const char * argv[2] = { szPageLabel, buf };
	m_ps->formatComment("Page",argv,2);

	// TODO add other page-header comments here

	m_ps->formatComment("BeginPageSetup");

	sprintf(buf,"%ld %ld %d %s\n",iWidth,iHeight,PS_RESOLUTION,((bPortrait) ? "BPP" : "BPL"));
	m_ps->writeBytes(buf);
	
	// TODO add page-setup stuff here

	m_ps->formatComment("EndPageSetup");

	// Note, the actual PS for the page will be sent by the
	// individual drawing routings following the EndPageSetup.

	// Note, need to reset font at the beginning of each page
	_emit_SetFont();

	return UT_TRUE;
}

UT_Bool PS_Graphics::_endPage(void)
{
	// emit stuff following each page

	m_ps->formatComment("PageTrailer");

	if(m_bNeedStroked)
		m_ps->writeBytes("stroke\n");
	m_ps->writeBytes("EP\n");

	// TODO add any page-trailer stuff here
	// TODO (this inludes an atend's that we used in the page header)

	// Note, either the next page or the document-trailer will follow this.
	return UT_TRUE;
}

UT_Bool PS_Graphics::_endDocument(void)
{
	// emit the document trailer

	m_ps->formatComment("Trailer");
	
	// TODO add any trailer stuff here
	// TODO (this includes an atend's that we used in the document header)

	UT_Bool bStatus = m_ps->formatComment("EOF");

	m_ps->closeFile();
	delete m_ps;
	m_ps = 0;

	return bStatus;
}

void PS_Graphics::_emit_DocumentNeededResources(void)
{
	UT_Vector vec;
	UT_uint32 k;
	UT_uint32 kLimit = m_vecFontList.getItemCount();

	for (k=0; k<kLimit; k++)
	{
		vec.addItem("font");
		PSFont * psf = (PSFont *)m_vecFontList.getNthItem(k);
		vec.addItem(psf->getMetricsData()->gfi->fontName);
	}

	// TODO add any other resources here
	
	m_ps->formatComment("DocumentNeededResources",&vec);
}

void PS_Graphics::_emit_IncludeResource(void)
{
	UT_uint32 k;
	UT_uint32 kLimit = m_vecFontList.getItemCount();

	for (k=0; k<kLimit; k++)
	{
		PSFont * psf = (PSFont *) m_vecFontList.getNthItem(k);
		
		//m_ps->formatComment("IncludeResource",psf->getMetricsData()->gfi->fontName);

		// Instead of including the resources, we actually splat the fonts
		// into the document.  This looks really slow... perhaps buffer line
		// by line or in larger chunks the font data.
		AP_UnixFont * unixfont = psf->getUnixFont();
		unixfont->openPFA();
		char ch = 0;
		while ((ch = unixfont->getPFAChar()))
			m_ps->writeBytes((UT_Byte *) &ch, 1);
		unixfont->closePFA();
	}

	// TODO add any other IncludeResource's here
}

#define NrElements(a)	(sizeof(a)/sizeof(a[0]))

void PS_Graphics::_emit_PrologMacros(void)
{
	static const char * t[] = {
		"/FSF {findfont exch scalefont} bind def",			// Find & scale a font. <size> /<fontname> FSD
		"/MS  {neg moveto show} bind def",					// Move and draw. (<string>) <x> <y> MS
		"/BP  {gsave 72 exch div dup scale} bind def",		// Begin Page. <res> BP
		"/SZ  {/HH exch def /WW exch def} bind def",		// Page Size.  <w> <h> SZ
		"/BPP {BP SZ 0 HH translate} bind def",				// Begin Portrait Page.  <w> <h> <res> BPP
		"/BPL {BP SZ 90 rotate} bind def",					// Begin Landscape Page. <w> <h> <res> BPP
		"/EP  {grestore showpage} bind def",				// EP
		"/ML  {neg moveto neg lineto} bind def",			// Move and Line. <x2> <y2> <x1> <y1> ML
	};

	char buf[1024];
	
	for (unsigned int k=0; k<NrElements(t); k++)
	{
		sprintf(buf,"  %s\n",t[k]);
		m_ps->writeBytes(buf);
	}
}

void PS_Graphics::_emit_FontMacros(void)
{
	char buf[1024];
	UT_uint32 k;
	UT_uint32 kLimit = m_vecFontList.getItemCount();

	for (k=0; k<kLimit; k++)
	{
		PSFont * psf = (PSFont *)m_vecFontList.getNthItem(k);
		sprintf(buf,"  /F%ld {%ld /%s FSF setfont} bind def\n", k,
				psf->getSize(), psf->getMetricsData()->gfi->fontName);
		m_ps->writeBytes(buf);
	}
}

void PS_Graphics::_emit_SetFont(void)
{
	char buf[1024];
	sprintf(buf,"F%ld\n", m_pCurrentFont->getIndex());
	m_ps->writeBytes(buf);
}
