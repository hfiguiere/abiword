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
 * 
 * Translation by SoftCatal� (jesus@softcatala.org)
 * 
 */


/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.
******************************************************************
*****************************************************************/

// We use the Win32 '&' character to denote a keyboard accelerator on a menu item.
// If your platform doesn't have a way to do accelerators or uses a different
// character, remove or change the '&' in your menu constructor code.

// If the third argument is true, then this is the fall-back for
// this language (named in the first argument).

BeginSet(ca,ES,true)

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,			"&Fitxer",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"&Nou", 			"Crea un nou document")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"&Obre",			"Obre un document existent")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"&Tanca", 			"Tanca el document")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"&Desa", 			"Desa el document")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"&Anomena i desa", 		"Desa el document amb un altre nom")

	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,		"Config&uraci� de la p�gina",	"Canvia les opcions d'impressi�")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"Im&primeix",			"Imprimeix el document o una part d'ell")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",			"Obre aquest document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",			"Obre aquest document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",			"Obre aquest document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",			"Obre aquest document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",			"Obre aquest document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",			"Obre aquest document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",			"Obre aquest document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",			"Obre aquest document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",			"Obre aquest document")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"&Surt", 			"Tanca totes les finestres de l'aplicaci� i surt")

	MenuLabel(AP_MENU_ID_EDIT,			"&Edita",			NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"&Desf�s",			"Desf�s l'edici�")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"&Ref�s",			"Ref�s l'edici� desfeta anteriorment")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"Re&talla",			"Retalla la selecci� i posa-la al porta-retalls")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"&Copia",			"Copia la selecci� i posa-la al porta-retalls")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"&Enganxa",			"Insereix el contingut del porta-retalls")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"Esborr&a",			"Suprimeix la selecci�")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,		"Selecciona-&ho tot",		"Selecciona tot el document")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"&Cerca",			"Cerca el text especificat")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"R&eempla�a",			"Reempla�a el text especificat amb un altre")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"&V�s a",			"Mou el punt d'inserci� a una ubicaci� determinada")

	MenuLabel(AP_MENU_ID_VIEW,			"&Visualitza",			NULL)
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&Barres d'eines",		NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"E&st�ndard",			"Mostra o amaga la barra d'eines est�ndard")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,		"&Formatatge",			"Mostra o amaga la barra d'eines de formatatge")
	MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,		"&Extra",			"Mostra o amaga la barra d'eines extra")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"&Regle",			"Mostra o amaga els regles")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,		"B&arra d'estat",		"Mostra o amaga la barra d'estat")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"Mostra els par�&grafs",	"Visualitza els car�cters no imprimibles")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"&Cap�alera i peu de p�gina",	"Edita el text a la part superior o inferior de cada p�gina")
	MenuLabel(AP_MENU_ID_VIEW_FULLSCREEN, 		"&Pantalla completa", 		"Visualitza el document utilitzant tota la pantalla")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Zoom",			"Redueix o amplia la visualitzaci� del document")

	MenuLabel(AP_MENU_ID_INSERT,			"&Insereix",			NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"&Salt",			"Insereix un salt de p�gina, columna o secci�")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"N&�meros de p�gina",		"Insereix autom�ticament el n�mero de p�gina")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,		"Da&ta i hora",			"Insereix la data i/o hora")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"&Camp",			"Insereix un camp calculat")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"&S�mbol",			"Insereix un s�mbol o aun altre car�cter especial")
	 MenuLabel(AP_MENU_ID_INSERT_PICTURE, 		"&Gr�fic", 			"Inserta un gr�fic")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,		"&Del fitxer",			"Insereix una imatge existent d'un altre fitxer")

	MenuLabel(AP_MENU_ID_FORMAT,			"F&ormat",			NULL)
	MenuLabel(AP_MENU_ID_FMT_FONT,			"&Tipus de lletra",		"Canvia el tipus de lletra del text seleccionat")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"&Par�graf",			"Canvia el format del par�graf seleccionat")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"Vinyetes i &numeraci�",	"Afegeix o modifica les vinyetes i la numeraci� dels par�grafs seleccionats")
	MenuLabel(AP_MENU_ID_FMT_DOCUMENT,              "&Document",            	 "Estableix les propietats de p�gina del document, per exemple la mida i els marges")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"Contorns i &ombrejat",		"Afegeix contorns i ombrejat a la selecci�")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"&Columnes",			"Canvia el nombre de columnes")
	MenuLabel(AP_MENU_ID_FMT_TOGGLECASE, 		"&Maj�scules i min�scules", 	"Canvia la caixa del text seleccionat")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"&Estil",			"Defineix o aplica un estil per a la selecci�")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabuladors",			"Estableix aturades tabulars")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"&Negreta",			"Posa la selecci� en negreta (commuta)")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"Curs&iva",			"Posa la selecci� en cursiva (commuta)")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"S&ubratllat",			"Subratlla la selecci� (commuta)")
	MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"Su&perratllat",		"Superratlla la selecci� (commuta)")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"&Barra",			"Barra la selecci� (commuta)")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,		"Supe&r�ndex",			"Converteix la selecci� en super�ndex")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"S&ub�ndex",			"Converteix la selecci� en sub�ndex")

	MenuLabel(AP_MENU_ID_TOOLS,			"&Eines",			NULL)
	MenuLabel(AP_MENU_ID_TOOLS_SPELLING, 		"Or&tografia", 			NULL)
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"O&rtografia",			"Comprova l'ortografia del document")
	MenuLabel(AP_MENU_ID_TOOLS_AUTOSPELL, 		"Comprovaci� &autom�tica", 	"Comprova l'ortografia autom�ticament")
	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,		"N�mero de &paraules", 		"Fa un recompte del n�mero de paraules que hi ha al document")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Opcions",			"Estableix les opcions")
	MenuLabel(AP_MENU_ID_TOOLS_LANGUAGE, 		"&Idioma", 			"Canvia l'idioma del text seleccionat")
	
	MenuLabel(AP_MENU_ID_ALIGN,			"&Alineaci�",			NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"&Esquerra",			"Alinea el par�graf a l'esquerra")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&Centre",			"Centra el par�graf")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"&Dreta",			"Alinea el par�graf a la dreta")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Justificada",			"Justifica el par�graf")


	MenuLabel(AP_MENU_ID_WEB_SAVEASWEB,            "Desa-ho com a &web", 		"Desa el document com una p�gina web")
     	MenuLabel(AP_MENU_ID_WEB_WEBPREVIEW,           "&Mostra-ho com a p�gina web", 	"Previsualitza la p�gina web")

	MenuLabel(AP_MENU_ID_WINDOW,			"&Finestra",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&Nova finestra",		"Obre una altra finestra per al document")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",			"Visualitza aquesta finestra")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",			"Visualitza aquesta finestra")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",			"Visualitza aquesta finestra")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",			"Visualitza aquesta finestra")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",			"Visualitza aquesta finestra")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",			"Visualitza aquesta finestra")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",			"Visualitza aquesta finestra")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",			"Visualitza aquesta finestra")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",			"Visualitza aquesta finestra")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&M�s finestres",		"Mostra la llista completa de finestres")

	MenuLabel(AP_MENU_ID_HELP,			"&Ajuda",			NULL)
	MenuLabel(AP_MENU_ID_HELP_CONTENTS,		"&Continguts",			"Mostra el contingut de l'ajuda")
	MenuLabel(AP_MENU_ID_HELP_INDEX,		"&�ndex",			"Mostra l'�ndex de l'ajuda")
	MenuLabel(AP_MENU_ID_HELP_CHECKVER,		"&Versi�",			"Mostra el n�mero de versi� del programa")
	MenuLabel(AP_MENU_ID_HELP_SEARCH,		"&Busca ajuda",			"Busca ajuda sobre...")
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&Quant a l'%s",		"Mostra informaci� del programa, versi� i drets d'autor")
	MenuLabel(AP_MENU_ID_HELP_ABOUTOS,		"Quant al programari &lliure",	"Mostra informaci� sobre el programari lliure")


	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,		"%s",				"Canvia a l'ortografia suggerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,		"%s",				"Canvia a l'ortografia suggerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,		"%s",				"Canvia a l'ortografia suggerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,		"%s",				"Canvia a l'ortografia suggerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,		"%s",				"Canvia a l'ortografia suggerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,		"%s",				"Canvia a l'ortografia suggerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,		"%s",				"Canvia a l'ortografia suggerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,		"%s",				"Canvia a l'ortografia suggerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,		"%s",				"Canvia a l'ortografia suggerida")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,		"&Ignora-les totes", 		"Ignora totes les aparicions d'aquesta paraula al diccionari")
	MenuLabel(AP_MENU_ID_SPELL_ADD,			"&Afegeix", 			"Afegeix aquesta paraula al diccionari personalitzat")

 	/* autotext submenu labels */
 	
     	MenuLabel(AP_MENU_ID_INSERT_AUTOTEXT, "&Text autom�tic", "")
     	MenuLabel(AP_MENU_ID_AUTOTEXT_ATTN, "A l'atenci� de:", "")
     	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING, "Cloenda:", "") 
     	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL, "Instruccions de correu:", "")
     	MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE, "Refer�ncia:", "")
     	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION, "Salutaci�:", "")
     	MenuLabel(AP_MENU_ID_AUTOTEXT_SUBJECT, "Assumpte:", "")
     	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL, "Correu electr�nic:", "")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
