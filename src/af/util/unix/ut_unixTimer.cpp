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
 

#include <gtk/gtk.h>

#include "ut_unixTimer.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

/*****************************************************************/
	
UT_Timer* UT_Timer::static_constructor(UT_TimerCallback pCallback,
									   void* pData)
{
	UT_ASSERT(pCallback);

	UT_Timer * p = new UT_UNIXTimer();

	if (p)
	{
		p->setCallback(pCallback);
		p->setInstanceData(pData);
	}

	return p;
}

UT_UNIXTimer::~UT_UNIXTimer()
{
	UT_DEBUGMSG(("ut_unixTimer.cpp:  timer destructor\n"));
	
	gtk_timeout_remove(getIdentifier());
}

/*****************************************************************/

static int _Timer_Proc(void *p)
{
	UT_UNIXTimer* pTimer = (UT_UNIXTimer*) p;
	UT_ASSERT(pTimer);

//	UT_DEBUGMSG(("ut_unixTimer.cpp:  timer fire\n"));
	
	pTimer->fire();

	/*
	  We need to manually reset the timer here.  This cross-platform
	  timer was designed to emulate the semantics of Win32 timers,
	  which continually fire until they are killed.
	*/
	pTimer->reset();

	return 0;
}

UT_sint32 UT_UNIXTimer::set(UT_uint32 iMilliseconds)
{
	/*
	  The goal here is to set this timer to go off after iMilliseconds
	  have passed.  This method should not block.  It should call some
	  OS routine which provides timing facilities.  It is assumed that this
	  routine requires a C callback.  That callback, when it is called,
	  must look up the UT_Timer object which corresponds to it, and
	  call its fire() method.  See ut_Win32Timer.cpp for an example
	  of how it's done on Windows.  We're hoping that something similar will work
	  for other platforms.
	*/

	UT_sint32 idTimer = gtk_timeout_add(iMilliseconds, _Timer_Proc, this);
	
	setIdentifier(idTimer);
	
	m_iMilliseconds = iMilliseconds;

	return 0;
}

void UT_UNIXTimer::reset(void)
{
	set(m_iMilliseconds);
}

