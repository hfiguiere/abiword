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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ut_types.h"
#include "ut_assert.h"
#include "ut_string.h"
#include "ut_units.h"
#include "ap_Toolbar_Id.h"
#include "ap_Toolbar_Functions.h"
#include "ev_Toolbar_Actions.h"
#include "ev_Toolbar_Labels.h"
#include "xap_App.h"
#include "xap_Clipboard.h"
#include "xap_Frame.h"
#include "fv_View.h"


#define ABIWORD_VIEW  	FV_View * pView = static_cast<FV_View *>(pAV_View)

/****************************************************************/

Defun_EV_GetToolbarItemState_Fn(ap_ToolbarGetState_Changes)
{
	ABIWORD_VIEW;
	UT_ASSERT(pView);

	EV_Toolbar_ItemState s = EV_TIS_ZERO;

	switch(id)
	{
	case AP_TOOLBAR_ID_EDIT_UNDO:
		if (!pView->canDo(UT_TRUE))
			s = EV_TIS_Gray;
		break;

	case AP_TOOLBAR_ID_EDIT_REDO:
		if (!pView->canDo(UT_FALSE))
			s = EV_TIS_Gray;
		break;

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		break;
	}

	return s;
}

Defun_EV_GetToolbarItemState_Fn(ap_ToolbarGetState_Selection)
{
	ABIWORD_VIEW;
	UT_ASSERT(pView);

	EV_Toolbar_ItemState s = EV_TIS_ZERO;

	switch(id)
	{
	case AP_TOOLBAR_ID_EDIT_CUT:
	case AP_TOOLBAR_ID_EDIT_COPY:
		if (pView->isSelectionEmpty())
			s = EV_TIS_Gray;
		break;

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		break;
	}

	return s;
}

Defun_EV_GetToolbarItemState_Fn(ap_ToolbarGetState_Clipboard)
{
	ABIWORD_VIEW;
	UT_ASSERT(pView);

	EV_Toolbar_ItemState s = EV_TIS_ZERO;
	AP_Clipboard* pClip = AP_App::getClipboard();

	switch(id)
	{
	case AP_TOOLBAR_ID_EDIT_PASTE:
		// TODO handle UNICODE text pasting
		s = EV_TIS_Gray;
		if (pClip->open())
		{
			if (
				(pClip->hasFormat(AP_CLIPBOARD_TEXTPLAIN_8BIT))
// for now we only copy with 8bit text from the clip				
//				|| (pClip->hasFormat(AP_CLIPBOARD_RTF))
				)
			{
				s = EV_TIS_ZERO;
			}
			pClip->close();
		}
		break;

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		break;
	}

	return s;
}

Defun_EV_GetToolbarItemState_Fn(ap_ToolbarGetState_CharFmt)
{
	ABIWORD_VIEW;
	UT_ASSERT(pView);
	UT_Bool bMultiple = UT_FALSE;
	UT_Bool bSize = UT_FALSE;
	UT_Bool bString = UT_FALSE;

	EV_Toolbar_ItemState s = EV_TIS_ZERO;

	const XML_Char * prop = NULL;
	const XML_Char * val  = NULL;

	switch(id)
	{
	case AP_TOOLBAR_ID_FMT_FONT:
		prop = "font-family";
		val  = "";
		bString = UT_TRUE;
		break;

	case AP_TOOLBAR_ID_FMT_SIZE:
		prop = "font-size";
		val  = "";
		bSize = UT_TRUE;
		break;

	case AP_TOOLBAR_ID_FMT_BOLD:
		prop = "font-weight";
		val  = "bold";
		break;

	case AP_TOOLBAR_ID_FMT_ITALIC:
		prop = "font-style";
		val  = "italic";
		break;

	case AP_TOOLBAR_ID_FMT_UNDERLINE:
		prop = "text-decoration";
		val  = "underline";
		bMultiple = UT_TRUE;
		break;

	case AP_TOOLBAR_ID_FMT_STRIKE:
		prop = "text-decoration";
		val  = "line-through";
		bMultiple = UT_TRUE;
		break;

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		break;
	}

	if (prop && val)
	{
		// get current font info from pView
		const XML_Char ** props_in = NULL;
		const XML_Char * sz;

		if (!pView->getCharFormat(&props_in))
			return s;

		sz = UT_getAttribute(prop, props_in);
		if (sz)
		{
			if (bSize)
			{	
				static char buf[5];
				int pt = (int) UT_convertToPoints(sz);
				sprintf(buf, "%d", pt);
				*pszState = buf;
				s = EV_TIS_UseString;
			}
			else if (bString)
			{	
				static const char * sz2;
				sz2 = sz;
				*pszState = sz2;
				s = EV_TIS_UseString;
			}
			else if (bMultiple)
			{	
				// some properties have multiple values
				if (strstr(sz, val))
					s = EV_TIS_Toggled;
			}
			else
			{
				if (0 == UT_stricmp(sz, val))
					s = EV_TIS_Toggled;
			}
		}
		
		free(props_in);
	}

	return s;
}

Defun_EV_GetToolbarItemState_Fn(ap_ToolbarGetState_BlockFmt)
{
	ABIWORD_VIEW;
	UT_ASSERT(pView);

	EV_Toolbar_ItemState s = EV_TIS_ZERO;

	const XML_Char * prop = "text-align";
	const XML_Char * val  = NULL;

	switch(id)
	{
	case AP_TOOLBAR_ID_ALIGN_LEFT:
		val  = "left";
		break;

	case AP_TOOLBAR_ID_ALIGN_CENTER:
		val  = "center";
		break;

	case AP_TOOLBAR_ID_ALIGN_RIGHT:
		val  = "right";
		break;

	case AP_TOOLBAR_ID_ALIGN_JUSTIFY:
		val  = "justify";
		break;

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		break;
	}

	if (prop && val)
	{
		// get current font info from pView
		const XML_Char ** props_in = NULL;
		const XML_Char * sz;

		if (!pView->getBlockFormat(&props_in))
			return s;

		sz = UT_getAttribute(prop, props_in);
		if (sz && (0 == UT_stricmp(sz, val)))
			s = EV_TIS_Toggled;
		
		free(props_in);
	}

	return s;
}
