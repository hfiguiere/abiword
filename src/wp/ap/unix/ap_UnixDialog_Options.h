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

#ifndef AP_UNIXDIALOG_OPTIONS_H
#define AP_UNIXDIALOG_OPTIONS_H

#include "xap_UnixFontManager.h"

#include "ap_Dialog_Options.h"

class XAP_UnixFrame;

/*****************************************************************/
class AP_UnixDialog_Options: public AP_Dialog_Options
{
public:
	AP_UnixDialog_Options(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~AP_UnixDialog_Options(void);

	virtual void			runModal(XAP_Frame * pFrame);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);

	// callbacks can fire these events
	virtual void event_OK(void);
	virtual void event_Cancel(void);
	virtual void event_WindowDelete(void);

 protected:

	GtkWidget *_lookupWidget( tControl id );
	virtual void _controlEnable( tControl id, UT_Bool value );

	// we implement these so the XP dialog can set/grab our data
#define SET_GATHER_BOOL(a) virtual UT_Bool _gather##a(void);  \
						   virtual void    _set##a( UT_Bool )

	SET_GATHER_BOOL			(SpellCheckAsType);
	SET_GATHER_BOOL			(SpellHideErrors);
	SET_GATHER_BOOL			(SpellSuggest);
	SET_GATHER_BOOL			(SpellMainOnly);
	SET_GATHER_BOOL			(SpellUppercase);
	SET_GATHER_BOOL			(SpellNumbers);
	SET_GATHER_BOOL			(SpellInternet);

	SET_GATHER_BOOL			(PrefsAutoSave);

	SET_GATHER_BOOL			(ViewShowRuler);
		// have a units option
	SET_GATHER_BOOL			(ViewShowToolbars);

	SET_GATHER_BOOL			(ViewAll);
	SET_GATHER_BOOL			(ViewHiddenText);
	SET_GATHER_BOOL			(ViewUnprintable);

#undef SET_GATHER_BOOL
	
 protected:
	
	// private construction functions
	GtkWidget * _constructWindow(void);

	// pointers to widgets we need to query/set
	// there are a ton of them in this dialog

	GtkWidget * m_windowMain;

    GtkWidget * m_checkbuttonSpellCheckAsType;
    GtkWidget * m_checkbuttonSpellHideErrors;
    GtkWidget * m_checkbuttonSpellSuggest;
    GtkWidget * m_checkbuttonSpellMainOnly;
    GtkWidget * m_checkbuttonSpellUppercase;
    GtkWidget * m_checkbuttonSpellNumbers;
    GtkWidget * m_checkbuttonSpellInternet;
	GtkWidget * m_listSpellDicts;
	GtkWidget * m_listSpellDicts_menu;
	GtkWidget * m_buttonSpellDictionary;

    GtkWidget * m_checkbuttonPrefsAutoSave;
	GtkWidget * m_comboPrefsScheme;

    GtkWidget * m_checkbuttonViewShowRuler;
    GtkWidget * m_listViewRulerUnits;
    GtkWidget * m_checkbuttonViewShowToolbars;
    GtkWidget * m_checkbuttonViewAll;
    GtkWidget * m_checkbuttonViewHiddenText;
    GtkWidget * m_checkbuttonViewUnprintable;

	GtkWidget * m_buttonSave;
	GtkWidget * m_buttonDefaults;
	GtkWidget * m_buttonOK;
	GtkWidget * m_buttonCancel;

	friend void s_checkbutton_toggle(GtkWidget *, AP_UnixDialog_Options *);
	friend void s_defaults_clicked(GtkWidget *, AP_UnixDialog_Options *);

};

#endif /* AP_UNIXDIALOG_OPTIONS_H */
