/* -*- c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */
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

#ifndef EV_MACMENU_H
#define EV_MACMENU_H

// Mac includes
#include <Menus.h>
#include <ToolUtils.h>

#include "ut_types.h"
#include "xap_Types.h"
#include "ev_Menu.h"

class AV_View;
class XAP_MacApp;
class XAP_MacFrame;

#if UNIVERSAL_INTERFACES_VERSION <= 0x0330
	typedef Handle MenuBarHandle;
#endif

/*****************************************************************/

class EV_MacMenu : public EV_Menu
{
public:
	EV_MacMenu(XAP_MacApp * pMacApp, XAP_MacFrame * pMacFrame,
				 const char * szMenuLayoutName,
				 const char * szMenuLabelSetName);
	~EV_MacMenu(void);

	bool onCommand(XAP_Menu_Id id);
	
	bool synthesize(void);


	bool synthesizeMenuBar(void);
    
#if !defined(USE_CARBON_EVENTS)
    XAP_Menu_Id findMenuId (short menu, short item);
#endif
private:
	enum {
		EV_MAC_MENUBAR,
		EV_MAC_MENU,
		EV_MAC_HELPMENU
	};
	XAP_MacApp *m_pMacApp;
	XAP_MacFrame *m_pMacFrame;
#if UNIVERSAL_INTERFACES_VERSION <= 0x0330
	typedef Handle MenuBarHandle;
#endif
    MenuBarHandle	m_hMacMenubar;
    
    short m_lastSubMenuID;

	static OSErr _insertAppleMenu(void);
	static void _convertToMac (char * buf, size_t bufSize, const char * label);
	static char _getItemCmd (const char * mnemonic, UInt8 & modifiers, SInt16 & glyph);
    
 protected:
	virtual bool		_doAddMenuItem(UT_uint32 layout_pos) { UT_ASSERT(UT_NOT_IMPLEMENTED); }
};

#endif /* EV_MACMENU_H */
