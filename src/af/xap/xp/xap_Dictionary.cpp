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
#include <string.h>
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ut_growbuf.h"
#include "ut_hash.h"
#include "ut_types.h"

#include "xap_Dictionary.h"
#include "ut_string_class.h"

/*****************************************************************/
/*****************************************************************/

/*
	Dictionary is an alphabetic list of words, one per line.  
	Import/export as a plain text file formatted in UTF8.
	We allow either LF or CR or CRLF line termination.

	(This code shamelessly cribbed from the impexp for UTF8.)
*/

/*****************************************************************/
/*****************************************************************/

XAP_Dictionary::XAP_Dictionary(const char * szFilename)
	: m_hashWords(29)
{
	UT_ASSERT(szFilename && *szFilename);
	UT_cloneString((char *&)m_szFilename, szFilename);

	m_fp = 0;
	m_bDirty = false;
}

XAP_Dictionary::~XAP_Dictionary()
{
	if (m_fp)
		_closeFile();

	FREEP(m_szFilename);

  	UT_HASH_PURGEDATA(UT_UCSChar *, (&m_hashWords), free);
}

const char * XAP_Dictionary::getShortName(void) const
{
	// TODO: get just the filename (no path), for use in UI
	return NULL;
}

/*****************************************************************/
/*****************************************************************/

bool XAP_Dictionary::_openFile(const char * mode)
{
	UT_ASSERT(!m_fp);

	// TODO add code to make a backup of the original file, if it exists.
	
	m_fp = fopen(m_szFilename,mode);
	return (m_fp != 0);
}

UT_uint32 XAP_Dictionary::_writeBytes(const UT_Byte * pBytes, UT_uint32 length)
{
	UT_ASSERT(m_fp);
	UT_ASSERT(pBytes);
	UT_ASSERT(length);
	
	return fwrite(pBytes,sizeof(UT_Byte),length,m_fp);
}

bool XAP_Dictionary::_writeBytes(const UT_Byte * sz)
{
	UT_ASSERT(m_fp);
	UT_ASSERT(sz);
	int length = strlen((const char *)sz);
	UT_ASSERT(length);
	
	return (_writeBytes(sz,length)==(UT_uint32)length);
}

bool XAP_Dictionary::_closeFile(void)
{
	if (m_fp)
		fclose(m_fp);
	m_fp = 0;
	return true;
}

void XAP_Dictionary::_abortFile(void)
{
	// abort the write.
	// TODO close the file and do any restore and/or cleanup.

	_closeFile();
	return;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

bool XAP_Dictionary::load(void)
{
	UT_ASSERT(m_hashWords.size() == 0);

	if (!_openFile("r"))
		return false;

	if (!_parseUTF8())
		_abortFile();
	else
		_closeFile();

	m_bDirty = false;
//
// Hardwire in some words that should be in the English Language :-)
//
	addWord("AbiWord");
	addWord("Abiword");
	addWord("abiword");
	addWord("abisource");
	return true;
}

static void _smashUTF8(UT_GrowBuf * pgb)
{
	// smash any utf8 sequences into a single ucs char

	// since we change the GrowBuf in-place, we
	// recompute the length and buffer pointer
	// to avoid accidents....

	for (UT_uint32 k=0; (k < pgb->getLength()); k++)
	{
		UT_uint16 * p = pgb->getPointer(k);
		UT_uint16 ck = *p;
		
		if (ck < 0x0080)						// latin-1
			continue;
		else if ((ck & 0x00f0) == 0x00f0)		// lead byte in 4-byte surrogate pair, ik
		{
			UT_ASSERT(UT_NOT_IMPLEMENTED);
			continue;
		}
		else if ((ck & 0x00e0) == 0x00e0)		// lead byte in 3-byte sequence
		{
			UT_ASSERT(k+2 < pgb->getLength());
			XML_Char buf[4];
			buf[0] = (XML_Char)p[0];
			buf[1] = (XML_Char)p[1];
			buf[2] = (XML_Char)p[2];
			buf[3] = 0;
			UT_UCSChar ucs = UT_decodeUTF8char(buf,3);
			pgb->overwrite(k,&ucs,1);
			pgb->del(k+1,2);
			continue;
		}
		else if ((ck & 0x00c0) == 0x00c0)		// lead byte in 2-byte sequence
		{
			UT_ASSERT(k+1 < pgb->getLength());
			XML_Char buf[3];
			buf[0] = (XML_Char)p[0];
			buf[1] = (XML_Char)p[1];
			buf[2] = 0;
			UT_UCSChar ucs = UT_decodeUTF8char(buf,2);
			pgb->overwrite(k,&ucs,1);
			pgb->del(k+1,1);
			continue;
		}
		else // ((ck & 0x0080) == 0x0080)		// trailing byte in multi-byte sequence
		{
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			// let it remain as is...
			continue;
		}
	}
}

#define X_ReturnIfFail(exp)		do { bool b = (exp); if (!b) return (false); } while (0)

bool XAP_Dictionary::_parseUTF8(void)
{
	UT_GrowBuf gbBlock(1024);
	bool bEatLF = false;
	bool bSmashUTF8 = false;
	unsigned char c;

	while (fread(&c, 1, sizeof(c), m_fp) > 0)
	{
		switch (c)
		{
		case '\r':
		case '\n':
			if ((c == '\n') && bEatLF)
			{
				bEatLF = false;
				break;
			}

			if (c == '\r')
			{
				bEatLF = true;
			}
			
			// we interprete either CRLF, CR, or LF as a word delimiter.
			
			if (gbBlock.getLength() > 0)
			{
				if (bSmashUTF8)
					_smashUTF8(&gbBlock);

				X_ReturnIfFail(addWord(gbBlock.getPointer(0), gbBlock.getLength()));
				gbBlock.truncate(0);
				bSmashUTF8 = false;
			}
			break;

		default:
			bEatLF = false;

			// deal with plain character.  to simplify parsing logic,
			// we just stuff all text chars (latin-1 and utf8 escape
			// sequences) into the GrowBuf and will smash the utf8
			// sequences into unicode in a moment.

			if (c > 0x7f)
				bSmashUTF8 = true;
			UT_UCSChar uc = (UT_UCSChar) c;
			X_ReturnIfFail(gbBlock.ins(gbBlock.getLength(),&uc,1));
			break;
		}
	} 

	if (gbBlock.getLength() > 0)
	{
		// if we have text left over (without final CR/LF), emit it now
		if (bSmashUTF8)
			_smashUTF8(&gbBlock);

		X_ReturnIfFail(addWord(gbBlock.getPointer(0), gbBlock.getLength()));
	}

	return true;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

bool XAP_Dictionary::save(void)
{
	if (!m_bDirty)
		return true;

	if (!_openFile("w"))
		return false;

	UT_Vector * pVec = m_hashWords.enumerate();
	UT_ASSERT(pVec);

	UT_uint32 size = pVec->size();

	for (UT_uint32 i = 0; i < size; i++)
	{
		UT_UCSChar * pWord = (UT_UCSChar *) pVec->getNthItem(i);
		_outputUTF8(pWord, UT_UCS_strlen(pWord));
		_writeBytes((UT_Byte *)"\n");
	}

	_closeFile();

	delete pVec;
	m_bDirty = false;

	return true;
}

void XAP_Dictionary::_outputUTF8(const UT_UCSChar * data, UT_uint32 length)
{
	UT_String buf;
	const UT_UCSChar * pData;

	for (pData = data; (pData<data+length); /**/)
	{
		if (*pData > 0x007f)
		{
			const XML_Char * s = UT_encodeUTF8char(*pData++);
			while (*s)
				buf += (char)*s++;
		}
		else
		{
			buf += (char)*pData++;
		}
	}

	_writeBytes((UT_Byte *)buf.c_str(),buf.size());
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

bool XAP_Dictionary::addWord(const UT_UCSChar * pWord, UT_uint32 len)
{
	char * key = (char *) calloc(len+1, sizeof(char));
	UT_UCSChar * copy = (UT_UCSChar *) calloc(len+1, sizeof(UT_UCSChar));
    if (!key || !copy)
	{
		UT_DEBUGMSG(("mem failure adding word to dictionary\n"));
		FREEP(key);
		FREEP(copy);
		return false;
	}
	UT_uint32 i = 0;
	for (i = 0; i < len; i++)
	{
		UT_UCSChar currentChar;
		currentChar = pWord[i];
		// map smart quote apostrophe to ASCII right single quote
		if (currentChar == UCS_RQUOTE) currentChar = '\'';
		key[i] =  (char) static_cast<unsigned char>(currentChar);
		copy[i] = currentChar;
		xxx_UT_DEBUGMSG(("addWord: key[%d] = %c %d \n",i,key[i],key[i]));
		if(key[i] == 0)
		{
			break;
		}
	}
	key[i] = 0;
//
// Get exactly the same length.
//
	char * key2 = UT_strdup(key);
	copy[i] = 0;
#if 0
//
// Useful debugging code
//
	char * ucs_dup = (char *) calloc(2*len+1, sizeof(char));
	UT_UCS_strcpy_to_char( ucs_dup, copy);
	UT_DEBUGMSG(("Inserting word %s with key %s into hash \n",ucs_dup,key));
	FREEP(ucs_dup);

#endif
	m_hashWords.insert(key2, 
			   (void *) copy);

	FREEP(key);
	FREEP(key2);

	// TODO: is this right?
	m_bDirty = true;
	return true;
}

bool XAP_Dictionary::addWord(const char * word)
{
	UT_sint32 len = strlen(word);
	if(len <=0)
	{
		return false;
	}
	UT_UCSChar * ucs_dup = (UT_UCSChar *) calloc(len+1, sizeof(UT_UCSChar));
	UT_UCS_strcpy_char(ucs_dup, word);
	addWord(ucs_dup,len);
	FREEP(ucs_dup);
	return true;
}

bool XAP_Dictionary::isWord(const UT_UCSChar * pWord, UT_uint32 len) const
{
//
// This original code does not work. I believe it is a bug in the hash table.
//
#if 0
	char * key = (char*) calloc(len+1, sizeof(char));
	if (!key)
	{
		UT_DEBUGMSG(("mem failure looking up word in dictionary\n"));
		FREEP(key);
		return false;
	}
	UT_uint32 i =0;
	for (i = 0; i < len; i++)
	{
		key[i] =  (char) static_cast<unsigned char>( pWord[i]);
		xxx_UT_DEBUGMSG(("isword key[%d] = %c %d \n",i,key[i],key[i]));
		if(key[i] == 0)
			break;
	}
	key[i] = 0;
	char * key2 = UT_strdup(key);
	bool contains = m_hashWords.contains (key2, NULL);
	FREEP(key);
	FREEP(key2);
	return contains;
#else
//
// This code is a work around for the buggy "contains" method used above.
//
	char * key = (char*) calloc(len+1, sizeof(char));
	if (!key)
	{
		UT_DEBUGMSG(("mem failure looking up word in dictionary\n"));
		FREEP(key);
		return false;
	}
	UT_uint32 i =0;
	for (i = 0; i < len; i++)
	{
		key[i] =  (char) static_cast<unsigned char>( pWord[i]);
		xxx_UT_DEBUGMSG(("isword key[%d] = %c %d \n",i,key[i],key[i]));
		if(key[i] == 0)
			break;
	}
	key[i] = 0;
	char * key2 = UT_strdup(key);

	UT_Vector * pVec = m_hashWords.enumerate();
	UT_ASSERT(pVec);

	UT_uint32 size = pVec->size();
	bool bFound = false;
	for (i = 0; (i < size) && !bFound; i++)
	{
		UT_UCSChar * pWord = (UT_UCSChar *) pVec->getNthItem(i);
		UT_uint32 lenstr = UT_UCS_strlen(pWord);
		char * szV = (char *)  calloc(lenstr+1, sizeof(char));
		UT_UCS_strcpy_to_char(szV,pWord);
		xxx_UT_DEBUGMSG(("isWord: Comparing key %s to entry %s \n",key2,szV));
		bFound = (strcmp(szV,key2) == 0); 
		FREEP(szV);
	}
	FREEP(key);
	FREEP(key2);
	delete pVec;
	return bFound;
#endif
}

