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

#include <gtk/gtk.h>
#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ap_UnixTopRuler.h"
#include "gr_UnixGraphics.h"

#define DELETEP(p)		do { if (p) delete p; p = NULL; } while (0)
#define REPLACEP(p,q)	do { if (p) delete p; p = q; } while (0)
#define ENSUREP(p)		do { UT_ASSERT(p); if (!p) goto Cleanup; } while (0)

/*****************************************************************/

AP_UnixTopRuler::AP_UnixTopRuler(XAP_Frame * pFrame)
	: AP_TopRuler(pFrame)
{
	m_wTopRuler = NULL;
	m_pG = NULL;
}

AP_UnixTopRuler::~AP_UnixTopRuler(void)
{
	DELETEP(m_pG);
}

GtkWidget * AP_UnixTopRuler::createWidget(UT_uint32 iHeight)
{
	UT_ASSERT(!m_pG && !m_wTopRuler);
	
	setHeight(iHeight);

	m_wTopRuler = gtk_drawing_area_new();

	gtk_object_set_user_data(GTK_OBJECT(m_wTopRuler),this);
	gtk_widget_show(m_wTopRuler);
	gtk_widget_set_usize(m_wTopRuler, -1, iHeight);

	gtk_widget_set_events(GTK_WIDGET(m_wTopRuler), (GDK_EXPOSURE_MASK |
													GDK_BUTTON_PRESS_MASK |
													GDK_POINTER_MOTION_MASK |
													GDK_BUTTON_RELEASE_MASK |
													GDK_KEY_PRESS_MASK |
													GDK_KEY_RELEASE_MASK));

	gtk_signal_connect(GTK_OBJECT(m_wTopRuler), "expose_event",
					   GTK_SIGNAL_FUNC(_fe::expose), NULL);
  
	gtk_signal_connect(GTK_OBJECT(m_wTopRuler), "button_press_event",
					   GTK_SIGNAL_FUNC(_fe::button_press_event), NULL);

	gtk_signal_connect(GTK_OBJECT(m_wTopRuler), "button_release_event",
					   GTK_SIGNAL_FUNC(_fe::button_release_event), NULL);

	gtk_signal_connect(GTK_OBJECT(m_wTopRuler), "motion_notify_event",
					   GTK_SIGNAL_FUNC(_fe::motion_notify_event), NULL);
  
	gtk_signal_connect(GTK_OBJECT(m_wTopRuler), "configure_event",
					   GTK_SIGNAL_FUNC(_fe::configure_event), NULL);

	return m_wTopRuler;
}

void AP_UnixTopRuler::setView(AV_View * pView)
{
	AP_TopRuler::setView(pView);

	// We really should allocate m_pG in createWidget(), but
	// unfortunately, the actual window (m_wTopRuler->window)
	// is not created until the frame's top-level window is
	// shown.
	
	DELETEP(m_pG);
	m_pG = new UNIXGraphics(m_wTopRuler->window);
	UT_ASSERT(m_pG);
}

/*****************************************************************/

void AP_UnixTopRuler::scrollRuler(UT_sint32 xoff)
{
	// platform-dependent scrolling for the ruler
	UT_DEBUGMSG(("UnixTopRuler: received scrollRuler [x %d]\n",xoff));
}

/*****************************************************************/

gint AP_UnixTopRuler::_fe::button_press_event(GtkWidget * w, GdkEventButton * e)
{
	// a static function
	AP_UnixTopRuler * pUnixTopRuler = (AP_UnixTopRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	UT_DEBUGMSG(("UnixTopRuler: [p %p] received button_press_event\n",pUnixTopRuler));
	return 1;
}

gint AP_UnixTopRuler::_fe::button_release_event(GtkWidget * w, GdkEventButton * e)
{
	// a static function
	AP_UnixTopRuler * pUnixTopRuler = (AP_UnixTopRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	UT_DEBUGMSG(("UnixTopRuler: [p %p] received button_release_event\n",pUnixTopRuler));
	return 1;
}
	
gint AP_UnixTopRuler::_fe::configure_event(GtkWidget* w, GdkEventConfigure *e)
{
	// a static function
	AP_UnixTopRuler * pUnixTopRuler = (AP_UnixTopRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	UT_DEBUGMSG(("UnixTopRuler: [p %p] [size w %d h %d] received configure_event\n",
				 pUnixTopRuler, e->width, e->height));

	// TODO for some reason, I cannot get GTK to respect the height request
	// TODO that we made when we created the widget. for now, we just deal
	// TODO with what they gave us....

	UT_uint32 iHeight = (UT_uint32)e->height;
	if (iHeight != pUnixTopRuler->getHeight())
	{
		pUnixTopRuler->setHeight(iHeight);
		// TODO do we need to invalidate and redraw
	}

	// TODO add code in the LeftRuler's configure_event code to send us
	// TODO a setOffsetLeftRuler() message.
	
	return 1;
}
	
gint AP_UnixTopRuler::_fe::motion_notify_event(GtkWidget* w, GdkEventMotion* e)
{
	// a static function
	AP_UnixTopRuler * pUnixTopRuler = (AP_UnixTopRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	UT_DEBUGMSG(("UnixTopRuler: [p %p] received motion_notify_event\n",pUnixTopRuler));
	return 1;
}
	
gint AP_UnixTopRuler::_fe::key_press_event(GtkWidget* w, GdkEventKey* e)
{
	// a static function
	AP_UnixTopRuler * pUnixTopRuler = (AP_UnixTopRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	UT_DEBUGMSG(("UnixTopRuler: [p %p] received key_press_event\n",pUnixTopRuler));
	return 1;
}
	
gint AP_UnixTopRuler::_fe::delete_event(GtkWidget * w, GdkEvent * /*event*/, gpointer /*data*/)
{
	// a static function
	AP_UnixTopRuler * pUnixTopRuler = (AP_UnixTopRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	UT_DEBUGMSG(("UnixTopRuler: [p %p] received delete_event\n",pUnixTopRuler));
	return 1;
}
	
gint AP_UnixTopRuler::_fe::expose(GtkWidget * w, GdkEventExpose* pExposeEvent)
{
	// a static function
	AP_UnixTopRuler * pUnixTopRuler = (AP_UnixTopRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	UT_DEBUGMSG(("UnixTopRuler: [p %p] received expose\n",pUnixTopRuler));
	pUnixTopRuler->draw(NULL);
	return 0;
}

void AP_UnixTopRuler::_fe::destroy(GtkWidget * /*widget*/, gpointer /*data*/)
{
	// a static function
}
