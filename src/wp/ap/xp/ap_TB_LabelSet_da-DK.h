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

// Danish translations provided by Martin Willemoes Hansen <mwh@stampede.org>
//                             , Birger Langkjer <birger.langkjer@image.dk>
//                             and Rasmus Toftdahl Olesen <rto@post.tele.dk>
//                             Kenneth Christiansen <kenneth@gnu.org>

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

BeginSet(da,DK,true)

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,NULL)

	//          (id, 		                    szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"Ny", 		tb_new_xpm,	NULL, "Nyt dokument")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"�bn",		tb_open_xpm,	NULL, "�bn et eksisterende dokument")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Gem",	 	tb_save_xpm,	NULL, "Gem dokumentet")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Gem som", 	tb_save_as_xpm,	NULL, "Gem dokumentet under et nyt navn")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Udskriv",	tb_print_xpm,	NULL, "Udskriv dokumentet")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Fortryd",	tb_undo_xpm,	NULL, "Fortryd")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Gentag",	tb_redo_xpm,	NULL, "Gentag")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Klip",		tb_cut_xpm,	NULL, "Klip")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Kopi�r",	tb_copy_xpm,	NULL, "Kopi�r")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Inds�t",	tb_paste_xpm,	NULL, "Inds�t")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"Stil",		NoIcon,			NULL, "Stil")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"Skrift",		NoIcon,			NULL, "Skrifttype")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Skriftst�rrelse", 	NoIcon,		NULL, "Skrifttypest�rrelse")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Fed",		tb_text_bold_F_xpm,	NULL, "Fed")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"Kursiv",	tb_text_italic_K_xpm,	NULL, "Kursiv")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Understreget",	tb_text_underline_xpm,	NULL, "Understreget")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Overstreget", tb_text_overline_xpm,	NULL, "Overstreget")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"Gennemstreget", tb_text_strikeout_G_xpm,	NULL, "Gennemstreget")
    ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,		"H�vet",		tb_text_superscript_xpm,        NULL, "H�vet skrift")
    ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,		"S�nket",		tb_text_subscript_xpm,          NULL, "S�nket skrift")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,	"Symbol",	tb_symbol_xpm,		NULL, "Inds�t symbol eller specialtegn")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"Venstre",	tb_text_align_left_xpm,	NULL, "Venstrestillet")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Centreret",	tb_text_center_xpm,	NULL, "Centreret")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"H�jre",	tb_text_align_right_xpm,NULL, "H�jrestillet")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Lige marginer", tb_text_justify_xpm,	NULL, "Lige marginer")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Intet f�r",	tb_para_0before_xpm,	NULL, "Afsnitsmellemrum: intet")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 pt f�r",	tb_para_12before_xpm,	NULL, "Afsnitsmellemrum: 12 pt")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Intet f�r",		tb_para_0before_xpm,	NULL, "Afsnitsmellemrum: intet")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 pkt f�r",		tb_para_12before_xpm,	NULL, "Afsnitsmellemrum: 12 pkt")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Enkelt linjeafstand",	tb_line_single_space_xpm,	NULL, "Enkelt linjeafstand")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"1.5 linjeafstand",	tb_line_middle_space_xpm,	NULL, "1.5 linjeafstand")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Dobbelt linjeafstand",	tb_line_double_space_xpm,	NULL, "Dobbelt linjeafstand")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,		"1 kolonne",	tb_1column_xpm,		NULL, "1 kolonne")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,		"2 kolonner",	tb_2column_xpm,		NULL, "2 kolonner")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,		"3 kolonner",	tb_3column_xpm,		NULL, "3 kolonner")

	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,		"Zoom",		NoIcon,			NULL, "Zoom")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,		"Punktliste",		tb_lists_bullets_xpm,		NULL,		"Inds�t punktliste")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,		"Nummerliste",		tb_lists_numbers_xpm,		NULL,		"Inds�t nummereret liste")
        ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,          "Forgrundsfarve",     NoIcon,                 NULL, "�ndr forgrundsfarve")
        ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,          "Baggrundsfarve",     NoIcon,                 NULL, "�ndr baggrundsfarve")
        ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW, "Vis udskrift", tb_print_preview_xpm, NULL, "Vis dokumentet f�r det udskrives")
        ToolbarLabel(AP_TOOLBAR_ID_INDENT, "Ryk paragraf ind", tb_text_indent_xpm, NULL, "�g indrykningen af paragraf")
        ToolbarLabel(AP_TOOLBAR_ID_UNINDENT, "Ryk paragraf ud", tb_text_unindent_xpm, NULL, "Formindsk indrykningen af paragraf")
        ToolbarLabel(AP_TOOLBAR_ID_SPELLCHECK, "Stavekontrol", tb_spellcheck_xpm, NULL, "Stavekontroll�r dokumentet")
	// ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()


