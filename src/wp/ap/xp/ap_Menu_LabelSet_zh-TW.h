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

/*
 * Traditional Chinese Translation by
 *
 *  Chinese Linux Extensions Project  http://cle.linux.org.tw/
 *     Chih-Wei Huang <cwhuang@linux.org.tw>
 *     Armani         <armani@cle.linux.org.tw>
 *
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

BeginSet(zh,TW,true)

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,			"�ɮ�[&F]",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"�}�s�ɮ�[&N]",			"�إߤ@���s�����")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"�}������[&O]",			"�}������")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"����[&C]", 			"�����o�����")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"�x�s[&S]", 			"�x�s�o�����")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"�x�s��[&A]", 			"�N�ɮצs���䥦���ɦW")
	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,		"�����]�w[&u]",			"���ܿ�X�ﶵ")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"�C�L[&P]",			"�C�L�X�Ҧ��γ������")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"[&1] %s",			"�}�ҳo�����")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"[&2] %s",			"�}�ҳo�����")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"[&3] %s",			"�}�ҳo�����")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"[&4] %s",			"�}�ҳo�����")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"[&5] %s",			"�}�ҳo�����")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"[&6] %s",			"�}�ҳo�����")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"[&7] %s",			"�}�ҳo�����")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"[&8] %s",			"�}�ҳo�����")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"[&9] %s",			"�}�ҳo�����")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"���}[&x]", 			"�����o�����ε{���Ҧ����������}")

	MenuLabel(AP_MENU_ID_EDIT,			"�s��[&E]",			NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"�_��[&U]",			"�_��")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"����[&R]",			"����")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"�ŤU[&t]",			"�ŤU����ϰ쪺���e�öK��ŶKï�W")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"�ƻs[&C]",			"�ƨ����Ϥ������e�öK��ŶKï�W")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"�K�W[&P]",			"�N�ŶKï�W�����e�K��u�@��")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"�M��[&a]",			"�R������d�򤺪����e")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,		"����[&l]",			"������������e���")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"�M��[&F]",			"�j�M�S�w����r")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"�m��[&e]",			"�m�����P���r��Φr��")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"��..[&G]",			"����s�����J�I")
	
	MenuLabel(AP_MENU_ID_VIEW,			"�˵�[&V]",		NULL)
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"�u��[&T]",		NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"��{/���� �зǤu��C[&S]",	"��{/���üзǤu��C")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,		"��{/���� �榡�u��C[&F]",     "��{/���î榡�u��C")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"�سW[&R]",			"��{/���äسW�u��C")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,		"���A�C[&S]",			"��{/���äسW�u��C")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"��ܬq���ؽu[&g]",		"��ܤ��|�C�L�X�Ӫ��q���ؽu�аO")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"����/����[&H]",		"�s��C�@��������/����")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"�Y�����[&Z]",			"�Y�����")

	MenuLabel(AP_MENU_ID_INSERT,			"���J[&I]",			NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"����u[&B]",			"���J�@�ӭ����Ϊ����Ϊ̰Ϭq���j�u���O")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"���X[&u]",			"���J�@�ӭ��X")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,		"���/�ɶ�[&T]",		"���J�ɶ��Τ��")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"�϶�[&F]",			"���J�@�ӭp��϶�")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"�Ÿ�[&m]",			"���J�@�ӲŸ��ίS�O�r��")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,		"�Ϥ�[&P]",			"�q�{�����ɮ׶}�ҹϤ�")

	MenuLabel(AP_MENU_ID_FORMAT,			"�榡[&o]",			NULL)
	MenuLabel(AP_MENU_ID_FMT_FONT,			"�r��[&F]",			"���ܦr��")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"�q��[&P]",			"���ܬq��")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"���زŸ�[&N]",			"�W�[�έק�q���̪����زŸ�")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"��ؤκ���[&d]",		"��ؤκ����]�w")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"��������[&C]",		"��������")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"�榡[&y]",			"�q������]�w")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabs",			"Set tab stops")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"����[&B]",			"�Ͽ���Ϧr���ܲ�")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"����[&I]",			"�Ͽ���Ϧr���ܱ�")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"���u[&U]",			"�b����ϥ[�J���u")
	MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"�~��[&O]",			"�b����ϥ[�J�~��")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"�����u[&k]",			"�N�r��Φr���q���b��")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,		"�W��[&r]",			"�Ͽ���Ϥ�r�����W�Цr")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"�U��[&S]",			"�Ͽ���Ϥ�r�����U�Цr")

	MenuLabel(AP_MENU_ID_TOOLS,			"�u��[&T]",			NULL)   
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"���r�ˬd[&S]",			"�ˬd���r")
	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,		"�r�Ʋέp[&W]",			"�p�⥻�g�峹���r��")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"�ﶵ[&O]",			"�]�w�ﶵ")

	MenuLabel(AP_MENU_ID_ALIGN,			"���[&A]",			NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"�a�����[&L]",			"�a�����")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"�m�����[&C]",			"�m�����")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"�a�k���[&R]",			"�a�k���")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"���k���[&J]",				"�ھڤ峹���")

	MenuLabel(AP_MENU_ID_WINDOW,			"����[&W]",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"�b�s�����}��..[&N]",		"�N�����峹�H�t�@�����}��")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"[&1] %s",			"��_�o�ӵ���")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"[&2] %s",			"��_�o�ӵ���")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"[&3] %s",			"��_�o�ӵ���")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"[&4] %s",			"��_�o�ӵ���")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"[&5] %s",			"��_�o�ӵ���")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"[&6] %s",			"��_�o�ӵ���")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"[&7] %s",			"��_�o�ӵ���")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"[&8] %s",			"��_�o�ӵ���")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"[&9] %s",			"��_�o�ӵ���")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"��h������..[&M]",		"����Ҧ������C��")

	MenuLabel(AP_MENU_ID_HELP,			"����[&H]",			NULL)
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"����[&A] %s",		"�{����T, �������X,���v�k�� ")

	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",				"���ܬ��o�ӫ�ĳ���r�覡")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",				"���ܬ��o�ӫ�ĳ���r�覡")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",				"���ܬ��o�ӫ�ĳ���r�覡")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",				"���ܬ��o�ӫ�ĳ���r�覡")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",				"���ܬ��o�ӫ�ĳ���r�覡")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",				"���ܬ��o�ӫ�ĳ���r�覡")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",				"���ܬ��o�ӫ�ĳ���r�覡")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",				"���ܬ��o�ӫ�ĳ���r�覡")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",				"���ܬ��o�ӫ�ĳ���r�覡")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"����[&I]", 			"�����Ҧ��{�檺���")
	MenuLabel(AP_MENU_ID_SPELL_ADD,		"�W�[[&A]", 			"�W�[�o�Ӧr��۩w���r��")

	// ... add others here ...

	MenuLabel(AP_MENU_ID__BOGUS2__,			NULL,				NULL)

EndSet()
