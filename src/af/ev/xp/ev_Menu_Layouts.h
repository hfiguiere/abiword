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
 


#ifndef EV_MENU_LAYOUTS_H
#define EV_MENU_LAYOUTS_H

#include "ut_types.h"
#include "xap_Types.h"


/*****************************************************************
******************************************************************
** This file defines the basis for defining a menu layout.  A
** menu layout describes the actual items and their ordering
** for a specific menu.  With this we can do things like have
** simple (novice) or complex (expert) menus for a window and/or
** have different menus for different types of windows (such as
** normal-view vs outline-view vs page-preview-view).
******************************************************************
*****************************************************************/

typedef enum _ev_Menu_LayoutFlags
{
	EV_MLF_Normal,
	EV_MLF_BeginSubMenu,
	EV_MLF_EndSubMenu,
	EV_MLF_BeginPopupMenu,
	EV_MLF_EndPopupMenu,
	EV_MLF_Separator

} EV_Menu_LayoutFlags;

/*****************************************************************/

class EV_Menu_LayoutItem
{
public:
	EV_Menu_LayoutItem(XAP_Menu_Id id, EV_Menu_LayoutFlags flags);
	~EV_Menu_LayoutItem(void);

	XAP_Menu_Id						getMenuId(void) const;
	EV_Menu_LayoutFlags				getMenuLayoutFlags(void) const;

protected:
	XAP_Menu_Id						m_id;
	EV_Menu_LayoutFlags				m_flags;
};

/*****************************************************************/

class EV_Menu_Layout					/* a glorified array with bounds checking */
{
public:
	EV_Menu_Layout(const char * szName, UT_uint32 nrLayoutItems);
	~EV_Menu_Layout(void);

	bool					setLayoutItem(UT_uint32 indexLayoutItem, XAP_Menu_Id id, EV_Menu_LayoutFlags flags);
	EV_Menu_LayoutItem *	getLayoutItem(UT_uint32 indexLayoutItem) const;
	const char *			getName(void) const;
	UT_uint32				getLayoutItemCount(void) const;

protected:
	char *					m_szName;			/* the name of our layout (like "MainMenu") */
	UT_uint32				m_nrLayoutItems;
	EV_Menu_LayoutItem **	m_layoutTable;
};

#endif /* EV_MENU_LAYOUTS_H */
