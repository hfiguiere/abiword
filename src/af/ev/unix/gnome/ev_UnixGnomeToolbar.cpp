/* AbiSource Program Utilities
 * Copyright (C) 1998-2000 AbiSource, Inc.
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
#ifdef HAVE_BONOBO
//#include <libgnorba/gnorba.h>
#include <bonobo.h>
#endif

#include <string.h>
#include <stdlib.h>
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ev_UnixToolbar.h"
#include "xap_Types.h"
#include "xap_UnixGnomeApp.h"
#include "xap_UnixGnomeFrame.h"
#include "ev_Toolbar_Actions.h"
#include "ev_Toolbar_Layouts.h"
#include "ev_Toolbar_Labels.h"
#include "ev_Toolbar_Control.h"
#include "ev_EditEventMapper.h"
#include "xap_UnixToolbar_Icons.h"
#include "ev_UnixToolbar_ViewListener.h"
#include "xav_View.h"
#include "xmlparse.h"
#include "xap_Prefs.h"
#include "ev_UnixGnomeToolbar.h"

int EV_UnixGnomeToolbar::nbBands = 0;

/*****************************************************************/
#define COMBO_BUF_LEN 256

class _wd								// a private little class to help
{										// us remember all the widgets that
public:									// we create...
	_wd(EV_UnixGnomeToolbar * pUnixToolbar, XAP_Toolbar_Id id, GtkWidget * widget = NULL)
	{
		m_pUnixToolbar = pUnixToolbar;
		m_id = id;
		m_widget = widget;
		m_blockSignal = false;
		m_comboEntryBuffer[0] = 0;
	};
	
	~_wd(void)
	{
	};

	static void s_callback(GtkWidget * /* widget */, gpointer user_data)
	{
		// this is a static callback method and does not have a 'this' pointer.
		// map the user_data into an object and dispatch the event.
	
		_wd * wd = (_wd *) user_data;
		UT_ASSERT(wd);

		if (!wd->m_blockSignal)
			wd->m_pUnixToolbar->toolbarEvent(wd, 0, 0);
	};

	// TODO: should this move out of wd?  It's convenient here; maybe I'll make
	// a microclass for combo boxes.
	static void s_combo_changed(GtkWidget * widget, gpointer user_data)
	{
		_wd * wd = (_wd *) user_data;
		UT_ASSERT(wd);

		// only act if the widget has been shown and embedded in the toolbar
		if (wd->m_widget)
		{
			// if the popwin is still shown, this is a copy run and widget has a ->parent
			if (widget->parent)
			{
				// block is only honored here
				if (!wd->m_blockSignal)
				{
					gchar * buffer = gtk_entry_get_text(GTK_ENTRY(widget));
					UT_uint32 length = strlen(buffer);
					UT_ASSERT(length > 0);
					UT_ASSERT(length < 1024);
					strcpy(wd->m_comboEntryBuffer, buffer);
				}
			}
			else // widget has no ->parent, so use the buffer's results
			{
				UT_uint32 size = strlen(wd->m_comboEntryBuffer);

				// TODO : do a real conversion to UT_UCSChar or figure out the casting

				// don't do changes for empty combo texts
				if (UT_stricmp(wd->m_comboEntryBuffer, ""))
				{
					UT_UCSChar * text = (UT_UCSChar *) wd->m_comboEntryBuffer;
					UT_ASSERT(text);

					wd->m_pUnixToolbar->toolbarEvent(wd, text, size);
				}
			}
		}
			
	};

	// unblock when the menu goes away
	static void s_combo_hide(GtkWidget * widget, gpointer user_data)
	{
		_wd * wd = (_wd *) user_data;
		UT_ASSERT(wd);

		// manually force an update
		s_combo_changed(widget, user_data);
	}

	EV_UnixGnomeToolbar *	m_pUnixToolbar;
	XAP_Toolbar_Id		m_id;
	GtkWidget *			m_widget;
	bool				m_blockSignal;

	char 				m_comboEntryBuffer[1024];
};

/*****************************************************************/

EV_UnixGnomeToolbar::EV_UnixGnomeToolbar(XAP_UnixGnomeApp * pUnixApp, XAP_UnixGnomeFrame * pUnixFrame,
										 const char * szToolbarLayoutName,
										 const char * szToolbarLabelSetName)
	: EV_UnixToolbar(static_cast<XAP_UnixApp *> (pUnixApp), static_cast<XAP_UnixFrame *> (pUnixFrame),
					 szToolbarLayoutName,
					 szToolbarLabelSetName),
	  nbToolbarsInBand(0)
{
}

EV_UnixGnomeToolbar::~EV_UnixGnomeToolbar(void)
{
	UT_VECTOR_PURGEALL(GtkWidget *, m_vecToolbars);
}

UT_Bool EV_UnixGnomeToolbar::synthesize(void)
{
	// create a GTK toolbar from the info provided.
	const EV_Toolbar_ActionSet * pToolbarActionSet = m_pUnixApp->getToolbarActionSet();
	UT_ASSERT(pToolbarActionSet);

	XAP_Toolbar_ControlFactory * pFactory = m_pUnixApp->getControlFactory();
	UT_ASSERT(pFactory);
	
	UT_uint32 nrLabelItemsInLayout = m_pToolbarLayout->getLayoutItemCount();
	UT_ASSERT(nrLabelItemsInLayout > 0);

	GtkWidget * wTLW = m_pUnixFrame->getTopLevelWindow();

	m_wToolbar = _makeToolbar ();

	for (UT_uint32 k=0; (k < nrLabelItemsInLayout); k++)
	{
		EV_Toolbar_LayoutItem * pLayoutItem = m_pToolbarLayout->getLayoutItem(k);
		UT_ASSERT(pLayoutItem);

		XAP_Toolbar_Id id = pLayoutItem->getToolbarId();
		EV_Toolbar_Action * pAction = pToolbarActionSet->getAction(id);
		UT_ASSERT(pAction);
		EV_Toolbar_Label * pLabel = m_pToolbarLabelSet->getLabel(id);
		UT_ASSERT(pLabel);

		switch (pLayoutItem->getToolbarLayoutFlags())
		{
		case EV_TLF_Normal:
		{
			_wd * wd = new _wd(this,id);
			UT_ASSERT(wd);

			const char * szToolTip = pLabel->getToolTip();
			if (!szToolTip || !*szToolTip)
				szToolTip = pLabel->getStatusMsg();

			switch (pAction->getItemType())
			{
			case EV_TBIT_PushButton:
				{
					UT_ASSERT(UT_stricmp(pLabel->getIconName(),"NoIcon")!=0);
					GtkWidget * wPixmap;
					UT_Bool bFoundIcon =
						m_pUnixToolbarIcons->getPixmapForIcon(wTLW->window,
															  &wTLW->style->bg[GTK_STATE_NORMAL],
															  pLabel->getIconName(),
															  &wPixmap);
					UT_ASSERT(bFoundIcon);

					wd->m_widget = gtk_toolbar_append_item(GTK_TOOLBAR(m_wToolbar),
														   pLabel->getToolbarLabel(),
														   szToolTip,(const char *)NULL,
														   wPixmap,
														   GTK_SIGNAL_FUNC(_wd::s_callback),
														   wd);
				}
				break;

			case EV_TBIT_ToggleButton:
			case EV_TBIT_GroupButton:
				{
					UT_ASSERT(UT_stricmp(pLabel->getIconName(),"NoIcon")!=0);
					GtkWidget * wPixmap;
					UT_Bool bFoundIcon =
						m_pUnixToolbarIcons->getPixmapForIcon(wTLW->window,
															  &wTLW->style->bg[GTK_STATE_NORMAL],
															  pLabel->getIconName(),
															  &wPixmap);
					UT_ASSERT(bFoundIcon);

					wd->m_widget = gtk_toolbar_append_element(GTK_TOOLBAR(m_wToolbar),
															  GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
															  (GtkWidget *)NULL,
															  pLabel->getToolbarLabel(),
															  szToolTip,(const char *)NULL,
															  wPixmap,
															  GTK_SIGNAL_FUNC(_wd::s_callback),
															  wd);
				}
				break;

			case EV_TBIT_EditText:
				break;
					
			case EV_TBIT_DropDown:
				break;
					
			case EV_TBIT_ComboBox:
			{
				EV_Toolbar_Control * pControl = pFactory->getControl(this, id);
				UT_ASSERT(pControl);

				// default, shouldn't be used for well-defined controls
				int iWidth = 100;

				if (pControl)
				{
					iWidth = pControl->getPixelWidth();
				}

				GtkWidget * comboBox = gtk_combo_new();
				UT_ASSERT(comboBox);

				// Combo boxes flash on 8-bit displays unless you set its colormap
				// to agree with what we're using elsewhere (gdk_rgb's version)
				gtk_widget_set_colormap(comboBox, gdk_rgb_get_cmap());
				
				// set the size of the entry to set the total combo size
				gtk_widget_set_usize(GTK_COMBO(comboBox)->entry, iWidth, 0);

				// the entry is read-only for now
				gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(comboBox)->entry), FALSE);
										 
				// handle popup events, so we can block our signals until the popdown
				GtkWidget * button = GTK_WIDGET(GTK_COMBO(comboBox)->button);
				UT_ASSERT(button);
				GtkWidget * popwin = GTK_WIDGET(GTK_COMBO(comboBox)->popwin);
				UT_ASSERT(popwin);
// we don't use this
#if 0
				gtk_signal_connect(GTK_OBJECT(popwin),
								   "show",
								   GTK_SIGNAL_FUNC(_wd::s_combo_show),
								   wd);
#endif
				gtk_signal_connect(GTK_OBJECT(popwin),
								   "hide",
								   GTK_SIGNAL_FUNC(_wd::s_combo_hide),
								   wd);

				// handle changes in content
				GtkEntry * blah = GTK_ENTRY(GTK_COMBO(comboBox)->entry);
				GtkEditable * yuck = GTK_EDITABLE(blah);
				gtk_signal_connect(GTK_OBJECT(&yuck->widget),
								   "changed",
								   GTK_SIGNAL_FUNC(_wd::s_combo_changed),
								   wd);
				
				// populate it
				if (pControl)
				{
					pControl->populate();

					const UT_Vector * v = pControl->getContents();
					UT_ASSERT(v);

					if (v)
					{
						UT_uint32 items = v->getItemCount();
						for (UT_uint32 m=0; m < items; m++)
						{
							char * sz = (char *)v->getNthItem(m);
							GtkWidget * li = gtk_list_item_new_with_label(sz);
							gtk_widget_show(li);
							gtk_container_add (GTK_CONTAINER(GTK_COMBO(comboBox)->list), li);
						}
					}
				}
 
				// give a final show
				gtk_widget_show(comboBox);

				// stick it in the toolbar
				gtk_toolbar_append_widget(GTK_TOOLBAR(m_wToolbar),
										  comboBox,
										  szToolTip,
										  (const char *) NULL);
				wd->m_widget = comboBox;

				// for now, we never repopulate, so can just toss it
				DELETEP(pControl);
			}
			break;
					
			case EV_TBIT_StaticLabel:
				// TODO do these...
				break;
					
			case EV_TBIT_Spacer:
				break;
					
			case EV_TBIT_BOGUS:
			default:
				UT_ASSERT(0);
				break;
			}

			// add item after bindings to catch widget returned to us
			m_vecToolbarWidgets.addItem(wd);
		}
		break;
			
		case EV_TLF_Spacer:
		{
			// Append to the vector even if spacer, to sync up with refresh
			// which expects each item in the layout to have a place in the
			// vector.
			_wd * wd = new _wd(this,id);
			UT_ASSERT(wd);
			m_vecToolbarWidgets.addItem(wd);

			gtk_widget_show(m_wToolbar);
			_addToolbar(m_wToolbar);
			m_wToolbar = _makeToolbar();
			break;
		}
		
		default:
			UT_ASSERT(0);
		}
	}

	// show the complete thing
	gtk_widget_show(m_wToolbar);

	// add the toolbar to the band
	_addToolbar(m_wToolbar);
	nbBands++;

	return UT_TRUE;
}

UT_Bool EV_UnixGnomeToolbar::_addToolbar (GtkWidget *toolbar)
{
	GnomeDockItemBehavior beh;
	char *buf;

	// an arbitrary padding to make our document not run into our buttons
	gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);

	if (gnome_preferences_get_toolbar_detachable())
		beh = GNOME_DOCK_ITEM_BEH_NORMAL;
	else
		beh = GNOME_DOCK_ITEM_BEH_LOCKED;

	// HACK: UGLIEST HACK OF THE CENTURY!!
	// HACK: I want the GNOME_DOCK_ITEM_NEVER_VERTICAL for the first
	// HACK: toolbar of the second band... (this must to be set in the xp
	// HACK: toolbar description)
	if (nbBands == 1 && nbToolbarsInBand == 0)
		beh = static_cast<GnomeDockItemBehavior> (beh | GNOME_DOCK_ITEM_BEH_NEVER_VERTICAL);

	buf = g_strdup_printf("Toolbar %d-%d", nbBands, ++nbToolbarsInBand);
	//	g_print ("Toolbar style = %d\n", static_cast<int> (toolbar->style));
	gnome_app_add_toolbar(GNOME_APP(m_pUnixFrame->getTopLevelWindow()),
						  GTK_TOOLBAR (toolbar), buf, beh, GNOME_DOCK_TOP,
						  nbBands + 1, nbToolbarsInBand, 0);
	m_vecToolbars.addItem(toolbar);
	free(buf);
	return UT_TRUE;
}

GtkWidget *EV_UnixGnomeToolbar::_makeToolbar(void)
{

	////////////////////////////////////////////////////////////////
	// get toolbar button appearance from the preferences
	////////////////////////////////////////////////////////////////
	const XML_Char * szValue = NULL;
	static GtkToolbarStyle style = GTK_TOOLBAR_ICONS;
	static UT_Bool firstTime = UT_TRUE;
	GtkWidget *toolbar;

	if (firstTime)
	{
		firstTime = UT_FALSE;
		m_pUnixApp->getPrefsValue(XAP_PREF_KEY_ToolbarAppearance, &szValue);
		UT_ASSERT((szValue) && (*szValue));
		
		if (UT_XML_stricmp(szValue,"icon") == 0)
			style = GTK_TOOLBAR_ICONS;
		else if (UT_XML_stricmp(szValue,"text") == 0)
			style = GTK_TOOLBAR_TEXT;
		else if (UT_XML_stricmp(szValue,"both") == 0)
			style = GTK_TOOLBAR_BOTH;
	}
		
	toolbar = gtk_toolbar_new(GTK_ORIENTATION_HORIZONTAL, style);
	UT_ASSERT(GTK_TOOLBAR (toolbar));
	gtk_toolbar_set_button_relief(GTK_TOOLBAR (toolbar), GTK_RELIEF_NONE);
	gtk_toolbar_set_tooltips(GTK_TOOLBAR (toolbar), TRUE);
	gtk_toolbar_set_space_size(GTK_TOOLBAR (toolbar), 10);

	return toolbar;
}

void EV_UnixGnomeToolbar::show(void)
{
	UT_uint32 nbToolbars = m_vecToolbars.getItemCount();
	for (UT_uint32 i = 0; i < nbToolbars; i++)
	{
		GtkWidget *toolbar = static_cast<GtkWidget *> (m_vecToolbars.getNthItem(i));
		gtk_widget_show (toolbar->parent);
	}
}

void EV_UnixGnomeToolbar::hide(void)
{
	UT_uint32 nbToolbars = m_vecToolbars.getItemCount();
	for (UT_uint32 i = 0; i < nbToolbars; i++)
	{
		GtkWidget *toolbar = static_cast<GtkWidget *> (m_vecToolbars.getNthItem(i));
		gtk_widget_hide (toolbar->parent);
		gtk_widget_queue_resize (toolbar->parent->parent);
	}
}
