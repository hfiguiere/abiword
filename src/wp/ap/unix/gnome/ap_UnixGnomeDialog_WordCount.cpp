/* AbiWord
 * Copyright (C) 2000 AbiSource, Inc.
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

#include <stdlib.h>
#include <gnome.h>

#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

// This header defines some functions for Unix dialogs,
// like centering them, measuring them, etc.
#include "xap_UnixDialogHelper.h"

#include "xap_UnixApp.h"
#include "xap_UnixFrame.h"

#include "ap_Strings.h"
#include "ap_Dialog_Id.h"
#include "ap_UnixGnomeDialog_WordCount.h"
#include "xap_UnixDialogHelper.h"

/*****************************************************************/

#define	WIDGET_ID_TAG_KEY "id"

/*****************************************************************/

static void
cb_close (GtkWidget * w, AP_UnixGnomeDialog_WordCount * dlg)
{
  UT_ASSERT(dlg);
  dlg->event_WindowDelete();
}

XAP_Dialog * AP_UnixGnomeDialog_WordCount::static_constructor(XAP_DialogFactory * pFactory, XAP_Dialog_Id id)
{
	AP_UnixGnomeDialog_WordCount * p = new AP_UnixGnomeDialog_WordCount(pFactory,id);
	return p;
}

AP_UnixGnomeDialog_WordCount::AP_UnixGnomeDialog_WordCount(XAP_DialogFactory * pDlgFactory,
														   XAP_Dialog_Id id)
	: AP_UnixDialog_WordCount(pDlgFactory,id)
{
}

AP_UnixGnomeDialog_WordCount::~AP_UnixGnomeDialog_WordCount(void)
{
}

/*****************************************************************/

GtkWidget * AP_UnixGnomeDialog_WordCount::_constructWindow(void)
{
	const XAP_StringSet * pSS = m_pApp->getStringSet();

	ConstructWindowName();
	m_windowMain = gnome_dialog_new (m_WindowName, NULL);

	// update button
	gnome_dialog_append_button_with_pixmap(GNOME_DIALOG(m_windowMain), 
					       pSS->getValue(XAP_STRING_ID_DLG_Update),
					       GNOME_STOCK_PIXMAP_REFRESH);

	// close button
	gnome_dialog_append_button(GNOME_DIALOG(m_windowMain),
				   GNOME_STOCK_BUTTON_CLOSE);

	_constructWindowContents();
	gtk_container_add (GTK_CONTAINER (GNOME_DIALOG (m_windowMain)->vbox), m_wContent);
   	m_buttonUpdate = GTK_WIDGET (g_list_first (GNOME_DIALOG (m_windowMain)->buttons)->data);
   	m_buttonClose = GTK_WIDGET (g_list_last (GNOME_DIALOG (m_windowMain)->buttons)->data);

	g_signal_connect (G_OBJECT (m_windowMain),
			    "close",
			    G_CALLBACK(cb_close),
			    (gpointer)this);

	_connectSignals ();
	setDefaultButton (GNOME_DIALOG(m_windowMain), 1);

	return m_windowMain;
}
