/* AbiSource Application Framework
 * Copyright (C) 2001 AbiSource, Inc.
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

#include <stdlib.h>
#include <string.h>

#include "ut_AbiObject.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

// uncomment this line to have all objects initialized to 0 (NULL)
//#define ZERO_SET_BTYES 1

/*!
 * AbiObject is a very generic class, meant to mirror 
 * GTK+'s GObject && java.lang.Object. It defines an object with
 * Some simple methods handling reference counting, memory (de)allocation
 * Hash Codes, and the like. Objects are created with an initial reference
 * Count of 1
 */
UT_AbiObject::UT_AbiObject () 
	: m_refs (1)
{
}

/*!
 * This should only get called when no references to the object exist
 */
UT_AbiObject::~UT_AbiObject ()
{
}

/*!
 * Increases the reference count of the object
 * /return the current #references
 */
UT_uint32 UT_AbiObject::ref ()
{
	UT_uint32 cpy = ++m_refs;
	return cpy;
}

/*!
 * Decreases the reference count of the object
 * If the reference count == 0, deletes the object
 * /return the #references still valid
 */
UT_uint32 UT_AbiObject::unref ()
{
	UT_uint32 cpy = --m_refs; // make a copy so that in case we delete 'this'
	if (!m_refs)
		delete this;

	return cpy;
}

/*!
 * /return the #references on the object
 */
UT_uint32 UT_AbiObject::count ()
{
	return m_refs;
}

/*!
 * "Sinks" the object's reference count - sets it to 0 but doesn't delete
 * the object so that you can claim ownership by referencing the object
 * or you can just outright delete the object afterwords
 */
void UT_AbiObject::sink ()
{
	m_refs = 0;
}

/*!
 * Simple equality test, returns pointer equality
 */
bool UT_AbiObject::equal (UT_AbiObject * other) const
{
	return (this == other);
}

/*!
 * Returns a hashcode for this object
 * Should be overridden by subclasses that care or can
 * Generate better hash codes
 */
UT_uint32 UT_AbiObject::hashcode () const
{
  // 9987001 is a reasnonably large prime
  long theAddr = (long)(this) * 0x9863b9;
  return (UT_uint32) (theAddr);
}

/****************************************************************************/
/* TODO: we may want a more aggressive memory allocation and de-allocation  */
/* TODO: scheme. Possibly use MemChunks or some global memory manager class */
/* TODO: consider creating a ut_new.[cpp|h] for this sort of thing          */
/****************************************************************************/

#ifdef MANAGE_MEMORY

/*!
 * Allocates memory for one of these objects
 */
/* static */ void * UT_AbiObject::operator new (size_t nbytes)
{
	UT_DEBUGMSG(("DOM: operator new allocating %d bytes\n", nbytes));
	UT_ASSERT (nbytes > 0);
	
	void * pBytes = malloc (nbytes);
	
#ifdef ZERO_SET_BYTES
	memset (pBytes, 0, nbytes);
#endif
	
	return pBytes;
}

/*!
 * Possibly deallocates memory for an UT_AbiObject
 * if count is 0 or 1, frees the memory
 * if count > 1, decrements the reference count and returns
 */
/* static */ void UT_AbiObject::operator delete (void * pbytes, size_t nbytes)
{
	UT_DEBUGMSG(("DOM: operator delete destroying %d bytes\n", nbytes));
	UT_ASSERT (pbytes && nbytes);
	
	UT_AbiObject * pObj = static_cast <UT_AbiObject *>(pbytes);
	if (pObj->m_refs > 1)
    {
		pObj->m_refs--; // just lower the reference count, as if they had just called unref()
		return;
    }
	
	free (pbytes);
}

#endif /* MANAGE_MEMORY */
