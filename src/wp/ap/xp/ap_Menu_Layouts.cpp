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


#include "ut_types.h"
#include "ut_string.h"
#include "ut_assert.h"
#include "xap_Menu_Layouts.h"
#include "ap_Menu_Id.h"
#include "ut_debugmsg.h"
#include "ev_Menu_Labels.h"
#include "xap_Menu_ActionSet.h"
#include "ap_Menu_Id.h"
#include "xap_EncodingManager.h"
#include "ev_Menu.h"
#include "ap_Strings.h"

	struct _lt
	{
		EV_Menu_LayoutFlags			m_flags;
		XAP_Menu_Id					m_id;
	};

	struct _tt
	{
		const char *				m_name;
		UT_uint32					m_nrEntries;
		struct _lt *				m_lt;
		EV_EditMouseContext			m_emc;
	};

	class _vectt
	{
	public:
		_vectt(_tt orig)
			{
				m_name = orig.m_name;
		        m_emc = orig.m_emc;
				m_Vec_lt.clear();
				UT_uint32 k = 0;
				for(k = 0; k < orig.m_nrEntries; k++)
				{
					_lt * plt = new _lt;
					*plt = orig.m_lt[k];
					m_Vec_lt.addItem((void *) plt);
				}
			};
		~_vectt()
			{
				UT_VECTOR_PURGEALL(_lt *,m_Vec_lt);
			};
		UT_uint32 getNrEntries(void)
			{
				return m_Vec_lt.getItemCount();
			};
		_lt * getNth_lt(UT_uint32 n)
			{
				return (_lt *) m_Vec_lt.getNthItem(n);
			};
		void insertItemAt(void * p, XAP_Menu_Id id)
			{
				UT_uint32 i = 0;
				bool bFound = false;
				for(i=0; i< m_Vec_lt.getItemCount(); i++)
				{
					_lt * plt = (_lt *) m_Vec_lt.getNthItem(i);
					if(plt->m_id == id)
					{
						if((i+1) == m_Vec_lt.getItemCount())
						{
							m_Vec_lt.addItem(p);
						}
						else
						{
							m_Vec_lt.insertItemAt(p,i+1);
						}
						bFound = true;
					}
				}
				UT_ASSERT(bFound);
			};
		void setNthIDFlags(UT_uint32 n, XAP_Menu_Id id,EV_Menu_LayoutFlags flags)
			{
				_lt * plt = (_lt *) m_Vec_lt.getNthItem(n);
				plt->m_flags = flags;
				plt->m_id = id;
			};
		const char *				m_name;
		EV_EditMouseContext			m_emc;
	private:
		UT_Vector   				m_Vec_lt;
	};


/*****************************************************************
******************************************************************
** Here we begin a little CPP magic to load the layout for each
** menu layout in the application.  It is important that all of
** the ...Layout_*.h files allow themselves to be included more
** than one time.
******************************************************************
*****************************************************************/

#define BeginLayout(Name,Cxt)	static struct _lt s_ltTable_##Name[] = {
#define MenuItem(id)			{ EV_MLF_Normal,		(id)				 },
#define BeginSubMenu(id)		{ EV_MLF_BeginSubMenu,	(id)				 },
#define BeginPopupMenu()		{ EV_MLF_BeginPopupMenu,AP_MENU_ID__BOGUS1__ },
#define Separator()				{ EV_MLF_Separator,		AP_MENU_ID__BOGUS1__ },
#define EndSubMenu()			{ EV_MLF_EndSubMenu,	AP_MENU_ID__BOGUS1__ },
#define EndPopupMenu()			{ EV_MLF_EndPopupMenu,	AP_MENU_ID__BOGUS1__ },
#define EndLayout()				};

#include "ap_Menu_Layouts_All.h"

#undef BeginLayout
#undef MenuItem
#undef BeginSubMenu
#undef BeginPopupMenu
#undef Separator
#undef EndSubMenu
#undef EndPopupMenu
#undef EndLayout


/*****************************************************************
******************************************************************
** Here we begin a little CPP magic to construct a table containing
** the names and addresses of all the tables we constructed in the
** previous section.
******************************************************************
*****************************************************************/

#define BeginLayout(Name,Cxt)	{ #Name, NrElements(s_ltTable_##Name), s_ltTable_##Name, Cxt },
#define MenuItem(id)			/*nothing*/
#define BeginSubMenu(id)		/*nothing*/
#define BeginPopupMenu()		/*nothing*/
#define Separator()				/*nothing*/
#define EndSubMenu()			/*nothing*/
#define EndPopupMenu()			/*nothing*/
#define EndLayout()				/*nothing*/

static struct _tt s_ttTable[] =
{

#include "ap_Menu_Layouts_All.h"
	
};

#undef BeginLayout
#undef MenuItem
#undef BeginSubMenu
#undef BeginPopupMenu
#undef Separator
#undef EndSubMenu
#undef EndPopupMenu
#undef EndLayout



/*****************************************************************
******************************************************************
** Put it all together and have a "load Layout by Name"
******************************************************************
*****************************************************************/


/*!
 * Load these cleverly constructed static tables into vectors so they can be 
 * manipulated dynamically.
 */
XAP_Menu_Factory::XAP_Menu_Factory(XAP_App * pApp) :
		m_pApp(pApp),
        m_maxID(0),
        m_pLabelSet(NULL)
{
	UT_uint32 k = 0;
	m_vecTT.clear();
	for (k=0; k<NrElements(s_ttTable); k++)
	{
		_vectt * pVectt = new _vectt(s_ttTable[k]);
		m_vecTT.addItem((void *) pVectt);
	}
}

XAP_Menu_Factory::~XAP_Menu_Factory()
{
    UT_VECTOR_PURGEALL(_vectt *,m_vecTT);
}

XAP_Menu_Id XAP_Menu_Factory::getNewID(void)
{
	if(m_maxID > 0)
	{
		m_maxID++;
		return m_maxID;
	}
	UT_uint32 i,j =0;
	for(i=0; i < m_vecTT.getItemCount(); i++)
	{
		_vectt * pTT = (_vectt *) m_vecTT.getNthItem(i);
		for(j=0; j < pTT->getNrEntries(); j++)
		{
			_lt * plt = (_lt *) pTT->getNth_lt(j);
			if(plt->m_id > m_maxID)
			{
				m_maxID = plt->m_id;
			}
		}
	}
	m_maxID++;
	return m_maxID;
}
		

EV_Menu_Layout * XAP_Menu_Factory::CreateMenuLayout(const char * szName)
{
	UT_ASSERT(szName && *szName);		// no defaults

	for (UT_uint32 k=0; k< m_vecTT.getItemCount(); k++)
	{
		_vectt * pVectt = (_vectt *)m_vecTT.getNthItem(k);
		if (UT_stricmp(szName,pVectt->m_name)==0)
		{
			UT_uint32 NrEntries = pVectt->getNrEntries();
			EV_Menu_Layout * pLayout = new EV_Menu_Layout(pVectt->m_name,NrEntries);
			UT_DEBUGMSG(("pLayout 0x%x (1)\n", pLayout));
			UT_ASSERT(pLayout);
			
			for (UT_uint32 j=0; (j < NrEntries); j++)
			{
				_lt * plt = pVectt->getNth_lt(j);
				UT_DEBUGMSG(("SEVIOR: Adding jth %d id %d, flags 0x%x to menu %s \n",j, plt->m_id,plt->m_flags, pVectt->m_name));
				bool bResult = pLayout->setLayoutItem(j, plt->m_id, plt->m_flags);
				UT_ASSERT(bResult);
			}
			UT_DEBUGMSG(("pLayout 0x%x (2)\n", pLayout));

			return pLayout;
		}
	}
	UT_ASSERT(0);						// no defaults
	return NULL;
}

const char * XAP_Menu_Factory::FindContextMenu(EV_EditMouseContext emc)
{

	for (UT_uint32 k=0; k< m_vecTT.getItemCount(); k++)
	{
		_vectt * pVectt = (_vectt *) m_vecTT.getNthItem(k);
		if (emc == pVectt->m_emc)
		{
			return pVectt->m_name;
		}
	}
	UT_ASSERT(UT_NOT_IMPLEMENTED);
	return NULL;
}

XAP_Menu_Id XAP_Menu_Factory::addNewMenuAfter(const char * szMenu, 
											  const char * szLanguage,
											  const char * szAfter, 
											  EV_Menu_LayoutFlags flags,
											  XAP_Menu_Id newID)
{
	UT_ASSERT(szMenu && *szMenu);		// no defaults
	UT_uint32 k = 0;
	bool bFoundMenu = false;
	_vectt * pVectt = NULL;
	for (k=0; (k< m_vecTT.getItemCount()) && !bFoundMenu; k++)
	{
		pVectt = (_vectt *)m_vecTT.getNthItem(k);
		bFoundMenu = (UT_stricmp(szMenu,pVectt->m_name)==0);
	}
	if(!bFoundMenu)
	{
		return 0;
	}
	
// next we need to find the id of the label name
//
// OK now search for this label
//
	UT_String After = szAfter;
	XAP_Menu_Id afterID = EV_searchMenuLabel( m_pLabelSet, After);
	if(afterID == 0)
	{
		return 0;
	}
//
// OK got the menu ID at last, insert the new id here.
//
	if(newID == 0)
	{
		newID = getNewID();
	}
//
// Now insert our new ID
//
	_lt * plt = new _lt;
	plt->m_id = newID;
	plt->m_flags = flags;
	pVectt->insertItemAt((void *) plt, afterID);
	return (XAP_Menu_Id) newID;
}


XAP_Menu_Id XAP_Menu_Factory::addNewMenuBefore(const char * szMenu, 
											   const char * szLanguage,
											   const char * szAfter, 
											   EV_Menu_LayoutFlags flags)
{
	return 0;
}

XAP_Menu_Id XAP_Menu_Factory::removeMenuItem(const char * szMenu,
									  const char * szLanguage,
											 const char * szNuke)
{
	return 0;
}

void XAP_Menu_Factory::resetMenusToDefault(void)
{
    UT_VECTOR_PURGEALL(_vectt *,m_vecTT);
	m_vecTT.clear();
	UT_uint32 k = 0;
	for (k=0; k<NrElements(s_ttTable); k++)
	{
		_vectt * pVectt = new _vectt(s_ttTable[k]);
		m_vecTT.addItem((void *) pVectt);
	}
}

EV_Menu_LabelSet *  XAP_Menu_Factory::CreateMenuLabelSet(const char * szLanguage_)
{
	char buf[300];
	strcpy(buf,szLanguage_ ? szLanguage_ : "");
	char* szLanguage = buf;

	char* dot = strrchr(szLanguage,'.');
	if (dot)
		*dot = '\0'; /* remove encoding part from locale name */

	UT_DEBUGMSG(("CreateMenuLabelSet: szLanguage_ %s, szLanguage %s\n"
				,szLanguage_,szLanguage));


	const XAP_StringSet * pSS = XAP_App::getApp()->getStringSet();

	if( !m_pLabelSet )
	{
		m_pLabelSet = new EV_Menu_LabelSet(szLanguage,AP_MENU_ID__BOGUS1__,AP_MENU_ID__BOGUS2__);	
		#define menuitem(id) \
			m_pLabelSet->setLabel( (AP_MENU_ID_##id),	\
								    pSS->getValue(AP_STRING_ID_MENU_LABEL_##id), \
								    pSS->getValue(AP_STRING_ID_MENU_STATUSLINE_##id) );
			#include "ap_Menu_Id_List.h"
		#undef menuitem
	}

	return new EV_Menu_LabelSet(m_pLabelSet);
}

UT_uint32 XAP_Menu_Factory::GetMenuLabelSetLanguageCount(void)
{
	return 1;
}

const char * XAP_Menu_Factory::GetNthMenuLabelLanguageName(UT_uint32 ndx)
{
	return m_pLabelSet->getLanguage();
}

/*!
 * afterID has just been created. All labels of value equal or greater 
 * must be incremented
 */
bool XAP_Menu_Factory::addNewLabel(const char * szLanguage, 
										XAP_Menu_Id newID, 
										const char * szNewName,
										const char * szNewTooltip)
{
	// TODO incrementing label IDs??
	EV_Menu_Label * newLab = new EV_Menu_Label(newID,szNewName,szNewTooltip);
	return m_pLabelSet->addLabel(newLab);
}

bool XAP_Menu_Factory::removeLabel(const char * szLanguage, 
									   XAP_Menu_Id nukeID)
{
	return false;
}

void XAP_Menu_Factory::resetLabelsToDefault(void)
{
	// TODO - do nothing as extra labels in set don't hurt operation
}

