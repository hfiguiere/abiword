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

#include "gr_UnixImage.h"

GR_UnixImage::GR_UnixImage()
{
}

GR_UnixImage::~GR_UnixImage()
{
}

UT_sint32	GR_UnixImage::getWidth(void) const
{
	return m_pDIB->bmiHeader.biWidth;
}

UT_sint32	GR_UnixImage::getHeight(void) const
{
	return m_pDIB->bmiHeader.biHeight;
}

void		GR_UnixImage::getByteBuf(UT_ByteBuf** ppBB) const
{
	// TODO convert to PNG and copy to the byte buf
}

