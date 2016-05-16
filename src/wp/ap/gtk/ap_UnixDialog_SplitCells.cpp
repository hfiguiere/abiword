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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301 USA.
 */

#include <stdlib.h>

#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

// This header defines some functions for Unix dialogs,
// like centering them, measuring them, etc.
#include "xap_UnixDialogHelper.h"

#include "xap_App.h"
#include "xap_UnixApp.h"
#include "xap_Frame.h"

#include "ap_Strings.h"
#include "ap_Dialog_Id.h"
#include "ap_Dialog_SplitCells.h"
#include "ap_UnixDialog_SplitCells.h"
#include "ap_UnixDialog_Columns.h"

static void s_split_left(GtkWidget *widget, gpointer data )
{
	AP_UnixDialog_SplitCells * dlg = static_cast<AP_UnixDialog_SplitCells *>(data);
	UT_return_if_fail(widget && dlg);
	dlg->setSplitType(hori_left);
	dlg->onSplit();
}


static void s_split_hori_mid(GtkWidget *widget, gpointer data )
{
	AP_UnixDialog_SplitCells * dlg = static_cast<AP_UnixDialog_SplitCells *>(data);
	UT_return_if_fail(widget && dlg);
	dlg->setSplitType(hori_mid);
	dlg->onSplit();
}

static void s_split_right(GtkWidget *widget, gpointer data )
{
	AP_UnixDialog_SplitCells * dlg = static_cast<AP_UnixDialog_SplitCells *>(data);
	UT_return_if_fail(widget && dlg);
	dlg->setSplitType(hori_right);
	dlg->onSplit();
}

static void s_response(GtkWidget * wid, gint /*id*/, AP_UnixDialog_SplitCells * /*me*/ )
{
    abiDestroyWidget( wid ) ;// will emit signals for us
}

static void s_split_above(GtkWidget *widget, gpointer data )
{
	AP_UnixDialog_SplitCells * dlg = static_cast<AP_UnixDialog_SplitCells *>(data);
	UT_return_if_fail(widget && dlg);
	dlg->setSplitType(vert_above);
	dlg->onSplit();
}


static void s_split_vert_mid(GtkWidget *widget, gpointer data )
{
	AP_UnixDialog_SplitCells * dlg = static_cast<AP_UnixDialog_SplitCells *>(data);
	UT_return_if_fail(widget && dlg);
	dlg->setSplitType(vert_mid);
	dlg->onSplit();
}

static void s_split_below(GtkWidget *widget, gpointer data )
{
	AP_UnixDialog_SplitCells * dlg = static_cast<AP_UnixDialog_SplitCells *>(data);
	UT_return_if_fail(widget && dlg);
	dlg->setSplitType(vert_below);
	dlg->onSplit();
}

/*****************************************************************/

#define	WIDGET_ID_TAG_KEY "id"

/*****************************************************************/

XAP_Dialog * AP_UnixDialog_SplitCells::static_constructor(XAP_DialogFactory * pFactory,
													       XAP_Dialog_Id id)
{
	return new AP_UnixDialog_SplitCells(pFactory,id);
}

AP_UnixDialog_SplitCells::AP_UnixDialog_SplitCells(XAP_DialogFactory * pDlgFactory,
										             XAP_Dialog_Id id)
	: AP_Dialog_SplitCells(pDlgFactory,id),
	  m_windowMain(NULL),
	  m_wContents(NULL),
	  m_wSplitLeft(NULL),
	  m_wSplitHoriMid(NULL),
	  m_wSplitRight(NULL),
	  m_wSplitAbove(NULL),
	  m_wSplitVertMid(NULL),
	  m_wSplitBelow(NULL),
	  m_lwSplitLeft(NULL),
	  m_lwSplitHoriMid(NULL),
	  m_lwSplitRight(NULL),
	  m_lwSplitAbove(NULL),
	  m_lwSplitVertMid(NULL),
	  m_lwSplitBelow(NULL)
{
}

AP_UnixDialog_SplitCells::~AP_UnixDialog_SplitCells(void)
{
}

void AP_UnixDialog_SplitCells::runModeless(XAP_Frame * pFrame)
{
	// Build the window's widgets and arrange them
	GtkWidget * mainWindow = _constructWindow();
	UT_return_if_fail(mainWindow);

	// Populate the window's data items
	_populateWindowData();
	_connectSignals();
	abiSetupModelessDialog(GTK_DIALOG(mainWindow),pFrame,this,BUTTON_CLOSE);
	startUpdater();
}

void AP_UnixDialog_SplitCells::setSensitivity(AP_CellSplitType splitThis, bool bSens)
{
	switch(splitThis)
	{
	case hori_left:
		gtk_widget_set_sensitive( m_wSplitLeft, bSens);
		gtk_widget_set_sensitive( m_lwSplitLeft, bSens);
		break;
	case hori_mid:
		gtk_widget_set_sensitive( m_wSplitHoriMid, bSens);
		gtk_widget_set_sensitive( m_lwSplitHoriMid, bSens);
		break;
	case hori_right:
		gtk_widget_set_sensitive( m_wSplitRight, bSens);
		gtk_widget_set_sensitive( m_lwSplitRight, bSens);
		break;
	case vert_above:
		gtk_widget_set_sensitive( m_wSplitAbove, bSens);
		gtk_widget_set_sensitive( m_lwSplitAbove, bSens);
		break;
	case vert_mid:
		gtk_widget_set_sensitive( m_wSplitVertMid, bSens);
		gtk_widget_set_sensitive( m_lwSplitVertMid, bSens);
		break;
	case vert_below:
		gtk_widget_set_sensitive( m_wSplitBelow, bSens);
		gtk_widget_set_sensitive( m_lwSplitBelow, bSens);
		break;
	default:
		break;
	}
}

void AP_UnixDialog_SplitCells::event_Close(void)
{
	m_answer = AP_Dialog_SplitCells::a_CANCEL;
	destroy();
}

void AP_UnixDialog_SplitCells::destroy(void)
{
	finalize();
	gtk_widget_destroy(m_windowMain);
	m_windowMain = NULL;
}
void AP_UnixDialog_SplitCells::activate(void)
{
	UT_ASSERT (m_windowMain);
        
	ConstructWindowName();
	gtk_window_set_title (GTK_WINDOW (m_windowMain), m_WindowName);
	setAllSensitivities();
	gdk_window_raise (gtk_widget_get_window(m_windowMain));
}

void AP_UnixDialog_SplitCells::notifyActiveFrame(XAP_Frame * /*pFrame*/)
{
    UT_ASSERT(m_windowMain);
	ConstructWindowName();
	gtk_window_set_title (GTK_WINDOW (m_windowMain), m_WindowName);
	setAllSensitivities();
}

/*****************************************************************/

GtkWidget * AP_UnixDialog_SplitCells::_constructWindow(void)
{
	GtkWidget * vboxMain;
	GtkWidget * windowSplitCells;
	ConstructWindowName();
	const XAP_StringSet *pSS = XAP_App::getApp()->getStringSet();

	windowSplitCells = abiDialogNew ( "split cell dialog", TRUE, static_cast<char *>(m_WindowName));
	gtk_window_set_position(GTK_WINDOW(windowSplitCells), GTK_WIN_POS_MOUSE);
	gtk_window_set_resizable (GTK_WINDOW(windowSplitCells), false);
	vboxMain = gtk_dialog_get_content_area(GTK_DIALOG(windowSplitCells));
	gtk_container_set_border_width (GTK_CONTAINER (vboxMain), 10);
	_constructWindowContents();
	gtk_box_pack_start (GTK_BOX (vboxMain), m_wContents, FALSE, FALSE, 0);
	abiAddButton(GTK_DIALOG(windowSplitCells),
                     pSS->getValue(XAP_STRING_ID_DLG_Close), BUTTON_CLOSE);

	// Update member variables with the important widgets that
	// might need to be queried or altered later.

	m_windowMain = windowSplitCells;

	return windowSplitCells;
}

GtkWidget * AP_UnixDialog_SplitCells::_constructWindowContents(void)
{
	GtkWidget *wContents;

	wContents = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_show (wContents);
	GtkWidget *frame1;
	GtkWidget *grid1;
	GtkWidget *wlSplitLeft;
	GtkWidget *wlSplitHoriMid;
	GtkWidget *wlSplitRight;
	GtkWidget *wlSplitAbove;
	GtkWidget *wlSplitVertMid;
	GtkWidget *wlSplitBelow;
	GtkWidget *wSplitLeft;
	GtkWidget *wSplitHoriMid;
	GtkWidget *wSplitRight;
	GtkWidget *wSplitAbove;
	GtkWidget *wSplitVertMid;
	GtkWidget *wSplitBelow;
	const XAP_StringSet * pSS = m_pApp->getStringSet();
	std::string s;
	pSS->getValueUTF8(AP_STRING_ID_DLG_SplitCells_Frame,s);
	frame1 = gtk_frame_new (NULL);
	gtk_widget_show (frame1);
	gtk_container_add (GTK_CONTAINER (wContents), frame1);
	gtk_container_set_border_width (GTK_CONTAINER (frame1), 3);
	gtk_frame_set_shadow_type(GTK_FRAME(frame1), GTK_SHADOW_NONE);

	grid1 = gtk_grid_new ();
	gtk_widget_show (grid1);
	gtk_container_add (GTK_CONTAINER (frame1), grid1);
	g_object_set (G_OBJECT (grid1),
	              "row-spacing", 6,
	              "column-spacing", 12,
	              NULL);

	pSS->getValueUTF8(AP_STRING_ID_DLG_SplitCells_Left,s);
	wlSplitLeft = gtk_widget_new (GTK_TYPE_LABEL, "label", s.c_str(),
                                      "xalign", 0.0, "yalign", 0.5, NULL);
	gtk_widget_show (wlSplitLeft);
	gtk_grid_attach (GTK_GRID (grid1), wlSplitLeft, 0, 0, 1, 1);

	pSS->getValueUTF8(AP_STRING_ID_DLG_SplitCells_HoriMid,s);
	wlSplitHoriMid = gtk_widget_new (GTK_TYPE_LABEL, "label", s.c_str(),
                                         "xalign", 0.0, "yalign", 0.5, NULL);
	gtk_widget_show (wlSplitHoriMid);
	gtk_grid_attach (GTK_GRID (grid1), wlSplitHoriMid, 0, 1, 1, 1);

	pSS->getValueUTF8(AP_STRING_ID_DLG_SplitCells_Right,s);
	wlSplitRight = gtk_widget_new (GTK_TYPE_LABEL, "label", s.c_str(),
                                      "xalign", 0.0, "yalign", 0.5, NULL);
	gtk_widget_show (wlSplitRight);
	gtk_grid_attach (GTK_GRID (grid1), wlSplitRight, 0, 2, 1, 1);

	pSS->getValueUTF8(AP_STRING_ID_DLG_SplitCells_Above,s);
	wlSplitAbove = gtk_widget_new (GTK_TYPE_LABEL, "label", s.c_str(),
                                       "xalign", 0.0, "yalign", 0.5, NULL);
	gtk_widget_show (wlSplitAbove);
	gtk_grid_attach (GTK_GRID (grid1), wlSplitAbove, 0, 3, 1, 1);

	pSS->getValueUTF8(AP_STRING_ID_DLG_SplitCells_VertMid,s);
	wlSplitVertMid = gtk_widget_new (GTK_TYPE_LABEL, "label", s.c_str(),
                                        "xalign", 0.0, "yalign", 0.5, NULL);
	gtk_widget_show (wlSplitVertMid);
	gtk_grid_attach (GTK_GRID (grid1), wlSplitVertMid, 0, 4, 1, 1);

	pSS->getValueUTF8(AP_STRING_ID_DLG_SplitCells_Below,s);
	wlSplitBelow = gtk_widget_new (GTK_TYPE_LABEL, "label", s.c_str(),
                                       "xalign", 0.0, "yalign", 0.5, NULL);
	gtk_widget_show (wlSplitBelow);
	gtk_grid_attach (GTK_GRID (grid1), wlSplitBelow, 0, 5, 1, 1);

	wSplitLeft = gtk_button_new();
	gtk_widget_show (wSplitLeft);
	label_button_with_abi_pixmap(wSplitLeft, "tb_SplitLeft_xpm");

	gtk_grid_attach (GTK_GRID (grid1), wSplitLeft, 1, 0, 1, 1);

	wSplitHoriMid = gtk_button_new();
	gtk_widget_show (wSplitHoriMid);
	label_button_with_abi_pixmap(wSplitHoriMid, "tb_SplitHoriMid_xpm");

	gtk_grid_attach (GTK_GRID (grid1), wSplitHoriMid, 1, 1, 1, 1);


	wSplitRight = gtk_button_new();
	gtk_widget_show (wSplitRight);
	label_button_with_abi_pixmap(wSplitRight, "tb_SplitRight_xpm");
	gtk_grid_attach (GTK_GRID (grid1), wSplitRight, 1, 2, 1, 1);

	wSplitAbove = gtk_button_new();
	gtk_widget_show (wSplitAbove);
	label_button_with_abi_pixmap(wSplitAbove, "tb_SplitAbove_xpm");

	gtk_grid_attach (GTK_GRID (grid1), wSplitAbove, 1, 3, 1, 1);


	wSplitVertMid = gtk_button_new();
	gtk_widget_show (wSplitVertMid);
	label_button_with_abi_pixmap(wSplitVertMid, "tb_SplitVertMid_xpm");

	gtk_grid_attach (GTK_GRID (grid1), wSplitVertMid, 1, 4, 1, 1);


	wSplitBelow = gtk_button_new();
	gtk_widget_show (wSplitBelow);
	label_button_with_abi_pixmap(wSplitBelow, "tb_SplitBelow_xpm");

	gtk_grid_attach (GTK_GRID (grid1), wSplitBelow, 1, 5, 1, 1);

	m_wSplitLeft = wSplitLeft;
	m_wSplitHoriMid = wSplitHoriMid;
	m_wSplitRight = wSplitRight;
	m_wSplitAbove = wSplitAbove;
	m_wSplitVertMid = wSplitVertMid;
	m_wSplitBelow = wSplitBelow;
	m_lwSplitLeft = wlSplitLeft;
	m_lwSplitRight = wlSplitRight;
	m_lwSplitAbove = wlSplitAbove;
	m_lwSplitVertMid = wlSplitVertMid;
	m_lwSplitHoriMid = wlSplitHoriMid;
	m_lwSplitBelow = wlSplitBelow;

	m_wContents = wContents;
	
	return m_wContents;
}

static void s_destroy_clicked(GtkWidget * /* widget */,
			      AP_UnixDialog_SplitCells * dlg)
{
	UT_ASSERT(dlg);
	dlg->event_Close();
}


static void s_delete_clicked(GtkWidget * widget,
			     gpointer,
			     gpointer * /*dlg*/)
{
	abiDestroyWidget(widget);
}

void AP_UnixDialog_SplitCells::_connectSignals(void)
{
  g_signal_connect(G_OBJECT(m_windowMain), "response", 
		   G_CALLBACK(s_response), this);

	// the catch-alls
	// Dont use gtk_signal_connect_after for modeless dialogs
	g_signal_connect(G_OBJECT(m_windowMain),
			   "destroy",
			   G_CALLBACK(s_destroy_clicked),
			   static_cast<gpointer>(this));
	g_signal_connect(G_OBJECT(m_windowMain),
			   "delete_event",
			   G_CALLBACK(s_delete_clicked),
			   static_cast<gpointer>(this));

	g_signal_connect(G_OBJECT(m_wSplitLeft),
						   "clicked",
						   G_CALLBACK(s_split_left),
						   static_cast<gpointer>(this));
	g_signal_connect(G_OBJECT(m_wSplitHoriMid),
						   "clicked",
						   G_CALLBACK(s_split_hori_mid),
						   static_cast<gpointer>(this));
	g_signal_connect(G_OBJECT(m_wSplitRight),
						   "clicked",
						   G_CALLBACK(s_split_right),
						   static_cast<gpointer>(this));
	g_signal_connect(G_OBJECT(m_wSplitAbove),
						   "clicked",
						   G_CALLBACK(s_split_above),
						   static_cast<gpointer>(this));
	g_signal_connect(G_OBJECT(m_wSplitVertMid),
						   "clicked",
						   G_CALLBACK(s_split_vert_mid),
						   static_cast<gpointer>(this));
	g_signal_connect(G_OBJECT(m_wSplitBelow),
						   "clicked",
						   G_CALLBACK(s_split_below),
						   static_cast<gpointer>(this));

}

void AP_UnixDialog_SplitCells::_populateWindowData(void)
{
   setAllSensitivities();
}

void AP_UnixDialog_SplitCells::_storeWindowData(void)
{
}
