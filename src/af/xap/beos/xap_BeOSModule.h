/* AbiSource Application Framework
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

#ifndef XAP_BEOSMODULE_H
#define XAP_BEOSMODULE_H

#include "xap_Module.h"
#include <image.h>

class XAP_BeOSModule : public XAP_Module 
{

	friend class XAP_ModuleManager;

protected:
	
	XAP_BeOSModule () ;
	virtual ~XAP_BeOSModule (void);
	
	virtual bool   load (const char * name);
	virtual bool   unload (void);

public:
	virtual bool   resolveSymbol (const char * symbol_name, void ** symbol);
	virtual bool   getModuleName (char ** dest) const;
	virtual bool   getErrorMsg (char ** dest) const;

 private:
	char * m_szname;
	image_id m_module;//void * m_module;
};

#endif /* XAP_BEOSMODULE_H */
