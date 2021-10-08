/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* AbiWord
 * Copyright (C) 2016-2021 Hubert Figuiere
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

#include <iostream>

#include "tf_test.h"
#include "pp_AttrProp.h"

#define TFSUITE "core.text.ptbl.attrprop"

// because this function isn't public.
extern void ascii_strdown(char *p, size_t len);

TFTEST_MAIN("ascii_strdown")
{
    const char* t1 = "aBd*87T";

    char* lowert1 = strdup(t1);
    ascii_strdown(lowert1, 7);
    TFPASSEQ(lowert1, "abd*87t");
    char* glowert1 = g_ascii_strdown(lowert1, 7);
    TFPASSEQ(glowert1, lowert1);
    g_free(glowert1);
    free(lowert1);

    // test with a larger value and still NUL terminated string.
    lowert1 = strdup(t1);
    ascii_strdown(lowert1, 0xffff);
    TFPASSEQ(lowert1, "abd*87t");
    free(lowert1);
}


// "props" has a defined syntax.
static const PP_PropertyVector attr1 = {
    { "foo", "bar" },
    { "buzz", "word" },
    { "style", "and a style" }
};

static const PP_PropertyVector attr2 = {
    { "font-family", "Comic Sans" },
    { "width", "3in" },
    { "height", "1cm" },
    { "font-size", "42pt" }
};

static const PP_PropertyVector true_attr = {
    { "props", "dom-dir:ltr; font-family:Times New Roman; font-size:10pt; lang:en-GB; line-height:1.0; text-align:justify" },
    { "xid", "1" },
    { "buzz", "word" },
    { "gin", "tonic" }
};

static const PP_PropertyVector broken_attr = {
    { "props", "this is a broken prop" },
    { "xid", "1" },
    { "buzz", "word" },
    { "gin", "tonic" }
};


TFTEST_MAIN("PP_getAttribute")
{
    TFPASSEQ(attr1.size(), 3);
    TFPASS(PP_getAttribute(_PN("foo"), attr1) == "bar");
    TFPASS(PP_getAttribute(_PN("style"), attr1) == "and a style");
    TFPASS(PP_getAttribute(_PN("bar"), attr1) == "");

    PP_PropertyVector attr_odd = attr1;
    attr_odd.push_back({ "oddProp", "" });
    TFPASSEQ(attr_odd.size(), 4);
    TFPASS(PP_getAttribute(_PN("foo"), attr_odd) == "bar");
    TFPASS(PP_getAttribute(_PN("style"), attr_odd) == "and a style");
    TFPASS(PP_getAttribute(_PN("bar"), attr_odd) == "");
}

TFTEST_MAIN("PP_hasAttribute")
{
    TFPASSEQ(attr1.size(), 3);
    TFPASS(PP_hasAttribute(_PN("foo"), attr1));
    TFPASS(!PP_hasAttribute(_PN("bar"), attr1));

    PP_PropertyVector attr_odd = attr1;
    attr_odd.push_back({ "oddProp", "" });
    TFPASSEQ(attr_odd.size(), 4);
    TFPASS(PP_hasAttribute(_PN("foo"), attr_odd));
    TFPASS(!PP_hasAttribute(_PN("bar"), attr_odd));
}


TFTEST_MAIN("PP_setAttribute")
{
    PP_PropertyVector attr = attr1;
    TFPASSEQ(attr.size(), 3);
    TFPASS(PP_setAttribute(_PN("foo"), "fiz", attr));
    TFPASSEQ(attr.size(), 3);
    TFPASS(!PP_setAttribute(_PN("bar"), "buzz", attr));
    TFPASSEQ(attr.size(), 3);
    TFPASS(PP_getAttribute(_PN("foo"), attr) == "fiz");
    TFPASS(PP_getAttribute(_PN("style"), attr) == "and a style");
    TFPASS(PP_getAttribute(_PN("bar"), attr) == "");
    TFPASS(!PP_hasAttribute(_PN("bar"), attr));

    attr = attr1;
    attr.push_back({ "oddProp", "" });
    TFPASSEQ(attr.size(), 4);
    TFPASS(PP_setAttribute(_PN("foo"), "fiz", attr));
    TFPASSEQ(attr.size(), 4);
    TFPASS(!PP_setAttribute(_PN("bar"), "buzz", attr));
    TFPASSEQ(attr.size(), 4);
    TFPASS(PP_getAttribute(_PN("foo"), attr) == "fiz");
    TFPASS(PP_getAttribute(_PN("style"), attr) == "and a style");
    TFPASS(PP_getAttribute(_PN("bar"), attr) == "");
    TFPASS(!PP_hasAttribute(_PN("bar"), attr));
}

TFTEST_MAIN("PP_addOrSetAttribute")
{
    PP_PropertyVector attr = attr1;
    TFPASSEQ(attr.size(), 3);
    TFPASS(PP_setAttribute(_PN("foo"), "fiz", attr));
    TFPASSEQ(attr.size(), 3);
    PP_addOrSetAttribute(_PN("bar"), "buzz", attr);
    TFPASSEQ(attr.size(), 4);

    attr = attr1;
    attr.push_back({ "oddProp", "" });
    TFPASSEQ(attr.size(), 4);
    TFPASS(PP_setAttribute(_PN("foo"), "fiz", attr));
    TFPASSEQ(attr.size(), 4);
    PP_addOrSetAttribute(_PN("bar"), "buzz", attr);
    TFPASSEQ(attr.size(), 5);
}

TFTEST_MAIN("PP_removeAttribute")
{
    PP_PropertyVector attr = attr1;
    TFPASSEQ(attr.size(), 3);
    TFPASS(!PP_removeAttribute(_PN("oddProp"), attr));
    TFPASSEQ(attr.size(), 3);
    TFPASS(PP_removeAttribute(_PN("buzz"), attr));
    TFPASSEQ(attr.size(), 2);

    attr = attr1;
    attr.push_back({ "oddProp", "" });
    TFPASSEQ(attr.size(), 4);
    TFPASS(PP_removeAttribute(_PN("buzz"), attr));
    TFPASSEQ(attr.size(), 3);
    TFPASS(PP_removeAttribute(_PN("oddProp"), attr));
    TFPASSEQ(attr.size(), 2);
}

TFTEST_MAIN("PP_cloneAndDecodeAttributes")
{
    const char *attrs[] = {
        "inline", "&amp; &lt; &gt; &quot;",
        "props", "abc: d; efg: hij",
        NULL, NULL
    };
    PP_PropertyVector props = PP_cloneAndDecodeAttributes(attrs);

    TFPASSEQ(props.size(), 2);
    TFPASS(props[0].value == "& < > \"");
    TFPASS(props[1].name == "props");

    const char *attrs2[] = {
        "inline", "&amp; &lt; &gt; &quot;",
        "props", NULL
    };
    props = PP_cloneAndDecodeAttributes(attrs2);
    TFPASSEQ(props.size(), 2);
    TFPASS(props[1].value == "");
}

TFTEST_MAIN("PP_std_setPropsToValue")
{
    PP_PropertyVector attr;
    TFPASSEQ(attr1.size(), 3);

    attr = PP_std_setPropsToValue(attr1, "winter is coming");
    TFPASSEQ(attr.size(), 3);
    TFPASS(PP_getAttribute(_PN("foo"), attr) == "winter is coming");
    TFPASS(PP_getAttribute(_PN("style"), attr) == "winter is coming");
    TFPASS(PP_getAttribute(_PN("bar"), attr) == "");

    attr = PP_std_setPropsToValue(attr1, nullptr);
    TFPASSEQ(attr.size(), 3);
    TFPASS(PP_getAttribute(_PN("foo"), attr) == "");
    TFPASS(PP_getAttribute(_PN("style"), attr) == "");
    TFPASS(PP_getAttribute(_PN("bar"), attr) == "");

    TFPASS(PP_hasAttribute(_PN("foo"), attr));
    TFPASS(PP_hasAttribute(_PN("style"), attr));
    TFPASS(!PP_hasAttribute(_PN("bar"), attr));
}

TFTEST_MAIN("PP_std_copyProps")
{
    const char *cprops[] = {
        "style", "Normal",
        "lang", "fr",
        NULL, NULL
    };
    PP_PropertyVector attr = PP_std_copyProps(cprops);
    TFPASSEQ(attr.size(), 2);

    TFPASS(PP_getAttribute(_PN("lang"), attr) == "fr");
    TFPASS(PP_getAttribute(_PN("style"), attr) == "Normal");
    TFPASS(PP_getAttribute(_PN("bar"), attr) == "");
    TFPASS(!PP_hasAttribute(_PN("bar"), attr));

    const char *cprops2[] = {
        "style", "Normal",
        "lang", NULL,
        NULL, NULL
    };


    attr = PP_std_copyProps(cprops2);
    TFPASSEQ(attr.size(), 2);

    TFPASS(PP_getAttribute(_PN("lang"), attr) == "");
    TFPASS(PP_hasAttribute(_PN("lang"), attr));
    TFPASS(PP_getAttribute(_PN("style"), attr) == "Normal");
    TFPASS(PP_getAttribute(_PN("bar"), attr) == "");
    TFPASS(!PP_hasAttribute(_PN("bar"), attr));

    attr = PP_std_copyProps(nullptr);
    TFPASS(attr.empty());
}

TFTEST_MAIN("PP_std_setPropsToNothing")
{
    PP_PropertyVector attr = PP_std_setPropsToNothing(attr1);
    TFPASSEQ(attr.size(), 3);
    TFPASS(PP_getAttribute(_PN("foo"), attr) == "");
    TFPASS(PP_getAttribute(_PN("style"), attr) == "");
    TFPASS(PP_getAttribute(_PN("bar"), attr) == "");

    TFPASS(PP_hasAttribute(_PN("foo"), attr));
    TFPASS(PP_hasAttribute(_PN("style"), attr));
    TFPASS(!PP_hasAttribute(_PN("bar"), attr));

    PP_PropertyVector attr_odd = attr1;
    attr_odd.push_back({ "oddProp", "" });
    attr = PP_std_setPropsToNothing(attr_odd);
    TFPASSEQ(attr.size(), 4); // yep, expanded.
    TFPASS(PP_getAttribute(_PN("foo"), attr) == "");
    TFPASS(PP_getAttribute(_PN("style"), attr) == "");
    TFPASS(PP_getAttribute(_PN("bar"), attr) == "");

    TFPASS(PP_hasAttribute(_PN("foo"), attr));
    TFPASS(PP_hasAttribute(_PN("style"), attr));
    TFPASS(PP_hasAttribute(_PN("oddProp"), attr)); // it is in there now.
    TFPASS(!PP_hasAttribute(_PN("bar"), attr));
}

TFTEST_MAIN("PP_makePropString")
{
    PP_PropertyVector props = {
        { "font", "Comic Sans" },
        { "size", "42pt" },
        { "text-align", "left" }
    };

    std::string result = PP_makePropString(PP_NOPROPS);
    TFPASS(result.empty());

    result = PP_makePropString(props);
    TFPASS(result == "font:Comic Sans; size:42pt; text-align:left");

    // Testing after erasing
    props.erase(props.end() - 1, props.end());
    TFPASS(props.size() == 2);
    result = PP_makePropString(props);
    TFPASS(result == "font:Comic Sans; size:42pt");
}

extern UT_uint32 hashcodeBytesAP(UT_uint32 init, const char * pv, UT_uint32 cb);

TFTEST_MAIN("PP_AttrProp checksum")
{
    UT_uint32 crc = hashcodeBytesAP(0, "12345678", 8);
    TFPASSEQ(crc, 0x910e0084);
    crc = hashcodeBytesAP(crc, "abcdefghijklmnopqrstuvwxyz", 26);
    TFPASSEQ(crc, 0x616c5508);

    PP_AttrProp attrProps;

    attrProps.setAttributes(true_attr);
    attrProps.markReadOnly();
    UT_uint32 cs = attrProps.getCheckSum();
    TFPASSEQ(cs, 0x12301177);

    PP_AttrProp attrProps2;

    attrProps2.setAttributes(attr1);
    attrProps2.markReadOnly();
    cs = attrProps2.getCheckSum();
    TFPASSEQ(cs, 0x67232a84);
}

TFTEST_MAIN("PP_AttrProp - add props/attr")
{
    PP_AttrProp attrProps;

    TFPASSEQ(attrProps.getPropertyCount(), 0);
    TFPASS(!attrProps.hasProperties());
    TFPASSEQ(attrProps.getAttributeCount(), 0);
    TFPASS(!attrProps.hasAttributes());

    attrProps.setAttributes(attr1);
    TFPASSEQ(attrProps.getAttributeCount(), attr1.size());

    attrProps.setProperties(attr2);
    TFPASSEQ(attrProps.getAttributeCount(), attr1.size());
    TFPASSEQ(attrProps.getPropertyCount(), attr2.size());
}

TFTEST_MAIN("PP_AttrProp - add attr with props")
{
    PP_AttrProp attrProps;

    TFPASSEQ(attrProps.getPropertyCount(), 0);
    TFPASSEQ(attrProps.getAttributeCount(), 0);

    attrProps.setAttributes(true_attr);
    TFPASSEQ(attrProps.getAttributeCount(), 2);
    TFPASSEQ(attrProps.getPropertyCount(), 6);

    attrProps.setProperties(attr2);
    TFPASSEQ(attrProps.getAttributeCount(), 2);
    TFPASSEQ(attrProps.getPropertyCount(), 8);
}

TFTEST_MAIN("PP_AttrProp - add attr with broken props")
{
    PP_AttrProp attrProps;

    TFPASSEQ(attrProps.getPropertyCount(), 0);
    TFPASSEQ(attrProps.getAttributeCount(), 0);

    attrProps.setAttributes(broken_attr);
    TFPASSEQ(attrProps.getAttributeCount(), 0);
    TFPASSEQ(attrProps.getPropertyCount(), 0);

    attrProps.setProperties(attr2);
    TFPASSEQ(attrProps.getAttributeCount(), 0);
    TFPASSEQ(attrProps.getPropertyCount(), 4);
}

TFTEST_MAIN("PP_AttrProp - operator=")
{
    PP_AttrProp attrProps;

    TFPASSEQ(attrProps.getPropertyCount(), 0);
    TFPASSEQ(attrProps.getAttributeCount(), 0);

    PP_PropertyVector attr = true_attr;
    attrProps.setAttributes(attr);
    attrProps.setProperties(attr2);
    TFPASSEQ(attr2.size(), 4);
    TFPASSEQ(attrProps.getAttributeCount(), 2);
    TFPASSEQ(attrProps.getPropertyCount(), 8);


    PP_AttrProp attrProps2;

    TFPASSEQ(attrProps2.getPropertyCount(), 0);
    TFPASSEQ(attrProps2.getAttributeCount(), 0);

    attrProps2 = attrProps;

    TFPASSEQ(attrProps.getPropertyCount(), attrProps2.getPropertyCount());
    TFPASSEQ(attrProps.getAttributeCount(), attrProps2.getAttributeCount());

#if 0
    printf("attrProps\n");
    for (const auto & prop : attrProps.getAttributes()) {
        std::cout << prop << std::endl;
    }
    printf("attrProps2\n");
    for (const auto & prop : attrProps2.getAttributes()) {
        std::cout << prop << std::endl;
    }

    // this is FAILING for now. The order is probably not the same.
    TFPASS(attrProps.getAttributes() == attrProps2.getAttributes());
    TFPASS(attrProps.getProperties() == attrProps2.getProperties());
#endif
}
