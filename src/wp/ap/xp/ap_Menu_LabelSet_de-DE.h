/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
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

// German translation provided by Christian Terboven <cterboven@gmx.net>

/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.
******************************************************************
*****************************************************************/

// We use the Win32 '&' character to denote a keyboard accelerator on a menu item.
// If your platform doesn't have a way to do accelerators or uses a different
// character, remove or change the '&' in your menu constructor code.

BeginSet(DeDE)

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,				"&Datei",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"&Neu",				"Neues Dokument erstellen")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"�&ffnen",			"Vorhandenes Dokument �ffnen")
        MenuLabel(AP_MENU_ID_FILE_CLOSE,		"S&chlie�en",		"Aktuelles Dokument schlie�en")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"&Speichern",		"Aktuelles Dokument speichern")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"Speichern &unter",	"Aktuelles Dokument unter einem anderen Namen speichern")
        MenuLabel(AP_MENU_ID_FILE_PAGESETUP,	"Seite ein&richten","Layout-Optionen f�r aktuelles Dokument festlegen")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"&Drucken",			"Dokument oder Teile davon drucken")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",			"Dieses Dokument �ffnen")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",			"Dieses Dokument �ffnen")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",			"Dieses Dokument �ffnen")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",			"Dieses Dokument �ffnen")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",			"Dieses Dokument �ffnen")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",			"Dieses Dokument �ffnen")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",			"Dieses Dokument �ffnen")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",			"Dieses Dokument �ffnen")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",			"Dieses Dokument �ffnen")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"&Beenden",			"Alle Fenster schlie�en und die Anwendung beenden")

	MenuLabel(AP_MENU_ID_EDIT,				"&Bearbeiten",		NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"&R�ckg�ngig",		"Letzten Befehl r�ckg�ngig machen")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"&Wiederholen",		"Letzten Befehl wiederholen")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"A&usschneiden",	"Markierung l�schen und in die Zwischenablage kopieren")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"&Kopieren",		"Markierung in die Zwischenablage kopieren")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"E&inf�gen",		"Inhalt der Zwischenablage einf�gen")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"L�&schen",			"Markierung l�schen")
        MenuLabel(AP_MENU_ID_EDIT_SELECTALL,	"&Alles markieren",	"Das ganze Dokument markieren")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"&Suchen",			"Text im Dokument suchen")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"&Ersetzen",		"Text im Dokument ersetzen")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"&Gehe zu",			"Einf�gemarke im Dokument bewegen")
	MenuLabel(AP_MENU_ID_EDIT_SPELL,		"&Spelling",		"Check the document for incorrect spelling")
	MenuLabel(AP_MENU_ID_EDIT_OPTIONS,		"&Optionen",		"Einstellungen f�r das Programm �ndern")
	
	MenuLabel(AP_MENU_ID_VIEW,				"&Ansicht",			NULL)
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&Symbolleisten",	NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"&Standard",		"Symbolleiste <Standard> anzeigen oder verbergen")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,	"&Format",			"Symbolleiste <Format> anzeigen oder verbergen")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"&Lineal",			"Lineale anzeigen oder verbergen")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,	"Status&leiste",	"Statusleiste anzeigen oder verbergen")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"Para&graph",		"Paragraphen anzeigen oder verbergen")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"&Kopf- und Fu�zeilen",	"Kopf- und Fu�zeilen editieren")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Zoom",			"Ansichtsgr��e des Dokuments festlegen")

	MenuLabel(AP_MENU_ID_INSERT,			"&Einf�gen",		NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"Manueller &Wechsel",	"Seiten-, Spalten- oder Absatzwechsel einf�gen")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"&Seitenzahlen",	"Seitenzahlen einf�gen")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,	"Datum und &Uhrzeit",	"Datum und/oder Uhrzeit einf�gen")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"F&eld",			"Berechnetes Feld einf�gen")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"So&nderzeichen",	"Symbol oder Sonderzeichen einf�gen")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,	"&Grafik",			"Grafik aus Datei einf�gen")

	MenuLabel(AP_MENU_ID_FORMAT,			"Forma&t",			NULL)
	MenuLabel(AP_MENU_ID_FMT_FONT,			"&Schrift",			"Schrifteigenschaften der Markierung �ndern")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"&Zeilenabstand",	"Zeilenabstand der Markierung �ndern")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"&Numerierung und Aufz�hlung",	"Aufz�hlungszeichen oder Numerierung einf�gen")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"&Rahmen und Schattierung",	"Rahmen und Schattierung der Markierung bearbeiten")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"&Spalten",			"Spaltenzahl der Markierung �ndern")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"St&il",			"Stil der Markierung festlegen")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabulatoren",		"Tabulatoren setzen")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"&Fett",			"Markierung fett (�ndern)")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"&Kursiv",			"Markierung kursiv (�ndern)")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"&Unterstrichen",	"Markierung unterstrichen (�ndern)")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"&Durchgestrichen",	"Markierung durchgestrichen (�ndern)")
	
	MenuLabel(AP_MENU_ID_ALIGN,				"&Anordnung",		NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"&Links",			"Absatz links ausrichten")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&Zentriert",		"Absatz zentriert ausrichten")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"&Rechts",			"Absatz recht ausrichten")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"A&usrichten",		"Absatz ausrichten")

	MenuLabel(AP_MENU_ID_WINDOW,			"&Fenster",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&Neues Fenster",	"Neues Fenster f�r ein Dokument �ffnen")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",			"Zu diesem Fenster wechseln")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",			"Zu diesem Fenster wechseln")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",			"Zu diesem Fenster wechseln")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",			"Zu diesem Fenster wechseln")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",			"Zu diesem Fenster wechseln")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",			"Zu diesem Fenster wechseln")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",			"Zu diesem Fenster wechseln")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",			"Zu diesem Fesnter wechseln")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",			"Zu diesem Fenster wechseln")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&Mehr Fenster",        	"Liste aller Fenster zeigen")

	MenuLabel(AP_MENU_ID_HELP,				"&Hilfe",		NULL)
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&�ber %s",	        	"Programminformation, Verion und Copyright anzeigen")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
