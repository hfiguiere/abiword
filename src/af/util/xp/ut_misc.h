/* AbiSource Program Utilities
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
 


#ifndef UTMISC_H
#define UTMISC_H

#include "ut_types.h"
#include "ut_xml.h"

class UT_RGBColor;
class UT_Rect;
class UT_Vector;
class UT_String;

// ----------------------------------------------------------------
class ABI_EXPORT UT_RGBColor
{
public:
	UT_RGBColor();
	UT_RGBColor(unsigned char, unsigned char, unsigned char);
	
	unsigned char m_red;
	unsigned char m_grn;
	unsigned char m_blu;
};

void UT_setColor(UT_RGBColor & col, unsigned char r, unsigned char g, unsigned char b);
void UT_parseColor(const char*, UT_RGBColor&);

// ----------------------------------------------------------------
class ABI_EXPORT UT_Rect
{
public:
	UT_Rect();
	UT_Rect(UT_sint32 iLeft, UT_sint32 iTop, UT_sint32 iWidth, UT_sint32 iHeight);
	UT_Rect(const UT_Rect &);
	UT_Rect(const UT_Rect * r);

	bool containsPoint(UT_sint32 x, UT_sint32 y) const;
	void set(UT_sint32 iLeft, UT_sint32 iTop, UT_sint32 iWidth, UT_sint32 iHeight);
	bool intersectsRect(const UT_Rect * pRect) const;
	void unionRect( const UT_Rect *pRect);
	UT_sint32	left;
	UT_sint32	top;
	UT_sint32	width;
	UT_sint32	height;
};

// ----------------------------------------------------------------
struct ABI_EXPORT UT_Point
{
	UT_sint32	x;
	UT_sint32	y;
};

#define UT_MAX(A,B)	(((A) > (B)) ? (A) : (B))
#define UT_MIN(A,B)	(((A) < (B)) ? (A) : (B))
#define UT_ABS(A)	( ((A) < 0) ? (-(A)) : (A) )

const char * UT_pathSuffix(const char * path);
bool UT_isWordDelimiter(UT_UCSChar currentChar, UT_UCSChar followChar);
ABI_EXPORT const XML_Char* UT_getAttribute(const XML_Char* name, const XML_Char** atts);

UT_sint32 signedHiWord(UT_uint32 dw);
UT_sint32 signedLoWord(UT_uint32 dw);

UT_Vector * simpleSplit (const UT_String & str, char separator = ' ',
						 size_t max = 0 /* 0 == full split */);

void warpString(UT_String& str, size_t col_max = 75);

#endif /* UTMISC_H */
