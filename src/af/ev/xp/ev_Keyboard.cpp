/* AbiSource Program Utilities
 * Copyright (C) 1998-2000 AbiSource, Inc.
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ev_Keyboard.h"
#include "ev_EditMethod.h"
#include "ev_EditBinding.h"
#include "ev_EditEventMapper.h"

EV_Keyboard::EV_Keyboard(EV_EditEventMapper * pEEM)
{
	setEditEventMap(pEEM);
}

EV_Keyboard::~EV_Keyboard(void)
{
}

void EV_Keyboard::setEditEventMap(EV_EditEventMapper * pEEM)
{
	UT_ASSERT(pEEM);
	m_pEEM = pEEM;
}

bool EV_Keyboard::invokeKeyboardMethod(AV_View * pView,
										  EV_EditMethod * pEM,
										  UT_UCSChar * pData,
										  UT_uint32 dataLength)
{
	UT_ASSERT(pView);
	UT_ASSERT(pEM);

#if 0
	UT_DEBUGMSG(("invokeKeyboardMethod: %s length %d with [",
				 pEM->getName(),dataLength));
	if (pData && dataLength)
		for (UT_uint32 k=0; k<dataLength; k++)
			UT_DEBUGMSG(("%04x(%c) ",pData[k],pData[k]));
	UT_DEBUGMSG(("]\n"));
#endif	

	EV_EditMethodType t = pEM->getType();

	if (((t & EV_EMT_REQUIREDATA) != 0) && (!pData || !dataLength))
	{
		// This method requires character data and the caller did not provide any.
		UT_DEBUGMSG(("    invoke aborted due to lack of data\n"));
		return false;
	}

	EV_EditMethodCallData emcd(pData,dataLength);
	pEM->Fn(pView,&emcd);

	return true;
}


