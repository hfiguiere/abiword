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

BeginSet(cs,CZ,true)

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,NULL)

	//          (id, 		                    szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"Nov�",		tb_new_xpm,	NULL, "Vytvo�it nov� dokument")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"Otev��t",	tb_open_xpm,	NULL, "Otev��t ulo�en� dokument")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Ulo�it", 	tb_save_xpm,	NULL, "Ulo�it dokument")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Ulo�it jako", 	tb_save_as_xpm,	NULL, "Ulo�it dokument pod jin�m jm�nem")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Tisk",		tb_print_xpm,	NULL, "Vytisknout dokument")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW,	"N�hled",	tb_print_preview_xpm, NULL, "N�hled dokumentu")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Zp�t",		tb_undo_xpm,	NULL, "Undo editing")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Vp�ed",	tb_redo_xpm,	NULL, "Redo editing")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Vyjmout",	tb_cut_xpm,	NULL, "Cut")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Kop�rovat",	tb_copy_xpm,	NULL, "Kop�rovat")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Vlo�it",	tb_paste_xpm,	NULL, "Vlo�it")
	ToolbarLabel(AP_TOOLBAR_ID_SPELLCHECK,		"Pravopis",	tb_spellcheck_xpm,	NULL, "Naj�t p�eklepy v dokumentu")
	ToolbarLabel(AP_TOOLBAR_ID_IMG,			"Vlo�it obr�zek", tb_insert_graphic_xpm,NULL, "Vlo�it obr�zek")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"Styl",		NoIcon,			NULL, "Styl")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"P�smo",	NoIcon,			NULL, "P�smo")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Velikost p�sma", NoIcon,		NULL, "Velikost p�sma")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Tu�n�",	tb_text_bold_xpm,	NULL, "Tu�n�")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"Kurz�va",	tb_text_italic_xpm,	NULL, "Kurz�va")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Podtr�en�",	tb_text_underline_xpm,	NULL, "Podtr�en�")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Pod �arou",	tb_text_overline_xpm,	NULL, "Pod �arou")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"P�e�krtnut�",	tb_text_strikeout_xpm,	NULL, "P�e�krtnut�")
	ToolbarLabel(AP_TOOLBAR_ID_HELP,		"Pomoc",	tb_help_xpm,		NULL, "Pomoc")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"Horn� index",	tb_text_superscript_xpm,NULL, "Horn� index")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"Spodn� index",	tb_text_subscript_xpm,	NULL, "Spodn� index")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,       "Symbol",       tb_symbol_xpm,          NULL, "Symbol")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"Vlevo",	tb_text_align_left_xpm,	NULL, "Zarovnat vlevo")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Uprost�ed",	tb_text_center_xpm,	NULL, "Zarovnat doprost�ed")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"Vpravo",	tb_text_align_right_xpm,	NULL, "Zarovnat vpravo")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Do bloku",	tb_text_justify_xpm,	NULL, "Zarovnat do bloku")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Nic naposled",		tb_para_0before_xpm,	NULL, "Nic naposled")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 bod� naposled",	tb_para_12before_xpm,	NULL, "12 bod� naposled")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Oby�ejn� r�dkov�n�",	tb_line_single_space_xpm,	NULL, "Oby�ejn� r�dkov�n�")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"1.5 ��dkov�n�",	tb_line_middle_space_xpm,	NULL, "1.5 ��dkov�n�")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Dvojit� ��dkov�n�",	tb_line_double_space_xpm,	NULL, "Dvojit� ��dkov�n�")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,		"1 sloupec",		tb_1column_xpm,			NULL, "1 sloupec")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,		"2 sloupce",		tb_2column_xpm,			NULL, "2 sloupce")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,		"3 sloupce",		tb_3column_xpm,			NULL, "3 sloupce")

	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,		"Zv�t�en�",		NoIcon,			NULL,	"Zv�t�en�")
        ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,	"Odr�ky",		tb_lists_bullets_xpm,	NULL,	"Start/Stop Bullet Lists")
        ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,	"��slovan� seznam",	tb_lists_numbers_xpm,	NULL,	"Start/Stop Numbered Lists") 
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,		"Barva p�sma",		tb_text_fgcolor_xpm,	NULL,	"Barva p�sma")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,		"Highlight",		tb_text_bgcolor_xpm,	NULL,	"Highlight")
	ToolbarLabel(AP_TOOLBAR_ID_INDENT,		"Zv�t�it odsazen�",	tb_text_indent_xpm,	NULL,	"Zv�t�it odsazen�")
	ToolbarLabel(AP_TOOLBAR_ID_UNINDENT,            "Zmen�it odsazen�",	tb_text_unindent_xpm,	NULL,	"Zmen�it odsazen�")
	
	// ... add others here ...
#ifdef BIDI_ENABLED
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_LTR, "Text z leva do prava", tb_text_direction_ltr_xpm, NULL, "")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_RTL, "Text z prava do leva", tb_text_direction_rtl_xpm, NULL, "")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DOM_DIRECTION, "Sm�r odstavce",  tb_text_dom_direction_rtl_xpm,  NULL, "")
#endif

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
