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

BeginSetEnc(sk,SK,true,"iso-8859-2")

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,NULL)

	//          (id, 		                    szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"Nov�",		tb_new_xpm,	NULL, "Vytvori5 nov� dokument")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"Otvori�",	tb_open_xpm,	NULL, "Otvori� ulo�en� dokument")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Ulo�i�", 	tb_save_xpm,	NULL, "Ulo�i� dokument")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Ulo�i� ako", 	tb_save_as_xpm,	NULL, "Ulo�i� dokument pod in�m menom")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Tla�",		tb_print_xpm,	NULL, "Vytla�i� dokument")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW,	"Uk�ka",	tb_print_preview_xpm, NULL, "N�h�ad dokumentu")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Sp�",		tb_undo_xpm,	NULL, "Vr�ti� �rpavy")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Opakova�",	tb_redo_xpm,	NULL, "Opakova� �pravy")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Vystrihn��",	tb_cut_xpm,	NULL, "Vystrihn�� text/objekt")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Kop�rova�",	tb_copy_xpm,	NULL, "Kop�rova�")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Vlo�i�",	tb_paste_xpm,	NULL, "Vlo�i�")
	ToolbarLabel(AP_TOOLBAR_ID_SPELLCHECK,		"Pravopis",	tb_spellcheck_xpm,	NULL, "N�js� preklepy v dokumente")
	ToolbarLabel(AP_TOOLBAR_ID_IMG,			"Vlo�i� obr�zok", tb_insert_graphic_xpm,NULL, "Vlo�i� obr�zok")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"�t�l",		NoIcon,			NULL, "�t�l")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"P�smo",	NoIcon,			NULL, "P�smo")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Ve�kos� p�sma", NoIcon,		NULL, "Ve�kos� p�sma")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Tu�n�",	tb_text_bold_xpm,	NULL, "Tu�n�")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"�ikm�",	tb_text_italic_xpm,	NULL, "�ikm�")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Pod�iarknut�",	tb_text_underline_xpm,	NULL, "Pod�iarknut�")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Pod �iarou",	tb_text_overline_xpm,	NULL, "Pod �iarou")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"Pre�krtnutie",	tb_text_strikeout_xpm,	NULL, "Pre�krtnutie")
	ToolbarLabel(AP_TOOLBAR_ID_HELP,		"Pomocn�k",	tb_help_xpm,		NULL, "Pomocn�k")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"Horn� index",	tb_text_superscript_xpm,NULL, "Horn� index")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"Doln� index",	tb_text_subscript_xpm,	NULL, "Doln� index")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,       "Symbol",       tb_symbol_xpm,          NULL, "Symbol")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"V�avo",	tb_text_align_left_xpm,	NULL, "Zarovna� v�avo")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Stred",	tb_text_center_xpm,	NULL, "Zarovna� do stredu")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"Vpravo",	tb_text_align_right_xpm,	NULL, "Zarovna� vpravo")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Do bloku",	tb_text_justify_xpm,	NULL, "Zarovna� do bloku")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Ni� posledn�",		tb_para_0before_xpm,	NULL, "Ni� posledn�")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 bodov posledn�ch",	tb_para_12before_xpm,	NULL, "12 bodov posledn�ch")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Oby�ajn� riadkovanie",	tb_line_single_space_xpm,	NULL, "Oby�ajn� riadkovanie")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"1.5 riadkovanie",	tb_line_middle_space_xpm,	NULL, "1.5 riadkovanie")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Dvojit� riadkovanie",	tb_line_double_space_xpm,	NULL, "Dvojit� riadkovanie")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,		"1 st�pec",		tb_1column_xpm,			NULL, "1 st�pec")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,		"2 st�pce",		tb_2column_xpm,			NULL, "2 st�pce")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,		"3 st�pce",		tb_3column_xpm,			NULL, "3 st�pce")

	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,		"Zv��enie",		NoIcon,			NULL,	"Zv��enie")
        ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,	"Odr�ky",		tb_lists_bullets_xpm,	NULL,	"Za�a�/Ukon�i� odr�kov� zoznamy")
        ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,	"��slovan� zoznam",	tb_lists_numbers_xpm,	NULL,	"Za�a�/Ukon�i� ��slovan� zoznam") 
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,		"Farba p�sma",		tb_text_fgcolor_xpm,	NULL,	"Farba p�sma")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,		"Zv�raznanie",		tb_text_bgcolor_xpm,	NULL,	"Zv�raznenie")
	ToolbarLabel(AP_TOOLBAR_ID_INDENT,		"Zv��i� odsadenie",	tb_text_indent_xpm,	NULL,	"Zv��i� odsadenie")
	ToolbarLabel(AP_TOOLBAR_ID_UNINDENT,            "Zmen�i� odsadenie",	tb_text_unindent_xpm,	NULL,	"Zmen�i� odsadenie")
	
	// ... add others here ...
#ifdef BIDI_ENABLED
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_LTR, "Text z �ava do prava", tb_text_direction_ltr_xpm, NULL, "")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_RTL, "Text z prava do �ava", tb_text_direction_rtl_xpm, NULL, "")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DOM_DIRECTION, "Smer odstavca",  tb_text_dom_direction_rtl_xpm,  NULL, "")
#endif

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
