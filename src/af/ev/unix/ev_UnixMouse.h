 
/*
** The contents of this file are subject to the AbiSource Public
** License Version 1.0 (the "License"); you may not use this file
** except in compliance with the License. You may obtain a copy
** of the License at http://www.abisource.com/LICENSE/ 
** 
** Software distributed under the License is distributed on an
** "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
** implied. See the License for the specific language governing
** rights and limitations under the License. 
** 
** The Original Code is AbiWord.
** 
** The Initial Developer of the Original Code is AbiSource, Inc.
** Portions created by AbiSource, Inc. are Copyright (C) 1998 AbiSource, Inc. 
** All Rights Reserved. 
** 
** Contributor(s):
**  
*/

#ifndef EV_UNIXMOUSE_H
#define EV_UNIXMOUSE_H

#include <gdk/gdk.h>

#include "ut_types.h"

#include "ev_Mouse.h"

class EV_UnixMouse : public EV_Mouse
{
public:
	EV_UnixMouse(EV_EditEventMapper * pEEM);

	void mouseClick(FV_View* pView, GdkEventButton* e);
	void mouseMotion(FV_View* pView, GdkEventMotion *event);
};

#endif // EV_UNIXMOUSE_H
