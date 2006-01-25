/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */

/* AbiSource Application Framework
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2002 William Lachance 
 * Copyright (C) 2005 INdT 
 * Author: Renato Araujo <renato.filho@indt.org.br>
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
 * 
 */

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#include "ap_Features.h"

#include "ut_types.h"
#include "ut_assert.h"
#include "ut_files.h"
#include "ut_sleep.h"
#include "ev_UnixMenuBar.h"
#include "ev_EditMethod.h"
#include "xap_ViewListener.h"
#include "xap_Frame.h"
#include "xap_Prefs.h"
#include "xap_UnixHildonApp.h"

#include "fv_View.h"

#include "ap_UnixHildonFrameImpl.h"

#include <hildon-widgets/hildon-app.h>
#include <hildon-widgets/hildon-appview.h>

/**
 * A Constructor
 * @param pUnixFrame the pointer of frame
 * @param pUnixApp the pointer of App
 */
AP_UnixHildonFrameImpl::AP_UnixHildonFrameImpl(AP_UnixFrame *pUnixFrame, XAP_UnixApp * pUnixApp) 
:AP_UnixFrameImpl(pUnixFrame, pUnixApp),
	 m_pUnixApp(pUnixApp)
{
	UT_DEBUGMSG(("Created AP_UnixHildonFrameImpl %x \n",this));
}

/**
 * A Destructor
 */
AP_UnixHildonFrameImpl::~AP_UnixHildonFrameImpl() 
{ 	
}

/**
 * Create a new frame instance
 * @return the pointer of new frame
 */
XAP_FrameImpl * AP_UnixHildonFrameImpl::createInstance(XAP_Frame *pFrame, XAP_App *pApp)
{
	XAP_FrameImpl *pFrameImpl = new AP_UnixHildonFrameImpl(static_cast<AP_UnixFrame *>(pFrame), static_cast<XAP_UnixApp *>(pApp));

	return pFrameImpl;
}


/*!
    Because of the peculiar way hildon handles windows and views, we need a custom
    function for the delete_event
*/
static gint s_delete_event(GtkWidget * w, GdkEvent * /*event*/, gpointer /*data*/)
{
	AP_UnixHildonFrameImpl * pFrameImpl = static_cast<AP_UnixHildonFrameImpl *>(g_object_get_data(G_OBJECT(w), "user_data"));
	XAP_Frame* pFrame = pFrameImpl->getFrame();
	XAP_App * pApp = XAP_App::getApp();

	if(pApp->isBonoboRunning())
		return FALSE;

	UT_uint32 iFrameCount = pApp->getFrameCount();
	
	const EV_Menu_ActionSet * pMenuActionSet = pApp->getMenuActionSet();
	UT_ASSERT(pMenuActionSet);

	const EV_EditMethodContainer * pEMC = pApp->getEditMethodContainer();
	UT_ASSERT(pEMC);

	// was "closeWindow", TRUE, FALSE
	const EV_EditMethod * pEM = pEMC->findEditMethodByName("closeWindowX");
	UT_ASSERT(pEM);

	if (pEM)
	{
		if (pEM->Fn(pFrame->getCurrentView(),NULL))
		{
			// returning FALSE means destroy the window, continue along the
			// chain of Gtk destroy events
			
			// with hildon, our frames are not proper windows, but behave more like tabs
			// in a single window. Pressing the 'close' button, however, closes the main
			// window, not the individual tab. So, if there are frames left, we must
			// return true to stop the processing chain. We also have to update the window
			// title 
			
			if(iFrameCount > 1)
			{
				// switch the main window to a different view
				GtkWidget * pHildonAppWidget = (static_cast<XAP_UnixHildonApp*>(XAP_App::getApp()))->getHildonAppWidget();
				UT_return_val_if_fail( pHildonAppWidget, TRUE );

				pFrame = pApp->getFrame(0);
				UT_return_val_if_fail( pFrame, TRUE );

				pFrameImpl = static_cast<AP_UnixHildonFrameImpl*>(pFrame->getFrameImpl());
				UT_return_val_if_fail( pFrameImpl, TRUE );
				
				hildon_app_set_appview(HILDON_APP(pHildonAppWidget), HILDON_APPVIEW(pFrameImpl->getTopLevelWindow()));
				
				return TRUE;
			}
			else
				return FALSE;
		}
	}

	// returning TRUE means do NOT destroy the window; halt the message
	// chain so it doesn't see destroy
	return TRUE;
}

// TODO: split me up into smaller pieces/subfunctions
void AP_UnixHildonFrameImpl::_createTopLevelWindow(void)
{
	// create a top-level window for us.
	bool bResult;
	GtkWidget * pHildonAppWidget = (static_cast<XAP_UnixHildonApp*>(XAP_App::getApp()))->getHildonAppWidget();
	UT_return_if_fail( pHildonAppWidget );
	
	if(m_iFrameMode == XAP_NormalFrame)
	{
		m_wTopLevelWindow = hildon_appview_new(m_pUnixApp->getApplicationTitleForTitleBar());
		
		// This should not be needed -- autoregistration is turned On in getHildonAppWidget();
		// 
		// hildon_app_register_view(HILDON_APP(pHildonAppWidget), (gpointer*)m_wTopLevelWindow);
		
		hildon_app_set_appview(HILDON_APP(pHildonAppWidget), HILDON_APPVIEW(m_wTopLevelWindow));
				
		gtk_widget_show_all(GTK_WIDGET(pHildonAppWidget));		
		
		g_object_set_data(G_OBJECT(m_wTopLevelWindow), "ic_attr", NULL);
		g_object_set_data(G_OBJECT(m_wTopLevelWindow), "ic", NULL);		
	}
	
	g_object_set_data(G_OBJECT(m_wTopLevelWindow), "toplevelWindowFocus",
					  GINT_TO_POINTER(FALSE));
	
	g_object_set_data(G_OBJECT(m_wTopLevelWindow), "user_data", this); 

	g_object_set_data(G_OBJECT(pHildonAppWidget), "user_data", this); 

	g_signal_connect(G_OBJECT(pHildonAppWidget), "realize",
					   G_CALLBACK(_fe::realize), NULL);

	g_signal_connect(G_OBJECT(pHildonAppWidget), "unrealize",
					   G_CALLBACK(_fe::unrealize), NULL);

	g_signal_connect(G_OBJECT(pHildonAppWidget), "size_allocate",
					   G_CALLBACK(_fe::sizeAllocate), NULL);

	g_signal_connect(G_OBJECT(m_wTopLevelWindow), "focus_in_event",
					   G_CALLBACK(_fe::focusIn), NULL);
	g_signal_connect(G_OBJECT(m_wTopLevelWindow), "focus_out_event",
					   G_CALLBACK(_fe::focusOut), NULL);

	g_signal_connect(G_OBJECT(pHildonAppWidget), "delete_event",
					   G_CALLBACK(s_delete_event), NULL);
	g_signal_connect(G_OBJECT(pHildonAppWidget), "destroy",
					   G_CALLBACK(_fe::destroy), NULL);

	g_signal_connect(G_OBJECT(m_wTopLevelWindow), "focus_in_event",
					   G_CALLBACK(_fe::focus_in_event), NULL);
	g_signal_connect(G_OBJECT(m_wTopLevelWindow), "focus_out_event",
					   G_CALLBACK(_fe::focus_out_event), NULL);

	// create a VBox inside it.

	m_wVBox = gtk_vbox_new(FALSE,0);
	g_object_set_data(G_OBJECT(m_wTopLevelWindow), "vbox", m_wVBox);
	g_object_set_data(G_OBJECT(m_wVBox),"user_data", this);
	gtk_container_add(GTK_CONTAINER(m_wTopLevelWindow), m_wVBox);

	if (m_iFrameMode != XAP_NoMenusWindowLess) {
		// synthesize a menu from the info in our base class.
		m_pUnixMenu = new EV_UnixMenuBar(m_pUnixApp, getFrame(), m_szMenuLayoutName,
										 m_szMenuLabelSetName);
		UT_ASSERT(m_pUnixMenu);
		bResult = m_pUnixMenu->synthesizeMenuBar();
		UT_ASSERT(bResult);
	}

	// create a toolbar instance for each toolbar listed in our base class.
	// TODO for some reason, the toolbar functions require the TLW to be
	// TODO realized (they reference m_wTopLevelWindow->window) before we call them.

	if(m_iFrameMode == XAP_NormalFrame)
		gtk_widget_realize(m_wTopLevelWindow);

	_createIMContext(pHildonAppWidget->window);

	g_signal_connect(G_OBJECT(m_wTopLevelWindow), "key_press_event",
					   G_CALLBACK(_fe::key_press_event), NULL);
	g_signal_connect(G_OBJECT(m_wTopLevelWindow), "key_release_event",
					   G_CALLBACK(_fe::key_release_event), NULL);

	if(m_iFrameMode == XAP_NormalFrame)
		_createToolbars();

	// Let the app-specific frame code create the contents of
	// the child area of the window (between the toolbars and
	// the status bar).
	XAP_UnixFrameImpl::m_wSunkenBox = _createDocumentWindow();
	gtk_container_add(GTK_CONTAINER(m_wVBox), m_wSunkenBox);
	gtk_widget_show(m_wSunkenBox);

	// Create statusLet the app-specific frame code create the status bar
	// if it wants to.  we will put it below the document
	// window (a peer with toolbars and the overall sunkenbox)
	// so that it will appear outside of the scrollbars.
	m_wStatusBar = NULL;
	if(m_iFrameMode == XAP_NormalFrame)
		m_wStatusBar = _createStatusBarWindow();

	if (m_wStatusBar) 
	{
		gtk_widget_show(m_wStatusBar);
		gtk_box_pack_end(GTK_BOX(m_wVBox), m_wStatusBar, FALSE, FALSE, 0);
	}

	gtk_widget_show(m_wVBox);
}

bool AP_UnixHildonFrameImpl::_raise()
{
	UT_ASSERT(m_wTopLevelWindow);
	gtk_window_present(GTK_WINDOW (gtk_widget_get_parent(m_wTopLevelWindow)));	
	return true;
}

bool AP_UnixHildonFrameImpl::_updateTitle()
{
	if (!XAP_FrameImpl::_updateTitle() || (m_wTopLevelWindow== NULL) || (m_iFrameMode != XAP_NormalFrame))
	{
		// no relevant change, so skip it
		return false;
	}

	if(getFrame()->getFrameMode() == XAP_NormalFrame)
	{
		const char * szTitle = getFrame()->getTitle(MAX_TITLE_LENGTH);
		hildon_appview_set_title(HILDON_APPVIEW(m_wTopLevelWindow), szTitle);
	}
	return true;
}

void AP_UnixHildonFrameImpl::_setFullScreen(bool changeToFullScreen)
{
	hildon_appview_set_fullscreen(HILDON_APPVIEW(m_wTopLevelWindow), changeToFullScreen);
}

