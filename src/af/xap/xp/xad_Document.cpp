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

#include <stdlib.h>
#include "ut_assert.h"
#include "xad_Document.h"
#include "ut_string.h"

AD_Document::AD_Document()
{
	m_iRefCount = 1;
	m_szFilename = NULL;

	// TODO do we need to auto-increase the bucket count,
   	// TODO if the ignore list gets long?
	m_pIgnoreList = new UT_AlphaHashTable(11);
}

AD_Document::~AD_Document()
{
	UT_ASSERT(m_iRefCount == 0);

   	// free all of the words on the list first
	for (int i = 0; i < m_pIgnoreList->getEntryCount(); i++) {
	   	UT_HashEntry * pHE = m_pIgnoreList->getNthEntry(i);
	   	FREEP(pHE->pData);
	}
   	// the free the ignore list
	DELETEP(m_pIgnoreList);

   	// NOTE: let subclass clean up m_szFilename, so it matches the alloc mechanism
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

UT_Bool AD_Document::appendIgnore(const UT_UCSChar * pszWord)
{
   UT_ASSERT(m_pIgnoreList);

   char * cword = (char*) calloc(UT_UCS_strlen(pszWord)+1, sizeof(char));
   UT_ASSERT(cword);
   UT_UCS_strcpy_to_char(cword, pszWord);
   
   UT_UCSChar * copy = NULL;
   UT_UCS_cloneString(&copy, pszWord);
   
   UT_sint32 iRes = m_pIgnoreList->addEntry(cword, NULL, (void*) copy);
   
   FREEP(cword);
   
   if (iRes == 0)
     return UT_TRUE;
   else
     return UT_FALSE;
}

UT_Bool AD_Document::isIgnore(UT_UCSChar * pszWord) const
{
   UT_ASSERT(m_pIgnoreList);

   char * cword = (char*) calloc(UT_UCS_strlen(pszWord)+1, sizeof(char));
   UT_ASSERT(cword);
   UT_UCS_strcpy_to_char(cword, pszWord);

   UT_HashEntry * pHE = m_pIgnoreList->findEntry(cword);
   
   FREEP(cword);
   
   if (pHE != NULL)
     return UT_TRUE;
   else 
     return UT_FALSE;
   
}

UT_Bool AD_Document::enumIgnores(UT_uint32 k, const UT_UCSChar * pszWord) const
{
   UT_ASSERT(m_pIgnoreList);

   if ((int)k >= m_pIgnoreList->getEntryCount())
     {
	pszWord = NULL;
	return UT_FALSE;
     }
   
   UT_HashEntry * pHE = m_pIgnoreList->getNthEntry(k);
   
   UT_ASSERT(pHE);
   
   pszWord = (UT_UCSChar*) pHE->pData;

   return UT_TRUE;
}

UT_Bool AD_Document::clearIgnores(void)
{
   UT_ASSERT(m_pIgnoreList);
   
   for (int i = 0; i < m_pIgnoreList->getEntryCount(); i++) {
      UT_HashEntry * pHE = m_pIgnoreList->getNthEntry(i);
      FREEP(pHE->pData);
   }
   
   DELETEP(m_pIgnoreList);
   
   m_pIgnoreList = new UT_AlphaHashTable(11);
   
   UT_ASSERT(m_pIgnoreList);
   
   return UT_TRUE;
}
