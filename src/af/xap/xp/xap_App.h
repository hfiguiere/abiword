/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */ 


#ifndef AP_AP_H
#define AP_AP_H

#include "ut_types.h"

class EV_EditMethodContainer;
class EV_Menu_ActionSet;

/*****************************************************************
******************************************************************
** This file defines the base class for the cross-platform 
** application.  This is used to hold all of the application-specific
** data.  Only one of these is created by the application.
******************************************************************
*****************************************************************/

class AP_Ap
{
public:
	AP_Ap(void);
	virtual ~AP_Ap(void);

	virtual UT_Bool					initialize(int argc, char ** argv);
	EV_EditMethodContainer *		getEditMethodContainer(void) const;
	const EV_Menu_ActionSet *		getMenuActionSet(void) const;

protected:
	EV_EditMethodContainer *	m_pEMC;				/* the set of all possible EditMethods in the app */
	EV_Menu_ActionSet *			m_pMenuActionSet;	/* the set of all possible menu actions in the app */

};

#endif /* AP_AP_H */
