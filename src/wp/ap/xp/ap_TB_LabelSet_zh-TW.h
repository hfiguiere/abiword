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
 *  ThizLinux Laboratory Ltd.  http://www.thizlinux.com/
 *     Anthony Fok <anthony@thizlinux.com>
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

BeginSetEnc(zh,TW,true,"BIG5")

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,		NULL,		NULL)

	//          (id,				szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"�s�W", 	tb_new_xpm,	NULL, "�إߤ@���s�����")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"�}��",		tb_open_xpm,	NULL, "�}�Ҳ{�����")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"�x�s", 	tb_save_xpm,	NULL, "�x�s���")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"�t�s�s��", 	tb_save_as_xpm,	NULL, "�N���t�s�s��")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"�C�L",		tb_print_xpm,	NULL, "�L�X�o�����")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW,	"�w���C�L",	tb_print_preview_xpm, NULL, "�C�L�e���b�ù��W�w���C�L�ĪG")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"�_��",		tb_undo_xpm,	NULL, "�_��")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"����",		tb_redo_xpm,	NULL, "����")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"�ŤU",		tb_cut_xpm,	NULL, "�ŤU")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"�ƻs",		tb_copy_xpm,	NULL, "�ƻs")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"�K�W",		tb_paste_xpm,	NULL, "�K�W")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_HEADER,		"�s�譶��",	tb_edit_editheader_xpm,		NULL, "�s�譶��")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_FOOTER,		"�s�譶��",	tb_edit_editfooter_xpm,		NULL, "�s�譶��")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REMOVEHEADER,	"��������",	tb_edit_removeheader_xpm,	NULL, "��������")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REMOVEFOOTER,	"��������",	tb_edit_removefooter_xpm,	NULL, "��������")
	ToolbarLabel(AP_TOOLBAR_ID_SPELLCHECK,		"���r�ˬd",	tb_spellcheck_xpm,		NULL, "�����i����r�ˬd")
	ToolbarLabel(AP_TOOLBAR_ID_IMG,			"���J�v��",	tb_insert_graphic_xpm,	NULL, "��@�i�v�����J�����")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"�˦�",		NoIcon,		NULL, "�˦�")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"�r��",		NoIcon,		NULL, "�r��")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_HYPERLINK,	"���J�W�s��", tb_hyperlink, NULL, "��@�ӶW�s�����J�����")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOOKMARK,	"���J����", tb_anchor, NULL, "��@�Ӯ��Ҵ��J�����")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"�r���j�p", 	NoIcon,		NULL, "�r���j�p")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"����",		tb_text_bold_xpm,	NULL, "����")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"����",		tb_text_italic_xpm,	NULL, "����")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"���u",		tb_text_underline_xpm,	NULL, "���u")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"�W�нu",	tb_text_overline_xpm,	NULL, "�W�нu")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"�R���u", 	tb_text_strikeout_xpm,	NULL, "�R���u")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_TOPLINE,		"�泻�[�u",	tb_text_topline_xpm,	NULL, "�泻�[�u")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOTTOMLINE,	"�橳�[�u",	tb_text_bottomline_xpm,	NULL, "�橳�[�u")
	ToolbarLabel(AP_TOOLBAR_ID_HELP,		"����",		tb_help_xpm,		NULL, "����")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"�W�Цr",	tb_text_superscript_xpm,	NULL, "�W�Цr")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"�U�Цr",	tb_text_subscript_xpm,		NULL, "�U�Цr")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,	"�Ÿ�",		tb_symbol_xpm,		NULL, "���J�Ÿ�")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"��",		tb_text_align_left_xpm,		NULL, "�a�����")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"��",		tb_text_center_xpm,	NULL, "�m�����")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"�k",		tb_text_align_right_xpm,	NULL, "�a�k���")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"���k",		tb_text_justify_xpm,	NULL, "���k���")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"�P�e�q�Z��: �L",	tb_para_0before_xpm,	NULL, "�P�e�q�Z��: �L")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"�P�e�q�Z��: 12 �I",	tb_para_12before_xpm,	NULL, "�P�e�q�Z��: 12 �I")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"��涡�Z",	tb_line_single_space_xpm,	NULL, "��涡�Z")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"1.5 ���氪",	tb_line_middle_space_xpm,	NULL, "1.5 ���氪")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"�⭿�氪",	tb_line_double_space_xpm,	NULL, "�⭿�氪")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,		"�@��",		tb_1column_xpm,			NULL, "�@��")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,		"����",		tb_2column_xpm,			NULL, "����")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,		"�T��",		tb_3column_xpm,			NULL, "�T��")

	ToolbarLabel(AP_TOOLBAR_ID_VIEW_SHOWPARA,	"�������",	tb_view_showpara_xpm,	NULL, "���/���� �q���аO")
	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,		"�Y����",	NoIcon,			NULL, "�Y����")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,	"���زŸ�",	tb_lists_bullets_xpm,	NULL, "���زŸ�")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,	"�s��",		tb_lists_numbers_xpm,	NULL, "�s��")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,		"�r����m",	tb_text_fgcolor_xpm,	NULL, "�r����m")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,		"���ش���",	tb_text_bgcolor_xpm,	NULL, "���ش���")
	ToolbarLabel(AP_TOOLBAR_ID_INDENT,		"�W�[�Y��",	tb_text_indent_xpm, 	NULL, "�W�[�Y��")
	ToolbarLabel(AP_TOOLBAR_ID_UNINDENT,		"��p�Y��",	tb_text_unindent_xpm,	NULL, "��p�Y��")
	ToolbarLabel(AP_TOOLBAR_ID_SCRIPT_PLAY,		"���� script",	tb_script_play_xpm, 	NULL, "���� script")

#ifdef BIDI_ENABLED
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_LTR,	"�j���r���ܥk��",	tb_text_direction_ltr_xpm,	NULL, "�j���r�q���ܥk�ƦC")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_RTL,        "�j���r�k�ܥ���",	tb_text_direction_rtl_xpm,	NULL, "�j���r�q�k�ܥ��ƦC")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DOM_DIRECTION,	"�q����V",	tb_text_dom_direction_rtl_xpm,	NULL, "�ܧ�q����r���D�n��V")
#endif
	
	// ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
