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

// TODO change the name of all dg_ and DG_ classes to gr_ and GR_

#ifndef GR_DRAWARGS_H
#define GR_DRAWARGS_H

#include "ut_types.h"

class DG_Graphics;

struct dg_DrawArgs
{
	dg_DrawArgs();
	
	DG_Graphics*	pG;
	UT_sint32		xoff;
	UT_sint32		yoff;
	UT_sint32		x;
	UT_sint32		y;
	UT_sint32		width;
	UT_sint32		height;

	// actually the following are PT_DocPosition
	UT_uint32		iSelPos1;
	UT_uint32		iSelPos2;
};

#endif /* GR_DRAWARGS_H */
