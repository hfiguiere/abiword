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
#include "xap_Strings.h"
#include "xap_UnixGnomeDlg_Language.h"
#include "xap_UnixDialogHelper.h"

XAP_Dialog * XAP_UnixGnomeDialog_Language::static_constructor(XAP_DialogFactory * pFactory,
													 XAP_Dialog_Id id)
{
	XAP_UnixGnomeDialog_Language * p = new XAP_UnixGnomeDialog_Language(pFactory,id);
	return p;
}

XAP_UnixGnomeDialog_Language::XAP_UnixGnomeDialog_Language(XAP_DialogFactory * pDlgFactory,
											 XAP_Dialog_Id id)
	: XAP_UnixDialog_Language(pDlgFactory,id)
{
}

XAP_UnixGnomeDialog_Language::~XAP_UnixGnomeDialog_Language(void)
{
}

/*****************************************************************/

static void s_delete_clicked(GtkWidget * /* widget */,
							 gpointer /* data */,
							 XAP_Dialog_Language::tAnswer * answer)
{
	*answer = XAP_Dialog_Language::a_CANCEL;
	gtk_main_quit();
}

static void s_ok_clicked(GtkWidget * /* widget */,
						 XAP_Dialog_Language::tAnswer * answer)
{	*answer = XAP_Dialog_Language::a_OK;
	gtk_main_quit();
}

static void s_cancel_clicked(GtkWidget * /* widget */,
							 XAP_Dialog_Language::tAnswer * answer)
{
	*answer = XAP_Dialog_Language::a_CANCEL;
	gtk_main_quit();
}

/*****************************************************************/

GtkWidget * XAP_UnixGnomeDialog_Language::constructWindow(void)
{
	const XAP_StringSet * pSS = m_pApp->getStringSet();
	GtkWidget *windowLangSelection;
	GtkWidget *buttonOK;
	GtkWidget *buttonCancel;

	windowLangSelection = gnome_dialog_new (pSS->getValue(XAP_STRING_ID_DLG_ULANG_LangTitle),
						GNOME_STOCK_BUTTON_OK, GNOME_STOCK_BUTTON_CANCEL, NULL);

	constructWindowContents(G_OBJECT(GNOME_DIALOG (windowLangSelection)->vbox));

	buttonOK = GTK_WIDGET (g_list_first (GNOME_DIALOG (windowLangSelection)->buttons)->data);
	GTK_WIDGET_SET_FLAGS (buttonOK, GTK_CAN_DEFAULT);
	gtk_widget_grab_default (buttonOK);

	buttonCancel = GTK_WIDGET (g_list_last (GNOME_DIALOG (windowLangSelection)->buttons)->data);

	g_signal_connect_after(G_OBJECT(windowLangSelection),
				 "destroy",
				 NULL,
				 NULL);
	g_signal_connect(G_OBJECT(windowLangSelection),
			   "delete_event",
			   G_CALLBACK(s_delete_clicked),
			   (gpointer) &m_answer);

	g_signal_connect(G_OBJECT(buttonOK),
			   "clicked",
			   G_CALLBACK(s_ok_clicked),
			   (gpointer) &m_answer);
	g_signal_connect(G_OBJECT(buttonCancel),
			   "clicked",
			   G_CALLBACK(s_cancel_clicked),
			   (gpointer) &m_answer);
	
	g_signal_connect(G_OBJECT(windowLangSelection),
			   "close",
			   G_CALLBACK(s_cancel_clicked),
			   (gpointer) &m_answer);

	// make cancel button the default
	setDefaultButton (GNOME_DIALOG (windowLangSelection), 1);
	return windowLangSelection;
}
