/* AbiWord
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


/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.
******************************************************************
*****************************************************************/

/*****************************************************************
******************************************************************
** FOR EACH ICON YOU ADD, ADD IT TO BOTH SECTIONS OF THIS FILE. **
******************************************************************
*****************************************************************/

#ifndef AP_TOOLBAR_ICONS_ALL_H

#	define AP_TOOLBAR_ICONS_ALL_H

	// Include each toolbar icon that we want to build.

#	include "ToolbarIcons/tb_text_center.xpm"
#	include "ToolbarIcons/tb_text_justify.xpm"
#	include "ToolbarIcons/tb_text_align_left.xpm"
#	include "ToolbarIcons/tb_text_align_right.xpm"
#	include "ToolbarIcons/tb_text_bold.xpm"
#	include "ToolbarIcons/tb_text_font.xpm"
#	include "ToolbarIcons/tb_text_italic.xpm"
#	include "ToolbarIcons/tb_text_strikeout.xpm"
#	include "ToolbarIcons/tb_text_underline.xpm"
#	include "ToolbarIcons/tb_para_0before.xpm"
#	include "ToolbarIcons/tb_para_12before.xpm"
#	include "ToolbarIcons/tb_copy.xpm"
#	include "ToolbarIcons/tb_cut.xpm"
#	include "ToolbarIcons/tb_edit.xpm"
#	include "ToolbarIcons/tb_new.xpm"
#	include "ToolbarIcons/tb_open.xpm"
#	include "ToolbarIcons/tb_paste.xpm"
#	include "ToolbarIcons/tb_print.xpm"
#	include "ToolbarIcons/tb_redo.xpm"
#	include "ToolbarIcons/tb_save.xpm"
#	include "ToolbarIcons/tb_save_as.xpm"
#	include "ToolbarIcons/tb_undo.xpm"
	
	// ... add new icons here (don't forget to add below the #else) ...

#else

	// Declare each toolbar icon that we are building.
	
	DefineToolbarIcon(tb_text_center_xpm)
	DefineToolbarIcon(tb_text_justify_xpm)
	DefineToolbarIcon(tb_text_align_left_xpm)
	DefineToolbarIcon(tb_text_align_right_xpm)
	DefineToolbarIcon(tb_text_bold_xpm)
	DefineToolbarIcon(tb_text_font_xpm)
	DefineToolbarIcon(tb_text_italic_xpm)
	DefineToolbarIcon(tb_text_strikeout_xpm)
	DefineToolbarIcon(tb_text_underline_xpm)
	DefineToolbarIcon(tb_para_0before_xpm)
	DefineToolbarIcon(tb_para_12before_xpm)
	DefineToolbarIcon(tb_copy_xpm)
	DefineToolbarIcon(tb_cut_xpm)
	DefineToolbarIcon(tb_edit_xpm)
	DefineToolbarIcon(tb_new_xpm)
	DefineToolbarIcon(tb_open_xpm)
	DefineToolbarIcon(tb_paste_xpm)
	DefineToolbarIcon(tb_print_xpm)
	DefineToolbarIcon(tb_redo_xpm)
	DefineToolbarIcon(tb_save_xpm)
	DefineToolbarIcon(tb_save_as_xpm)
	DefineToolbarIcon(tb_undo_xpm)

	// ... also add new icons here ...

#endif
