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

/* Original italian translations provided by Mauro Colorio <macolori@tin.it> */
/* Work continued by Marco Innocenti <dot0037@iperbole.bologna.it> */

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

// If the second argument is UT_TRUE, then this is the fall-back for
// this language (named in the first two letters).

BeginSet(ItIT,UT_TRUE)

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,			NoIcon,			NULL,NULL)

	//          (id, 		                    szLabel,		IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"Nuovo", 		tb_new_xpm,		NULL, "Crea un nuovo documento")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"Apri",			tb_open_xpm,	NULL, "Apre un documento esistente")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Salva", 		tb_save_xpm,	NULL, "Salva il documento")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Salva con nome", 	tb_save_as_xpm,	NULL, "Salva il documento con un altro nome")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Stampa",		tb_print_xpm,	NULL, "Stampa il documento")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Annulla",		tb_undo_xpm,	NULL, "Annulla la modifica")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Ripristina",	tb_redo_xpm,	NULL, "Ripristina la modifica")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Taglia",		tb_cut_xpm,		NULL, "Taglia")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Copia",		tb_copy_xpm,	NULL, "Copia")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Incolla",		tb_paste_xpm,	NULL, "Incolla")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"Stile",		NoIcon,			NULL, "Stile")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"Carattere",	NoIcon,			NULL, "Carattere")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Grandezza carattere", NoIcon,		NULL, "Grandezza carattere")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Grassetto",	tb_text_bold_G_xpm,		NULL, "Grassetto")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"Italico",		tb_text_italic_xpm,		NULL, "Italico")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Sottolineato",	tb_text_underline_S_xpm,	NULL, "Sottolineato")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Overline",tb_text_overline_xpm,	NULL, "Mette una linea sopra i caratteri")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"Barrato",		tb_text_strikeout_B_xpm,	NULL, "Barrato")

        ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,     "Apice",  tb_text_superscript_xpm,        NULL, "Apice")
        ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,       "Pedice",    tb_text_subscript_xpm,          NULL, "Pedice")
		
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"Sinistra",		tb_text_align_left_xpm,	NULL, "Allineamento a sinistra")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Centro",		tb_text_center_xpm,		NULL, "Allineamento al centro")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"Destra",		tb_text_align_right_xpm,NULL, "Allineamento a destra")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Giustificato",	tb_text_justify_xpm,	NULL, "Giustifica il paragrafo")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Nessuna",		tb_para_0before_xpm,	NULL, "Spaziatura: Nessuna")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 pt",		tb_para_12before_xpm,	NULL, "Spaziatura: 12 pt")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Interlinea singola",	tb_line_single_space_xpm,	NULL, "Interlinea singola")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"Interlinea 1.5",		tb_line_middle_space_xpm,	NULL, "Interlinea 1.5")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Interlinea doppia",	tb_line_double_space_xpm,	NULL, "Interlinea doppia")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,			"1 Colonna",	tb_1column_xpm,			NULL, "1 Colonna")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,			"2 Colonne",	tb_2column_xpm,			NULL, "2 Colonne")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,			"3 Colonne",	tb_3column_xpm,			NULL, "3 Colonne")

	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,			"Zoom",			NoIcon,			NULL, "Zoom")
	
	// ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,			NoIcon,			NULL,NULL)

EndSet()
