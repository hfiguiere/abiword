/* AbiWord
 * Copyright (C) 1998-2001 AbiSource, Inc.
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

/* Translation by  hj <huangj@citiz.net>
 *                 ThizLinux Laboratory Ltd.  http://www.thizlinux.com/
 *                 Anthony Fok <anthony@thizlinux.com>
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

BeginSetEnc(zh,CN,true,"GB2312")

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,NULL)

	//          (id, 		                szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"New", 		tb_new_xpm,	NULL, "�������ĵ�")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"Open",		tb_open_xpm,	NULL, "���ĵ�")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Save", 	tb_save_xpm,	NULL, "�����ĵ�")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Save As", 	tb_save_as_xpm,	NULL, "����ĵ�")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Print",	tb_print_xpm,	NULL, "��ӡ�ĵ�")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW,	"Print Preview",	tb_print_preview_xpm, NULL, "��ӡǰ������Ļ��Ԥ����ӡЧ��")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Undo",		tb_undo_xpm,	NULL, "�ָ��༭")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Redo",		tb_redo_xpm,	NULL, "�����༭")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Cut",		tb_cut_xpm,	NULL, "����")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Copy",		tb_copy_xpm,	NULL, "����")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Paste",	tb_paste_xpm,	NULL, "ճ��")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_HEADER,		"�༭ҳü",	tb_edit_editheader_xpm,		NULL, "�༭ҳü")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_FOOTER,		"�༭ҳ��",	tb_edit_editfooter_xpm,		NULL, "�༭ҳ��")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REMOVEHEADER,	"�Ƴ�ҳü",	tb_edit_removeheader_xpm,	NULL, "�Ƴ�ҳü")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REMOVEFOOTER,	"�Ƴ�ҳ��",	tb_edit_removefooter_xpm,	NULL, "�Ƴ�ҳ��")
	ToolbarLabel(AP_TOOLBAR_ID_SPELLCHECK,		"ƴд���",	tb_spellcheck_xpm,		NULL, "Ϊ�ĵ�����ƴд���")
	ToolbarLabel(AP_TOOLBAR_ID_IMG,			"����Ӱ��",	tb_insert_graphic_xpm,		NULL, "��һ��Ӱ������ĵ���")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"Style",	NoIcon,		NULL, "��ʽ")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"Font",		NoIcon,		NULL, "����")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_HYPERLINK,	"���볬����",	tb_hyperlink,	NULL, "��һ�������Ӳ����ĵ���")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOOKMARK,	"������ǩ",	tb_anchor,	NULL, "��һ����ǩ�����ĵ���")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Font Size", NoIcon,		NULL, "�����С")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Bold",		tb_text_bold_xpm,	NULL, "����")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"Italic",	tb_text_italic_xpm,	NULL, "б��")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Underline",tb_text_underline_xpm,	NULL, "�»���")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Overline",tb_text_overline_xpm,	NULL, "�ϻ���")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"Strike",   tb_text_strikeout_xpm,	NULL, "�л���")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_TOPLINE,		"�ж���",	tb_text_topline_xpm,	NULL, "�ж���")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOTTOMLINE,	"�е���",	tb_text_bottomline_xpm,	NULL, "�е���")
	ToolbarLabel(AP_TOOLBAR_ID_HELP,		"����",		tb_help_xpm,		NULL, "����")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"Superscript",	tb_text_superscript_xpm,	NULL, "�ϱ�")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"Subscript",	tb_text_subscript_xpm,		NULL, "�±�")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,	"����",		tb_symbol_xpm,		NULL, "�������")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"Left",		tb_text_align_left_xpm,	NULL, "�����")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Center",	tb_text_center_xpm,	NULL, "�м����")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"Right",	tb_text_align_right_xpm,	NULL, "�Ҷ���")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Justify",	tb_text_justify_xpm,	NULL, "���˶���")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"None before",		tb_para_0before_xpm,	NULL, "��ǰ���:��")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 pt before",		tb_para_12before_xpm,	NULL, "��ǰ���:12 pt")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Single Spacing",	tb_line_single_space_xpm,	NULL, "�����о�")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"1.5 Spacing",		tb_line_middle_space_xpm,	NULL, "1.5���о�")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Double Spacing",	tb_line_double_space_xpm,	NULL, "˫���о�")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,		"1 Column",		tb_1column_xpm,			NULL, "1��")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,		"2 Columns",		tb_2column_xpm,			NULL, "2��")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,		"3 Columns",		tb_3column_xpm,			NULL, "3��")

	ToolbarLabel(AP_TOOLBAR_ID_VIEW_SHOWPARA,	"ȫ����ʾ",	tb_view_showpara_xpm,	NULL, "��ʾ/���� ������")
	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,		"���ű���",	NoIcon,			NULL, "���ű���")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,	"��Ŀ����",	tb_lists_bullets_xpm,	NULL, "��Ŀ����")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,	"���",		tb_lists_numbers_xpm,	NULL, "���")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,		"������ɫ",	tb_text_fgcolor_xpm,	NULL, "������ɫ")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,		"������ɫ",	tb_text_bgcolor_xpm,	NULL, "������ɫ")
	ToolbarLabel(AP_TOOLBAR_ID_INDENT,		"��������",	tb_text_indent_xpm, 	NULL, "��������")
	ToolbarLabel(AP_TOOLBAR_ID_UNINDENT,		"��С����",	tb_text_unindent_xpm,	NULL, "��С����")
	ToolbarLabel(AP_TOOLBAR_ID_SCRIPT_PLAY,		"ִ�нű�",	tb_script_play_xpm, 	NULL, "ִ�нű�")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_LTR,	"ǿ��������������",	tb_text_direction_ltr_xpm,	NULL, "ǿ�����ִ�����������")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_RTL,        "ǿ��������������",	tb_text_direction_rtl_xpm,	NULL, "ǿ�����ִ�����������")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DOM_DIRECTION,	"���䷽��",	tb_text_dom_direction_rtl_xpm,	NULL, "�ı�������ֵ���Ҫ����")

	// ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
