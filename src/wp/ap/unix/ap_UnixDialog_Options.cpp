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

#include "ut_types.h"
#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

// This header defines some functions for Unix dialogs,
// like centering them, measuring them, etc.
#include "ut_dialogHelper.h"

#include "gr_UnixGraphics.h"

#include "xap_App.h"
#include "xap_UnixApp.h"
#include "xap_UnixFrame.h"
#include "xap_Prefs.h"

#include "ap_Dialog_Id.h"
#include "ap_Prefs_SchemeIds.h"

#include "ap_Strings.h"

#include "ap_UnixDialog_Options.h"

/*****************************************************************/

#define WIDGET_DIALOG_TAG "dialog"

/*****************************************************************/

XAP_Dialog * AP_UnixDialog_Options::static_constructor(XAP_DialogFactory * pFactory,
                                                         XAP_Dialog_Id id)
{
    AP_UnixDialog_Options * p = new AP_UnixDialog_Options(pFactory,id);
    return p;
}

AP_UnixDialog_Options::AP_UnixDialog_Options(XAP_DialogFactory * pDlgFactory,
                                                 XAP_Dialog_Id id)
    : AP_Dialog_Options(pDlgFactory,id)
{
#if 0
	/* DEBUG stuff */
	XAP_Prefs *prefs = m_pApp->getPrefs();
	UT_ASSERT(prefs);
	UT_DEBUGMSG(("AP_UnixDialog_Options::AP_UnixDialog_Options[%s:%d]", __FILE__, __LINE__));
	UT_DEBUGMSG(("    current pref : %s",
		prefs->getCurrentScheme()->getSchemeName()) );

	UT_Bool b = prefs->savePrefsFile();
	UT_DEBUGMSG(("    prefs saved (%d) in %s", b, prefs->getPrefsPathname() ));

	UT_uint32 i;
	XAP_PrefsScheme *ps;
	for ( i = 0; (ps = prefs->getNthScheme(i)) != 0; i++ ) {
		UT_DEBUGMSG(("    %d [%s]", i, ps->getSchemeName() ));
	
		XML_Char const *pszKey, *pszValue;
		for ( UT_uint32 j = 0; ps->getNthValue(j, &pszKey, &pszValue ); j++ ) {
			UT_DEBUGMSG(("        %x %-30s : %s", j, pszKey, pszValue ));
		}
	}
#endif
}

AP_UnixDialog_Options::~AP_UnixDialog_Options(void)
{
}

/*****************************************************************/

// sample callback function
static void s_ok_clicked(GtkWidget * widget, AP_UnixDialog_Options * dlg)
{ UT_ASSERT(widget && dlg); dlg->event_OK(); }

static void s_cancel_clicked(GtkWidget * widget, AP_UnixDialog_Options * dlg)
{ UT_ASSERT(widget && dlg); dlg->event_Cancel(); }

static void s_delete_clicked(GtkWidget * /* widget */,
                             gpointer /* data */,
                             AP_UnixDialog_Options * dlg)
{ UT_ASSERT(dlg); dlg->event_WindowDelete(); }

// these function will allow multiple widget to tie into the same logic
// function (at the AP level) to enable/disable stuff
void s_checkbutton_toggle( GtkWidget *w, AP_UnixDialog_Options *dlg )
{ 
	UT_ASSERT(dlg); 
	UT_ASSERT( w && GTK_IS_WIDGET(w));
	int i = (int) gtk_object_get_data( GTK_OBJECT(w), "tControl" );
	dlg->_enableDisableLogic( (AP_Dialog_Options::tControl) i );
}

void s_defaults_clicked( GtkWidget *widget, AP_UnixDialog_Options *dlg)
{ UT_ASSERT(widget && dlg); dlg->_event_SetDefaults(); }


/*****************************************************************/

void AP_UnixDialog_Options::runModal(XAP_Frame * pFrame)
{
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
    gtk_window_set_transient_for(GTK_WINDOW(mainWindow), GTK_WINDOW(parentWindow));

    // Show the top level dialog,
    gtk_widget_show(mainWindow);

    // Make it modal, and stick it up top
    gtk_grab_add(mainWindow);

    // Run into the GTK event loop for this window.
    gtk_main();

	switch ( m_answer )
	{
	case AP_Dialog_Options::a_OK:
		_storeWindowData();
		break;

	case AP_Dialog_Options::a_CANCEL:
		break;

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		break;
	};

    
    gtk_widget_destroy(mainWindow);
}

void AP_UnixDialog_Options::event_OK(void)
{
    m_answer = AP_Dialog_Options::a_OK;
    gtk_main_quit();
}

void AP_UnixDialog_Options::event_Cancel(void)
{
    m_answer = AP_Dialog_Options::a_CANCEL;
    gtk_main_quit();
}

void AP_UnixDialog_Options::event_WindowDelete(void)
{
    m_answer = AP_Dialog_Options::a_CANCEL;    
    gtk_main_quit();
}

/*****************************************************************/

GtkWidget* AP_UnixDialog_Options::_constructWindow ()
{
    //////////////////////////////////////////////////////////////////////
	// BEGIN: glade stuff (interface.c)

#define _(a) a
	GtkWidget *windowOptions;
	GtkWidget *table2;
	GtkWidget *hbuttonbox2;
	GtkWidget *buttonSave;
	GtkWidget *buttonDefaults;
	GtkWidget *buttonOk;
	GtkWidget *buttonCancel;
	GtkWidget *notebook1;
	GtkWidget *tableSpell;
	GtkWidget *checkbuttonSpellHideErrors;
	GtkWidget *checkbuttonSpellSuggest;
	GtkWidget *checkbuttonSpellMainOnly;
	GtkWidget *checkbuttonSpellUppercase;
	GtkWidget *checkbuttonSpellNumbers;
	GtkWidget *checkbuttonSpellInternet;
	GtkWidget *label4;
	GtkWidget *checkbuttonSpellCheckAsType;
	GtkWidget *buttonSpellIgnoreReset;
	GtkWidget *buttonSpellIgnoreEdit;
	GtkWidget *buttonSpellDictionary;
	GtkWidget *label5;
	GtkWidget *listSpellDicts;
	GtkWidget *listSpellDicts_menu;
	GtkWidget *glade_menuitem;
	GtkWidget *labelSpell;
	GtkWidget *tablePreferences;
	GtkWidget *checkbuttonPrefsAutoSave;
	GtkWidget *label6;
	GtkWidget *comboPrefsSchemes;
	GtkWidget *comboPrefsSchemesEdit;
	GtkWidget *labelPreferences;
	GtkWidget *hboxView;
	GtkWidget *vbox4;
	GtkWidget *frame2;
	GtkWidget *vbox7;
	GtkWidget *hbox10;
	GtkWidget *checkbuttonViewRuler;
	GtkWidget *labelUnits;
	GtkWidget *listViewRulerUnit;
	GtkWidget *listViewRulerUnit_menu;
	GtkWidget *checkbuttonViewToolbars;
	GtkWidget *frameViewStuff;
	GtkWidget *vbox6;
	GtkWidget *checkbuttonViewAll;
	GtkWidget *checkbuttonViewHidden;
	GtkWidget *checkbuttonViewUnprintable;
	GtkWidget *labelView;

	windowOptions = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_object_set_data (GTK_OBJECT (windowOptions), "windowOptions", windowOptions);
	gtk_window_set_title (GTK_WINDOW (windowOptions), _("Options"));

	table2 = gtk_table_new (2, 1, FALSE);
	gtk_widget_ref (table2);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "table2", table2,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (table2);
	gtk_container_add (GTK_CONTAINER (windowOptions), table2);

	hbuttonbox2 = gtk_hbutton_box_new ();
	gtk_widget_ref (hbuttonbox2);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "hbuttonbox2", hbuttonbox2,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (hbuttonbox2);
	gtk_table_attach (GTK_TABLE (table2), hbuttonbox2, 0, 1, 1, 2,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (GTK_FILL), 0, 0);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox2), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox2), 10);

	buttonSave = gtk_button_new_with_label (_("Sa&ve"));
	gtk_widget_ref (buttonSave);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "buttonSave", buttonSave,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (buttonSave);
	gtk_container_add (GTK_CONTAINER (hbuttonbox2), buttonSave);
	GTK_WIDGET_SET_FLAGS (buttonSave, GTK_CAN_DEFAULT);

	buttonDefaults = gtk_button_new_with_label (_("De&faults"));
	gtk_widget_ref (buttonDefaults);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "buttonDefaults", buttonDefaults,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (buttonDefaults);
	gtk_container_add (GTK_CONTAINER (hbuttonbox2), buttonDefaults);
	GTK_WIDGET_SET_FLAGS (buttonDefaults, GTK_CAN_DEFAULT);

	buttonOk = gtk_button_new_with_label (_("OK"));
	gtk_widget_ref (buttonOk);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "buttonOk", buttonOk,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (buttonOk);
	gtk_container_add (GTK_CONTAINER (hbuttonbox2), buttonOk);
	GTK_WIDGET_SET_FLAGS (buttonOk, GTK_CAN_DEFAULT);

	buttonCancel = gtk_button_new_with_label (_("Cancel"));
	gtk_widget_ref (buttonCancel);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "buttonCancel", buttonCancel,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (buttonCancel);
	gtk_container_add (GTK_CONTAINER (hbuttonbox2), buttonCancel);
	GTK_WIDGET_SET_FLAGS (buttonCancel, GTK_CAN_DEFAULT);

	notebook1 = gtk_notebook_new ();
	gtk_widget_ref (notebook1);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "notebook1", notebook1,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (notebook1);
	gtk_table_attach (GTK_TABLE (table2), notebook1, 0, 1, 0, 1,
	                  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	                  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 10, 7);

	tableSpell = gtk_table_new (9, 3, FALSE);
	gtk_widget_ref (tableSpell);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "tableSpell", tableSpell,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (tableSpell);
	gtk_container_add (GTK_CONTAINER (notebook1), tableSpell);
	gtk_container_set_border_width (GTK_CONTAINER (tableSpell), 10);

	checkbuttonSpellHideErrors = gtk_check_button_new_with_label (_("Hide &spelling errors in the document"));
	gtk_widget_ref (checkbuttonSpellHideErrors);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonSpellHideErrors", checkbuttonSpellHideErrors,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonSpellHideErrors);
	gtk_table_attach (GTK_TABLE (tableSpell), checkbuttonSpellHideErrors, 0, 3, 1, 2,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);

	checkbuttonSpellSuggest = gtk_check_button_new_with_label (_("A&lways suggest corrections"));
	gtk_widget_ref (checkbuttonSpellSuggest);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonSpellSuggest", checkbuttonSpellSuggest,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonSpellSuggest);
	gtk_table_attach (GTK_TABLE (tableSpell), checkbuttonSpellSuggest, 0, 3, 2, 3,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);

	checkbuttonSpellMainOnly = gtk_check_button_new_with_label (_("Suggest from &main dictionary only"));
	gtk_widget_ref (checkbuttonSpellMainOnly);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonSpellMainOnly", checkbuttonSpellMainOnly,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonSpellMainOnly);
	gtk_table_attach (GTK_TABLE (tableSpell), checkbuttonSpellMainOnly, 0, 3, 3, 4,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);

	checkbuttonSpellUppercase = gtk_check_button_new_with_label (_("Ignore words in &UPPERCASE"));
	gtk_widget_ref (checkbuttonSpellUppercase);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonSpellUppercase", checkbuttonSpellUppercase,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonSpellUppercase);
	gtk_table_attach (GTK_TABLE (tableSpell), checkbuttonSpellUppercase, 0, 3, 4, 5,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);

	checkbuttonSpellNumbers = gtk_check_button_new_with_label (_("Ignore words with num&bers"));
	gtk_widget_ref (checkbuttonSpellNumbers);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonSpellNumbers", checkbuttonSpellNumbers,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonSpellNumbers);
	gtk_table_attach (GTK_TABLE (tableSpell), checkbuttonSpellNumbers, 0, 3, 5, 6,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);

	checkbuttonSpellInternet = gtk_check_button_new_with_label (_("Ignore Internet and &file addresses"));
	gtk_widget_ref (checkbuttonSpellInternet);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonSpellInternet", checkbuttonSpellInternet,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonSpellInternet);
	gtk_table_attach (GTK_TABLE (tableSpell), checkbuttonSpellInternet, 0, 3, 6, 7,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);

	label4 = gtk_label_new (_("Ignored words: "));
	gtk_widget_ref (label4);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "label4", label4,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (label4);
	gtk_table_attach (GTK_TABLE (tableSpell), label4, 0, 1, 8, 9,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_LEFT);

	checkbuttonSpellCheckAsType = gtk_check_button_new_with_label (_("Check s&pelling as you type"));
	gtk_widget_ref (checkbuttonSpellCheckAsType);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonSpellCheckAsType", checkbuttonSpellCheckAsType,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonSpellCheckAsType);
	gtk_table_attach (GTK_TABLE (tableSpell), checkbuttonSpellCheckAsType, 0, 3, 0, 1,
	                  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);

	buttonSpellIgnoreReset = gtk_button_new_with_label (_("&Reset"));
	gtk_widget_ref (buttonSpellIgnoreReset);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "buttonSpellIgnoreReset", buttonSpellIgnoreReset,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (buttonSpellIgnoreReset);
	gtk_table_attach (GTK_TABLE (tableSpell), buttonSpellIgnoreReset, 1, 2, 8, 9,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 3);

	buttonSpellIgnoreEdit = gtk_button_new_with_label (_("&Edit..."));
	gtk_widget_ref (buttonSpellIgnoreEdit);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "buttonSpellIgnoreEdit", buttonSpellIgnoreEdit,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (buttonSpellIgnoreEdit);
	gtk_table_attach (GTK_TABLE (tableSpell), buttonSpellIgnoreEdit, 2, 3, 8, 9,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 4, 0);

	buttonSpellDictionary = gtk_button_new_with_label (_("&Dictionary..."));
	gtk_widget_ref (buttonSpellDictionary);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "buttonSpellDictionary", buttonSpellDictionary,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (buttonSpellDictionary);
	gtk_table_attach (GTK_TABLE (tableSpell), buttonSpellDictionary, 2, 3, 7, 8,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 4, 0);

	label5 = gtk_label_new (_("Custom Dictionary:"));
	gtk_widget_ref (label5);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "label5", label5,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (label5);
	gtk_table_attach (GTK_TABLE (tableSpell), label5, 0, 1, 7, 8,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_LEFT);

	listSpellDicts = gtk_option_menu_new ();
	gtk_widget_ref (listSpellDicts);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "listSpellDicts", listSpellDicts,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (listSpellDicts);
	gtk_table_attach (GTK_TABLE (tableSpell), listSpellDicts, 1, 2, 7, 8,
	                  (GtkAttachOptions) (GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);
	listSpellDicts_menu = gtk_menu_new ();
	glade_menuitem = gtk_menu_item_new_with_label (_("custom.dic"));
	gtk_widget_show (glade_menuitem);
	gtk_menu_append (GTK_MENU (listSpellDicts_menu), glade_menuitem);
	gtk_option_menu_set_menu (GTK_OPTION_MENU (listSpellDicts), listSpellDicts_menu);

	labelSpell = gtk_label_new (_("Spelling"));
	gtk_widget_ref (labelSpell);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "labelSpell", labelSpell,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (labelSpell);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), labelSpell);

	tablePreferences = gtk_table_new (2, 3, FALSE);
	gtk_widget_ref (tablePreferences);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "tablePreferences", tablePreferences,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (tablePreferences);
	gtk_container_add (GTK_CONTAINER (notebook1), tablePreferences);
	gtk_container_set_border_width (GTK_CONTAINER (tablePreferences), 10);

	checkbuttonPrefsAutoSave = gtk_check_button_new_with_label (_("&Automatically save preferences"));
	gtk_widget_ref (checkbuttonPrefsAutoSave);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonPrefsAutoSave", checkbuttonPrefsAutoSave,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonPrefsAutoSave);
	gtk_table_attach (GTK_TABLE (tablePreferences), checkbuttonPrefsAutoSave, 0, 3, 0, 1,
	                  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);

	label6 = gtk_label_new (_("Current Preferences Scheme:"));
	gtk_widget_ref (label6);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "label6", label6,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (label6);
	gtk_table_attach (GTK_TABLE (tablePreferences), label6, 0, 1, 1, 2,
	                  (GtkAttachOptions) (0),
	                  (GtkAttachOptions) (0), 0, 0);

	comboPrefsSchemes = gtk_combo_new ();
	gtk_widget_ref (comboPrefsSchemes);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "comboPrefsSchemes", comboPrefsSchemes,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (comboPrefsSchemes);
	gtk_table_attach (GTK_TABLE (tablePreferences), comboPrefsSchemes, 2, 3, 1, 2,
	                  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	                  (GtkAttachOptions) (0), 0, 0);

	comboPrefsSchemesEdit = GTK_COMBO (comboPrefsSchemes)->entry;
	gtk_widget_ref (comboPrefsSchemesEdit);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "comboPrefsSchemesEdit", comboPrefsSchemesEdit,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (comboPrefsSchemesEdit);

	labelPreferences = gtk_label_new (_("Preferences"));
	gtk_widget_ref (labelPreferences);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "labelPreferences", labelPreferences,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (labelPreferences);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), labelPreferences);

	hboxView = gtk_hbox_new (FALSE, 0);
	gtk_widget_ref (hboxView);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "hboxView", hboxView,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (hboxView);
	gtk_container_add (GTK_CONTAINER (notebook1), hboxView);
	gtk_container_set_border_width (GTK_CONTAINER (hboxView), 10);

	vbox4 = gtk_vbox_new (FALSE, 10);
	gtk_widget_ref (vbox4);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "vbox4", vbox4,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (vbox4);
	gtk_box_pack_start (GTK_BOX (hboxView), vbox4, TRUE, TRUE, 5);

	frame2 = gtk_frame_new (_("Show/Hide..."));
	gtk_widget_ref (frame2);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "frame2", frame2,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (frame2);
	gtk_box_pack_start (GTK_BOX (vbox4), frame2, FALSE, FALSE, 0);

	vbox7 = gtk_vbox_new (FALSE, 0);
	gtk_widget_ref (vbox7);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "vbox7", vbox7,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (vbox7);
	gtk_container_add (GTK_CONTAINER (frame2), vbox7);

	hbox10 = gtk_hbox_new (FALSE, 0);
	gtk_widget_ref (hbox10);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "hbox10", hbox10,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (hbox10);
	gtk_box_pack_start (GTK_BOX (vbox7), hbox10, FALSE, FALSE, 0);

	checkbuttonViewRuler = gtk_check_button_new_with_label (_("&Ruler"));
	gtk_widget_ref (checkbuttonViewRuler);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonViewRuler", checkbuttonViewRuler,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonViewRuler);
	gtk_box_pack_start (GTK_BOX (hbox10), checkbuttonViewRuler, FALSE, FALSE, 0);

	labelUnits = gtk_label_new (_("&units:"));
	gtk_widget_ref (labelUnits);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "labelUnits", labelUnits,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (labelUnits);
	gtk_box_pack_start (GTK_BOX (hbox10), labelUnits, TRUE, TRUE, 0);
	gtk_label_set_justify (GTK_LABEL (labelUnits), GTK_JUSTIFY_RIGHT);

	listViewRulerUnit = gtk_option_menu_new ();
	gtk_widget_ref (listViewRulerUnit);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "listViewRulerUnit", listViewRulerUnit,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (listViewRulerUnit);
	gtk_box_pack_start (GTK_BOX (hbox10), listViewRulerUnit, FALSE, FALSE, 0);
	listViewRulerUnit_menu = gtk_menu_new ();
	glade_menuitem = gtk_menu_item_new_with_label (_("inch"));
	gtk_widget_show (glade_menuitem);
	gtk_menu_append (GTK_MENU (listViewRulerUnit_menu), glade_menuitem);
	glade_menuitem = gtk_menu_item_new_with_label (_("mm"));
	gtk_widget_show (glade_menuitem);
	gtk_menu_append (GTK_MENU (listViewRulerUnit_menu), glade_menuitem);
	glade_menuitem = gtk_menu_item_new_with_label (_("cm"));
	gtk_widget_show (glade_menuitem);
	gtk_menu_append (GTK_MENU (listViewRulerUnit_menu), glade_menuitem);
	glade_menuitem = gtk_menu_item_new_with_label (_("twips"));
	gtk_widget_show (glade_menuitem);
	gtk_menu_append (GTK_MENU (listViewRulerUnit_menu), glade_menuitem);
	glade_menuitem = gtk_menu_item_new_with_label (_("points"));
	gtk_widget_show (glade_menuitem);
	gtk_menu_append (GTK_MENU (listViewRulerUnit_menu), glade_menuitem);
	glade_menuitem = gtk_menu_item_new_with_label (_("pixels"));
	gtk_widget_show (glade_menuitem);
	gtk_menu_append (GTK_MENU (listViewRulerUnit_menu), glade_menuitem);
	gtk_option_menu_set_menu (GTK_OPTION_MENU (listViewRulerUnit), listViewRulerUnit_menu);

	checkbuttonViewToolbars = gtk_check_button_new_with_label (_("&Toolbars"));
	gtk_widget_ref (checkbuttonViewToolbars);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonViewToolbars", checkbuttonViewToolbars,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonViewToolbars);
	gtk_box_pack_start (GTK_BOX (vbox7), checkbuttonViewToolbars, FALSE, FALSE, 0);

	frameViewStuff = gtk_frame_new (_("View..."));
	gtk_widget_ref (frameViewStuff);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "frameViewStuff", frameViewStuff,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (frameViewStuff);
	gtk_box_pack_start (GTK_BOX (vbox4), frameViewStuff, FALSE, FALSE, 0);

	vbox6 = gtk_vbox_new (FALSE, 0);
	gtk_widget_ref (vbox6);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "vbox6", vbox6,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (vbox6);
	gtk_container_add (GTK_CONTAINER (frameViewStuff), vbox6);

	checkbuttonViewAll = gtk_check_button_new_with_label (_("A&ll"));
	gtk_widget_ref (checkbuttonViewAll);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonViewAll", checkbuttonViewAll,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonViewAll);
	gtk_box_pack_start (GTK_BOX (vbox6), checkbuttonViewAll, FALSE, FALSE, 0);

	checkbuttonViewHidden = gtk_check_button_new_with_label (_("&Hidden Text"));
	gtk_widget_ref (checkbuttonViewHidden);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonViewHidden", checkbuttonViewHidden,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonViewHidden);
	gtk_box_pack_start (GTK_BOX (vbox6), checkbuttonViewHidden, FALSE, FALSE, 0);

	checkbuttonViewUnprintable = gtk_check_button_new_with_label (_("Un&printable characters"));
	gtk_widget_ref (checkbuttonViewUnprintable);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "checkbuttonViewUnprintable", checkbuttonViewUnprintable,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (checkbuttonViewUnprintable);
	gtk_box_pack_start (GTK_BOX (vbox6), checkbuttonViewUnprintable, FALSE, FALSE, 0);

	labelView = gtk_label_new (_("View"));
	gtk_widget_ref (labelView);
	gtk_object_set_data_full (GTK_OBJECT (windowOptions), "labelView", labelView,
	                          (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show (labelView);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), labelView);
/*
	gtk_signal_connect (GTK_OBJECT (windowOptions), "destroy",
	                    GTK_SIGNAL_FUNC (on_windowOptions_destroy),
	                    NULL);
	gtk_signal_connect (GTK_OBJECT (buttonSave), "clicked",
	                    GTK_SIGNAL_FUNC (on_buttonSave_clicked),
	                    (gpointer *) this);
	gtk_signal_connect (GTK_OBJECT (buttonDefaults), "clicked",
	                    GTK_SIGNAL_FUNC (on_buttonDefaults_clicked),
	                    (gpointer *)this);
	gtk_signal_connect (GTK_OBJECT (buttonOk), "clicked",
	                    GTK_SIGNAL_FUNC (on_buttonOk_clicked),
	                    (gpointer *)this);
	gtk_signal_connect (GTK_OBJECT (buttonCancel), "clicked",
	                    GTK_SIGNAL_FUNC (on_buttonCancel_clicked),
	                    (gpointer *) this);
	gtk_signal_connect (GTK_OBJECT (checkbuttonSpellCheckAsType), "toggled",
	                    GTK_SIGNAL_FUNC (on_checkbutton_toggled),
	                    (gpointer *) this);

	return windowOptions;
*/

    //////////////////////////////////////////////////////////////////////
	// END: glade stuff


    //////////////////////////////////////////////////////////////////////
    // the control buttons
    gtk_signal_connect(GTK_OBJECT(buttonOk),
                       "clicked",
                       GTK_SIGNAL_FUNC(s_ok_clicked),
                       (gpointer) this);
    
    gtk_signal_connect(GTK_OBJECT(buttonCancel),
                       "clicked",
                       GTK_SIGNAL_FUNC(s_cancel_clicked),
                       (gpointer) this);


    // the catch-alls
    gtk_signal_connect_after(GTK_OBJECT(windowOptions),
                             "delete_event",
                             GTK_SIGNAL_FUNC(s_delete_clicked),
                             (gpointer) this);


    //////////////////////////////////////////////////////////////////////
    // the control buttons
    gtk_signal_connect(GTK_OBJECT(buttonOk),
                       "clicked",
                       GTK_SIGNAL_FUNC(s_ok_clicked),
                       (gpointer) this);
    
    gtk_signal_connect(GTK_OBJECT(buttonCancel),
                       "clicked",
                       GTK_SIGNAL_FUNC(s_cancel_clicked),
                       (gpointer) this);

    gtk_signal_connect(GTK_OBJECT(buttonDefaults),
                       "clicked",
                       GTK_SIGNAL_FUNC(s_defaults_clicked),
                       (gpointer) this);

	gtk_signal_connect(GTK_OBJECT(checkbuttonSpellCheckAsType),
						"toggled",
                       GTK_SIGNAL_FUNC(s_checkbutton_toggle),
                       (gpointer) this);

    // the catch-alls
    gtk_signal_connect_after(GTK_OBJECT(windowOptions),
                             "delete_event",
                             GTK_SIGNAL_FUNC(s_delete_clicked),
                             (gpointer) this);

    gtk_signal_connect_after(GTK_OBJECT(windowOptions),
                             "destroy",
                             NULL,
                             NULL);

    // Update member variables with the important widgets that
    // might need to be queried or altered later.

    m_windowMain = windowOptions;

    m_checkbuttonSpellCheckAsType	= checkbuttonSpellCheckAsType;
    m_checkbuttonSpellHideErrors	= checkbuttonSpellHideErrors;
    m_checkbuttonSpellSuggest		= checkbuttonSpellSuggest;
    m_checkbuttonSpellMainOnly		= checkbuttonSpellMainOnly;
    m_checkbuttonSpellUppercase		= checkbuttonSpellUppercase;
    m_checkbuttonSpellNumbers		= checkbuttonSpellNumbers;
    m_checkbuttonSpellInternet		= checkbuttonSpellInternet;
	m_listSpellDicts				= listSpellDicts;
	m_listSpellDicts_menu			= listSpellDicts_menu;
	m_buttonSpellDictionary			= buttonSpellDictionary;

    m_checkbuttonPrefsAutoSave		= checkbuttonPrefsAutoSave;
	m_comboPrefsScheme				= comboPrefsSchemes;

    m_checkbuttonViewShowRuler		= checkbuttonViewRuler;
    m_listViewRulerUnits			= listViewRulerUnit;
    m_checkbuttonViewShowToolbars	= checkbuttonViewToolbars;
    m_checkbuttonViewAll			= checkbuttonViewAll;
    m_checkbuttonViewHiddenText		= checkbuttonViewHidden;
    m_checkbuttonViewUnprintable	= checkbuttonViewUnprintable;

    m_buttonSave					= buttonSave;
    m_buttonDefaults				= buttonDefaults;
    m_buttonOK						= buttonOk;
    m_buttonCancel					= buttonCancel;

	// create the accelerators from &'s
	createLabelAccelerators(windowOptions);

	// create user data tControl -> stored in widgets 
	for ( int i = 0; i < id_last; i++ )
	{
		GtkWidget *w = _lookupWidget( (tControl)i );
		UT_ASSERT( w && GTK_IS_WIDGET(w) );
		gtk_object_set_data( GTK_OBJECT(w), "tControl", (gpointer) i );
	}

    return windowOptions;
}

GtkWidget *AP_UnixDialog_Options::_lookupWidget ( tControl id )
{
	switch (id)
	{
	case id_CHECK_SPELL_CHECK_AS_TYPE:
		return m_checkbuttonSpellCheckAsType;
		break;

	case id_CHECK_SPELL_HIDE_ERRORS:
		return m_checkbuttonSpellHideErrors;
		break;

	case id_CHECK_SPELL_SUGGEST:
		return m_checkbuttonSpellSuggest;
		break;

	case id_CHECK_SPELL_MAIN_ONLY:
		return m_checkbuttonSpellMainOnly;
		break;

	case id_CHECK_SPELL_UPPERCASE:
		return m_checkbuttonSpellUppercase;
		break;

	case id_CHECK_SPELL_NUMBERS:
		return m_checkbuttonSpellNumbers;
		break;

	case id_CHECK_SPELL_INTERNET:
		return m_checkbuttonSpellInternet;
		break;

	case id_LIST_DICTIONARY:
		return m_listSpellDicts;
		break;

	case id_CHECK_PREFS_AUTO_SAVE:
		return m_checkbuttonPrefsAutoSave;
		break;

	case id_COMBO_PREFS_SCHEME:
		return m_comboPrefsScheme;
		break;

	case id_CHECK_VIEW_SHOW_RULER:
		return m_checkbuttonViewShowRuler;
		break;

	case id_LIST_VIEW_RULER_UNITS:
		return m_listViewRulerUnits;
		break;

	case id_CHECK_VIEW_SHOW_TOOLBARS:
		return m_checkbuttonViewShowToolbars;
		break;

	case id_CHECK_VIEW_ALL:
		return m_checkbuttonViewAll;
		break;

	case id_CHECK_VIEW_HIDDEN_TEXT:
		return m_checkbuttonViewHiddenText;
		break;

	case id_CHECK_VIEW_UNPRINTABLE:
		return m_checkbuttonViewUnprintable;
		break;

	case id_BUTTON_SAVE:
		return m_buttonSave;
		break;

	case id_BUTTON_DEFAULTS:
		return m_buttonDefaults;
		break;

	case id_BUTTON_OK:
		return m_buttonOK;
		break;

	case id_BUTTON_CANCEL:
		return m_buttonCancel;
		break;

	default:
		UT_ASSERT("Unknown Widget");
		return 0;
		break;
	}
}

void AP_UnixDialog_Options::_controlEnable( tControl id, UT_Bool value )
{
	GtkWidget *w = _lookupWidget(id);
	UT_ASSERT( w && GTK_IS_WIDGET(w) );
	gtk_widget_set_sensitive( w, value );
}


#define DEFINE_GET_SET_BOOL(button) \
UT_Bool     AP_UnixDialog_Options::_gather##button(void) {				\
	UT_ASSERT(m_checkbutton##button && GTK_IS_BUTTON(m_checkbutton##button)); \
	return gtk_toggle_button_get_active(								\
				GTK_TOGGLE_BUTTON(m_checkbutton##button) ); }			\
void        AP_UnixDialog_Options::_set##button(UT_Bool b) {	\
	UT_ASSERT(m_checkbutton##button && GTK_IS_BUTTON(m_checkbutton##button)); \
	gtk_toggle_button_set_active (										\
				GTK_TOGGLE_BUTTON(m_checkbutton##button), b ); }

DEFINE_GET_SET_BOOL(SpellCheckAsType);
DEFINE_GET_SET_BOOL(SpellHideErrors);
DEFINE_GET_SET_BOOL(SpellSuggest);
DEFINE_GET_SET_BOOL(SpellMainOnly);
DEFINE_GET_SET_BOOL(SpellUppercase);
DEFINE_GET_SET_BOOL(SpellNumbers);
DEFINE_GET_SET_BOOL(SpellInternet);

DEFINE_GET_SET_BOOL(PrefsAutoSave);

DEFINE_GET_SET_BOOL	(ViewShowRuler);
	// have a units option
DEFINE_GET_SET_BOOL	(ViewShowToolbars);

DEFINE_GET_SET_BOOL	(ViewAll);
DEFINE_GET_SET_BOOL	(ViewHiddenText);
DEFINE_GET_SET_BOOL	(ViewUnprintable);

#undef DEFINE_GET_SET_BOOL

