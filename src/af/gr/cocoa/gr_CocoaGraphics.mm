/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2001-2002 Hubert Figuiere
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

#include "ut_endian.h"
#include "xap_CocoaApp.h"
#include "xap_CocoaFontManager.h"
#include "xap_CocoaFont.h"
#include "gr_CocoaGraphics.h"
#include "gr_CocoaImage.h"
#include "ut_sleep.h"
#include "xap_CocoaFrame.h"

#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_misc.h"
#include "ut_string.h"
//#include "ut_dialogHelper.h"
#include "ut_wctomb.h"
#include "xap_EncodingManager.h"
#include "ut_OverstrikingChars.h"

#import <Cocoa/Cocoa.h>

#define DISABLE_VERBOSE 1
#define USE_OFFSCREEN 1
//#undef USE_OFFSCREEN

#ifdef DISABLE_VERBOSE
#undef UT_DEBUGMSG
#define UT_DEBUGMSG(x)
#endif

#ifdef USE_OFFSCREEN
#define LOCK_CONTEXT__ 	StNSImageLocker locker (m_offscreen)
#else
#define LOCK_CONTEXT__	StNSViewLocker locker(m_pWin)
#endif

// create a stack object like that to lock a NSView, then it will be unlocked on scope exit.
// never do a new
class StNSViewLocker {
public:
	StNSViewLocker (NSView * view) { 
		m_view = view; 
		m_hasLock = [view lockFocusIfCanDraw]; 
	}
	~StNSViewLocker () {
		if (m_hasLock == YES) {
			[m_view unlockFocus];
		}
	}
private:
	NSView *m_view;
	BOOL m_hasLock;
	
	void * operator new (size_t size);	// private so that we never call new for that class. Never defined.
};

class StNSImageLocker {
public:
	StNSImageLocker (NSImage * img) { 
		m_image = img; 
		[img lockFocus]; 
	}
	~StNSImageLocker () {
		[m_image unlockFocus];
	}
private:
	NSImage *m_image;
	
	void * operator new (size_t size);	// private so that we never call new for that class. Never defined.
};


XAP_CocoaFontHandle *	GR_CocoaGraphics::s_pFontGUI = NULL;
UT_uint32 				GR_CocoaGraphics::s_iInstanceCount = 0;

GR_CocoaGraphics::GR_CocoaGraphics(NSView * win, XAP_CocoaFontManager * fontManager, XAP_App * app)
	: m_updateCallback(NULL),
	m_updateCBparam (NULL),
	m_pFontManager (fontManager),
	m_offscreen (nil),
	m_currentColor (nil),
	m_pFont (NULL)
{
	m_pApp = app;
	UT_ASSERT (win);
	NSRect viewBounds = [win bounds];
	//xxx_UT_DEBUGMSG (("frame is %f %f %f %f\n", theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
	if (![win isKindOfClass:[Abi_NSView class]]) {
		m_pWin = [[Abi_NSView alloc] initWithFrame:viewBounds];
		[win addSubview:m_pWin];
	}
	else {
		[win retain];
		m_pWin = win;
	}
	[m_pWin setAutoresizingMask:(NSViewWidthSizable | NSViewHeightSizable)];
	[m_pWin setGraphics:this];
	m_iLineWidth = 0;
	s_iInstanceCount++;
	init3dColors ();
	
	m_offscreen = [[NSImage alloc] initWithSize:viewBounds.size];
	[m_offscreen setFlipped:YES];
	
	LOCK_CONTEXT__;
	
	[NSBezierPath setDefaultLineWidth:m_iLineWidth];
	[NSBezierPath setDefaultLineCapStyle:NSButtLineCapStyle];
	[NSBezierPath setDefaultLineJoinStyle:NSMiterLineJoinStyle];
	m_currentColor = [[NSColor blackColor] copy];
 	[m_currentColor set];
		
	m_cs = GR_Graphics::GR_COLORSPACE_COLOR;
	m_cursor = GR_CURSOR_INVALID;
	setCursor(GR_CURSOR_DEFAULT);
	[[NSGraphicsContext currentContext] setShouldAntialias:NO];
	/* save initial graphics state that has no clipping */
	[NSGraphicsContext saveGraphicsState];
# ifndef USE_OFFSCREEN
	StNSImageLocker locker (m_offscreen);
# endif
	NSEraseRect ([m_pWin bounds]);
}

GR_CocoaGraphics::~GR_CocoaGraphics()
{
	[m_pWin removeFromSuperview];
	
	s_iInstanceCount--;
	if(!s_iInstanceCount) {
		DELETEP(s_pFontGUI);
	}
	for (int i = 0; i < COUNT_3D_COLORS; i++) {
		[m_3dColors[i] release];
	}
}

bool GR_CocoaGraphics::queryProperties(GR_Graphics::Properties gp) const
{
	switch (gp)
	{
	case DGP_SCREEN:
	case DGP_OPAQUEOVERLAY:
		return true;
	case DGP_PAPER:
		return false;
	default:
		UT_ASSERT(0);
		return false;
	}
}

/* let's cache this, since construction of UT_Wctomb is rather slow */
static UT_Wctomb* w = NULL;
static char text[MB_LEN_MAX];
static int text_length;
static bool fallback_used;

#define WCTOMB_DECLS \
	if (!w) {	\
	    w = new UT_Wctomb;	\
	} else	\
	    w->initialize();	
	    
#define CONVERT_TO_MBS(c)	\
    	if (c<=0xff) {	\
		/* this branch is to allow Lists to function */	\
		text[0] = (unsigned char)c;			\
		text_length = 1;				\
		fallback_used = 0;				\
	} else	{\
		fallback_used = 0;	\
		if (!w->wctomb(text,text_length,(wchar_t)c)) {	\
		    w->wctomb_or_fallback(text,text_length,(wchar_t)c);	\
		    fallback_used = 1;	\
		}	\
	}	


// HACK: I need more speed
void GR_CocoaGraphics::drawChar(UT_UCSChar Char, UT_sint32 xoff, UT_sint32 yoff)
{
	UT_DEBUGMSG (("GR_CocoaGraphics::drawChar()\n"));
	UT_UCSChar Wide_char = remapGlyph(Char, false);
	if(Wide_char == 0x200B || Wide_char == 0xFEFF) //zero width spaces
		return;
		
	NSFont *font = m_pFont->getNSFont();

	NSString * string = [[NSString alloc] initWithData:
							[NSData dataWithBytes:&Wide_char length:sizeof(Wide_char)] 
							encoding:NSUnicodeStringEncoding];

	NSPoint point;
	point.x = xoff;
	point.y = yoff;
	
	LOCK_CONTEXT__;
	// TODO: set attributes
	[string drawAtPoint:point withAttributes:[NSDictionary dictionaryWithObject:font forKey:NSFontAttributeName]];	
	[string release];
}

void GR_CocoaGraphics::drawChars(const UT_UCSChar* pChars, int iCharOffset,
							 int iLength, UT_sint32 xoff, UT_sint32 yoff)
{
	UT_DEBUGMSG (("GR_CocoaGraphics::drawChar()\n"));
	if (!m_pFontManager)
		return;
	UT_ASSERT(m_pFont);
	WCTOMB_DECLS;
	NSFont *font = m_pFont->getNSFont();
	UT_sint32 x;

#ifdef BIDI_ENABLED	
	// to be able to handle overstriking characters, we have to remember the width
	// of the previous character printed
	// NB: overstriking characters are only supported under UTF-8, since on 8-bit locales
	// these are typically handled by combination glyphs

	static UT_sint32 prevWidth = 0;
	UT_sint32 curX;
	UT_sint32 curWidth;
#endif
	const UT_UCSChar *pC;
	
	// Lock the NSView
	LOCK_CONTEXT__;
	NSString * string = nil;

  	for(pC=pChars+iCharOffset, x=xoff; pC<pChars+iCharOffset+iLength; ++pC)
	{
		UT_UCSChar actual = remapGlyph(*pC,false);
		if(actual == 0x200B || actual == 0xFEFF) //zero width spaces
			continue;
		// TODO try to bufferize the string allocation
		string = [[NSString alloc] initWithData:[NSData dataWithBytes:&actual length:sizeof(UT_UCSChar)] 
							encoding:NSUnicodeStringEncoding];
			
		/*	if the locale is unicode (i.e., utf-8) then we do not want
			to convert the UCS string to anything,
			gdk_draw_text can draw 16-bit string, if the font is
			a matrix; however the string is interpreted as big-endian
		*/
		//unicode font
		//UT_DEBUGMSG(("CocoaGraphics::drawChars: utf-8\n"));
#ifdef BIDI_ENABLED
		switch(isOverstrikingChar(*pC))
		{
		case UT_NOT_OVERSTRIKING:
			curWidth = [font widthOfString:string];
			curX = x;
			break;
		case UT_OVERSTRIKING_RTL:
			curWidth = 0;
			curX = x;
			break;
		case UT_OVERSTRIKING_LTR:
			curWidth =  prevWidth;
			curX = x - prevWidth;
			break;
		}
	
		NSPoint point;
		point.x = curX;
		point.y = yoff;

		[string drawAtPoint:point withAttributes:[NSDictionary dictionaryWithObject:font forKey:NSFontAttributeName]];	
		
		//gdk_draw_text(m_pWin,font,m_pGC,curX,yoff+font->ascent,(gchar*)&beucs,2);
		x+=curWidth;
		prevWidth = curWidth;
#else
		NSPoint point;
		point.x = x;
		point.y = yoff;

		// TODO: set attributes
		[string drawAtPoint:point withAttributes:nil];	
		//gdk_draw_text(m_pWin,font,m_pGC,x,yoff+font->ascent,(gchar*)&beucs,2);
		x += [font widthOfString:string];
		//x+=gdk_text_width(font, (gchar*)&beucs, 2);
#endif
		[string release];
	}
	
	flush();
}

void GR_CocoaGraphics::setFont(GR_Font * pFont)
{
	XAP_CocoaFontHandle * pUFont = static_cast<XAP_CocoaFontHandle *> (pFont);
	UT_ASSERT (pUFont);
	m_pFont = pUFont;
}

UT_uint32 GR_CocoaGraphics::getFontHeight(GR_Font * fnt)
{
	return getFontAscent(fnt)+getFontDescent(fnt);
}

UT_uint32 GR_CocoaGraphics::getFontHeight()
{
	if (!m_pFontManager)
		return 0;

	return getFontAscent()+getFontDescent();
}

UT_uint32 GR_CocoaGraphics::measureUnRemappedChar(const UT_UCSChar c)
{
	// measureString() could be defined in terms of measureUnRemappedChar()
	// but its not (for presumed performance reasons).  Also, a difference
	// is that measureString() uses remapping to get past zero-width
	// character cells.
	
	if(c == 0x200B || c == 0xFEFF) // 0-with spaces
		return 0;

	UT_ASSERT(m_pFont);

	NSFont * font = m_pFont->getNSFont();
	NSString * string = [[NSString alloc] initWithData:[NSData dataWithBytes:&c length:sizeof(UT_UCSChar)] 
							encoding:NSUnicodeStringEncoding];
	float w = [font widthOfString:string];
	[string release];
	
	return w;
}


UT_uint32 GR_CocoaGraphics::_getResolution(void) const
{
	return 100;
}

/*
	Allocate the color. Must be released by caller.
 */
NSColor	*GR_CocoaGraphics::_utRGBColorToNSColor (const UT_RGBColor& clr)
{
	float r,g,b;
	r = (float)clr.m_red / 255.0f;
	g = (float)clr.m_grn / 255.0f;
	b = (float)clr.m_blu / 255.0f;
	UT_DEBUGMSG (("converting color r=%f, g=%f, b=%f from %d, %d, %d\n", r, g, b, clr.m_red, clr.m_grn, clr.m_blu)); 
	NSColor *c = [[NSColor colorWithDeviceRed:r green:g blue:b alpha:1.0] retain];	// is autoreleased, so retain
	return c;
}


void GR_CocoaGraphics::setColor(const UT_RGBColor& clr)
{
	UT_DEBUGMSG (("GR_CocoaGraphics::setColor(const UT_RGBColor&): setting color %d, %d, %d\n", clr.m_red, clr.m_grn, clr.m_blu)); 
	NSColor *c = _utRGBColorToNSColor (clr);
	_setColor(c);
	[c release];
}

/* c will be copied */
void GR_CocoaGraphics::_setColor(NSColor * c)
{
	UT_DEBUGMSG (("GR_CocoaGraphics::_setColor(NSColor *): setting NSColor\n"));
	[m_currentColor release];
	m_currentColor = [c copy];
	LOCK_CONTEXT__;
	[m_currentColor set];
}

GR_Font * GR_CocoaGraphics::getGUIFont(void)
{
	if (!m_pFontManager)
		return NULL;
	
	if (!s_pFontGUI)
	{
		// get the font resource
		UT_DEBUGMSG(("GR_CocoaGraphics::getGUIFont: getting default font\n"));
		XAP_CocoaFont * font = (XAP_CocoaFont *) m_pFontManager->getDefaultFont();
		UT_ASSERT(font);

		// bury it in a new font handle
		s_pFontGUI = new XAP_CocoaFontHandle(font, [NSFont labelFontSize]); // Hardcoded GUI font size
		UT_ASSERT(s_pFontGUI);
		DELETEP (font);			// s_pFontGUI has duplicated the font object. Free it.
	}

	return s_pFontGUI;
}

GR_Font * GR_CocoaGraphics::findFont(const char* pszFontFamily, 
									const char* pszFontStyle, 
									const char* /*pszFontVariant*/, 
									const char* pszFontWeight, 
									const char* /*pszFontStretch*/, 
									const char* pszFontSize)
{
	UT_DEBUGMSG (("GR_CocoaGraphics::findFont(%s, %s, %s)\n", pszFontFamily, pszFontStyle, pszFontSize));
	if (!m_pFontManager)
		return NULL;

	UT_ASSERT(pszFontFamily);
	UT_ASSERT(pszFontStyle);
	UT_ASSERT(pszFontWeight);
	UT_ASSERT(pszFontSize);
	
	// convert styles to XAP_CocoaFont:: formats
	XAP_CocoaFont::style s = XAP_CocoaFont::STYLE_NORMAL;

	// this is kind of sloppy
	if (!UT_strcmp(pszFontStyle, "normal") &&
		!UT_strcmp(pszFontWeight, "normal"))
	{
		s = XAP_CocoaFont::STYLE_NORMAL;
	}
	else if (!UT_strcmp(pszFontStyle, "normal") &&
			 !UT_strcmp(pszFontWeight, "bold"))
	{
		s = XAP_CocoaFont::STYLE_BOLD;
	}
	else if (!UT_strcmp(pszFontStyle, "italic") &&
			 !UT_strcmp(pszFontWeight, "normal"))
	{
		s = XAP_CocoaFont::STYLE_ITALIC;
	}
	else if (!UT_strcmp(pszFontStyle, "italic") &&
			 !UT_strcmp(pszFontWeight, "bold"))
	{
		s = XAP_CocoaFont::STYLE_BOLD_ITALIC;
	}
	else
	{
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}

	// Request the appropriate XAP_CocoaFont
	const XAP_CocoaFont * cocoafont = m_pFontManager->getFont(pszFontFamily, s);	
	if (!cocoafont)
	{
		// Oops!  We don't have that font here.
		// first try "Times New Roman", which should be sensible, and should
		// be there unless the user fidled with the installation
		cocoafont = m_pFontManager->getFont("Times New Roman", s);
		
		// Oh well, see if there are any fonts at all, and if so
		// just take the first one ...
		if(!cocoafont)
		{
				UT_Vector *	pVec = m_pFontManager->getAllFonts();
				if(pVec && pVec->getItemCount() > 0)
				{
					// get the first font we have
					cocoafont = static_cast<XAP_CocoaFont *>(pVec->getNthItem(0));
				}
		
		}
	}

	// bury the pointer to our Cocoa font in a XAP_CocoaFontHandle with the correct size.

//
// This piece of code scales the FONT chosen at low resolution to that at high
// resolution. This fixes bug 1632 and other non-WYSIWYG behaviour.
//
	UT_uint32 iSize = getAppropriateFontSizeFromString(pszFontSize);
	XAP_CocoaFontHandle * pFont = new XAP_CocoaFontHandle(cocoafont, iSize);
	UT_ASSERT(pFont);

	return pFont;
}

UT_uint32 GR_CocoaGraphics::getFontAscent(GR_Font * fnt)
{
	UT_ASSERT(fnt);

	XAP_CocoaFontHandle * hndl = static_cast<XAP_CocoaFontHandle *>(fnt);

	NSFont* pFont = hndl->getNSFont();
	return [pFont ascender];
}

UT_uint32 GR_CocoaGraphics::getFontAscent()
{
	return getFontAscent(m_pFont);
}

UT_uint32 GR_CocoaGraphics::getFontDescent(GR_Font * fnt)
{
	UT_ASSERT(fnt);

	XAP_CocoaFontHandle * hndl = static_cast<XAP_CocoaFontHandle *>(fnt);

	NSFont* pFont = hndl->getNSFont();
	return [pFont descender];
}

UT_uint32 GR_CocoaGraphics::getFontDescent()
{
	return getFontDescent(m_pFont);
}

void GR_CocoaGraphics::drawLine(UT_sint32 x1, UT_sint32 y1,
							   UT_sint32 x2, UT_sint32 y2)
{
	LOCK_CONTEXT__;
	UT_DEBUGMSG (("GR_CocoaGraphics::drawLine(%ld, %ld, %ld, %ld) width=%f\n", x1, y1, x2, y2, 
	              [NSBezierPath defaultLineWidth]));
	// TODO set the line width according to m_iLineWidth	
	[NSBezierPath strokeLineFromPoint:NSMakePoint(x1, y1) toPoint:NSMakePoint(x2, y2)];
}

void GR_CocoaGraphics::setLineWidth(UT_sint32 iLineWidth)
{
	LOCK_CONTEXT__;
	UT_DEBUGMSG (("GR_CocoaGraphics::setLineWidth(%ld) was %f\n", iLineWidth, [NSBezierPath defaultLineWidth]));
	m_iLineWidth = iLineWidth;

	[NSBezierPath setDefaultLineWidth:iLineWidth];
}

void GR_CocoaGraphics::xorLine(UT_sint32 x1, UT_sint32 y1, UT_sint32 x2,
			    UT_sint32 y2)
{
	// TODO use XOR mode NSCompositeXOR
	LOCK_CONTEXT__;
	[NSBezierPath strokeLineFromPoint:NSMakePoint(x1, y1) toPoint:NSMakePoint(x2, y2)];
}

void GR_CocoaGraphics::polyLine(UT_Point * pts, UT_uint32 nPoints)
{
	UT_DEBUGMSG (("GR_CocoaGraphics::polyLine() width=%f\n", [NSBezierPath defaultLineWidth]));
	NSBezierPath *path = [NSBezierPath bezierPath];		// autoreleased
	
	for (UT_uint32 i = 0; i < nPoints; i++) 
	{
		if (i == 0) {
			[path moveToPoint:NSMakePoint (pts[i].x, pts[i].y)];
		}
		else {
			[path lineToPoint:NSMakePoint (pts[i].x, pts[i].y)];
		}
	}
	LOCK_CONTEXT__;
	[path stroke];
}

void GR_CocoaGraphics::invertRect(const UT_Rect* pRect)
{
	UT_DEBUGMSG (("GR_CocoaGraphics::invertRect()\n"));
	UT_ASSERT(pRect);

	LOCK_CONTEXT__;
	// TODO handle invert. this is highlight.
	
	NSHighlightRect (NSMakeRect (pRect->left, pRect->top, pRect->width, pRect->height));
}

void GR_CocoaGraphics::setClipRect(const UT_Rect* pRect)
{
	UT_DEBUGMSG (("GR_CocoaGraphics::setClipRect()\n"));
	m_pRect = pRect;

	LOCK_CONTEXT__;
	/* discard the clipping */
	/* currently the only way is to restore the graphic state */
	[NSGraphicsContext restoreGraphicsState];
	[NSGraphicsContext saveGraphicsState];
	/* restore the graphics settings */
	[m_currentColor set];
	[NSBezierPath setDefaultLineWidth:m_iLineWidth];
	
	if (pRect) {
		UT_DEBUGMSG (("ClipRect set\n"));
		NSRectClip(NSMakeRect (pRect->left, pRect->top, pRect->width, pRect->height));
	}
	else {
		UT_DEBUGMSG (("ClipRect reset!!\n"));
	}
}

void GR_CocoaGraphics::fillRect(UT_RGBColor& c, UT_Rect &r)
{
	UT_DEBUGMSG(("GR_CocoaGraphics::fillRect(UT_RGBColor&, UT_Rect &)\n"));
	fillRect(c,r.left,r.top,r.width,r.height);
}

void GR_CocoaGraphics::fillRect(UT_RGBColor& clr, UT_sint32 x, UT_sint32 y,
							   UT_sint32 w, UT_sint32 h)
{
	UT_DEBUGMSG(("GR_CocoaGraphics::fillRect(UT_RGBColor&, %ld, %ld, %ld, %ld)\n", x, y, w, h));
	// save away the current color, and restore it after we fill the rect
	NSColor *c = _utRGBColorToNSColor (clr);
		
	LOCK_CONTEXT__;
	[NSGraphicsContext saveGraphicsState];
	[c set];
	NSRectFill (NSMakeRect (x, y, w, h));
	[NSGraphicsContext restoreGraphicsState];

	[c release];
}

void GR_CocoaGraphics::fillRect(GR_Color3D c, UT_sint32 x, UT_sint32 y, UT_sint32 w, UT_sint32 h)
{
	UT_ASSERT(c < COUNT_3D_COLORS);
	UT_DEBUGMSG(("GR_CocoaGraphics::fillRect(GR_Color3D %d, %ld, %ld, %ld, %ld)\n", c, x, y, w, h));
	
	LOCK_CONTEXT__;
	[NSGraphicsContext saveGraphicsState];
	[m_3dColors[c] set];
	NSRectFill (NSMakeRect (x, y, w, h));
	[NSGraphicsContext restoreGraphicsState];
}

void GR_CocoaGraphics::fillRect(GR_Color3D c, UT_Rect &r)
{
	UT_DEBUGMSG(("GR_CocoaGraphics::fillRect(GR_Color3D, UT_Rect &)\n"));
	UT_ASSERT(c < COUNT_3D_COLORS);
	fillRect(c,r.left,r.top,r.width,r.height);
}


void GR_CocoaGraphics::scroll(UT_sint32 dx, UT_sint32 dy)
{
	UT_ASSERT(UT_NOT_IMPLEMENTED);
}

void GR_CocoaGraphics::scroll(UT_sint32 x_dest, UT_sint32 y_dest,
						  UT_sint32 x_src, UT_sint32 y_src,
						  UT_sint32 width, UT_sint32 height)
{
	UT_ASSERT(UT_NOT_IMPLEMENTED);
}

void GR_CocoaGraphics::clearArea(UT_sint32 x, UT_sint32 y,
							 UT_sint32 width, UT_sint32 height)
{
  	UT_DEBUGMSG(("ClearArea: %d %d %d %d\n", x, y, width, height));
	if (width > 0)
	{
		NSEraseRect (NSMakeRect(x, y, width, height));
	}
}

bool GR_CocoaGraphics::startPrint(void)
{
	UT_ASSERT(0);
	return false;
}

bool GR_CocoaGraphics::startPage(const char * /*szPageLabel*/, UT_uint32 /*pageNumber*/,
								bool /*bPortrait*/, UT_uint32 /*iWidth*/, UT_uint32 /*iHeight*/)
{
	UT_ASSERT(0);
	return false;
}

bool GR_CocoaGraphics::endPrint(void)
{
	UT_ASSERT(0);
	return false;
}


GR_Image* GR_CocoaGraphics::createNewImage(const char* pszName, const UT_ByteBuf* pBB, UT_sint32 iDisplayWidth, UT_sint32 iDisplayHeight, GR_Image::GRType iType)
{
   	GR_Image* pImg = NULL;
   	if (iType == GR_Image::GRT_Raster)
   		pImg = new GR_CocoaImage(pszName);
   	else
	   	pImg = new GR_VectorImage(pszName);
	     
	pImg->convertFromBuffer(pBB, iDisplayWidth, iDisplayHeight);
   	return pImg;
}

void GR_CocoaGraphics::drawImage(GR_Image* pImg, UT_sint32 xDest, UT_sint32 yDest)
{
	UT_ASSERT(pImg);

   	if (pImg->getType() != GR_Image::GRT_Raster) {
	   	pImg->render(this, xDest, yDest);
	   	return;
	}
	   
   	GR_CocoaImage * pCocoaImage = static_cast<GR_CocoaImage *>(pImg);

   	Fatmap * image = pCocoaImage->getData();

	if (image == 0)
	{
		UT_DEBUGMSG(("Found no image data. This is probably SVG masquerading as a raster!\n"));
		return;
	}

   	UT_sint32 iImageWidth = pCocoaImage->getDisplayWidth();
   	UT_sint32 iImageHeight = pCocoaImage->getDisplayHeight();

	UT_ASSERT(UT_NOT_IMPLEMENTED);
	/*
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


void GR_CocoaGraphics::flush(void)
{
	[[NSGraphicsContext currentContext] flushGraphics];
}

void GR_CocoaGraphics::setColorSpace(GR_Graphics::ColorSpace /* c */)
{
	// we only use ONE color space here now (GdkRGB's space)
	// and we don't let people change that on us.
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
}

GR_Graphics::ColorSpace GR_CocoaGraphics::getColorSpace(void) const
{
	return m_cs;
}

void GR_CocoaGraphics::setCursor(GR_Graphics::Cursor c)
{
	if (m_cursor == c)
		return;
	
	m_cursor = c;
	
	NSCursor * cursor;
	
	switch (c)
	{
	default:
		UT_ASSERT(UT_NOT_IMPLEMENTED);
		/*FALLTHRU*/
	case GR_CURSOR_DEFAULT:
		cursor = [NSCursor arrowCursor];
		break;
		
	case GR_CURSOR_IBEAM:
		cursor = [NSCursor IBeamCursor];
		break;

	case GR_CURSOR_WAIT:
		// There is no wait cursor for Cocoa.  Or something.
		cursor = [NSCursor arrowCursor];
		break;
#if 0

	//I have changed the shape of the arrow so get a consistent
	//behaviour in the bidi build; I think the new arrow is better
	//for the purpose anyway
	
	case GR_CURSOR_RIGHTARROW:
		cursor_number = GDK_SB_RIGHT_ARROW; //GDK_ARROW;
		break;

#ifdef BIDI_ENABLED
//#error choose a suitable cursor; this is just a placeholder !!!		
	case GR_CURSOR_LEFTARROW:
		cursor_number = GDK_SB_LEFT_ARROW; //GDK_LEFT_PTR;
		break;
#endif		

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
		
	case GR_CURSOR_LEFTRIGHT:
		cursor_number = GDK_SB_H_DOUBLE_ARROW;
		break;

	case GR_CURSOR_UPDOWN:
		cursor_number = GDK_SB_V_DOUBLE_ARROW;
		break;

	case GR_CURSOR_EXCHANGE:
		cursor_number = GDK_EXCHANGE;
		break;

	case GR_CURSOR_GRAB:
		cursor_number = GDK_HAND1;
		break;
#endif
	}
	[m_pWin addCursorRect:[m_pWin bounds] cursor:cursor];
	[cursor setOnMouseEntered:YES];
}

GR_Graphics::Cursor GR_CocoaGraphics::getCursor(void) const
{
	return m_cursor;
}

void GR_CocoaGraphics::setColor3D(GR_Color3D c)
{
	UT_DEBUGMSG(("GR_CocoaGraphics::setColor3D(GR_Color3D %d)\n", c));
	UT_ASSERT(c < COUNT_3D_COLORS);
	_setColor(m_3dColors[c]);
}

void GR_CocoaGraphics::init3dColors()
{
	m_3dColors[CLR3D_Foreground] = [[NSColor blackColor] retain];
	m_3dColors[CLR3D_Background] = [[NSColor controlColor] retain];
//	m_3dColors[CLR3D_Background] = [[NSColor grayColor] retain];
	m_3dColors[CLR3D_BevelUp] = [[NSColor lightGrayColor] retain];
	m_3dColors[CLR3D_BevelDown] = [[NSColor darkGrayColor] retain];
	m_3dColors[CLR3D_Highlight] = [[NSColor controlHighlightColor] retain];
}

void GR_CocoaGraphics::polygon(UT_RGBColor& clr,UT_Point *pts,UT_uint32 nPoints)
{
	NSBezierPath *path = [NSBezierPath bezierPath];		//autoreleased ?
	
	for (UT_uint32 i = 0; i < nPoints; i++) 
	{
		if (i == 0) {
			[path moveToPoint:NSMakePoint (pts[i].x, pts[i].y)];
		}
		else {
			[path lineToPoint:NSMakePoint (pts[i].x, pts[i].y)];
		}
	}
	[path closePath];
	NSColor *c = _utRGBColorToNSColor (clr);
	LOCK_CONTEXT__;
	[NSGraphicsContext saveGraphicsState];
	[c set];
	[path stroke];
	[NSGraphicsContext restoreGraphicsState];
	[c release];
}


void GR_CocoaGraphics::_setUpdateCallback (gr_cocoa_graphics_update callback, void * param)
{
	m_updateCallback = callback;
	m_updateCBparam = param;
}

/*!
	Call the update Callback that has been set
	
	\param aRect: the rect that should be updated
	\return false if no callback. Otherwise returns what the callback returns.
 */
bool GR_CocoaGraphics::_callUpdateCallback(NSRect * aRect) 
{
	if (m_updateCallback == NULL) {
		return false;
	}
	return (*m_updateCallback) (aRect, this, m_updateCBparam);
}

//////////////////////////////////////////////////////////////////
// This is a static method in the GR_Font base class implemented
// in platform code.
//////////////////////////////////////////////////////////////////

void GR_Font::s_getGenericFontProperties(const char * /*szFontName*/,
										 FontFamilyEnum * pff,
										 FontPitchEnum * pfp,
										 bool * pbTrueType)
{
	// describe in generic terms the named font.

	// Note: most of the unix font handling code is in abi/src/af/xap/unix
	// Note: rather than in the graphics class.  i'm not sure this matters,
	// Note: but it is just different....

	// TODO add code to map the given font name into one of the
	// TODO enums in GR_Font and set *pff and *pft.

	*pff = FF_Unknown;
	*pfp = FP_Unknown;
	*pbTrueType = true;
}


@implementation Abi_NSView


- (void)setGraphics:(GR_CocoaGraphics *)gr
{
	m_pGR = gr;
}

/*!
	Cocoa overridden method. Redraw the screen.
 */
- (void)drawRect:(NSRect)aRect
{
#if 0 // large debug code. show Abi_NSView in red
	[NSGraphicsContext saveGraphicsState];
	NSColor *c = [NSColor colorWithDeviceRed:1.0 green:0.0 blue:0.0 alpha:1.0]; // autoreleased
	[c set];
	NSRectFill ([self bounds]);
	// and show 3d colors just for fun.
	if (m_pGR) {
		int i;
		for (i = 0; i < COUNT_3D_COLORS; i++) {
			[m_pGR->m_3dColors[i] set];
			NSRectFill (NSMakeRect (i * 10, i * 10, 10, 10));
		}
	}
	[NSGraphicsContext restoreGraphicsState];
#else
	if (m_pGR) {
# ifdef USE_OFFSCREEN

		NSImage * img = m_pGR->_getOffscreen ();
		// TODO: only draw what is needed.
# endif 
		if ([self inLiveResize]) {
			UT_DEBUGMSG (("Is resizing\n"));
# ifdef USE_OFFSCREEN
			NSRect myBounds = [self bounds];
			[img setSize:myBounds.size];
			// take care of erasing after resizing.
			{
				StNSImageLocker locker (img);
				NSEraseRect (myBounds);
			}
# endif 
			[NSGraphicsContext saveGraphicsState];
			NSRectClip (aRect);
			if (!m_pGR->_callUpdateCallback (&aRect)) {
			
			}
			[NSGraphicsContext restoreGraphicsState];
		}
# ifdef USE_OFFSCREEN
		[img drawAtPoint:aRect.origin fromRect:aRect operation:NSCompositeCopy fraction:1.0f];
# endif
		UT_DEBUGMSG (("- (void)drawRect:(NSRect)aRect: calling callback !\n"));
	}
#endif
}


/*!
	Cocoa overridden method. 
	
	\return NO. Coordinates are still upside down, but we'll reverse the offscreen instead
 */
- (BOOL)isFlipped
{
# ifdef USE_OFFSCREEN
	return NO;
# else
	return YES;
#endif
}


/*!
	Cocoa overridden method. 
	
	\return NO. Not opaque.
 */
- (BOOL)isOpaque
{
	return YES;
}


@end
