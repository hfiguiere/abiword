/* AbiWord
 * Copyright (C) 2002 Tomas Frydrych <tomas@frydrych.uklinux.net>
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

/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.
******************************************************************
*****************************************************************/

BeginLayout(ContextRevision,EV_EMC_REVISION)

	BeginPopupMenu()
		MenuItem(AP_MENU_ID_CONTEXT_REVISIONS_ACCEPT_REVISION)
		MenuItem(AP_MENU_ID_CONTEXT_REVISIONS_REJECT_REVISION)
		Separator()
		MenuItem(AP_MENU_ID_EDIT_CUT)
		MenuItem(AP_MENU_ID_EDIT_COPY)
		MenuItem(AP_MENU_ID_EDIT_PASTE)
	    MenuItem(AP_MENU_ID_EDIT_PASTE_SPECIAL)
		Separator()
     BeginSubMenu(AP_MENU_ID_TABLE)
		MenuItem(AP_MENU_ID_TABLE_INSERT_TABLE)
		MenuItem(AP_MENU_ID_TABLE_MERGE_CELLS)
		MenuItem(AP_MENU_ID_TABLE_SPLIT_CELLS)
		Separator()
		MenuItem(AP_MENU_ID_TABLE_FORMAT)
		MenuItem(AP_MENU_ID_TABLE_AUTOFIT)
     EndSubMenu()
		Separator()
		MenuItem(AP_MENU_ID_FMT_FONT)
	    MenuItem(AP_MENU_ID_FMT_LANGUAGE)
		MenuItem(AP_MENU_ID_FMT_PARAGRAPH)
	    MenuItem(AP_MENU_ID_FMT_BULLETS)
	EndPopupMenu()

EndLayout()

