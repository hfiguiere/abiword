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

#ifndef GR_GRAPHICS_H
#define GR_GRAPHICS_H

#include "ut_types.h"
#include "ut_units.h"
#include "ut_misc.h"
#include "gr_Image.h"

class UT_RGBColor;

/*
	GR_Font is a reference to a font.  As it happens, everything about fonts
	is platform-specific, so the class contains nothing.  All of its behavior
	and functionality is contained within its subclasses, each of which provides
	the implementation for some specific font technology.
*/
class GR_Font
{
};

/*
	GR_Graphics is a portable interface to a simple 2-d graphics layer.  It is not
	an attempt at a general purpose portability layer.  Rather, it contains only
	functions which are needed.
*/
class GR_Graphics
{
public:
	GR_Graphics();
	virtual ~GR_Graphics();

	virtual void drawChars(const UT_UCSChar* pChars, 
		int iCharOffset, int iLength, UT_sint32 xoff, UT_sint32 yoff) = 0;
	virtual void setFont(GR_Font* pFont) = 0;

	virtual UT_uint32 getFontAscent() = 0;
	virtual UT_uint32 getFontDescent() = 0;
	virtual UT_uint32 getFontHeight() = 0;
	
	virtual UT_uint32 measureString(const UT_UCSChar*s, int iOffset, int num, unsigned short* pWidths) = 0;
	
	virtual void setColor(UT_RGBColor& clr) = 0;
	
	virtual GR_Font* getGUIFont() = 0;
	virtual GR_Font* findFont(
		const char* pszFontFamily, 
		const char* pszFontStyle, 
		const char* pszFontVariant, 
		const char* pszFontWeight, 
		const char* pszFontStretch, 
		const char* pszFontSize) = 0;
	UT_sint32 convertDimension(const char*) const;
	const char * invertDimension(UT_Dimension, double) const;
	UT_Bool scaleDimensions(const char * szLeftIn, const char * szWidthIn,
				UT_uint32 iWidthAvail,
				UT_sint32 * piLeft,
				UT_uint32 * piWidth) const;

	virtual void drawImage(GR_Image* pImg, UT_sint32 xDest, UT_sint32 yDest) = 0;
	virtual GR_Image* createNewImage(const char* pszName, const UT_ByteBuf* pBBPNG, UT_sint32 iDisplayWidth, UT_sint32 iDisplayHeight) = 0;
	
	/* For drawLine() and xorLine():
	**   x0,y0 give the starting pixel.
	**   x1,y1 give the first pixel ***not drawn***.
	*/
	virtual void drawLine(UT_sint32, UT_sint32, UT_sint32, UT_sint32) = 0;
	virtual void xorLine(UT_sint32, UT_sint32, UT_sint32, UT_sint32) = 0;
	virtual void setLineWidth(UT_sint32) = 0;
	virtual void polyLine(UT_Point * pts, UT_uint32 nPoints) = 0;
	virtual void fillRect(UT_RGBColor& c, UT_sint32 x, UT_sint32 y, UT_sint32 w, UT_sint32 h) = 0;
	virtual void fillRect(UT_RGBColor& c, UT_Rect &r) = 0;
	virtual void invertRect(const UT_Rect* pRect) = 0;
	virtual void setClipRect(const UT_Rect* pRect) = 0;
	virtual void scroll(UT_sint32, UT_sint32) = 0;
	virtual void scroll(UT_sint32 x_dest, UT_sint32 y_dest,
						UT_sint32 x_src, UT_sint32 y_src,
						UT_sint32 width, UT_sint32 height) = 0;
	virtual void clearArea(UT_sint32, UT_sint32, UT_sint32, UT_sint32) = 0;
	
	typedef enum { DGP_SCREEN, DGP_PAPER } Properties;
	
	virtual UT_Bool queryProperties(GR_Graphics::Properties gp) const = 0;

	/* the following 3 are only used for printing */
	
	virtual UT_Bool startPrint(void) = 0;
	virtual UT_Bool startPage(const char * szPageLabel, UT_uint32 pageNumber,
							  UT_Bool bPortrait, UT_uint32 iWidth, UT_uint32 iHeight) = 0;
	virtual UT_Bool endPrint(void) = 0;

	virtual void flush(void);

	/* multiple cursor support */
	
	typedef enum { GR_CURSOR_INVALID=0,
				   GR_CURSOR_DEFAULT,
				   GR_CURSOR_IBEAM,
				   GR_CURSOR_RIGHTARROW,
				   GR_CURSOR_IMAGE,
				   GR_CURSOR_IMAGESIZE_NW,
				   GR_CURSOR_IMAGESIZE_N,
				   GR_CURSOR_IMAGESIZE_NE,
				   GR_CURSOR_IMAGESIZE_E,
				   GR_CURSOR_IMAGESIZE_SE,
				   GR_CURSOR_IMAGESIZE_S,
				   GR_CURSOR_IMAGESIZE_SW,
				   GR_CURSOR_IMAGESIZE_W
	} Cursor;

	virtual void setCursor(GR_Graphics::Cursor c) = 0;
	virtual GR_Graphics::Cursor getCursor(void) const = 0;

	void setZoomPercentage(UT_uint32 iZoom);
	UT_uint32 getZoomPercentage(void) const;
	UT_uint32 getResolution(void) const;

	typedef enum { CLR3D_Foreground=0,				/* color of text/foreground on a 3d object */
				   CLR3D_Background=1,				/* color of face/background on a 3d object */
				   CLR3D_BevelUp=2,					/* color of bevel-up  */
				   CLR3D_BevelDown=3,				/* color of bevel-down */
				   CLR3D_Highlight=4				/* color half-way between up and down */
	} GR_Color3D;
#define COUNT_3D_COLORS 5
	
	virtual void					setColor3D(GR_Color3D c) = 0;
	virtual void					fillRect(GR_Color3D c, UT_sint32 x, UT_sint32 y, UT_sint32 w, UT_sint32 h) = 0;
	virtual void					fillRect(GR_Color3D c, UT_Rect &r) = 0;

protected:
	virtual UT_uint32 _getResolution(void) const = 0;
	
	UT_uint32	m_iZoomPercentage;
	
};

#endif /* GR_GRAPHICS_H */
