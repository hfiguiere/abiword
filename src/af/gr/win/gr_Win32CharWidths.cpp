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

#include <windows.h>
#include "ut_Win32OS.h"
#include "gr_Win32CharWidths.h"

//////////////////////////////////////////////////////////////////

void GR_Win32CharWidths::setCharWidthsOfRange(HDC hdc, UT_UCSChar c0, UT_UCSChar c1)
{
#if 0
	UT_uint32 loc0 = (c0 & 0xff);
	UT_uint32 loc1 = (c1 & 0xff);
	UT_uint32 hic0 = ((c0 >> 8) & 0xff);
	UT_uint32 hic1 = ((c1 >> 8) & 0xff);

	if (hic0 == hic1)					// we are contained within one page
	{
		Array256 * pA = NULL;
		if (hic0 == 0)
			pA = &m_aLatin1;
		else if (m_vecHiByte.getItemCount() > hic0)
			pA = (Array256 *)m_vecHiByte.getNthItem(hic0);
		if (pA)
		{
			if (UT_IsWinNT())
				GetCharWidth32(hdc, loc0, loc1, &(pA->aCW[loc0]));
			else
				// GetCharWidth(hdc, loc0, loc1, &(pA->aCW[loc0]));
				GetTextExtentPoint32W( ...se below... );
			return;
		}
	}

	// if we fall out of the above, we're either spanning
	// different pages or we are on a page that hasn't
	// be loaded yet.  do it the hard way....
#endif
	UINT k;
	int w;

	// Windows NT and Windows 95 support the Unicode Font file. 
	// All of the Unicode glyphs can be rendered if the glyph is found in
	// the font file. However, Windows 95 does  not support the Unicode 
	// characters other than the characters for which the particular codepage
	// of the font file is defined.
	// Reference Microsoft knowledge base:
	// Q145754 - PRB ExtTextOutW or TextOutW Unicode Text Output Is Blank
	if (UT_IsWinNT())
	{
		for (k=c0; k<=c1; k++)
		{
			GetCharWidth32W(hdc,k,k,&w);
			setWidth(k,w);
		}
	}
	else
	{
		HFONT hFont = (HFONT) GetCurrentObject(hdc, OBJ_FONT);
		LOGFONT aLogFont;
		int iRes = GetObject(hFont, sizeof(LOGFONT), &aLogFont);
		UT_ASSERT(iRes);

		if(aLogFont.lfCharSet == SYMBOL_CHARSET)
		{
			// Symbol character handling
			for (k=c0; k<=c1; k++)
			{
				SIZE Size;
				char str[sizeof(UT_UCSChar)];
				int iConverted = WideCharToMultiByte(CP_ACP, NULL, 
					(unsigned short*) &k, 1, str, sizeof(str), NULL, NULL);
				GetTextExtentPoint32A(hdc, str, iConverted, &Size);
				setWidth(k,Size.cx);
			}
		}
		else
		{
			// Unicode font and default character sets
			for (k=c0; k<=c1; k++)
			{
				SIZE Size;
				wchar_t sz1[2];
				sz1[0] = k;
				GetTextExtentPoint32W(hdc, sz1, 1, &Size);
				setWidth(k,Size.cx);
			}
		}
	}
}

