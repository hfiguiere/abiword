/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */

/* AbiSource Application Framework
 * Copyright (C) 1998,1999 AbiSource, Inc.
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

#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ut_hash.h"
#include "ut_vector.h"

#include "xad_Document.h"

#ifdef ENABLE_RESOURCE_MANAGER
#include "xap_ResourceManager.h"
#endif

AD_Document::AD_Document() :
#ifdef ENABLE_RESOURCE_MANAGER
	m_pResourceManager(new XAP_ResourceManager),
#else
	m_pResourceManager(0),
#endif
	m_iRefCount(1),
	m_szFilename(NULL),
	m_szEncodingName(""), // Should this have a default? UTF-8, perhaps?
	m_pIgnoreList(new UT_StringPtrMap(11)), // TODO do we need to auto-increase the bucket count, if the ignore list gets long?
	m_lastSavedTime(time(NULL))
{
	// 
}

AD_Document::~AD_Document()
{
	UT_ASSERT(m_iRefCount == 0);
	for (UT_uint32 i = 0; i < (unsigned)m_pIgnoreList->size(); i++)
	{
		// not careful results checking, etc, in this loop, but it's just for debugging anyhow
		const UT_UCSChar *word = 0;
		enumIgnores(i, &word);
	}

   	// free all of the words on the list first
	clearIgnores();
   	// then free the ignore list
	DELETEP(m_pIgnoreList);

   	// NOTE: let subclass clean up m_szFilename, so it matches the alloc mechanism

	// & finally...
#ifdef ENABLE_RESOURCE_MANAGER
	DELETEP(m_pResourceManager);
#endif
}

void AD_Document::ref(void)
{
	UT_ASSERT(m_iRefCount > 0);

	m_iRefCount++;
}

void AD_Document::unref(void)
{
	UT_ASSERT(m_iRefCount > 0);

	if (--m_iRefCount == 0)
	{
		delete this;
	}
}

const char * AD_Document::getFilename(void) const
{
	return m_szFilename;
}


// Methods for maintaining document-wide "Ignore All" list

bool AD_Document::appendIgnore(const UT_UCSChar * pWord, UT_uint32 len)
{
	UT_ASSERT(m_pIgnoreList);

	char _key[150], *key = _key;
	if (len > 145) key = new char[len + 1];
	UT_UCSChar *copy = new UT_UCSChar[len + 1];

	for (UT_uint32 i = 0; i < len; i++)
	{
		UT_UCSChar currentChar;
		currentChar = pWord[i];
		// convert smart quote apostrophe to ASCII single quote
		if (currentChar == UCS_RQUOTE) currentChar = '\'';
		key[i] = (char) currentChar;
		copy[i] = currentChar;
	}
	key[len] = 0;
	copy[len] = 0;

	if (!isIgnore(pWord, len))
	{
		// If it's already on the ignored word list, don't add it again.
		// This can happen if you are looking at a longish document.  You
		// "ignore all" a word, but spell-check doesn't get around to removing
		// the squiggles in the background for a while.  Then, you "ignore all"
		// that word (or another instance of it) again, and ka-bloom, the 
		// hash table stuff asserts on a duplicate entry.
		m_pIgnoreList->insert(key, 
				      (void *) copy);
	}

	if (key != _key) DELETEPV(key);

	return true;
}

bool AD_Document::isIgnore(const UT_UCSChar * pWord, UT_uint32 len) const
{
	UT_ASSERT(m_pIgnoreList);

	char _key[150], *key = _key;
	if (len > 145) key = new char[len + 1];

	for (UT_uint32 i = 0; i < len; i++)
	{
		UT_UCSChar currentChar;
		currentChar = pWord[i];
		// convert smart quote apostrophe to ASCII single quote
		if (currentChar == UCS_RQUOTE) currentChar = '\'';
		key[i] = (char) currentChar;
	}
	key[len] = 0;

	const void * pHE = m_pIgnoreList->pick(key);

	if (key != _key) DELETEPV(key);

	if (pHE != NULL)
		return true;
	else 
		return false;
   
}

bool AD_Document::enumIgnores(UT_uint32 k, const UT_UCSChar **pszWord) const
{
	UT_ASSERT(m_pIgnoreList);

	if (k >= m_pIgnoreList->size())
	{
		*pszWord = NULL;
		return false;
	}

	UT_Vector * v = m_pIgnoreList->enumerate();

	const void * pHE = v->getNthItem(k);	
	UT_ASSERT(pHE);
   
	*pszWord = (const UT_UCSChar*) pHE;

	DELETEP(v);

	return true;
}

bool AD_Document::clearIgnores(void)
{
	UT_ASSERT(m_pIgnoreList);

	UT_Vector * pVec = m_pIgnoreList->enumerate();
	UT_ASSERT(pVec);

	UT_uint32 size = pVec->size();

	for (UT_uint32 i = 0; i < size; i++)
	{
		UT_UCSChar * pData = (UT_UCSChar *)pVec->getNthItem(i);
		DELETEPV(pData);
	}

	DELETEP(pVec);
	DELETEP(m_pIgnoreList);
   
	m_pIgnoreList = new UT_StringPtrMap(11);
	UT_ASSERT(m_pIgnoreList);
   
	return true;
}

// Document-wide Encoding name used for some file formats (Text, RTF, HTML)

void AD_Document::setEncodingName(const char *szEncodingName)
{
	if (szEncodingName == NULL)
		szEncodingName = "";

	m_szEncodingName = szEncodingName;
}

const char * AD_Document::getEncodingName(void) const
{
	return m_szEncodingName.size() ? m_szEncodingName.c_str() : 0;
}
