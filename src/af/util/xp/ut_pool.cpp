/* AbiSource Program Utilities
 * Copyright (C) 1998 AbiSource, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */ 
 


#include <stdlib.h>
#include <string.h>

#include "ut_pool.h"

#define UT_POOL_BUCKET_SIZE		1024

UT_StringPool::UT_StringPool()
{
	m_pFirstBucket = NULL;
	addBucket();
}

UT_StringPool::~UT_StringPool()
{
	while (m_pFirstBucket)
	{
		UT_PoolBucket* pTmp = m_pFirstBucket->pNext;
		delete m_pFirstBucket;
		m_pFirstBucket = pTmp;
	}
}


UT_StringPool::UT_PoolBucket::UT_PoolBucket(int iSize)
{
	// TODO how do we handle constructor failure here?
	pChars = new char[iSize];
	iSpace = iSize;
	iCurLen = 0;
	pNext = NULL;
}

UT_StringPool::UT_PoolBucket::~UT_PoolBucket()
{
	delete pChars;
}

int UT_StringPool::addBucket()
{
	UT_PoolBucket* pBuck = new UT_PoolBucket(1024);
	pBuck->pNext = m_pFirstBucket;
	m_pFirstBucket = pBuck;

	return 0; // TODO return code
}

char* UT_StringPool::addString(const char* p)
{
	int len = strlen(p);

	if ((m_pFirstBucket->iCurLen + len + 1) > m_pFirstBucket->iSpace)
	{
		if (0 != addBucket())
		{
			return NULL; // out of mem
		}
	}

	char*	pResult = m_pFirstBucket->pChars + m_pFirstBucket->iCurLen;

	for (int i=0; i<=len; i++)		// does the zero terminator too
	{
		m_pFirstBucket->pChars[m_pFirstBucket->iCurLen++] = p[i];
	}

	return pResult;
}

