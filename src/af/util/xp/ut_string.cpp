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

UT_sint32 UT_stricmp(const char *s1, const char *s2)
{
#ifdef WIN32
	return stricmp(s1,s2);
#else
	return strcasecmp(s1,s2);
#endif
}

UT_sint32 UT_strnicmp(const char *s1, const char *s2, int lenS1)
{
#ifdef WIN32
	return strnicmp(s1,s2,lenS1);
#else
	// TODO - fix this!  strncasecmp is NOT portable (for example, AIX doesn't have it)
	return strncasecmp(s1,s2,lenS1);
#endif
}

UT_Bool UT_cloneString(char *& rszDest, const char * szSource)
{
	if (szSource && *szSource)
		rszDest = strdup(szSource);
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

UT_uint32 UT_UCS_strlen(const UT_UCSChar * string)
{
	UT_uint32 i;

	for(i = 0; *string != NULL; string++, i++)
		;

	return i;
}
