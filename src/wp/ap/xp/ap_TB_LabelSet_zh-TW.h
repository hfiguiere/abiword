/* AbiWord
 * Copyright (C) 1998-2000 AbiSource, Inc.
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

/*
 * Traditional Chinese Translation by
 *
 *  Chinese Linux Extensions Project  http://cle.linux.org.tw/
 *     Chih-Wei Huang <cwhuang@linux.org.tw>
 *     Armani         <armani@cle.linux.org.tw>
 *
*/

/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.
******************************************************************
*****************************************************************/

// Note: if the tooltip is blank, the status message will be used as the
// Note: tooltip.  therefore, we probably don't need most tooltip strings
// Note: here -- unless the status message is too long to look good in
// Note: a tooltip.

// Note: the icon field should not be localized unless absolutely necessary.
// Note: the icon name here is to a specific icon (pixmap or bitmap or whatever)
// Note: that will always be in the application.  if, for example, a big fat 'B'
// Note: for BOLD doesn't make sense in another language, change the entry in
// Note: the localization and add the icon to whatever table.

// Note: if a tool item does not use an icon (like a combo box), use the
// Note: constant "NoIcon" in that column.

// If the third argument is true, then this is the fall-back for
// this language (named in the first argument).

BeginSet(zh,TW,true)

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,NULL)

	//          (id, 		                    szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"�s�W", 	tb_new_xpm,		NULL, "�إߤ@���s�����")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"�}��",		tb_open_xpm,	NULL, "�}�Ҳ{�����")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"�x�s", 	tb_save_xpm,	NULL, "�x�s���")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"�t�s�s��", 	tb_save_as_xpm,	NULL, "�N���t�s�s��")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"�C�L",		tb_print_xpm,	NULL, "�L�X�o�����")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"�_��",		tb_undo_xpm,	NULL, "�_��")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"����",		tb_redo_xpm,	NULL, "����")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"�ŤU",		tb_cut_xpm,		NULL, "�ŤU")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"�ƻs",		tb_copy_xpm,	NULL, "�ƻs")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"�K�W",	tb_paste_xpm,	NULL, "�K�W")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"�榡",		NoIcon,			NULL, "�榡")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"�r��",		NoIcon,			NULL, "�r��")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"�r���j�p", 	NoIcon,		NULL, "�r���j�p")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"����",		tb_text_bold_xpm,	NULL, "����")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"����",	tb_text_italic_xpm,	NULL, "����")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"���u",tb_text_underline_xpm,	NULL, "���u")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"�W�нu",tb_text_overline_xpm,	NULL, "�W�нu")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"�����u",   tb_text_strikeout_xpm,	NULL, "�����u")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"�W�Цr",	tb_text_superscript_xpm,	NULL, "�W�Цr")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"�U�Цr",	tb_text_subscript_xpm,		NULL, "�U�Цr")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"��",		tb_text_align_left_xpm,		NULL, "�a�����")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"��",		tb_text_center_xpm,	NULL, "�m�����")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"�k",	tb_text_align_right_xpm,	NULL, "�a�k���")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"�վ�",	tb_text_justify_xpm,	NULL, "�վ���")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"�P�e�q�Z��: �L",		tb_para_0before_xpm,	NULL, "�P�e�q�Z��: �L")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"�P�e�q�Z��: 12 �I",		tb_para_12before_xpm,	NULL, "�P�e�q�Z��: 12 �I")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"��@�氪",	tb_line_single_space_xpm,	NULL, "��@�氪")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"1.5 ���氪",		tb_line_middle_space_xpm,	NULL, "1.5 ���氪")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"�⭿�氪",	tb_line_double_space_xpm,	NULL, "�⭿�氪")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,			"1 ��",			tb_1column_xpm,			NULL, "1 ��")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,			"2 ��",		tb_2column_xpm,			NULL, "2 ��")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,			"3 ��",		tb_3column_xpm,			NULL, "3 ��")

	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,			"�Y��",		NoIcon,			NULL, "�Y��")
	
	// ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
