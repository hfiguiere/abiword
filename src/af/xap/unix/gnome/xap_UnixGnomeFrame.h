/* AbiSource Application Framework
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


#ifndef XAP_UNIXGNOMEFRAME_H
#define XAP_UNIXGNOMEFRAME_H

extern "C" {
#include <gnome.h>
#ifdef HAVE_BONOBO
#include <libgnorba/gnorba.h>
#include <bonobo/gnome-bonobo.h>
#endif
}

#include "xap_UnixFrame.h"
#include "xap_UnixApp.h"

class XAP_UnixGnomeApp;

/*****************************************************************
******************************************************************
** This file defines the unix-platform-specific class for the
** cross-platform application frame.  This is used to hold all
** unix-specific data.  One of these is created for each top-level
** document window.
******************************************************************
*****************************************************************/

class XAP_UnixGnomeFrame : public XAP_UnixFrame
{
public:
	XAP_UnixGnomeFrame(XAP_UnixGnomeApp * app);
	XAP_UnixGnomeFrame(XAP_UnixGnomeFrame * f);
	XAP_UnixGnomeFrame(XAP_UnixApp * app);
	XAP_UnixGnomeFrame(XAP_UnixFrame * f);
	virtual ~XAP_UnixGnomeFrame(void);

	virtual UT_Bool				openURL(const char * szURL);

protected:
	virtual void				_createTopLevelWindow(void);

	virtual EV_Toolbar *		_newToolbar(XAP_App *app, XAP_Frame *frame, const char *, const char *);

  	enum
  	{
		TARGET_URI_LIST,
		TARGET_URL
  	};

	static void _dnd_drop_event (GtkWidget *widget, GdkDragContext */*context*/, gint /*x*/, gint /*y*/,
								 GtkSelectionData *selection_data, guint info, guint /*time*/);
};

#endif /* XAP_UNIXGNOMEFRAME_H */
