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

// If the third argument is UT_TRUE, then this is the fall-back for
// this language (named in the first argument).

BeginSetEnc(ja,JP,true,"EUC-JP")

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,				"�ե�����(_F)",		NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"����(_N)",			"������ʸ���������ޤ�")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"����(_O)",			"��¸��ʸ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"�Ĥ���(_C)", 		"ʸ����Ĥ��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"��¸(_S)", 		"ʸ�����¸���ޤ�")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"̾�����դ�����¸", "ʸ����̤�̾����Ĥ�����¸���ޤ�")
	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,	"�ڡ�������(_U)",	"����������ѹ����ޤ�")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"����(_P)",			"���Ƥޤ���ʸ��ΰ�����������ޤ�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"(_1) %s",			"����ʸ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"(_2) %s",			"����ʸ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"(_3) %s",			"����ʸ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"(_4) %s",			"����ʸ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"(_5) %s",			"����ʸ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"(_6) %s",			"����ʸ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"(_7) %s",			"����ʸ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"(_8) %s",			"����ʸ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"(_9) %s",			"����ʸ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"��λ(_X)", 		"���ƤΥ�����ɥ����Ĥ��ơ���λ���ޤ�")

	MenuLabel(AP_MENU_ID_EDIT,				"�Խ�(_E)",			NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"����ɥ�(_U)",		"�Խ����Ƥ򸵤��ᤷ�ޤ�")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"��ɥ�(_R)",		"�Խ����Ƥ��������ᤷ�ޤ�")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"�ڤ���(_T)",		"�����ϰϤ��ڤ��äơ�����åץܡ��ɤ˳�Ǽ���ޤ�")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"ʣ��(_C)",			"�����ϰϤ򥳥ԡ����ơ�����åץܡ��ɤ˳�Ǽ���ޤ�")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"Ž���դ�(_P)",		"����åץܡ��ɤ����������ޤ�")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"���ꥢ(_A)",		"�����ϰϤ������ޤ�")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,	"��������(_L)",		"ʸ�����Τ����򤷤ޤ�")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"����(_F)",			"���ꤷ��ʸ�򸡺����ޤ�")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"�ִ�(_E)",			"���ꤷ��ʸ���̤�ʸ���֤������ޤ�")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"������(_G)",		"���ꤷ�����˥����פ��ޤ�")
	
	MenuLabel(AP_MENU_ID_VIEW,				"ɽ��(_V)",			        NULL)
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"�ġ���С�(_T)",	        NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"ɸ��(_S)",		            "ɸ��ġ���С���ɽ��/��ɽ��")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,	"��(_F)",                 "�񼰥ġ���С���ɽ��/��ɽ��")
	MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,		"��ĥ(_E)",			        "��ĥ�ġ���С���ɽ��/��ɽ��")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"�롼�顼(_R)",		        "�롼�顼��ɽ��/��ɽ��")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,	"���ơ������С�(_S)",	    "���ơ������С���ɽ��/��ɽ��")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"����ɽ��(_G)",	            "�����ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"�إå����եå�(_H)",	    "�ƥڡ����ξ岼�ˤ���ʸ���Խ����ޤ�")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"������(_Z)",			    "ʸ���ɽ����̾�/���礷��ɽ�����ޤ�")

	MenuLabel(AP_MENU_ID_INSERT,			"����(_I)",			        NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"�ڡ������ڤ�(_B)",	        "�ڡ����������ޤ��ϥ��������ζ��ڤ���������ޤ�")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"�ڡ����ֹ�(_U)",	        "�ڡ����ֹ��ưŪ�˹��������������ޤ�")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,	"����(_T)",	                "���դȻ�����������ޤ�")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"�ե������(_F)",	        "�ե�����ɤ�׻���̤��������ޤ�")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"����ܥ�(_M)",		        "����ޤ���¾���ü��ʸ�����������ޤ�")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,	"����(_P)",			        "��¸�β����ե�������������ޤ�")

	MenuLabel(AP_MENU_ID_FORMAT,			"��(_O)",			        NULL)
	MenuLabel(AP_MENU_ID_FMT_FONT,			"�ե����(_F)",		        "���򤷤�ʸ�Υե���Ȥ��ѹ����ޤ�")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"����(_P)",		            "���򤷤�����ν񼰤��ѹ����ޤ�")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"Bullets���ֹ��դ�(_N)",	"���򤷤������ Bullets ���ֹ��դ����ɲäޤ����ѹ�")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"�����ȥ�������(_D)",	    "�����ϰϤζ����ȥ������ɤ��ɲä��ޤ�")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"��(_C)",			        "������ѹ����ޤ�")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"��������(_Y)",		        "�����ϰϤν񼰤����������Ŭ�Ѥ��ޤ�")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"����(_T)",			        "���֤򥻥åȤ��ޤ�")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"����(_B)",			        "�����ϰϤ������ˤ��ޤ� (�ڤ��ؤ���)")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"������å�(_I)",	        "�����ϰϤ򥤥���å��ˤ��ޤ� (�ڤ��ؤ���)")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"����(_U)",		            "�����ϰϤ˲������դ��ޤ� (�ڤ��ؤ���)")
	MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"����(_O)",		            "�����ϰϤ˾������դ��ޤ� (�ڤ��ؤ���)")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"��ä���(_K)",		        "�����ϰϤ˼��ä������դ��ޤ� (�ڤ��ؤ���)")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,	"���դ�(_R)",	            "�����ϰϤ���դ�ʸ���ˤ��ޤ� (�ڤ��ؤ���)")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"���դ�(_S)",		        "�����ϰϤ��դ�ʸ���ˤ��ޤ� (�ڤ��ؤ���)")

	MenuLabel(AP_MENU_ID_TOOLS,				"�ġ���(_T)",			    NULL)   
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"���ڥ�����å�(_S)",		"ʸ����δְ�ä��֤������å����ޤ�")
	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,	"ʸ�����������(_W)",		"ʸ�����ñ��ι�פ򥫥���Ȥ��ޤ�")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"����(_P)",		            "���̤�����")

	MenuLabel(AP_MENU_ID_ALIGN,				"����(_A)",			    NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"���ͤ�(_L)",			"����򺸵ͤ�ˤ��ޤ�")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"����ͤ�(_C)",			"���������ͤ�ˤ��ޤ�")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"���ͤ�(_R)",			"����򱦵ͤ�ˤ��ޤ�")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"����·��(_J)",			"����������·���ޤ�")

	MenuLabel(AP_MENU_ID_WINDOW,			"������ɥ�(_W)",		NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"����������ɥ�(_N)",	"�⤦��ĥ�����ɥ��򳫤��ޤ�")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"(_1) %s",			    "���Υ�����ɥ���ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"(_2) %s",			    "���Υ�����ɥ���ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"(_3) %s",			    "���Υ�����ɥ���ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"(_4) %s",			    "���Υ�����ɥ���ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"(_5) %s",			    "���Υ�����ɥ���ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"(_6) %s",			    "���Υ�����ɥ���ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"(_7) %s",			    "���Υ�����ɥ���ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"(_8) %s",			    "���Υ�����ɥ���ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"(_9) %s",			    "���Υ�����ɥ���ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"���˥�����ɥ�(_M)",	"Show full list of windows")

	MenuLabel(AP_MENU_ID_HELP,			    "�إ��(_H)",		NULL)
	MenuLabel(AP_MENU_ID_HELP_CONTENTS,		"�إ��(_C)",	    "�إ�פ�ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_HELP_INDEX,		"�ܼ�(_I)",		    "�إ�פ��ܼ���ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_HELP_CHECKVER,		"�С������(_V)",	"�ץ����ΥС�������ɽ�����ޤ�")
	MenuLabel(AP_MENU_ID_HELP_SEARCH,		"����(_S)",	        "�إ�פ��鸡�����ޤ�...")
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"%s ����",		    "�ץ�������(�С����������)��ɽ�����ޤ�") 
	MenuLabel(AP_MENU_ID_HELP_ABOUTOS,		"Open Source(_O)",	"Open Source �ˤĤ��ƾ����ɽ�����ޤ�")

	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",				"�����֤���ѹ�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",				"�����֤���ѹ�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",				"�����֤���ѹ�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",				"�����֤���ѹ�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",				"�����֤���ѹ�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",				"�����֤���ѹ�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",				"�����֤���ѹ�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",				"�����֤���ѹ�")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",				"�����֤���ѹ�")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"����̵��(_I)", 	"ʸ����Τ���ñ��ˤĤ��Ƥ�����̵�뤷�ޤ�")
	MenuLabel(AP_MENU_ID_SPELL_ADD,			"�ɲ�(_A)", 		"����ñ���桼����������ɲä��ޤ�")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
