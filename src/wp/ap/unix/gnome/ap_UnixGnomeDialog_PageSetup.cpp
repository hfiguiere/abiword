/* AbiWord
 * Copyright (C) 2001 AbiSource, Inc.
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

#include <gnome.h>
#include "ap_UnixGnomeDialog_PageSetup.h"
#include "ap_Strings.h"
#include "ut_debugmsg.h"
#include "xap_UnixDialogHelper.cpp"

static void
cb_close (GtkWidget * w, AP_UnixGnomeDialog_PageSetup * dlg)
{
  UT_ASSERT(dlg);
  dlg->event_Cancel();
}

AP_UnixGnomeDialog_PageSetup::AP_UnixGnomeDialog_PageSetup(XAP_DialogFactory * pDlgFactory, 
			       XAP_Dialog_Id id) :
  AP_UnixDialog_PageSetup (pDlgFactory, id)
{
}

AP_UnixGnomeDialog_PageSetup::~AP_UnixGnomeDialog_PageSetup(void)
{
}

XAP_Dialog * AP_UnixGnomeDialog_PageSetup::static_constructor(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id)
{
  AP_UnixGnomeDialog_PageSetup * dlg = new AP_UnixGnomeDialog_PageSetup (pDlgFactory, id);
  return dlg;
}

GtkWidget * AP_UnixGnomeDialog_PageSetup::_constructWindow (void)
{
  const XAP_StringSet * pSS = m_pApp->getStringSet();
  m_window = gnome_dialog_new (pSS->getValue(AP_STRING_ID_DLG_PageSetup_Title), NULL);

  _constructWindowContents (GNOME_DIALOG(m_window)->vbox);

  gnome_dialog_append_button (GNOME_DIALOG (m_window), GNOME_STOCK_BUTTON_OK);
  m_buttonOK = GTK_WIDGET (g_list_last (GNOME_DIALOG (m_window)->buttons)->data);

  gnome_dialog_append_button (GNOME_DIALOG (m_window), GNOME_STOCK_BUTTON_CANCEL);
  m_buttonCancel = GTK_WIDGET (g_list_last (GNOME_DIALOG (m_window)->buttons)->data);

  g_signal_connect(G_OBJECT(m_window),
		     "close",
		     G_CALLBACK(cb_close), 
		     (gpointer)this);

  _connectSignals ();
  setDefaultButton (GNOME_DIALOG(m_window), 1);
  return m_window;
}
