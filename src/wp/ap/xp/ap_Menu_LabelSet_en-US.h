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

BeginSet(en,US,true)

	MenuLabel(AP_MENU_ID__BOGUS1__,			NULL,				NULL)

	//       (id,                       	szLabel,           	szStatusMsg)

	MenuLabel(AP_MENU_ID_FILE,				"&File",			NULL)
	MenuLabel(AP_MENU_ID_FILE_NEW,			"&New", 			"Create a new document")	
	MenuLabel(AP_MENU_ID_FILE_OPEN,			"&Open",			"Open an existing document")
	MenuLabel(AP_MENU_ID_FILE_CLOSE,		"&Close", 			"Close the document")
	MenuLabel(AP_MENU_ID_FILE_SAVE,			"&Save", 			"Save the document")
	MenuLabel(AP_MENU_ID_FILE_SAVEAS,		"Save &As", 		"Save the document under a different name")
	MenuLabel(AP_MENU_ID_FILE_IMPORT,       "Op&en Copy",			"Import the document")
	MenuLabel(AP_MENU_ID_FILE_EXPORT,       "Sa&ve Copy",			"Save the document without changing the current name")

	MenuLabel(AP_MENU_ID_FILE_PAGESETUP,	"Page Set&up",		"Change the printing options")
	MenuLabel(AP_MENU_ID_FILE_PRINT,		"&Print",			"Print all or part of the document")
	MenuLabel(AP_MENU_ID_FILE_PRINT_DIRECTLY,"Print &directly",	"Print using the internal PS driver")
	MenuLabel(AP_MENU_ID_FILE_PRINT_PREVIEW, "Print P&review", "Preview the document before printing")
	MenuLabel(AP_MENU_ID_FILE_RECENT_1,		"&1 %s",			"Open this document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_2,		"&2 %s",			"Open this document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_3,		"&3 %s",			"Open this document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_4,		"&4 %s",			"Open this document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_5,		"&5 %s",			"Open this document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_6,		"&6 %s",			"Open this document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_7,		"&7 %s",			"Open this document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_8,		"&8 %s",			"Open this document")
	MenuLabel(AP_MENU_ID_FILE_RECENT_9,		"&9 %s",			"Open this document")
     MenuLabel(AP_MENU_ID_FILE_REVERT, "&Revert", "Revert the document to the last saved state")
	MenuLabel(AP_MENU_ID_FILE_EXIT,			"E&xit", 			"Close all windows in the application and exit")

	MenuLabel(AP_MENU_ID_EDIT,				"&Edit",			NULL)
	MenuLabel(AP_MENU_ID_EDIT_UNDO,			"&Undo",			"Undo editing")
	MenuLabel(AP_MENU_ID_EDIT_REDO,			"&Redo",			"Redo previously undone editing")
	MenuLabel(AP_MENU_ID_EDIT_CUT,			"Cu&t",				"Cut the selection and put it on the Clipboard")
	MenuLabel(AP_MENU_ID_EDIT_COPY,			"&Copy",			"Copy the selection and put it on the Clipboard")
	MenuLabel(AP_MENU_ID_EDIT_PASTE,		"&Paste",			"Insert Clipboard contents")
	MenuLabel(AP_MENU_ID_EDIT_PASTE_SPECIAL, "P&aste Unformatted",  "Insert unformatted clipboard contents")
	MenuLabel(AP_MENU_ID_EDIT_CLEAR,		"Cle&ar",			"Delete the selection")
	MenuLabel(AP_MENU_ID_EDIT_SELECTALL,	"Select A&ll",		"Select the entire document")
	MenuLabel(AP_MENU_ID_EDIT_FIND,			"&Find",			"Find the specified text")
	MenuLabel(AP_MENU_ID_EDIT_REPLACE,		"R&eplace",			"Replace the specified text with different text")
	MenuLabel(AP_MENU_ID_EDIT_GOTO,			"&Go To",			"Move the insertion point to a specific location")
	MenuLabel(AP_MENU_ID_EDIT_EDITHEADER,			"Edit Header",			"Edit the Header on the current page")
	MenuLabel(AP_MENU_ID_EDIT_EDITFOOTER,			"Edit Footer",			"Edit the Footer on the current page")
	MenuLabel(AP_MENU_ID_EDIT_REMOVEHEADER,			"Remove Header",			"Remove the Header on this page from the Document")
	MenuLabel(AP_MENU_ID_EDIT_REMOVEFOOTER,			"Remove Footer",			"Remove the Footer on this page from the Document")
	
	MenuLabel(AP_MENU_ID_VIEW,				"&View",			NULL)
	MenuLabel(AP_MENU_ID_VIEW_NORMAL, "&Normal Layout", "Normal View")
	MenuLabel(AP_MENU_ID_VIEW_WEB, "&Web Layout", "Web Layout")
	MenuLabel(AP_MENU_ID_VIEW_PRINT, "&Print Layout", "Print Layout")
	MenuLabel(AP_MENU_ID_VIEW_TOOLBARS,		"&Toolbars",		NULL)
	MenuLabel(AP_MENU_ID_VIEW_TB_STD,		"&Standard",		"Show or hide the standard toolbar")
	MenuLabel(AP_MENU_ID_VIEW_TB_FORMAT,	"&Formatting",		"Show or hide the formatting toolbar")
	MenuLabel(AP_MENU_ID_VIEW_TB_EXTRA,		"&Extra",			"Show or hide the extra toolbar")
	MenuLabel(AP_MENU_ID_VIEW_RULER,		"Show &Ruler",			"Show or hide the rulers")
	MenuLabel(AP_MENU_ID_VIEW_STATUSBAR,	"&Show Status Bar",		"Show or hide the status bar")
	MenuLabel(AP_MENU_ID_VIEW_SHOWPARA,		"Show Para&graphs",	"Display non-printing characters")
	MenuLabel(AP_MENU_ID_VIEW_HEADFOOT,		"&Header and Footer",	"Edit text at the top or bottom of every page")
	MenuLabel(AP_MENU_ID_VIEW_FULLSCREEN, "F&ull Screen", "View the document in full screen mode")
     MenuLabel(AP_MENU_ID_VIEW_ZOOM_MENU, "&Zoom", "Reduce or enlarge the document display")
	MenuLabel(AP_MENU_ID_VIEW_ZOOM,			"&Zoom",			"Reduce or enlarge the document display")
     MenuLabel(AP_MENU_ID_VIEW_ZOOM_200, "Zoom to &200%", "Zoom to 200%")
     MenuLabel(AP_MENU_ID_VIEW_ZOOM_100, "Zoom to &100%", "Zoom to 100%")
     MenuLabel(AP_MENU_ID_VIEW_ZOOM_75, "Zoom to &75%", "Zoom to 75%")
     MenuLabel(AP_MENU_ID_VIEW_ZOOM_50, "Zoom to &50%", "Zoom to 50%")
     MenuLabel(AP_MENU_ID_VIEW_ZOOM_WIDTH, "Zoom to &Page Width", "Zoom to page width")
     MenuLabel(AP_MENU_ID_VIEW_ZOOM_WHOLE, "Zoom to &Whole Page", "Zoom to whole page")

	MenuLabel(AP_MENU_ID_INSERT,			"&Insert",			NULL)
	MenuLabel(AP_MENU_ID_INSERT_BREAK,		"&Break",			"Insert a page, column, or section break")
	MenuLabel(AP_MENU_ID_INSERT_PAGENO,		"Page N&umbers",	"Insert an automatically-updated page number")
	MenuLabel(AP_MENU_ID_INSERT_DATETIME,	"Date and &Time",	"Insert the date and/or time")
	MenuLabel(AP_MENU_ID_INSERT_FIELD,		"&Field",			"Insert a calculated field")
     MenuLabel(AP_MENU_ID_INSERT_FILE, "&Insert File", "Insert the contents of another file")
	MenuLabel(AP_MENU_ID_INSERT_SYMBOL,		"Sy&mbol",			"Insert a symbol or other special character")
	MenuLabel(AP_MENU_ID_INSERT_ENDNOTE,	"&Endnote",			"Insert an endnote")
	MenuLabel(AP_MENU_ID_INSERT_PICTURE, 	"&Picture", 		"Insert a picture")
	MenuLabel(AP_MENU_ID_INSERT_CLIPART, 	"&Clip Art", 		"Insert clipart")
	MenuLabel(AP_MENU_ID_INSERT_GRAPHIC,	"&From File",		"Insert an existing picture from another file")
	MenuLabel(AP_MENU_ID_INSERT_BOOKMARK,	"&Bookmark",		"Insert bookmark")
	MenuLabel(AP_MENU_ID_INSERT_HYPERLINK,	"&Hyperlink",		"Insert hyperlink")

	MenuLabel(AP_MENU_ID_FORMAT,			"F&ormat",			NULL)
	MenuLabel(AP_MENU_ID_FMT_LANGUAGE,		"Set &Language",		"Change the language of the selected text")
	MenuLabel(AP_MENU_ID_FMT_FONT,			"&Font",			"Change the font of the selected text")
	MenuLabel(AP_MENU_ID_FMT_PARAGRAPH,		"&Paragraph",		"Change the format of the selected paragraph")
	MenuLabel(AP_MENU_ID_FMT_BULLETS,		"Bullets and &Numbering",	"Add or modify bullets and numbering for selected paragraphs")
	MenuLabel(AP_MENU_ID_FMT_DOCUMENT,		"&Document",             "Setup your document's page properties such as page size and margins")
	MenuLabel(AP_MENU_ID_FMT_BORDERS,		"Borders and Shading",		"Add borders and shading to the selection")
	MenuLabel(AP_MENU_ID_FMT_HDRFTR,		"Format Header/Footers",   "Set the types of Headers and Footers")
	MenuLabel(AP_MENU_ID_FMT_COLUMNS,		"&Columns",			"Change the number of columns")
	MenuLabel(AP_MENU_ID_FMT_TOGGLECASE,   	"C&hange Case", "Change the case of the selected text")
	MenuLabel(AP_MENU_ID_FMT_BACKGROUND,   	"Back&ground", "Change your document's background color")
	MenuLabel(AP_MENU_ID_FMT_STYLE,			"St&yle",			"Define or apply style for the selection")
	MenuLabel(AP_MENU_ID_FMT_TABS,			"&Tabs",			"Set tab stops")
	MenuLabel(AP_MENU_ID_FMT_BOLD,			"&Bold",			"Make the selection bold (toggle)")
	MenuLabel(AP_MENU_ID_FMT_ITALIC,		"&Italic",			"Make the selection italic (toggle)")
	MenuLabel(AP_MENU_ID_FMT_UNDERLINE,		"&Underline",		"Underline the selection (toggle)")
	MenuLabel(AP_MENU_ID_FMT_OVERLINE,		"&Overline",		"Overline the selection (toggle)")
	MenuLabel(AP_MENU_ID_FMT_STRIKE,		"Stri&ke",			"Strikeout the selection (toggle)")
	MenuLabel(AP_MENU_ID_FMT_TOPLINE,		"Topline",			"Line above the selection (toggle)")
	MenuLabel(AP_MENU_ID_FMT_BOTTOMLINE,	"Bottomline",		"Line below the selection (toggle)")
	MenuLabel(AP_MENU_ID_FMT_SUPERSCRIPT,	"Supe&rscript",		"Make the selection superscript (toggle)")
	MenuLabel(AP_MENU_ID_FMT_SUBSCRIPT,		"&Subscript",		"Make the selection subscript (toggle)")

	MenuLabel(AP_MENU_ID_TOOLS,				"&Tools",			NULL)   
	MenuLabel(AP_MENU_ID_TOOLS_SPELLING, "&Spelling", NULL)
	MenuLabel(AP_MENU_ID_TOOLS_SPELL,		"Check &Spelling",		"Check the document for incorrect spelling")
	MenuLabel(AP_MENU_ID_TOOLS_AUTOSPELL, "&Auto Spellcheck", "Automatically spell-check the document")
     MenuLabel(AP_MENU_ID_TOOLS_SPELLPREFS, "Spelling &Options", "Set your spelling preferences")
	MenuLabel(AP_MENU_ID_TOOLS_WORDCOUNT,	"&Word Count",		"Count the number of words in the document")
	MenuLabel(AP_MENU_ID_TOOLS_OPTIONS,		"&Preferences",		"Set preferences")
	MenuLabel(AP_MENU_ID_TOOLS_LANGUAGE, "&Language", "Change the language of the selected text")
	MenuLabel(AP_MENU_ID_TOOLS_PLUGINS, "P&lugins", "Manage plugins")
	MenuLabel(AP_MENU_ID_TOOLS_SCRIPTS, "S&cripts", "Execute helper scripts")

	MenuLabel(AP_MENU_ID_ALIGN,				"&Align",			NULL)
	MenuLabel(AP_MENU_ID_ALIGN_LEFT,		"&Left",			"Left-justify the paragraph")
	MenuLabel(AP_MENU_ID_ALIGN_CENTER,		"&Center",			"Center-align the paragraph")
	MenuLabel(AP_MENU_ID_ALIGN_RIGHT,		"&Right",			"Right-justify the paragraph")
	MenuLabel(AP_MENU_ID_ALIGN_JUSTIFY,		"&Justify",			"Justify the paragraph")

	MenuLabel(AP_MENU_ID_WEB, "We&b", NULL)
	MenuLabel(AP_MENU_ID_WEB_SAVEASWEB,    "Save as &web",		"Save the document as a web page")
	MenuLabel(AP_MENU_ID_WEB_WEBPREVIEW,   "Pre&view web page", "Preview the document as a web page")

	MenuLabel(AP_MENU_ID_WINDOW,			"&Window",			NULL)
	MenuLabel(AP_MENU_ID_WINDOW_NEW,		"&New Window",		"Open another window for the document")
	MenuLabel(AP_MENU_ID_WINDOW_1,			"&1 %s",			"Raise this window")
	MenuLabel(AP_MENU_ID_WINDOW_2,			"&2 %s",			"Raise this window")
	MenuLabel(AP_MENU_ID_WINDOW_3,			"&3 %s",			"Raise this window")
	MenuLabel(AP_MENU_ID_WINDOW_4,			"&4 %s",			"Raise this window")
	MenuLabel(AP_MENU_ID_WINDOW_5,			"&5 %s",			"Raise this window")
	MenuLabel(AP_MENU_ID_WINDOW_6,			"&6 %s",			"Raise this window")
	MenuLabel(AP_MENU_ID_WINDOW_7,			"&7 %s",			"Raise this window")
	MenuLabel(AP_MENU_ID_WINDOW_8,			"&8 %s",			"Raise this window")
	MenuLabel(AP_MENU_ID_WINDOW_9,			"&9 %s",			"Raise this window")
	MenuLabel(AP_MENU_ID_WINDOW_MORE,		"&More Windows",	"Show full list of windows")

	MenuLabel(AP_MENU_ID_HELP,			"&Help",		NULL)
     MenuLabel(AP_MENU_ID_HELP_CREDITS, "C&redits", "Display Credits")
	MenuLabel(AP_MENU_ID_HELP_CONTENTS,		"Help &Contents",	"Display Help Contents")
	MenuLabel(AP_MENU_ID_HELP_INDEX,		"Help &Index",		"Display Help Index")
	MenuLabel(AP_MENU_ID_HELP_CHECKVER,		"Check &Version",	"Display program version number")
	MenuLabel(AP_MENU_ID_HELP_SEARCH,		"&Search for Help",	"Search for help about...")
	MenuLabel(AP_MENU_ID_HELP_ABOUT,		"&About %s",		"Display program information, version number, and copyright") 
	MenuLabel(AP_MENU_ID_HELP_ABOUTOS,		"About &Open Source",	"Display information about Open Source")
     MenuLabel(AP_MENU_ID_HELP_ABOUT_GNU, "About &GNU Free Software", "About the GNU project")
     MenuLabel(AP_MENU_ID_HELP_ABOUT_GNOMEOFFICE, "About G&NOME Office", "About the GNOME Office project")

	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_1,	"%s",				"Change to this suggested spelling")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_2,	"%s",				"Change to this suggested spelling")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_3,	"%s",				"Change to this suggested spelling")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_4,	"%s",				"Change to this suggested spelling")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_5,	"%s",				"Change to this suggested spelling")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_6,	"%s",				"Change to this suggested spelling")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_7,	"%s",				"Change to this suggested spelling")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_8,	"%s",				"Change to this suggested spelling")
	MenuLabel(AP_MENU_ID_SPELL_SUGGEST_9,	"%s",				"Change to this suggested spelling")
	MenuLabel(AP_MENU_ID_SPELL_IGNOREALL,	"&Ignore All", 		"Ignore all occurrences of this word in the document")
	MenuLabel(AP_MENU_ID_SPELL_ADD,			"&Add", 			"Add this word to the custom dictionary")

	/* autotext submenu labels */
	MenuLabel(AP_MENU_ID_INSERT_AUTOTEXT, "&Autotext", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_ATTN, "Attention:", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_CLOSING, "Closing:", "") 
	MenuLabel(AP_MENU_ID_AUTOTEXT_MAIL, "Mail Instructions:", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_REFERENCE, "Reference:", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_SALUTATION, "Salutation:", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_SUBJECT, "Subject:", "")
	MenuLabel(AP_MENU_ID_AUTOTEXT_EMAIL, "Email:", "")

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
