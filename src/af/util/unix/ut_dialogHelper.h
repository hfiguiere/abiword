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

#ifndef UT_DIALOGHELPER_H
#define UT_DIALOGHELPER_H

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

// This is a very thin message box; only use it for startup errors
// or places where you can't use the message box class (like when
// you don't have a frame yet).
void messageBoxOK(const char * message);

// Returns the root window of the X display, useful for doing
// pointer or coordinate measurement on an absolute scale.
GdkWindowPrivate * getRootWindow(GtkWidget * widget);

// Centers a GTK window inside a parent window 
void centerDialog(GtkWidget * parent, GtkWidget * child);

// Returns the index of an entry in a GtkCList by entry contents
gint searchCList(GtkCList * clist, char * compareText);
	
#endif /* UT_DIALOGHELPER_H */
