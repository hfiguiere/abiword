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

/* Galician translation by Jesus Bravo Alvarez <jba@pobox.com>
 * Last Revision: May 16, 2000
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

// If the third argument is UT_TRUE, then this is the fall-back for
// this language (named in the first argument).

BeginSet(gl,ES,UT_TRUE)

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,				"&Ficheiro",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"&Novo", 			"Crear un novo documento")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"&Abrir",			"Abrir un documento existente")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"&Pechar", 			"Pecha-lo documento")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"&Gardar", 			"Garda-lo documento")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"Gardar &Como", 		"Garda-lo documento cun nome diferente")
	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,	"Configuraci�n da P�&xina",		"Cambia-las opci�ns de impresi�n")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"&Imprimir",			"Imprimi-lo documento enteiro ou unha parte")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",			"Abrir este documento")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",			"Abrir este documento")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",			"Abrir este documento")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",			"Abrir este documento")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",			"Abrir este documento")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",			"Abrir este documento")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",			"Abrir este documento")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",			"Abrir este documento")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",			"Abrir este documento")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"&Sa�r", 			"Pechar t�dalas fiestras da aplicaci�n e sa�r")

	MenuLabel(AP_MENU_ID_EDIT,				"&Editar",			NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"&Desfacer",			"Desface-la edici�n")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"&Refacer",			"Reface-la �ltima edici�n desfeita")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"Cor&tar",			"Corta-la selecci�n e po�ela no Cartafol")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"Co&piar",			"Copia-la selecci�n e po�ela no Cartafol")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"&Pegar",			"Inseri-los contidos do Cartafol")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"&Limpar",			"Borra-la selecci�n")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,	"Seleccionar T&odo",		"Selecciona-lo documento enteiro")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"&Buscar",			"Busca-lo texto especificado")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"Re&mprazar",			"Rempraza-lo texto especificado cun texto diferente")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"&Ir A",			"Move-lo punto de inserci�n � localizaci�n indicada")
	
	MenuLabel(AP_MENU_ID_VIEW,				"&Ver",			NULL)
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&Barras de Ferramentas",		NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"&Est�ndar",		"Amosar ou agocha-la barra de ferramentas est�ndar")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,	"&Formatado",		"Amosar ou agocha-la barra de ferramentas de formatado")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"&Regra",			"Amosar ou agocha-las regras")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,	"Barra de E&stado",		"Amosar ou agocha-la barra de estado")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"Amosar &Par�grafos",	"Amosa-los caracteres non imprimibles")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"&Cabeceira e P�",	"Edita-lo texto � comezo e � remate de cada p�xina")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Escala",			"Reducir ou aumenta-la visualizaci�n do documento")

	MenuLabel(AP_MENU_ID_INSERT,			"&Inserir",			NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"&Salto",			"Inserir un salto de p�xina, columna ou secci�n")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"&N�meros de P�xinas",	"Inserir un n�mero de p�xina actualizado autom�ticamente")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,	"&Data e Hora",	"Inseri-la data e/ou a hora")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"&Campo",			"Inserir un campo calculado")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"S�&mbolo",			"Inserir un s�mbolo ou outro car�cter especial")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,	"&Imaxe",			"Inserir unha imaxe doutro ficheiro")

	MenuLabel(AP_MENU_ID_FORMAT,			"F&ormato",			NULL)
	MenuLabel(AP_MENU_ID_FMT_FONT,			"&Fonte",			"Cambia-la fonte do texto seleccionado")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"&Par�grafo",		"Cambia-lo formato do par�grafo seleccionado")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"Puntos e &Numeraci�n",	"Engadir ou modifica-los puntos e a numeraci�n nos par�grafos seleccionados")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"&Bordos e Sombras",		"Engadir bordos e sombras � selecci�n")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"&Columnas",			"Cambia-lo n�mero de columnas")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"&Estilo",			"Definir ou aplicar un estilo � selecci�n")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabuladores",			"Establece-las paradas do tabulador")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"&Negri�a",			"Troca a negri�a da selecci�n")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"Cursi&va",			"Troca a cursiva da selecci�n")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"&Subli�ado",		"Troca o subli�ado da selecci�n")
	MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"Super&li�ado",		"Troca o superli�ado da selecci�n")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"&Riscado",			"Troca o riscado da selecci�n")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,	"Super�n&dice",		"Troca o estado de super�ndice da selecci�n")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"S&ub�ndice",		"Troca o estado de sub�ndice da selecci�n")

	MenuLabel(AP_MENU_ID_TOOLS,				"Ferra&mentas",			NULL)   
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"Orto&graf�a",		"Comproba-la ortograf�a do documento")
	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,	"Conta de &Palabras",		"Conta-lo n�mero de palabras no documento")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Opci�ns",			"Establecer Opci�ns")

	MenuLabel(AP_MENU_ID_ALIGN,				"&Ali�ar",			NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"&Esquerda",			"Xustifica-lo par�grafo � esquerda")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&Centro",			"Ali�a-lo par�grafo � centro")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"&Dereita",			"Xustifica-lo par�grafo � dereita")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Xustificar",			"Xustifica-lo par�grafo")

	MenuLabel(AP_MENU_ID_WINDOW,			"Fies&tra",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&Nova Fiestra",		"Abrir outra fiestra para o documento")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",			"Elevar esta fiestra")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",			"Elevar esta fiestra")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",			"Elevar esta fiestra")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",			"Elevar esta fiestra")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",			"Elevar esta fiestra")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",			"Elevar esta fiestra")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",			"Elevar esta fiestra")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",			"Elevar esta fiestra")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",			"Elevar esta fiestra")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&M�is Fiestras",	"Amosa-la lista completa de fiestras")

	MenuLabel(AP_MENU_ID_HELP,				"A&xuda",			NULL)
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&Acerca de %s",		"Amosa-la informaci�n do programa, o n�mero de versi�n, e o copyright")

	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",				"Cambiar a esta ortograf�a suxerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",				"Cambiar a esta ortograf�a suxerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",				"Cambiar a esta ortograf�a suxerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",				"Cambiar a esta ortograf�a suxerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",				"Cambiar a esta ortograf�a suxerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",				"Cambiar a esta ortograf�a suxerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",				"Cambiar a esta ortograf�a suxerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",				"Cambiar a esta ortograf�a suxerida")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",				"Cambiar a esta ortograf�a suxerida")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"&Ignorar Todas", 		"Ignorar t�dalas aparici�ns desta palabra no documento")
	MenuLabel(AP_MENU_ID_SPELL_ADD,			"&Engadir", 			"Engadir esta palabra � diccionario personalizado")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
