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

/* Translations are provided by "alper" <shullgum@yahoo.com> */

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

BeginSetEnc(tr,TR,true,"cp1254")

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,			NoIcon,					NULL,NULL)

	//          (id, 		                    szLabel,		IconName,     			szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"Yeni", 		tb_new_xpm,				NULL, "Yeni belge yarat")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"A�",			tb_open_xpm,			NULL, "Mevcut bir belgeyi a�")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Kaydet", 		tb_save_xpm,			NULL, "Belgeyi kaydet")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Farkl� kaydet",tb_save_as_xpm,			NULL, "Belgeyi farkl� adla kaydet")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Yazd�r",		tb_print_xpm,			NULL, "Belgeyi yazd�r")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW,	"Bas�m �nizleme",	tb_print_preview_xpm, NULL, "Bas�mdan �nce belgeyi �nizle")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Geri al",		tb_undo_xpm,			NULL, "D�zenlemeyi geri al")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Yinele",		tb_redo_xpm,			NULL, "Geri al�nan� tekrar yap")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Kes",			tb_cut_xpm,				NULL, "Kes")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Kopyala",		tb_copy_xpm,			NULL, "Kopyala")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Yap��t�r",		tb_paste_xpm,			NULL, "Yap��t�r")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_HEADER,	 "�stbilgi d�zenle",tb_edit_editheader_xpm,			NULL, "�stbilgiyi d�zenle")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_FOOTER,	 "Altbilgi d�zenle",tb_edit_editfooter_xpm,			NULL, "Altbilgiyi d�zenle")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REMOVEHEADER, "�stbilgiyi kald�r",	tb_edit_removeheader_xpm,			NULL, "�stbilgiyi kald�r")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REMOVEFOOTER, "Altbilgiyi kald�r",	tb_edit_removefooter_xpm,			NULL, "Altbilgiyi kald�r")
	ToolbarLabel(AP_TOOLBAR_ID_SPELLCHECK,		"Yaz�m Denetimi",tb_spellcheck_xpm,		NULL, "Belgede yaz�m denetimi yap")
	ToolbarLabel(AP_TOOLBAR_ID_IMG,				"�mge ekle",	tb_insert_graphic_xpm,	NULL, "Belgeye bir imge ekle")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"Bi�em",		NoIcon,					NULL, "Bi�em")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"Yaz�tipi",		NoIcon,					NULL, "Yaz�tipi")
    ToolbarLabel(AP_TOOLBAR_ID_FMT_HYPERLINK,  "Ba�lant� ekle", tb_hyperlink,           NULL, "Belgeye bir metin ba�lant�s� ekle")
    ToolbarLabel(AP_TOOLBAR_ID_FMT_BOOKMARK, "Yer imi ekle",    tb_anchor,              NULL, "Belgeye bir yer imi ekle")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Yaz�tipi boyutu",	NoIcon,				NULL, "Yaz�tipi boyutu")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Kal�n",		tb_text_bold_xpm,		NULL, "Kal�n")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"�talik",		tb_text_italic_xpm,		NULL, "�talik")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Alt�izgi",	    tb_text_underline_xpm,	NULL, "Alt�izgi")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"�st�izgi",		tb_text_overline_xpm,	NULL, "�st�izgi")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"��kart",		tb_text_strikeout_xpm,	NULL, "��kart")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_TOPLINE,		"Tepe�izgi",	tb_text_topline_xpm,	NULL, "Tepe�izgi")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOTTOMLINE,	"Dip�izgi",		tb_text_bottomline_xpm,	NULL, "Dip�izgi")
	ToolbarLabel(AP_TOOLBAR_ID_HELP,			"Yard�m",		tb_help_xpm,			NULL, "Yard�m")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"�stsimge",	    tb_text_superscript_xpm,	NULL, "�stsimge")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"Altsimge",     tb_text_subscript_xpm,		NULL, "Altsimge")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,	"Sembol",		tb_symbol_xpm,				NULL, "Araya sembol ekle")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"Sola",			tb_text_align_left_xpm,		NULL, "Sola yasl�")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Ortala",		tb_text_center_xpm,			NULL, "Ortala")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"Sa�a",		    tb_text_align_right_xpm,	NULL, "Sa�a yasl�")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Tam",		    tb_text_justify_xpm,		NULL, "Tam yasl�")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"�nce aral�k yok",	tb_para_0before_xpm,	NULL, "�ncesinde aral�k yok")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"�nce 12 pt var",	tb_para_12before_xpm,	NULL, "�ncesinde 12 pt aral�k var")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Tek aral�k",	tb_line_single_space_xpm,	NULL, "Tek sat�r aral���")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"1.5 aral�k",	tb_line_middle_space_xpm,	NULL, "1.5 sat�r aral���")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"�ift aral�k",	tb_line_double_space_xpm,	NULL, "�ift sat�r aral���")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,			"1 S�tun",		tb_1column_xpm,			NULL, "1 S�tun")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,			"2 S�tun",		tb_2column_xpm,			NULL, "2 S�tun")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,			"3 S�tun",		tb_3column_xpm,			NULL, "3 S�tun")

	ToolbarLabel(AP_TOOLBAR_ID_VIEW_SHOWPARA,	"T�m�n� g�ster",    tb_view_showpara_xpm,		NULL, "� Gizle/G�ster")
	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,			"B�y�lt",			NoIcon,					NULL, "B�y�lt")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,	"Mermi imi",		tb_lists_bullets_xpm,	NULL, "Mermi imleri")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,	"Numaralama",	    tb_lists_numbers_xpm,	NULL, "Numaralama")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,		"Yaz�tipi rengi",	tb_text_fgcolor_xpm,	NULL, "Yaz�tipi rengi")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,		"Vurgula",		    tb_text_bgcolor_xpm,	NULL, "Vurgula")
	ToolbarLabel(AP_TOOLBAR_ID_INDENT,			"Girinti artt�r",	tb_text_indent_xpm, 	NULL, "Girinti artt�r")
	ToolbarLabel(AP_TOOLBAR_ID_UNINDENT,		"Girinti azalt",	tb_text_unindent_xpm,	NULL, "Girinti azalt")

	ToolbarLabel(AP_TOOLBAR_ID_SCRIPT_PLAY,		"Betik y�r�t",	    tb_script_play_xpm,		NULL, "Beti�i y�r�t")

#ifdef BIDI_ENABLED
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_LTR,	"Soldan-sa�a metin",		tb_text_direction_ltr_xpm,	NULL, "Soldan-sa�a metin zorunlu")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_RTL,	"Sa�dan-sola metin",		tb_text_direction_rtl_xpm,	NULL, "Sa�dan-sola metin zorunlu")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DOM_DIRECTION,		"Paragraf Y�n�",	        tb_text_dom_direction_rtl_xpm,	NULL, "Bask�n paragraf y�n�n� de�i�tir")
#endif

     // ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,			NoIcon,			NULL,NULL)

EndSet()
