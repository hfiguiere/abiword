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

// If the third argument is UT_TRUE, then this is the fall-back for
// this language (named in the first argument).

BeginSet(ga,IE,UT_TRUE)

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,NULL)

	//          (id, 		                    szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"&Nua", 		tb_new_xpm,		NULL, "Create an comhad nua")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"&Oscail",		tb_open_xpm,	NULL, "Oscail comhad")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"&S�bh�l", 	tb_save_xpm,	NULL, "S�bh�l an comhad")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"S�bh�l &Mar", 	tb_save_as_xpm,	NULL, "S�bh�l an comhad mar an t-ainm nua")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Cl�",	tb_print_xpm,	NULL, "Cl� an comhad")
#ifdef HAVE_GNOME
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW, "R�amhthaispe�ntas Cl�", tb_print_preview_xpm, NULL, "Preview the document before cl�")
#endif

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Fo-dhean",		tb_undo_xpm,	NULL, "Fo-dhea editing")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Athdh�an",		tb_redo_xpm,	NULL, "Athdh�an editing")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Gearr",		tb_cut_xpm,		NULL, "Gearr")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"&C�ip",		tb_copy_xpm,	NULL, "C�ip")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Greamaigh",	tb_paste_xpm,	NULL, "Greamaigh")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"St�l",	NoIcon,			NULL, "St�l")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"Font",		NoIcon,			NULL, "Font")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Font Size", NoIcon,		NULL, "Font Size")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Cl� Dubh",		tb_text_bold_xpm,		NULL, "Cl� dubh")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"&Iod�ileach",	tb_text_italic_xpm,	NULL, "Cl� iod�ileach")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"L�n Faoi",tb_text_underline_xpm,	NULL, "L�n faoi")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"L�n Thar",tb_text_overline_xpm,	NULL, "L�n thar")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"L�n Tr�d",   tb_text_strikeout_xpm,	NULL, "L�n trid")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"Superscript",	tb_text_superscript_xpm,	NULL, "Superscript")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"Subscript",	tb_text_subscript_xpm,		NULL, "Subscript")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,	"Siombal",	tb_symbol_xpm,		NULL, "Cur siombal")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,	"Cl�",	tb_text_align_left_xpm,		NULL, "alignment cl�")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"L�r",	tb_text_center_xpm,	NULL, "alignment l�r")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,	"Deis",	tb_text_align_right_xpm,	NULL, "alignment deis")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Comhfhadaigh",	tb_text_justify_xpm,	NULL, "Comhfhadaigh paragraf")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Dada roimh",		tb_para_0before_xpm,	NULL, "Space before: Dada")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 pt roimh",		tb_para_12before_xpm,	NULL, "Space before: 12 pt")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Sp�s�il Singil",	tb_line_single_space_xpm,	NULL, "Sp�s�il singil")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"Sp�s�il 1.5",	tb_line_middle_space_xpm,	NULL, "Sp�s�il 1.5")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Sp�s�il Dh�bailte",	tb_line_double_space_xpm,	NULL, "Sp�s�il dh�bailte")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,			"1 Col�n",		tb_1column_xpm,			NULL, "1 Col�n")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,			"2 Col�n",		tb_2column_xpm,			NULL, "2 Col�n")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,			"3 Col�n",		tb_3column_xpm,			NULL, "3 Col�n")

	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,			"Feith",		NoIcon,			NULL, "Feith")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,		"Liosta Pil�araithe",		tb_lists_bullets_xpm,		NULL,		"Thosaigh/Stop Liosta Pil�araithe")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,		"Liosta Uimhrithe",		tb_lists_numbers_xpm,		NULL,		"Thosaigh/Stop Liosta Uimhrithe")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,          "Dath an C�lra",     NoIcon,                 NULL, "Athraigh Dath an C�lra")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,          "Dath an R�amhionad",     NoIcon,                 NULL, "Athraigh Dath an R�amhionad")
	ToolbarLabel(AP_TOOLBAR_ID_INDENT, "Eangaigh Paragraf", tb_text_indent_xpm, NULL, "Eangaigh Paragraf")
	ToolbarLabel(AP_TOOLBAR_ID_UNINDENT, "Bhain Eanganna as Paragraf", tb_text_unindent_xpm, NULL, "Bhain Eanganna as Paragraf")

	// ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
