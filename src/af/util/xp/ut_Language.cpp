
#include "ut_Language.h"
#include "ut_string.h"
#include "xap_Strings.h"
#include <stdlib.h>


// Please keep the list below alphabetised by the lang; even though 
// this is not required for it to work, it will make it easier to maintain.

// to add a new language:
// (1) check on this list it is not already there;
// (2) add it to the list in xap_String_Id.h 
// (3) add it here, using the ID corresponding to the one
//     from xap_String_Id.h

static lang_entry s_Table[] = 
{
	//the property value, the localised translation, the numerical id
    {"-none-",	    NULL, XAP_STRING_ID_LANG_0},
	{"cs-CZ",		NULL, XAP_STRING_ID_LANG_4},
	{"da-DK",		NULL, XAP_STRING_ID_LANG_13},
	{"da-NL",		NULL, XAP_STRING_ID_LANG_14},
	//{"de-AU",		NULL, XAP_STRING_ID_LANG_5},
	{"de-DE",		NULL, XAP_STRING_ID_LANG_6},
	{"en-AU",		NULL, XAP_STRING_ID_LANG_1},
	{"en-GB",		NULL, XAP_STRING_ID_LANG_2},
	{"en-US",		NULL, XAP_STRING_ID_LANG_3},
	{"es-ES",		NULL, XAP_STRING_ID_LANG_10},
	{"fi-FI",		NULL, XAP_STRING_ID_LANG_15},
	{"fr-FR",		NULL, XAP_STRING_ID_LANG_7},
	{"it-IT",		NULL, XAP_STRING_ID_LANG_9},
	{"iw-IL",		NULL, XAP_STRING_ID_LANG_8},
	{"pt-PT",		NULL, XAP_STRING_ID_LANG_11},
	{"ru-RU",		NULL, XAP_STRING_ID_LANG_12},
	{"sv-SE",       NULL, XAP_STRING_ID_LANG_16},
};

static int s_compareQ(const void * a, const void *b)
{
	const lang_entry * A = (const lang_entry *) a;
	const lang_entry * B = (const lang_entry *) b;
	return UT_strcmp(A->prop, B->prop);
}

static int s_compareB(const void * l, const void *e)
{
	const XML_Char * L   = (const XML_Char * ) l;
	const lang_entry * E = (const lang_entry *) e;
	return UT_strcmp(L, E->prop);
}

bool UT_Language::s_Init = true;


// the constructor looks up the translations for the property and sorts out the table
// alphabetically by the property

UT_Language::UT_Language()
{
	if(s_Init) //only do this once
	{
		const XAP_StringSet * pSS = XAP_App::getApp()->getStringSet();

		for(UT_uint32 i = 0; i < NrElements(s_Table); i++)
		{
			s_Table[i].lang = (XML_Char *) pSS->getValue(s_Table[i].id);
		}

		qsort(s_Table, NrElements(s_Table), sizeof(lang_entry), s_compareQ);
		s_Init = false;
	}
}

UT_uint32 UT_Language::getCount()
{
	return (NrElements(s_Table));
}

const XML_Char * UT_Language::getNthProperty(UT_uint32 n)
{
	return (s_Table[n].prop);
}

const XML_Char * UT_Language::getNthLanguage(UT_uint32 n)
{
	return (s_Table[n].lang);
}


const XML_Char * UT_Language::getPropertyFromLanguage(const XML_Char * lang)
{
	for(UT_uint32 i = 0; i < NrElements(s_Table); i++)
	{
		if(!UT_strcmp(lang, s_Table[i].lang))
			return s_Table[i].prop;
	}
	return NULL;
}

UT_uint32 UT_Language::getIndxFromProperty(const XML_Char * prop)
{
	for(UT_uint32 i = 0; i < NrElements(s_Table); i++)
	{
		if(!UT_strcmp(prop, s_Table[i].prop))
			return i;
	}
	return 0;
}

UT_uint32 UT_Language::getIdFromProperty(const XML_Char * prop)
{
	lang_entry * e = (lang_entry *) bsearch(prop, s_Table, NrElements(s_Table), sizeof(lang_entry), s_compareB);
	if(e)
		return e->id;
	else
		return 0;
}

// this function is not as useless as might seem; it takes a pointer to a property string, finds the same
// property in the static table and returns the pointer to it
// this is used by fp_TextRun to set its m_pLanguage member; by always refering into the static table
// it is possible to compare the language property by simply comparing the pointers, rather than
// having to use strcmp

const XML_Char *  UT_Language::getPropertyFromProperty(const XML_Char * prop)
{
	lang_entry * e = (lang_entry *) bsearch(prop, s_Table, NrElements(s_Table), sizeof(lang_entry), s_compareB);
	if(e)
		return e->prop;
	else
		return 0;
}

