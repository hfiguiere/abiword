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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gr_BeOSGraphics.h"

#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_misc.h"
#include "ut_string.h"

#define DPRINTF(x) 	
#if defined(USE_BACKING_BITMAP)
#define UPDATE_VIEW			m_pFrontView->Window()->Lock();				\
							m_pFrontView->DrawBitmap(m_pShadowBitmap);	\
							m_pFrontView->Sync();						\
							m_pFrontView->Window()->Unlock();
#else
//Do a flush instead of a sync
#define UPDATE_VIEW			m_pShadowView->Window()->Lock();			\
							m_pShadowView->Sync();						\
							m_pShadowView->Window()->Unlock();
#endif

GR_BEOSGraphics::GR_BEOSGraphics(BView *docview) {
	m_pShadowView = NULL;
	m_pShadowBitmap = NULL;
	m_pBeOSFont = NULL;
	m_pFrontView = docview;
	if (!m_pFrontView)
		return;

#if defined(USE_BACKING_BITMAP)
	m_pFrontView->Window()->Lock();
	BRect r = m_pFrontView->Bounds();
	m_pFrontView->Window()->Unlock();
	if (!(m_pShadowBitmap = new BBitmap(r, B_RGB32, true, false))) {
		UT_ASSERT(0);
		return;
	}
	if (!(m_pShadowView = new BView(r, "ShadowView", NULL, NULL))) {
		UT_ASSERT(0);
		return; 
	}
	m_pShadowBitmap->Lock();
	m_pShadowBitmap->AddChild(m_pShadowView);
	m_pShadowBitmap->Unlock();
#else
	m_pShadowView = m_pFrontView;
#endif
}		

GR_BEOSGraphics::~GR_BEOSGraphics() {
#if defined(USE_BACKING_BITMAP)
	if (!m_pShadowBitmap)
		return;
	m_pShadowBitmap->Lock();
	m_pShadowBitmap->RemoveChild(m_pShadowView);
	m_pShadowBitmap->Unlock();
	delete m_pShadowBitmap;
	delete m_pShadowView;
#endif
}

void GR_BEOSGraphics::ResizeBitmap(BRect r) {
#if defined(USE_BACKING_BITMAP)
	if (m_pShadowBitmap) {
		m_pShadowBitmap->Lock();
		m_pShadowBitmap->RemoveChild(m_pShadowView);
		m_pShadowBitmap->Unlock();
		//Don't really need to nuke the View, just resize
		delete m_pShadowBitmap;	
		delete m_pShadowView;	
	}
	
	if (!(m_pShadowBitmap = new BBitmap(r, B_RGB32, true, false))) {
		UT_ASSERT(0);
		return;
	}
	if (!(m_pShadowView = new BView(r, "ShadowView", NULL, NULL))) {
		UT_ASSERT(0);
		return; 
	}
	m_pShadowBitmap->Lock();
	m_pShadowBitmap->AddChild(m_pShadowView);
	m_pShadowBitmap->Unlock();
#endif
}

UT_Bool GR_BEOSGraphics::queryProperties(GR_Graphics::Properties gp) const
{
	switch (gp)
	{
	case DGP_SCREEN:
		return UT_TRUE;
	case DGP_PAPER:
		return UT_FALSE;
	default:
		UT_ASSERT(0);
		return UT_FALSE;
	}
}

//Draw this string of characters on the screen in current font
void GR_BEOSGraphics::drawChars(const UT_UCSChar* pChars, int iCharOffset,
							 int iLength, UT_sint32 xoff, UT_sint32 yoff)
{
	int i;

/*     I wonder if this is expecting the 
	   value to be drawn in the middle
	   of the ascent and descent values?
	   |____
	   |     Ascent
	   |----String
	   |----String
	   |____ Descent
	  
*/

	//Someday learn to output 16 bit values ...
	DPRINTF(printf("GR: Draw Chars\n"));
	char *buffer = new char[iLength +1];
	for (i=0; i<iLength; i++) {
		buffer[i] = (char)pChars[i+iCharOffset];
	}
	buffer[i] = '\0';

	m_pShadowView->Window()->Lock();
	//Should I manipulate high and low colour here?
	//rgb_color old = m_pShadowView->LowColor();
	//m_pShadowView->SetLowColor(m_pShadowView->HighColor());

	//This is really strange ... I have to offset all the 
	//text by the ascent, descent, leading values ...
	font_height fh;
	m_pShadowView->GetFontHeight(&fh);
	//int offset = (int)(fh.ascent + fh.descent + fh.leading + 0.5);
	int offset = (int)(fh.ascent + 0.5);
	m_pShadowView->DrawString(buffer, BPoint(xoff, yoff + offset));

	//m_pShadowView->SetLowColor(old);
	m_pShadowView->Window()->Unlock();
	delete [] buffer;
	
	UPDATE_VIEW
}

BFont *findClosestFont(const char* pszFontFamily, 
		 			 	const char* pszFontStyle, 
						const char* pszFontWeight) {

	BFont 	*aFont = new BFont();
	font_family family;
	font_style style; 
	uint32 flags, i; 

	//Try and get a good family ... or one that matches
	int32 numFamilies = count_font_families(); 
   	for (i = 0; i < numFamilies; i++ ) { 
       uint32 flags; 
       if ((get_font_family(i, &family, &flags) == B_OK) &&  
           (strcmp(family, pszFontFamily) == 0)) {
           DPRINTF(printf("Font Match %s and %s \n", family, pszFontFamily));
           break;
       }
	}
	if (i >= numFamilies) {
		strcpy(family, "Dutch801 Rm BT");
		DPRINTF(printf("Didn't find font to match %s \n", pszFontFamily));
	}

#define REGULAR_BIT	0x1
#define ROMAN_BIT	0x2
#define BOLD_BIT	0x4
#define ITALIC_BIT	0x8
#define BOLD_ITALIC_BIT	0x10
	
	//Then try and match the styles 
	//(sub normal for Regular if possible, Roman if required)
	int32 numStyles = count_font_styles(family); 
	int32 stylemask = 0;
	for ( int32 j = 0; j < numStyles; j++ ) { 
		if ( get_font_style(family, j, &style, &flags) == B_OK ) {
			if (strcmp(style, "Regular") == 0) 
				stylemask |= REGULAR_BIT;
			else if (strcmp(style, "Roman") == 0)
				stylemask |= ROMAN_BIT;
			else if (strcmp(style, "Italic") == 0)
				stylemask |= ITALIC_BIT;
			else if (strcmp(style, "Bold") == 0)
				stylemask |= BOLD_BIT;
			else if (strcmp(style, "Bold Italic") == 0)
				stylemask |= BOLD_ITALIC_BIT;			
		} 
	}
	
	int32 targetstyle = 0;								 
	if ((strcmp(pszFontStyle, "italic") == 0) &&
	    (strcmp(pszFontWeight, "bold") == 0)) {
	    targetstyle |= BOLD_BIT | ITALIC_BIT | BOLD_ITALIC_BIT;
	}
	else if (strcmp(pszFontStyle, "italic") == 0) 
		targetstyle |= ITALIC_BIT;
	else if (strcmp(pszFontWeight, "bold") == 0) 
		targetstyle |= BOLD_BIT;
	else 
		targetstyle |= ROMAN_BIT | REGULAR_BIT;
		
	//Search order preference
	//Bold Italic --> Bold Italic; 
	//Bold    --> Bold
	//Italic  --> Italic
	//Regular --> Normal/Roman
	if (targetstyle & stylemask & BOLD_ITALIC_BIT) 
		strcpy(style, "Bold Italic");
	else if (targetstyle & stylemask & ITALIC_BIT) 
		strcpy(style, "Italic");
	else if (targetstyle & stylemask & BOLD_BIT) 
		strcpy(style, "Bold");
	else if (targetstyle & stylemask & ROMAN_BIT) 
		strcpy(style, "Roman");
	else if (targetstyle & stylemask & REGULAR_BIT) 
		strcpy(style, "Regular");

	DPRINTF(printf("Setting Style %s \n", style));
	
	aFont->SetFamilyAndStyle((strlen(family) == 0) ? NULL : family, 
	                         (strlen(style) == 0) ? NULL : style);
	return(aFont);
}

GR_Font* GR_BEOSGraphics::getGUIFont(void)
{
	if (!m_pFontGUI)
	{
		BFont *aFont = new BFont();
		m_pFontGUI = new BeOSFont(aFont);
	}
	return m_pFontGUI;
}

GR_Font* GR_BEOSGraphics::findFont(const char* pszFontFamily, 
								const char* pszFontStyle, 
								const char* /*pszFontVariant*/, 
								const char* pszFontWeight, 
								const char* /*pszFontStretch*/, 
								const char* pszFontSize)
{
	BFont 	*aFont;
	int 	size = atoi(pszFontSize);
	DPRINTF(printf("GR Find Font:\n\tFamily: %s \n\tStyle: %s \n\tWeight: %s \n\tSize: %s (%d)\n",
			pszFontFamily, pszFontStyle, pszFontWeight, pszFontSize, size));
	
	aFont = findClosestFont(pszFontFamily, pszFontStyle, pszFontWeight);
	aFont->SetSize(size);
	DPRINTF(printf("GR: -- Located Font: \n"));
	DPRINTF(aFont->PrintToStream());
	m_pBeOSFont = new BeOSFont(aFont);
	return(m_pBeOSFont);
}

//Set the font to something (I guess we set pFont to be like BFont somewhere)
void GR_BEOSGraphics::setFont(GR_Font* pFont)
{
	BeOSFont *tmpFont;
	
	DPRINTF(printf("GR: Set Font\n"));
	tmpFont = static_cast<BeOSFont*> (pFont);
	UT_ASSERT(tmpFont);
	
	m_pBeOSFont = tmpFont;
	m_pShadowView->Window()->Lock();
	m_pShadowView->SetFont(m_pBeOSFont->get_font());
	m_pShadowView->Window()->Unlock();
}

//Get the height of the font
UT_uint32 GR_BEOSGraphics::getFontHeight()
{
	font_height fh;
	m_pShadowView->Window()->Lock();
	m_pShadowView->GetFontHeight(&fh);
	m_pShadowView->Window()->Unlock();
	DPRINTF(printf("GR: Get Font Height %d\n",(int)(fh.ascent + fh.descent + fh.leading + 0.5)));
	//Gives ascent, descent, leading
	return((UT_uint32)(fh.ascent + fh.descent + fh.leading + 0.5));
}

UT_uint32 GR_BEOSGraphics::getFontAscent()
{
	font_height fh;

	m_pShadowView->Window()->Lock();
	m_pShadowView->GetFontHeight(&fh);
	m_pShadowView->Window()->Unlock();
	//Gives ascent, descent, leading
	DPRINTF(printf("GR: Font Ascent %d\n",(int)(fh.ascent + 0.5)));
	return((UT_uint32)(fh.ascent + 0.5));
}

UT_uint32 GR_BEOSGraphics::getFontDescent()
{
	font_height fh;
	m_pShadowView->Window()->Lock();
	m_pShadowView->GetFontHeight(&fh);
	m_pShadowView->Window()->Unlock();
	//Gives ascent, descent, leading
	DPRINTF(printf("GR: Font Descent %d\n",(int)(fh.descent + 0.5)));
	return((UT_uint32)(fh.descent + 0.5));
}

UT_uint32 GR_BEOSGraphics::measureString(const UT_UCSChar* s, int iOffset,
									  int num,  unsigned short* pWidths)
{
	DPRINTF(printf("GR: Measure String\n"));
	UT_uint32	size, i;	
	char 		*buffer = new char[num+1];
	if (!buffer) {
		return(0);
	}
	
	//Set the character, then set the length of the character
	memset(buffer, 0, num+1*sizeof(char));
	for (i=0; i<num; i++) {
		buffer[i] = (char)(s[i+iOffset]);						
		pWidths[i] = m_pShadowView->StringWidth(&buffer[i]);				
	}
	size = m_pShadowView->StringWidth(buffer);
	delete [] buffer;
	return(size);
	
//	m_pShadowView->StringWidth(&s[iOffset]);
/*	
	int iCharWidth = 0;
	for (int i=0; i<num; i++)
	{
		// TODO should this assert be s[i+iOffset] ??
		UT_ASSERT(s[i] < 256);	// TODO we currently cannot deal with Unicode properly

		iCharWidth += m_aCharWidths[s[i + iOffset]];
		pWidths[i] = m_aCharWidths[s[i + iOffset]];
	}
	return iCharWidth;
*/
}

UT_uint32 GR_BEOSGraphics::_getResolution() const
{
	return 75;
}

void GR_BEOSGraphics::setColor(UT_RGBColor& clr)
{
	DPRINTF(printf("GR: setColor\n"));
	m_pShadowView->Window()->Lock();
	m_pShadowView->SetHighColor(clr.m_red, clr.m_grn, clr.m_blu);
	m_pShadowView->Window()->Unlock();
}

void GR_BEOSGraphics::drawLine(UT_sint32 x1, UT_sint32 y1, UT_sint32 x2,
							UT_sint32 y2)
{
	DPRINTF(printf("GR: Draw Line\n"));
	m_pShadowView->Window()->Lock();
	m_pShadowView->StrokeLine(BPoint(x1, y1), BPoint(x2, y2));
	m_pShadowView->Window()->Unlock();

	UPDATE_VIEW
}

void GR_BEOSGraphics::setLineWidth(UT_sint32 iLineWidth)
{
	//m_iLineWidth = iLineWidth;
}

void GR_BEOSGraphics::polyLine(UT_Point * pts, UT_uint32 nPoints)
{
	printf("GR: Poly Line \n");	
/*
	GdkPoint * points = (GdkPoint *)calloc(nPoints, sizeof(GdkPoint));
	UT_ASSERT(points);

	for (UT_uint32 i = 0; i < nPoints; i++)
	{
		points[i].x = pts[i].x;
		// It seems that Windows draws each pixel along the the Y axis
		// one pixel beyond where GDK draws it (even though both coordinate
		// systems start at 0,0 (?)).  Subtracting one clears this up so
		// that the poly line is in the correct place relative to where
		// the rest of UNIXGraphics:: does things (drawing text, clearing
		// areas, etc.).
		points[i].y = pts[i].y - 1;	
	}

	gdk_draw_lines(m_pWin, m_pGC, points, nPoints);

	FREEP(points);
*/
}


void GR_BEOSGraphics::xorLine(UT_sint32 x1, UT_sint32 y1, UT_sint32 x2,
			    UT_sint32 y2)
{
	DPRINTF(printf("GR: XOR Line\n"));
	m_pShadowView->Window()->Lock();
	drawing_mode oldmode = m_pShadowView->DrawingMode();
	m_pShadowView->SetDrawingMode(B_OP_INVERT);	//or B_OP_BLEND
	m_pShadowView->StrokeLine(BPoint(x1, y1), BPoint(x2, y2));
	m_pShadowView->SetDrawingMode(oldmode);
	m_pShadowView->Window()->Unlock();

	UPDATE_VIEW
}

void GR_BEOSGraphics::invertRect(const UT_Rect* pRect)
{
	DPRINTF(printf("GR: Invert Rect\n"));
	m_pShadowView->Window()->Lock();
	drawing_mode oldmode = m_pShadowView->DrawingMode();
	m_pShadowView->SetDrawingMode(B_OP_INVERT);	//or B_OP_BLEND
	m_pShadowView->StrokeRect(BRect(pRect->left, pRect->top,
									pRect->left + pRect->width,
									pRect->top + pRect->height));
	m_pShadowView->SetDrawingMode(oldmode);
	m_pShadowView->Window()->Unlock();

	UPDATE_VIEW
}

void GR_BEOSGraphics::fillRect(UT_RGBColor& c, UT_Rect &r) {
	fillRect(c,r.left,r.top,r.width,r.height);
}


void GR_BEOSGraphics::fillRect(UT_RGBColor& c, UT_sint32 x, UT_sint32 y,
							UT_sint32 w, UT_sint32 h)
{
	DPRINTF(printf("GR: Flll Rect\n"));
	m_pShadowView->Window()->Lock();
	rgb_color old_colour = m_pShadowView->HighColor();
	m_pShadowView->SetHighColor(c.m_red, c.m_grn, c.m_blu);
	m_pShadowView->FillRect(BRect(x, y, x+w, y+h));
	m_pShadowView->SetHighColor(old_colour);
	m_pShadowView->Window()->Unlock();
	
	UPDATE_VIEW
}

void GR_BEOSGraphics::setClipRect(const UT_Rect* pRect)
{
	/*
	printf("GR: Set Clip Rect: %d-%d -> %d-%d\n", 
			pRect->left, pRect->top, 
			pRect->left+pRect->width,
			pRect->top+pRect->height);
	*/
}

void GR_BEOSGraphics::scroll(UT_sint32 dx, UT_sint32 dy)
{
	printf("GR: Scroll dx %d dy %d\n", dx, dy);
	m_pShadowView->Window()->Lock();
	//If we are moving down, right offset positive
/*
	BRect r = m_pShadowView->Bounds();
	(dy < 0) ? (r.top -= dy) : (r.bottom -= dy);
	(dx < 0) ? (r.left -= dx) : (r.right -= dx);
	printf("Invalidating "); r.PrintToStream();
	m_pShadowView->Invalidate(r);
*/
	m_pShadowView->Invalidate();
	m_pShadowView->Window()->Unlock();
}

void GR_BEOSGraphics::scroll(UT_sint32 x_dest, UT_sint32 y_dest,
						  UT_sint32 x_src, UT_sint32 y_src,
						  UT_sint32 width, UT_sint32 height)
{
	printf("GR: Move Area\n");
/*
	gdk_window_copy_area(m_pWin, m_pGC, x_dest, y_dest,
						 m_pWin, x_src, y_src, width, height);
*/
}

void GR_BEOSGraphics::clearArea(UT_sint32 x, UT_sint32 y,
			     UT_sint32 width, UT_sint32 height)
{
	printf("GR: Clear Area %d-%d -> %d-%d\n", x, y, x+width, y+height);
	m_pShadowView->Window()->Lock();
	rgb_color old_colour = m_pShadowView->HighColor();
	m_pShadowView->SetHighColor(m_pShadowView->ViewColor());
	m_pShadowView->FillRect(BRect(x, y, x+width, y+height));
	m_pShadowView->SetHighColor(old_colour);
	m_pShadowView->Window()->Unlock();
	
	UPDATE_VIEW
}

UT_Bool GR_BEOSGraphics::startPrint(void)
{
	UT_ASSERT(0);
	return UT_FALSE;
}

UT_Bool GR_BEOSGraphics::startPage(const char * /*szPageLabel*/, UT_uint32 /*pageNumber*/,
								UT_Bool /*bPortrait*/, UT_uint32 /*iWidth*/, UT_uint32 /*iHeight*/)
{
	UT_ASSERT(0);
	return UT_FALSE;
}

UT_Bool GR_BEOSGraphics::endPrint(void)
{
	UT_ASSERT(0);
	return UT_FALSE;
}

GR_Image* GR_BEOSGraphics::createNewImage(const char* pszName, const UT_ByteBuf* pBBPNG, UT_sint32 iDisplayWidth, UT_sint32 iDisplayHeight)
{
/*	GR_UnixImage* pImg = new GR_UnixImage(NULL, pszName);

	pImg->convertFromPNG(pBBPNG, iDisplayWidth, iDisplayHeight);

	return pImg;
*/
	return(NULL);
}

void GR_BEOSGraphics::drawImage(GR_Image* pImg, UT_sint32 xDest, UT_sint32 yDest)
{
/*
	UT_ASSERT(pImg);
	
	GR_UnixImage * pUnixImage = static_cast<GR_UnixImage *>(pImg);

	Fatmap * image = pUnixImage->getData();

	UT_sint32 iImageWidth = pUnixImage->getDisplayWidth();
	UT_sint32 iImageHeight = pUnixImage->getDisplayHeight();

	gdk_draw_rgb_image(m_pWin,
					   m_pGC,
					   xDest,
					   yDest,
					   iImageWidth,
					   iImageHeight,
					   GDK_RGB_DITHER_NORMAL,
					   image->data,
					   image->width * 3); // This parameter is the total bytes across one row,
	                                      // which is pixels * 3 (we use 3 bytes per pixel).
*/
}

void GR_BEOSGraphics::flush(void)
{
}

void GR_BEOSGraphics::setCursor(GR_Graphics::Cursor c)
{
/*
	if (m_cursor == c)
		return;
	
	m_cursor = c;
	
	enum GdkCursorType cursor_number;
	
	switch (c)
	{
	default:
		UT_ASSERT(UT_NOT_IMPLEMENTED);
		//FALLTHRU
	case GR_CURSOR_DEFAULT:
		cursor_number = GDK_TOP_LEFT_ARROW;
		break;
		
	case GR_CURSOR_IBEAM:
		cursor_number = GDK_XTERM;
		break;

	case GR_CURSOR_RIGHTARROW:
		cursor_number = GDK_ARROW;
		break;
		
	case GR_CURSOR_IMAGE:
		cursor_number = GDK_FLEUR;
		break;
		
	case GR_CURSOR_IMAGESIZE_NW:
		cursor_number = GDK_TOP_LEFT_CORNER;
		break;
		
	case GR_CURSOR_IMAGESIZE_N:
		cursor_number = GDK_TOP_SIDE;
		break;
		
	case GR_CURSOR_IMAGESIZE_NE:
		cursor_number = GDK_TOP_RIGHT_CORNER;
		break;
		
	case GR_CURSOR_IMAGESIZE_E:
		cursor_number = GDK_RIGHT_SIDE;
		break;
		
	case GR_CURSOR_IMAGESIZE_SE:
		cursor_number = GDK_BOTTOM_RIGHT_CORNER;
		break;
		
	case GR_CURSOR_IMAGESIZE_S:
		cursor_number = GDK_BOTTOM_SIDE;
		break;
		
	case GR_CURSOR_IMAGESIZE_SW:
		cursor_number = GDK_BOTTOM_LEFT_CORNER;
		break;
		
	case GR_CURSOR_IMAGESIZE_W:
		cursor_number = GDK_LEFT_SIDE;
		break;
	}

	GdkCursor * cursor = gdk_cursor_new(cursor_number);
	gdk_window_set_cursor(m_pWin, cursor);
	gdk_cursor_destroy(cursor);
*/
}

GR_Graphics::Cursor GR_BEOSGraphics::getCursor(void) const
{
	return m_cursor;
}


