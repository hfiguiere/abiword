/* AbiSource Application Framework
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
** MORE THAN ONE TIME.
******************************************************************
*****************************************************************/

dcl(DLG_OK,					"OK")
dcl(DLG_Cancel,				"Cancel")

/* More Windows dialog */
dcl(DLG_MoreWindows,		"More Windows")
dcl(DLG_Activate,			"Activate")

/* Unix Font Selector dialog */
dcl(DLG_FontTitle,			"Font")
dcl(DLG_FontLabel,			"Font:")
dcl(DLG_StyleLabel,			"Style:")
dcl(DLG_SizeLabel,			"Size:")
dcl(DLG_EncodingLabel,		"Encoding:")
dcl(DLG_EffectsFrameLabel,	"Effects")
dcl(DLG_StrikeoutCheck,		"Strikeout")
dcl(DLG_UnderlineCheck,		"Underline")
dcl(DLG_FontTab,			"   Font   ")
dcl(DLG_ColorTab,			"   Color   ")
dcl(DLG_StyleRegular,		"Regular")
dcl(DLG_StyleItalic,		"Italic")
dcl(DLG_StyleBold,			"Bold")
dcl(DLG_StyleBoldItalic,	"Bold Italic")

/* Unix FileOpenSaveAs Dialog */
dcl(DLG_FOSA_OpenTitle,					"Open File")
dcl(DLG_FOSA_SaveAsTitle,				"Save File As")
dcl(DLG_FOSA_PrintToFileTitle,			"Print To File")
dcl(DLG_InvalidPathname,				"Invalid pathname.")
dcl(DLG_NoSaveFile_DirNotExist,			"A directory in the given pathname does not exist.")
dcl(DLG_NoSaveFile_DirNotWriteable,		"The directory '%s' is write-protected.")
dcl(DLG_OverwriteFile,					"File already exists.  Overwrite file '%s'?")
