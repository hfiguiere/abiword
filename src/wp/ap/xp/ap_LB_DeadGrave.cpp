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


// ********************************************************************************
// ********************************************************************************
// *** THIS FILE DEFINES THE BINDINGS TO HANG OFF THE DeadGrave PREFIX KEY IN   ***
// *** THE DEFAULT BINDINGS TABLE.                                              ***
// ********************************************************************************
// ********************************************************************************

#include "ut_assert.h"
#include "ut_types.h"
#include "ev_EditBits.h"
#include "ev_EditBinding.h"
#include "ev_EditMethod.h"
#include "ev_NamedVirtualKey.h"

#define NrElements(a)	((sizeof(a)/sizeof(a[0])))

#define _S		| EV_EMS_SHIFT
#define _C		| EV_EMS_CONTROL
#define _A		| EV_EMS_ALT

/*****************************************************************
******************************************************************
** load bindings for the non-nvk
** (character keys).  note that SHIFT-state is implicit in the
** character value and is not included in the table.  note that
** we do not include the ASCII control characters (\x00 -- \x1f
** and others) since we don't map keystrokes into them.
******************************************************************
*****************************************************************/

struct _iChar
{
	EV_EditBits			m_eb;			// sans ems & shift
	const char *		m_szMethod[EV_COUNT_EMS_NoShift];
};

// TODO finish filling out this table.

static struct _iChar s_CharTable[] =
{
//	{char, /* desc   */ { none,						_C,		_A,		_A_C	}},
	{0x41, /* A      */ { "insertGraveData",		"",		"",		""		}},
	{0x45, /* E      */ { "insertGraveData",		"",		"",		""		}},
	{0x49, /* I      */ { "insertGraveData",		"",		"",		""		}},
	{0x4f, /* O      */ { "insertGraveData",		"",		"",		""		}},
	{0x55, /* U      */ { "insertGraveData",		"",		"",		""		}},
	{0x61, /* a      */ { "insertGraveData",		"",		"",		""		}},
	{0x65, /* e      */ { "insertGraveData",		"",		"",		""		}},
	{0x69, /* i      */ { "insertGraveData",		"",		"",		""		}},
	{0x6f, /* o      */ { "insertGraveData",		"",		"",		""		}},
	{0x75, /* u      */ { "insertGraveData",		"",		"",		""		}},
};

static void s_loadChar(EV_EditMethodContainer * /*pemc*/, EV_EditBindingMap * pebm)
{
	int k, m;
	int kLimit = NrElements(s_CharTable);

	for (k=0; k<kLimit; k++)
		for (m=0; m<EV_COUNT_EMS_NoShift; m++)
			if (s_CharTable[k].m_szMethod[m] && *s_CharTable[k].m_szMethod[m])
			{
				EV_EditModifierState ems = EV_EMS_FromNumberNoShift(m);
				pebm->setBinding(EV_EKP_PRESS|s_CharTable[k].m_eb|ems,s_CharTable[k].m_szMethod[m]);
			}
}

/*****************************************************************
******************************************************************
** put it all together and load the default bindings.
******************************************************************
*****************************************************************/

UT_Bool ap_LoadBindings_DeadGrave(EV_EditMethodContainer * pemc,
								  EV_EditBindingMap **ppebm)
{
	UT_ASSERT(pemc);
	UT_ASSERT(ppebm);

	*ppebm = 0;
	EV_EditBindingMap * pNewEBM = new EV_EditBindingMap(pemc);
	if (!pNewEBM)
		return UT_FALSE;

	s_loadChar(pemc,pNewEBM);

	*ppebm = pNewEBM;
	return UT_TRUE;
}
