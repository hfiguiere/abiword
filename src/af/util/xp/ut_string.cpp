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
 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "ut_types.h"
#include "ut_misc.h"
#include "ut_assert.h"
#include "ut_string.h"
#include "ut_growbuf.h"

//////////////////////////////////////////////////////////////////
// The following functions defined in ut_string.h are defined
// in platform code:
//    UT_strcmp()
//    UT_stricmp()
//    UT_strnicmp()
//////////////////////////////////////////////////////////////////

UT_Bool UT_cloneString(char *& rszDest, const char * szSource)
{
	if (szSource && *szSource)
	{
		UT_uint32 length = strlen(szSource) + 1;
		rszDest = (char *)calloc(length,sizeof(char));
		if (!rszDest)
			return UT_FALSE;
		memmove(rszDest,szSource,length*sizeof(char));
	}
	else
		rszDest = NULL;
	return UT_TRUE;
}

UT_Bool UT_replaceString(char *& rszDest, const char * szSource)
{
	if (rszDest)
		free(rszDest);
	rszDest = NULL;

	return UT_cloneString(rszDest,szSource);
}

UT_uint32 UT_XML_strlen(const XML_Char * sz)
{
	if (!sz || !*sz)
		return 0;

	UT_uint32 k = 0;
	while (sz[k])
		k++;

	return k;
}

// Is this function implemented somewhere else?

UT_Bool UT_XML_cloneList(XML_Char **& rszDest, const XML_Char ** szSource)
{
	if (!szSource)
		return UT_TRUE;

	XML_Char ** newmemory = (XML_Char **)
		calloc(UT_pointerArrayLength((void **) szSource) + 1, sizeof(XML_Char *));

	if (newmemory == NULL)
		return UT_FALSE;

	memcpy((void *) newmemory, (const void *) szSource,
		   UT_pointerArrayLength((void **) szSource ) * sizeof(XML_Char *));

	rszDest = newmemory;

	return UT_TRUE;
}

UT_Bool UT_XML_replaceList(XML_Char **& rszDest, const XML_Char ** szSource)
{
	FREEP(rszDest);

	return UT_XML_cloneList(rszDest, szSource);
}
	
UT_Bool UT_XML_cloneString(XML_Char *& rszDest, const XML_Char * szSource)
{
	UT_uint32 length = UT_XML_strlen(szSource) + 1;
	rszDest = (XML_Char *)calloc(length,sizeof(XML_Char));
	if (!rszDest)
		return UT_FALSE;
	memmove(rszDest,szSource,length*sizeof(XML_Char));
	return UT_TRUE;
}

UT_sint32 UT_XML_stricmp(const XML_Char * sz1, const XML_Char * sz2)
{
	UT_ASSERT(sizeof(char) == sizeof(XML_Char));
	return UT_stricmp(sz1,sz2);
}

UT_sint32 UT_XML_strcmp(const XML_Char * sz1, const XML_Char * sz2)
{
	UT_ASSERT(sizeof(char) == sizeof(XML_Char));
	return UT_strcmp(sz1,sz2);
}

UT_Bool UT_XML_cloneNoAmpersands(XML_Char *& rszDest, const XML_Char * szSource)
{
	if (szSource == NULL)
		return UT_FALSE;

	UT_uint32 length = UT_XML_strlen(szSource) + 1;
	rszDest = (XML_Char *) calloc(length, sizeof(XML_Char));

	if (!rszDest)
		return UT_FALSE;

	const XML_Char * o = szSource;
	XML_Char * n = rszDest;
	while (*o != 0)
	{
		if (*o != '&')
		{
			*n = *o;
			n++;
		}
		o++;
	}

	return UT_TRUE;
}

UT_uint32 UT_pointerArrayLength(void ** array)
{
	if (! (array && *array))
		return 0;

	UT_uint32 i = 0;
	while (array[i])
		i++;

	return i;
}

/*
  The following code is from the GNU C library, version 2.0.6.
  It has been reformatted and tweaked to do Unicode strstrs.
  All this licensing stuff is kinda ugly, but I didn't want
  to risk merging the licensing for fear I might break some law.
*/

/* Copyright (C) 1994, 1996 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/*
 * My personal strstr() implementation that beats most other algorithms.
 * Until someone tells me otherwise, I assume that this is the
 * fastest implementation of strstr() in C.
 * I deliberately chose not to comment it.  You should have at least
 * as much fun trying to understand it, as I had to write it :-).
 *
 * Stephen R. van den Berg, berg@pool.informatik.rwth-aachen.de */

typedef UT_UCSChar chartype;

UT_UCSChar * UT_UCS_strstr(const UT_UCSChar * phaystack, const UT_UCSChar * pneedle)
{
	register const UT_UCSChar *haystack, *needle;
	register chartype b, c;

	haystack = (const UT_UCSChar *) phaystack;
	needle = (const UT_UCSChar *) pneedle;

	b = *needle;
	if (b != '\0')
    {
		haystack--;                               /* possible ANSI violation */
		do
        {
			c = *++haystack;
			if (c == '\0')
				goto ret0;
        }
		while (c != b);

		c = *++needle;
		if (c == '\0')
			goto foundneedle;
		++needle;
		goto jin;

		for (;;)
        {
			register chartype a;
			register const UT_UCSChar *rhaystack, *rneedle;

			do
            {
				a = *++haystack;
				if (a == '\0')
					goto ret0;
				if (a == b)
					break;
				a = *++haystack;
				if (a == '\0')
					goto ret0;
			shloop: ; // need a statement here for EGCS 1.1.1 to accept it
			}
			while (a != b);

		jin:	a = *++haystack;
			if (a == '\0')
				goto ret0;

			if (a != c)
				goto shloop;

			rhaystack = haystack-- + 1;
			rneedle = needle;
			a = *rneedle;

			if (*rhaystack == a)
				do
				{
					if (a == '\0')
						goto foundneedle;
					++rhaystack;
					a = *++needle;
					if (*rhaystack != a)
						break;
					if (a == '\0')
						goto foundneedle;
					++rhaystack;
					a = *++needle;
				}
				while (*rhaystack == a);

			needle = rneedle;             /* took the register-poor approach */

			if (a == '\0')
				break;
        }
    }
 foundneedle:
	return (UT_UCSChar *) haystack;
 ret0:
	return 0;
}

UT_sint32 UT_UCS_strcmp(const UT_UCSChar* left, const UT_UCSChar* right)
{
	UT_ASSERT(left);
	UT_ASSERT(right);
	
	while (*left && *right)
	{
		if (*left < *right)
		{
			return -1;
		}

		if (*left > *right)
		{
			return 1;
		}

		left++;
		right++;
	}

	if (*left)
	{
		return -1;
	}
	else if (*right)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
  Latin-1 Unicode case-insensitive string comparison and casing done by
  Pierre Sarrazin <ps@cam.org>.
*/

/*	Converts the given character to lowercase if it is an uppercase letter.
	Returns it unchanged if it is not.
	This function created by Pierre Sarrazin 1999-02-06
*/
  
UT_UCSChar UT_UCS_tolower(UT_UCSChar c)
{
	if (c < 128)
		return tolower(c);
	if (c >= 256)
		return c;  /* Unicode but not Latin-1 - don't know what to do */
	if (c >= 0xC0 && c <= 0xDE && c != 0xD7)  /* uppercase Latin-1 chars */
		return c + 0x20;
	return c;
}


/*	Characters are converted to lowercase (if applicable) when they
	are read from the needle or the haystack. See UT_UCS_tolower().
	This function created by Pierre Sarrazin 1999-02-06
*/

UT_UCSChar * UT_UCS_stristr(const UT_UCSChar * phaystack, const UT_UCSChar * pneedle)
{
	register const UT_UCSChar *haystack, *needle;
	register chartype b, c;

	haystack = (const UT_UCSChar *) phaystack;
	needle = (const UT_UCSChar *) pneedle;

	b = UT_UCS_tolower(*needle);
	if (b != '\0')
    {
		haystack--;                               /* possible ANSI violation */
		do
        {
			c = UT_UCS_tolower(*++haystack);
			if (c == '\0')
				goto ret0;
        }
		while (c != b);

		c = UT_UCS_tolower(*++needle);
		if (c == '\0')
			goto foundneedle;
		++needle;
		goto jin;

		for (;;)
        {
			register chartype a;
			register const UT_UCSChar *rhaystack, *rneedle;

			do
            {
				a = UT_UCS_tolower(*++haystack);
				if (a == '\0')
					goto ret0;
				if (a == b)
					break;
				a = UT_UCS_tolower(*++haystack);
				if (a == '\0')
					goto ret0;
			shloop: ; // need a statement here for EGCS 1.1.1 to accept it
			}
			while (a != b);

		jin:	a = UT_UCS_tolower(*++haystack);
			if (a == '\0')
				goto ret0;

			if (a != c)
				goto shloop;

			rhaystack = haystack-- + 1;
			rneedle = needle;
			a = UT_UCS_tolower(*rneedle);

			if (UT_UCS_tolower(*rhaystack) == a)
				do
				{
					if (a == '\0')
						goto foundneedle;
					++rhaystack;
					a = UT_UCS_tolower(*++needle);
					if (UT_UCS_tolower(*rhaystack) != a)
						break;
					if (a == '\0')
						goto foundneedle;
					++rhaystack;
					a = UT_UCS_tolower(*++needle);
				}
				while (UT_UCS_tolower(*rhaystack) == a);

			needle = rneedle;             /* took the register-poor approach */

			if (a == '\0')
				break;
        }
    }
 foundneedle:
	return (UT_UCSChar *) haystack;
 ret0:
	return 0;
}
/****************************************************************************/

UT_uint32 UT_UCS_strlen(const UT_UCSChar * string)
{
	UT_uint32 i;

	for(i = 0; *string != NULL; string++, i++)
		;

	return i;
}

UT_UCSChar * UT_UCS_strcpy(UT_UCSChar * dest, const UT_UCSChar * src)
{
	UT_ASSERT(dest);
	UT_ASSERT(src);
	
	UT_UCSChar * d = dest;
	UT_UCSChar * s = (UT_UCSChar *) src;

	while (*s != NULL)
		*d++ = *s++;
	*d = NULL;

	return dest;
}

// TODO shouldn't all of the 'char *' strings be 'unsigned char *' strings ??

UT_UCSChar * UT_UCS_strcpy_char(UT_UCSChar * dest, const char * src)
{
	UT_ASSERT(dest);
	UT_ASSERT(src);
	
	UT_UCSChar * d 		= dest;
	unsigned char * s	= (unsigned char *) src;

	while (*s != NULL)
		*d++ = *s++;
	*d = NULL;

	return dest;
}

char * UT_UCS_strcpy_to_char(char * dest, const UT_UCSChar * src)
{
	UT_ASSERT(dest);
	UT_ASSERT(src);

	char * 			d = dest;
	UT_UCSChar * 	s = (UT_UCSChar *) src;

	while (*s != NULL)
		*d++ = (char) *s++;
	*d = NULL;
	
	return dest;
}

UT_Bool UT_UCS_cloneString(UT_UCSChar ** dest, const UT_UCSChar * src)
{
	UT_uint32 length = UT_UCS_strlen(src) + 1;
	*dest = (UT_UCSChar *)calloc(length,sizeof(UT_UCSChar));
	if (!*dest)
		return UT_FALSE;
	memmove(*dest,src,length*sizeof(UT_UCSChar));

	return UT_TRUE;
}

UT_Bool UT_UCS_cloneString_char(UT_UCSChar ** dest, const char * src)
{
	UT_uint32 length = strlen(src) + 1;
	*dest = (UT_UCSChar *)calloc(length,sizeof(UT_UCSChar));
	if (!*dest)
		return UT_FALSE;
	UT_UCS_strcpy_char(*dest, src);

	return UT_TRUE;
}

// convert each character in a string to ASCII uppercase
char * UT_upperString(char * string)
{
	if (!string)
		return NULL;

	for (char * ch = string; *ch != NULL; ch++)
		*ch = toupper(*ch);

	return string;
}

// convert each character in a string to ASCII lowercase
char * UT_lowerString(char * string)
{
	if (!string)
		return NULL;

	for (char * ch = string; *ch != NULL; ch++)
		*ch = tolower(*ch);

	return string;
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

UT_UCSChar UT_decodeUTF8char(const XML_Char * p, UT_uint32 len)
{
	UT_UCSChar ucs, ucs1, ucs2, ucs3;
	
	switch (len)
	{
	case 2:
		ucs1 = (UT_UCSChar)(p[0] & 0x1f);
		ucs2 = (UT_UCSChar)(p[1] & 0x3f);
		ucs  = (ucs1 << 6) | ucs2;
		return ucs;
		
	case 3:
		ucs1 = (UT_UCSChar)(p[0] & 0x0f);
		ucs2 = (UT_UCSChar)(p[1] & 0x3f);
		ucs3 = (UT_UCSChar)(p[2] & 0x3f);
		ucs  = (ucs1 << 12) | (ucs2 << 6) | ucs3;
		return ucs;
		
	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		return 0;
	}
}

void UT_decodeUTF8string(const XML_Char * pString, UT_uint32 len, UT_GrowBuf * pResult)
{
	// decode the given string [ p[0]...p[len] ] and append to the given growbuf.

	UT_ASSERT(sizeof(XML_Char) == sizeof(UT_Byte));
	UT_Byte * p = (UT_Byte *)pString;	// XML_Char is signed...
	
	int bytesInSequence = 0;
	int bytesExpectedInSequence = 0;
	XML_Char buf[5];
	
	for (UT_uint32 k=0; k<len; k++)
	{
		if (p[k] < 0x80)						// plain us-ascii part of latin-1
		{
			UT_ASSERT(bytesInSequence == 0);
			UT_UCSChar c = p[k];
			pResult->append(&c,1);
		}
		else if ((p[k] & 0xf0) == 0xf0)			// lead byte in 4-byte surrogate pair
		{
			// surrogate pairs are defined in section 3.7 of the
			// unicode standard version 2.0 as an extension
			// mechanism for rare characters in future extensions
			// of the unicode standard.
			UT_ASSERT(bytesInSequence == 0);
			UT_ASSERT(UT_NOT_IMPLEMENTED);
		}
		else if ((p[k] & 0xe0) == 0xe0)			// lead byte in 3-byte sequence
		{
			UT_ASSERT(bytesInSequence == 0);
			bytesExpectedInSequence = 3;
			buf[bytesInSequence++] = p[k];
		}
		else if ((p[k] & 0xc0) == 0xc0)			// lead byte in 2-byte sequence
		{
			UT_ASSERT(bytesInSequence == 0);
			bytesExpectedInSequence = 2;
			buf[bytesInSequence++] = p[k];
		}
		else if ((p[k] & 0x80) == 0x80)			// trailing byte in multi-byte sequence
		{
			UT_ASSERT(bytesInSequence > 0);
			buf[bytesInSequence++] = p[k];
			if (bytesInSequence == bytesExpectedInSequence)		// final byte in multi-byte sequence
			{
				UT_UCSChar c = UT_decodeUTF8char(buf,bytesInSequence);
				pResult->append(&c,1);
				bytesInSequence = 0;
				bytesExpectedInSequence = 0;
			}
		}
	}
}

