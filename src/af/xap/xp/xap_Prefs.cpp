/* AbiSource Application Framework
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

// TODO add code to do an auto save anytime anything is changed.

#include <stdlib.h>
#include <stdio.h>

#include "ut_debugmsg.h"
#include "xap_Prefs.h"

#define NrElements(a)		(sizeof(a) / sizeof(a[0]))
#define FREEP(p)	do { if (p) free(p); (p)=NULL; } while (0)
#define DELETEP(p)	do { if (p) delete(p); (p)=NULL; } while (0)

/*****************************************************************/

XAP_PrefsScheme::XAP_PrefsScheme(const XML_Char * szSchemeName)
	: m_hash(13)
{
	if (szSchemeName && *szSchemeName)
		UT_XML_cloneString(m_szName,szSchemeName);
	else
		m_szName = NULL;
}

XAP_PrefsScheme::~XAP_PrefsScheme(void)
{
	FREEP(m_szName);
}

const XML_Char * XAP_PrefsScheme::getSchemeName(void) const
{
	return m_szName;
}

UT_Bool XAP_PrefsScheme::setSchemeName(const XML_Char * szNewSchemeName)
{
	FREEP(m_szName);
	return UT_XML_cloneString(m_szName,szNewSchemeName);
}

UT_Bool XAP_PrefsScheme::setValue(const XML_Char * szKey, const XML_Char * szValue)
{
	UT_HashTable::UT_HashEntry * pEntry = m_hash.findEntry(szKey);
	if (pEntry)
	{
		if (UT_stricmp(szValue,pEntry->pszRight) == 0)
			return UT_TRUE;				// equal values, no changes required
		
		m_hash.setEntry(pEntry, szValue, NULL); // update with new value
		return UT_TRUE;
	}

	// otherwise, need to add a new entry

	m_hash.addEntry(szKey,szValue,NULL);
	return UT_TRUE;
}

UT_Bool XAP_PrefsScheme::getValue(const XML_Char * szKey, const XML_Char ** pszValue) const
{
	UT_HashTable::UT_HashEntry * pEntry = m_hash.findEntry(szKey);
	if (!pEntry)
		return UT_FALSE;

	if (pszValue)
		*pszValue = pEntry->pszRight;
	return UT_TRUE;
}

UT_Bool XAP_PrefsScheme::getNthValue(UT_uint32 k, const XML_Char ** pszKey, const XML_Char ** pszValue) const
{
	// TODO we should fix hash to use ut_uint32 rather than int
	
	if (k >= (UT_uint32)m_hash.getEntryCount())
		return UT_FALSE;
	
	UT_HashTable::UT_HashEntry * pEntry = m_hash.getNthEntryAlpha(k);

	if (!pEntry)
		return UT_FALSE;

	if (pszKey)
		*pszKey = pEntry->pszLeft;
	if (pszValue)
		*pszValue = pEntry->pszRight;

	return UT_TRUE;
}
/*****************************************************************/

UT_Bool XAP_Prefs::getAutoSave(void) const
{
	return m_bAutoSave;
}

void XAP_Prefs::setAutoSave(UT_Bool bAuto)
{
	m_bAutoSave = bAuto;

	// TODO if turning autosave on, we should do a save now....
}


/*****************************************************************/

XAP_Prefs::XAP_Prefs(XAP_App * pApp)
{
	m_pApp = pApp;
	m_bAutoSave = UT_TRUE;				// TODO this is true for testing, set it to false later.
	m_currentScheme = NULL;

	// NOTE: since constructors cannot report malloc
	// NOTE: failures (and since it is virtual back
	// NOTE: to the application), our creator must call
	// NOTE: loadBuiltinPrefs() if it so desires.

	// NOTE: we do not initialize the values in m_parserState
	// NOTE: since they are only used by the parser, it can
	// NOTE: initialize them.
}

XAP_Prefs::~XAP_Prefs(void)
{
	UT_VECTOR_PURGEALL(XAP_PrefsScheme *, m_vecSchemes);
}

XAP_PrefsScheme * XAP_Prefs::getNthScheme(UT_uint32 k) const
{
	UT_uint32 kLimit = m_vecSchemes.getItemCount();
	if (k < kLimit)
		return (XAP_PrefsScheme *)m_vecSchemes.getNthItem(k);
	else
		return NULL;
}

XAP_PrefsScheme * XAP_Prefs::getScheme(const XML_Char * szSchemeName) const
{
	UT_uint32 kLimit = m_vecSchemes.getItemCount();
	UT_uint32 k;

	for (k=0; k<kLimit; k++)
	{
		XAP_PrefsScheme * p = getNthScheme(k);
		UT_ASSERT(p);
		if (UT_XML_stricmp(szSchemeName,p->getSchemeName()) == 0)
			return p;
	}

	return NULL;
}

UT_Bool XAP_Prefs::addScheme(XAP_PrefsScheme * pNewScheme)
{
	return (m_vecSchemes.addItem(pNewScheme) == 0);
}


XAP_PrefsScheme * XAP_Prefs::getCurrentScheme(void) const
{
	return m_currentScheme;
}

UT_Bool XAP_Prefs::setCurrentScheme(const XML_Char * szSchemeName)
{
	// set the current scheme.

	// TODO notify the application that the scheme has changed

	XAP_PrefsScheme * p = getScheme(szSchemeName);
	if (!p)
		return UT_FALSE;

	UT_DEBUGMSG(("Preferences::setCurrentScheme [%s].\n",szSchemeName));
	
	m_currentScheme = p;
	return UT_TRUE;
}

/*****************************************************************/

UT_Bool XAP_Prefs::getPrefsValue(const XML_Char * szKey, const XML_Char ** pszValue) const
{
	// a convenient routine to get a name/value pair from the current scheme

	UT_ASSERT(m_currentScheme);

	return m_currentScheme->getValue(szKey,pszValue);
}

/*****************************************************************
******************************************************************
** C-style callback functions that we register with the XML parser
******************************************************************
*****************************************************************/

static void startElement(void *userData, const XML_Char *name, const XML_Char **atts)
{
	XAP_Prefs * pPrefs = (XAP_Prefs *)userData;
	pPrefs->_startElement(name,atts);
}

static void endElement(void *userData, const XML_Char *name)
{
	XAP_Prefs * pPrefs = (XAP_Prefs *)userData;
	pPrefs->_endElement(name);
}

static void charData(void* userData, const XML_Char *s, int len)
{
	XAP_Prefs * pPrefs = (XAP_Prefs *)userData;
	pPrefs->_charData(s,len);
}
/*****************************************************************/

void XAP_Prefs::_startElement(const XML_Char *name, const XML_Char **atts)
{
	XAP_PrefsScheme * pNewScheme = NULL; // must be freed
	
	if (!m_parserState.m_parserStatus)		// eat if already had an error
		return;

	if (UT_XML_stricmp(name, "AbiPreferences") == 0)
	{
		m_parserState.m_bFoundAbiPreferences = UT_TRUE;

		// we expect something of the form:
		// <AbiPreferences app="AbiWord" ver="1.0">...</AbiPreferences>

		const XML_Char ** a = atts;
		while (*a)
		{
			UT_ASSERT(a[1] && *a[1]);	// require a value for each attribute keyword

			if (UT_XML_stricmp(a[0], "app") == 0)
			{
				const char * szThisApp = m_pApp->getApplicationName();
				UT_DEBUGMSG(("Found preferences for application [%s] (this is [%s]).\n",
							a[1],szThisApp));
				if (UT_XML_stricmp(a[1],szThisApp) != 0)
				{
					UT_DEBUGMSG(("Preferences file does not match this application.\n"));
					goto InvalidFileError;
				}
			}
			else if (UT_XML_stricmp(a[0], "ver") == 0)
			{
				// TODO test version number
			}

			a += 2;
		}
	}
	else if (UT_XML_stricmp(name, "Select") == 0)
	{
		m_parserState.m_bFoundSelect = UT_TRUE;
		
		// we expect something of the form:
		// <Select scheme="myScheme" autosave="1" />

		const XML_Char ** a = atts;
		while (*a)
		{
			UT_ASSERT(a[1] && *a[1]);	// require a value for each attribute keyword

			if (UT_XML_stricmp(a[0], "scheme") == 0)
			{
				DELETEP(m_parserState.m_szSelectedSchemeName);
				if (!UT_cloneString(m_parserState.m_szSelectedSchemeName,a[1]))
					goto MemoryError;
			}
			else if (UT_XML_stricmp(a[0], "autosave") == 0)
			{
				// m_bAutoSave controls whether we automatically
				// save any changes in the preferences during
				// interactive use/manipulation of the UI or if
				// we wait until the user explicitly does a save.
				// MSFT has this annoyning tendency to session-persist
				// almost everything in the UI -- we can do that,
				// but lets not make it the default....
				
				m_bAutoSave = (*a[1] == '1');
			}

			a += 2;
		}

		if (!m_parserState.m_szSelectedSchemeName)
		{
			UT_DEBUGMSG(("No scheme selected in <Select...>\n"));
			goto InvalidFileError;
		}
	}
	else if (UT_XML_stricmp(name, "Scheme") == 0)
	{
		// we found a preferences scheme.  we expect something of the form:
		// <Scheme name="myScheme" n0="v0" n1="v1" ... />
		// where the [nk,vk] are arbitrary name/value pairs that mean
		// something to the application.
		//
		// if there are duplicated name/value pairs our behavior is
		// undefined -- we remember the last one that the XML parser
		// give us.

		UT_Bool bIsNamed = UT_FALSE;
		
		pNewScheme = new XAP_PrefsScheme(NULL);
		if (!pNewScheme)
			goto MemoryError;
		
		const XML_Char ** a = atts;
		while (*a)
		{
			UT_ASSERT(a[1] && *a[1]);	// require a value for each attribute keyword

			if (UT_XML_stricmp(a[0], "name") == 0)
			{
				bIsNamed = UT_TRUE;
				
				const XML_Char * szBuiltinSchemeName = getBuiltinSchemeName();

				if (UT_XML_stricmp(a[1], szBuiltinSchemeName) == 0)
				{
					UT_DEBUGMSG(("Reserved scheme name [%s] found in file; ignoring.\n",a[1]));
					goto IgnoreThisScheme;
				}

				if (getScheme(a[1]))
				{
					UT_DEBUGMSG(("Duplicate scheme [%s]; ignoring latter instance.\n",a[1]));
					goto IgnoreThisScheme;
				}

				if (!pNewScheme->setSchemeName(a[1]))
					goto MemoryError;

				UT_DEBUGMSG(("Found Preferences scheme [%s].\n",a[1]));
			}
			else
			{
				if (!pNewScheme->setValue(a[0],a[1]))
					goto MemoryError;
			}

			a += 2;
		}

		if (!addScheme(pNewScheme))
			goto MemoryError;
		pNewScheme = NULL;				// we don't own it anymore
	}

	// successful parse of tag...
	
IgnoreThisScheme:
	DELETEP(pNewScheme);
	return;								// success

MemoryError:
	UT_DEBUGMSG(("Memory error parsing preferences file.\n"));
InvalidFileError:
	m_parserState.m_parserStatus = UT_FALSE;			// cause parser driver to bail
	DELETEP(pNewScheme);
	return;
}

void XAP_Prefs::_endElement(const XML_Char *name)
{
	// everything in this file is contained in start-tags
	return;
}

void XAP_Prefs::_charData(const XML_Char *s, int len)
{
	// everything in this file is contained in start-tags
	return;
}

/*****************************************************************/

UT_Bool XAP_Prefs::loadPrefsFile(void)
{
	UT_Bool bResult = UT_FALSE;			// assume failure
	const char * szFilename;
	FILE * fp = NULL;
	XML_Parser parser = NULL;
	int done = 0;
	char buf[4096];

	m_parserState.m_parserStatus = UT_TRUE;
	m_parserState.m_bFoundAbiPreferences = UT_FALSE;
	m_parserState.m_bFoundSelect = UT_FALSE;
	m_parserState.m_szSelectedSchemeName = NULL;

	szFilename = getPrefsPathname();
	if (!szFilename)
	{
		UT_DEBUGMSG(("could not get pathname for preferences file.\n"));
		goto Cleanup;
	}

	fp = fopen(szFilename, "r");
	if (!fp)
	{
		UT_DEBUGMSG(("could not open preferences file [%s].\n",szFilename));
		goto Cleanup;
	}
	
	parser = XML_ParserCreate(NULL);
	if (!parser)
	{
		UT_DEBUGMSG(("could not create parser for preferences file.\n"));
		goto Cleanup;
	}
	
	XML_SetUserData(parser, this);
	XML_SetElementHandler(parser, startElement, endElement);
	XML_SetCharacterDataHandler(parser, charData);

	while (!done)
	{
		size_t len = fread(buf, 1, sizeof(buf), fp);
		done = (len < sizeof(buf));

		if (!XML_Parse(parser, buf, len, done)) 
		{
			UT_DEBUGMSG(("%s at line %d\n",
						XML_ErrorString(XML_GetErrorCode(parser)),
						XML_GetCurrentLineNumber(parser)));
			goto Cleanup;
		}

		if (!m_parserState.m_parserStatus)
		{
			UT_DEBUGMSG(("Problem reading document\n"));
			goto Cleanup;
		}
	} 

	// we succeeded in parsing the file,
	// now check for higher-level consistency.

	if (!m_parserState.m_bFoundAbiPreferences)
	{
		UT_DEBUGMSG(("Did not find <AbiPreferences...>\n"));
		goto Cleanup;
	}
	if (!m_parserState.m_bFoundSelect)
	{
		UT_DEBUGMSG(("Did not find <Select...>\n"));
		goto Cleanup;
	}

	UT_ASSERT(m_parserState.m_szSelectedSchemeName);
	if (!setCurrentScheme(m_parserState.m_szSelectedSchemeName))
	{
		UT_DEBUGMSG(("Selected scheme [%s] not found in preferences file.\n",
					m_parserState.m_szSelectedSchemeName));
		goto Cleanup;
	}

	bResult = UT_TRUE;

Cleanup:
	DELETEP(m_parserState.m_szSelectedSchemeName);
	if (parser)
		XML_ParserFree(parser);
	if (fp)
		fclose(fp);
	return bResult;
}

UT_Bool XAP_Prefs::savePrefsFile(void)
{
	UT_Bool bResult = UT_FALSE;			// assume failure
	const char * szFilename;
	FILE * fp = NULL;

	szFilename = getPrefsPathname();
	if (!szFilename)
	{
		UT_DEBUGMSG(("could not get pathname for preferences file.\n"));
		goto Cleanup;
	}

	fp = fopen(szFilename, "w");
	if (!fp)
	{
		UT_DEBUGMSG(("could not open preferences file [%s].\n",szFilename));
		goto Cleanup;
	}

	// write a comment block as a prolog.
	// NOTE: this is human readable information only.
	
	fprintf(fp,"<!-- =====================================================================  -->\n");
	fprintf(fp,"<!-- This file contains AbiSuite Preferences.  AbiSuite is a suite of Open  -->\n");
	fprintf(fp,"<!-- Source desktop applications developed by AbiSource, Inc.  Information  -->\n");
	fprintf(fp,"<!-- about this application can be found at http://www.abisource.com        -->\n");
	fprintf(fp,"<!-- You should not edit this file by hand.                                 -->\n");
	fprintf(fp,"<!-- =====================================================================  -->\n");
	fprintf(fp,"\n");

	if (XAP_App::s_szBuild_ID && XAP_App::s_szBuild_ID[0])
	{
		fprintf(fp,"<!--         Build_ID          = ");
		fprintf(fp,XAP_App::s_szBuild_ID);
		fprintf(fp," -->\n");
	}
	if (XAP_App::s_szBuild_Version && XAP_App::s_szBuild_Version[0])
	{
		fprintf(fp,"<!--         Build_Version     = ");
		fprintf(fp,XAP_App::s_szBuild_Version);
		fprintf(fp," -->\n");
	}
	if (XAP_App::s_szBuild_Options && XAP_App::s_szBuild_Options[0])
	{
		fprintf(fp,"<!--         Build_Options     = ");
		fprintf(fp,XAP_App::s_szBuild_Options);
		fprintf(fp," -->\n");
	}
	if (XAP_App::s_szBuild_Target && XAP_App::s_szBuild_Target[0])
	{
		fprintf(fp,"<!--         Build_Target      = ");
		fprintf(fp,XAP_App::s_szBuild_Target);
		fprintf(fp," -->\n");
	}
	if (XAP_App::s_szBuild_CompileTime && XAP_App::s_szBuild_CompileTime[0])
	{
		fprintf(fp,"<!--         Build_CompileTime = ");
		fprintf(fp,XAP_App::s_szBuild_CompileTime);
		fprintf(fp," -->\n");
	}
	if (XAP_App::s_szBuild_CompileDate && XAP_App::s_szBuild_CompileDate[0])
	{
		fprintf(fp,"<!--         Build_CompileDate = ");
		fprintf(fp,XAP_App::s_szBuild_CompileDate);
		fprintf(fp," -->\n");
	}
	
	// end of prolog.
	// now we begin the actual document.

	fprintf(fp,"\n<AbiPreferences\n\t\tapp=\"%s\"\n\t\tver=\"%s\">\n",
			m_pApp->getApplicationName(),
			"1.0");
	{
		fprintf(fp,"\n\t<Select\n\t\tscheme=\"%s\"\n\t\tautosave=\"%ld\" />\n",
				m_currentScheme->getSchemeName(),
				(UT_uint32)m_bAutoSave);

		UT_uint32 kLimit = m_vecSchemes.getItemCount();
		UT_uint32 k;

		const XML_Char * szBuiltinSchemeName = getBuiltinSchemeName();

		for (k=0; k<kLimit; k++)
		{
			XAP_PrefsScheme * p = getNthScheme(k);
			UT_ASSERT(p);

			const XML_Char * szThisSchemeName = p->getSchemeName();

			if (UT_XML_stricmp(szThisSchemeName, szBuiltinSchemeName) == 0)
			{
				fprintf(fp,"\n\t<!-- Scheme %s contains the built-in application defaults.  It is         -->\n",
						szBuiltinSchemeName);
				fprintf(fp,"\t<!-- ignored on input.  It is only written here as a reference.  But then        -->\n");
				fprintf(fp,"\t<!-- as the comment above says, you shouldn't be editing this file anyway.  :-)  -->\n");
			}

			fprintf(fp,"\n\t<Scheme\n\t\tname=\"%s\"\n",szThisSchemeName);

			const XML_Char * szKey;
			const XML_Char * szValue;
			UT_uint32 j;
			for (j=0; (p->getNthValue(j,&szKey,&szValue)); j++)
				fprintf(fp,"\t\t%s=\"%s\"\n",szKey,szValue);
				
			fprintf(fp,"\t\t/>\n");
		}
	}

	fprintf(fp,"\n</AbiPreferences>\n");
	
Cleanup:
	if (fp)
		fclose(fp);
	return bResult;

}
