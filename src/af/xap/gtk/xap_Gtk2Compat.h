/* AbiWord
 * Copyright (C) 2011-2013 Hubert Figuiere
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

// header for gtk2 compatibility

#ifndef _XAP_GTK2COMPAT_H_
#define _XAP_GTK2COMPAT_H_

#include "ut_types.h"

#include <gtk/gtk.h>

// keynames
#include <gdk/gdkkeysyms-compat.h>


typedef GdkWindow GdkDrawable;


// wrapper because gtk_color_button_set_rgba is deprecated.
inline void XAP_gtk_color_button_set_rgba(GtkColorButton* colorbtn,
				     const GdkRGBA* color)
{
  gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(colorbtn), color);
}

// wrapper because gtk_color_button_get_rgba is deprecated.
inline void XAP_gtk_color_button_get_rgba(GtkColorButton* colorbtn,
					  GdkRGBA* color)
{
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(colorbtn), color);
}


// Now onto the device were the deprecated functions are to be
// with several line of code.
inline
GdkGrabStatus XAP_gdk_keyboard_grab(GdkWindow *window,
				    gboolean owner_events,
				    guint32 time_)
{
	GdkDeviceManager *manager
		= gdk_display_get_device_manager(gdk_display_get_default());
	GdkDevice *device = gdk_device_manager_get_client_pointer (manager);

	return gdk_device_grab (gdk_device_get_associated_device(device),
				window,
				GDK_OWNERSHIP_WINDOW,
				owner_events,
				GDK_ALL_EVENTS_MASK,
				NULL,
				time_);
}

inline
GdkGrabStatus XAP_gdk_pointer_grab(GdkWindow *window,
				   gboolean owner_events,
				   GdkEventMask event_mask,
				   GdkWindow *confine_to,
				   GdkCursor *cursor,
				   guint32 time_)
{
	GdkDeviceManager *manager
		= gdk_display_get_device_manager(gdk_display_get_default());
	GdkDevice *device = gdk_device_manager_get_client_pointer (manager);
	UT_UNUSED(confine_to);
	return gdk_device_grab (device, window,
				GDK_OWNERSHIP_WINDOW,
				owner_events, event_mask,
				cursor, time_);
}


// http://permalink.gmane.org/gmane.comp.gnome.svn/520942
inline
void XAP_gdk_keyboard_ungrab(guint32 t)
{
	GdkDeviceManager *manager
		= gdk_display_get_device_manager(gdk_display_get_default());
	GdkDevice *device = gdk_device_manager_get_client_pointer (manager);
	gdk_device_ungrab (gdk_device_get_associated_device(device),
			   t);
}

inline
void XAP_gdk_pointer_ungrab(guint32 t)
{
	GdkDeviceManager *manager
		= gdk_display_get_device_manager(gdk_display_get_default());
	GdkDevice *device = gdk_device_manager_get_client_pointer (manager);
	gdk_device_ungrab (device, t);
}

#endif
