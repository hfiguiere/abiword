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

// Lithuanian Translation provided by Gediminas Paulauskas <menesis@delfi.lt>

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

BeginSet(lt,LT,true)

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,			"&Byla",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"&Nauja",		"Sukurti nauj� dokument�")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"A&tidaryti",		"Atidaryti dokument�")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"&I�saugoti",		"I�saugoti esam� dokument�")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"I�saugoti k&aip",	"I�saugoti esam� dokument� kitu vardu")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"&U�daryti",		"U�daryti esam� dokument�")
	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,		"P&uslapio nuostatos",	"I�d�stymo nuostatos esamo dokumento spausdinimui")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"S&pausdinti",		"Spausdinti dokument� ar jo dal�")
	MenuLabel(AP_MENU_ID_FILE_PRINT_PREVIEW,	"Spaudinio pe&r�i�ra",	"Per�i�r�ti, kaip atrodys atspausdintas dokumentas")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",		"Atidaryti �� dokument�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",		"Atidaryti �� dokument�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",		"Atidaryti �� dokument�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",		"Atidaryti �� dokument�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",		"Atidaryti �� dokument�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",		"Atidaryti �� dokument�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",		"Atidaryti �� dokument�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",		"Atidaryti �� dokument�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",		"Atidaryti �� dokument�")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"I&�eiti",		"U�daryti visus langus ir i�eiti i� programos")

	MenuLabel(AP_MENU_ID_EDIT,			"&Keisti",		NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"At�a&ukti",		"At�aukti paskutin� atlikt� veiksm�")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"Paka&rtoti",		"Pakartoti at�aukt� veiksm�")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"&I�kirpti",		"Pa�ym�jim� i�kirpti ir pad�ti � krep��")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"&Kopijuoti",		"Pa�ym�jim� kopijuoti � krep��")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"�&d�ti",		"�d�ti krep�io turin�")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"I�&trinti",		"I�trinti pa�ym�jim�")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,		"Pa�ym�ti &visk�",	"Pa�ym�ti vis� tekst�")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"R&asti",		"Ie�koti dokumente eilut�s")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"Pak&eisti",		"Pakeisti rast� eilut� kita")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"Eiti &�",		"Perkelti �ymekl� � nurodyt� viet�")
	
	MenuLabel(AP_MENU_ID_VIEW,			"&Vaizdas",		NULL)
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"�ranki� &juostos",	NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"&Standartin�",		"Rodyti/sl�pti standartini� �ranki� juost�")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,		"&Formatavimo",		"Rodyti/sl�pti teksto formatavimo �ranki� juost�")
	MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,		"&Papildoma",		"Rodyti/sl�pti papildom� �ranki� juost�")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"&Liniuot�",		"Rodyti/sl�pti liniuot�")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,		"&B�senos juosta",	"Rodyti/sl�pti b�senos juost�")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"Rodyti �enklus",	"Rodyti nespausdinamus �enklus")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"Antra�t� ir pora�t�",	"Keisti antra�t� ir pora�t�")
        MenuLabel(AP_MENU_ID_VIEW_FULLSCREEN,		"&Visame ekrane", "Rodyti dokument� viso ekrano re�imu")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Mastelis",		"Suma�inti ar padidinti rodom� vaizd�")

	MenuLabel(AP_MENU_ID_INSERT,			"�terpt&i",		NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"&Skyrikl�...",		"Iterpti puslapio, skyriaus ar skilties skyrikl�")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"Puslapio &numer�...",	"�terpti savaime atsinaujinant� puslapio numer�")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,		"&Dat� ir laik�...",	"�terpti dat� ir/ar laik�")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"&Lauk�...",		"�terpti apskai�iuojam� lauk�")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"�&enkl�...",		"�terpti simbol� ar ypating� �enkl�")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,		"&Paveiksl�l�",		"�terpti paveiksl�l� i� bylos")

	MenuLabel(AP_MENU_ID_FORMAT,			"F&ormatas",		NULL)
	MenuLabel(AP_MENU_ID_FMT_FONT,			"&�riftas",		"Keisti pa�ym�to teksto �rift�")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"&Pastraipa",		"Keisti esamos pastraipos format�")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"&S�ra�as",	"Prid�ti ar pakeisti s�ra�� esamoje pastraipoje")
        MenuLabel(AP_MENU_ID_FMT_DOCUMENT,              "&Dokumentas",             "Nustatyti dokumento puslapio savybes, pvz. dyd� ir para�tes")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"&R�meliai ir �e��liai","Prid�ti ar pakeisti r�melius ar �e��lius esamai pastraipai")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"Skilt&ys",		"Keisti skil�i� skai�i� puslapyje")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"&Stilius",		"Nurodyti ar pritaikyti stili� pa�ym�jimui")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabai",		"Nurodyti tab� sustojimus")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"St&oras",		"Pastorinti pa�ym�t� tekst� (perjungti)")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"Pasv&ir�s",		"Pakreipti pa�ym�t� tekst� (perjungti)")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"Pabra&uktas",		"Pabraukti pa�ym�t� tekst� (perjungti)")
	MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"Br�k�nys &vir�uje",	"Perjungti pa�ym�to teksto pabraukim� vir�uje")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"P&erbraukti",		"Perbraukti pa�ym�t� tekst� (perjungti)")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,		"Pa&keltas",		"Padaryti pa�ym�t� tekst� pakelt� (perjungti)")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"Nu&leistas",		"Padaryti pa�ym�t� tekst� nuleist� (perjungti)")

	MenuLabel(AP_MENU_ID_TOOLS,			"�&rankiai",		NULL)   
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"&Ra�yba",		"Patikrinti dokumento ra�ybos teisingum�")
	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,		"&Statistika",		"Suskai�iuoti, kiek dokumente �od�i�, puslapi� ir kt.")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Nuostatos",		"Nustatyti programos savybes")

	MenuLabel(AP_MENU_ID_ALIGN,			"&Lygiuoti",		NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"Pagal &kair�",		"Lygiuoti pastraip� pagal kair�j� kra�t�")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&Centruoti",		"Lygiuoti eilutes pagal vidur�")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"Pagal &de�in�",	"Lygiuoti pastraip� pagal de�in�j� kra�t�")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Justify",		"I�lyginti pastraip� pagal abu kra�tus")

	MenuLabel(AP_MENU_ID_WINDOW,			"&Langas",		NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&Naujas langas",	"Atidaryti nauj� lang� dokumentui")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",		"I�kelti �� lang�")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",		"I�kelti �� lang�")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",		"I�kelti �� lang�")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",		"I�kelti �� lang�")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",		"I�kelti �� lang�")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",		"I�kelti �� lang�")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",		"I�kelti �� lang�")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",		"I�kelti �� lang�")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",		"I�kelti �� lang�")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&Visi langai",		"Rodyti vis� lang� s�ra��")

	MenuLabel(AP_MENU_ID_HELP,			"&Pagalba",		NULL)
	MenuLabel(AP_MENU_ID_HELP_CONTENTS,		"Pagalbos &turinys",	"Parodyti pagalbos turin�")
	MenuLabel(AP_MENU_ID_HELP_INDEX,		"Pagalbos &Indeksas",	"Parodyti pagalbos indeks�")
	MenuLabel(AP_MENU_ID_HELP_CHECKVER,		"&Versija",		"Parodyti versij�")
	MenuLabel(AP_MENU_ID_HELP_SEARCH,		"&Paie�ka pagalboje...",	"Ie�koti pagalboje")
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&Apie %s",		"Parodyti informacij� apie program�, versij� bei licenzij�") 
	MenuLabel(AP_MENU_ID_HELP_ABOUTOS,		"Apie &Open Source",	"Parodyti informacij� apie Open Source")

	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,		"%s",			"Pakeisti �iuo pasi�lytu �od�iu")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,		"%s",			"Pakeisti �iuo pasi�lytu �od�iu")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,		"%s",			"Pakeisti �iuo pasi�lytu �od�iu")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,		"%s",			"Pakeisti �iuo pasi�lytu �od�iu")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,		"%s",			"Pakeisti �iuo pasi�lytu �od�iu")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,		"%s",			"Pakeisti �iuo pasi�lytu �od�iu")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,		"%s",			"Pakeisti �iuo pasi�lytu �od�iu")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,		"%s",			"Pakeisti �iuo pasi�lytu �od�iu")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,		"%s",			"Pakeisti �iuo pasi�lytu �od�iu")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,		"&Nepaisyti vis�",	"Nepaisyti vis� �io �od�io pasikartojim� �iame dokumente")
	MenuLabel(AP_MENU_ID_SPELL_ADD,			"&Prid�ti", 		"Prid�ti �� �od� � nuosav� �odyn�")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
