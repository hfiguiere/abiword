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

// Lithuanian Translation provided by Gediminas Paulauskas <menesis@delfi.lt>

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
// Note: the icon name here is to a specific icon (pixmap or bitmap or whatever)AbiWord
// Note: that will always be in the application.  if, for example, a big fat 'B'
// Note: for BOLD doesn't make sense in another language, change the entry in
// Note: the localization and add the icon to whatever table.

// Note: if a tool item does not use an icon (like a combo box), use the
// Note: constant "NoIcon" in that column.

// If the third argument is UT_TRUE, then this is the fall-back for
// this language (named in the first argument).

BeginSet(lt,LT,UT_TRUE)

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,		NULL)

	//          (id, 		                    szLabel,	IconName,	    szToolTip,	szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"Naujas",		tb_new_xpm,		NULL,		"")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"Atidaryti",	tb_open_xpm,	NULL,		"")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Saugoti", 	tb_save_xpm,NULL,		"")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Saugoti kaip", 	tb_save_as_xpm,	NULL,	"")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Spausdinti",	tb_print_xpm,	NULL,		"")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"At�aukti",	tb_undo_xpm,NULL,		"")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Kartoti",	tb_redo_xpm,NULL,		"")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"I�kirpti",	tb_cut_xpm,	NULL,		"")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Kopijuoti",	tb_copy_xpm,	NULL,		"")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"�d�ti",	tb_paste_xpm,	NULL,		"")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"Stilius",		NoIcon,			NULL,		"")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"�riftas",   NoIcon,		NULL,		"")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"�rifto dydis", NoIcon,		NULL,		"")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Storas",		tb_text_bold_xpm,	NULL,	"")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"Kursyvas",	tb_text_italic_xpm,	NULL,	"")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Pabrauktas",tb_text_underline_xpm,	NULL,	"")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Br�k�nys vir�",tb_text_overline_xpm,	NULL, "")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"Perbrauktas", tb_text_strikeout_xpm,	NULL,	"")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"Pakeltas",	tb_text_superscript_xpm,	NULL, "")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"Nuleistas",	tb_text_subscript_xpm,		NULL, "")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"Kair�n",	tb_text_align_left_xpm, NULL, "Lygiuoti pagal kair�")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Centre",tb_text_center_xpm,		NULL, "Lygiuoti pagal centr�")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"De�in�n",	tb_text_align_right_xpm,NULL, "Lygiuoti pagal de�in�")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"I�lyginti",	tb_text_justify_xpm,	NULL, "I�lyginti abu kra�tus")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Jokio",	tb_para_0before_xpm,	NULL,	"Tarpas prie�: jokio")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 pt",	tb_para_12before_xpm,	NULL,	"Tarpas prie�: 12 pt")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Viengubi",	tb_line_single_space_xpm,	NULL,	"Viengubi tarpai tarp eilu�i�")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"1,5 eilut�s",	   tb_line_middle_space_xpm,	NULL, "1,5 eilut�s tarpai")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Dvigubi",	tb_line_double_space_xpm,	NULL, "Dvigubi tarpai")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,		"1 stulpelis",		tb_1column_xpm,	NULL,	"")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,		"2 stulpeliai",	tb_2column_xpm,	NULL,	"")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,		"3 stulpeliai",	tb_3column_xpm,	NULL,	"")

	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,		"Mastelis",			NoIcon,			NULL,	"")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,	"Punkt� s�ra�ai",	tb_lists_bullets_xpm,		NULL,		"Prad�ti/Sustabdyti punkt� s�ra�us")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,	"Numeruoti s�ra�ai",	tb_lists_numbers_xpm,		NULL,		"Prad�ti/Sustabdyti numeruotus s�ra�us")
	
	// ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
