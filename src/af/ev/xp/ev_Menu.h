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

#ifndef EV_MENU_H
#define EV_MENU_H

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
/*****************************************************************/

#include "ut_types.h"

class EV_EditMethodContainer;
class EV_EditMethod;
class FV_View;

class EV_Menu
{
public:
	EV_Menu(EV_EditMethodContainer * pEMC);
	UT_Bool invokeMenuMethod(FV_View * pView,
								 EV_EditMethod * pEM,
								 UT_uint32 iPrefixCount,
								 UT_UCSChar * pData,
								 UT_uint32 dataLength);
	
protected:
	EV_EditMethodContainer *	m_pEMC;
};

#endif /* EV_MENU_H */
