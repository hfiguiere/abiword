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

// Swedish translations provided by Henrik Berg <henrik@lansen.se>

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

BeginSetEnc(sv,SE,true,"iso-8859-1")

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,				"&Arkiv",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"&Nytt", 			"Skapar ett nytt tomt dokument")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"&�ppna",			"�ppnar ett befintligt dokument")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"St&�ng", 			"St�nger dokumentet")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"&Spara", 			"Sparar dokumentet")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"Spara so&m", 		"Sparar dokumentet med ett nytt namn")
	MenuLabel(AP_MENU_ID_FILE_IMPORT,       "&Importera",			"Importera dokumentet")
	MenuLabel(AP_MENU_ID_FILE_EXPORT,       "&Exportera",			"Spara dokumentet utan att �ndra namn")

 	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,	"Utskriftsforma&t", "�ndra inst�llningar f�r dokumentet")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"Skriv &ut",		"Skriver ut allt eller delar av dokument")
        MenuLabel(AP_MENU_ID_FILE_PRINT_DIRECTLY,   "Skriv ut &direkt",                       "Skriv ut med den interna PostScript-drivrutinen")
        MenuLabel(AP_MENU_ID_FILE_PRINT_PREVIEW, "&F�rhandsgranska", "F�rhandsgranska dokumentet vid utskrift")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",			"�ppnar detta dokumentet")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",			"�ppnar detta dokumentet")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",			"�ppnar detta dokumentet")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",			"�ppnar detta dokumentet")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",			"�ppnar detta dokumentet")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",			"�ppnar detta dokumentet")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",			"�ppnar detta dokumentet")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",			"�ppnar detta dokumentet")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",			"�ppnar detta dokumentet")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"&Avsluta", 		"St�nger programmets alla f�nster, och avslutar")

	MenuLabel(AP_MENU_ID_EDIT,				"&Redigera",		NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"&�ngra",			"�ngra inmatningen")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"&Upprepa",			"Upprepa f�reg�ende �ngrade inmatning")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"&Klipp ut",		"Klipper ut markeringen och placerar den i Urklippet")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"K&opiera",			"Kopierar markeringen och placerar den i Urklippet")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"K&listra in",		"Infogar inneh�llet i Urklipp")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"Klistra in spe&cial",		"Infogar inneh�llet i Urklipp oformaterat")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"&Radera",			"Raderar markeringen")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,	"&Markera allt",	"Markerar hela dokumentet")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"&S�k",				"S�ker efter angiven text")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"&Ers�tt",			"Ers�tter angiven text med annan text")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"&G� till",			"G� till en ny plats i dokumentet")
        MenuLabel(AP_MENU_ID_EDIT_EDITHEADER,                   "Redigera sid&huvud",                "Redigera text vid dokumenthuvud")
        MenuLabel(AP_MENU_ID_EDIT_EDITFOOTER,                   "Redigera sid&fot",                "Redigera text vid dokumentfot")
     
	MenuLabel(AP_MENU_ID_VIEW,				"Vi&sa",			NULL)
     MenuLabel(AP_MENU_ID_VIEW_NORMAL, "&Normall�ge", "Normall�ge")
     MenuLabel(AP_MENU_ID_VIEW_WEB, "&Webblayoutl�ge", "Webblayoutl�ge")
     MenuLabel(AP_MENU_ID_VIEW_PRINT, "&Utskriftslayoutl�ge", "Utskriftslayoutl�ge")
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&Verktygsf�lt",	NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"&Standard",		"Visar eller d�ljer verktygsf�ltet Standard")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,	"&Formatera",		"Visar eller d�ljer verktygsf�ltet Formatera")
	MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,		"&Extra",			"Visar eller d�ljer verktygsf�ltet Extra")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"&Linjal",			"Visar eller d�ljer linjalen")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,	"S&tatusrad",		"Visar eller d�ljer statusraden")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"&Visa alla",		"Visar alla icke utskrivbara tecken")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"&Sidhuvud och sidfot",	"Redigera text vid huvud eller fot p� varje sida")
        MenuLabel(AP_MENU_ID_VIEW_FULLSCREEN, "&Helsida", "Visa dokumentet som helsida")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Zooma",			"Minska eller f�rstora visningen av dokumentet")

	MenuLabel(AP_MENU_ID_INSERT,			"&Infoga",			NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"&Brytning",		"Infogar sid-, spalt-, eller styckebrytning")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"Sid&nummer",		"Infogar sidnumrering som uppdateras automatisk")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,	"Datum och &tid",	"Infogar dagens datum och/eller tid")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"&F�lt",			"Infogar ett automatiskt f�lt")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"Sy&mbol",			"Infogar en symbol eller annat specialtecken")
	MenuLabel(AP_MENU_ID_INSERT_PICTURE, 	"B&ild", 		"Infoga en bild")
	MenuLabel(AP_MENU_ID_INSERT_CLIPART, 	"&ClipArt", 		"Infoga clipart")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,	"&Bildobjekt",		"Infogar en ny bild fr�n en annan fil")

	MenuLabel(AP_MENU_ID_FORMAT,			"&Format",			NULL)
	MenuLabel(AP_MENU_ID_FMT_LANGUAGE,		"&Spr�k",		"�ndra spr�k p� den markerade texten")
	MenuLabel(AP_MENU_ID_FMT_FONT,			"T&ecken",			"V�ljer teckensnitt f�r aktuell markering")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"&Stycke",			"Formaterar aktuellt eller markerade stycken")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"Punkter och &numrering",	"Infogar eller �ndrar punkter och numrering f�r markerade stycken")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"Kantlinjer och f&yllning",	"Infogar kantlinjer och fyllning till aktuell markering")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"S&palter",			"�ndrar antalet spalter")
	MenuLabel(AP_MENU_ID_FMT_DOCUMENT,		"&Dokument",             "�ndrar inst�llningar f�r dokumentet s� som pappersstorlek och marginaler")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"Format&mall", "Definiera och anv�nda formatmallar")
	MenuLabel(AP_MENU_ID_FMT_TOGGLECASE,   	"&�ndra skiftl�ge", "�ndrar skiftl�get p� den markerade texten")
	MenuLabel(AP_MENU_ID_FMT_BACKGROUND,   	"Bak&grund", "�ndrar dokumentets bakgrundsf�rg")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabbar",			"St�ller in tabbar")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"&Fet",				"G�r aktuell markering fetstil (v�xlande)")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"&Kursiv",			"G�r aktuell markering kursiv (v�xlande)")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"&Understruken",	"G�r aktuell markering understruken (v�xlande)")
	MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"&�verstruken",		"G�r aktuell markering �verstruken (v�xlande)")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"&Genomstruken",	"G�r aktuell markering genomstruken (v�xlande)")
	MenuLabel(AP_MENU_ID_FMT_TOPLINE,		"�vre kantlinje",			"Linje ovanf�r markeringen (v�xlande)")
	MenuLabel(AP_MENU_ID_FMT_BOTTOMLINE,	"Undre kantlinje",		"Linje under markeringen (v�xlande)")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,	"Upp&h�jd",			"G�r aktuell markering upph�jd (v�xlande)")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"Neds&�nkt",		"G�r aktuell markering neds�nkt (v�xlande)")

	MenuLabel(AP_MENU_ID_TOOLS,				"&Verktyg",			NULL)
        MenuLabel(AP_MENU_ID_TOOLS_SPELLING, "&Stavning", NULL)
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"&Stavning",		"Kontroller stavningen i dokumentet")
        MenuLabel(AP_MENU_ID_TOOLS_AUTOSPELL, "&Automatisk Stavningskontroll","Automatisk stavningskontroll av dokumentet")
 	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,	"&Ordstatistik",	"R�kna antal ord, rader, paragrafer, osv i dokumentet")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Alternativ",		"Anger alternativ")
        MenuLabel(AP_MENU_ID_TOOLS_LANGUAGE, "S&pr�k", "�ndra spr�k p� den markerade texten")

	MenuLabel(AP_MENU_ID_ALIGN,				"&Justering",		NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"&V�nster",			"V�nsterjustera aktuellt eller markerade stycken")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&Centrerat",		"Centrera aktuellt eller markerade stycken")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"&H�ger",			"H�gerjustera aktuellt eller markerade stycken")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Marginaler",		"Marginaljustera aktuellt eller markerade stycken")

	MenuLabel(AP_MENU_ID_WEB, "&Webb", NULL)
        MenuLabel(AP_MENU_ID_WEB_SAVEASWEB,               "&Spara som webbsida",   "Spara dokumentet som webbsida")
        MenuLabel(AP_MENU_ID_WEB_WEBPREVIEW,              "&F�rhandsgranska som webbsida",    "F�rhandsgranska dokumentet som webbsida")

	MenuLabel(AP_MENU_ID_WINDOW,			"F&�nster",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&Nytt f�nster",	"�ppnar ett annat f�nster f�r dokumentet")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",			"Aktiverar detta f�nstret")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",			"Aktiverar detta f�nstret")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",			"Aktiverar detta f�nstret")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",			"Aktiverar detta f�nstret")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",			"Aktiverar detta f�nstret")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",			"Aktiverar detta f�nstret")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",			"Aktiverar detta f�nstret")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",			"Aktiverar detta f�nstret")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",			"Aktiverar detta f�nstret")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&Fler f�nster",	"Visa hela lista med f�nster")

	MenuLabel(AP_MENU_ID_HELP,				"&Hj�lp",			NULL)
	MenuLabel(AP_MENU_ID_HELP_CONTENTS,		"Hj�lp &Contents",	"Visar hj�lp Contents")
	MenuLabel(AP_MENU_ID_HELP_INDEX,		"Hj�lp &index",		"Visar hj�lp Index")
	MenuLabel(AP_MENU_ID_HELP_CHECKVER,		"Visa &version",	"Visar programmets versionsnummer")
	MenuLabel(AP_MENU_ID_HELP_SEARCH,		"&Search for Help",	"S�ker i hj�lpen")
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&Om %s",			"Visar programinformation, versionsnummer, och copyright")
	MenuLabel(AP_MENU_ID_HELP_ABOUTOS,		"About &Open Source",	"Visar information om Open Source")

	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",				"�ndrar till f�reslagen stavning")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",				"�ndrar till f�reslagen stavning")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",				"�ndrar till f�reslagen stavning")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",				"�ndrar till f�reslagen stavning")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",				"�ndrar till f�reslagen stavning")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",				"�ndrar till f�reslagen stavning")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",				"�ndrar till f�reslagen stavning")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",				"�ndrar till f�reslagen stavning")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",				"�ndrar till f�reslagen stavning")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"&Ignorera alla", 	"Ignorera alla f�rekomster av detta ord i dokumentet")
	MenuLabel(AP_MENU_ID_SPELL_ADD,			"&L�gg till", 		"L�gg till detta ord till den egna ordlistan")

     /* autotext submenu labels */
     /* stupid stupid autotext */

     MenuLabel(AP_MENU_ID_INSERT_AUTOTEXT, "&Autotext", "")
     MenuLabel(AP_MENU_ID_AUTOTEXT_ATTN, "Observera:", "")
     MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING, "Avslutningsfraser:", "") 
     MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL, "Brevkategorier:", "")
     MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE, "Ang�ende:", "")
     MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION, "H�lsningsfraser:", "")
     MenuLabel(AP_MENU_ID_AUTOTEXT_SUBJECT, "�rende:", "")
     MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL, "Epost:", "")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
