/* AbiSource Program Utilities
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
 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ev_Menu.h"
#include "ev_EditMethod.h"
#include "ev_EditBinding.h"

EV_Menu::EV_Menu(EV_EditMethodContainer * pEMC)
{
	// TODO decide if we really need to remember pEMC
	
	UT_ASSERT(pEMC);

	m_pEMC = pEMC;
}

UT_Bool EV_Menu::invokeMenuMethod(FV_View * pView,
										  EV_EditMethod * pEM,
										  UT_uint32 iPrefixCount,
										  UT_UCSChar * pData,
										  UT_uint32 dataLength)
{
	UT_ASSERT(pView);
	UT_ASSERT(pEM);

#ifdef UT_DEBUG
	UT_DEBUGMSG(("invokeMenuMethod: %s repeat %d length %d with [",
				 pEM->getName(),iPrefixCount,dataLength));
	if (pData && dataLength)
		for (UT_uint32 k=0; k<dataLength; k++)
			UT_DEBUGMSG(("%04x(%c) ",pData[k],pData[k]));
	UT_DEBUGMSG(("]\n"));
#endif	

	EV_EditMethodType t = pEM->getType();

	if (((t & EV_EMT_ALLOWMULTIPLIER) == 0) && (iPrefixCount != 1))
	{
		// they gave a prefix multiplier and this method doesn't permit it.
		// TODO should be beep or complain or just silently fail ?? 
		UT_DEBUGMSG(("    invoke aborted due to multiplier\n"));
		return UT_FALSE;
	}

	if (((t & EV_EMT_REQUIREDATA) != 0) && (!pData || !dataLength))
	{
		// This method requires character data and the caller did not provide any.
		UT_DEBUGMSG(("    invoke aborted due to lack of data\n"));
		return UT_FALSE;
	}

	EV_EditMethodCallData emcd(1,pData,dataLength);
	(*pEM->getFn())(pView,&emcd);

	return UT_TRUE;
	
}


