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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ut_misc.h"
#include "ut_units.h"
#include "ut_dialogHelper.h"
#include "xap_UnixDlg_FontChooser.h"
#include "xap_UnixApp.h"
#include "xap_UnixFrame.h"
#include "xap_EncodingManager.h"
#include "gr_UnixGraphics.h"

#define SIZE_STRING_SIZE	(10+2)	

#define PREVIEW_BOX_BORDER_WIDTH_PIXELS 8
#define PREVIEW_BOX_HEIGHT_PIXELS	80

// your typographer's standard nonsense latin font phrase
#define PREVIEW_ENTRY_DEFAULT_STRING	"Lorem ipsum dolor sit amet, consectetaur adipisicing..."

/*****************************************************************/
XAP_Dialog * XAP_UnixDialog_FontChooser::static_constructor(XAP_DialogFactory * pFactory,
														 XAP_Dialog_Id id)
{
	XAP_UnixDialog_FontChooser * p = new XAP_UnixDialog_FontChooser(pFactory,id);
	return p;
}

XAP_UnixDialog_FontChooser::XAP_UnixDialog_FontChooser(XAP_DialogFactory * pDlgFactory,
												   XAP_Dialog_Id id)
	: XAP_Dialog_FontChooser(pDlgFactory,id)
{
	m_fontManager = NULL;
	m_fontList = NULL;
	m_styleList = NULL;
	m_sizeList = NULL;
	m_checkStrikeOut = NULL;
	m_checkUnderline = NULL;
	m_colorSelector = NULL;
	m_preview = NULL;

	m_lastFont = NULL;
	m_gc = NULL;
	m_pUnixFrame = NULL;

	m_doneFirstFont = false;
}

XAP_UnixDialog_FontChooser::~XAP_UnixDialog_FontChooser(void)
{
	DELETEP(m_gc);
	DELETEP(m_lastFont);
}

/*****************************************************************/

// This is a little UI hack.  Check the widget callback connect point
// for more info.

static gint s_color_wheel_clicked(GtkWidget * area,
                                  GdkEvent * event)
{
	GtkColorSelection * colorSelector;

	colorSelector = (GtkColorSelection *) gtk_object_get_data(GTK_OBJECT(area), "_GtkColorSelection");
  
	// these were stolen from gtkcolorsel.c, are private data,
	// and as such are subject to immediate change and breakage
	// without any warning.  the things we do for the user.  :)
	enum
	{
		HUE,
		SATURATION,
		VALUE,
		RED,
		GREEN,
		BLUE,
		OPACITY,
		NUM_CHANNELS
	};

	switch (event->type)
    {
    case GDK_BUTTON_PRESS:
		// if the color is black (RGB:0,0,0), and someone clicked on
		// the wheel, and since the wheel has no black area,
		// snap the value up high

		// I first tried checking the hue, but I _always_ got the
		// "modified" version, which means it was never -1 but always
		// processed through the wheel coordinate system, even if
		// I did connect_after(), etc.  So I catch the RGB values,
		// since they're a direct product of the hue, but aren't updated
		// unless the value slider itself is.

		// the 'less than' case catches the state of the color selector
		// when no single color occupied the entire run of text
		if ((gdouble) GTK_COLOR_SELECTION(colorSelector)->values[RED] 	<= (gdouble) 0 &&
			(gdouble) GTK_COLOR_SELECTION(colorSelector)->values[GREEN] <= (gdouble) 0 &&
			(gdouble) GTK_COLOR_SELECTION(colorSelector)->values[BLUE] 	<= (gdouble) 0)

		{
			// snap the "value" slider high
			GTK_COLOR_SELECTION(colorSelector)->values[VALUE] = 1.0;
		}
		break;
	default:
		break;
    }

	return FALSE;
}

static gint s_color_update(GtkWidget * /* widget */,
						   GdkEvent * /* event */,
						   XAP_UnixDialog_FontChooser * dlg)
{
	UT_ASSERT(dlg);
	dlg->updatePreview();

	return FALSE;
}

static void s_delete_clicked(GtkWidget * /* widget */,
							 gpointer /* data */,
							 XAP_Dialog_FontChooser::tAnswer * answer)
{
	*answer = XAP_Dialog_FontChooser::a_CANCEL;
	gtk_main_quit();
}

static void s_ok_clicked(GtkWidget * /* widget */,
						 XAP_Dialog_FontChooser::tAnswer * answer)
{	*answer = XAP_Dialog_FontChooser::a_OK;
	gtk_main_quit();
}

static void s_cancel_clicked(GtkWidget * /* widget */,
							 XAP_Dialog_FontChooser::tAnswer * answer)
{
	*answer = XAP_Dialog_FontChooser::a_CANCEL;
	gtk_main_quit();
}

static void s_select_row_font(GtkWidget * /* widget */,
							  gint /* row */,
							  gint /* column */,
							  GdkEventButton * /* event */,
							  XAP_UnixDialog_FontChooser * dlg)
{
	UT_ASSERT(dlg);

	// redisplay the preview text
	dlg->updatePreview();
}

static void s_select_row_style(GtkWidget * /* widget */,
							   gint /* row */,
							   gint /* column */,
							   GdkEventButton * /* event */,
							   XAP_UnixDialog_FontChooser * dlg)
{
	UT_ASSERT(dlg);

	// redisplay the preview text
	dlg->updatePreview();
}

static void s_select_row_size(GtkWidget * /* widget */,
							  gint /* row */,
							  gint /* column */,
							  GdkEventButton * /* event */,
							  XAP_UnixDialog_FontChooser * dlg)
{
	UT_ASSERT(dlg);

	// redisplay the preview text
	dlg->updatePreview();
}

static gint s_drawing_area_expose(GtkWidget * w,
								  GdkEventExpose * /* pExposeEvent */)
{
	XAP_UnixDialog_FontChooser * dlg = (XAP_UnixDialog_FontChooser *)
		                              gtk_object_get_user_data(GTK_OBJECT(w));

	// if a font has been set since this dialog was launched, draw things with it
	if (dlg->m_doneFirstFont)
	{
		char * entryString;

		if (!dlg->getEntryString(&entryString))
			return FALSE;

		UT_UCSChar * unicodeString = NULL;
		UT_UCS_cloneString_char(&unicodeString, entryString);

		// erase the area with the background color of the document
		UT_RGBColor bgcolor;
		dlg->getBackgroundColor(&bgcolor);
		dlg->m_gc->fillRect(bgcolor, 0, 0, GTK_WIDGET(dlg->m_preview)->allocation.width,
							GTK_WIDGET(dlg->m_preview)->allocation.height);

		// get metrics to center vertically in box
		UT_sint32 top = (UT_sint32) PREVIEW_BOX_HEIGHT_PIXELS / (UT_sint32) 2 -
			            (UT_sint32) dlg->m_gc->getFontHeight() / (UT_sint32) 2;

		top -= 4; // things seem to line up better this way

		// draw in 5 pixels or so from left edge
		dlg->m_gc->drawChars(unicodeString, 0, UT_UCS_strlen(unicodeString), 5, top);
		FREEP(unicodeString);
#if 0
		// TODO:
		
		// we draw decorations after the text, even though the underline looks
		// kinda funny this way
		UT_sint32 iDrop = (dlg->m_gc->getFontDescent() / 3);
		dlg->m_gc->drawLine(5, top + iDrop + dlg->m_gc->getFontAscent(),
							5 + getWidth(), yoff + iDrop + m_iAscent);
			if (m_fDecorations & TEXT_DECOR_OVERLINE)
	{
		UT_sint32 y2 = yoff;
		m_pG->drawLine(xoff, y2, xoff+getWidth(), y2);
	}

	if (m_fDecorations & TEXT_DECOR_LINETHROUGH)
	{
		UT_sint32 y2 = yoff + getAscent() * 2 / 3;
		m_pG->drawLine(xoff, y2, xoff+getWidth(), y2);
		
	}
#endif

	}
	else
	{
		// we get here when NO font has been set, because the user hasn't
		// set one and the dialog didn't know enough to find its own

		// simply fill
		UT_RGBColor bgcolor;
		dlg->getBackgroundColor(&bgcolor);
		dlg->m_gc->fillRect(bgcolor, 0, 0, GTK_WIDGET(dlg->m_preview)->allocation.width,
							GTK_WIDGET(dlg->m_preview)->allocation.height);
	}
		
	return FALSE;
}

/*****************************************************************/

// Glade helper function
void XAP_UnixDialog_FontChooser::set_notebook_tab(GtkWidget * notebook, gint page_num,
												 GtkWidget * widget)
{
	GtkNotebookPage *page;
	GtkWidget *notebook_page;

	page = (GtkNotebookPage*) g_list_nth (GTK_NOTEBOOK (notebook)->children, page_num)->data;
	notebook_page = page->child;
	gtk_widget_ref (notebook_page);
	gtk_notebook_remove_page (GTK_NOTEBOOK (notebook), page_num);
	gtk_notebook_insert_page (GTK_NOTEBOOK (notebook), notebook_page,
							  widget, page_num);
	gtk_widget_unref (notebook_page);
}

GtkWidget * XAP_UnixDialog_FontChooser::constructWindow(void)
{
	const XAP_StringSet * pSS = m_pApp->getStringSet();
	GtkWidget *windowFontSelection;
	GtkWidget *vboxMain;
	GtkWidget *vboxOuter;

	GtkWidget *fixedButtons;
	GtkWidget *buttonOK;
	GtkWidget *buttonCancel;

	windowFontSelection = gtk_window_new (GTK_WINDOW_DIALOG);
	gtk_object_set_data (GTK_OBJECT (windowFontSelection), "windowFontSelection", windowFontSelection);
	gtk_window_set_title (GTK_WINDOW (windowFontSelection), (const gchar*)pSS->getValue(XAP_STRING_ID_DLG_UFS_FontTitle));
	gtk_window_set_policy (GTK_WINDOW (windowFontSelection), FALSE, TRUE, FALSE);

	vboxOuter = gtk_vbox_new (FALSE, 0);
	gtk_object_set_data (GTK_OBJECT (windowFontSelection), "vboxOuter", vboxOuter);
	gtk_widget_show (vboxOuter);
	gtk_container_add (GTK_CONTAINER (windowFontSelection), vboxOuter);

	vboxMain = constructWindowContents(GTK_OBJECT (windowFontSelection));
	gtk_box_pack_start (GTK_BOX (vboxOuter), vboxMain, TRUE, TRUE, 0);

	fixedButtons = gtk_fixed_new ();
	gtk_object_set_data (GTK_OBJECT (windowFontSelection), "fixedButtons", fixedButtons);
	gtk_widget_show (fixedButtons);
	gtk_box_pack_start (GTK_BOX (vboxOuter), fixedButtons, FALSE, TRUE, 0);

	buttonOK = gtk_button_new_with_label ((const gchar*)pSS->getValue(XAP_STRING_ID_DLG_OK));
	gtk_object_set_data (GTK_OBJECT (windowFontSelection), "buttonOK", buttonOK);
	gtk_widget_show (buttonOK);
	gtk_fixed_put (GTK_FIXED (fixedButtons), buttonOK, 279, 0);
	GTK_WIDGET_SET_FLAGS (buttonOK, GTK_CAN_DEFAULT);
	gtk_widget_grab_default (buttonOK);

	buttonCancel = gtk_button_new_with_label ((const gchar*)pSS->getValue(XAP_STRING_ID_DLG_Cancel));
	gtk_object_set_data (GTK_OBJECT (windowFontSelection), "buttonCancel", buttonCancel);
	gtk_widget_show (buttonCancel);
	gtk_fixed_put (GTK_FIXED (fixedButtons), buttonCancel, 374, 6);

	gtk_signal_connect_after(GTK_OBJECT(windowFontSelection),
							  "destroy",
							  NULL,
							  NULL);
	gtk_signal_connect_after(GTK_OBJECT(windowFontSelection),
							 "delete_event",
							 GTK_SIGNAL_FUNC(s_delete_clicked),
							 (gpointer) &m_answer);

	gtk_signal_connect(GTK_OBJECT(buttonOK),
					   "clicked",
					   GTK_SIGNAL_FUNC(s_ok_clicked),
					   (gpointer) &m_answer);
	gtk_signal_connect(GTK_OBJECT(buttonCancel),
					   "clicked",
					   GTK_SIGNAL_FUNC(s_cancel_clicked),
					   (gpointer) &m_answer);

	return windowFontSelection;
}

// Glade generated dialog, using fixed widgets to closely match
// the Windows layout, with some changes for color selector
GtkWidget * XAP_UnixDialog_FontChooser::constructWindowContents(GtkObject *parent)
{
	GtkWidget *vboxMain;
	GtkWidget *notebookMain;
	GtkWidget *labelFont;
	GtkWidget *labelStyle;
	GtkWidget *listFonts;
	GtkWidget *labelSize;
	GtkWidget *frameEffects;
	GtkWidget *hboxDecorations;
	GtkWidget *checkbuttonStrikeout;
	GtkWidget *checkbuttonUnderline;
	GtkWidget *labelEncoding;
	GtkWidget *comboEncoding;
	GtkWidget *listStyles;
	GtkWidget *listSizes;
	GtkWidget *hbox1;
	GtkWidget *colorSelector;
	GtkWidget *labelTabFont;
	GtkWidget *labelTabColor;
	GtkWidget *frame4;

	/*
	  GtkWidget *fixedFont;
	  GtkWidget *frameStyle;
	  GtkWidget *frameFonts;
	  GtkWidget *frameSize;
	  GtkWidget *fixedColor;
	*/

	// the entry is a special drawing area full of one
	// of our graphics contexts
	GtkWidget *entryArea;

	const XAP_StringSet * pSS = m_pApp->getStringSet();
	
	vboxMain = gtk_vbox_new (FALSE, 0);
	gtk_object_set_data (parent, "vboxMain", vboxMain);
	gtk_widget_show (vboxMain);

	notebookMain = gtk_notebook_new ();
	gtk_object_set_data (parent, "notebookMain", notebookMain);
	gtk_widget_show (notebookMain);
	gtk_box_pack_start (GTK_BOX (vboxMain), notebookMain, 1, 1, 0);
	gtk_container_set_border_width (GTK_CONTAINER (notebookMain), 8);

	GtkObject *window1 = parent;	
  	GtkWidget *table1;
  	GtkWidget *vbox1;
  
 	GtkWidget *scrolledwindow1;
  	GtkWidget *vbox2;
  	GtkWidget *scrolledwindow2;
  	GtkWidget *vbox3;
  	GtkWidget *scrolledwindow3;
  	GtkWidget *vboxmisc;
  	GtkWidget *hboxForEncoding;

	table1 = gtk_table_new (2, 3, FALSE);
	gtk_widget_set_name (table1, "table1");
	gtk_widget_ref (table1);
	gtk_object_set_data_full (GTK_OBJECT (window1), "table1", table1,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (table1);
	gtk_container_add (GTK_CONTAINER (notebookMain), table1);

	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_set_name (vbox1, "vbox1");
	gtk_widget_ref (vbox1);
	gtk_object_set_data_full (GTK_OBJECT (window1), "vbox1", vbox1,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (vbox1);
	gtk_table_attach (GTK_TABLE (table1), vbox1, 0, 1, 0, 2,
	                  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	                  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

	labelFont = gtk_label_new ((const gchar *)pSS->getValue(XAP_STRING_ID_DLG_UFS_FontLabel));
	gtk_widget_set_name (labelFont, "labelFont");
	gtk_widget_ref (labelFont);
	gtk_object_set_data_full (GTK_OBJECT (window1), "labelFont", labelFont,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (labelFont);
	gtk_box_pack_start (GTK_BOX (vbox1), labelFont, FALSE, FALSE, 0);

	scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_name (scrolledwindow1, "scrolledwindow1");
	gtk_widget_ref (scrolledwindow1);
	gtk_object_set_data_full (GTK_OBJECT (window1), "scrolledwindow1", scrolledwindow1,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (scrolledwindow1);
	gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow1), 3);

	listFonts = gtk_clist_new (1);
	gtk_widget_set_name (listFonts, "listFonts");
	gtk_widget_ref (listFonts);
	gtk_object_set_data_full (GTK_OBJECT (window1), "listFonts", listFonts,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (listFonts);
	gtk_container_add (GTK_CONTAINER (scrolledwindow1), listFonts);
	gtk_clist_set_column_auto_resize (GTK_CLIST(listFonts), 0, TRUE);	

	vbox2 = gtk_vbox_new (FALSE, 0);
	gtk_widget_set_name (vbox2, "vbox2");
	gtk_widget_ref (vbox2);
	gtk_object_set_data_full (GTK_OBJECT (window1), "vbox2", vbox2,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (vbox2);
	gtk_table_attach (GTK_TABLE (table1), vbox2, 1, 2, 0, 1,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (GTK_FILL), 0, 0);

	labelStyle = gtk_label_new ((const gchar*)pSS->getValue(XAP_STRING_ID_DLG_UFS_StyleLabel));
	gtk_widget_set_name (labelStyle, "labelStyle");
	gtk_widget_ref (labelStyle);
	gtk_object_set_data_full (GTK_OBJECT (window1), "labelStyle", labelStyle,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (labelStyle);
	gtk_box_pack_start (GTK_BOX (vbox2), labelStyle, FALSE, FALSE, 0);

	scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_name (scrolledwindow2, "scrolledwindow2");
	gtk_widget_ref (scrolledwindow2);
	gtk_object_set_data_full (GTK_OBJECT (window1), "scrolledwindow2", scrolledwindow2,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (scrolledwindow2);
	gtk_box_pack_start (GTK_BOX (vbox2), scrolledwindow2, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow2), GTK_POLICY_NEVER, GTK_POLICY_NEVER);
	gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow2), 3);

	listStyles = gtk_clist_new (1);
	gtk_widget_set_name (listStyles, "listStyles");
	gtk_widget_ref (listStyles);
	gtk_object_set_data_full (GTK_OBJECT (window1), "listStyles", listStyles,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (listStyles);
	gtk_container_add (GTK_CONTAINER (scrolledwindow2), listStyles);
	gtk_clist_set_column_auto_resize (GTK_CLIST(listStyles), 0, TRUE);

	vbox3 = gtk_vbox_new (FALSE, 0);
	gtk_widget_set_name (vbox3, "vbox3");
	gtk_widget_ref (vbox3);
	gtk_object_set_data_full (GTK_OBJECT (window1), "vbox3", vbox3,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (vbox3);
	gtk_table_attach (GTK_TABLE (table1), vbox3, 2, 3, 0, 1,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (GTK_FILL), 0, 0);

	labelSize = gtk_label_new ((const gchar*)pSS->getValue(XAP_STRING_ID_DLG_UFS_SizeLabel));
	gtk_widget_set_name (labelSize, "labelSize");
	gtk_widget_ref (labelSize);
	gtk_object_set_data_full (GTK_OBJECT (window1), "labelSize", labelSize,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (labelSize);
	gtk_box_pack_start (GTK_BOX (vbox3), labelSize, FALSE, FALSE, 0);

	scrolledwindow3 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_name (scrolledwindow3, "scrolledwindow3");
	gtk_widget_ref (scrolledwindow3);
	gtk_object_set_data_full (GTK_OBJECT (window1), "scrolledwindow3", scrolledwindow3,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (scrolledwindow3);
	gtk_box_pack_start (GTK_BOX (vbox3), scrolledwindow3, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow3), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow3), 3);
	
	listSizes = gtk_clist_new (1);
	gtk_widget_set_name (listSizes, "listSizes");
	gtk_widget_ref (listSizes);
	gtk_object_set_data_full (GTK_OBJECT (window1), "listSizes", listSizes,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (listSizes);
	gtk_container_add (GTK_CONTAINER (scrolledwindow3), listSizes);
	gtk_clist_set_column_auto_resize (GTK_CLIST(listSizes), 0, TRUE);

	vboxmisc = gtk_vbox_new (FALSE, 0);
	gtk_widget_set_name (vboxmisc, "vboxmisc");
	gtk_widget_ref (vboxmisc);
	gtk_object_set_data_full (GTK_OBJECT (window1), "vboxmisc", vboxmisc,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (vboxmisc);
	gtk_table_attach (GTK_TABLE (table1), vboxmisc, 1, 3, 1, 2,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (GTK_FILL), 0, 0);

	frameEffects = gtk_frame_new ((const gchar*)pSS->getValue(XAP_STRING_ID_DLG_UFS_EffectsFrameLabel));
	gtk_object_set_data (parent, "frameEffects", frameEffects);
	gtk_widget_show (frameEffects);
	gtk_box_pack_start(GTK_BOX (vboxmisc), frameEffects, 0,0, 2);

	hboxDecorations = gtk_hbox_new (FALSE, 0);
	gtk_object_set_data (parent, "hboxDecorations", hboxDecorations);
	gtk_widget_show (hboxDecorations);
	gtk_container_add (GTK_CONTAINER (frameEffects), hboxDecorations);

	checkbuttonStrikeout = gtk_check_button_new_with_label ((const gchar*)pSS->getValue(XAP_STRING_ID_DLG_UFS_StrikeoutCheck));
	gtk_object_set_data (parent, "checkbuttonStrikeout", checkbuttonStrikeout);
	gtk_container_border_width (GTK_CONTAINER (checkbuttonStrikeout), 5);
	gtk_widget_show (checkbuttonStrikeout);
	gtk_box_pack_start (GTK_BOX (hboxDecorations), checkbuttonStrikeout, TRUE, TRUE, 0);

	checkbuttonUnderline = gtk_check_button_new_with_label ((const gchar*)pSS->getValue(XAP_STRING_ID_DLG_UFS_UnderlineCheck));
	gtk_object_set_data (parent, "checkbuttonUnderline", checkbuttonUnderline);
	gtk_container_border_width (GTK_CONTAINER (checkbuttonUnderline), 5);
	gtk_widget_show (checkbuttonUnderline);
	gtk_box_pack_start (GTK_BOX (hboxDecorations), checkbuttonUnderline, TRUE, TRUE, 0);


	hboxForEncoding = gtk_hbox_new (FALSE, 0);
	gtk_widget_set_name (hboxForEncoding, "hboxForEncoding");
	gtk_widget_ref (hboxForEncoding);
	gtk_object_set_data_full (GTK_OBJECT (window1), "hboxForEncoding", hboxForEncoding,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (hboxForEncoding);
	gtk_box_pack_start (GTK_BOX (vboxmisc), hboxForEncoding, TRUE, TRUE, 0);
	
	labelEncoding = gtk_label_new ((const gchar*)pSS->getValue(XAP_STRING_ID_DLG_UFS_EncodingLabel));
	gtk_object_set_data (parent, "labelEncoding", labelEncoding);
	gtk_widget_show (labelEncoding);
	gtk_box_pack_start (GTK_BOX (hboxForEncoding), labelEncoding, 1,1, 2);
	gtk_label_set_justify (GTK_LABEL (labelEncoding), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (labelEncoding), 0, 0.5);
	
	comboEncoding = gtk_combo_new ();
	gtk_object_set_data (parent, "comboEncoding", comboEncoding);
	gtk_widget_show (comboEncoding);
	gtk_box_pack_start(GTK_BOX (hboxForEncoding), comboEncoding, 1, 1, 0); 
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (GTK_COMBO (comboEncoding)->popup),
									GTK_POLICY_NEVER,
									GTK_POLICY_AUTOMATIC);
	GList * comboEncoding_items = NULL;
	comboEncoding_items = g_list_append (comboEncoding_items, (void *) "Placeholder");
//	comboEncoding_items = g_list_append (comboEncoding_items, "Canadian");
//	comboEncoding_items = g_list_append (comboEncoding_items, "British");
//	comboEncoding_items = g_list_append (comboEncoding_items, "Irish");
//	comboEncoding_items = g_list_append (comboEncoding_items, "Broken English");
//	comboEncoding_items = g_list_append (comboEncoding_items, "These Are Bogus");	
	gtk_combo_set_popdown_strings (GTK_COMBO (comboEncoding), comboEncoding_items);
	g_list_free (comboEncoding_items);
	/*another Notebook page*/
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_object_set_data (parent, "hbox1", hbox1);
	gtk_widget_show (hbox1);
	gtk_container_add (GTK_CONTAINER (notebookMain),hbox1);

	colorSelector = gtk_color_selection_new ();
	gtk_object_set_data (parent, "colorSelector", colorSelector);
	gtk_widget_show (colorSelector);
	gtk_box_pack_start (GTK_BOX (hbox1), colorSelector, TRUE, TRUE, 0);

	labelTabFont = gtk_label_new ((const gchar*)pSS->getValue(XAP_STRING_ID_DLG_UFS_FontTab));
	gtk_object_set_data (parent, "labelTabFont", labelTabFont);
	gtk_widget_show (labelTabFont);
	set_notebook_tab (notebookMain, 0, labelTabFont);

	labelTabColor = gtk_label_new ((const gchar*)pSS->getValue(XAP_STRING_ID_DLG_UFS_ColorTab));
	gtk_object_set_data (parent, "labelTabColor", labelTabColor);
	gtk_widget_show (labelTabColor);
	set_notebook_tab (notebookMain, 1, labelTabColor);

	/* frame with preview */
	frame4 = gtk_frame_new (NULL);
	gtk_object_set_data (parent, "frame4", frame4);
	gtk_widget_show (frame4);
	gtk_box_pack_start (GTK_BOX (vboxMain), frame4, FALSE, FALSE, PREVIEW_BOX_BORDER_WIDTH_PIXELS);
	// setting the height takes into account the border applied on all
	// sides, so we need to double the single border width
	gtk_widget_set_usize (frame4, -1, PREVIEW_BOX_HEIGHT_PIXELS + (PREVIEW_BOX_BORDER_WIDTH_PIXELS * 2));
	gtk_container_border_width (GTK_CONTAINER (frame4), PREVIEW_BOX_BORDER_WIDTH_PIXELS);
	gtk_frame_set_shadow_type (GTK_FRAME (frame4), GTK_SHADOW_IN);

	entryArea = gtk_drawing_area_new ();
	gtk_widget_set_events(entryArea, GDK_EXPOSURE_MASK);
	gtk_signal_connect(GTK_OBJECT(entryArea), "expose_event",
					   GTK_SIGNAL_FUNC(s_drawing_area_expose), NULL);
	gtk_widget_set_usize (entryArea, -1, PREVIEW_BOX_HEIGHT_PIXELS);
	gtk_widget_show (entryArea);
	gtk_container_add (GTK_CONTAINER (frame4), entryArea);


	// save out to members for callback and class access
    m_fontList = listFonts;
	m_styleList = listStyles;
	m_sizeList = listSizes;
	m_colorSelector = colorSelector;
	m_preview = entryArea;
	m_checkStrikeOut = checkbuttonStrikeout;
	m_checkUnderline = checkbuttonUnderline;

	// bind signals to things
	gtk_signal_connect(GTK_OBJECT(listFonts),
					   "select_row",
					   GTK_SIGNAL_FUNC(s_select_row_font),
					   (gpointer) this);
	gtk_signal_connect(GTK_OBJECT(listStyles),
					   "select_row",
					   GTK_SIGNAL_FUNC(s_select_row_style),
					   (gpointer) this);
	gtk_signal_connect(GTK_OBJECT(listSizes),
					   "select_row",
					   GTK_SIGNAL_FUNC(s_select_row_size),
					   (gpointer) this);

	// we catch the color tab's wheel click event so we can do some nasty
	// trickery with the value slider, so that when the user first does
	// some wheel work, the value soars to 100%, instead of 0%, so the
	// color they get is not always black
	//
	// also, we must do connect_after() so we get the information before
	// any other native color selector events get the chance to change
	// things on us
	gtk_signal_connect_after(GTK_OBJECT(GTK_COLOR_SELECTION(colorSelector)->wheel_area),
							 "event",
							 GTK_SIGNAL_FUNC(s_color_wheel_clicked),
							 (gpointer) GTK_COLOR_SELECTION(colorSelector)->wheel_area);

	// This is a catch-all color selector callback which catches any
	// real-time updating of the color so we can refresh our preview
	// text
	gtk_signal_connect(GTK_OBJECT(colorSelector),
					   "event",
					   GTK_SIGNAL_FUNC(s_color_update),
					   (gpointer) this);
	
	GTK_WIDGET_SET_FLAGS(listFonts, GTK_CAN_FOCUS);
	GTK_WIDGET_SET_FLAGS(listStyles, GTK_CAN_FOCUS);
	GTK_WIDGET_SET_FLAGS(listSizes, GTK_CAN_FOCUS);

	gchar * text[2] = {NULL, NULL};

	// update the styles list
	gtk_clist_freeze(GTK_CLIST(m_styleList));
	gtk_clist_clear(GTK_CLIST(m_styleList));
	text[0] = (gchar *) pSS->getValue(XAP_STRING_ID_DLG_UFS_StyleRegular); 		gtk_clist_append(GTK_CLIST(m_styleList), text);
	text[0] = (gchar *) pSS->getValue(XAP_STRING_ID_DLG_UFS_StyleItalic); 		gtk_clist_append(GTK_CLIST(m_styleList), text);
	text[0] = (gchar *) pSS->getValue(XAP_STRING_ID_DLG_UFS_StyleBold); 	   	gtk_clist_append(GTK_CLIST(m_styleList), text);
	text[0] = (gchar *) pSS->getValue(XAP_STRING_ID_DLG_UFS_StyleBoldItalic);  	gtk_clist_append(GTK_CLIST(m_styleList), text);	
	gtk_clist_thaw(GTK_CLIST(m_styleList));

	gtk_clist_freeze(GTK_CLIST(m_sizeList));
	gtk_clist_clear(GTK_CLIST(m_sizeList));
	// TODO perhaps populate the list based on the selected font/style?
	{
	    int sz = XAP_EncodingManager::fontsizes_list.size();
	    for(int i=0;i<sz;++i) {
		text[0]=(char*)XAP_EncodingManager::fontsizes_list.nth2(i);
		gtk_clist_append(GTK_CLIST(m_sizeList), text);
	    };
	}
	gtk_clist_thaw(GTK_CLIST(m_sizeList));

	return vboxMain;
}

void XAP_UnixDialog_FontChooser::runModal(XAP_Frame * pFrame)
{
	m_pUnixFrame = (XAP_UnixFrame *)pFrame;
	UT_ASSERT(m_pUnixFrame);

	UT_ASSERT(m_pApp);

	UT_DEBUGMSG(("FontChooserStart: Family[%s] Size[%s] Weight[%s] Style[%s] Color[%s] Underline[%d] StrikeOut[%d]\n",
				 ((m_pFontFamily) ? m_pFontFamily : ""),
				 ((m_pFontSize) ? m_pFontSize : ""),
				 ((m_pFontWeight) ? m_pFontWeight : ""),
				 ((m_pFontStyle) ? m_pFontStyle : ""),
				 ((m_pColor) ? m_pColor : "" ),
				 (m_bUnderline),
				 (m_bStrikeOut)));

	// These define the color element offsets in a vector
	guint RED = 0;
	guint GREEN = 1;
	guint BLUE = 2;
	gdouble currentColor[3] = { 0, 0, 0 };
	gdouble funkyColor[3] = { -1, -1, -1 };

	// this is used many times below to grab pointers to
	// strings inside list elements
	gchar * text[2] = {NULL, NULL};
	// used similarly to convert between text and numeric arguments
	char sizeString[SIZE_STRING_SIZE];
	
	// Set up our own color space so we work well on 8-bit
	// displays.
    gtk_widget_push_visual(gtk_preview_get_visual());
    gtk_widget_push_colormap(gtk_preview_get_cmap());

	// establish the font manager before dialog creation
	XAP_App * app = m_pUnixFrame->getApp();
	XAP_UnixApp * unixapp = static_cast<XAP_UnixApp *> (app);
	m_fontManager = unixapp->getFontManager();

	// build the dialog
	GtkWidget * cf = constructWindow();
	UT_ASSERT(cf);
	connectFocus(GTK_WIDGET(cf),pFrame);

	// freeze updates of the preview
	m_blockUpdate = true;

	// to sort out dupes
	UT_HashTable fontHash(256);

	// throw them in the hash save duplicates
	XAP_UnixFont ** fonts = m_fontManager->getAllFonts();
	for (UT_uint32 i = 0; i < m_fontManager->getCount(); i++)
	{
		if (!fontHash.findEntry(fonts[i]->getName()))
			fontHash.addEntry((char *) fonts[i]->getName(),
							  (char *) fonts[i]->getName(), NULL);
	}
	DELETEP(fonts);

	// fetch them out
	UT_HashEntry * entry;
	gtk_clist_freeze(GTK_CLIST(m_fontList));
	gtk_clist_clear(GTK_CLIST(m_fontList));
	for (UT_uint32 k = 0; k < (UT_uint32) fontHash.getEntryCount(); k++)
	{
		entry = fontHash.getNthEntry((int) k);
		UT_ASSERT(entry);
		text[0] = (gchar *) entry->pszLeft;
		gtk_clist_append(GTK_CLIST(m_fontList), text);
	}
	gtk_clist_thaw(GTK_CLIST(m_fontList));

	// Set the defaults in the list boxes according to dialog data
	gint foundAt = 0;

	// is this safe with an XML_Char * string?
	foundAt = searchCList(GTK_CLIST(m_fontList), (char *) m_pFontFamily);

	if (foundAt >= 0)
	{
		gtk_clist_select_row(GTK_CLIST(m_fontList), foundAt, 0);
		//gtk_clist_moveto(GTK_CLIST(m_fontList), foundAt, 0, 0, -1);
	}
	
	// this is pretty messy
	listStyle st = LIST_STYLE_NORMAL;
	if (!m_pFontStyle || !m_pFontWeight)
	{
		// select nothing
	        st = LIST_STYLE_NONE;
	}
	else if (!strcmp((const char*)m_pFontStyle, "normal"))
	{
		if (!strcmp((const char*)m_pFontWeight, "normal"))
			st = LIST_STYLE_NORMAL;
		else if (!strcmp((const char*)m_pFontWeight, "bold"))
			st = LIST_STYLE_BOLD;
		else
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}
	else if (!strcmp((const char*)m_pFontStyle, "italic"))
	{
		if (!strcmp((const char*)m_pFontWeight, "normal"))
			st = LIST_STYLE_ITALIC;		
		else if (!strcmp((const char*)m_pFontWeight, "bold"))
			st = LIST_STYLE_BOLD_ITALIC;
		else
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}
	else
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	if (st != LIST_STYLE_NONE)
	  gtk_clist_select_row(GTK_CLIST(m_styleList), st, 0);
	//gtk_clist_moveto(GTK_CLIST(m_styleList), st, 0, 0, -1);

	g_snprintf(sizeString, SIZE_STRING_SIZE, "%s", std_size_string(UT_convertToPoints((const char*)m_pFontSize)));
	foundAt = searchCList(GTK_CLIST(m_sizeList), (char *)XAP_EncodingManager::fontsizes_list.getSecond(sizeString));
	

	if (foundAt >= 0)
	{
		gtk_clist_select_row(GTK_CLIST(m_sizeList), foundAt, 0);
		//gtk_clist_moveto(GTK_CLIST(m_sizeList), foundAt, 0, 0, -1);
	}
	
	// Set color in the color selector
	if (m_pColor)
	{
		UT_RGBColor c;
		UT_parseColor((const char*)m_pColor, c);

		currentColor[RED] = ((gdouble) c.m_red / (gdouble) 255.0);
		currentColor[GREEN] = ((gdouble) c.m_grn / (gdouble) 255.0);
		currentColor[BLUE] = ((gdouble) c.m_blu / (gdouble) 255.0);

		gtk_color_selection_set_color(GTK_COLOR_SELECTION(m_colorSelector), currentColor);
	}
	else
	{
		// if we have no color, use a placeholder of funky values
		// the user can't pick interactively.  This catches ALL
		// the cases except where the user specifically enters -1 for
		// all Red, Green and Blue attributes manually.  This user
		// should expect it not to touch the color.  :)
		gtk_color_selection_set_color(GTK_COLOR_SELECTION(m_colorSelector), funkyColor);
	}

	// set the strikeout and underline check buttons
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_checkStrikeOut), m_bStrikeOut);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_checkUnderline), m_bUnderline);	
	
	// get top level window and its GtkWidget *
	XAP_UnixFrame * frame = static_cast<XAP_UnixFrame *>(pFrame);
	UT_ASSERT(frame);
	GtkWidget * parent = frame->getTopLevelWindow();
	UT_ASSERT(parent);
	// center it
    centerDialog(parent, GTK_WIDGET(cf));
	
	// Run the dialog
	gtk_widget_show(GTK_WIDGET(cf));
	gtk_grab_add(GTK_WIDGET(cf));

	m_doneFirstFont = false;
	
	// attach a new graphics context
	XAP_App *pApp = frame->getApp();
	m_gc = new GR_UnixGraphics(m_preview->window, m_fontManager, pApp);
	gtk_object_set_user_data(GTK_OBJECT(m_preview), this);
	
	// unfreeze updates of the preview
	m_blockUpdate = false;
	// manually trigger an update
	updatePreview();
	
	gtk_main();

	if (m_answer == XAP_Dialog_FontChooser::a_OK)
	{
		GList * selectedRow = NULL;
		gint rowNumber = 0;
		
		selectedRow = GTK_CLIST(m_fontList)->selection;
		if (selectedRow)
		{
			rowNumber = GPOINTER_TO_INT(selectedRow->data);
			gtk_clist_get_text(GTK_CLIST(m_fontList), rowNumber, 0, text);
			UT_ASSERT(text && text[0]);
			if (!m_pFontFamily || strcmp((const char*)m_pFontFamily, text[0]))
			{
				setFontFamily((XML_Char*)text[0]);
				m_bChangedFontFamily = true;
			}
		}
		
		selectedRow = GTK_CLIST(m_styleList)->selection;
		if (selectedRow)
		{
			rowNumber = GPOINTER_TO_INT(selectedRow->data);
			gtk_clist_get_text(GTK_CLIST(m_styleList), rowNumber, 0, text);
			UT_ASSERT(text && text[0]);

		// perhaps these attributes really should be smashed
		// into bitfields.  :)
			switch (rowNumber)
			{
			case LIST_STYLE_NORMAL:
			{
				if(!m_pFontStyle || strcmp((const char*)m_pFontStyle, "normal"))
				{
					setFontStyle((XML_Char*)"normal");
					m_bChangedFontStyle = true;
				}
				if(!m_pFontWeight || strcmp((const char*)m_pFontWeight, "normal"))
				{
					setFontWeight((XML_Char*)"normal");
					m_bChangedFontWeight = true;
				}
				break;
			}
			case LIST_STYLE_BOLD:
			{
				if(!m_pFontStyle || strcmp((const char*)m_pFontStyle, "normal"))
				{
					setFontStyle((XML_Char*)"normal");
					m_bChangedFontStyle = true;
				}
				if(!m_pFontWeight || strcmp((const char*)m_pFontWeight, "bold"))
				{
					setFontWeight((XML_Char*)"bold");
					m_bChangedFontWeight = true;
				}
				break;
			}
			case LIST_STYLE_ITALIC:
			{
				if(!m_pFontStyle || strcmp((const char*)m_pFontStyle, "italic"))
				{
					setFontStyle((XML_Char*)"italic");
					m_bChangedFontStyle = true;
				}
				if(!m_pFontWeight || strcmp((const char*)m_pFontWeight, "normal"))
				{
					setFontWeight((XML_Char*)"normal");
					m_bChangedFontWeight = true;
				}
				break;
			}
			case LIST_STYLE_BOLD_ITALIC:
			{
				if(!m_pFontStyle || strcmp((const char*)m_pFontStyle, "italic"))
				{
					setFontStyle((XML_Char*)"italic");
					m_bChangedFontStyle = true;
				}
				if(!m_pFontWeight || strcmp((const char*)m_pFontWeight, "bold"))
				{
					setFontWeight((XML_Char*)"bold");
					m_bChangedFontWeight = true;
				}
				break;
			}
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			}
		}
		
		selectedRow = GTK_CLIST(m_sizeList)->selection;
		if (selectedRow)
		{
			rowNumber = GPOINTER_TO_INT(selectedRow->data);
			gtk_clist_get_text(GTK_CLIST(m_sizeList), rowNumber, 0, text);
			UT_ASSERT(text && text[0]);

			g_snprintf(sizeString, SIZE_STRING_SIZE, "%spt", 
			    XAP_EncodingManager::fontsizes_list.getFirst(text[0]));

			if (!m_pFontSize || strcmp((const char*)m_pFontSize, sizeString))
			{
				setFontSize((XML_Char*)sizeString);
				m_bChangedFontSize = true;
			}
		}
		
		gtk_color_selection_get_color(GTK_COLOR_SELECTION(m_colorSelector), currentColor);

		// test for funkyColor-has-been-changed-to-sane-color case
		if (currentColor[RED] >= 0 &&
			currentColor[GREEN] >= 0 &&
			currentColor[BLUE] >= 0)
		{
			char buf_color[6];
			sprintf(buf_color, "%02x%02x%02x",
					(unsigned int) (currentColor[RED] 	* (gdouble) 255.0),
					(unsigned int) (currentColor[GREEN]	* (gdouble) 255.0),
					(unsigned int) (currentColor[BLUE] 	* (gdouble) 255.0));
		
			if (!m_pColor || strcmp((const char*)m_pColor, buf_color))
			{
				setColor((XML_Char*)buf_color);
				m_bChangedColor = true;
			}
		}

		m_bChangedStrikeOut = (m_bStrikeOut != gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_checkStrikeOut)));
		m_bChangedUnderline = (m_bUnderline != gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_checkUnderline)));
		if (m_bChangedStrikeOut)
			m_bStrikeOut = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_checkStrikeOut));
		if (m_bChangedUnderline)
			m_bUnderline = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_checkUnderline));
	}

	gtk_widget_destroy (GTK_WIDGET(cf));

	// these dialogs are cached around through the dialog framework,
	// and this variable needs to get set back
	m_doneFirstFont = false;
	
    gtk_widget_pop_visual();
    gtk_widget_pop_colormap();
	
	UT_DEBUGMSG(("FontChooserEnd: Family[%s%s] Size[%s%s] Weight[%s%s] Style[%s%s] Color[%s%s] Underline[%d%s] StrikeOut[%d%s]\n",
				 ((m_pFontFamily) ? m_pFontFamily : ""),	((m_bChangedFontFamily) ? "(chg)" : ""),
				 ((m_pFontSize) ? m_pFontSize : ""),		((m_bChangedFontSize) ? "(chg)" : ""),
				 ((m_pFontWeight) ? m_pFontWeight : ""),	((m_bChangedFontWeight) ? "(chg)" : ""),
				 ((m_pFontStyle) ? m_pFontStyle : ""),		((m_bChangedFontStyle) ? "(chg)" : ""),
				 ((m_pColor) ? m_pColor : "" ),				((m_bChangedColor) ? "(chg)" : ""),
				 (m_bUnderline),							((m_bChangedUnderline) ? "(chg)" : ""),
				 (m_bStrikeOut),							((m_bChangedStrikeOut) ? "(chg)" : "")));

	// answer should be set by the appropriate callback
	// the caller can get the answer from getAnswer().

	m_pUnixFrame = NULL;
}
	
bool XAP_UnixDialog_FontChooser::getFont(XAP_UnixFont ** font)
{
	UT_ASSERT(font);
	
	gchar * fontText[2] = {NULL, NULL};
	XAP_UnixFont::style styleNumber;

	GList * selectedRow = NULL;
	gint rowNumber = 0;

	selectedRow = GTK_CLIST(m_fontList)->selection;
	if (selectedRow)
	{
		rowNumber = GPOINTER_TO_INT(selectedRow->data);
		gtk_clist_get_text(GTK_CLIST(m_fontList), rowNumber, 0, fontText);
		UT_ASSERT(fontText && fontText[0]);
	}
	else
	{
		return false;
	}
		
	selectedRow = GTK_CLIST(m_styleList)->selection;
	if (selectedRow)
	{
		gint style = GPOINTER_TO_INT(selectedRow->data);

		switch(style)
		{
		case LIST_STYLE_NORMAL:
			styleNumber = XAP_UnixFont::STYLE_NORMAL;
			break;
		case LIST_STYLE_BOLD:
			styleNumber = XAP_UnixFont::STYLE_BOLD;
			break;
		case LIST_STYLE_ITALIC:
			styleNumber = XAP_UnixFont::STYLE_ITALIC;
			break;
		case LIST_STYLE_BOLD_ITALIC:
			styleNumber = XAP_UnixFont::STYLE_BOLD_ITALIC;
			break;
		case LIST_STYLE_NONE: // if we're this, selectedRow should be 0
		default:
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			return false;
		}
	}
	else
	{
		return false;
	}
	
	const XAP_UnixFont * tempUnixFont = m_fontManager->getFont((const char *) fontText[0], styleNumber);

	if (tempUnixFont)
	{
		// we got a font, set the variables and return success
		*font = (XAP_UnixFont *) tempUnixFont;
		return true;
	}

	return false;
}

bool XAP_UnixDialog_FontChooser::getDecoration(bool * strikeout,
												 bool * underline)
{
	UT_ASSERT(strikeout && underline);

	*strikeout = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_checkStrikeOut));
	*underline = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_checkUnderline));

	return true;
}

bool XAP_UnixDialog_FontChooser::getSize(UT_uint32 * pointsize)
{
	UT_ASSERT(pointsize);

	GList * selectedRow = NULL;
	gchar * sizeText[2] = {NULL, NULL};
	gint rowNumber = 0;
	
	selectedRow = GTK_CLIST(m_sizeList)->selection;
	if (selectedRow)
	{
		rowNumber = GPOINTER_TO_INT(selectedRow->data);
		gtk_clist_get_text(GTK_CLIST(m_sizeList), rowNumber, 0, sizeText);
		UT_ASSERT(sizeText && sizeText[0]);

		*pointsize = (UT_uint32) atol(sizeText[0]);
		return true;
	}

	return false;
}

bool XAP_UnixDialog_FontChooser::getEntryString(char ** string)
{
	UT_ASSERT(string);

	// Maybe this will be editable in the future, if one wants to
	// hook up a mini formatter to the entry area.  Probably not.

	*string = PREVIEW_ENTRY_DEFAULT_STRING;

	return true;
}

bool XAP_UnixDialog_FontChooser::getForegroundColor(UT_RGBColor * color)
{
	UT_ASSERT(color);
	
	gdouble currentColor[3] = { 0, 0, 0 };

	enum
	{
		RED = 0,
		GREEN,
		BLUE
	};
	
	gtk_color_selection_get_color(GTK_COLOR_SELECTION(m_colorSelector), currentColor);

	color->m_red = (unsigned char) (currentColor[RED]   * (gdouble) 255);
	color->m_grn = (unsigned char) (currentColor[GREEN] * (gdouble) 255);
	color->m_blu = (unsigned char) (currentColor[BLUE]  * (gdouble) 255);

	return true;
}

bool XAP_UnixDialog_FontChooser::getBackgroundColor(UT_RGBColor * color)
{
	// this just returns white now, it should later query the document
	// in the frame which launched this dialog
	
	UT_ASSERT(color);
	
	color->m_red = 255;
	color->m_grn = 255;
	color->m_blu = 255;

	return true;
}


void XAP_UnixDialog_FontChooser::updatePreview(void)
{
	// if we don't have anything yet, just ignore this request
	if (!m_gc)
		return;
	
//	bool strikeout = false;
//	bool underline = false;

	XAP_UnixFont * font = NULL;
	if (!getFont(&font))
		return;

	UT_uint32 pointsize = 0;
	if (!getSize(&pointsize))
		return;

	UT_uint32 pixelsize = (UT_uint32) ((double) pointsize / (double) 72 * (double) m_gc->getResolution());

	UT_ASSERT(pixelsize > 0);
	
	// Do some trickery to convert point sizes (as listed in the list box)
	// to real pixel sizes for this GC.  The layout engine does this
	// automatically because it's just that good.
	XAP_UnixFontHandle * entry = new XAP_UnixFontHandle(font, pixelsize);

	if (entry)
	{
		// set the new font
		m_gc->setFont(entry);

		// if we've set a font, this variable is true
		m_doneFirstFont = true;

		// now do the switch
		DELETEP(m_lastFont);
		m_lastFont = entry;
	}
	else
	{
		// we didn't get the font we requested, which is really weird
		// since this dialog can only let the user pick fonts the
		// font manager says it KNOWS it has.
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}
		

	// do the foreground (text) color
	UT_RGBColor fgcolor;
	if (!getForegroundColor(&fgcolor))
		return;
	m_gc->setColor(fgcolor);

	s_drawing_area_expose(m_preview, NULL);
	
}

