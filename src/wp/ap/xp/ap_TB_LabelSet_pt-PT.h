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

BeginSet(pt,PT,UT_TRUE)

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,	NULL)

	//          (id, 		                    szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"Novo",		tb_new_xpm,		NULL,	"Criar um novo documento")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"Abrir",	tb_open_xpm,		NULL,	"Abrir um documento existente")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Gravar",	tb_save_xpm,		NULL,	"Gravar o documento")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Gravar como",	tb_save_as_xpm,		NULL,	"Gravar o documento com um nome diferente")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Imprimir",	tb_print_xpm,		NULL,	"Imprimir o docuemnto")

#ifdef HAVE_GNOME
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW,	"Ver Antes",	tb_print_preview_xpm,	NULL,	"Ver o documento antes de o imprimir")
#endif

	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Desfazer",	tb_undo_xpm,		NULL,	"Desfazer a edi��o")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"Refazer",	tb_redo_xpm,		NULL,	"Refazer a edi��o")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Corta",	tb_cut_xpm,		NULL,	"Cortar")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"Copiar",	tb_copy_xpm,		NULL,	"Copiar")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Colar",	tb_paste_xpm,		NULL,	"Colar")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"Estilo",	NoIcon,			NULL,	"Estilo")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"Tipo",		NoIcon,			NULL,	"Tipo da letra")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Tamanho",	NoIcon,			NULL,	"Tamanho do tipo da letra")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"Negrito",	tb_text_bold_xpm,	NULL,	"Negrito")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"It�lico",	tb_text_italic_xpm,	NULL,	"It�lico")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Sublinhado",	tb_text_underline_S_xpm,NULL,	"Sublinhado")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Sobrelinhado",	tb_text_overline_xpm,	NULL,	"Sobrelinhado")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"Cortado",	tb_text_strikeout_xpm,	NULL,	"Cortado")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"Expoente",	tb_text_superscript_xpm,NULL,	"Expoente")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"�ndice",	tb_text_subscript_xpm,	NULL,	"�ndice")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,	"S�mbolo",	tb_symbol_xpm,		NULL,	"Inserir s�mbolo")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"� Esquerda",	tb_text_align_left_xpm,	NULL,	"Alinhar � esquerda")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"Ao Centro",	tb_text_center_xpm,	NULL,	"Centrar")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"� Direita",	tb_text_align_right_xpm,NULL,	"Alinhar � direita")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Justificar",	tb_text_justify_xpm,	NULL,	"Justificar par�grafo")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Nada antes",	tb_para_0before_xpm,	NULL,	"Espa�amento anterior: nenhum")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 pt antes",	tb_para_12before_xpm,	NULL,	"Espa�amento anterior: 12 pt")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Espa�amento simples",	tb_line_single_space_xpm,	NULL,"Espa�amento simples")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"Espa�amento 1,5",	tb_line_middle_space_xpm,	NULL,"Espa�amento 1,5")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Espa�amento duplo",	tb_line_double_space_xpm,	NULL,"Espa�amento duplo")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,		"1 Coluna",	tb_1column_xpm,		NULL,	"1 Coluna")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,		"2 Colunas",	tb_2column_xpm,		NULL,	"2 Colunas")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,		"3 Colunas",	tb_3column_xpm,		NULL,	"3 Colunas")

	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,		"Zoom",		NoIcon,			NULL,	"Zoom")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,	"Lista Pontuada",tb_lists_bullets_xpm,	NULL,	"Come�ar/Terminar listas pontuadas")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,	"Lista Numerada",tb_lists_numbers_xpm,	NULL,	"Come�ar/Terminar listas numeradas")
	
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,		"Cor",		NoIcon,			NULL,	"Mudar a cor")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,		"Cor de fundo",	NoIcon,			NULL,	"Mudar a cor de fundo")
	ToolbarLabel(AP_TOOLBAR_ID_INDENT,		"Indentar",	tb_text_indent_xpm,	NULL,	"Aumentar indenta��o do par�grafo")
	ToolbarLabel(AP_TOOLBAR_ID_UNINDENT,		"Desindentar",	tb_text_unindent_xpm,	NULL,	"Diminuir a indenta��o do par�grafo")

	// ... add others here ...

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,	NULL)

EndSet()
