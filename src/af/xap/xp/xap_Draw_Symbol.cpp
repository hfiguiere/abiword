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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_types.h"
#include "ut_string.h"

#include "xap_Draw_Symbol.h"

static UT_UCSChar m_Insert_Symbol_font[50]; 

XAP_Draw_Symbol::XAP_Draw_Symbol(GR_Graphics * gc)
	: XAP_Preview(gc)
{

	m_CurrentSymbol = m_PreviousSymbol = UCS_SPACE;

	m_pFont = NULL;
	m_areagc = NULL;
	// m_gc is set in base class, so set up defaults
	m_drawWidth = 0;
	m_drawHeight = 0;
}


XAP_Draw_Symbol::~XAP_Draw_Symbol(void)
{
  	DELETEP(m_pFont);
}

void XAP_Draw_Symbol::setWindowSize( UT_uint32 width, UT_uint32 height)
{
	m_drawWidth = width;
	m_drawHeight = height;
}

void XAP_Draw_Symbol::setAreaGc( GR_Graphics * gc)
{
	m_areagc = gc;   
}

void XAP_Draw_Symbol::setAreaSize( UT_uint32 width, UT_uint32 height)
{
	m_drawareaWidth = width;
	m_drawareaHeight = height;
	setFontStringarea();
}


void XAP_Draw_Symbol::setFontString( void )
{
	setFontToGC(m_gc, m_drawWidth / 32, 14);	
}

void XAP_Draw_Symbol::setFontToGC(GR_Graphics *p_gc, UT_uint32 MaxWidthAllowable, UT_sint32 PointSize)
{
	UT_ASSERT(p_gc);
	UT_ASSERT(m_Insert_Symbol_font);
	UT_ASSERT(MaxWidthAllowable);

	GR_Font * found = NULL;

	int SizeOK = UT_FALSE;
	UT_UCSChar *p_buffer = new UT_UCSChar[224];
	for(int i = 0; i < 224; i++)
	{
		p_buffer[i] = i + 32;
	}

	char font[50];
	UT_UCS_strcpy_to_char(font, m_Insert_Symbol_font);

	while(!SizeOK)
	{
		char temp[10];
		sprintf(temp, "%ipt", PointSize);

		found = p_gc->findFont(font, "normal", "", "normal", "", temp);
		if (found)
		{
			p_gc->setFont(found);
			//              REPLACEP(m_pFont, found);
		}
		else
		{

			found = p_gc->findFont("Standard Symbols","normal", "","normal", "", temp);
			if(found)
			{
				p_gc->setFont(found);
				//  REPLACEP(m_pFont, found);
			}
			else
			{
				UT_DEBUGMSG(("COULD NOT find Standard Symbols font \n"));
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
				SizeOK = UT_TRUE;	// so we can break loop
			}
		}

		if(found)
		{
			UT_uint32 MaxWidth = p_gc->getMaxCharacterWidth(p_buffer, 224);
			if(MaxWidth < MaxWidthAllowable)
			{
				SizeOK = UT_TRUE;
			}
			else
			{
				PointSize--;
				UT_ASSERT(PointSize);

			}
		}
	}

	delete p_buffer;

}

UT_UCSChar * XAP_Draw_Symbol::getSelectedFont(void)
{
	return m_Insert_Symbol_font;
}

void XAP_Draw_Symbol::setSelectedFont(UT_UCSChar *font)
{
	// TODO: We need UT_UCS_strncpy_*

	UT_UCS_strcpy(m_Insert_Symbol_font, font);
	setFontString ();
	setFontStringarea ();

	m_CurrentSymbol = m_PreviousSymbol = UCS_SPACE;
}

void XAP_Draw_Symbol::setFontStringarea( void )
{
	setFontToGC(m_areagc, m_drawareaWidth, 32);
}


void XAP_Draw_Symbol::draw(void)
{
	UT_ASSERT(m_gc);
	UT_UCSChar c;
	UT_uint32 wwidth, wheight, i, j, yoff, xoff, x, y;

	wwidth = m_drawWidth;
	wheight = m_drawHeight;
	UT_uint32 tmpw = wwidth / 32;
	UT_uint32 tmph = wheight / 7;
	yoff = wheight / 14;
	xoff = wwidth / 64;
	m_gc->clearArea(0, 0, wwidth, wheight);

	x = 2;
	for(i = 0; i <= 31; i++)
	{
		y = 0;
		c = (char) (i + 32);
		for(j = 0; j <= 6; j++)
		{
			m_gc->drawChar(c, x, y);
			y += tmph;
			c += 32;
		}
		x += tmpw;
	}

	y = 0;
	for(i = 0; i <= 6; i++)
	{
		m_gc->drawLine(0, y, wwidth - 1, y);
		y += tmph;
	}

	x = 0;
	for(i = 0; i <= 31; i++)
	{
		m_gc->drawLine(x, 0, x, wheight - 1);
		x += tmpw;
	}
}

UT_UCSChar XAP_Draw_Symbol::calcSymbol(UT_uint32 x, UT_uint32 y)
{
	UT_uint32 wwidth,wheight,ix,iy;

	wwidth = m_drawWidth;
	wheight = m_drawHeight;

	if (x > wwidth || y > wheight)
		return (UT_UCSChar) 0;

	iy = y / (wheight / 7);
	ix = x / (wwidth / 32);

	return (UT_UCSChar) 32 + 32 * iy + ix;
}


void XAP_Draw_Symbol::drawarea( UT_UCSChar c, UT_UCSChar p)
	//
	// This function displays the symbol c into the Selected Area.
	// It also highlights the selected symbol in the Symbol Table.
	//
{
	UT_ASSERT(m_areagc);
	UT_uint32 wwidth,wheight,x,y,cx,cy,px,py,swidth,sheight;
	UT_uint32 cx1,cy1,px1,py1;

	wwidth = m_drawareaWidth;
	wheight = m_drawareaHeight;
	
	// Centre the character.
	unsigned short CharacterWidth;
	m_areagc->measureString(&c, 0, 1, &CharacterWidth);
	x = (m_drawareaWidth - CharacterWidth) / 2;
	y = (m_drawareaHeight - m_areagc->getFontHeight()) / 2;

	m_areagc->clearArea(0, 0, wwidth, wheight);
	m_areagc->drawChar(c, x, y);
	//
	// Calculate the cordinates of the current and previous symbol
	// along with the widths of the appropriate boxes.

	swidth = m_drawWidth;
	sheight = m_drawHeight;
	UT_uint32 tmpw = m_drawWidth / 32;
	UT_uint32 tmph = m_drawHeight / 7;
	UT_uint32 ic = (UT_uint32) c;
	UT_uint32 ip = (UT_uint32) p;

	cx = (ic % 32) * tmpw;
	cx1 = cx + tmpw;
	cy = ((ic / 32) - 1) * tmph;
	cy1 = cy + tmph;

	px =  (ip % 32) * tmpw;
	px1 = px + tmpw;
	py = ((ip / 32) - 1) * tmph;
	py1 = cy + tmph;

	// 
	// Redraw the Previous Character in black on White
	//
	m_gc->clearArea(px + 1, py + 1, tmpw - 1, tmph - 1);
	m_gc->drawChar(p, px + 2, py);
	// 
	// Redraw the Current Character in black on Blue
	//
	UT_RGBColor colour(128, 128, 192);
	m_gc->fillRect(colour, cx + 1, cy + 1, tmpw - 1, tmph - 1);
	m_gc->drawChar(c, cx + 2, cy);
}

void XAP_Draw_Symbol::onLeftButtonDown(UT_sint32 x, UT_sint32 y)
{
	setCurrent(calcSymbol(x, y));
}







