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

#include <stdlib.h>
#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"

#include "xap_UnixPSFont.h"

/*
  This class is much like the UnixGraphics class in
  abi/src/wp/gr/unix/gr_UnixGraphics.h.  Why?  Because it's
  a light wrapper around the cross-application XAP_UnixFont
  class, exposing appropriate methods to the type of GC
  that needs access to the font resource (which itself
  handles keeping X fonts in sync with files on the local
  machine and their metrics).

  It's not pretty, and these wrappers should probably go
  away if someone gets around to making GR_Font basically
  XAP_UnixFont, but at a cross platform class where there
  is no Windows implementation.
*/

PSFont::PSFont(XAP_UnixFont * pFont, UT_uint32 size)
{
	UT_ASSERT(pFont);
  
	m_hFont = pFont;
	m_pointSize = size;
	m_index = 0;
	xxx_UT_DEBUGMSG(("Creating PS font %x \n",this));
}

PSFont::~PSFont(void)
{
	xxx_UT_DEBUGMSG(("Deleting PS font %x \n",this));
}

XAP_UnixFont * PSFont::getUnixFont(void)
{
	UT_ASSERT(m_hFont);
	return m_hFont;
}

ABIFontInfo * PSFont::getMetricsData(void)
{
	UT_ASSERT(m_hFont);
	return m_hFont->getMetricsData();
}

UT_uint16 PSFont::getCharWidth(UT_UCSChar c)
{
	UT_ASSERT(m_hFont);
	UT_UCSChar cc = c;
	UT_uint16 width = m_hFont->getCharWidth(cc);
	return width;
}
