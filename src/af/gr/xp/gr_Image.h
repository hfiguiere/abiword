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

#ifndef GR_IMAGE_H
#define GR_IMAGE_H

#include "ut_types.h"

#define	GR_IMAGE_MAX_NAME_LEN	63

class UT_ByteBuf;

class GR_Image
{
public:
	GR_Image();
	virtual ~GR_Image();
	
	virtual UT_sint32	getWidth(void) const = 0;
	virtual UT_sint32	getHeight(void) const = 0;
	virtual void		getByteBuf(UT_ByteBuf** ppBB) const = 0;

	void				getName(char* szName) const;
	
protected:
	char				m_szName[GR_IMAGE_MAX_NAME_LEN+1];
};

#endif /* GR_IMAGE */
