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

#ifndef AP_UnixDialog_Styles_H
#define AP_UnixDialog_Styles_H

#include "ap_Dialog_Columns.h"
#include "xap_UnixFontManager.h"
#include "gr_UnixGraphics.h"

#include "ut_types.h"
#include "ut_string.h"
#include "ap_Dialog_Styles.h"


class XAP_UnixFrame;

/*****************************************************************/

class AP_UnixDialog_Styles: public AP_Dialog_Styles
{
public:
	typedef enum _StyleType 
	  {USED_STYLES, ALL_STYLES, USER_STYLES} StyleType;

	AP_UnixDialog_Styles(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~AP_UnixDialog_Styles(void);

	virtual void			runModal(XAP_Frame * pFrame);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);

	// callbacks can fire these events

	void                            event_paraPreviewExposed(void);
	void                            event_charPreviewExposed(void);
	
	virtual void			event_OK(void);
	virtual void			event_Cancel(void);

	virtual void event_DeleteClicked(void);
	virtual void event_NewClicked(void);
	virtual void event_ModifyClicked(void);
	virtual void event_ClistClicked(int row, int col);
	virtual void event_ListClicked(const char * which);
	virtual void			event_WindowDelete(void);
	void new_styleName(void);

/////////////////////////////////////////////////////////////////////////
// Modify window
/////////////////////////////////////////////////////////////////////////

	void         event_Modify_OK(void);
	void         event_Modify_Cancel(void);
	void         event_ModifyDelete(void);
	void         event_ModifyParagraph();
	void         event_ModifyFont();
	void         event_ModifyNumbering();
	void         event_ModifyTabs();
	void         event_ModifyPreviewExposed();

	void         modifyRunModal(void);

	void         setIsNew(bool isNew) {m_bIsNew = isNew;}
	const bool   isNew(void) const { return m_bIsNew;}     
protected:

	// private construction functions
	virtual GtkWidget * _constructWindow(void);
	GtkWidget * _constructWindowContents(GtkWidget * parent);
	void				_populateWindowData(void);
	void                            _populateCList(void) const;
	void 				_storeWindowData(void) const;
	void				_connectsignals(void) const;
	virtual const char * getCurrentStyle (void) const;
	virtual void setDescription (const char * desc) const;

	GR_UnixGraphics	* 		m_pParaPreviewWidget;
	GR_UnixGraphics	* 		m_pCharPreviewWidget;

	// pointers to widgets we need to query/set
	GtkWidget * m_windowMain;

	GtkWidget * m_wbuttonOk;
	GtkWidget * m_wbuttonCancel;
	GtkWidget * m_wbuttonNew;
	GtkWidget * m_wbuttonModify;
	GtkWidget * m_wbuttonDelete;
	GtkWidget * m_wParaPreviewArea;
	GtkWidget * m_wCharPreviewArea;
	GtkWidget * m_wGnomeButtons;

	GtkWidget * m_wclistStyles;
	GtkWidget * m_wlistTypes;
	GtkWidget * m_wlabelDesc;

	gint m_whichRow, m_whichCol;
	StyleType m_whichType;

//////////////////////////////////////////////////////////////////////////
// Modify window
/////////////////////////////////////////////////////////////////////////

	GtkWidget * _constructModifyDialog(void);
	void        _constructGnomeModifyButtons( GtkWidget * dialog_sction_area1);
	void        _constructFormatList(GtkWidget * FormatMenu);
	void        _connectModifySignals(void);
	void        _constructModifyDialogContents(GtkWidget * modifyDialog);
	virtual void setModifyDescription( const char * desc);
	bool        _populateModify(void);

	GR_UnixGraphics	* 		m_pAbiPreviewWidget;

	GtkWidget *	m_wModifyDialog;
	GtkWidget *	m_wStyleNameEntry;
	GtkWidget *	m_wBasedOnCombo;
	GtkWidget *	m_wBasedOnEntry;
	GtkWidget * m_wFollowingCombo;
	GtkWidget *	m_wFollowingEntry;
	GtkWidget *	m_wModifyDrawingArea;
	GtkWidget *	m_wLabDescription;

	GtkWidget *	m_wModifyOk;
	GtkWidget *	m_wModifyCancel;
	GtkWidget *	m_wFormatMenu;
	GtkWidget *	m_wModifyShortCutKey;

	GtkWidget *	m_wFormat;
	GtkWidget *	m_wModifyParagraph;
	GtkWidget *	m_wModifyFont;
	GtkWidget *	m_wModifyNumbering;
	GtkWidget *	m_wModifyTabs;
	GList *     m_gbasedOnStyles;
	GList *     m_gfollowedByStyles;
private:
	bool m_bIsNew;
};

#endif /* AP_UnixDialog_Styles_H */






