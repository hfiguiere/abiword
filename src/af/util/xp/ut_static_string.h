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

#pragma once

#include <string>

#include <string.h>

// For hashcode()
#include "ut_string_class.h"

#if IN_TFTEST
size_t UT_StaticString_clear_interned();
#endif

#if DEBUG
size_t UT_StaticString_count_interned();
#endif

/// Enforce a static string for property name
///
/// The asumption is that UT_StaticString is like a const char* in size
/// And that eveything is optimized out.
/// Doesn't protect from nullptr
///
class UT_StaticString
{
public:
    static UT_StaticString Interned(const std::string& s);
    static UT_StaticString Interned(const char* s);

    UT_StaticString()
        : m_str(nullptr) {}
    explicit UT_StaticString(const char* s)
        : m_str(s) {}
    // This constructor is deleted forcing an error on non const string.
    UT_StaticString(char* s) = delete;
    bool operator==(UT_StaticString rhs) const
        {
            return *this == rhs.c_str();
        }
    bool operator==(const std::string& rhs) const
        {
            return rhs == m_str;
        }
    bool operator==(const char* rhs) const
        {
            return strcmp(m_str, rhs) == 0;
        }
    bool operator!=(UT_StaticString rhs) const
        {
            return !(*this == rhs);
        }
    bool operator!=(const char* rhs) const
        {
            return strcmp(m_str, rhs) != 0;
        }
    bool operator!() const
        {
            return m_str == nullptr;
        }
    bool operator<(UT_StaticString rhs) const
        {
            return strcmp(this->c_str(), rhs.c_str()) < 0;
        }

    const char* c_str() const
        {
            return m_str;
        }
    size_t size() const
        {
            if (m_str) {
                return strlen(m_str);
            }
            return 0;
        }
private:
    const char* m_str;
};

inline
bool operator==(const std::string& lhs, UT_StaticString ss)
{
    return strcmp(lhs.c_str(), ss.c_str()) == 0;
}

inline
std::string& operator+=(std::string& s, UT_StaticString ss)
{
    if (!!ss) {
        s += ss.c_str();
    }
    return s;
}

namespace std
{
    template<> struct hash<UT_StaticString>
    {
        std::size_t operator()(UT_StaticString s) const noexcept
            {
                return hashcode(s.c_str());
            }
    };
}

#define _SS(x) UT_StaticString(x)
