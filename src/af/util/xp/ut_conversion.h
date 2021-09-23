/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */

/* AbiSource Program Utilities
 * Copyright (C) 2010 AbiSource, Inc.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef UT_CONVERSION_H
#define UT_CONVERSION_H

#include <string>
#include <sstream>

template < typename ClassName >
static ClassName toType( const char* s )
{
    ClassName ret = 0;
    std::stringstream ss;
    ss << s;
    ss >> ret;
    return ret;
}
template < typename ClassName >
static ClassName toType( std::string s )
{
    ClassName ret = 0;
    std::stringstream ss;
    ss << s;
    ss >> ret;
    return ret;
}

template < typename T >
static std::string tostr( T v )
{
    std::stringstream ss;
    ss << v;
    return ss.str();
}

#endif // reinclude protection
