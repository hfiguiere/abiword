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

#include <stdlib.h>
#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

// This header defines some functions for Unix dialogs,
// like centering them, measuring them, etc.
#include "xap_UnixDialogHelper.h"

#include "gr_UnixGraphics.h"

#include "xap_App.h"
#include "xap_UnixGnomeApp.h"
#include "xap_UnixFrame.h"

#include "xap_Dialog_Id.h"
#include "xap_Dlg_Zoom.h"
#include "xap_UnixDlg_Zoom.h"
#include "xap_UnixGnomeDlg_Zoom.h"

/*****************************************************************/

#define	WIDGET_ID_TAG_KEY "id"

/*****************************************************************/

XAP_Dialog * XAP_UnixGnomeDialog_Zoom::static_constructor(XAP_DialogFactory * pFactory,
													   XAP_Dialog_Id id)
{
	XAP_UnixGnomeDialog_Zoom * p = new XAP_UnixGnomeDialog_Zoom(pFactory,id);
	return p;
}

XAP_UnixGnomeDialog_Zoom::XAP_UnixGnomeDialog_Zoom(XAP_DialogFactory * pDlgFactory,
											   XAP_Dialog_Id id)
	: XAP_UnixDialog_Zoom(pDlgFactory,id)
{
}

XAP_UnixGnomeDialog_Zoom::~XAP_UnixGnomeDialog_Zoom(void)
{
}

/*****************************************************************/

static void s_ok_clicked(GtkWidget * widget, XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(widget && dlg);
	dlg->event_OK();
}
static void s_cancel_clicked(GtkWidget * widget, XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(widget && dlg);
	dlg->event_Cancel();
}
static void s_delete_clicked(GtkWidget * /* widget */,
							 gpointer /* data */,
							 XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(dlg);
	dlg->event_WindowDelete();
}
static void s_radio_200_clicked(GtkWidget * widget, XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(widget && dlg);
	dlg->event_Radio200Clicked();
}
static void s_radio_100_clicked(GtkWidget * widget, XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(widget && dlg);
	dlg->event_Radio100Clicked();
}
static void s_radio_75_clicked(GtkWidget * widget, XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(widget && dlg);
	dlg->event_Radio75Clicked();
}
static void s_radio_PageWidth_clicked(GtkWidget * widget, XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(widget && dlg);
	dlg->event_RadioPageWidthClicked();
}
static void s_radio_WholePage_clicked(GtkWidget * widget, XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(widget && dlg);
	dlg->event_RadioWholePageClicked();
}
static void s_radio_Percent_clicked(GtkWidget * widget, XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(widget && dlg);
	dlg->event_RadioPercentClicked();
}
static void s_spin_Percent_changed(GtkWidget * widget, XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(widget && dlg);
	dlg->event_SpinPercentChanged();
}

static gint s_preview_exposed(GtkWidget * /* widget */,
							  GdkEventExpose * /* pExposeEvent */,
							  XAP_UnixDialog_Zoom * dlg)
{
	UT_ASSERT(dlg);
	dlg->event_PreviewAreaExposed();

	return FALSE;
}

/*****************************************************************/

void XAP_UnixGnomeDialog_Zoom::runModal(XAP_Frame * pFrame)
{
	m_pFrame = pFrame;

	// Build the window's widgets and arrange them
	GtkWidget * mainWindow = _constructWindow();
	UT_ASSERT(mainWindow);

	// Populate the window's data items
	_populateWindowData();
	
	// To center the dialog, we need the frame of its parent.
	XAP_UnixFrame * pUnixFrame = static_cast<XAP_UnixFrame *>(pFrame);
	UT_ASSERT(pUnixFrame);
	
	// Get the GtkWindow of the parent frame
	GtkWidget * parentWindow = pUnixFrame->getTopLevelWindow();
	UT_ASSERT(parentWindow);
	
	// Center our new dialog in its parent and make it a transient
	// so it won't get lost underneath
	centerDialog(parentWindow, mainWindow);

	// Show the top level dialog,
	gtk_widget_show(mainWindow);

	// Make it modal, and stick it up top
	gtk_grab_add(mainWindow);

	// *** this is how we add the gc ***
	{
		// attach a new graphics context to the drawing area
		XAP_UnixApp * unixapp = static_cast<XAP_UnixApp *> (m_pApp);
		UT_ASSERT(unixapp);

		UT_ASSERT(m_previewArea && m_previewArea->window);

		// make a new Unix GC
		m_unixGraphics = new GR_UnixGraphics(m_previewArea->window, unixapp->getFontManager(), m_pApp);
		
		// let the widget materialize
		_createPreviewFromGC(m_unixGraphics,
							 (UT_uint32) m_previewArea->allocation.width,
							 (UT_uint32) m_previewArea->allocation.height);
	}

	// HACK : we call this TWICE so it generates an update on the buttons to
	// HACK : trigger a preview
	_populateWindowData();

	// Run into the GTK event loop for this window.
	gtk_main();

	_storeWindowData();
	
	gtk_widget_destroy(mainWindow);
}

/*****************************************************************/

GtkWidget * XAP_UnixGnomeDialog_Zoom::_constructWindow(void)
{
	GtkWidget * windowZoom;

	GtkWidget * vboxZoom;
	GtkWidget * hboxFrames;
	GtkWidget * frameZoomTo;
	GtkWidget * vboxZoomTo;
	GSList * 	vboxZoomTo_group = NULL;

	GtkWidget * radiobutton200;
	GtkWidget * radiobutton100;
	GtkWidget * radiobutton75;
	GtkWidget * radiobuttonPageWidth;
	GtkWidget * radiobuttonWholePage;
	GtkWidget * radiobuttonPercent;
	GObject * spinbuttonPercent_adj;
	GtkWidget * spinbuttonPercent;

	GtkWidget * framePreview;
	GtkWidget * drawingareaPreview;

	GtkWidget * buttonOK;
	GtkWidget * buttonCancel;

	const XAP_StringSet * pSS = m_pApp->getStringSet();

	XML_Char * tmp = NULL;
	
	windowZoom = gnome_dialog_new (pSS->getValue(XAP_STRING_ID_DLG_Zoom_ZoomTitle),
								   GNOME_STOCK_BUTTON_OK, GNOME_STOCK_BUTTON_CANCEL, NULL);
	g_object_set_data (G_OBJECT (windowZoom), "windowZoom", windowZoom);
	UT_XML_cloneNoAmpersands(tmp, pSS->getValue(XAP_STRING_ID_DLG_Zoom_ZoomTitle));
	FREEP(tmp);
	gtk_window_set_policy (GTK_WINDOW (windowZoom), FALSE, FALSE, FALSE);

	vboxZoom = GNOME_DIALOG (windowZoom)->vbox;
	g_object_set_data (G_OBJECT (windowZoom), "vboxZoom", vboxZoom);

	hboxFrames = gtk_hbox_new (FALSE, 10);
	g_object_set_data (G_OBJECT (windowZoom), "hboxFrames", hboxFrames);
	gtk_widget_show (hboxFrames);
	gtk_box_pack_start (GTK_BOX (vboxZoom), hboxFrames, FALSE, TRUE, 0);

	UT_XML_cloneNoAmpersands(tmp, pSS->getValue(XAP_STRING_ID_DLG_Zoom_RadioFrameCaption));
	frameZoomTo = gtk_frame_new (tmp);
	FREEP(tmp);
	g_object_set_data (G_OBJECT (windowZoom), "frameZoomTo", frameZoomTo);
	gtk_widget_show (frameZoomTo);
	gtk_box_pack_start (GTK_BOX (hboxFrames), frameZoomTo, FALSE, TRUE, 0);

	vboxZoomTo = gtk_vbox_new (FALSE, 0);
	g_object_set_data (G_OBJECT (windowZoom), "vboxZoomTo", vboxZoomTo);
	gtk_widget_show (vboxZoomTo);
	gtk_container_add (GTK_CONTAINER (frameZoomTo), vboxZoomTo);
	gtk_container_border_width (GTK_CONTAINER (vboxZoomTo), 10);

	UT_XML_cloneNoAmpersands(tmp, pSS->getValue(XAP_STRING_ID_DLG_Zoom_200));
	radiobutton200 = gtk_radio_button_new_with_label (vboxZoomTo_group, tmp);
	FREEP(tmp);
	vboxZoomTo_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton200));
	g_object_set_data (G_OBJECT (windowZoom), "radiobutton200", radiobutton200);
	g_object_set_data (G_OBJECT (radiobutton200), WIDGET_ID_TAG_KEY, GINT_TO_POINTER(XAP_Frame::z_200));
	gtk_widget_show (radiobutton200);
	gtk_box_pack_start (GTK_BOX (vboxZoomTo), radiobutton200, FALSE, TRUE, 0);

	UT_XML_cloneNoAmpersands(tmp, pSS->getValue(XAP_STRING_ID_DLG_Zoom_100));
	radiobutton100 = gtk_radio_button_new_with_label (vboxZoomTo_group, tmp);
	FREEP(tmp);
	vboxZoomTo_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton100));
	g_object_set_data (G_OBJECT (windowZoom), "radiobutton100", radiobutton100);
	g_object_set_data (G_OBJECT (radiobutton100), WIDGET_ID_TAG_KEY, GINT_TO_POINTER(XAP_Frame::z_100));
	gtk_widget_show (radiobutton100);
	gtk_box_pack_start (GTK_BOX (vboxZoomTo), radiobutton100, FALSE, TRUE, 0);

	UT_XML_cloneNoAmpersands(tmp, pSS->getValue(XAP_STRING_ID_DLG_Zoom_75));
	radiobutton75 = gtk_radio_button_new_with_label (vboxZoomTo_group, tmp);
	FREEP(tmp);
	vboxZoomTo_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton75));
	g_object_set_data (G_OBJECT (windowZoom), "radiobutton75", radiobutton75);
	g_object_set_data (G_OBJECT (radiobutton75), WIDGET_ID_TAG_KEY, GINT_TO_POINTER(XAP_Frame::z_75));
	gtk_widget_show (radiobutton75);
	gtk_box_pack_start (GTK_BOX (vboxZoomTo), radiobutton75, TRUE, TRUE, 0);

	UT_XML_cloneNoAmpersands(tmp, pSS->getValue(XAP_STRING_ID_DLG_Zoom_PageWidth));
	radiobuttonPageWidth = gtk_radio_button_new_with_label (vboxZoomTo_group, tmp);
	FREEP(tmp);
	vboxZoomTo_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobuttonPageWidth));
	g_object_set_data (G_OBJECT (windowZoom), "radiobuttonPageWidth", radiobuttonPageWidth);
	g_object_set_data (G_OBJECT (radiobuttonPageWidth), WIDGET_ID_TAG_KEY, GINT_TO_POINTER(XAP_Frame::z_PAGEWIDTH));
	gtk_widget_show (radiobuttonPageWidth);
	gtk_box_pack_start (GTK_BOX (vboxZoomTo), radiobuttonPageWidth, TRUE, TRUE, 0);

	UT_XML_cloneNoAmpersands(tmp, pSS->getValue(XAP_STRING_ID_DLG_Zoom_WholePage));
	radiobuttonWholePage = gtk_radio_button_new_with_label (vboxZoomTo_group, tmp);
	FREEP(tmp);
	vboxZoomTo_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobuttonWholePage));
	g_object_set_data (G_OBJECT (windowZoom), "radiobuttonWholePage", radiobuttonWholePage);
	g_object_set_data (G_OBJECT (radiobuttonWholePage), WIDGET_ID_TAG_KEY, GINT_TO_POINTER(XAP_Frame::z_WHOLEPAGE));
	gtk_widget_show (radiobuttonWholePage);
	gtk_box_pack_start (GTK_BOX (vboxZoomTo), radiobuttonWholePage, TRUE, TRUE, 0);

	UT_XML_cloneNoAmpersands(tmp, pSS->getValue(XAP_STRING_ID_DLG_Zoom_Percent));
	radiobuttonPercent = gtk_radio_button_new_with_label (vboxZoomTo_group, tmp);
	FREEP(tmp);
	vboxZoomTo_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobuttonPercent));
	g_object_set_data (G_OBJECT (windowZoom), "radiobuttonPercent", radiobuttonPercent);
	g_object_set_data (G_OBJECT (radiobuttonPercent), WIDGET_ID_TAG_KEY, GINT_TO_POINTER(XAP_Frame::z_PERCENT));
	gtk_widget_show (radiobuttonPercent);
	gtk_box_pack_start (GTK_BOX (vboxZoomTo), radiobuttonPercent, TRUE, TRUE, 0);

	spinbuttonPercent_adj = gtk_adjustment_new (1, 1, 500, 1, 10, 10);
	spinbuttonPercent = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonPercent_adj), 1, 0);
	g_object_set_data (G_OBJECT (windowZoom), "spinbuttonPercent", spinbuttonPercent);
	gtk_widget_show (spinbuttonPercent);
	gtk_box_pack_end (GTK_BOX (vboxZoomTo), spinbuttonPercent, TRUE, TRUE, 0);
	gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonPercent), TRUE);

	UT_XML_cloneNoAmpersands(tmp, pSS->getValue(XAP_STRING_ID_DLG_Zoom_PreviewFrame));
	framePreview = gtk_frame_new (tmp);
	FREEP(tmp);
	g_object_set_data (G_OBJECT (windowZoom), "framePreview", framePreview);
	gtk_widget_show (framePreview);
	gtk_box_pack_start (GTK_BOX (hboxFrames), framePreview, TRUE, TRUE, 0);

	// *** This is how we do a preview widget ***
	{
		drawingareaPreview = createDrawingArea ();
		g_object_set_data (G_OBJECT (windowZoom), "drawingareaPreview", drawingareaPreview);
		gtk_widget_show (drawingareaPreview);
		gtk_container_add (GTK_CONTAINER (framePreview), drawingareaPreview);
		gtk_widget_set_usize (drawingareaPreview, 149, 10);
   	}
	
	buttonOK = GTK_WIDGET (g_list_first (GNOME_DIALOG (windowZoom)->buttons)->data);
	buttonCancel = GTK_WIDGET (g_list_last (GNOME_DIALOG (windowZoom)->buttons)->data);
	g_object_set_data (G_OBJECT (windowZoom), "buttonOK", buttonOK);
	g_object_set_data (G_OBJECT (windowZoom), "buttonCancel", buttonCancel);

	// the control buttons
	gnome_dialog_button_connect(GNOME_DIALOG(windowZoom), 0,
								G_CALLBACK(s_ok_clicked), (gpointer) this);

	gnome_dialog_button_connect(GNOME_DIALOG(windowZoom), 1,
								G_CALLBACK(s_cancel_clicked), (gpointer) this);

	// the radio buttons
	g_signal_connect(G_OBJECT(radiobutton200),
					   "clicked",
					   G_CALLBACK(s_radio_200_clicked),
					   (gpointer) this);
	g_signal_connect(G_OBJECT(radiobutton100),
					   "clicked",
					   G_CALLBACK(s_radio_100_clicked),
					   (gpointer) this);
	g_signal_connect(G_OBJECT(radiobutton75),
					   "clicked",
					   G_CALLBACK(s_radio_75_clicked),
					   (gpointer) this);
	g_signal_connect(G_OBJECT(radiobuttonPageWidth),
					   "clicked",
					   G_CALLBACK(s_radio_PageWidth_clicked),
					   (gpointer) this);
	g_signal_connect(G_OBJECT(radiobuttonWholePage),
					   "clicked",
					   G_CALLBACK(s_radio_WholePage_clicked),
					   (gpointer) this);
	g_signal_connect(G_OBJECT(radiobuttonPercent),
					   "clicked",
					   G_CALLBACK(s_radio_Percent_clicked),
					   (gpointer) this);

	// the spin button
	g_signal_connect(G_OBJECT(spinbuttonPercent_adj),
					   "value_changed",
					   G_CALLBACK(s_spin_Percent_changed),
					   (gpointer) this);
	
	// the catch-alls
	
	g_signal_connect(G_OBJECT(windowZoom),
			   "delete_event",
			   G_CALLBACK(s_delete_clicked),
			   (gpointer) this);

	g_signal_connect_after(G_OBJECT(windowZoom),
							 "destroy",
							 NULL,
							 NULL);

	g_signal_connect(G_OBJECT(windowZoom),
			   "close",
			   G_CALLBACK(s_cancel_clicked),
			   (gpointer) this);

	// the expose event off the preview
	g_signal_connect(G_OBJECT(drawingareaPreview),
					   "expose_event",
					   G_CALLBACK(s_preview_exposed),
					   (gpointer) this);
	
	// Update member variables with the important widgets that
	// might need to be queried or altered later.

	m_windowMain = windowZoom;

	m_buttonOK = buttonOK;
	m_buttonCancel = buttonCancel;

	m_previewArea = 	drawingareaPreview;
	
	m_radio200 = 		radiobutton200;
	m_radio100 = 		radiobutton100;
	m_radio75 = 		radiobutton75;
	m_radioPageWidth = 	radiobuttonPageWidth;
	m_radioWholePage = 	radiobuttonWholePage;
	m_radioPercent = 	radiobuttonPercent;

	m_spinPercent = spinbuttonPercent;

	m_radioGroup = vboxZoomTo_group;
	
	setDefaultButton (GNOME_DIALOG(windowZoom), 1);

	return windowZoom;
}
