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

// Czech translation provided by Zbynek Miksa <xmiksa@informatics.muni.cz>

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

BeginSet(cs,CZ,UT_TRUE)

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,			"&Soubor",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"&Nov�", 			"Vytvo�it nov� dokument")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"&Otev��t",			"Otev��t existuj�c� dokument")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"&Zav��t", 			"Zav��t dokument")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"&Ulo�it", 			"Ulo�it dokument")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"Ulo�it &jako", 		"Ulo�it dokument pod jin�m jm�nem")
	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,		"Nasta&ven� tisku ",		"Zm�nit nastaven� tisku")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"&Tisk",			"Vytisknout cel� dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",			"Otev��t tento dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",			"Otev��t tento dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",			"Otev��t tento dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",			"Otev��t tento dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",			"Otev��t tento dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",			"Otev��t tento dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",			"Otev��t tento dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",			"Otev��t tento dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",			"Otev��t tento dokument")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"&Konec", 			"Zav��t v�echny okna a ukon�it aplikaci")

	MenuLabel(AP_MENU_ID_EDIT,			"�pr&avy",			NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"&Zp�t",			"Zp�t editace")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"V&p�ed",			"Redo previously undone editing")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"&Vyjmout",			"P�esunout ozna�en� oblasti do schr�nky")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"&Kop�rovat",			"Zkop�rovat ozna�en� oblasti do schr�nky")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"V&lo�it",			"Vlo�it obsah schr�nky")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"Od&stranit",			"Smazat ozna�enou oblast")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,		"Vybr&at v�e",			"Vybrat cel� dokument")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"&Naj�t",			"Nal�zt ur�it� text")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"Nah&radit",			"Nahradit ur�it� text jin�m")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"&Skok na",			"P�esun kurzoru na po�adovanou stranu")
	
	MenuLabel(AP_MENU_ID_VIEW,				"&Zobrazit",			NULL)
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&N�stroje",		NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"&Standartn�",			"Zobrazit nebo skr�t standartn� panel n�stroj�")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,		"&Form�t",			"Zobrazit nebo skr�t panel n�stroj� pro form�tov�n�")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"&Prav�tko",			"Zobrazit nebo skr�t prav�tko")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,		"&Stavov� ��dek",		"Zobrazit nebo skr�t stavov� ��dek")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"Zobrazovat Para&graphs",	"Zobrazovat netisknuteln� znaky")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"&Z�hlav� a z�pat�",		"Editov�n� textu v z�hlav� a z�pat�")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Lupa",			"Zmen�en� nebo zv�t�en� zobrazen� dokumentu")

	MenuLabel(AP_MENU_ID_INSERT,			"&Vlo�it",			NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"&Konec",			"Vlo�it konec str�nky, sloupce nebo odstavce")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"��s&la str�nek",		"Vlo�it automatick� ��slov�n�")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,		"Datum a �as",			"Vlo�it �as a/nebo datum")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"&Pole",			"Vlo�it po��tan� pole")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"&Symbol",			"Vlo�it symbol nebo jin� speci�ln� znak")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,		"&Obr�zek",			"Vlo�it obr�zek ze souboru")

	MenuLabel(AP_MENU_ID_FORMAT,			"F&orm�t",			NULL)
	MenuLabel(AP_MENU_ID_FMT_FONT,			"&Font",			"Zm�nit font vybran�ho textu")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"&Odstavec",			"Zm�nit form�t vybran�ho odstavce")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"Odr�ky a ��slov�n�",		"P�idat nebo zm�nit odr�ky a ��slov�n� vybran�ho odstavce")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"Okraje a st�nov�n�",		"P�idat okraje a st�nov�n� vybran�mu textu")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"&Sloupce",			"Zm�nit po�et sloupc�")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"St&yl",			"Definovat nebo aplikovat styl na vybran�")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabul�tory",			"Nastaven� tabul�tor�")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"&Tu�n�",			"Nastaven� tu�n�ho p�sma pro vybranou oblast")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"&Kurz�va",			"Nastaven� kurzivy pro vybranou oblast")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"&Potr�en�",			"Podtr�en� p�sma ve vybran� oblasti")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"P�e�k&rknut�",			"P�e�krknut� p�sma ve vybran� oblasti")

	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"Pravopi&s",			"Zkontrolovat pravopis")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Mo�nosti",			"Nastaven� mo�nost�")

	MenuLabel(AP_MENU_ID_ALIGN,				"&Um�st�n�",			NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"&Vlevo",			"Zarovn�n� odstavce vlevo")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&Uprost�ed",			"Zarovn�n� odstavce uprost�ed")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"V&pravo",			"Zarovn�n� odstavce vpravo")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Do bloku",			"Zarovn�n� odstavce do bloku")

	MenuLabel(AP_MENU_ID_WINDOW,			"&Okno",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&Nov� Okno",			"Otev��t nov� okno")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",			"Zdvihnut� toho okna")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",			"Zdvihnut� toho okna")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",			"Zdvihnut� toho okna")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",			"Zdvihnut� toho okna")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",			"Zdvihnut� toho okna")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",			"Zdvihnut� toho okna")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",			"Zdvihnut� toho okna")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",			"Zdvihnut� toho okna")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",			"Zdvihnut� toho okna")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&V�ce oken",			"Zobrazit seznam v�ech oken")

	MenuLabel(AP_MENU_ID_HELP,			"N�po&v�da",			NULL)
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&O programu %s",		"Zobrazit informace o programu, ��slo verze a copyright")

	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",				"Zm�nit na tento navrhovan� pravopis")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",				"Zm�nit na tento navrhovan� pravopis")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",				"Zm�nit na tento navrhovan� pravopis")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",				"Zm�nit na tento navrhovan� pravopis")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",				"Zm�nit na tento navrhovan� pravopis")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",				"Zm�nit na tento navrhovan� pravopis")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",				"Zm�nit na tento navrhovan� pravopis")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",				"Zm�nit na tento navrhovan� pravopis")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",				"Zm�nit na tento navrhovan� pravopis")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"&Ignorovat v�e", 		"Ignorovat v�echny v�skyty tohoto slova v dokumentu")
	MenuLabel(AP_MENU_ID_SPELL_ADD,		"&P�idat", 			"P�idat toto slovo do u�ivatelsk�ho slovn�ku")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
