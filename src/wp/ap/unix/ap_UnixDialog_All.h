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

/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.  Each time you add an entry to the top-half
** of this file be sure to add a corresponding entry to the other
** half and be sure to add an entry to each of the other platforms.
******************************************************************
*****************************************************************/

#ifndef AP_UNIXDIALOG_ALL_H
#define AP_UNIXDIALOG_ALL_H

#	define DEFAULT_BUTTON_WIDTH	85

#	include "xap_UnixDlg_MessageBox.h"
#	include "xap_UnixDlg_FileOpenSaveAs.h"
#	include "xap_UnixDlg_Print.h"
#   include "xap_UnixDlg_PrintPreview.h"
#	include "xap_UnixDlg_WindowMore.h"
#	include "xap_UnixDlg_FontChooser.h"
#	include "xap_UnixDlg_About.h"
#	include "xap_UnixDlg_Zoom.h"
#	include "xap_UnixDlg_Insert_Symbol.h"
#	include "xap_UnixDlg_Language.h"
#   include "xap_UnixDlg_PluginManager.h"

#	include "ap_UnixDialog_Replace.h"
#	include "ap_UnixDialog_Break.h"
#	include "ap_UnixDialog_Goto.h"
#   include "ap_UnixDialog_PageNumbers.h"
#   include "ap_UnixDialog_PageSetup.h"
#	include "ap_UnixDialog_Paragraph.h"
#	include "ap_UnixDialog_Options.h"
#	include "ap_UnixDialog_Spell.h"
#	include "ap_UnixDialog_Styles.h"
#	include "ap_UnixDialog_Tab.h"
#	include "ap_UnixDialog_Insert_DateTime.h"
#	include "ap_UnixDialog_WordCount.h"
#	include "ap_UnixDialog_Field.h"
#	include "ap_UnixDialog_Lists.h"
#	include "ap_UnixDialog_Columns.h"
#	include "ap_UnixDialog_Tab.h"
#   include "ap_UnixDialog_ToggleCase.h"
#   include "ap_UnixDialog_Background.h"
#   include "ap_UnixDialog_New.h"

#ifdef HAVE_GNOME
#	include "xap_UnixGnomeDlg_About.h"
#	include "xap_UnixGnomeDlg_FontChooser.h"
#	include "xap_UnixGnomeDlg_Insert_Symbol.h"
#	include "xap_UnixGnomeDlg_MessageBox.h"
#   include "xap_UnixGnomeDlg_Print.h"
#   include "xap_UnixGnomeDlg_PrintPreview.h"
#	include "xap_UnixGnomeDlg_Zoom.h"
#   include "xap_UnixGnomeDlg_Language.h"
#   include "xap_UnixGnomeDlg_PluginManager.h"

#   include "ap_UnixGnomeDialog_Break.h"
#	include "ap_UnixGnomeDialog_Replace.h"
#	include "ap_UnixGnomeDialog_Goto.h"
#   include "ap_UnixGnomeDialog_Insert_DateTime.h"
#	include "ap_UnixGnomeDialog_Paragraph.h"
#   include "ap_UnixGnomeDialog_PageNumbers.h"
#   include "ap_UnixGnomeDialog_PageSetup.h"
#	include "ap_UnixGnomeDialog_Options.h"
#	include "ap_UnixGnomeDialog_WordCount.h"
#	include "ap_UnixGnomeDialog_Field.h"
#   include "ap_UnixGnomeDialog_Lists.h"
#   include "ap_UnixGnomeDialog_Columns.h"
#   include "ap_UnixGnomeDialog_Spell.h"
#   include "ap_UnixGnomeDialog_Styles.h"
#   include "ap_UnixGnomeDialog_Tab.h"
#   include "ap_UnixGnomeDialog_ToggleCase.h"
#   include "ap_UnixGnomeDialog_Background.h"
#   include "ap_UnixGnomeDialog_New.h"

#	ifdef HAVE_GNOME_DIRECT_PRINT
#		include "xap_UnixDlg_Print.h"
#	endif
#endif

	// ... add new dialogs here ...

#else

#   ifdef HAVE_GNOME
    DeclareDialog(XAP_DIALOG_ID_MESSAGE_BOX,	XAP_UnixGnomeDialog_MessageBox)
	DeclareDialog(XAP_DIALOG_ID_FILE_OPEN,		XAP_UnixDialog_FileOpenSaveAs)
	DeclareDialog(XAP_DIALOG_ID_FILE_SAVEAS,	XAP_UnixDialog_FileOpenSaveAs)
	DeclareDialog(XAP_DIALOG_ID_PRINT,			XAP_UnixGnomeDialog_Print)
    DeclareDialog(XAP_DIALOG_ID_PRINTPREVIEW, XAP_UnixGnomeDialog_PrintPreview)
	DeclareDialog(XAP_DIALOG_ID_PRINTTOFILE,	XAP_UnixDialog_FileOpenSaveAs)
#	ifdef HAVE_GNOME_DIRECT_PRINT	
		DeclareDialog(XAP_DIALOG_ID_PRINT_DIRECTLY,	XAP_UnixDialog_Print)
#	endif
	DeclareDialog(XAP_DIALOG_ID_FONT,			XAP_UnixGnomeDialog_FontChooser)
	DeclareDialog(XAP_DIALOG_ID_WINDOWMORE,		XAP_UnixDialog_WindowMore)
	DeclareDialog(XAP_DIALOG_ID_ZOOM,			XAP_UnixGnomeDialog_Zoom)
	DeclareDialog(XAP_DIALOG_ID_ABOUT,			XAP_UnixGnomeDialog_About)
	DeclareDialog(XAP_DIALOG_ID_INSERT_SYMBOL,  XAP_UnixGnomeDialog_Insert_Symbol)
	DeclareDialog(XAP_DIALOG_ID_INSERT_PICTURE,	XAP_UnixDialog_FileOpenSaveAs)

	DeclareDialog(AP_DIALOG_ID_REPLACE,			AP_UnixGnomeDialog_Replace)
	DeclareDialog(AP_DIALOG_ID_FIND,			AP_UnixGnomeDialog_Replace)
    DeclareDialog(AP_DIALOG_ID_BACKGROUND,      AP_UnixGnomeDialog_Background)
	DeclareDialog(AP_DIALOG_ID_GOTO,			AP_UnixGnomeDialog_Goto)
	DeclareDialog(AP_DIALOG_ID_BREAK,			AP_UnixGnomeDialog_Break)
	DeclareDialog(AP_DIALOG_ID_SPELL,			AP_UnixGnomeDialog_Spell)
	DeclareDialog(AP_DIALOG_ID_STYLES,			AP_UnixGnomeDialog_Styles)
	DeclareDialog(AP_DIALOG_ID_TAB,				AP_UnixGnomeDialog_Tab)
    DeclareDialog(AP_DIALOG_ID_PAGE_NUMBERS,    AP_UnixGnomeDialog_PageNumbers)
    DeclareDialog(AP_DIALOG_ID_FILE_PAGESETUP,  AP_UnixGnomeDialog_PageSetup) 
	DeclareDialog(AP_DIALOG_ID_PARAGRAPH,		AP_UnixGnomeDialog_Paragraph)
	DeclareDialog(AP_DIALOG_ID_OPTIONS,			AP_UnixGnomeDialog_Options)
	DeclareDialog(AP_DIALOG_ID_INSERT_DATETIME,	AP_UnixGnomeDialog_Insert_DateTime)
	DeclareDialog(AP_DIALOG_ID_WORDCOUNT,       AP_UnixGnomeDialog_WordCount)
	DeclareDialog(AP_DIALOG_ID_FIELD,			AP_UnixGnomeDialog_Field)
	DeclareDialog(AP_DIALOG_ID_LISTS,		    AP_UnixGnomeDialog_Lists)
	DeclareDialog(AP_DIALOG_ID_COLUMNS,			AP_UnixGnomeDialog_Columns)
	DeclareDialog(AP_DIALOG_ID_TAB,				AP_UnixGnomeDialog_Tab)
    DeclareDialog(XAP_DIALOG_ID_LANGUAGE,		XAP_UnixGnomeDialog_Language)
    DeclareDialog(AP_DIALOG_ID_TOGGLECASE,      AP_UnixGnomeDialog_ToggleCase)
	DeclareDialog(AP_DIALOG_ID_FILE_NEW,        AP_UnixGnomeDialog_New)
	DeclareDialog(XAP_DIALOG_ID_PLUGIN_MANAGER, XAP_UnixGnomeDialog_PluginManager)

	// ... also add new dialogs here ...
#   else
	DeclareDialog(XAP_DIALOG_ID_MESSAGE_BOX,	XAP_UnixDialog_MessageBox)
	DeclareDialog(XAP_DIALOG_ID_FILE_OPEN,		XAP_UnixDialog_FileOpenSaveAs)
	DeclareDialog(XAP_DIALOG_ID_FILE_SAVEAS,	XAP_UnixDialog_FileOpenSaveAs)
	DeclareDialog(XAP_DIALOG_ID_PRINT,			XAP_UnixDialog_Print)
    DeclareDialog(AP_DIALOG_ID_BACKGROUND, AP_UnixDialog_Background)
	DeclareDialog(XAP_DIALOG_ID_PRINTTOFILE,	XAP_UnixDialog_FileOpenSaveAs)
	DeclareDialog(XAP_DIALOG_ID_FONT,			XAP_UnixDialog_FontChooser)
	DeclareDialog(XAP_DIALOG_ID_LANGUAGE,		XAP_UnixDialog_Language)
	DeclareDialog(XAP_DIALOG_ID_WINDOWMORE,		XAP_UnixDialog_WindowMore)
	DeclareDialog(XAP_DIALOG_ID_ZOOM,			XAP_UnixDialog_Zoom)
	DeclareDialog(XAP_DIALOG_ID_INSERT_SYMBOL,  XAP_UnixDialog_Insert_Symbol)
	DeclareDialog(XAP_DIALOG_ID_INSERT_PICTURE,	XAP_UnixDialog_FileOpenSaveAs)

	DeclareDialog(XAP_DIALOG_ID_ABOUT,			XAP_UnixDialog_About)

	DeclareDialog(AP_DIALOG_ID_REPLACE,			AP_UnixDialog_Replace)
	DeclareDialog(AP_DIALOG_ID_FIND,			AP_UnixDialog_Replace)
	DeclareDialog(AP_DIALOG_ID_GOTO,			AP_UnixDialog_Goto)
	DeclareDialog(AP_DIALOG_ID_BREAK,			AP_UnixDialog_Break)
	DeclareDialog(AP_DIALOG_ID_SPELL,			AP_UnixDialog_Spell)
	DeclareDialog(AP_DIALOG_ID_STYLES,			AP_UnixDialog_Styles)
    DeclareDialog(AP_DIALOG_ID_PAGE_NUMBERS,    AP_UnixDialog_PageNumbers)
	DeclareDialog(AP_DIALOG_ID_PARAGRAPH,		AP_UnixDialog_Paragraph)	
	DeclareDialog(AP_DIALOG_ID_OPTIONS,			AP_UnixDialog_Options)	
	DeclareDialog(AP_DIALOG_ID_TAB,				AP_UnixDialog_Tab)
	DeclareDialog(AP_DIALOG_ID_INSERT_DATETIME,	AP_UnixDialog_Insert_DateTime)
	DeclareDialog(AP_DIALOG_ID_WORDCOUNT,		AP_UnixDialog_WordCount)
	DeclareDialog(AP_DIALOG_ID_FIELD,			AP_UnixDialog_Field)
	DeclareDialog(AP_DIALOG_ID_LISTS,			AP_UnixDialog_Lists)
	DeclareDialog(AP_DIALOG_ID_COLUMNS,			AP_UnixDialog_Columns)
	DeclareDialog(AP_DIALOG_ID_TAB,				AP_UnixDialog_Tab)
	DeclareDialog(AP_DIALOG_ID_FILE_PAGESETUP,  AP_UnixDialog_PageSetup)
	DeclareDialog(AP_DIALOG_ID_TOGGLECASE,      AP_UnixDialog_ToggleCase)
	DeclareDialog(AP_DIALOG_ID_FILE_NEW,        AP_UnixDialog_New)
	DeclareDialog(XAP_DIALOG_ID_PLUGIN_MANAGER, XAP_UnixDialog_PluginManager)

	// ... also add new dialogs here ...
#   endif

#endif
