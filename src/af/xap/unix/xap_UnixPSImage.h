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

#ifndef XAP_UNIXPSIMAGE_H
#define XAP_UNIXPSIMAGE_H

#include "gr_Image.h"
#include <glib.h>

// This structure is similar to the 24-bit chunk of
// data the WP's Unix GR class uses to store images.
struct PSFatmap
{
	gint width;
	gint height;

	// Always 24-bit pixel data
	guchar * data;
};

class PS_Image : public GR_Image
{
public:
	PS_Image(PSFatmap * image, const char* pszName);
	~PS_Image();

	virtual UT_sint32	getWidth(void) const;
	virtual UT_sint32	getHeight(void) const;
	virtual UT_Bool		getByteBuf(UT_ByteBuf** ppBB) const;
	virtual UT_Bool		convertFromPNG(const UT_ByteBuf* pBB);

	PSFatmap *			getData(void) const { return m_image; }
	
protected:

	PSFatmap * m_image;
};

class PS_ImageFactory : public GR_ImageFactory
{
public:
	virtual GR_Image *	createNewImage(const char* pszName);
};


#endif /* XAP_UNIXPSIMAGE_H */
