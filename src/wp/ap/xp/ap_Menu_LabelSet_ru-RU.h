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

// If the third argument is true, then this is the fall-back for
// this language (named in the first argument).

BeginSetEnc(ru,RU,true,"koi8-r")

MenuLabel(AP_MENU_ID__BOGUS1__,		NULL,			NULL)

//       (id,				szLabel,           	szStatusMsg)

MenuLabel(AP_MENU_ID_FILE,		"&����",		NULL)
 MenuLabel(AP_MENU_ID_FILE_NEW,		"&�����", 		"������� ����� ��������")	
 MenuLabel(AP_MENU_ID_FILE_OPEN,	"&�������",		"������� ������������ ��������")
 MenuLabel(AP_MENU_ID_FILE_SAVE,	"&���������", 		"��������� ��������")
 MenuLabel(AP_MENU_ID_FILE_SAVEAS,	"��������� &���", 	"��������� �������� ��� ������ ������")
 MenuLabel(AP_MENU_ID_FILE_PAGESETUP,	"�&�������� ��������",	"�������� ��������� ������")
 MenuLabel(AP_MENU_ID_FILE_PRINT,	"&������",		"�������� �������� ��� ��� �����")
 MenuLabel(AP_MENU_ID_FILE_PRINT_DIRECTLY,		"�������� ����&����",			"�������� �������� �� ��������� gnome-print")
 MenuLabel(AP_MENU_ID_FILE_PRINT_PREVIEW,	"�&�������������� ��������",		"����������, ��� ����� ��������� ������������ ��������") 
 MenuLabel(AP_MENU_ID_FILE_RECENT_1,	"&1 %s",		"������� ���� ��������")
 MenuLabel(AP_MENU_ID_FILE_RECENT_2,	"&2 %s",		"������� ���� ��������")
 MenuLabel(AP_MENU_ID_FILE_RECENT_3,	"&3 %s",		"������� ���� ��������")
 MenuLabel(AP_MENU_ID_FILE_RECENT_4,	"&4 %s",		"������� ���� ��������")
 MenuLabel(AP_MENU_ID_FILE_RECENT_5,	"&5 %s",		"������� ���� ��������")
 MenuLabel(AP_MENU_ID_FILE_RECENT_6,	"&6 %s",		"������� ���� ��������")
 MenuLabel(AP_MENU_ID_FILE_RECENT_7,	"&7 %s",		"������� ���� ��������")
 MenuLabel(AP_MENU_ID_FILE_RECENT_8,	"&8 %s",		"������� ���� ��������")
 MenuLabel(AP_MENU_ID_FILE_RECENT_9,	"&9 %s",		"������� ���� ��������")
 MenuLabel(AP_MENU_ID_FILE_CLOSE,	"&�������", 		"������� ��������")
 MenuLabel(AP_MENU_ID_FILE_EXIT,	"&�����", 		"������� ��� ��������� � �����")
    
MenuLabel(AP_MENU_ID_EDIT,		"&��������������",	NULL)
 MenuLabel(AP_MENU_ID_EDIT_UNDO,	"&������",		"�������� ���������� ��������")
 MenuLabel(AP_MENU_ID_EDIT_REDO,	"&������",		"��������� ���������� ���������� ��������")
 MenuLabel(AP_MENU_ID_EDIT_CUT,		"&��������",		"�������� ��������� � ��������� ��� � ����� ������")
 MenuLabel(AP_MENU_ID_EDIT_COPY,	"&����������",		"����������� ��������� � ��������� ��� � ����� ������")
 MenuLabel(AP_MENU_ID_EDIT_PASTE,	"�&�������",		"�������� ���������� ������ ������ � ��������")
 MenuLabel(AP_MENU_ID_EDIT_CLEAR,	"�&�������",		"�������� ���������")
 MenuLabel(AP_MENU_ID_EDIT_SELECTALL,	"�&������� ���",	"�������� ���� ��������")
 MenuLabel(AP_MENU_ID_EDIT_FIND,	"&�����",		"����� �������� �����")
 MenuLabel(AP_MENU_ID_EDIT_REPLACE,	"&��������",		"�������� �������� ����� ������")
 MenuLabel(AP_MENU_ID_EDIT_GOTO,	"��&����� �",		"����������� ������ � �������� �����")

MenuLabel(AP_MENU_ID_VIEW,		"&���",			NULL)
 MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,	"&������",		NULL)
  MenuLabel(AP_MENU_ID_VIEW_TB_STD,	"&�����������",		"��������/�������� ����������� ������")
  MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,	"&��������������",	"��������/�������� ������ ��������������")
  MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,	"&��������������",	"��������/�������� �������������� ������")
 MenuLabel(AP_MENU_ID_VIEW_RULER,	"&�������",		"��������/�������� �������")
 MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,	"&������ �������",	"��������/�������� ������ �������")
 MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,	"�&������� �����������",	"��������/�������� ������������ �������")
 MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,	"&������� � ������ �����������",	"������������� ����� ����� � ������ ������ ��������")
 MenuLabel(AP_MENU_ID_VIEW_ZOOM,	"&�������",		"���������/��������� ������� ������������� ���������")
 MenuLabel(AP_MENU_ID_VIEW_FULLSCREEN,	"�����&�������� �����",		"������ ��� �������� ����������������� ���������� ����� ����")

MenuLabel(AP_MENU_ID_INSERT,		"�&������",		NULL)
 MenuLabel(AP_MENU_ID_INSERT_BREAK,	"&������",		"�������� ������ ��������, ������� ��� ������")
 MenuLabel(AP_MENU_ID_INSERT_PAGENO,	"&������ �������",	"�������� ��������������� ������ �������")
 MenuLabel(AP_MENU_ID_INSERT_DATETIME,	"&���� � �����",	"�������� ����/�����")
 MenuLabel(AP_MENU_ID_INSERT_FIELD,	"&����",		"�������� ��������������� ����")
 MenuLabel(AP_MENU_ID_INSERT_SYMBOL,	"&������",		"�������� ������ ��� ������ ����������� ����")
 MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,	"�&������",		"�������� ������������ ������� �� �������� �����")

MenuLabel(AP_MENU_ID_FORMAT,		"�&�������������",	NULL)
 MenuLabel(AP_MENU_ID_FMT_FONT,		"&�����",		"�������� ����� ����������� ������")
 MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,	"&�����",		"�������� ������ ����������� ������")
 MenuLabel(AP_MENU_ID_FMT_BULLETS,	"&������",		"��������/������ �������/��������� � �������")
 MenuLabel(AP_MENU_ID_FMT_BORDERS,	"&������� � �������",	"��������/������ ����� � ������� � ���������")
 MenuLabel( AP_MENU_ID_FMT_DOCUMENT,	"��������� ������&��",	"�������� ��������� ������")
 MenuLabel(AP_MENU_ID_FMT_COLUMNS,	"�&������",		"�������� ���-�� �������")
 MenuLabel(AP_MENU_ID_FMT_TABS,		"&���������",		"���������� ������� ���������")
 MenuLabel(AP_MENU_ID_FMT_BOLD,		"&������",		"������� ��������� ������")
 MenuLabel(AP_MENU_ID_FMT_ITALIC,	"&������",		"������� ��������� ���������")
 MenuLabel(AP_MENU_ID_FMT_UNDERLINE,	"&�������������",	"����������� ���������")
 MenuLabel(AP_MENU_ID_FMT_OVERLINE,	"���&����������",	"����������� ���������")
 MenuLabel(AP_MENU_ID_FMT_STRIKE,	"�&�������������",	"������������ ���������")
 MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,	"&������ ������",	"��������� ��������� � ������� ������")
 MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,	"&������ ������",	"��������� ��������� � ������ ������")
 MenuLabel(AP_MENU_ID_ALIGN,		"�&�����������",	NULL)
  MenuLabel(AP_MENU_ID_ALIGN_LEFT,	"�� &������ ����",	"��������� ����� �� ������ ����")
  MenuLabel(AP_MENU_ID_ALIGN_CENTER,	"�� &������",		"��������� ����� �� ������")
  MenuLabel(AP_MENU_ID_ALIGN_RIGHT,	"�� &������� ����",	"��������� ����� �� ������� ����")
  MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,	"�� &������",		"��������� ����� �� ������ �����")
 MenuLabel(AP_MENU_ID_FMT_STYLE,	"��&���",		"���������� ��� ��������� ����� � ���������")

MenuLabel(AP_MENU_ID_TOOLS,		"&������",		NULL)   
 MenuLabel(AP_MENU_ID_TOOLS_SPELL,	"&��������� ������������",	"��������� �������� �� ������������")
 MenuLabel(AP_MENU_ID_TOOLS_SPELLING,	"&������������",	"��������� �������� �� ������������") 
 MenuLabel(AP_MENU_ID_TOOLS_AUTOSPELL,	"��������� &�������������","��������� ������������ �������������") 
 MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,	"&����������",		"��������� ���-�� ����, �������� � �. �. � ���������")
 MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,	"&���������",		"�������� ���������")

MenuLabel(AP_MENU_ID_WINDOW,		"&����",		NULL)
 MenuLabel(AP_MENU_ID_WINDOW_NEW,	"&����� ����",		"������� ����� ���� ��� ���������")
 MenuLabel(AP_MENU_ID_WINDOW_1,		"&1 %s",		"������������� �� ��� ����")
 MenuLabel(AP_MENU_ID_WINDOW_2,		"&2 %s",		"������������� �� ��� ����")
 MenuLabel(AP_MENU_ID_WINDOW_3,		"&3 %s",		"������������� �� ��� ����")
 MenuLabel(AP_MENU_ID_WINDOW_4,		"&4 %s",		"������������� �� ��� ����")
 MenuLabel(AP_MENU_ID_WINDOW_5,		"&5 %s",		"������������� �� ��� ����")
 MenuLabel(AP_MENU_ID_WINDOW_6,		"&6 %s",		"������������� �� ��� ����")
 MenuLabel(AP_MENU_ID_WINDOW_7,		"&7 %s",		"������������� �� ��� ����")
 MenuLabel(AP_MENU_ID_WINDOW_8,		"&8 %s",		"������������� �� ��� ����")
 MenuLabel(AP_MENU_ID_WINDOW_9,		"&9 %s",		"������������� �� ��� ����")
 MenuLabel(AP_MENU_ID_WINDOW_MORE,	"&������ ����",		"�������� ������ ������ ����")

MenuLabel(AP_MENU_ID_HELP,		"&������",		NULL)
 MenuLabel(AP_MENU_ID_HELP_CONTENTS,	"&����������",		"������� ���������� ������")
 MenuLabel(AP_MENU_ID_HELP_INDEX,	"&����������",		"������� ���������� ������")
 MenuLabel(AP_MENU_ID_HELP_CHECKVER,	"&������",		"�������� ������ ���������")
 MenuLabel(AP_MENU_ID_HELP_SEARCH,	"&�����",		"������ ������ �...")
 MenuLabel(AP_MENU_ID_HELP_ABOUT,	"� &��������� %s",	"�������� ������, ������ � �. �.") 
 MenuLabel(AP_MENU_ID_HELP_ABOUTOS,	"� &Open Source",	"�������� ���������� �� Open Source")

MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",			"�������� ���� ��������� ���������")
MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",			"�������� ���� ��������� ���������")
MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",			"�������� ���� ��������� ���������")
MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",			"�������� ���� ��������� ���������")
MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",			"�������� ���� ��������� ���������")
MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",			"�������� ���� ��������� ���������")
MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",			"�������� ���� ��������� ���������")
MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",			"�������� ���� ��������� ���������")
MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",			"�������� ���� ��������� ���������")
MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"&������������", 	"������������ ��� ���������")
MenuLabel(AP_MENU_ID_SPELL_ADD,		"&��������", 		"�������� ����� � �������")

// ... add others here ...

MenuLabel(AP_MENU_ID__BOGUS2__,		NULL,			NULL)

EndSet()
