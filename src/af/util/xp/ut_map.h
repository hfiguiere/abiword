/* AbiWord
 * Copyright (C) 2001 Joaqu�n Cuenca Abela
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

#ifndef UT_MAP_H
#define UT_MAP_H

#include "ut_rbtree.h"
#include "ut_types.h"

class UT_Pair;

class ABI_EXPORT UT_Map
{
public:
	// Please note that UT_RBTree::key_t is (conceptually)
	// not the same as UT_Map::key_t.
	typedef const void* key_t;
	typedef const void* data_t;
	typedef const UT_Pair* value_t;
	typedef UT_RBTree::Iterator Iterator;
	typedef UT_RBTree::comparator comparator;

	// you should also note that until I come up with a wrapper
	// for comparator, UT_Map & UT_Set are exactly the same one
	// (but the default constructor).
	UT_Map();
	UT_Map(comparator comp);

	bool insert(value_t item) { return m_rbtree.insert(item); }
	void erase(Iterator& c) { m_rbtree.erase(c); }

	Iterator find(key_t item) { return m_rbtree.find(item); }
	Iterator find_if(key_t item, comparator pred) { return m_rbtree.find_if(item, pred); }

	Iterator begin() { return m_rbtree.begin(); }
	Iterator end() { return m_rbtree.end(); }

	size_t size() { return m_rbtree.size(); }

#ifdef DEBUG
	void print() const { m_rbtree.print(); }
	// This method should be const.  See UT_RBTree::checkInvariants comment
	bool checkInvariants() { return m_rbtree.checkInvariants(); }
#endif

private:
	UT_Map(const UT_Map&);
	UT_Map& operator= (const UT_Map&);

	UT_RBTree m_rbtree;
};

bool ut_map_lexico_lesser(UT_Map::key_t x, UT_Map::key_t y);
bool ut_map_lexico_greater(UT_Map::key_t x, UT_Map::key_t y);
bool ut_map_lexico_equal(UT_Map::key_t x, UT_Map::key_t y);

#endif // UT_MAP_H
