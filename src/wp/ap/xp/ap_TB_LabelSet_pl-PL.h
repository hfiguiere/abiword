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

// First Polish translation by Sercxemulo <explo@poczta.wp.pl>
// Since December 2001 maintained by Piotr Banski <bansp@bigfoot.com>
// Since February 2002 Andrzej Pruszynski <ap@ignis.pl> joined
// Last update: 14-apr-2002
BeginSetEnc(pl,PL,true,"iso-8859-2")

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,NULL)

	//          (id, 		                    szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"Nowy", 		tb_new_xpm,		NULL, "Tworzy nowy dokument")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"Otw�rz",		tb_open_xpm,	NULL, "Otwiera dokument z pliku...")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Zapisz", 	tb_save_xpm,	NULL, "Zapisuje")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Zapisz jako", 	tb_save_as_xpm,	NULL, "Zapisuje jako...")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Drukuj",	tb_print_xpm,	NULL, "Drukuje...")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW,	"Podgl�d wydruku",	tb_print_preview_xpm, NULL, "Wy�wietla podgl�d wydruku")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Cofnij",		tb_undo_xpm,	NULL, "Cofa ostatni� czynno��")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Anuluj cofni�cie",		tb_redo_xpm,	NULL, "Anuluje cofni�cie ostatniej czynno�ci")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Wytnij",		tb_cut_xpm,		NULL, "Wycina do schowka")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Skopiuj",		tb_copy_xpm,	NULL, "Kopiuje do schowka")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Wklej",	tb_paste_xpm,	NULL, "Wkleja ze schowka")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_HEADER,	 "Nag��wek",		tb_edit_editheader_xpm,			NULL, "Edycja nag��wka")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_FOOTER,	 "Stopka",		tb_edit_editfooter_xpm,			NULL, "Edycja stopki")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REMOVEHEADER, "Usu� nag��wek",		tb_edit_removeheader_xpm,			NULL, "Usuwa nag��wek")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REMOVEFOOTER, "Usu� stopk�",		tb_edit_removefooter_xpm,			NULL, "Usuwa stopk�")
	ToolbarLabel(AP_TOOLBAR_ID_SPELLCHECK,		"Pisownia",	tb_spellcheck_xpm,		NULL, "Sprawdza pisowni�")
	ToolbarLabel(AP_TOOLBAR_ID_IMG,				"Rycina",	tb_insert_graphic_xpm,	NULL, "Wstawia rycin�")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"Styl",	NoIcon,			NULL, "Styl akapitu")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"Rodzina czcionki",		NoIcon,			NULL, "Rodzina czcionki")
   ToolbarLabel(AP_TOOLBAR_ID_FMT_HYPERLINK, "Hiper��cze", tb_hyperlink, NULL, "Wstawia hiper��cze")
   ToolbarLabel(AP_TOOLBAR_ID_FMT_BOOKMARK, "Zak�adka", tb_anchor, NULL, "Wstawia zak�adk�")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Rozmiar czcionki", NoIcon,		NULL, "Rozmiar czcionki")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Pogrubiona",		tb_text_bold_xpm,		NULL, "Czcionka pogrubiona")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"Kursywa",	tb_text_italic_xpm,	NULL, "Kursywa")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Podkre�lona",tb_text_underline_xpm,	NULL, "Czcionka podkre�lona")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Nadkre�lona",		tb_text_overline_xpm,	NULL, "Czcionka nadkre�lona")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"Przekre�lona",   tb_text_strikeout_xpm,	NULL, "Czcionka przekre�lona")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_TOPLINE,		"Linia nad tekstem",		tb_text_topline_xpm,	NULL, "Linia nad tekstem")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOTTOMLINE,		"Linia pod tekstem",		tb_text_bottomline_xpm,	NULL, "Linia pod tekstem")
	ToolbarLabel(AP_TOOLBAR_ID_HELP,			"Pomoc",			tb_help_xpm,			NULL, "System pomocy")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"Indeks g�rny",	tb_text_superscript_xpm,	NULL, "Indeks g�rny")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"Indeks dolny",	tb_text_subscript_xpm,		NULL, "Indeks dolny")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,	"Symbol",		tb_symbol_xpm,				NULL, "Wstawia symbol")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"Do lewej",		tb_text_align_left_xpm,		NULL, "Wyr�wnuje do lewej")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Wy�rodkowanie",	tb_text_center_xpm,	NULL, "Wy�rodkowuje")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"Do prawej",	tb_text_align_right_xpm,	NULL, "Wyr�wnuje do prawej")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Justowanie",	tb_text_justify_xpm,	NULL, "Wyjustowuje")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Nic przed",		tb_para_0before_xpm,	NULL, "Bez odst�pu przed akapitem")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12pkt przed",		tb_para_12before_xpm,	NULL, "Odst�p 12 punkt�w przed akapitem")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Interlinia 1",	tb_line_single_space_xpm,	NULL, "Interlinia pojedyncza")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"Interlinia 1.5",		tb_line_middle_space_xpm,	NULL, "Interlinia p�tora wiersza")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Interlinia 2",	tb_line_double_space_xpm,	NULL, "Interlinia podw�jna")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,			"1 Kolumna",			tb_1column_xpm,			NULL, "1 Kolumna")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,			"2 Kolumny",		tb_2column_xpm,			NULL, "2 Kolumny")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,			"3 Kolumny",		tb_3column_xpm,			NULL, "3 Kolumny")

	ToolbarLabel(AP_TOOLBAR_ID_VIEW_SHOWPARA,	"Kody steruj�ce",			tb_view_showpara_xpm,		NULL, "Prze��cza widoczno�� kod�w steruj�cych")
	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,			"Skala",		NoIcon,			NULL, "Skali podgl�du")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,	"Wypunktowanie",			tb_lists_bullets_xpm,	NULL, "Lista wypunktowana")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,	"Numerowanie",		tb_lists_numbers_xpm,	NULL, "Lista numerowana")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,		"Kolor czcionki",		tb_text_fgcolor_xpm,	NULL, "Kolor czcionki")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,		"Kolor t�a czcionki",		tb_text_bgcolor_xpm,	NULL, "Kolor t�a czcionki")
	ToolbarLabel(AP_TOOLBAR_ID_INDENT,			"Wi�ksze wci�cie",	tb_text_indent_xpm, 	NULL, "Zwi�ksza wci�cie akapitu")
	ToolbarLabel(AP_TOOLBAR_ID_UNINDENT,		"Mniejsze wci�cie",	tb_text_unindent_xpm,	NULL, "Zmniejsza wci�cie akapitu")

	ToolbarLabel(AP_TOOLBAR_ID_SCRIPT_PLAY,		"Skrypt",	tb_script_play_xpm,		NULL, "Uruchamia skrypt")

#ifdef BIDI_ENABLED
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_LTR,	"Tekst L->P",		tb_text_direction_ltr_xpm,	NULL, "Wymusza kierunek z lewej do prawej")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_RTL,	"Tekst P->L",		tb_text_direction_rtl_xpm,	NULL, "Wymusza kierunek z prawej do lewej")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DOM_DIRECTION,		"Kierunek tekstu",	tb_text_dom_direction_rtl_xpm,	NULL, "Zmienia kierunek tekstu w akapicie")
#endif

	// ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
