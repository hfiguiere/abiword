/* AbiSource Program Utilities
 * Copyright (C) 1998,1999 AbiSource, Inc.
 * Copyright (C) 2000,2001 Hubert Figuiere <hfiguiere@teaser.fr>
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
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>

#include "ut_types.h"
#include "ut_misc.h"
#include "ut_assert.h"
#include "ut_string.h"
#include "ut_MacString.h"
  
/* this function is defined in ut_string.h */
char * UT_catPathname(const char * szPath, const char * szFile)
{
	UT_ASSERT((szPath) && (*szPath));
	UT_ASSERT((szFile) && (*szFile));
	
	char * szPathname = (char *)UT_calloc(sizeof(char),strlen(szPath)+strlen(szFile)+2);
	UT_ASSERT(szPathname);
	
	sprintf(szPathname,"%s%s%s",
			szPath,
			((szPath[strlen(szPath)-1]=='\\') ? "" : "\\"),
			szFile);

	return szPathname;
}

char * UT_tmpnam(char * base)
{
  return tmpnam(base);
}

void UT_unlink (const char * base)
{
#if defined(XP_MAC_TARGET_MACOSX) 
	unlink (base);
#else
	UT_ASSERT (UT_NOT_IMPLEMENTED);
#endif
}


#if defined(TARGET_API_MAC_CARBON) && (TARGET_API_MAC_CARBON == 1)
/*
    Convert a C string to a Pascal Str255 with String conversion
    
    TODO: move it away of CoreFoundation to something compatible outside of Carbon
*/
void UT_C2PStrWithConversion (const char *inStr, StringPtr outStr, CFStringBuiltInEncodings inCharset, 
                              CFStringBuiltInEncodings outCharset)
{
        CFStringRef  myCFStr = CFStringCreateWithCString (NULL, inStr, inCharset);
        UT_ASSERT (CFStringGetPascalString (myCFStr, outStr, sizeof (Str255), outCharset));
}
#endif
