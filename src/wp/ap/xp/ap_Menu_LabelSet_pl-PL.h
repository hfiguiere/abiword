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

// We use the Win32 '&' character to denote a keyboard accelerator on a menu item.
// If your platform doesn't have a way to do accelerators or uses a different
// character, remove or change the '&' in your menu constructor code.
// translated by: Sercxemulo <explo@poczta.wp.pl>
// last update: 07.14.2000A.D.
// If the third argument is true, then this is the fall-back for
// this language (named in the first argument).

BeginSet(pl,PL,true)

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,				"&Plik",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"&Nowy", 			"Stw�rz nowy dokument")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"&Otw�rz",			"Otw�rz istniej�cy dokument")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"&Zamknij", 			"Zamknij dokument")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"Z&apisz", 			"Zapisz dokument")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"Zapisz &Jako", 		"Zachowaj dokument pod inn� nazw�")
	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,	"Opcje D&ruku",		"Zmie� ustawienia wydruku")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"&Drukuj",			"Drukuj ca�o�� lub cz�� dokumentu")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",			"Otw�rz ten dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",			"Otw�rz ten dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",			"Otw�rz ten dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",			"Otw�rz ten dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",			"Otw�rz ten dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",			"Otw�rz ten dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",			"Otw�rz ten dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",			"Otw�rz ten dokument")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",			"Otw�rz ten dokument")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"&Koniec", 			"Zamknij wszystkie okna robocze i zako�cz")

	MenuLabel(AP_MENU_ID_EDIT,				"&Edit",			NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"&Cofnij",			"Cofnij zmiany")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"&Cofnij zmiany",			"Cofnij poprawki, czyli poprzednie poprawki")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"Zachowaj i &wytnij",				"Wytnij zaznaczone i zapami�taj w Schowku")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"&Zachowaj",			"Zapami�taj w Schowku")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"&Wklej",			"Wklej zawarto�� Schowka")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"Wyc&zy��",			"Skasuj zaznaczenie")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,	"Zaznacz W&szystko",		"Select the entire document")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"&Znajd�",			"Znajd� podany tekst")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"Z&amie�",			"Zmie� tekst w innych tekstach")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"&Id� do",			"Przenie� punkt w kt�rym wpisujesz tekst do")
	
	MenuLabel(AP_MENU_ID_VIEW,				"&Widok",			NULL)
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&Pasek narz�dziowy",		NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"&Standard",		"Poka� lub schowaj standardowy pasek narz�dziowy")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,	"&Formatowanie",		"Poka� lub schowaj pasek z narz�dziami do formatowania")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"&Przymiar",			"Poka� lub schowaj przymiar")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,	"&Pasek informacyjny",		"Poka� lub schowaj pasek informacjyjny")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"Poka� Para&grafy",	"Poka� znaczki niedrukowalne")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"&Nag��wek i Stopka",	"Popraw tekst kt�ry si� uka�e na g�rze i na dole ka�dej stron")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Powi�kszenie",			"Zmniejsz i powi�ksz obszar roboczy")

	MenuLabel(AP_MENU_ID_INSERT,			"&Wstaw",			NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"&Przerwij",			"Wstaw stron�, kolumn� lub przerwij zaznaczanie")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"N&umerowanie stron",	"Wstaw automatyczne numerowanie stron")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,	"Data i &Czas",	"Wstaw dat� i/lub czas")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"&Pole",			"Wstaw wyliczone pole")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"&Symbol",			"Wstaw symbol lub inny specyficzny znak")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,	"&Obrazek",			"Wstaw obrazek z innego istniej�cego pliku")

	MenuLabel(AP_MENU_ID_FORMAT,			"F&ormatuj",			NULL)
	MenuLabel(AP_MENU_ID_FMT_FONT,			"&Czcionka",			"Change the font of the selected text")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"&Paragraf",		"Zmie� format zaznaczonego paragrafu")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"podpunkty i &Numerowanie",	"Dodaj lub zmie� wyliczenia stosowane w paragrafie")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"&Ramki& i Cienie",		"Dodaj ramk� i cie�")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"&Kolumny",			"Zmie� ilo�� kolumn")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"St&yle",			"Zdefinuj lub uaktualnij styl paragrafu")	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabulatory",			"Set tab stops")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"&Pogrubienie",			"Ustaw pogrubienie (prze��cznik)")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"&Kursywa",			"Ustaw kursyw� (prze��cznik)")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"&Podkre�lenie",		"Ustaw podkre�lenie (prze��cznik)")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"Przek&re�lenie",			"Ustaw przekre�lenie (prze��cznik)")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,	"Indeks &g�rny",		"Indeks g�rny (prze��cznik)")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"Indeks d&olny",		"Indeks dolny (prze��cznik)")

	MenuLabel(AP_MENU_ID_TOOLS,				"&Narz�dzia",			NULL)   
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"&Ortografia",		"Sprawd� ortografi� tekstu")
	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,	"&Zliczanie s��w",		"Policz ile jest s��w w dokumencie")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Opcje",			"Ustaw opcje")

	MenuLabel(AP_MENU_ID_ALIGN,				"&Wyr�wnywanie",			NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"W &lewo",			"Wyr�wnaj w lewo")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"Na �&rodku",			"Centrowanie akapitu")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"W &prawo",			"Wyr�wnaj w prawo akapit")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Justowanie",			"Justify the paragraph")

	MenuLabel(AP_MENU_ID_WINDOW,			"&Okno",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&Nowe Okno",		"Otw�rz nowe okno dla dokumentu")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",			"Poka� to okno")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",			"Poka� to okno")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",			"Poka� to okno")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",			"Poka� to okno")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",			"Poka� to okno")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",			"Poka� to okno")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",			"Poka� to okno")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",			"Poka� to okno")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",			"Poka� to okno")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&Wi�cej okien",	"Poka� ca�� list� okien")

	MenuLabel(AP_MENU_ID_HELP,				"&Pomoc",			NULL)
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&O ... %s",		"Poka� informacje o programie, wersji i prawach autorskich")

	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",				"Zmie� na t� sugerowan� fraz�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",				"Zmie� na t� sugerowan� fraz�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",				"Zmie� na t� sugerowan� fraz�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",				"Zmie� na t� sugerowan� fraz�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",				"Zmie� na t� sugerowan� fraz�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",				"Zmie� na t� sugerowan� fraz�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",				"Zmie� na t� sugerowan� fraz�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",				"Zmie� na t� sugerowan� fraz�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",				"Zmie� na t� sugerowan� fraz�")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"&Zignoruj wszystko", 		"Zignoruj wszystkie wyst�pienia tego wyrazu")
	MenuLabel(AP_MENU_ID_SPELL_ADD,			"&Dodaj", 			"Dodaj to s�owo do dadatkowego s�ownika")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
