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


/*****************************************************************/

class EV_MacMenu : public EV_Menu
{
public:
	EV_MacMenu(XAP_MacApp * pMacApp, XAP_MacFrame * pMacFrame,
				 const char * szMenuLayoutName,
				 const char * szMenuLabelSetName);
	~EV_MacMenu(void);

	bool onCommand(AV_View * pView, 
								WindowPtr hWnd, int wParam);
	
	bool synthesize(void);

	bool onInitMenu(AV_View * pView, WindowPtr hWnd, 
					Handle hMenuBar);

        bool synthesizeMenuBar(void);

private:
	enum {
		EV_MAC_MENUBAR,
		EV_MAC_MENU
	};
	XAP_MacApp *m_pMacApp;
	XAP_MacFrame *m_pMacFrame;
#if UNIVERSAL_INTERFACES_VERSION <= 0x0330
	typedef Handle MenuBarHandle;
#endif
    MenuBarHandle	m_hMacMenubar;
    
    short m_lastMenuID;
    short m_lastSubMenuID;
	
	static void _convertToMac (char * buf, size_t bufSize, const char * label);
};

#endif /* EV_MACMENU_H */
