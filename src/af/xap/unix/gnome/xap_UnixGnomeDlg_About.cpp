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

#include <libgnomeui-2.0/gnome.h>

#include "xap_Dialog_Id.h"
#include "xap_UnixGnomeDlg_About.h"
#include "xap_UnixDialogHelper.h"
#include "xap_UnixFrame.h"
#include "xap_App.h"
#include "ut_string_class.h"

XAP_Dialog * XAP_UnixGnomeDialog_About::static_constructor(XAP_DialogFactory * pFactory,
													 XAP_Dialog_Id id)
{
	XAP_UnixGnomeDialog_About * p = new XAP_UnixGnomeDialog_About(pFactory,id);
	return p;
}

XAP_UnixGnomeDialog_About::XAP_UnixGnomeDialog_About(XAP_DialogFactory * pDlgFactory,
											 XAP_Dialog_Id id)
	: XAP_Dialog_About(pDlgFactory,id)
{
}

XAP_UnixGnomeDialog_About::~XAP_UnixGnomeDialog_About(void)
{
}

/*****************************************************************/

static void cb_closed (GtkWidget *w, XAP_UnixGnomeDialog_About *dlg)
{
  UT_ASSERT (w && dlg);
  dlg->event_WindowDelete ();
}

static void cb_delete (GtkWidget *w, gpointer, XAP_UnixGnomeDialog_About *dlg)
{
  UT_ASSERT (w && dlg);
  dlg->event_WindowDelete ();
}

/*****************************************************************/

void XAP_UnixGnomeDialog_About::runModal(XAP_Frame * pFrame)
{
  static const gchar *authors[] = {"Abi the Ant <abi@abisource.com>",
				   "and many other intrepid hackers",
				   NULL};

  UT_String str = XAP_App::getApp()->getAbiSuiteLibDir();
  str += "/icons/abiword_logo.xpm";
  const gchar *gstr = g_stpcpy(gstr, str.c_str());
  const gchar *gcopystr = g_stpcpy(gcopystr, "(c) 1998-2002 AbiSource, Inc.\n"
					     "Released under the GNU GPL v2.0");
  const gchar *gcommentstr = g_stpcpy(gcommentstr, "Gnome Port maintained by Dom Lachowicz <cinamod@hotmail.com> with help\n");
  const gchar *gdoccerstr = g_stpcpy(gdoccerstr, "David Chart <linux@dchart.demon.co.uk> and other contributors");
  const gchar *gtranslatorstr = g_stpcpy(gtranslatorstr, "Many diverse translators");
  // Build the window's widgets and arrange them
  GtkWidget * mainWindow = gnome_about_new("AbiWord", 
										   XAP_App::s_szBuild_Version,
										   *gcopystr,
										   *gcommentstr,
										   **authors,
										   *gdoccerstr,
										   *gtranslatorstr,
										   *gstr);
  UT_ASSERT(mainWindow);
  
  GtkWidget * hbox = gtk_hbox_new (TRUE, 0);
  GtkWidget * href1 = gnome_href_new ("http://www.abisource.com/",
				      "AbiSource.com");

  gtk_box_pack_start (GTK_BOX (hbox), href1, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (GNOME_DIALOG (mainWindow)->vbox),
		      hbox, TRUE, FALSE, 0);
  gtk_widget_show_all (hbox);

  connectFocus(GTK_WIDGET(mainWindow), pFrame);
	
  // To center the dialog, we need the frame of its parent.
  XAP_UnixFrame * pUnixFrame = static_cast<XAP_UnixFrame *>(pFrame);
  UT_ASSERT(pUnixFrame);
  
  // Get the GtkWindow of the parent frame
  GtkWidget * parentWindow = pUnixFrame->getTopLevelWindow();
  UT_ASSERT(parentWindow);
  
  // Center our new dialog in its parent and make it a transient
  // so it won't get lost underneath
  centerDialog(parentWindow, mainWindow);
  
  g_signal_connect (G_OBJECT(mainWindow),
		     "delete_event",
		     G_CALLBACK(cb_delete),
		     (gpointer) this);

  g_signal_connect (G_OBJECT (mainWindow), "close",
		      G_CALLBACK (cb_closed), (gpointer)this);
  
  g_signal_connect_after(G_OBJECT(mainWindow),
			   "destroy",
			   NULL,
			   NULL);
  
  // Show the top level dialog,
  gtk_widget_show(mainWindow);
  
  // Make it modal, and stick it up top
  gtk_grab_add(mainWindow);
  
  // Run into the GTK event loop for this window.
  gtk_main();
  
  gtk_widget_destroy(mainWindow);
}

void XAP_UnixGnomeDialog_About::event_WindowDelete(void)
{
  gtk_main_quit();
}
