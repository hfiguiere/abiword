/* AbiSuite
 * Copyright (C) 2001 Dom Lachowicz <cinamod@hotmail.com> 
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

#ifndef SPELL_MANAGER_H
#define SPELL_MANAGER_H

#include "ut_types.h"
#include "ut_string.h"
#include "ut_vector.h"
#include "ut_hash.h"

// forward declaration
class SpellManager;

class SpellChecker
{
	friend class SpellManager;
	
public:
	
	enum SpellCheckResult
	{
		LOOKUP_SUCCEEDED = 0, // looking up the word succeeded
		LOOKUP_FAILED = 1,    // could not find the word
		LOOKUP_ERROR = 2      // internal error
	};
	
	virtual SpellCheckResult	checkWord(const UT_UCSChar* word, size_t len) = 0;
	virtual UT_Vector*			suggestWord(const UT_UCSChar* word, size_t len) = 0;
	
protected:
	virtual bool requestDictionary (const char * szLang) = 0;
	
protected:
    SpellChecker();
    virtual ~SpellChecker();

private:
	SpellChecker(const SpellChecker&);		// no impl
	void operator=(const SpellChecker&);	// no impl
};


class SpellManager
{
public:	
	static SpellManager & instance (void);
	
	virtual ~SpellManager ();
	
	virtual SpellChecker * requestDictionary (const char * szLang);
	virtual SpellChecker * lastDictionary (void) const;

	UT_uint32 numLoadedDicts () const { return m_nLoadedDicts; }

private:
	SpellManager ();
	SpellManager ( const SpellManager & other ) ;
	SpellManager & operator= ( const SpellManager & other ) ;

	UT_StringPtrMap m_map;
	UT_String m_missingHashs;
	SpellChecker *  m_lastDict;
	UT_uint32 m_nLoadedDicts;
};

#endif /* SPELL_MANAGER_H */
