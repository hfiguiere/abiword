/* AbiWord
 * Copyright (C) 2000 AbiSource, Inc.
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

// Hungarian translations provided by Tamas Decsi <tamas.decsi@techie.com>

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

// If the second argument is true, then this is the fall-back for
// this language (named in the first two letters).

BeginSet(hu,HU,true)

	ToolbarLabel(AP_TOOLBAR_ID__BOGUS1__,		NULL,		NoIcon,			NULL,NULL)

	//          (id, 		                    szLabel,	IconName,     	szToolTip,      szStatusMsg)

	ToolbarLabel(AP_TOOLBAR_ID_FILE_NEW,		"�j", 		tb_new_xpm,		NULL, "�j dokumentum l�trehoz�sa")	
	ToolbarLabel(AP_TOOLBAR_ID_FILE_OPEN,		"Megnyit",		tb_open_xpm,	NULL, "Megl�v� dokumentum megnyit�sa")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVE,		"Ment", 	tb_save_xpm,	NULL, "A dokumentum ment�se")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_SAVEAS,		"Ment�s m�sk�nt", 	tb_save_as_xpm,	NULL, "A dokumentum ment�se m�s n�ven")
	ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT,		"Nyomtat",	tb_print_xpm,	NULL, "A dokumentum nyomtat�sa")
  ToolbarLabel(AP_TOOLBAR_ID_FILE_PRINT_PREVIEW,	"Nyomtat�si k�p",	tb_print_preview_xpm, NULL, "A dokumentum megtekint�se nyomtat�s el�tt")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_UNDO,		"Visszavon",		tb_undo_xpm,	NULL, "A szerkeszt�s visszavon�sa")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_REDO,		"�jra",		tb_redo_xpm,	NULL, "A szerkeszt�s ism�telt v�grehajt�sa")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_CUT,		"Kiv�g�s",		tb_cut_xpm,		NULL, "Kiv�g�s")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_COPY,		"M�sol�s",		tb_copy_xpm,	NULL, "M�sol�s")
	ToolbarLabel(AP_TOOLBAR_ID_EDIT_PASTE,		"Beilleszt�s",	tb_paste_xpm,	NULL, "Beilleszt�s")
	ToolbarLabel(AP_TOOLBAR_ID_SPELLCHECK,		"Helyes�r�s",	tb_spellcheck_xpm,		NULL, "A dokumentum helyes�r�s�nak ellen�rz�se")
	ToolbarLabel(AP_TOOLBAR_ID_IMG,				"K�p besz�r�sa",	tb_insert_graphic_xpm,	NULL, "K�p besz�r�sa a dokumentumba")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STYLE,		"St�lus",	NoIcon,			NULL, "St�lus")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_FONT,		"Bet�t�pus",		NoIcon,			NULL, "Bet�t�pus")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SIZE,		"Bet� M�ret", NoIcon,		NULL, "Bet� M�ret")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_BOLD,		"F�lk�v�r",		tb_text_bold_F_xpm,		NULL, "F�lk�v�r")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_ITALIC,		"D�lt",	tb_text_italic_D_xpm,	NULL, "D�lt")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_UNDERLINE,	"Al�h�zott",tb_text_underline_A_xpm,	NULL, "Al�h�zott")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_OVERLINE,	"Fel�lvon�s",tb_text_overline_F_xpm,	NULL, "Fel�lvon�s")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_STRIKE,		"Kih�zott",   tb_text_strikeout_K_xpm,	NULL, "Kih�zott")
	ToolbarLabel(AP_TOOLBAR_ID_HELP,			"S�g�",			tb_help_xpm,			NULL, "S�g�")

	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUPERSCRIPT,	"Fels� index",	tb_text_superscript_xpm,	NULL, "Fels� index")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_SUBSCRIPT,	"Als� index",	tb_text_subscript_xpm,		NULL, "Als� index")
	ToolbarLabel(AP_TOOLBAR_ID_INSERT_SYMBOL,	"Szimb�lum",	tb_symbol_xpm,		NULL, "Szimb�lum beilleszt�se")

	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_LEFT,		"Balra",		tb_text_align_left_xpm,		NULL, "Balra igaz�t�s")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_CENTER,	"K�z�pre",	tb_text_center_xpm,	NULL, "K�z�pre igaz�t�s")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_RIGHT,		"Jobbra",	tb_text_align_right_xpm,	NULL, "Jobbra igaz�t�s")
	ToolbarLabel(AP_TOOLBAR_ID_ALIGN_JUSTIFY,	"Sorkiegyenl�t�s",	tb_text_justify_xpm,	NULL, "A bekezd�s sorkiegyenl�t�se")

	ToolbarLabel(AP_TOOLBAR_ID_PARA_0BEFORE,	"Nincs el�tte",		tb_para_0before_xpm,	NULL, "Nincs el�tte hely")
	ToolbarLabel(AP_TOOLBAR_ID_PARA_12BEFORE,	"12 pont el�tte",		tb_para_12before_xpm,	NULL, "12 pont hely van el�tte")

	ToolbarLabel(AP_TOOLBAR_ID_SINGLE_SPACE,	"Szimpla sork�z",	tb_line_single_space_xpm,	NULL, "Szimpla sork�z")
	ToolbarLabel(AP_TOOLBAR_ID_MIDDLE_SPACE,	"1,5 sor",		tb_line_middle_space_xpm,	NULL, "1,5 sor")
	ToolbarLabel(AP_TOOLBAR_ID_DOUBLE_SPACE,	"Dupla sork�z",	tb_line_double_space_xpm,	NULL, "Dupla sork�z")

	ToolbarLabel(AP_TOOLBAR_ID_1COLUMN,			"1 Has�b",			tb_1column_xpm,			NULL, "1 Has�b")
	ToolbarLabel(AP_TOOLBAR_ID_2COLUMN,			"2 Has�b",		tb_2column_xpm,			NULL, "2 Has�b")
	ToolbarLabel(AP_TOOLBAR_ID_3COLUMN,			"3 Has�b",		tb_3column_xpm,			NULL, "3 Has�b")

	ToolbarLabel(AP_TOOLBAR_ID_VIEW_SHOWPARA,	"Mindent mutat",			tb_view_showpara_xpm,		NULL, "� (bekezd�sv�g jel) megjelen�t�se/elrejt�se")
	ToolbarLabel(AP_TOOLBAR_ID_ZOOM,			"Nagy�t�s",		NoIcon,			NULL, "Nagy�t�s")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_BULLETS,		"Felsorol�sok",		tb_lists_bullets_xpm,		NULL,		"Felsorol�sok kezd�se/lez�r�sa")
	ToolbarLabel(AP_TOOLBAR_ID_LISTS_NUMBERS,		"Sorsz�mozott List�k",		tb_lists_numbers_xpm,		NULL,		"Sorsz�mozott List�k kezd�se/lez�r�sa")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_FORE,		"Bet�sz�n",		tb_text_fgcolor_xpm,	NULL, "Bet�sz�n")
	ToolbarLabel(AP_TOOLBAR_ID_COLOR_BACK,		"Kiemel�s",		tb_text_bgcolor_xpm,	NULL, "Kiemel�s")
	ToolbarLabel(AP_TOOLBAR_ID_INDENT,			"Nagyobb beh�z�s",	tb_text_indent_xpm, 	NULL, "Nagyobb beh�z�s")
	ToolbarLabel(AP_TOOLBAR_ID_UNINDENT,		"Kisebb beh�z�s",	tb_text_unindent_xpm,	NULL, "Kisebb beh�z�s")

     // ... add others here ...
#ifdef BIDI_ENABLED
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_LTR,	"Mindenk�pp Balr�l Jobbra",		tb_text_direction_ltr_xpm,	NULL, "A sz�veg ir�ny mindenk�pp balr�l jobbra legyen")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DIR_OVERRIDE_RTL,	"Mindenk�pp Jobbr�l Balra",		tb_text_direction_rtl_xpm,	NULL, "A sz�veg ir�ny mindenk�pp jobbr�l balra legyen")
	ToolbarLabel(AP_TOOLBAR_ID_FMT_DOM_DIRECTION,		"Bekezd�s Ir�nya",	tb_text_dom_direction_rtl_xpm,	NULL, "A bekezd�s domin�ns ir�ny�nak megv�ltoztat�sa")
#endif
	ToolbarLabel(AP_TOOLBAR_ID__BOGUS2__,		NULL,		NoIcon,			NULL,NULL)

EndSet()
