/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */
/* AbiWord
 * Copyright (C) 2021 Hubert Figuière
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


#include <unordered_set>

#include "ut_static_string.h"

static std::unordered_set<UT_StaticString> s_interned_strings;

// XXX this will leak. test only
size_t UT_StaticString_clear_interned()
{
    size_t size = s_interned_strings.size();
    s_interned_strings.clear();
    return size;
}

size_t UT_StaticString_count_interned()
{
    return s_interned_strings.size();
}

UT_StaticString UT_StaticString::Interned(const std::string& s)
{
    return UT_StaticString::Interned(s.c_str());
}

UT_StaticString UT_StaticString::Interned(const char* s)
{
    auto iter = s_interned_strings.find(UT_StaticString(s));
    if (iter != s_interned_strings.end()) {
        return *iter;
    }

    // This will leak. We consider this being static.
    UT_StaticString ss(static_cast<const char*>(strdup(s)));
    s_interned_strings.insert(ss);

    return ss;
}
