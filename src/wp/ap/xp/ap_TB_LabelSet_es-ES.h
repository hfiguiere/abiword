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

BeginSetEnc(es,ES,true,"iso-8859-1")

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,NULL)

	//          (id, 		                    szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"Nuevo", 			tb_new_xpm,		NULL, "Crear un nuevo documento")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"Abrir",			tb_open_xpm,	NULL, "Abrir un documento existente")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Guardar",			tb_save_xpm,	NULL, "Guardar el documento")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Guardar como",		tb_save_as_xpm,	NULL, "Guardar el documento con un nombre diferente")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Imprimir",			tb_print_xpm,	NULL, "Imprimir el documento")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW, "Presentaci�n preliminar", tb_print_preview_xpm, NULL, "Presentaci�n preliminar")

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Deshacer",			tb_undo_xpm,	NULL, "Deshacer la edici�n")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Rehacer",			tb_redo_xpm,	NULL, "Rehacer la edici�n")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Cortar",			tb_cut_xpm,		NULL, "Cortar")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Copiar",			tb_copy_xpm,	NULL, "Copiar")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Pegar",			tb_paste_xpm,	NULL, "Pegar")

	ToolbarLabel(AP_TOOLBAR_ID_SPELLCHECK,		"Correcci�n ortogr�fica", tb_spellcheck_xpm, NULL, "Correcci�n ortogr�fica")
	ToolbarLabel(AP_TOOLBAR_ID_IMG, 			"Insertar imagen",	tb_insert_graphic_xpm, NULL, "Insertar una imagen en el documento")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"Estilo",			NoIcon,			NULL, "Estilo")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"Fuente",			NoIcon,			NULL, "Fuente")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Tama�o",			NoIcon,			NULL, "Tama�o de la fuente")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Negrita",			tb_text_bold_N_xpm,		NULL, "Negrita")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"Italica",			tb_text_italic_K_xpm,	NULL, "Cursiva")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Subrayado",		tb_text_underline_S_xpm,NULL, "Subrayado")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Superrayado",		tb_text_overline_xpm,	NULL, "Superrayado")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"Tachado",			tb_text_strikeout_xpm,	NULL, "Tachado")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"Super�ndice",		tb_text_superscript_xpm,NULL, "Super�ndice")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"Sub�ndice",		tb_text_subscript_xpm,	NULL, "Sub�ndice")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,	"S�mbolo",			tb_symbol_xpm,			NULL, "Insertar s�mbolo")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"Izquierda",		tb_text_align_left_xpm,	NULL, "Alinear a la izquierda")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Centro",			tb_text_center_xpm,		NULL, "Alinear en el centro")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"Derecha",			tb_text_align_right_xpm,NULL, "Alinear a la derecha")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Justificar",		tb_text_justify_xpm,	NULL, "Justificar p�rrafo")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Nada antes",		tb_para_0before_xpm,	NULL, "Espaciado anterior: ninguno")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 pt antes",		tb_para_12before_xpm,	NULL, "Espaciado anterior: 12 pt")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Espaciado simple",	tb_line_single_space_xpm,	NULL, "Espaciado simple")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"Espaciado 1,5",	tb_line_middle_space_xpm,	NULL, "Espaciado 1,5")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Espaciado doble",	tb_line_double_space_xpm,	NULL, "Espaciado doble")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,			"1 Columna",		tb_1column_xpm,			NULL, "1 Columna")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,			"2 Columnas",		tb_2column_xpm,			NULL, "2 Columnas")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,			"3 Columnas",		tb_3column_xpm,			NULL, "3 Columnas")

	ToolbarLabel(AP_TOOLBAR_ID_VIEW_SHOWPARA,	"Ver todo",			tb_view_showpara_xpm,		NULL, "Mostrar/esconder �")
	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,			"Zoom",				NoIcon,					NULL, "Zoom")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,	"Vi�etas",			tb_lists_bullets_xpm,	NULL, "Vi�etas")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,	"Listas",			tb_lists_numbers_xpm,	NULL, "Listas numeradas")
	
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,		"Color de la tinta",	tb_text_fgcolor_xpm,				NULL, "Cambiar el color de la tinta")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,		"Color de fondo",	tb_text_bgcolor_xpm,					NULL, "Cambiar el color del fondo")
	ToolbarLabel(AP_TOOLBAR_ID_INDENT,			"Indentar p�rrafo", tb_text_indent_xpm,		NULL, "Incrementar la indentaci�n del p�rrafo")
	ToolbarLabel(AP_TOOLBAR_ID_UNINDENT,		"Desindentar p�rrafo", tb_text_unindent_xpm,	NULL, "Reducir la indentaci�n del p�rrafo")

	// ... add others here ...
#ifdef BIDI_ENABLED
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DOM_DIRECTION,		"Direcci�n de p�rrafo",	tb_text_dom_direction_rtl_xpm,	NULL, "Change dominant direction of paragraph")
#endif
	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
