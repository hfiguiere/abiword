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
 
// Slovenian translation provided by Andraz Tor <andraz.tori1@guest.arnes.si>


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

BeginSetEnc(sl,SI,true,"iso-8859-2")

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,				"&Datoteka",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"&Nova", 			"Ustvari nov dokument")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"&Odpri",			"Odpri obstoje� dokument")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"&Zapri", 			"Zapri dokument")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"&Shrani", 			"Shrani dokument")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"Shrani &kot", 		"Shrani dokument pod druga�nim imenom")
	MenuLabel(AP_MENU_ID_FILE_IMPORT,		"&Uvozi",			"Uvozi dokument")
	MenuLabel(AP_MENU_ID_FILE_EXPORT,       	"&Izvozi",			"Izvozi dokument")

	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,		"N&astavitev strani",		"Spremeni nastaviteve tiskanja")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"Na&tisni",			"Tiskaj celoten dokument ali njegov del")
	MenuLabel(AP_MENU_ID_FILE_PRINT_DIRECTLY,		"Tiskaj &neposredno",	"Tiskaj z uporabo internega gonilnika PostScript")
	MenuLabel(AP_MENU_ID_FILE_PRINT_PREVIEW, 		"P&redogled tiskanja", "Predogled dokumenta pred tiskanjem")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",			"Odpri ta dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",			"Odpri ta dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",			"Odpri ta dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",			"Odpri ta dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",			"Odpri ta dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",			"Odpri ta dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",			"Odpri ta dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",			"Odpri ta dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",			"Odpri ta dokument")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"Iz&hod", 			"Zapri vsa okna programa in kon�aj")

	MenuLabel(AP_MENU_ID_EDIT,			"&Uredi",			NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"&Razveljavi",			"Razveljavi urejanje")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"&Obnovi",			"Obnovi prej razveljavljeno urejanje")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"&Izre�i",				"Izre�i izbiro in jo daj na odlo�i��e")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"&Kopiraj",			"Kopiraj izbiro in jo daj na odlo�i��e")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"&Prilepi",			"Vstavi vsebino odlo�i��a")
	MenuLabel(AP_MENU_ID_EDIT_PASTE_SPECIAL, 	"Prilepi neo&blikovano",  "Vstavi neoblikovano vsebino odlo�i��a")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"Po�i&sti",			"Zbri�i izbiro")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,		"Izberi &vse",		"Izberi celoten dokument")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"I&��i",			"Poi��i dolo�eno besedilo")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"&Zamenjaj",			"Zamenjaj dolo�eno besedilo z drugim")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"Poj&di na",			"Prestavi kazalec na dolo�eno mesto")
	MenuLabel(AP_MENU_ID_EDIT_EDITHEADER,			"Uredi glavo",			"Uredi glavo trenutne strani")
	MenuLabel(AP_MENU_ID_EDIT_EDITFOOTER,			"Uredi nogo",			"Uredi nogo trenutne strani")
	
	MenuLabel(AP_MENU_ID_VIEW,			"Po&gled",			NULL)
	MenuLabel(AP_MENU_ID_VIEW_NORMAL, 		"O&bi�ajen", "Obi�ajen pogled")
	MenuLabel(AP_MENU_ID_VIEW_WEB, 			"Sp&letni pogled", "Spletni pogled")
	MenuLabel(AP_MENU_ID_VIEW_PRINT, 		"Pogled &tiskanja", "Tiskalni�ki pogled")
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&Orodjarne",		NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,			"O&bi�ajne",		"Ka�i/skrij obi�ajno orodjarno")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,			"&Oblikovanje",		"Ka�i/skrij oblikovalsko orodjarno")
	MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,			"&Dodatne",		"Ka�i/skrij dodatno orodjarno")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"&Ravnilo",			"Ka�i/skrij ravnila")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,		"Vrstica &stanja",		"Ka�i/skrij vrstico stanja")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"Ka�i o&dstavke",	"Ka�i znake, ki ozna�ujejo oblikovanje")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"&Glava in noga",	"Uredi besedilo na vrhu ali dnu vsake strani")
	MenuLabel(AP_MENU_ID_VIEW_FULLSCREEN, 		"�ez &cel zaslon", "Glej dokument �ez cel zason")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Pove�ava",		"Pove�aj ali zmanj�aj prikaz dokumenta")

	MenuLabel(AP_MENU_ID_INSERT,			"&Vstavi",			NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"&Prelom",			"Vstavi prelom strani, stolpca ali odseka")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"�t&evilke strani",	"Vstavi �tevilke strani")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,		"Datum in &uro",	"Vstavi datum in/ali uro")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"Po&lje",		"Vstavi prera�unano polje")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"Si&mbol",		"Vstavi simbol ali kak drug posebni znak")
	MenuLabel(AP_MENU_ID_INSERT_PICTURE, 		"&Sliko", 	"Vstavi sliko")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,			"&Iz datoteke",	"Vstavi obstoje�o sliko iz druge datoteke")

	MenuLabel(AP_MENU_ID_FORMAT,			"&Oblikovanje",			NULL)
	MenuLabel(AP_MENU_ID_FMT_LANGUAGE,		"&Jezik",		"Spremeni jezik izbranega besedila")
	MenuLabel(AP_MENU_ID_FMT_FONT,			"P&isava",			"Spremeni pisavo izbranega besedila")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"Odst&avek",		"Spremeni obliko izbranega odstavka")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"O&zna�evanje in o�tevil�enje",	"Dodaj ali spremeni ozna�evanje in o�tevil�enje izbranega odstavka")
	MenuLabel(AP_MENU_ID_FMT_DOCUMENT,		"&Dokument",             "Nastavi lastnosti strani dokumenta kot so velikost strani in robovi")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"Okvirji in s&en�enja",		"Izbiri dodaj robove in sen�enja")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"Sto&lpci",			"Spremeni �tevilo stolpcev")
	MenuLabel(AP_MENU_ID_FMT_TOGGLECASE,   	"Spremeni &velikost", "Spremeni velikost izbranega besedila")
	MenuLabel(AP_MENU_ID_FMT_BACKGROUND,   	"Ozad&je", "Spremeni barvo ozadja va�ega besedila")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"&Slog",			"Dolo�i ali uveljavi slog na izbiri")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabulatorji",			"Nastavi tabulatorska mesta")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"&Krepko",			"Naredi izbiro krepko (preklopi)")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"K&urzivno",			"Naredi izbiro kurzivno (preklopi)")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"Pod�&rtano",		"Naredi izbiro pod�rtano (preklopi)")
	MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"Nad&�rtano",		"Naredi izbiro nad�rtano (preklopi)")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"P&re�rtano",			"Pre�rtaj izbiro (preklopi)")
	MenuLabel(AP_MENU_ID_FMT_TOPLINE,		"�rta z&goraj",			"�rta nad izbiro (preklopi)")
	MenuLabel(AP_MENU_ID_FMT_BOTTOMLINE,	"�rta spoda&j",		"�rta pod izbiro (preklopi)")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,	"&Nadpisano",		"Naredi izbiro nadpisano (preklopi)")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"P&odpisano",		"Naredi izbiro podpisano (preklopi)")

	MenuLabel(AP_MENU_ID_TOOLS,			"O&rodja",			NULL)   
	MenuLabel(AP_MENU_ID_TOOLS_SPELLING, "�&rkovalnik", NULL)
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"Preveri �&rkovanje",		"Preveri dokument za napa�nim �rkovanjem")
	MenuLabel(AP_MENU_ID_TOOLS_AUTOSPELL, "&Samodejno �rkovanje", "Samodejno preverjaj �rkovanje v dokumentu")
	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,	"�tetje &besed",		"�tej �tevilo besed v dokumentu")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Nastavitve",		"Nastavi nastavitve")
	MenuLabel(AP_MENU_ID_TOOLS_LANGUAGE, "&Jezik", 				"Spremeni jezik izbranega besedila")
	MenuLabel(AP_MENU_ID_TOOLS_PLUGINS, "&Vti�niki",			"Upravljaj z vti�niki")
	MenuLabel(AP_MENU_ID_TOOLS_SCRIPTS, "S&kripte",				"Izvedi pomo�ne skripte")

	MenuLabel(AP_MENU_ID_ALIGN,				"&Poravnava",			NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"&Leva",			"Poravnaj odstavek levo")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&Sredi��na",			"Poravnaj odstavek na sredini")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"&Desna",			"Poravnaj odstavek desno")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Obojestranska",			"Poravnaj odstavek na obeh straneh")

	MenuLabel(AP_MENU_ID_WEB, 			"&Splet", NULL)
	MenuLabel(AP_MENU_ID_WEB_SAVEASWEB,    "Shrani kot &splet",		"Shrani dokument kot spletno stran")
	MenuLabel(AP_MENU_ID_WEB_WEBPREVIEW,   "P&redogled sletne strani", "Predogled dokumenta kot spletne strani")

	MenuLabel(AP_MENU_ID_WINDOW,			"O&kno",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&Novo okno",		"Odpri �e eno okno")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",			"Dvigni to okno")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",			"Dvigni to okno")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",			"Dvigni to okno")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",			"Dvigni to okno")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",			"Dvigni to okno")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",			"Dvigni to okno")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",			"Dvigni to okno")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",			"Dvigni to okno")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",			"Dvigni to okno")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&Ve� oken",	"Ka�i celoten seznam oken")

	MenuLabel(AP_MENU_ID_HELP,			"&Pomo�",		NULL)
	MenuLabel(AP_MENU_ID_HELP_CONTENTS,		"&Vsebina pomo�i",	"Ka�i vsebino pomo�i")
	MenuLabel(AP_MENU_ID_HELP_INDEX,		"&Kazalo pomo�i",		"Ka�i kazalo pomo�i")
	MenuLabel(AP_MENU_ID_HELP_CHECKVER,		"Ka�i &razli�ico",	"Poka�i razli�ico programa")
	MenuLabel(AP_MENU_ID_HELP_SEARCH,		"&I��i po pomo�i",	"I��i po pomo�i za...")
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&O %s",		"Poka�i razli�ico in licenco in podatke o programu") 
	MenuLabel(AP_MENU_ID_HELP_ABOUTOS,		"O O&dprtem programju",	"Poka�i podatke o odprtem programju")

	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",				"Spremeni na to predlagano �rkovanje")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",				"Spremeni na to predlagano �rkovanje")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",				"Spremeni na to predlagano �rkovanje")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",				"Spremeni na to predlagano �rkovanje")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",				"Spremeni na to predlagano �rkovanje")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",				"Spremeni na to predlagano �rkovanje")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",				"Spremeni na to predlagano �rkovanje")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",				"Spremeni na to predlagano �rkovanje")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",				"Spremeni na to predlagano �rkovanje")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"&Prezri vse", 		"Prezri vse pojavitve te besede v dokumentu")
	MenuLabel(AP_MENU_ID_SPELL_ADD,			"&Dodaj", 			"To besedo dodaj v prikrojen slovar")

	/* autotext submenu labels */
	MenuLabel(AP_MENU_ID_INSERT_AUTOTEXT, "Samodejno &besedilo", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_ATTN, "Opozorilo:", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING, "Zaklju�ek:", "") 
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL, "Po�tna navodila:", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE, "Reference:", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION, "Pozdrav:", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_SUBJECT, "Zadeva:", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL, "E-po�ta:", "")

	MenuLabel(AP_MENU_ID_AUTOTEXT_ATTN_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_ATTN_2, "%s", " ")
	
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_2, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_3, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_4, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_5, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_6, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_7, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_8, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_9, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_10, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_11, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING_12, "%s", " ")
	
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_2, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_3, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_4, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_5, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_6, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_7, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL_8, "%s", " ")
	
	MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE_2, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE_3, "%s", " ")
	
	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION_2, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION_3, "%s", " ")

	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION_4, "%s", " ")

	MenuLabel(AP_MENU_ID_AUTOTEXT_SUBJECT_1, "%s", " ")

	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_1, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_2, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_3, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_4, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_5, "%s", " ")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL_6, "%s", " ")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
