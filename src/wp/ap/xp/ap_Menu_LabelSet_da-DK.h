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

// Danish translations provided by Martin Willemoes Hansen <mwh@stampede.org>
//                             , Birger Langkjer <birger.langkjer@image.dk>
//                             and Rasmus Toftdahl Olesen <rto@post.tele.dk>

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

BeginSetEnc(da,DK,true,"iso-8859-1")

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)
        MenuLabel(AP_MENU_ID_TOOLS_AUTOSPELL, "&Auto stavekontrol", "Automatisk stavekontrol af dokumentet")
        MenuLabel(AP_MENU_ID_VIEW_FULLSCREEN, "&Fuldsk�rmsvisning", "Vis i fuldsk�rmstilstand")
        MenuLabel(AP_MENU_ID_TOOLS_SPELLING, "&Stavning", NULL)
        MenuLabel(AP_MENU_ID_FMT_DOCUMENT,              "&Dokument",             "Indstil egenskaber for dokument")
	MenuLabel(AP_MENU_ID_FILE_PRINT_DIRECTLY,		"Udskriv direkte",			"Udskriv via den interne PS-driver")
	MenuLabel(AP_MENU_ID_FILE,				"&Filer",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"&Ny", 				"Nyt dokument")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"�&bn",				"�bn et eksisterende dokument")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"&Luk", 			"Luk dokumentet")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"&Gem", 			"Gem dokumentet")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"Gem &som", 			"Gem dokumentet under et andet navn")
	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,		"&Sideops�tning",		"�ndr sideops�tning")
        MenuLabel(AP_MENU_ID_FILE_PRINT_PREVIEW, "Vis udskrift", "Vis dokumentet f�r den udskrives")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"&Udskriv",			"Udskriv hele dokumentet")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",			"�bn dette dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",			"�bn dette dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",			"�bn dette dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",			"�bn dette dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",			"�bn dette dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",			"�bn dette dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",			"�bn dette dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",			"�bn dette dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",			"�bn dette dokument")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"&Afslut", 			"Luk alle vinduer og afslut programmet")

	MenuLabel(AP_MENU_ID_EDIT,				"&Redig�r",			NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"F&ortryd",			"Fortryd �ndring")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"&Gentag",			"Gentag sidste fortrydte �ndring")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"K&lip",			"Udklip det markerede og put det i udklipsholderen")
    MenuLabel(AP_MENU_ID_EDIT_COPY,			"&Kopi�r",			"Kopi�r det markerede og put det i udklipsholderen")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"&Inds�t",			"Inds�t udklipsholderens indhold")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"&Ryd",				"Fjern det markerede")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,		"&Mark�r alt",		"Mark�r hele dokumentet")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"&S�g",				"S�g efter en angivet tekst")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"&Erstat",			"Erstat en angivet tekst med en anden tekst")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"G� &til",			"Flyt mark�ren til en angivet placering")
	
	MenuLabel(AP_MENU_ID_VIEW,				"&Vis",				NULL)
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&V�rkt�jslinjer",				NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"&Standard",			"Vis eller skjul standard v�rkt�jslinjen")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,		"&Formatering",			"Vis eller skjul formateringsv�rkt�jslinjen")
	MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,		"&Ekstra",			"Vis eller skjul den ekstra v�rkt�jslinje")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"&Lineal",			"Vis eller skjul linealerne")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,		"S&tatuslinje",			"Vis eller skjul statuslinjen")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"Vis &afsnit",		"Vis tegn der ikke skal udskrives")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"Side&hoved og sidefod",	"Redig�r tekst i toppen eller i bunden af hver side")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Zoom",			"Formindsk eller forst�r dokumentet p� sk�rmen")

	MenuLabel(AP_MENU_ID_INSERT,				"&Inds�t",			NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"&Skift",			"Inds�t et side-, r�kke- eller sektionsskift")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"Side&tal",			"Inds�t et automatisk opdateret sidetal")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,		"&Dato og klokkesl�t",		"Inds�t datoen og/eller klokkesl�t")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"&Felt",			"Inds�t et kalkuleret felt")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"Sy&mbol",			"Inds�t et symbol eller andre specielle tegn")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,		"&Billede",			"Inds�t et billede fra en fil")

	MenuLabel(AP_MENU_ID_FORMAT,				"F&ormat�r",			NULL)
	MenuLabel(AP_MENU_ID_FMT_FONT,			"&Skrifttype",			"�ndr skrifttype for den markerede tekst")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"&Afsnit",			"�ndr formatet for det markerede afsnit")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"&Punktopstilling",		"Tilf�j eller �ndr punktopstillingen i det markerede afsnit")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"&Rammer og skygge",		"Tilf�j rammer og skygge til den markerede tekst")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"Ko&lonner",			"�ndr antallet af kolonner")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"St&il",			"Angiv eller tilf�j stil for den markerede tekst")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabulatorer",			"S�t tabulatorstop")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"&Fed",				"G�r den markerede tekst fed")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"&Kursiv",			"G�r den markerede tekst kursiv")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"&Understreg",		"Understreg den markerede tekst")
	MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"&Overstreg",		"Overstreg den markerede tekst")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"&Gennemstreg",		"Gennemstreg den markerede tekst")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,		"&H�vet",		"G�r den markerede tekst h�jere end omgivelserne")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"S�&nket",		"G�r den markerede tekst lavere end omgivelserne")

	MenuLabel(AP_MENU_ID_TOOLS,				"V�r&kt�jer",			NULL)   
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"&Stavekontrol",	"Kontroll�r dokumentet for forkert stavede ord")
	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,	"O&rdopt�lling",		"Opt�l antallet af ord i dokumentet")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Indstillinger",			"Ops�t programmet...")

	MenuLabel(AP_MENU_ID_ALIGN,				"&Justering",			NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"&Venstrestillet",		"Venstrestil afsnittet")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&Centreret",			"Centr�r afsnittet")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"&H�jrestillet",		"H�jrestil afsnittet")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Lige marginer",		"Lav lige marginer p� afsnittet")

	MenuLabel(AP_MENU_ID_WINDOW,				"Vi&ndue",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&Nyt vindue",			"�bn et nyt vindue for dokumentet")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",			"Hent dette vindue")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",			"Hent dette vindue")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",			"Hent dette vindue")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",			"Hent dette vindue")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",			"Hent dette vindue")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",			"Hent dette vindue")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",			"Hent dette vindue")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",			"Hent dette vindue")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",			"Hent dette vindue")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&Flere vinduer",		"Vis en komplet liste over vinduer")

	MenuLabel(AP_MENU_ID_HELP,				"&Hj�lp",		NULL)
	MenuLabel(AP_MENU_ID_HELP_CONTENTS,		"&Indhold af hj�lp",	"Vis indhold af hj�lp")
	MenuLabel(AP_MENU_ID_HELP_INDEX,		"Hj�lpe&indeks",		"Vis hj�lpeindeks")
	MenuLabel(AP_MENU_ID_HELP_CHECKVER,		"Tjek &version",	"Vis programversionsnummer")
	MenuLabel(AP_MENU_ID_HELP_SEARCH,		"&S�g i hj�lp",	"Vis programinformation, versionsnummer og ophavsret")
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&Om %s",			"Vis programinformation, versionsnummer og ophavsret") 
	MenuLabel(AP_MENU_ID_HELP_ABOUTOS,		"Om &Open Source",	"Vis information om Open Source")


	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",				"�ndr til det foresl�ede ord")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",				"�ndr til det foresl�ede ord")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",				"�ndr til det foresl�ede ord")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",				"�ndr til det foresl�ede ord")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",				"�ndr til det foresl�ede ord")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",				"�ndr til det foresl�ede ord")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",				"�ndr til det foresl�ede ord")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",				"�ndr til det foresl�ede ord")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",				"�ndr til det foresl�ede ord")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"&Ignor�r alle", 		"Ignor�r alle forekomster af dette ord i dokumentet")
	MenuLabel(AP_MENU_ID_SPELL_ADD,			"&Tilf�j", 			"Tilf�j dette ord til din personlige ordliste")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()










