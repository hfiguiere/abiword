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

#ifndef EV_QNXMENU_H
#define EV_QNXMENU_H

#include "ut_types.h"
#include "ut_vector.h"
#include "xap_Types.h"
#include "ev_Menu.h"
#include <Pt.h>

class AV_View;
class XAP_QNXApp;
class XAP_QNXFrame;


/*****************************************************************/

class EV_QNXMenu : public EV_Menu
{
public:
	EV_QNXMenu(XAP_QNXApp * pQNXApp,
				XAP_Frame * pFrame,
				const char * szMenuLayoutName,
				const char * szMenuLabelSetName);
	virtual ~EV_QNXMenu(void);

	bool				synthesizeMenu(PtWidget_t * wMenuRoot);
	bool				synthesizeMenu(void * wMenuRoot);
	bool				menuEvent(XAP_Menu_Id id);
	virtual bool		refreshMenu(AV_View * pView) = 0;

	XAP_Frame * 	getFrame(void);

protected:

	bool				_refreshMenu(AV_View * pView, void * wMenuRoot);
	bool				_isItemPresent(XAP_Menu_Id id) const;

	XAP_QNXApp *		m_pQNXApp;
	XAP_Frame *		m_pFrame;

	UT_Vector			m_vecMenuWidgets;

	virtual bool		_doAddMenuItem(UT_uint32 layout_pos);
};

/*****************************************************************/

class EV_QNXMenuBar : public EV_QNXMenu
{
public:
	EV_QNXMenuBar(XAP_QNXApp * pQNXApp,
				   XAP_Frame * pFrame,
				   const char * szMenuLayoutName,
				   const char * szMenuLabelSetName);
	virtual ~EV_QNXMenuBar(void);

	bool				synthesizeMenuBar(void);
	virtual bool		refreshMenu(AV_View * pView);

protected:
	PtWidget_t *			m_wMenuBar;
	void * 		m_wHandleBox;
};

/*****************************************************************/

class EV_QNXMenuPopup : public EV_QNXMenu
{
public:
	EV_QNXMenuPopup(XAP_QNXApp * pQNXApp,
					 XAP_Frame * pFrame,
					 const char * szMenuLayoutName,
					 const char * szMenuLabelSetName);
	virtual ~EV_QNXMenuPopup(void);

	bool				synthesizeMenuPopup();
	virtual bool		refreshMenu(AV_View * pView);
	virtual PtWidget_t *	getMenuHandle(void) const;


protected:
	PtWidget_t *			m_wMenuPopup;
};

#endif /* EV_QNXMENU_H */
