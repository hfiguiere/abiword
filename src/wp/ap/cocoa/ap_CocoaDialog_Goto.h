/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2001 Hubert Figuiere
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

#ifndef AP_COCOADIALOG_GOTO_H
#define AP_COCOADIALOG_GOTO_H

#include "ap_Dialog_Goto.h"
class XAP_CocoaFrame;

/*****************************************************************/

class AP_CocoaDialog_Goto: public AP_Dialog_Goto
{
public:
	AP_CocoaDialog_Goto(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~AP_CocoaDialog_Goto(void);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);

	virtual void			runModeless(XAP_Frame * pFrame);
	virtual void			destroy(void);
	virtual void			activate(void);
	virtual void                    notifyActiveFrame(XAP_Frame *pFrame);
	void					setSelectedRow(int row);
	int						getSelectedRow(void);

#if 0
	/* CALLBACKS */
	static void				s_targetChanged(GtkWidget *clist, gint row, gint column,
											GdkEventButton *event, AP_CocoaDialog_Goto *me);
	static void				s_dataChanged (GtkWidget *widget, AP_CocoaDialog_Goto * me);
	static void				s_goto (const char *number, AP_CocoaDialog_Goto * me);
	static void				s_gotoClicked (GtkWidget * widget, AP_CocoaDialog_Goto * me);
	static void				s_nextClicked (GtkWidget * widget, AP_CocoaDialog_Goto * me);
	static void				s_prevClicked (GtkWidget * widget, AP_CocoaDialog_Goto * me);
	static void				s_closeClicked (GtkWidget * widget, AP_CocoaDialog_Goto * me);
	static void				s_deleteClicked (GtkWidget * widget, gpointer /* data */ , AP_CocoaDialog_Goto * me);
	static void				s_blist_clicked(GtkWidget *clist, gint row, gint column,
										  GdkEventButton *event, AP_CocoaDialog_Goto *me);
	
	/* Widgets members.  Publics to make them accesible to the callbacks */
	/* TODO: Convert them to private members, and add an inline accesor/mutator per member */
	GtkWidget *				m_wMainWindow;
	GtkWidget *				m_wEntry;
	GtkWidget *				m_wPrev;
	GtkWidget *				m_wNext;
	GtkWidget *				m_wGoto;
	GtkWidget *				m_wClose;
	GtkAccelGroup *			m_accelGroup;
	int						m_iRow;
	GtkWidget *				m_swindow;
	GtkWidget *				m_dlabel;
#endif
	const XML_Char **		m_pBookmarks;
	
	
protected:
#if 0
	virtual GtkWidget *		_constructWindow(void);
	GtkWidget *				_constructWindowContents(void);
	void					_populateWindowData(void);
	void					_connectSignals(void);

	static char *			s_convert(const char * st);
#endif
};

#endif /* AP_COCOADIALOG_GOTO_H */



