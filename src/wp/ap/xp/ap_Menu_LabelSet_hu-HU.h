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

// We use the Win32 '&' character to denote a keyboard accelerator on a menu item.
// If your platform doesn't have a way to do accelerators or uses a different
// character, remove or change the '&' in your menu constructor code.

// If the second argument is true, then this is the fall-back for
// this language (named in the first two letters).

BeginSet(hu,HU,true)

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,				"&F�jl",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"�&j", 			"�j dokumentum l�trehoz�sa")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"Meg&nyit�s",			"Dokumentum megnyit�sa")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"&Bez�r�s", 			"Dokumentum bez�r�sa")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"&Ment�s", 			"Dokumentum ment�se")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"Ment�s &m�sk�nt", 		"Dokumentum ment�se m�s n�ven")
	MenuLabel(AP_MENU_ID_FILE_SAVEASWEB,               "Ment�s &web oldalk�nt", "Dokumentum ment�se web oldalk�nt")
	MenuLabel(AP_MENU_ID_FILE_WEBPREVIEW,              "Web oldal el�nezet", "Dokumentum el�n�zete web oldalk�nt")
	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,	"&Oldalbe�ll�t�s",		"Nyomtat�si be�ll�t�sok")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"&Nyomtat�s",			"A teljes dokumentum, vagy egy r�sz�nek nyomtat�sa")
	MenuLabel(AP_MENU_ID_FILE_PRINT_DIRECTLY,		"K�zvetlen nyomtat�s",			"Nyomtat�s a be�p�tett PS nyomtat�meghajt�val")
	MenuLabel(AP_MENU_ID_FILE_PRINT_PREVIEW, "Nyomtat�si k�p", "A dokumentum megtekint�se nyomtat�s el�tt")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",			"Ennek a dokumentumnak a megnyit�sa")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",			"Ennek a dokumentumnak a megnyit�sa")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",			"Ennek a dokumentumnak a megnyit�sa")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",			"Ennek a dokumentumnak a megnyit�sa")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",			"Ennek a dokumentumnak a megnyit�sa")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",			"Ennek a dokumentumnak a megnyit�sa")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",			"Ennek a dokumentumnak a megnyit�sa")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",			"Ennek a dokumentumnak a megnyit�sa")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",			"Ennek a dokumentumnak a megnyit�sa")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"&Kil�p�s", 			"Az alkalmaz�s �sszes ablak�nak bez�r�sa �s kil�p�s")

	MenuLabel(AP_MENU_ID_EDIT,				"&Szerkeszt�s",			NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"&Visszavon",			"Szerkeszt�s visszavon�sa")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"�j&ra",			"Az el�z� visszavon�s �jra")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"&Kiv�g�s",				"A kijel�lt r�szt kiv�gja, �s a v�g�lapra helyezi")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"&M�sol�s",			"A kijel�lt r�szt a v�g�lapra m�solja")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"&Beilleszt�s",			"A v�g�lap tartalm�t besz�rja")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"&T�rl�s",			"A kijel�l�s t�rl�se")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,	"Mindent kijel�&l",		"A teljes dokumentum kijel�l�se")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"Kere&s",			"A megadott sz�veg keres�se")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"Cs&er�l",			"A megadott sz�veg cser�je m�s sz�vegre")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"U&gr�s",			"A kurzor �thelyez�se egy megadott helyre")
	MenuLabel(AP_MENU_ID_EDIT_EDITHEADER,			"Fejl�c",			"Az aktu�lis oldal fejl�c�nek szerkeszt�se")
	MenuLabel(AP_MENU_ID_EDIT_EDITFOOTER,			"L�bl�c",			"Az aktu�lis oldal l�bl�c�nek szerkeszt�se")
	
	MenuLabel(AP_MENU_ID_VIEW,				"&N�zet",			NULL)
	MenuLabel(AP_MENU_ID_VIEW_NORMAL, "&Norm�l", "Norm�l n�zet")
	MenuLabel(AP_MENU_ID_VIEW_WEB, "&Web elrendez�s", "Web elrendez�s")
	MenuLabel(AP_MENU_ID_VIEW_PRINT, "Nyomtat�si &elrendez�s", "Nyomtat�si elrendez�s")
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&Eszk�zt�rak",		NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"&Szabv�ny",		"A szabv�ny eszk�zt�r megjelen�t�se vagy elrejt�se")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,	"&Form�z�s",		"A form�z�s eszk�zt�r megjelen�t�se vagy elrejt�se")
	MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,		"E&xtra",			"Az extra eszk�zt�r megjelen�t�se vagy elrejt�se")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"&Vonalz�",			"A vonalz�k megjelen�t�se vagy elrejt�se")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,	"�llapot&sor",		"Az �llapotsor megjelen�t�se vagy elrejt�se")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"Bekezd�sjelek",	"A nem nyomtatott karakterek megjelen�t�se")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"&Fejl�c �s l�bl�c",	"Az oldalak tetej�n �s alj�n l�v� sz�veg szerkeszt�se")
	MenuLabel(AP_MENU_ID_VIEW_FULLSCREEN, "&Teljes k�perny�", "A dokumentum n�zete a teljes k�perny�n")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Nagy�t�s",			"A dokumentum n�zet nagy�t�sa vagy kicsiny�t�se")

	MenuLabel(AP_MENU_ID_INSERT,			"&Besz�r�s",			NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"T�r�spont",			"Oldal-, Has�b-, vagy Szekci�t�r�s besz�r�sa")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"&Oldalsz�m",	"Automatikusan friss�tett oldalsz�m besz�r�sa")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,	"&D�tum �s id�",	"D�tum �s/vagy id� besz�r�sa")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"&Mez�",			"Sz�m�tott mez� besz�r�sa")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"&Szimb�lum",			"Szimb�lum vagy speci�lis karakter besz�r�sa")
	MenuLabel(AP_MENU_ID_INSERT_PICTURE, "K�&p", "K�p besz�r�sa")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,	"&F�jlb�l",		"Megl�v� k�p besz�r�sa f�jlb�l")

	MenuLabel(AP_MENU_ID_FORMAT,			"F&orm�tum",			NULL)
	MenuLabel(AP_MENU_ID_FMT_LANGUAGE,		"&Nyelv",		"A kijel�lt sz�veg nyelv�nek megv�ltoztat�sa")
	MenuLabel(AP_MENU_ID_FMT_FONT,			"&Bet�",			"A kijel�lt sz�vegr�sz bet�t�pus�nak megv�ltoztat�sa")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"Be&kezd�s",		"A kijel�lt bekezd�s form�z�sa")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"Fel&sorol�s",	"Felsorol�s hozz�rendel�se vagy m�dos�t�sa kijel�lt bekezd�sekhez")
	MenuLabel(AP_MENU_ID_FMT_DOCUMENT,              "&Dokumentum",             "A dokumentum jellemz�inek (pl. oldalm�ret, marg�vastags�g) beall�t�sa")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"Sze&g�ly �s �rny�k",		"Szeg�ly �s �rny�k hozz�rendel�se a kijel�lt r�szhez")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"&Has�bok",			"A has�bok sz�m�nak megv�ltoztat�sa")
	MenuLabel(AP_MENU_ID_FMT_TOGGLECASE, "&Kis- �s nagybet�k", "Kis- �s nagybet�k m�dos�t�sa a kijel�lt sz�vegben")
	MenuLabel(AP_MENU_ID_FMT_BACKGROUND, "H�tt�r", "A dokumentum h�tt�rsz�n�nek megv�ltoztat�sa")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"S&t�lus",			"St�lus megad�sa vagy alkalmaz�sa a kijel�lt r�szhez")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabul�torok",			"Tabul�torok be�ll�t�sa")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"&F�lk�v�r",			"A kijel�lt r�sz f�lk�v�rr� t�tele (megford�t�s)")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"&D�lt",			"A kijel�lt r�sz d�ltt� t�tele (megford�t�s)")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"&Al�h�z�s",		"A kijel�lt r�sz al�h�z�sa (megford�t�s)")
	MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"Fel�lvon�s",		"A kijel�lt r�sz fel�lvon�sa (megford�t�s)")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"Kih�z�s",			"A kijel�lt r�sz kih�z�sa (megford�t�s)")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,	"Fels� index",		"A kijel�lt r�sz fels� indexsz� t�tele (megford�t�s)")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"Als� index",		"A kijel�lt r�sz als� indexsz� t�tele (megford�t�s)")

	MenuLabel(AP_MENU_ID_TOOLS,		"&Eszk�z�k",		NULL)
	MenuLabel(AP_MENU_ID_TOOLS_SPELLING, "&Helyes�r�s", NULL)
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"&Ellen�rz�s",		"A dokumentum helyes�r�s�nak ellen�rz�se")
	MenuLabel(AP_MENU_ID_TOOLS_AUTOSPELL, "&Automatikus", "A dokumentum helyes�r�s�nak automatikus ellen�rz�se")
	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,		"&Szavak sz�ma",		"A dokumentum szavainak megsz�ml�l�sa")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Be�ll�t�sok",			"Be�ll�t�sok szerkeszt�se")
	MenuLabel(AP_MENU_ID_TOOLS_LANGUAGE,		"&Nyelv",		"A kijel�lt sz�veg nyelv�nek megv�ltoztat�sa")

	MenuLabel(AP_MENU_ID_ALIGN,				"&Igaz�t�s",			NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"&Balra",			"A bekezd�s igaz�t�sa balra")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&K�z�pre",			"A bekezd�s igaz�t�sa k�z�pre")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"&Jobbra",			"A bekezd�s igaz�t�sa jobbra")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Sorkiegyenl�t�s",			"A bekezd�s sorkiegyenl�t�se")

	MenuLabel(AP_MENU_ID_WINDOW,			"&Ablak",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"�&j ablak",		"�j ablak nyit�sa a dokumentumnak")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",			"Ezt az ablakot mutassa")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",			"Ezt az ablakot mutassa")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",			"Ezt az ablakot mutassa")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",			"Ezt az ablakot mutassa")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",			"Ezt az ablakot mutassa")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",			"Ezt az ablakot mutassa")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",			"Ezt az ablakot mutassa")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",			"Ezt az ablakot mutassa")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",			"Ezt az ablakot mutassa")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&Tov�bbi ablakok",	"Az ablakok teljes list�ja")

	MenuLabel(AP_MENU_ID_HELP,				"S�&g�",			NULL)
	MenuLabel(AP_MENU_ID_HELP_CONTENTS,		"S�g� &Tartalom",	"S�g� tartalm�nak megjelen�t�se")
	MenuLabel(AP_MENU_ID_HELP_INDEX,		"S�g� Tartalomjegyz�k",		"S�g� tartalomjegyz�k�nek megjelen�t�se")
	MenuLabel(AP_MENU_ID_HELP_CHECKVER,		"&Verzi�sz�m",	"A program verzi�sz�m�nak megjelen�t�se")
	MenuLabel(AP_MENU_ID_HELP_SEARCH,		"S�g� &Keres�se",	"S�g� keres�se...")
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"%s &N�vjegy",		"Program inform�ci�k, verzi�sz�m �s copyright megjelen�t�se")
	MenuLabel(AP_MENU_ID_HELP_ABOUTOS,		"A &Szabad Forr�sk�dr�l",	"Inform�ci� a szabad forr�sk�dr�l")

	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",				"Cser�lje erre a javaslatra")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",				"Cser�lje erre a javaslatra")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",				"Cser�lje erre a javaslatra")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",				"Cser�lje erre a javaslatra")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",				"Cser�lje erre a javaslatra")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",				"Cser�lje erre a javaslatra")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",				"Cser�lje erre a javaslatra")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",				"Cser�lje erre a javaslatra")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",				"Cser�lje erre a javaslatra")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"M&indet Kihagy", 		"Hagyja figyelmen k�v�l a sz� dokumentumbeli �sszes el�fordul�s�t")
	MenuLabel(AP_MENU_ID_SPELL_ADD,			"&Felvesz", 			"Vegye fel ezt a sz�t az egyedi sz�t�rba")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
