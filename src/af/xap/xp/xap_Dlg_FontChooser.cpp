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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ut_string.h"
#include "ut_debugmsg.h"
#include "xap_Dlg_FontChooser.h"

/*****************************************************************/

XAP_Dialog_FontChooser::XAP_Dialog_FontChooser(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id)
	: XAP_Dialog_NonPersistent(pDlgFactory,id)
{
	m_answer				= a_CANCEL;
	m_pGraphics				= NULL;
	m_pFontFamily			= NULL;
	m_pFontSize				= NULL;
	m_pFontWeight			= NULL;
	m_pFontStyle			= NULL;
	m_pColor				= NULL;
	m_bUnderline			= UT_FALSE;
	m_bStrikeOut			= UT_FALSE;

	m_bChangedFontFamily	= UT_FALSE;
	m_bChangedFontSize		= UT_FALSE;
	m_bChangedFontWeight	= UT_FALSE;
	m_bChangedFontStyle		= UT_FALSE;
	m_bChangedColor			= UT_FALSE;
	m_bChangedUnderline		= UT_FALSE;
	m_bChangedStrikeOut		= UT_FALSE;
}

XAP_Dialog_FontChooser::~XAP_Dialog_FontChooser(void)
{
	FREEP(m_pFontFamily);
	FREEP(m_pFontSize);
	FREEP(m_pFontWeight);
	FREEP(m_pFontStyle);
	FREEP(m_pColor);
}

void XAP_Dialog_FontChooser::setGraphicsContext(GR_Graphics * pGraphics)
{
	m_pGraphics = pGraphics;
}

void XAP_Dialog_FontChooser::setFontFamily(const XML_Char * pFontFamily)
{
	CLONEP((char *&) m_pFontFamily, pFontFamily);
}

void XAP_Dialog_FontChooser::setFontSize(const XML_Char * pFontSize)
{
	CLONEP((char *&) m_pFontSize, pFontSize);
}

void XAP_Dialog_FontChooser::setFontWeight(const XML_Char * pFontWeight)
{
	CLONEP((char *&) m_pFontWeight, pFontWeight);
}

void XAP_Dialog_FontChooser::setFontStyle(const XML_Char * pFontStyle)
{
	CLONEP((char *&)m_pFontStyle, pFontStyle);
}

void XAP_Dialog_FontChooser::setColor(const XML_Char * pColor)
{
	CLONEP((char *&)m_pColor, pColor);
}

void XAP_Dialog_FontChooser::setFontDecoration(UT_Bool bUnderline, UT_Bool bStrikeOut)
{
	m_bUnderline = bUnderline;
	m_bStrikeOut = bStrikeOut;
}


XAP_Dialog_FontChooser::tAnswer XAP_Dialog_FontChooser::getAnswer(void) const
{
	return m_answer;
}

UT_Bool XAP_Dialog_FontChooser::getChangedFontFamily(const XML_Char ** pszFontFamily) const
{
	if (pszFontFamily)
		*pszFontFamily = m_pFontFamily;
	return m_bChangedFontFamily;
}

UT_Bool XAP_Dialog_FontChooser::getChangedFontSize(const XML_Char ** pszFontSize) const
{
	if (pszFontSize)
		*pszFontSize = m_pFontSize;
	return m_bChangedFontSize;
}

UT_Bool XAP_Dialog_FontChooser::getChangedFontWeight(const XML_Char ** pszFontWeight) const
{
	if (pszFontWeight)
		*pszFontWeight = m_pFontWeight;
	return m_bChangedFontWeight;
}

UT_Bool XAP_Dialog_FontChooser::getChangedFontStyle(const XML_Char ** pszFontStyle) const
{
	if (pszFontStyle)
		*pszFontStyle = m_pFontStyle;
	return m_bChangedFontStyle;
}

UT_Bool XAP_Dialog_FontChooser::getChangedColor(const XML_Char ** pszColor) const
{
	if (pszColor)
		*pszColor = m_pColor;
	return m_bChangedColor;
}

UT_Bool XAP_Dialog_FontChooser::getChangedUnderline(UT_Bool * pbUnderline) const
{
	if (pbUnderline)
		*pbUnderline = m_bUnderline;
	return m_bChangedUnderline;
}

UT_Bool XAP_Dialog_FontChooser::getChangedStrikeOut(UT_Bool * pbStrikeOut) const
{
	if (pbStrikeOut)
		*pbStrikeOut = m_bStrikeOut;
	return m_bChangedStrikeOut;
}
