/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2001 Hubert Figuiere
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

#import <Cocoa/Cocoa.h>

#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "xap_Frame.h"
#include "xap_CocoaFrame.h"
#include "ap_CocoaFrame.h"
#include "ap_CocoaLeftRuler.h"
#include "gr_CocoaGraphics.h"
#include "ut_sleep.h"

#define ENSUREP(p)		do { UT_ASSERT(p); if (!p) goto Cleanup; } while (0)

/*****************************************************************/

#if 0
static void s_getWidgetRelativeMouseCoordinates(AP_CocoaLeftRuler * pCocoaLeftRuler,
												gint * prx, gint * pry)
{
	// TODO there is what appears to be a bug in GTK where
	// TODO mouse coordinates that we receive (motion and
	// TODO release) when we have a grab are relative to
	// TODO whatever window the mouse is over ***AND NOT***
	// TODO relative to our window.  the following ***HACK***
	// TODO is used to map the mouse coordinates relative to
	// TODO our widget.

	// root (absolute) coordinates
	gint rx, ry;
	GdkModifierType mask;
	gdk_window_get_pointer((GdkWindow *) pCocoaLeftRuler->getRootWindow(), &rx, &ry, &mask);

	// local (ruler widget) coordinates
	gint wx, wy;
	pCocoaLeftRuler->getWidgetPosition(&wx, &wy);

	// subtract one from the other to catch all coordinates
	// relative to the widget's 0,
	*prx = rx - wx;
	*pry = ry - wy;
	return;
}

/*****************************************************************/

// evil ugly hack
static int ruler_style_changed (GtkWidget * w, GdkEventClient * event,
								AP_CocoaLeftRuler * ruler)
{
	static GdkAtom atom_rcfiles = GDK_NONE;
	g_return_val_if_fail (w != NULL, FALSE);
	g_return_val_if_fail (event != NULL, FALSE);
	if (!atom_rcfiles)
		atom_rcfiles = gdk_atom_intern ("_GTK_READ_RCFILES", FALSE);
	if (event->message_type != atom_rcfiles)
		return FALSE;
	ruler->_ruler_style_changed();
	return FALSE;
}
#endif


AP_CocoaLeftRuler::AP_CocoaLeftRuler(XAP_Frame * pFrame)
	: AP_LeftRuler(pFrame)
{
	m_rootWindow = nil;
	m_wLeftRuler = nil;
	m_pG = NULL;
    // change ruler color on theme change
	m_wLeftRuler = [(AP_CocoaFrameController *)(static_cast<XAP_CocoaFrame *>(m_pFrame)->_getController()) getVRuler];

#if 0
	NSView * toplevel = (static_cast<XAP_CocoaFrame *> (m_pFrame))->getTopLevelWindow();
	gtk_signal_connect_after (GTK_OBJECT(toplevel),
							  "client_event",
							  GTK_SIGNAL_FUNC(ruler_style_changed),
							  (gpointer)this);
// TODO handle theme. Does Cocoa even know what this is all about ?
#endif
}

AP_CocoaLeftRuler::~AP_CocoaLeftRuler(void)
{
	while(m_pG->isSpawnedRedraw())
	{
		UT_usleep(100);
	}
	DELETEP(m_pG);
}

#if 0
void AP_CocoaLeftRuler::_ruler_style_changed (void)
{
	_refreshView();
}
#endif

NSControl * AP_CocoaLeftRuler::createWidget(void)
{
#if 0
	gtk_signal_connect(GTK_OBJECT(m_wLeftRuler), "button_press_event",
					   GTK_SIGNAL_FUNC(_fe::button_press_event), NULL);

	gtk_signal_connect(GTK_OBJECT(m_wLeftRuler), "button_release_event",
					   GTK_SIGNAL_FUNC(_fe::button_release_event), NULL);

	gtk_signal_connect(GTK_OBJECT(m_wLeftRuler), "motion_notify_event",
					   GTK_SIGNAL_FUNC(_fe::motion_notify_event), NULL);
  
	gtk_signal_connect(GTK_OBJECT(m_wLeftRuler), "configure_event",
					   GTK_SIGNAL_FUNC(_fe::configure_event), NULL);
	if( m_iBackgroundRedrawID == 0)
	{
//
// Start background repaint
//
		m_iBackgroundRedrawID = gtk_timeout_add(200,(GtkFunction) _fe::abi_expose_repaint, (gpointer) this);
	}
	else
    {
		gtk_timeout_remove(m_iBackgroundRedrawID);
		m_iBackgroundRedrawID = gtk_timeout_add(200,(GtkFunction) _fe::abi_expose_repaint, (gpointer) this);
	}
#endif

	return m_wLeftRuler;
}

void AP_CocoaLeftRuler::setView(AV_View * pView)
{
	AP_LeftRuler::setView(pView);

	// We really should allocate m_pG in createWidget(), but
	// unfortunately, the actual window (m_wLeftRuler->window)
	// is not created until the frame's top-level window is
	// shown.
	
	DELETEP(m_pG);

	XAP_CocoaApp * app = static_cast<XAP_CocoaApp *>(m_pFrame->getApp());
	XAP_CocoaFontManager * fontManager = app->getFontManager();
	GR_CocoaGraphics * pG = new GR_CocoaGraphics(m_wLeftRuler, fontManager, m_pFrame->getApp());
	m_pG = pG;
	UT_ASSERT(m_pG);
	static_cast<GR_CocoaGraphics *>(m_pG)->_setUpdateCallback (&_graphicsUpdateCB, (void *)this);

//	GtkWidget * ruler = gtk_vruler_new ();
// TODO	pG->init3dColors(get_ensured_style (ruler));
}

void AP_CocoaLeftRuler::getWidgetPosition(int * x, int * y)
{
	UT_ASSERT(x && y);

	NSRect theBounds = [m_wLeftRuler bounds];
	*x = (int)theBounds.size.width;
	*y = (int)theBounds.size.height;
}

NSWindow * AP_CocoaLeftRuler::getRootWindow(void)
{
	// TODO move this function somewhere more logical, like
	// TODO the XAP_Frame level, since that's where the
	// TODO root window is common to all descendants.
	if (m_rootWindow)
		return m_rootWindow;

	m_rootWindow  = [m_wLeftRuler window];
	return m_rootWindow;
}


bool AP_CocoaLeftRuler::_graphicsUpdateCB(NSRect * aRect, GR_CocoaGraphics *pG, void* param)
{
	// a static function
	AP_CocoaLeftRuler * pCocoaLeftRuler = (AP_CocoaLeftRuler *)param;
	if (!pCocoaLeftRuler)
		return false;

	UT_Rect rClip;
	rClip.left = aRect->origin.x;
	rClip.top = aRect->origin.y;
	rClip.width = aRect->size.width;
	rClip.height = aRect->size.height;
	xxx_UT_DEBUGMSG(("Cocoa in leftruler expose painting area:  left=%d, top=%d, width=%d, height=%d\n", rClip.left, rClip.top, rClip.width, rClip.height));
	if(pG != NULL)
	{
//		pCocoaLeftRuler->getGraphics()->doRepaint(&rClip);
		pCocoaLeftRuler->draw(&rClip);
	}
	else {
		return false;
	}
#if 0
	else
	{
		UT_DEBUGMSG(("No graphics Context. Doing fallback. \n"));
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		pCocoaLeftRuler->draw(&rClip);
	}
#endif
	return true;
}

/*****************************************************************/

#if 0
gint AP_CocoaLeftRuler::_fe::button_press_event(GtkWidget * w, GdkEventButton * e)
{
	// a static function
	AP_CocoaLeftRuler * pCocoaLeftRuler = (AP_CocoaLeftRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	xxx_UT_DEBUGMSG(("CocoaLeftRuler: [p %p] received button_press_event\n",pCocoaLeftRuler));

	// grab the mouse for the duration of the drag.
	gtk_grab_add(w);
	
	EV_EditModifierState ems;
	EV_EditMouseButton emb = 0;
	
	ems = 0;
	
	if (e->state & GDK_SHIFT_MASK)
		ems |= EV_EMS_SHIFT;
	if (e->state & GDK_CONTROL_MASK)
		ems |= EV_EMS_CONTROL;
	if (e->state & GDK_MOD1_MASK)
		ems |= EV_EMS_ALT;

	if (e->state & GDK_BUTTON1_MASK)
		emb = EV_EMB_BUTTON1;
	else if (e->state & GDK_BUTTON2_MASK)
		emb = EV_EMB_BUTTON2;
	else if (e->state & GDK_BUTTON3_MASK)
		emb = EV_EMB_BUTTON3;

	pCocoaLeftRuler->mousePress(ems, emb, (long) e->x, (long) e->y);

	return 1;
}

gint AP_CocoaLeftRuler::_fe::button_release_event(GtkWidget * w, GdkEventButton * e)
{
	// a static function
	AP_CocoaLeftRuler * pCocoaLeftRuler = (AP_CocoaLeftRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	xxx_UT_DEBUGMSG(("CocoaLeftRuler: [p %p] received button_release_event\n",pCocoaLeftRuler));
	EV_EditModifierState ems;
	EV_EditMouseButton emb = 0;
	
	ems = 0;
	
	if (e->state & GDK_SHIFT_MASK)
		ems |= EV_EMS_SHIFT;
	if (e->state & GDK_CONTROL_MASK)
		ems |= EV_EMS_CONTROL;
	if (e->state & GDK_MOD1_MASK)
		ems |= EV_EMS_ALT;

	if (e->state & GDK_BUTTON1_MASK)
		emb = EV_EMB_BUTTON1;
	else if (e->state & GDK_BUTTON2_MASK)
		emb = EV_EMB_BUTTON2;
	else if (e->state & GDK_BUTTON3_MASK)
		emb = EV_EMB_BUTTON3;

	// Map the mouse into coordinates relative to our window.
	gint xrel, yrel;
	s_getWidgetRelativeMouseCoordinates(pCocoaLeftRuler,&xrel,&yrel);

	pCocoaLeftRuler->mouseRelease(ems, emb, xrel, yrel);

	// release the mouse after we are done.
	gtk_grab_remove(w);
	
	return 1;
}
	
gint AP_CocoaLeftRuler::_fe::configure_event(GtkWidget* w, GdkEventConfigure * e)
{
	// a static function
	AP_CocoaLeftRuler * pCocoaLeftRuler = (AP_CocoaLeftRuler *)gtk_object_get_user_data(GTK_OBJECT(w));

	// UT_DEBUGMSG(("CocoaLeftRuler: [p %p] [size w %d h %d] received configure_event\n",
	//			 pCocoaLeftRuler, e->width, e->height));

	UT_uint32 iHeight = (UT_uint32)e->height;
	if (iHeight != pCocoaLeftRuler->getHeight())
		pCocoaLeftRuler->setHeight(iHeight);

	UT_uint32 iWidth = (UT_uint32)e->width;
	if (iWidth != pCocoaLeftRuler->getWidth())
		pCocoaLeftRuler->setWidth(iWidth);
	
	return 1;
}
	
gint AP_CocoaLeftRuler::_fe::motion_notify_event(GtkWidget* w , GdkEventMotion* e)
{
	// a static function
	AP_CocoaLeftRuler * pCocoaLeftRuler = (AP_CocoaLeftRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	// UT_DEBUGMSG(("CocoaLeftRuler: [p %p] received motion_notify_event\n",pCocoaLeftRuler));

	EV_EditModifierState ems;
	
	ems = 0;
	
	if (e->state & GDK_SHIFT_MASK)
		ems |= EV_EMS_SHIFT;
	if (e->state & GDK_CONTROL_MASK)
		ems |= EV_EMS_CONTROL;
	if (e->state & GDK_MOD1_MASK)
		ems |= EV_EMS_ALT;

	// Map the mouse into coordinates relative to our window.
	gint xrel, yrel;
	s_getWidgetRelativeMouseCoordinates(pCocoaLeftRuler,&xrel,&yrel);

	pCocoaLeftRuler->mouseMotion(ems, xrel, yrel);
	return 1;
}
	
gint AP_CocoaLeftRuler::_fe::key_press_event(GtkWidget* w, GdkEventKey* /* e */)
{
	// a static function
	AP_CocoaLeftRuler * pCocoaLeftRuler = (AP_CocoaLeftRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	xxx_UT_DEBUGMSG(("CocoaLeftRuler: [p %p] received key_press_event\n",pCocoaLeftRuler));
	return 1;
}
	
gint AP_CocoaLeftRuler::_fe::delete_event(GtkWidget * /* w */, GdkEvent * /*event*/, gpointer /*data*/)
{
	// a static function
	// AP_CocoaLeftRuler * pCocoaLeftRuler = (AP_CocoaLeftRuler *)gtk_object_get_user_data(GTK_OBJECT(w));
	// UT_DEBUGMSG(("CocoaLeftRuler: [p %p] received delete_event\n",pCocoaLeftRuler));
	return 1;
}
	


/*!
 * Background abi repaint function.
\param XAP_CocoaFrame * p pointer to the Frame that initiated this background
       repainter.
 */
gint AP_CocoaLeftRuler::_fe::abi_expose_repaint( gpointer p)
{
//
// Grab our pointer so we can do useful stuff.
//
	UT_Rect localCopy;
	AP_CocoaLeftRuler * pR = static_cast<AP_CocoaLeftRuler *>(p);
	GR_Graphics * pG = pR->getGraphics();
	if(pG == NULL || pG->isDontRedraw())
	{
//
// Come back later
//
		return TRUE;
	}
	pG->setSpawnedRedraw(true);
	if(pG->isExposePending())
	{
		while(pG->isExposedAreaAccessed())
		{
			UT_usleep(10); // 10 microseconds
		}
		pG->setExposedAreaAccessed(true);
		localCopy.set(pG->getPendingRect()->left,pG->getPendingRect()->top,
					  pG->getPendingRect()->width,pG->getPendingRect()->height);
//
// Clear out this set of expose info
//
		pG->setExposePending(false);
		pG->setExposedAreaAccessed(false);
		xxx_UT_DEBUGMSG(("Painting area on Left ruler:  left=%d, top=%d, width=%d, height=%d\n", localCopy.left, localCopy.top, localCopy.width, localCopy.height));
		xxx_UT_DEBUGMSG(("SEVIOR: Repaint now \n"));
		pR->draw(&localCopy);
	}
//
// OK we've finshed. Wait for the next signal
//
	pG->setSpawnedRedraw(false);
	return TRUE;
}

void AP_CocoaLeftRuler::_fe::destroy(GtkWidget * /*widget*/, gpointer /*data*/)
{
	// a static function
}
#endif
