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
#include "ev_Menu_Layouts.h"
#include "ut_assert.h"
#include "ut_string.h"

/*****************************************************************/

EV_Menu_LayoutItem::EV_Menu_LayoutItem(XAP_Menu_Id id, EV_Menu_LayoutFlags flags)
{
	m_id = id;
	m_flags = flags;
}

EV_Menu_LayoutItem::~EV_Menu_LayoutItem(void)
{
}

XAP_Menu_Id EV_Menu_LayoutItem::getMenuId(void) const
{
	return m_id;
}

EV_Menu_LayoutFlags EV_Menu_LayoutItem::getMenuLayoutFlags(void) const
{
	return m_flags;
}

/*****************************************************************/

EV_Menu_Layout::EV_Menu_Layout(const char * szName, UT_uint32 nrLayoutItems)
{
	UT_ASSERT(nrLayoutItems > 0);
	m_nrLayoutItems = nrLayoutItems;
	// TODO tis bad to call malloc/calloc from a constructor, since we cannot report failure.
	// TODO move this allocation to somewhere else.
	m_layoutTable = (EV_Menu_LayoutItem **)calloc(nrLayoutItems,sizeof(EV_Menu_LayoutItem *));
	UT_ASSERT(m_layoutTable);
	UT_cloneString(m_szName,szName);
}

EV_Menu_Layout::~EV_Menu_Layout(void)
{
	FREEP(m_szName);
	if (!m_layoutTable)
		return;
	for (UT_uint32 k=0; k<m_nrLayoutItems; k++)
		DELETEP(m_layoutTable[k]);
	free(m_layoutTable);
}

bool EV_Menu_Layout::setLayoutItem(UT_uint32 indexLayoutItem, XAP_Menu_Id id, EV_Menu_LayoutFlags flags)
{
	UT_ASSERT(indexLayoutItem < m_nrLayoutItems);
	DELETEP(m_layoutTable[indexLayoutItem]);
	m_layoutTable[indexLayoutItem] = new EV_Menu_LayoutItem(id,flags);
	return (m_layoutTable[indexLayoutItem] != NULL);
}

EV_Menu_LayoutItem * EV_Menu_Layout::getLayoutItem(UT_uint32 indexLayoutItem) const
{
	UT_ASSERT(indexLayoutItem < m_nrLayoutItems);
	return m_layoutTable[indexLayoutItem];
}

const char * EV_Menu_Layout::getName(void) const
{
	return m_szName;
}

UT_uint32 EV_Menu_Layout::getLayoutItemCount(void) const
{
	return m_nrLayoutItems;
}

