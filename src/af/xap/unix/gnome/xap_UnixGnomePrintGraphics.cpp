/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* AbiSource Application Framework
 * Copyright (C) 2003 Dom Lachowicz
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
#include <ctype.h>

#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ut_misc.h"
#include "xap_Strings.h"
#include "xap_UnixGnomePrintGraphics.h"
#include "gr_UnixImage.h"
#include "ut_string_class.h"
#include "xap_UnixDialogHelper.h"

#include <libgnomeprintui/gnome-print-job-preview.h>

/***********************************************************************/
/*      This file provides an interface into Gnome Print               */
/***********************************************************************/

static bool isItalic(XAP_UnixFont::style s)
{
	return ((s == XAP_UnixFont::STYLE_ITALIC) || (s == XAP_UnixFont::STYLE_BOLD_ITALIC));
}

static GnomeFontWeight getGnomeFontWeight(XAP_UnixFont::style s)
{
	if (s == XAP_UnixFont::STYLE_BOLD_ITALIC ||
		s == XAP_UnixFont::STYLE_BOLD)
		return GNOME_FONT_BOLD;
	return GNOME_FONT_BOOK;
}

static const struct {
	char * abi;
	char * gnome;
} paperMap[] = {
	{"Letter", "US-Letter"},
	{"Legal",  "US-Legal"},
	{"Folio",  "Executive"}, // i think that this is correct
	{NULL,     NULL}
};

static const guchar * mapPageSize (const char * sz)
{
	if (!sz && !*sz)
		return gnome_print_paper_get_default ()->name;
	
	for (gsize i = 0; i < NrElements(paperMap); i++)
		if (paperMap[i].abi && !g_ascii_strcasecmp (sz, paperMap [i].abi))
			return (const guchar *)paperMap[i].gnome;

	// default to whatever was passed in
	return (const guchar *)sz;
}

XAP_UnixGnomePrintGraphics::XAP_UnixGnomePrintGraphics(GnomePrintContext *gpc,
													   XAP_UnixFontManager * fontManager,
													   XAP_App *pApp)
{
	m_gpm = NULL;
	m_gpc = gpc;
	m_paper = NULL;
	
	m_bIsPreview   = false;
	m_fm           = fontManager;
	m_bStartPrint  = false;
	m_bStartPage   = false;
	m_pCurrentFont = NULL;
	m_pCurrentPSFont = NULL;
	
	m_cs = GR_Graphics::GR_COLORSPACE_COLOR;
}

XAP_UnixGnomePrintGraphics::XAP_UnixGnomePrintGraphics(GnomePrintJob *gpm,
													   const char * pageSize,
													   XAP_UnixFontManager * fontManager,
													   XAP_App *pApp,
													   bool isPreview)
{
	m_pApp         = pApp;
	m_gpm          = gpm;
	m_gpc          = gnome_print_job_get_context(gpm);
	
	// TODO: be more robust about this
	const GnomePrintPaper * paper = gnome_print_paper_get_by_name(mapPageSize (pageSize));
	
	if (!paper)
		paper = gnome_print_paper_get_default ();
	
	m_paper = paper;

#if 0
	gnome_print_master_set_paper(m_gpm, paper);
#endif	

	m_bIsPreview   = isPreview;
	m_fm           = fontManager;
	m_bStartPrint  = false;
	m_bStartPage   = false;
	m_pCurrentFont = NULL;
	m_pCurrentPSFont = NULL;
	
	m_cs = GR_Graphics::GR_COLORSPACE_COLOR;
}

GnomeFont * XAP_UnixGnomePrintGraphics::_allocGnomeFont(PSFont* pFont)
{
	XAP_UnixFont *uf          = pFont->getUnixFont();
	XAP_UnixFont::style style = uf->getStyle();
	
	return gnome_font_find_closest_from_weight_slant((const guchar *)uf->getName(), getGnomeFontWeight(style), 
													 isItalic(style), (double)pFont->getSize());
}

XAP_UnixGnomePrintGraphics::~XAP_UnixGnomePrintGraphics()
{
	if (m_pCurrentFont != NULL && GNOME_IS_FONT(m_pCurrentFont))
		gnome_font_unref(m_pCurrentFont);
}

UT_uint32 XAP_UnixGnomePrintGraphics::measureUnRemappedChar(const UT_UCSChar c)
{
	return static_cast<UT_uint32>(m_pCurrentPSFont->getUnixFont()->measureUnRemappedChar(c, m_pCurrentPSFont->getSize()) * getResolution() / getDeviceResolution());
}

void XAP_UnixGnomePrintGraphics::drawGlyph (UT_uint32 Char, UT_sint32 xoff, UT_sint32 yoff)
{
	UT_ASSERT_NOT_REACHED ();
}

void XAP_UnixGnomePrintGraphics::getCoverage (UT_Vector& coverage)
{
	UT_ASSERT_NOT_REACHED ();
}

void XAP_UnixGnomePrintGraphics::drawChars(const UT_UCSChar* pChars, 
										   int iCharOffset, int iLength,
										   UT_sint32 xoff, UT_sint32 yoff,
										   int * pCharWidths)
{
	// The GR classes are expected to take yoff as the upper-left of
	// each glyph.  PostScript interprets the yoff as the baseline,
	// which doesn't match this expectation.  Adding the ascent of the
	// font will bring it back to the correct position.
	yoff += getFontAscent();

	UT_UTF8String utf8 (pChars + iCharOffset, iLength);

	// push a graphics state & save it. then set the font
	gnome_print_gsave (m_gpc);
	gnome_print_setfont (m_gpc, m_pCurrentFont);

	gnome_print_moveto(m_gpc, tdu(xoff), tdu(yoff));
	gnome_print_show_sized (m_gpc, (const guchar *)utf8.utf8_str(), utf8.byteLength());

	// pop the graphics state
	gnome_print_grestore (m_gpc);
}

void XAP_UnixGnomePrintGraphics::drawLine (UT_sint32 x1, UT_sint32 y1,
										   UT_sint32 x2, UT_sint32 y2)
{
	gnome_print_moveto (m_gpc, tdu(x1), tdu(y1));
	gnome_print_lineto (m_gpc, tdu(x2), tdu(y2));
	gnome_print_stroke (m_gpc);
}

void XAP_UnixGnomePrintGraphics::setFont(GR_Font* pFont)
{
	PSFont * psFont = (static_cast<PSFont*> (pFont));
	m_pCurrentPSFont = psFont;

	if(m_pCurrentFont && GNOME_IS_FONT(m_pCurrentFont))
		gnome_font_unref (m_pCurrentFont);

	m_pCurrentFont = _allocGnomeFont(psFont);
}

bool XAP_UnixGnomePrintGraphics::queryProperties(GR_Graphics::Properties gp) const
{
	switch (gp)
		{
		case DGP_SCREEN:
		case DGP_OPAQUEOVERLAY:
			return false;
		case DGP_PAPER:
			return true;
		default:
			UT_ASSERT_NOT_REACHED ();
			return false;
		}
}

void XAP_UnixGnomePrintGraphics::getColor(UT_RGBColor& clr)
{
	clr = m_currentColor;
}

void XAP_UnixGnomePrintGraphics::setColor(const UT_RGBColor& clr)
{
	if (m_currentColor == clr)
		return;

	m_currentColor = clr;

	double red   = (double)(m_currentColor.m_red / 255.0);
	double green = (double)(m_currentColor.m_grn / 255.0);
	double blue  = (double)(m_currentColor.m_blu / 255.0);
	gnome_print_setrgbcolor(m_gpc,red,green,blue);
}

void XAP_UnixGnomePrintGraphics::setLineWidth(UT_sint32 iLineWidth)
{
 	m_dLineWidth = tduD((double)iLineWidth);
	gnome_print_setlinewidth (m_gpc, m_dLineWidth); 
}

bool XAP_UnixGnomePrintGraphics::startPrint(void)
{
	UT_ASSERT(!m_bStartPrint);
	m_bStartPrint = true;
	return _startDocument();
}

bool XAP_UnixGnomePrintGraphics::startPage (const char *szPageLabel,
											UT_uint32 pageNo, bool portrait, 
											UT_uint32 width, UT_uint32 height)
{
	width  = tdu (width);
	height = tdu (height);

	if (m_bStartPage)
	  _endPage();
	m_bStartPage = true;
	m_bNeedStroked = false;
	return _startPage(szPageLabel);
}

bool XAP_UnixGnomePrintGraphics::endPrint()
{
	if (m_bStartPage)
	  _endPage();
	return _endDocument();
}

void XAP_UnixGnomePrintGraphics::setColorSpace(GR_Graphics::ColorSpace c)
{
	m_cs = c;
}

GR_Graphics::ColorSpace XAP_UnixGnomePrintGraphics::getColorSpace(void) const
{
	return m_cs;
}

UT_uint32 XAP_UnixGnomePrintGraphics::getDeviceResolution(void) const
{
	return 72; // 300
}

void XAP_UnixGnomePrintGraphics::_drawAnyImage (GR_Image* pImg, 
												UT_sint32 xDest, 
												UT_sint32 yDest, bool rgb)
{
	UT_sint32 iDestWidth  = pImg->getDisplayWidth ();
	UT_sint32 iDestHeight = pImg->getDisplayHeight ();

	GR_UnixImage * pImage = static_cast<GR_UnixImage *>(pImg);
	GdkPixbuf * image = pImage->getData ();
	UT_return_if_fail (image);

	gnome_print_gsave (m_gpc);
	gnome_print_translate (m_gpc, xDest, yDest + iDestHeight);
	gnome_print_scale (m_gpc, ((double) iDestWidth), ((double) iDestHeight));
	
	gint width, height, rowstride;
	const guchar * pixels;
	
	width     = gdk_pixbuf_get_width (image);
	height    = gdk_pixbuf_get_height (image);
	rowstride = gdk_pixbuf_get_rowstride (image);
	pixels    = gdk_pixbuf_get_pixels (image);

	/* Not sure about the grayimage part, but the other 2 are correct */
	if (!rgb)
		gnome_print_grayimage (m_gpc, pixels, width, height, rowstride);
	else if (gdk_pixbuf_get_has_alpha (image))
		gnome_print_rgbaimage (m_gpc, pixels, width, height, rowstride);
	else
		gnome_print_rgbimage (m_gpc, pixels, width, height, rowstride);

	gnome_print_grestore(m_gpc);
}

void XAP_UnixGnomePrintGraphics::drawImage(GR_Image* pImg, UT_sint32 xDest, 
										   UT_sint32 yDest)
{
	xDest = tdu(xDest);
	yDest = tdu(yDest);

   	if (pImg->getType() != GR_Image::GRT_Raster) 
	    pImg->render(this, xDest, yDest);
	else {
		switch(m_cs)
			{
			case GR_Graphics::GR_COLORSPACE_COLOR:
				_drawAnyImage(pImg, xDest, yDest, true);
				break;
			case GR_Graphics::GR_COLORSPACE_GRAYSCALE:
			case GR_Graphics::GR_COLORSPACE_BW:
				_drawAnyImage(pImg, xDest, yDest, false);
				break;
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			}
	}
}

GR_Image* XAP_UnixGnomePrintGraphics::createNewImage(const char* pszName, 
													 const UT_ByteBuf* pBB, 
													 UT_sint32 iDisplayWidth,
													 UT_sint32 iDisplayHeight, 
													 GR_Image::GRType iType)
{
	GR_Image* pImg = NULL;

   	if (iType == GR_Image::GRT_Raster)
		pImg = new GR_UnixImage(pszName,iType);
   	else if (iType == GR_Image::GRT_Vector)
		pImg = new GR_VectorImage(pszName);
   
	pImg->convertFromBuffer(pBB, tdu(iDisplayWidth), tdu(iDisplayHeight));

	return pImg;
}

/***********************************************************************/
/*                          Protected things                           */
/***********************************************************************/

bool XAP_UnixGnomePrintGraphics::_startDocument(void)
{
	return true;
}

bool XAP_UnixGnomePrintGraphics::_startPage(const char * szPageLabel)
{
	if (!m_gpm)
		return true ;
	
	gnome_print_beginpage(m_gpc, (const guchar *)szPageLabel);
	return true;
}

bool XAP_UnixGnomePrintGraphics::_endPage(void)
{
	if(m_bNeedStroked)
		gnome_print_stroke(m_gpc);
	
	if (!m_gpm)
		return true;
	
	gnome_print_showpage(m_gpc);
	return true;
}

bool XAP_UnixGnomePrintGraphics::_endDocument(void)
{
	// bonobo version, we'd don't own the context or the master, just return
	if(!m_gpm)
		return true;

	gnome_print_job_close(m_gpm);
		
	if(!m_bIsPreview)
		gnome_print_job_print(m_gpm);
	else
		{
			const XAP_StringSet * pSS = m_pApp->getStringSet();
			GtkWidget * preview = gnome_print_job_preview_new (m_gpm, 
															   pSS->getValue(XAP_STRING_ID_DLG_UP_PrintPreviewTitle));
#if 0
			g_signal_connect (G_OBJECT (preview), "unrealize",
			  G_CALLBACK (gtk_main_quit), NULL);
#endif
			gtk_widget_show(GTK_WIDGET(preview));
		}
	
	g_object_unref(G_OBJECT(m_gpm));
	return true;
}

UT_uint32 XAP_UnixGnomePrintGraphics::_getResolution(void) const
{
	return 72;
}

void XAP_UnixGnomePrintGraphics::fillRect(const UT_RGBColor& c, 
										  UT_sint32 x, UT_sint32 y, 
										  UT_sint32 w, UT_sint32 h)
{
	// set the bgcolor
	UT_RGBColor old (m_currentColor);
	setColor (c);
	
	x = tdu(x); y = tdu(y); w = tdu(w); h = tdu(h);

	gnome_print_newpath (m_gpc);
	gnome_print_moveto (m_gpc, x,   y);		
	gnome_print_lineto (m_gpc, x+w, y);
	gnome_print_lineto (m_gpc, x+w, y+h);
	gnome_print_lineto (m_gpc, x,   y+h);
	gnome_print_lineto (m_gpc, x,   y);
	gnome_print_closepath (m_gpc);
	gnome_print_fill (m_gpc);
	
	// reset color to its original state
	setColor (old);
}

void XAP_UnixGnomePrintGraphics::setClipRect(const UT_Rect* pRect)
{
	UT_ASSERT_NOT_REACHED ();
}

/***********************************************************************/
/*                    Things that souldn't happen                      */
/***********************************************************************/

void XAP_UnixGnomePrintGraphics::setCursor(GR_Graphics::Cursor)
{
	UT_ASSERT_NOT_REACHED ();
}

GR_Graphics::Cursor XAP_UnixGnomePrintGraphics::getCursor(void) const
{
	UT_ASSERT_NOT_REACHED ();
	return GR_CURSOR_INVALID;
}

void XAP_UnixGnomePrintGraphics::xorLine(UT_sint32, UT_sint32, UT_sint32, 
										 UT_sint32)
{
	UT_ASSERT_NOT_REACHED ();
}

void XAP_UnixGnomePrintGraphics::polyLine(UT_Point * /* pts */, 
										  UT_uint32 /* nPoints */)
{
	UT_ASSERT_NOT_REACHED ();
}

void XAP_UnixGnomePrintGraphics::invertRect(const UT_Rect* /*pRect*/)
{
	UT_ASSERT_NOT_REACHED ();
}

void XAP_UnixGnomePrintGraphics::clearArea(UT_sint32 /*x*/, UT_sint32 /*y*/,
										   UT_sint32 /*width*/, UT_sint32 /*height*/)
{
	UT_ASSERT_NOT_REACHED ();
}

void XAP_UnixGnomePrintGraphics::scroll(UT_sint32, UT_sint32)
{
	UT_ASSERT_NOT_REACHED ();
}

void XAP_UnixGnomePrintGraphics::scroll(UT_sint32 /* x_dest */,
										UT_sint32 /* y_dest */,
										UT_sint32 /* x_src */,
										UT_sint32 /* y_src */,
										UT_sint32 /* width */,
										UT_sint32 /* height */)
{
	UT_ASSERT_NOT_REACHED ();
}

UT_RGBColor * XAP_UnixGnomePrintGraphics::getColor3D(GR_Color3D /*c*/)
{
	UT_ASSERT_NOT_REACHED ();
	return NULL;
}

void XAP_UnixGnomePrintGraphics::setColor3D(GR_Color3D /*c*/)
{
	UT_ASSERT_NOT_REACHED ();
}

GR_Font* XAP_UnixGnomePrintGraphics::getGUIFont()
{
	UT_ASSERT_NOT_REACHED ();
	return NULL;
}

void XAP_UnixGnomePrintGraphics::fillRect(GR_Color3D c, UT_sint32 x, UT_sint32 y, UT_sint32 w, UT_sint32 h)
{
	UT_ASSERT_NOT_REACHED ();
}

void XAP_UnixGnomePrintGraphics::fillRect(GR_Color3D c, UT_Rect &r)
{
	UT_ASSERT_NOT_REACHED ();
}

/***********************************************************************/
/*                                 Done                                */
/***********************************************************************/

UT_uint32 XAP_UnixGnomePrintGraphics::getFontAscent(GR_Font *fnt)
{
	PSFont*	hndl = static_cast<PSFont*> (fnt);
	// FIXME we should really be getting stuff fromt he font in layout units,
	// FIXME but we're not smart enough to do that yet
	// we call getDeviceResolution() to avoid zoom
	return static_cast<UT_uint32>(hndl->getUnixFont()->getAscender(hndl->getSize()) * getResolution() / getDeviceResolution() + 0.5);
}

UT_uint32 XAP_UnixGnomePrintGraphics::getFontAscent()
{
	return getFontAscent(static_cast<GR_Font *>(m_pCurrentPSFont));
}

UT_uint32 XAP_UnixGnomePrintGraphics::getFontDescent(GR_Font *fnt)
{
	PSFont*	psfnt = static_cast<PSFont*> (fnt);
	// FIXME we should really be getting stuff fromt he font in layout units,
	// FIXME but we're not smart enough to do that yet
	return static_cast<UT_uint32>(psfnt->getUnixFont()->getDescender(psfnt->getSize()) * getResolution() / getDeviceResolution() + 0.5);
}

UT_uint32 XAP_UnixGnomePrintGraphics::getFontDescent()
{
	return getFontDescent(static_cast<GR_Font *>(m_pCurrentPSFont));
}

UT_uint32 XAP_UnixGnomePrintGraphics::getFontHeight()
{
	return (getFontAscent() + getFontDescent());
}

UT_uint32 XAP_UnixGnomePrintGraphics::getFontHeight(GR_Font *fnt)
{
	return (getFontAscent(fnt) + getFontDescent(fnt));
}

GR_Font* XAP_UnixGnomePrintGraphics::findFont(const char* pszFontFamily, 
											  const char* pszFontStyle, 
											  const char* /* pszFontVariant */,
											  const char* pszFontWeight, 
											  const char* /* pszFontStretch */,
											  const char* pszFontSize)
{
	// convert styles to XAP_UnixFont:: formats
	XAP_UnixFont::style s = XAP_UnixFont::STYLE_NORMAL;

	// this is kind of sloppy
	if (!UT_strcmp(pszFontStyle, "normal") &&
		!UT_strcmp(pszFontWeight, "normal"))
		s = XAP_UnixFont::STYLE_NORMAL;
	else if (!UT_strcmp(pszFontStyle, "normal") &&
			 !UT_strcmp(pszFontWeight, "bold"))
		s = XAP_UnixFont::STYLE_BOLD;
	else if (!UT_strcmp(pszFontStyle, "italic") &&
			 !UT_strcmp(pszFontWeight, "normal"))
		s = XAP_UnixFont::STYLE_ITALIC;
	else if (!UT_strcmp(pszFontStyle, "italic") &&
			 !UT_strcmp(pszFontWeight, "bold"))
		s = XAP_UnixFont::STYLE_BOLD_ITALIC;
	else {
		UT_ASSERT_NOT_REACHED ();
	}
	
	// Request the appropriate XAP_UnixFont::, and bury it in an
	// instance of a UnixFont:: with the correct size.
	XAP_UnixFont * unixfont = m_fm->getFont(pszFontFamily, s);
	XAP_UnixFontHandle * item = NULL;
	
	UT_uint32 iSize = static_cast<UT_uint32>(UT_convertToPoints(pszFontSize));

	// end up burying a pointer to the unix font, but first we need a handle
	item = new XAP_UnixFontHandle(unixfont,iSize);

	PSFont * pFont = new PSFont(item->getUnixFont(), iSize);
    delete item;

	return pFont;
}

void XAP_UnixGnomePrintGraphics::setPageSize(char* pageSizeName, UT_uint32 iwidth, UT_uint32 iheight)
{
	UT_ASSERT (UT_TODO);
}

static int
joinToPS (GR_Graphics::JoinStyle js)
{
  switch(js)
    {
    case GR_Graphics::JOIN_MITER: 
      return 0;
    case GR_Graphics::JOIN_ROUND: 
      return 1;
    case GR_Graphics::JOIN_BEVEL: 
      return 2;
    }

  return 1;
}

static int
capToPS (GR_Graphics::CapStyle cs)
{
  switch (cs)
    {
    case GR_Graphics::CAP_BUTT: 
      return 0;
    case GR_Graphics::CAP_ROUND: 
      return 1;
    case GR_Graphics::CAP_PROJECTING: 
      return 2;
    }

  return 1;
}

static const double*
dashToPS (GR_Graphics::LineStyle ls, gint & n_values, double &offset)
{
	static const double on_off_dash [] = {1., 1.};
	static const double double_dash [] = {1., 2.};

	switch(ls)
		{
		case GR_Graphics::LINE_SOLID:
			offset = 0.; n_values = 0; return NULL;
		case GR_Graphics::LINE_ON_OFF_DASH: 
			offset = 0.; n_values = 2; return on_off_dash;
		case GR_Graphics::LINE_DOUBLE_DASH: 
			offset = 0.; n_values = 2; return double_dash;
		case GR_Graphics::LINE_DOTTED: 
			UT_ASSERT(UT_TODO);
			offset = 0.; n_values = 0; return NULL;
		}
	
	n_values = 0; offset = 0.; return NULL;
}

void XAP_UnixGnomePrintGraphics::setLineProperties (double inWidthPixels,
													JoinStyle inJoinStyle,
													CapStyle inCapStyle,
													LineStyle inLineStyle)
{

	gnome_print_setlinejoin (m_gpc, joinToPS(inJoinStyle));
	gnome_print_setlinecap (m_gpc, capToPS(inCapStyle));

	gint n_values = 0;
	double offset = 0;
	const double * dash = NULL;

	dash = dashToPS (inLineStyle, n_values, offset);
	gnome_print_setdash (m_gpc, n_values, dash, offset);

#if 0
	// rounded up and most certainly ignored for now!!!
	m_iLineWidth = (UT_uint32)tdu(ceil(inWidthPixels));
#endif
}

/***********************************************************************/
/*                Private Scaling Conversion Routines                  */
/***********************************************************************/
