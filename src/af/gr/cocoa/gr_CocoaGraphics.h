/* Abiword
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2001-2003 Hubert Figuiere
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

#ifndef GR_COCOAGRAPHICS_H
#define GR_COCOAGRAPHICS_H

#include "xap_CocoaApp.h"
#include "xap_CocoaFont.h"
#include "xap_Frame.h"
#include "gr_Graphics.h"

class UT_ByteBuf;
class GR_CocoaGraphics;

@class XAP_CocoaNSView, XAP_CocoaNSScrollView;

class GR_CocoaGraphics : public GR_Graphics
{
 public:
	GR_CocoaGraphics(NSView * view, /*XAP_CocoaFontManager * fontManager,*/ XAP_App *app);
	~GR_CocoaGraphics();

    // HACK: I need more speed
	virtual void      drawGlyph(UT_uint32 glyph_idx, UT_sint32 xoff, UT_sint32 yoff) 
		{ UT_ASSERT (UT_NOT_IMPLEMENTED); };
	virtual void		drawChars(const UT_UCSChar* pChars, int iCharOffset,
								  int iLength, UT_sint32 xoff, UT_sint32 yoff,
								  int * pCharWidhths = NULL);

	virtual void		setFont(GR_Font* pFont);
	virtual void        clearFont(void) { m_pFont = NULL;}
	virtual UT_uint32	getFontHeight();

	virtual UT_uint32 measureUnRemappedChar(const UT_UCSChar c);
	virtual void getCoverage(UT_Vector& coverage);

	virtual void		setColor(const UT_RGBColor& clr);
	virtual void		getColor(UT_RGBColor& clr);

	virtual GR_Font*	getGUIFont();

	virtual UT_uint32	getFontAscent();
	virtual UT_uint32	getFontDescent();
	virtual void		drawLine(UT_sint32, UT_sint32, UT_sint32, UT_sint32);
	virtual void		setLineWidth(UT_sint32);
	virtual void		xorLine(UT_sint32, UT_sint32, UT_sint32, UT_sint32);
	virtual void		polyLine(UT_Point * pts, UT_uint32 nPoints);
	virtual void		fillRect(const UT_RGBColor& c,
								 UT_sint32 x, UT_sint32 y,
								 UT_sint32 w, UT_sint32 h);
	virtual void		invertRect(const UT_Rect* pRect);
	virtual void		setClipRect(const UT_Rect* pRect);
	virtual void		scroll(UT_sint32, UT_sint32);
	virtual void		scroll(UT_sint32 x_dest, UT_sint32 y_dest,
							   UT_sint32 x_src, UT_sint32 y_src,
							   UT_sint32 width, UT_sint32 height);
	virtual void		clearArea(UT_sint32, UT_sint32, UT_sint32, UT_sint32);
  
	virtual void		drawImage(GR_Image* pImg, UT_sint32 xDest, UT_sint32 yDest);
	virtual GR_Image*	createNewImage(const char* pszName, const UT_ByteBuf* pBB,
					       UT_sint32 iDisplayWidth, UT_sint32 iDisplayHeight,
					       GR_Image::GRType = GR_Image::GRT_Raster);
  
	virtual void setLineProperties ( double    inWidthPixels, 
				      JoinStyle inJoinStyle,
				      CapStyle  inCapStyle,
				      LineStyle inLineStyle);

	virtual bool		queryProperties(GR_Graphics::Properties gp) const;
	virtual bool		startPrint(void);
	virtual bool		startPage(const char * szPageLabel, UT_uint32 pageNumber,
								  bool bPortrait, UT_uint32 iWidth, UT_uint32 iHeight);
	virtual void      setZoomPercentage(UT_uint32 iZoom);
	virtual bool		endPrint(void);

	virtual void		flush(void);

	virtual void		setColorSpace(GR_Graphics::ColorSpace c);
	virtual GR_Graphics::ColorSpace getColorSpace(void) const;
	
	virtual void		setCursor(GR_Graphics::Cursor c);
	virtual GR_Graphics::Cursor getCursor(void) const;

	virtual void		setColor3D(GR_Color3D c);
	void				init3dColors();
	virtual void		fillRect(GR_Color3D c,
								 UT_sint32 x, UT_sint32 y,
								 UT_sint32 w, UT_sint32 h);
	virtual void		fillRect(GR_Color3D c, UT_Rect &r);
	
	virtual void		polygon(UT_RGBColor& c,UT_Point *pts,UT_uint32 nPoints);
	
	/* GR_Font versions of the above -- TODO: should I add drawChar* methods too? */
	virtual UT_uint32 getFontAscent(GR_Font *);
	virtual UT_uint32 getFontDescent(GR_Font *);
	virtual UT_uint32 getFontHeight(GR_Font *);

    virtual GR_Image * genImageFromRectangle(const UT_Rect & r);
	virtual void	  saveRectangle(UT_Rect & r, UT_uint32 iIndx);
	virtual void	  restoreRectangle(UT_uint32 iIndx);
	virtual UT_uint32 getDeviceResolution(void) const;


	typedef bool (*gr_cocoa_graphics_update) (NSRect * rect, GR_CocoaGraphics *pGr, void * param);
	void				_setUpdateCallback (gr_cocoa_graphics_update callback, void * param);
	bool				_callUpdateCallback(NSRect *aRect);
	XAP_CocoaNSView *	_getView () { return m_pWin; };
	void 				_updateRect(NSView * v, NSRect aRect);
	static bool			_isFlipped();
	static NSColor				*_utRGBColorToNSColor (const UT_RGBColor& clr);
	static void 				_utNSColorToRGBColor (NSColor *c, UT_RGBColor &clr);
	void				setIsPrinting(bool isPrinting) { m_bIsPrinting = isPrinting; };
	bool				isPrinting(void) const { return m_bIsPrinting; };
	/* Cocoa Specific */
	static	float		_getScreenResolution(void);
 protected:
	virtual GR_Font*	_findFont(const char* pszFontFamily, 
								  const char* pszFontStyle, 
								  const char* pszFontVariant, 
								  const char* pszFontWeight, 
								  const char* pszFontStretch, 
								  const char* pszFontSize);

	virtual UT_uint32 	_getResolution(void) const;
	void				_setColor(NSColor * c);
private:
	void		_setClipRectImpl(const UT_Rect* pRect);

	
	NSImage*			_makeNewCacheImage() 
	{
			NSImage * cache = [[NSImage alloc] initWithSize:NSMakeSize(0,0)];
			[cache setFlipped:YES];
			return cache;
	}
	
	void				_resetContext();	// reset m_CGContext to default values

	gr_cocoa_graphics_update	m_updateCallback;
	void 						*m_updateCBparam;
	XAP_CocoaNSView *  			m_pWin;
	NSMutableDictionary*		m_fontProps;
	CGContextRef				m_CGContext;
	UT_Vector					m_cacheArray;
	UT_Vector					m_cacheRectArray;
	NSImage*					m_xorCache;
	NSColor *					m_currentColor;

	// our currently requested font by handle
	XAP_CocoaFont *	m_pFont;
	NSFont*						m_fontForGraphics;

	XAP_CocoaFont*	m_pFontGUI;
	static UT_uint32		s_iInstanceCount;
  
	UT_sint32				m_iLineWidth;			// device unit
	GR_Graphics::Cursor		m_cursor;

	GR_Graphics::ColorSpace	m_cs;
	
	UT_uint32				m_screenResolution;
	bool					m_bIsPrinting;
public:		//HACK	
	NSColor	*			m_3dColors[COUNT_3D_COLORS];
private:
	/* private implementations. Allow esasy selection accross various ways */
	void _initMetricsLayouts(void);
	float _measureUnRemappedCharCached(const UT_UCSChar c);
	//private font metrics objects
	NSTextStorage *m_fontMetricsTextStorage;
    NSLayoutManager *m_fontMetricsLayoutManager;
    NSTextContainer *m_fontMetricsTextContainer;
};

#endif /* GR_COCOAGRAPHICS_H */
