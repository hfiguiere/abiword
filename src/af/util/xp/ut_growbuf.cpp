/* AbiSource Program Utilities
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
 


#include <malloc.h>
#include <memory.h>
#include <string.h>

#include "ut_assert.h"
#include "ut_types.h"
#include "ut_growbuf.h"

#define DEFAULT_CHUNK		1024*10
#define MIN_CHUNK			256

UT_GrowBuf::UT_GrowBuf(UT_uint32 iChunk)
{
	if (iChunk < MIN_CHUNK)
		iChunk = DEFAULT_CHUNK;
	m_iChunk = iChunk;

	// since constructors cannot report failure, we defer
	// the malloc until the first use.

	m_pBuf = 0;
	m_iSize = 0;
	m_iSpace = 0;
}

UT_GrowBuf::~UT_GrowBuf()
{
	if (m_pBuf)
		free(m_pBuf);
}

UT_Bool UT_GrowBuf::_growBuf(UT_uint32 spaceNeeded)
{
	// expand the buffer if necessary to accomidate the requested space.
	// round up to the next multiple of the chunk size.
	
	UT_uint32 newSize = ((m_iSpace+spaceNeeded+m_iChunk-1)/m_iChunk)*m_iChunk;
	UT_uint16 * pNew = (UT_uint16 *)calloc(newSize,sizeof(*m_pBuf));
	if (!pNew)
		return UT_FALSE;
	
	if (m_pBuf)
	{
		memmove(pNew,m_pBuf,m_iSize*sizeof(*m_pBuf));
		free(m_pBuf);
	}

	m_pBuf = pNew;
	m_iSpace = newSize;

	return UT_TRUE;
}
		
UT_Bool UT_GrowBuf::ins(UT_uint32 position, const UT_uint16 * pValue, UT_uint32 length)
{
	// insert the given buffer into the growbuf

	if (!length)
	{
		UT_ASSERT(!pValue);
		return UT_TRUE;
	}
	
	UT_ASSERT(pValue);
	
	if (m_iSpace-m_iSize < length)
		if (!_growBuf(length))
			return UT_FALSE;

	if (m_iSize > position)
		memmove(m_pBuf+position+length,m_pBuf+position,(m_iSize-position)*sizeof(*m_pBuf));
	m_iSize += length;
	memmove(m_pBuf+position,pValue,length*sizeof(*m_pBuf));

	return UT_TRUE;
}

UT_Bool UT_GrowBuf::ins(UT_uint32 position, UT_uint32 length)
{
	// insert zeroed space into the growbuf

	if (!length)
		return UT_TRUE;
	
	if (m_iSpace-m_iSize < length)
		if (!_growBuf(length))
			return UT_FALSE;

	if (m_iSize > position)
		memmove(m_pBuf+position+length,m_pBuf+position,(m_iSize-position)*sizeof(*m_pBuf));
	m_iSize += length;
	memset(m_pBuf+position,0,length*sizeof(*m_pBuf));

	return UT_TRUE;
}

UT_Bool UT_GrowBuf::del(UT_uint32 position, UT_uint32 amount)
{
	if (!amount)
		return UT_TRUE;

	if (!m_pBuf)
		return UT_FALSE;
	UT_ASSERT(position < m_iSize);
	UT_ASSERT(position+amount <= m_iSize);
	
	memmove(m_pBuf+position,m_pBuf+position+amount,(m_iSize-position-amount)*sizeof(*m_pBuf));
	m_iSize -= amount;

	// TODO consider adding some stuff to realloc-down if we cross a good-sized threshold.
	
	return UT_TRUE;
}

UT_uint32 UT_GrowBuf::getLength(void) const
{
	// return the number of items in the buffer

	return m_iSize;
}

UT_uint16 * UT_GrowBuf::getPointer(UT_uint32 position) const
{
	// return a read-only pointer to the buffer
	
	if (!m_pBuf || !m_iSize)
		return 0;
	UT_ASSERT(position < m_iSize);
	return m_pBuf+position;
}

UT_Bool UT_GrowBuf::overwrite(UT_uint32 position, UT_uint16 * pValue, UT_uint32 length)
{
	// overwrite the current cells at the given position for the given length.

	if (!length)
	{
		UT_ASSERT(!pValue);
		return UT_TRUE;
	}
	
	UT_ASSERT(pValue);

	if (m_iSpace < position+length)
		if (!_growBuf(position+length-m_iSpace))
			return UT_FALSE;

	memmove(m_pBuf+position,pValue,length*sizeof(*m_pBuf));
	return UT_TRUE;
}

void UT_GrowBuf::truncate(UT_uint32 position)
{
	if (position < m_iSize)
		m_iSize = position;

	// TODO consider reallocing down
}

