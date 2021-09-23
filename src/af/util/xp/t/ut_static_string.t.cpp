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

#include <string>
#include <set>
#include <unordered_set>

#include "tf_test.h"
#define IN_TFTEST 1
#include "ut_static_string.h"

#define TFSUITE "core.af.util.staticstring"

TFTEST_MAIN("UT_static_string")
{
    const char* a_string = "static string";
    UT_StaticString ss1(a_string);

    // Should be the same pointer
    TFPASS(ss1.c_str() == a_string);

    // Equality operator
    TFPASS(ss1 == a_string);
    TFPASS(ss1 == std::string(a_string));

    // UT_StaticString copy
    UT_StaticString ss2;

    TFPASS(!ss2);
    TFPASS(ss2.c_str() == nullptr);

    ss2 = ss1;
    TFPASS(ss2 == ss1);
    TFPASS(ss2.c_str() == a_string);
}

// Test UT_StaticString for use in containers
template<class T> void test_set()
{
    T a_set;

    TFPASSEQ(a_set.size(), 0);

    const char* a_string = "static string";
    UT_StaticString ss1(a_string);

    // Basic insertion
    auto result = a_set.insert(ss1);
    TFPASS(result.second);
    TFPASSEQ(a_set.size(), 1);
    // Find once inserted
    TFPASS(a_set.find(ss1) != a_set.end());

    // Same value, different string
    std::string str1(a_string);
    TFPASS(ss1.c_str() != str1.c_str());
    TFPASS(ss1 == str1);
    // NOTE: this is an incorrect use of UT_StaticString
    // But it allow us to have a different pointer.
    // Found by value
    TFPASS(a_set.find(UT_StaticString(str1.c_str())) != a_set.end());

    // NOTE: still an incorrect use of UT_StaticString
    // but lifetime should be OK
    result = a_set.insert(UT_StaticString(str1.c_str()));
    // Not added.
    TFPASS(!result.second);
    TFPASSEQ(a_set.size(), 1);

    const char* another_string = "yet another string";
    UT_StaticString ss2(another_string);

    // Different value
    TFPASS(a_set.find(ss2) == a_set.end());
}

TFTEST_MAIN("UT_static_string in map")
{
    test_set<std::set<UT_StaticString>>();
}

TFTEST_MAIN("UT_static_string in unordered_map")
{
    test_set<std::unordered_set<UT_StaticString>>();
}

TFTEST_MAIN("UT_static_string::Interned")
{
    UT_StaticString_clear_interned();

    const char* another_string = "yet another string";

    auto ss1 = UT_StaticString::Interned(another_string);
    TFPASSEQ(UT_StaticString_count_interned(), 1);

    TFPASS(ss1.c_str() != another_string);
    TFPASS(ss1 == another_string);

    const char* another_string2 = "yet another string";

    auto ss2 = UT_StaticString::Interned(another_string2);
    TFPASSEQ(UT_StaticString_count_interned(), 1);

    TFPASS(ss2.c_str() != another_string2);
    // This is supposed to be the same pointer.
    TFPASS(ss2.c_str() == ss1.c_str());
    // Check that the string is equal. It can pass while the above test fails.
    TFPASS(ss2 == another_string);
    TFPASS(ss2 == another_string2);


    const char* another_string3 = "what about this";

    auto ss3 = UT_StaticString::Interned(another_string3);
    TFPASSEQ(UT_StaticString_count_interned(), 2);

    TFPASS(ss3.c_str() != another_string3);
    // This is supposed to be a different pointer.
    TFPASS(ss3.c_str() != ss1.c_str());
    // check it's still the right string
    TFPASS(ss3 == another_string3);

}
