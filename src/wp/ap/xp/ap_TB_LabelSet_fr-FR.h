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

/* traduction fcella@mahj.org du 24/05/1999
 * compl�t�e par Philippe Duperron <duperron@mail.dotcom.fr> 08/06/1999  */

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

BeginSet(FrFR,UT_TRUE)

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,		NULL, NULL)

	//          (id, 		                szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"Nouveau", 			tb_new_xpm,	NULL, "Creer un nouveau document")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"Ouvrir",			tb_open_xpm,	NULL, "Ouvrir un document existant")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Enregistrer",		tb_save_xpm,	NULL, "Enregistrer le document")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Enregistrer sous",	tb_save_as_xpm,	NULL, "Enregistrer le document sous un nouveau nom")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Imprimer",			tb_print_xpm,	NULL, "Imprimer le document")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Annuler",			tb_undo_xpm,	NULL, "Annuler la frappe")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"R�p�ter",			tb_redo_xpm,	NULL, "R�p�ter la frappe")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Couper",			tb_cut_xpm,		NULL, "Couper")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Copier",			tb_copy_xpm,	NULL, "Copier")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Coller",			tb_paste_xpm,	NULL, "Coller")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"Style",			NoIcon,			NULL, "Style")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"Police",			NoIcon,			NULL, "Police")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Taille",			NoIcon,			NULL, "Taille")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Gras",				tb_text_bold_G_xpm,	NULL, "Gras")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"Italique",			tb_text_italic_xpm,	NULL, "Italique")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Soulign�",			tb_text_underline_S_xpm,	NULL, "Soulign�")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Barr� haut",		tb_text_overline_xpm,		NULL, "Barr� haut")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"Barr�",			tb_text_strikeout_B_xpm,	NULL, "Barr�")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"Exposant",			tb_text_superscript_xpm,	NULL, "Exposant")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"Indice",			tb_text_subscript_xpm,		NULL, "Indice")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"Gauche",			tb_text_align_left_xpm,		NULL, "Alignement � gauche")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Centr�",			tb_text_center_xpm,			NULL, "Paragraphe centr�")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"Droite",			tb_text_align_right_xpm,	NULL, "Alignement � droite")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Justifi�",			tb_text_justify_xpm,		NULL, "Paragraphe justifi�")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Pas d'espacement avant",		tb_para_0before_xpm,	NULL, "Espacement avant: aucun")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"Espacement de 12 pt avant",	tb_para_12before_xpm,	NULL, "Espacement avant: 12 pt")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Interligne simple",	tb_line_single_space_xpm,		NULL, "Interligne simple")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"Interligne : 1,5 lignes",	tb_line_middle_space_xpm,	NULL, "Interligne : 1,5 lignes")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Interligne double",	tb_line_double_space_xpm,		NULL, "Interligne double")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,			"Une colonne",		tb_1column_xpm,			NULL, "Une colonne")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,			"Deux colonnes",	tb_2column_xpm,			NULL, "Deux colonnes")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,			"Trois colonnes",	tb_3column_xpm,			NULL, "Trois colonnes")

	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,			"Zoom",				NoIcon,			NULL, "Zoom")
	
	// ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
