/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */

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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ut_types.h"
#include "ut_stack.h"
#include "ut_string.h"
#include "ut_debugmsg.h"
#include "xap_Types.h"
#include "ev_UnixGnomeMenuBar.h"
#include "ev_UnixGnomeMenuPopup.h"
#include "xap_UnixGnomeApp.h"
#include "xap_UnixGnomeFrame.h"
#include "ev_UnixKeyboard.h"
#include "ev_Menu_Layouts.h"
#include "ev_Menu_Actions.h"
#include "ev_Menu_Labels.h"
#include "ev_EditEventMapper.h"

// get the print-preview menu icon
#include "tb_menu_print_preview.xpm"
#include "tb_menu_insert_graphic.xpm"

// hack for international menus - goes against our XAP design
// I don't like it, but I like non-gnome menus even more
#include "../../../../wp/ap/xp/ap_Menu_Id.h"

static bool s_init = false;

/*****************************************************************/

struct __Aux {
	EV_UnixGnomeMenu *me;
	int id;
};

static const char ** _ev_GetLabelName(XAP_UnixApp * pUnixApp,
									  XAP_UnixFrame * pUnixFrame,
									  EV_Menu_Action * pAction,
									  EV_Menu_Label * pLabel)
{
	static const char * data[2] = {NULL, NULL};

	// hit the static pointers back to null each time around
	data[0] = NULL;
	data[1] = NULL;
	
	const char * szLabelName;
	
	if (pAction->hasDynamicLabel())
		szLabelName = pAction->getDynamicLabel(pUnixFrame,pLabel);
	else
		szLabelName = pLabel->getMenuLabel();

	if (!szLabelName || !*szLabelName)
		return data;	// which will be two nulls now

	static char accelbuf[32];
	{
		// see if this has an associated keybinding
		const char * szMethodName = pAction->getMethodName();

		if (szMethodName)
		{
			const EV_EditMethodContainer * pEMC = pUnixApp->getEditMethodContainer();
			UT_ASSERT(pEMC);

			EV_EditMethod * pEM = pEMC->findEditMethodByName(szMethodName);
			UT_ASSERT(pEM);						// make sure it's bound to something

			const EV_EditEventMapper * pEEM = pUnixFrame->getEditEventMapper();
			UT_ASSERT(pEEM);

			const char * string = pEEM->getShortcutFor(pEM);
			if (string && *string)
				strcpy(accelbuf, string);
			else
				// zero it out for this round
				*accelbuf = 0;
		}
	}

	// set shortcut mnemonic, if any
	if (*accelbuf)
		data[1] = accelbuf;
	
	if (!pAction->raisesDialog())
	{
		data[0] = szLabelName;
		return data;
	}

	// append "..." to menu item if it raises a dialog
	static char buf[128];
	memset(buf,0,NrElements(buf));
	strncpy(buf,szLabelName,NrElements(buf)-4);
	strcat(buf,"...");

	data[0] = buf;
	
	return data;
}

static void _ev_convert(char * bufResult,
						const char * szString)
{
	UT_ASSERT(szString && bufResult);
	
	strcpy(bufResult, szString);

	char * pl = bufResult;
	while (*pl)
	{
		if (*pl == '&')
			*pl = '_';
		pl++;
	}
}

/**********************************************************************/

EV_UnixGnomeMenu::EV_UnixGnomeMenu(XAP_UnixApp * pUnixApp,
								   XAP_UnixFrame * pUnixFrame,
								   const char * szMenuLayoutName,
								   const char * szMenuLabelSetName)
	: EV_UnixMenu(pUnixApp, pUnixFrame, szMenuLayoutName, szMenuLabelSetName)
{
	m_pUIInfo = NULL;

	if(!s_init)
		{
			/* register non-standard pixmaps with the gnome-stock engine */
			s_init = true;

			static struct AbiWordStockPixmap{
				int width, height;
				char const * const name;
				gchar **xpm_data;
			} const entry_names [] = {
				{ 16, 16, "Menu_AbiWord_PrintPreview", tb_menu_print_preview_xpm },
				{ 16, 16, "Menu_AbiWord_InsertGraphic", tb_menu_insert_graphic_xpm },
				{ 0, 0, NULL, NULL}
			};

			static GnomeStockPixmapEntry entry[sizeof(entry_names)/sizeof(struct AbiWordStockPixmap)-1];

			int i = 0;

			for (i = 0; entry_names[i].name != NULL ; i++) {
				entry[i].data.type = GNOME_STOCK_PIXMAP_TYPE_DATA;
				entry[i].data.width = entry_names[i].width;
				entry[i].data.height = entry_names[i].height;
				entry[i].data.xpm_data = entry_names[i].xpm_data;
				gnome_stock_pixmap_register (entry_names[i].name,
											 GNOME_STOCK_PIXMAP_REGULAR, entry + i);
			}
		}
}

EV_UnixGnomeMenu::~EV_UnixGnomeMenu(void)
{
	_destroyUIInfo (m_pUIInfo);
}

/**********************************************************************/

struct mapping {
	int id;
	char * gnome;
};

/**
 * This function tries to find the "gnome_stock" name of the pixmap
 * associate to the name of the menu entry.  Usually the name of the
 * gnome stock pixmap is only Menu_"name", but there are some exceptions
 * that are listed in the static exceptions var.
 */
void EV_UnixGnomeMenu::s_getStockPixmapFromName (int id, const char *name,
												 char *pixmap_name,
												 int n)
{
	char *true_name;
	char *tmp;
	int i = 0, j = 0;

	static struct mapping exceptions[] = {
		{AP_MENU_ID_FILE_NEW, GNOME_STOCK_MENU_NEW},
		{AP_MENU_ID_FILE_OPEN, GNOME_STOCK_MENU_OPEN},
		{AP_MENU_ID_FILE_SAVE, GNOME_STOCK_MENU_SAVE},
		{AP_MENU_ID_FILE_SAVEAS, GNOME_STOCK_MENU_SAVE_AS},
		{AP_MENU_ID_FILE_CLOSE, GNOME_STOCK_MENU_CLOSE},
		{AP_MENU_ID_FILE_PAGESETUP, GNOME_STOCK_MENU_PRINT},
		{AP_MENU_ID_FILE_PRINT, GNOME_STOCK_MENU_PRINT},
		{AP_MENU_ID_FILE_PRINT_PREVIEW, "Menu_AbiWord_PrintPreview"},
		{AP_MENU_ID_FILE_EXIT, GNOME_STOCK_MENU_QUIT},

		{AP_MENU_ID_FILE_RECENT_1, GNOME_STOCK_MENU_NEW},
		{AP_MENU_ID_FILE_RECENT_2, GNOME_STOCK_MENU_NEW},
		{AP_MENU_ID_FILE_RECENT_3, GNOME_STOCK_MENU_NEW},
		{AP_MENU_ID_FILE_RECENT_4, GNOME_STOCK_MENU_NEW},

		{AP_MENU_ID_EDIT_UNDO, GNOME_STOCK_MENU_UNDO},
		{AP_MENU_ID_EDIT_REDO, GNOME_STOCK_MENU_REDO},
		{AP_MENU_ID_EDIT_CUT, GNOME_STOCK_MENU_CUT},
		{AP_MENU_ID_EDIT_COPY, GNOME_STOCK_MENU_COPY},
		{AP_MENU_ID_EDIT_PASTE, GNOME_STOCK_MENU_PASTE},
		{AP_MENU_ID_EDIT_FIND, GNOME_STOCK_MENU_SEARCH},
		{AP_MENU_ID_EDIT_REPLACE, GNOME_STOCK_MENU_SRCHRPL},
		{AP_MENU_ID_EDIT_GOTO, GNOME_STOCK_MENU_JUMP_TO},

		{AP_MENU_ID_INSERT_DATETIME, GNOME_STOCK_MENU_TIMER},
		{AP_MENU_ID_INSERT_GRAPHIC, "Menu_AbiWord_InsertGraphic"},

		{AP_MENU_ID_FMT_FONT, GNOME_STOCK_MENU_FONT},

		{AP_MENU_ID_TOOLS_SPELL, GNOME_STOCK_MENU_SPELLCHECK},
		{AP_MENU_ID_TOOLS_OPTIONS, GNOME_STOCK_MENU_PREF},

		{AP_MENU_ID_WINDOW_1, GNOME_STOCK_MENU_NEW},
		{AP_MENU_ID_WINDOW_2, GNOME_STOCK_MENU_NEW},
		{AP_MENU_ID_WINDOW_3, GNOME_STOCK_MENU_NEW},
		{AP_MENU_ID_WINDOW_4, GNOME_STOCK_MENU_NEW},

		{AP_MENU_ID_HELP_ABOUT, GNOME_STOCK_MENU_ABOUT},
		
		{AP_MENU_ID__BOGUS2__, NULL}
	};

	do {
		if (id == exceptions[i].id) {
			strncpy (pixmap_name, exceptions[i].gnome, n);
			return;
		}
		else
			i++;
	} while (exceptions[i].id != AP_MENU_ID__BOGUS2__);

	true_name = g_new0 (char, n);

	for (i = 0; i < (int) strlen (name); i++)
		if ((tolower (name[i]) >= 'a') && (tolower (name[i]) <= 'z') || name[i] == ' ')
			true_name[j++] = name[i];
	
	tmp = g_strconcat ("Menu_", true_name, NULL);
	strncpy (pixmap_name, tmp, n);

	g_free (tmp);
	g_free (true_name);
}

/**
 * Callback called when the user "selects" a menu item (ie. the mouse pointer
 * is over this menu item).  It prints the statusbar message in the status bar.
 * The statusbar message of an item is saved in his associated hash table, with
 * the key "abi_statusbar_text".
 */
void EV_UnixGnomeMenu::s_onMenuItemSelect(GtkWidget * widget, gpointer data)
{
	__Aux *aux = (__Aux *) data;
	UT_ASSERT(aux && aux->me);
	XAP_UnixFrame * pFrame = aux->me->getFrame();
	UT_ASSERT(pFrame);
	
	const gchar* szMsg = (gchar *) gtk_object_get_data(GTK_OBJECT(widget),
													   "abi_statusbar_text");
	
	if (!szMsg || !*szMsg)
		szMsg = "TODO This menu item doesn't have a StatusMessage defined.";
	
	pFrame->setStatusMessage(szMsg);
};

/**
 * Callback called when the user "deselects" a menu item (ie. the mouse pointer
 * leaves this menu item).  It erases the menubar.
 */
void EV_UnixGnomeMenu::s_onMenuItemDeselect(GtkWidget * widget, gpointer data)
{
	__Aux *aux = (__Aux *) data;
	UT_ASSERT(aux && aux->me);

	XAP_UnixFrame * pFrame = ((EV_UnixMenu *) (aux->me))->getFrame();
	UT_ASSERT(pFrame);

	pFrame->setStatusMessage(NULL);
};

/**
 * Callback called each time the menu is displayed.
 */
void EV_UnixGnomeMenu::s_onInitMenu(GtkMenuItem * menuItem, gpointer data)
{
	__Aux *aux = (__Aux *) data;
	UT_ASSERT(aux);
	UT_ASSERT(aux->me);

	XAP_UnixFrame *pFrame = ((EV_UnixMenu *) (aux->me))->getFrame ();
	UT_ASSERT(pFrame);
		
	aux->me->refreshMenu(pFrame->getCurrentView ());
};

/**
 * Callback called each time that one menu disappears.
 */
void EV_UnixGnomeMenu::s_onDestroyMenu(GtkMenuItem * menuItem, gpointer data)
{
	__Aux *aux = (__Aux *) data;
	UT_ASSERT(aux && aux->me);

	// we always clear the status bar when a menu goes away, so we don't
	// leave a message behind
	XAP_UnixFrame * pFrame = aux->me->getFrame();
	UT_ASSERT(pFrame);

	pFrame->setStatusMessage(NULL);
};

// GTK wants to run popup menus asynchronously, but we want synchronous,
// so we need to do a gtk_main_quit() on our own to show we're done
// with our modal work.
#if 0
void EV_UnixGnomeMenu::s_onDestroyPopupMenu(GtkMenuItem * menuItem, gpointer callback_data)
{
	// do the grunt work
	s_onDestroyMenu(menuItem, callback_data);
	gtk_main_quit();
};
#endif

/**
 * This subroutine calcules the gdk accel_key, ac_mods associated to
 * a given string (for instance, str = "Ctrl+A" -> accel_key = 'A'
 * ac_mods = GDK_CONTROL_MASK)
 */
void EV_UnixGnomeMenu::_convertString2Accel(const char *str,
											guint &accel_key,
											GdkModifierType &ac_mods)
{
	if (str == NULL || *str == '\0')
		return;

	if (strncmp (str, "Ctrl+", 5) == 0) {
		ac_mods = (GdkModifierType) (ac_mods | GDK_CONTROL_MASK);
		str += 5;
	}

	if (strncmp (str, "Alt+", 4) == 0) {
		ac_mods = (GdkModifierType) (ac_mods | GDK_MOD1_MASK);
		str += 4;
	}

	if (strncmp (str, "Shift+", 6) == 0) {
		ac_mods = (GdkModifierType) (ac_mods | GDK_SHIFT_MASK);
		str += 6;
	}

	if (strncmp (str, "Del", 3) == 0) {
		accel_key = GDK_Delete;
	}
	else if (str[0] == 'F' &&
			 str[1] >= '0' &&
			 str[1] <= '9') {
		accel_key = 0xFFBD + atoi(str + 1);
	}
	else {
		accel_key = (guint) str[0];
	}
}

/**
 * This function does the "hard" work.  It converts the "AbiSource menu" into
 * a GnomeUIInfo structure (the "gnome format" to code the menus).  The function
 * starts the conversion in the @pos "item" (an item can be a menu item,
 * a submenu, etc...) of the menu.  Due to the recursive nature of this function,
 * the pos variable is passed by reference... <-- TODO: Explain that...
 */
GnomeUIInfo * EV_UnixGnomeMenu::_convertMenu2UIInfo (int &pos)
{
	const EV_Menu_ActionSet * pMenuActionSet = m_pUnixApp->getMenuActionSet();
	int nItems = (int) m_pMenuLayout->getLayoutItemCount();
	GnomeUIInfo * retval = NULL;
	bool endofsubmenu = false;
	int i;

	UT_ASSERT(nItems > pos);
	UT_ASSERT(pos >= 0);

	retval = g_new0 (GnomeUIInfo, nItems + 1);

	for (i = 0; (pos < nItems) && !endofsubmenu; i++)
	{
		EV_Menu_LayoutItem * pLayoutItem = m_pMenuLayout->getLayoutItem(pos);
		XAP_Menu_Id id = pLayoutItem->getMenuId();
		EV_Menu_Action * pAction = pMenuActionSet->getAction(id);
		EV_Menu_Label * pLabel = m_pMenuLabelSet->getLabel(id);

		UT_ASSERT(pLayoutItem);
		UT_ASSERT(pAction);
		UT_ASSERT(pLabel);
		
		switch (pLayoutItem->getMenuLayoutFlags())
		{
		case EV_MLF_Normal:
		{
			const char ** data = _ev_GetLabelName(m_pUnixApp, m_pUnixFrame, pAction, pLabel);
			const char *szLabelName = data[0];
			const char *szMnemonicName = data[1];
			
			if (szLabelName && *szLabelName)
			{
				char buf[1024];
				const char *tooltip;

				// convert label into underscored version
				_ev_convert(buf, szLabelName);
				tooltip = pLabel->getMenuStatusMessage();
				
				if ( !pAction->isCheckable() ) {
					retval[i].type = GNOME_APP_UI_ITEM;
					retval[i].pixmap_type = GNOME_APP_PIXMAP_STOCK;
					retval[i].pixmap_info = g_new0 (char, 64);
					s_getStockPixmapFromName (id, buf, (char *) retval[i].pixmap_info, 64);
				}
				else {
					retval[i].type = GNOME_APP_UI_TOGGLEITEM;
				}
				
				retval[i].label = g_strdup (buf);
				retval[i].hint = g_strdup (tooltip);
				retval[i].moreinfo = (void*)menuEvent;
				retval[i].user_data = g_new0 (__Aux, 1);
				((__Aux *) retval[i].user_data)->me = this;
				((__Aux *) retval[i].user_data)->id = id;
			}
			else
			{
				i--;
			}

			_convertString2Accel (szMnemonicName, retval[i].accelerator_key, retval[i].ac_mods);
			break;
		}
		case EV_MLF_BeginSubMenu:
		{
			const char ** data = _ev_GetLabelName(m_pUnixApp, m_pUnixFrame, pAction, pLabel);
			const char *szLabelName = data[0];
			
			if (szLabelName && *szLabelName)
			{
				char buf[1024];
				//const char *tooltip;
				
				// convert label into underscored version
				_ev_convert(buf, szLabelName);
				//tooltip = pLabel->getMenuStatusMessage();

				retval[i].type = GNOME_APP_UI_SUBTREE;
				retval[i].label = g_strdup (buf);
				
				retval[i].user_data = g_new0 (__Aux, 1);
				((__Aux *) retval[i].user_data)->me = this;
				((__Aux *) retval[i].user_data)->id = id;
				retval[i].moreinfo = _convertMenu2UIInfo (++pos);
				pos--;
			}
		
			break;
		}
		case EV_MLF_EndSubMenu:
		{
			endofsubmenu = true;
			break;
		}
		case EV_MLF_Separator:
		{
			retval[i].type = GNOME_APP_UI_SEPARATOR;
			retval[i].user_data = GINT_TO_POINTER (id);
			break;
		}
		case EV_MLF_BeginPopupMenu:
			i--;
			break;
		case EV_MLF_EndPopupMenu:
			endofsubmenu = true;
			break;
			
		default:
			UT_ASSERT(0);
			break;
		}
		pos++;
	}

	retval = g_renew(GnomeUIInfo, retval, i + 1);
	  
	return (retval);
}

/**
 * Free the mem associate with the uiinfo structure (in a recursive way).
 */
void EV_UnixGnomeMenu::_destroyUIInfo(GnomeUIInfo * uiinfo)
{
	g_return_if_fail (uiinfo != NULL);

	GnomeUIInfo *first = uiinfo;

	do {
		if (uiinfo->label != NULL)
			g_free (uiinfo->label);
		if (uiinfo->pixmap_info != NULL)
			g_free ((gpointer) uiinfo->pixmap_info);
		if (uiinfo->hint != NULL)
			g_free (uiinfo->hint);

		switch (uiinfo->type) {
		case GNOME_APP_UI_SUBTREE:
			_destroyUIInfo ((GnomeUIInfo *) uiinfo->moreinfo);
		case GNOME_APP_UI_ITEM:
		case GNOME_APP_UI_TOGGLEITEM:
			g_free(uiinfo->user_data);
			break;
		default:
			;
		}

		uiinfo++;
	} while (uiinfo->type != GNOME_APP_UI_ENDOFINFO);

	g_free (first);
}

bool EV_UnixGnomeMenu::_refreshMenu(AV_View * pView, GtkWidget * wMenuRoot)
{
	// update the status of stateful items on menu bar.
	const EV_Menu_ActionSet * pMenuActionSet = m_pUnixApp->getMenuActionSet();
	UT_ASSERT(pMenuActionSet);
	UT_uint32 nrLabelItemsInLayout = m_pMenuLayout->getLayoutItemCount();
	GtkWidget *item;

	for (UT_uint32 k=0; (k < nrLabelItemsInLayout); k++)
	{
		EV_Menu_LayoutItem * pLayoutItem = m_pMenuLayout->getLayoutItem(k);
		XAP_Menu_Id id = pLayoutItem->getMenuId();
		EV_Menu_Action * pAction = pMenuActionSet->getAction(id);
		EV_Menu_Label * pLabel = m_pMenuLabelSet->getLabel(id);

		switch (pLayoutItem->getMenuLayoutFlags())
		{
		case EV_MLF_Normal:
		{
			// TODO: Dynamic labels and widgets
			// see if we need to enable/disable and/or check/uncheck it.
			bool bEnable = true;
			bool bCheck = false;
			
			if (pAction->hasGetStateFunction())
			{
				EV_Menu_ItemState mis = pAction->getMenuItemState(pView);
				if (mis & EV_MIS_Gray)
					bEnable = false;
				if (mis & EV_MIS_Toggled)
					bCheck = true;
			}
			
			// Get the dynamic label
			const char ** data = _ev_GetLabelName(m_pUnixApp, m_pUnixFrame, pAction, pLabel);
			const char * szLabelName = data[0];

			if (szLabelName && *szLabelName)
			{
				char buf[1024];
				
				_ev_convert(buf, szLabelName);
				item = (GtkWidget *) gtk_object_get_data (GTK_OBJECT (wMenuRoot), buf);
			}
			else
				item = NULL;
			
			if (item == NULL)
				break;
			
			if (GTK_IS_CHECK_MENU_ITEM(item))
				GTK_CHECK_MENU_ITEM(item)->active = bCheck;
			
			gtk_widget_set_sensitive(GTK_WIDGET(item), bEnable);			
			break;
		}
		case EV_MLF_BeginSubMenu:
		case EV_MLF_EndSubMenu:
		case EV_MLF_Separator:
		case EV_MLF_BeginPopupMenu:
		case EV_MLF_EndPopupMenu:
			break;

		default:
			UT_ASSERT(0);
			break;
		}	
	}

	return true;
}

bool EV_UnixGnomeMenu::synthesizeMenu(GtkWidget * wMenuRoot)
{
	int i = 0;
	GtkWidget *app;

	m_pUIInfo = _convertMenu2UIInfo (i);
	app = m_pUnixFrame->getTopLevelWindow ();
	gnome_app_fill_menu (GTK_MENU_SHELL (wMenuRoot), m_pUIInfo,
						 GNOME_APP (app)->accel_group, TRUE, 0);
	_attachWidgetsAndSignals (wMenuRoot, m_pUIInfo);

	return true;
}

void EV_UnixGnomeMenu::_attachWidgetsAndSignals(GtkWidget * wMenuRoot, GnomeUIInfo * uiinfo)
{
	g_return_if_fail (uiinfo != NULL);
	g_return_if_fail (wMenuRoot != NULL);

	while (uiinfo->type != GNOME_APP_UI_ENDOFINFO) {
		if (uiinfo->widget != NULL) {
			
			if (uiinfo->label != NULL) {
				gtk_widget_ref (uiinfo->widget);
				
				gtk_object_set_data_full (GTK_OBJECT (wMenuRoot), uiinfo->label,
										  uiinfo->widget,
										  (GtkDestroyNotify) gtk_widget_unref);
			}
			
			if ((uiinfo->type == GNOME_APP_UI_ITEM) ||
			    (uiinfo->type == GNOME_APP_UI_TOGGLEITEM)) {
				// connect callbacks
				gtk_signal_connect(GTK_OBJECT(uiinfo->widget), "select",
								   GTK_SIGNAL_FUNC(s_onMenuItemSelect), uiinfo->user_data);
				gtk_signal_connect(GTK_OBJECT(uiinfo->widget), "deselect",
								   GTK_SIGNAL_FUNC(s_onMenuItemDeselect), uiinfo->user_data);
			}

			if (uiinfo->hint != NULL) {
				gtk_object_set_data (GTK_OBJECT (uiinfo->widget), "abi_statusbar_text",
									 uiinfo->hint);
			}
		}

		if (uiinfo->type == GNOME_APP_UI_SUBTREE) {
			// hack - refresh all of the menus whenever one gets selected
			// because we can't refresh just one
			gtk_signal_connect(GTK_OBJECT(((GnomeUIInfo *) uiinfo->widget)), "select",
							   GTK_SIGNAL_FUNC(s_onInitMenu), uiinfo->user_data);		
			gtk_signal_connect(GTK_OBJECT(((GnomeUIInfo *) uiinfo->moreinfo)->widget), "map",
							   GTK_SIGNAL_FUNC(s_onInitMenu), uiinfo->user_data);
			gtk_signal_connect(GTK_OBJECT(((GnomeUIInfo *) uiinfo->moreinfo)->widget), "unmap",
							   GTK_SIGNAL_FUNC(s_onDestroyMenu), uiinfo->user_data);

			_attachWidgetsAndSignals (wMenuRoot, (GnomeUIInfo *) uiinfo->moreinfo);
		}

		uiinfo++;
	};
}

void EV_UnixGnomeMenu::menuEvent(GtkWidget *, gpointer pAux)
{
	// user selected something from the menu.
	// invoke the appropriate function.
	// return true iff handled.
	__Aux *aux = (__Aux *) pAux;
	XAP_Menu_Id id = aux->id;

	const EV_Menu_ActionSet * pMenuActionSet = aux->me->m_pUnixApp->getMenuActionSet();
	UT_ASSERT(pMenuActionSet);
	const EV_Menu_Action * pAction = pMenuActionSet->getAction(id);
	UT_ASSERT(pAction);
	const char * szMethodName = pAction->getMethodName();
	
	const EV_EditMethodContainer * pEMC = aux->me->m_pUnixApp->getEditMethodContainer();
	UT_ASSERT(pEMC);

	EV_EditMethod * pEM = pEMC->findEditMethodByName(szMethodName);
	UT_ASSERT(pEM);						// make sure it's bound to something

	aux->me->invokeMenuMethod(aux->me->m_pUnixFrame->getCurrentView(), pEM, 0, 0);
}

/***********************************************************************/

EV_UnixGnomeMenuBar::EV_UnixGnomeMenuBar(XAP_UnixApp * pUnixApp,
										 XAP_UnixFrame * pUnixFrame,
										 const char * szMenuLayoutName,
										 const char * szMenuLabelSetName)
	: EV_UnixMenuBar(/*static_cast<XAP_UnixApp *>*/ pUnixApp, /*static_cast<XAP_UnixFrame *>*/ pUnixFrame,szMenuLayoutName,szMenuLabelSetName)
{
}

EV_UnixGnomeMenuBar::~EV_UnixGnomeMenuBar(void)
{
}

bool EV_UnixGnomeMenuBar::synthesizeMenuBar(void)
{
	m_wMenuBar = gtk_menu_bar_new();

	synthesizeMenu(m_wMenuBar);
	gtk_widget_show(m_wMenuBar);
	gnome_app_set_menus(GNOME_APP(m_pUnixFrame->getTopLevelWindow()), GTK_MENU_BAR(m_wMenuBar));

	return true;
}

bool EV_UnixGnomeMenuBar::refreshMenu(AV_View * pView)
{
	if (pView)
		return _refreshMenu(pView, m_wMenuBar);

	return true;
}

/***********************************************************************/

EV_UnixGnomeMenuPopup::EV_UnixGnomeMenuPopup(XAP_UnixApp * pUnixApp,
											 XAP_UnixFrame * pUnixFrame,
											 const char * szMenuLayoutName,
											 const char * szMenuLabelSetName)
	: EV_UnixMenuPopup(pUnixApp,pUnixFrame,szMenuLayoutName,szMenuLabelSetName)
{
	m_wMenuPopup = NULL;
}

EV_UnixGnomeMenuPopup::~EV_UnixGnomeMenuPopup(void)
{
	if (m_wMenuPopup != NULL)
		gtk_widget_destroy (m_wMenuPopup);
}

bool EV_UnixGnomeMenuPopup::synthesizeMenuPopup(void)
{
	m_wMenuPopup = gtk_menu_new();

	synthesizeMenu(m_wMenuPopup);

	return true;
}
