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
#include "ut_debugmsg.h"
#include "ut_growbuf.h"
#include "ut_mbtowc.h"
#include "ut_wctomb.h"

//////////////////////////////////////////////////////////////////
// char * UT_catPathname(const char * szPath, const char * szFile);
// is defined in platform-specific code.
//////////////////////////////////////////////////////////////////

char * UT_strdup(const char * szSource) 
{ 
	UT_ASSERT(szSource);

	int len = strlen(szSource)+1;
	if(char * ret = (char *)calloc(len, sizeof(char)))
		return((char *)memcpy(ret, szSource, len));
	else
		return(NULL);
}

UT_sint32 UT_stricmp(const char * s1, const char * s2)
{
	UT_ASSERT(s1);
	UT_ASSERT(s2);
	
	while(tolower(*s1) == tolower(*s2) && *s1 != '\0' && *s2 != '\0')
	{
		s1++;
		s2++;
	}
	
	if (*s1 == '\0' && *s2 == '\0')
	{
		return 0;
	}
	else
	{
		return ((*s1)-(islower(*s1)?tolower(*s2):(isupper(*s1)?toupper(*s2):*s2)));
	}
}

UT_sint32 UT_strnicmp(const char *s1, const char *s2, int ilen)
{
	UT_ASSERT(s1);
	UT_ASSERT(s2);

	while((ilen--)>0 && tolower(*s1) == tolower(*s2) && *s1 != '\0' && *s2 != '\0')
	{
		s1++;
		s2++;
	}
	
	if(ilen==-1 || (*s1 == '\0' && *s2 == '\0'))
	{
		return 0;
	}
	else
	{
		return ((*s1)-(islower(*s1)?tolower(*s2):(isupper(*s1)?toupper(*s2):*s2)));
	}
}

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
	return strcmp(sz1,sz2);
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

/* This uses the clone no ampersands but dumps into a static buffer */
XML_Char *UT_XML_transNoAmpersands(const XML_Char * szSource)
{
	static XML_Char *rszDestBuffer = NULL;
	static UT_uint32 iDestBufferLength = 0;

	if (szSource == NULL)
		return NULL;

	UT_uint32 length = UT_XML_strlen(szSource) + 1;
	if (length > iDestBufferLength) {
		if (rszDestBuffer && iDestBufferLength) {
			free(rszDestBuffer);
		}
		iDestBufferLength = 0;
		rszDestBuffer = (XML_Char *) calloc(length, sizeof(XML_Char));

		if (!rszDestBuffer)
			return NULL;

		iDestBufferLength = length;
	}
	memset(rszDestBuffer, 0, iDestBufferLength);

	const XML_Char * o = szSource;
	XML_Char * n = rszDestBuffer;
	while (*o != 0)
	{
		if (*o != '&')
		{
			*n = *o;
			n++;
		}
		o++;
	}

	return rszDestBuffer;
}


// TODO : put a better strncpy here; resolve to platform version if available

UT_uint32 UT_XML_strncpy(XML_Char * szDest, UT_uint32 nLen, const XML_Char * szSource)
{
	if (!szSource)
		return 0;
	
	UT_ASSERT(szDest);

	UT_uint32 i = 0;

	while (i < nLen - 1)
	{
		szDest[i] = szSource[i];

		// if we just wrote NULL, return
		if (szDest[i] == 0)
			return i;
		
		i++;
	}

	return i;
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

	UT_Mbtowc m;
	wchar_t wc;
	while (*s != NULL)
	  {
		if(m.mbtowc(wc,*s))*d++=wc;
		s++;
	  }
	*d = NULL;

	return dest;
}

char * UT_UCS_strcpy_to_char(char * dest, const UT_UCSChar * src)
{
	UT_ASSERT(dest);
	UT_ASSERT(src);

	char * 			d = dest;
	UT_UCSChar * 	s = (UT_UCSChar *) src;

	UT_Wctomb w;
	while (*s != NULL)
	  {
		int length;
		if(w.wctomb(d,length,*s++))
		  d+=length;
	  }
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
  UT_uint32 length = MB_LEN_MAX*strlen(src) + 1;
  *dest = (UT_UCSChar *)calloc(length,sizeof(UT_UCSChar));
  if (!*dest)
	return UT_FALSE;
  UT_UCSChar * d= *dest;
  unsigned char * s	= (unsigned char *) src;
  
  UT_Mbtowc m;
  wchar_t wc;

  while (*s != NULL)
	{
	  if(m.mbtowc(wc,*s))*d++=wc;
	  s++;
	}
  *d = NULL;  
  
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

#if 1 // i didn't get a chance to test this -- jeff
XML_Char* UT_encodeUTF8char(UT_UCSChar cIn)
{
	// convert the given unicode character into a UTF8 sequence
	// return pointer to static buffer.

	static XML_Char sBuf[10];

	memset(sBuf,0,sizeof(sBuf));
	if (cIn < 0x0080)
	{
		sBuf[0] = (XML_Char)cIn;
	}
	else if (cIn < 0x0800)
	{
		// 110xxxxx 10xxxxxx
		sBuf[0] = 0x00c0 | ((cIn >> 6) & 0x001f);
		sBuf[1] = 0x0080 | (    cIn    & 0x003f);
	}
	else
	{
		// 1110xxxx 10xxxxxx 10xxxxxx
		sBuf[0] = 0x00e0 | ((cIn >> 12) & 0x000f);
		sBuf[1] = 0x0080 | ((cIn >>  6) & 0x003f);
		sBuf[2] = 0x0080 | (    cIn     & 0x003f);
	}

	return sBuf;
}
#endif // --jeff

static void endElement(void *userData, const XML_Char *name)
{
}

static void startElement(void *userData, const XML_Char *name, const XML_Char **atts)
{
	XML_Char **pout = (XML_Char **)userData;
	//What do we do with this, is this cast safe!?
	*pout = (XML_Char *)atts[1];
}

XML_Char *UT_decodeXMLstring(XML_Char *in)
{
	// There has *got* to be an easier way to do this with expat, but I
	// didn't spot it from looking at the expat source code.  Anyhow, this
	// is just used during init to chomp the preference default value
	// strings, so the amount of work done probably doesn't matter too
	// much.
	const char s1[] = "<fake blah=\"";
	const char s2[] = "\"/>";
	XML_Char *out = 0;
	XML_Parser parser = 0;
	parser = XML_ParserCreate(0);
	XML_SetUserData(parser, &out);
	XML_SetElementHandler(parser, startElement, endElement);
	if (!XML_Parse(parser, s1, sizeof(s1)-1, 0)
	||  !XML_Parse(parser, in, strlen(in), 0)
	||  !XML_Parse(parser, s2, sizeof(s2)-1, 0))
	{
		UT_DEBUGMSG(("XML parsing error %s; %s:%d\n", XML_ErrorString(XML_GetErrorCode(parser)), __FILE__, __LINE__));
	}
	// TODO: who owns the storage for this?
	out = UT_strdup(out);
	if (parser) XML_ParserFree(parser);
	return out;
	
}
