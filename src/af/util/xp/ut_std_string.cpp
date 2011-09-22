/* AbiSource Program Utilities
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2009 Hubert Figuiere
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


#include <string.h>

#include "ut_std_string.h"
#include "ut_string.h"

#if 0
std::string & UT_escapeXML(std::string &s)
{
    gsize incr = 0;

	const char * ptr = s.c_str();
	while (*ptr) {
        if ((*ptr == '<') || (*ptr == '>')) {
            incr += 3;
        }
        else if (*ptr == '&') {
            incr += 4;
        }
        else if (*ptr == '"') {
            incr += 5;
        }
        ptr++;
    }

    gsize slice_size = s.size() + incr + 1;
    char * dest = (char *)g_slice_alloc(slice_size);
    char * current = dest;

	ptr = s.c_str();
	while (*ptr)
    {
        if (*ptr == '<')
        {
            memcpy(dest, "&lt;", 4);
            current += 4;
        }
        else if (*ptr == '>')
        {
            memcpy(dest, "&gt;", 4);
            current += 4;
        }
        else if (*ptr == '&')
        {
            memcpy(dest, "&amp;", 5);
            current += 5;
        }
        else if (*ptr == '"')
        {
            memcpy(dest, "&quot;", 6);
            current += 6;
        }
        ptr++;
    }
    *dest = 0;
    s = dest;
    g_slice_free1(slice_size, dest);
    return s;
}
#endif

 
std::string& UT_std_string_vprintf (std::string & inStr, const char *format,
                                    va_list      args1)
{
    char *buffer = g_strdup_vprintf(format, args1);
    inStr = buffer;
    g_free(buffer);

    return inStr;
}


std::string UT_std_string_sprintf(const char * inFormat, ...)
{
    std::string outStr;

    va_list args;
    va_start (args, inFormat);
    UT_std_string_vprintf (outStr, inFormat, args);
    va_end (args);

    return outStr;
}


bool ends_with( const std::string& s, const std::string& ending )
{
    if( ending.length() > s.length() )
        return false;
    
    return s.rfind(ending) == (s.length() - ending.length());
}

bool starts_with( const std::string& s, const std::string& starting )
{
    int starting_len = starting.length();
    int s_len = s.length();

    if( s_len < starting_len )
        return false;
    
    return !s.compare( 0, starting_len, starting );
}

std::string replace_all( const std::string& s, char oldc, char newc )
{
    std::string ret;
    for( std::string::const_iterator iter = s.begin(); iter != s.end(); ++iter )
    {
        if( *iter == oldc ) ret += newc;
        else                ret += *iter;
    }
    return ret;
}

std::string replace_all( const std::string& s,
                         const std::string& olds,
                         const std::string& news )
{
    std::string ret = s;
    int olds_length = olds.length();
    int news_length = news.length();
            
    int start = ret.find( olds );
    while( start != std::string::npos )
    {
        ret.replace( start, olds_length, news );
        start = ret.find( olds, start + news_length );
    }
    return ret;
}


std::string UT_XML_cloneNoAmpersands( const std::string& src )
{
    gchar* rszDest = 0;
    
    bool rc = UT_XML_cloneNoAmpersands( rszDest, src.c_str() );
    if( !rc )
        return src;

    std::string ret = rszDest;
    FREEP(rszDest);
    return ret;
}

