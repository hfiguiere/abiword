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
 


#include <stdlib.h>
#include <string.h>

#include "ut_types.h"
#include "ut_vector.h"
#include "ut_assert.h"

#ifndef ABI_OPT_STL

UT_Vector::UT_Vector(UT_uint32 sizehint)
  : m_pEntries(NULL), m_iCount(0), m_iSpace(0),
    m_iCutoffDouble(sizehint), m_iPostCutoffIncrement(32)
{
}

UT_Vector::UT_Vector(const UT_Vector& utv)
{
	m_iCutoffDouble = utv.m_iCutoffDouble;
	m_iPostCutoffIncrement = utv.m_iPostCutoffIncrement;
	copy(&utv);
}

UT_Vector& UT_Vector::operator=(const UT_Vector& utv)
{
	if(this != &utv)
	{
		m_iCutoffDouble = utv.m_iCutoffDouble;
		m_iPostCutoffIncrement = utv.m_iPostCutoffIncrement;
		copy(&utv);
	}
	return *this;
}

void UT_Vector::clear()
{
	m_iCount = 0;
	m_iSpace = 0;
	FREEP(m_pEntries);
	m_pEntries = NULL;
}

UT_Vector::~UT_Vector()
{
	FREEP(m_pEntries);
}

/*
 This function is called everytime we want to insert a new element but don't have
 enough space.  In this case we grow the array to be _at least_ ndx size.
*/
UT_sint32 UT_Vector::grow(UT_uint32 ndx)
{
	UT_uint32 new_iSpace;
	if(!m_iSpace) {
		new_iSpace = m_iPostCutoffIncrement;
	}
	else if (m_iSpace < m_iCutoffDouble) {
		new_iSpace = m_iSpace * 2;
	}
	else {
		new_iSpace = m_iSpace + m_iPostCutoffIncrement;
	}

	if (new_iSpace < ndx)
	{
		new_iSpace = ndx;
	}

	void ** new_pEntries = (void **)realloc(m_pEntries, new_iSpace * sizeof(void *));
	if (!new_pEntries) {
		return -1;
	}
	//Is this required? We always check Count first anyways.
	// TMN: Unfortunately it is, since the class GR_CharWidths
	// uses UT_Vector as a sparse array!
	memset(&new_pEntries[m_iSpace], 0, (new_iSpace - m_iSpace) * sizeof(void *));
	m_iSpace = new_iSpace;
	m_pEntries = new_pEntries;

	return 0;
}

UT_sint32 UT_Vector::insertItemAt(void* p, UT_uint32 ndx)
{
	if (ndx > m_iCount + 1)
		return -1;
	
	if ((m_iCount+1) > m_iSpace)
	{
		UT_sint32 err = grow(0);
		if (err)
		{
			return err;
		}
	}

	// bump the elements -> thataway up to the ndxth position
	memmove(&m_pEntries[ndx+1], &m_pEntries[ndx], (m_iCount - ndx) * sizeof(void *)); 

	m_pEntries[ndx] = p;
	++m_iCount;

	return 0;
}

UT_sint32 UT_Vector::addItem(void* p, UT_uint32 * pIndex)
{
	UT_sint32 err = addItem(p);
	if (!err && pIndex)
		*pIndex = m_iCount-1;
	return err;
}

UT_sint32 UT_Vector::addItem(void* p)
{
	if ((m_iCount+1) > m_iSpace)
	{
		UT_sint32 err = grow(0);
		if (err)
		{
			return err;
		}
	}

	m_pEntries[m_iCount++] = p;

	return 0;
}

/** It returns true if there were no errors, false elsewhere */
bool UT_Vector::pop_back()
{
	if (m_iCount > 0)
	{
		--m_iCount;
		return true;
	}
	else
		return false;
}

UT_sint32 UT_Vector::setNthItem(UT_uint32 ndx, void * pNew, void ** ppOld)
{
	const UT_uint32 old_iSpace = m_iSpace;

	// skip realloc in cases where we are removing an entry, as its probably an error
	UT_return_val_if_fail(!((ndx >= m_iSpace) && (pNew == NULL) && (ppOld == NULL)), -1)

	if (ndx >= m_iSpace)
	{
		const UT_sint32 err = grow(ndx+1);
		if (err)
		{
			return err;
		}
	}

	if (ppOld)
	{
		*ppOld = (ndx < old_iSpace) ? m_pEntries[ndx] : 0;
	}
	
	m_pEntries[ndx] = pNew;
	if (ndx >= m_iCount)
	{
		m_iCount = ndx + 1;
	}
	
	return 0;
}

void* UT_Vector::getLastItem() const
{
	UT_ASSERT(m_iCount > 0);

	return m_pEntries[m_iCount-1];
}

void* UT_Vector::getFirstItem() const
{
	UT_ASSERT(m_iCount > 0);
	UT_ASSERT(m_pEntries);

	return m_pEntries[0];
}

void UT_Vector::deleteNthItem(UT_uint32 n)
{
	UT_ASSERT(n < m_iCount);
	UT_ASSERT(m_iCount > 0);

	memmove(&m_pEntries[n], &m_pEntries[n+1], (m_iCount - (n + 1)) * sizeof(void*));
	
	m_pEntries[m_iCount-1] = 0;
	m_iCount--;

	return;
}

UT_sint32 UT_Vector::findItem(void* p) const
{
	for (UT_uint32 i=0; i<m_iCount; i++)
	{
		if (m_pEntries[i] == p)
		{
			return (UT_sint32) i;
		}
	}

	return -1;
}

void UT_Vector::qsort(int (*compar)(const void *, const void *))
{
	::qsort(m_pEntries, m_iCount, sizeof(void*), compar);
}

bool UT_Vector::copy(const UT_Vector *pVec)
{
	clear();

	for (UT_uint32 i=0; i < pVec->m_iCount; i++)
	{
		UT_sint32 err;

		err = addItem(pVec->m_pEntries[i]);
		if(err == -1)
			return (err ? true : false);
	}

	return 0;
}

const void* UT_Vector::operator[](UT_uint32 i) const
{
	return this->getNthItem(i);
}

#else /* ABI_OPT_STL */

UT_Vector::UT_Vector(UT_uint32 sizehint)
{
	//Ignore the sizehint
}

void UT_Vector::clear()
{
	m_STLVec.clear();
}

UT_Vector::~UT_Vector()
{
}

UT_uint32 UT_Vector::getItemCount() const
{
	return m_STLVec.size();
}

UT_sint32 UT_Vector::insertItemAt(void* p, UT_uint32 ndx)
{
	m_STLVec.insert(m_STLVec.begin()+ndx, p);
	return 0;
}

UT_sint32 UT_Vector::addItem(void* p, UT_uint32 * pIndex)
{
	UT_sint32 err = addItem(p);
	if (!err && pIndex)
		*pIndex = m_STLVec.size()-1;
	return err;
}

UT_sint32 UT_Vector::addItem(void* p)
{
	m_STLVec.push_back(p);

	return 0;
}

void* UT_Vector::getNthItem(UT_uint32 n) const
{
	return m_STLVec[n];
}

UT_sint32 UT_Vector::setNthItem(UT_uint32 ndx, void * pNew, void ** ppOld)
{
	if (ppOld)
	{
		*ppOld = m_STLVec[ndx];
	}
	
	if (m_STLVec.size() <= ndx)
		m_STLVec.resize (ndx+1);
	m_STLVec[ndx] = pNew;
		
	return 0;
}

void* UT_Vector::getLastItem() const
{
	return (void*)m_STLVec.back();
}

void* UT_Vector::getFirstItem() const
{
	return (void*)m_STLVec.front();
}

void UT_Vector::deleteNthItem(UT_uint32 n)
{
	m_STLVec.erase(m_STLVec.begin()+n);
}

UT_sint32 UT_Vector::findItem(void* p) const
{
	for (UT_uint32 i=0; i<m_STLVec.size(); i++)
	{
		if (m_STLVec[i] == p)
		{
			return (UT_sint32) i;
		}
	}

	return -1;
}

void UT_Vector::qsort(int (*compar)(const void *, const void *))
{
	sort(m_STLVec.begin(), m_STLVec.end(), compar);
}

bool UT_Vector::copy(UT_Vector *pVec)
{
	m_STLVec = pVec->m_STLVec;
	return 0;
}

const void* UT_Vector::operator[](UT_uint32 i) const
{
	return m_STLVec[i];
}

#endif /* ABI_OPT_STL */
