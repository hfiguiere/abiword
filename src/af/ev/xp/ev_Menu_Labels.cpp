/* AbiSource Program Utilities
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

#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ev_Menu_Labels.h"

/*****************************************************************/

EV_Menu_Label::EV_Menu_Label(XAP_Menu_Id id,
							 const char * szMenuLabel,
							 const char * szStatusMsg)
{
	m_id = id;
	UT_cloneString(m_szMenuLabel,szMenuLabel);
	UT_cloneString(m_szStatusMsg,szStatusMsg);
}

EV_Menu_Label::~EV_Menu_Label(void)
{
	FREEP(m_szMenuLabel);
	FREEP(m_szStatusMsg);
}

XAP_Menu_Id EV_Menu_Label::getMenuId(void) const
{
	return m_id;
}

const char * EV_Menu_Label::getMenuLabel(void) const
{
	return m_szMenuLabel;
}

const char * EV_Menu_Label::getMenuStatusMessage(void) const
{
	return m_szStatusMsg;
}

/*****************************************************************/

EV_Menu_LabelSet::EV_Menu_LabelSet(const char * szLanguage,
								   XAP_Menu_Id first, XAP_Menu_Id last)
{
	// TODO tis bad to call malloc/calloc from a constructor, since we cannot report failure.
	// TODO move this allocation to somewhere else.
	UT_cloneString(m_szLanguage,szLanguage);
	m_labelTable = (EV_Menu_Label **)calloc((last-first+1),sizeof(EV_Menu_Label *));
	m_first = first;
	m_last = last;
}

EV_Menu_LabelSet::~EV_Menu_LabelSet(void)
{
	FREEP(m_szLanguage);

	if (!m_labelTable)
		return;

	UT_uint32 k, kLimit;
	for (k=0, kLimit=(m_last-m_first+1); (k<kLimit); k++)
		DELETEP(m_labelTable[k]);
	free(m_labelTable);
}

bool EV_Menu_LabelSet::setLabel(XAP_Menu_Id id,
								   const char * szMenuLabel,
								   const char * szStatusMsg)
{
	if ((id < m_first) || (id > m_last))
		return false;

	UT_uint32 index = (id - m_first);
	DELETEP(m_labelTable[index]);
	m_labelTable[index] = new EV_Menu_Label(id,szMenuLabel,szStatusMsg);
	return (m_labelTable[index] != NULL);
}

#ifdef __MRC__
EV_Menu_Label * EV_Menu_LabelSet::getLabel(XAP_Menu_Id id)
#else
EV_Menu_Label * EV_Menu_LabelSet::getLabel(XAP_Menu_Id id) const
#endif
{
	if ((id < m_first) || (id > m_last))
		return NULL;

	UT_uint32 index = (id - m_first);
	
	EV_Menu_Label * pLabel = m_labelTable[index];

	// IDEA: some labelsets are sparse because their translation is behind
	// HACK: if no label, create a fallback JIT so we don't fail downstream
	// TODO: fall back to English instead like strings do (but not here)
	if (!pLabel)
	{
		UT_DEBUGMSG(("WARNING: %s translation for menu id [%d] not found.\n",m_szLanguage,id));
		// NOTE: only translators should see the following strings
		// NOTE: do *not* translate them
		pLabel = new EV_Menu_Label(id,"TODO","untranslated menu item");
		m_labelTable[index] = pLabel;
	}

	UT_ASSERT(pLabel && (pLabel->getMenuId()==id));
	return pLabel;
}

const char * EV_Menu_LabelSet::getLanguage(void) const
{
	return m_szLanguage;
}

void EV_Menu_LabelSet::setLanguage(const char *szLanguage)
{
	FREEP(m_szLanguage);
	UT_cloneString(m_szLanguage, szLanguage);
}

