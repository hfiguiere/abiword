/* AbiWord
 * Copyright (C) 2000 AbiSource, Inc.
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

#ifndef AP_UNIXDIALOG_WORDCOUNT_H
#define AP_UNIXDIALOG_WORDCOUNT_H

#include "ap_Dialog_WordCount.h"
#include "ut_timer.h"

class XAP_CocoaFrame;

/*****************************************************************/

class AP_CocoaDialog_WordCount: public AP_Dialog_WordCount
{
public:
	AP_CocoaDialog_WordCount(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~AP_CocoaDialog_WordCount(void);

	virtual void			runModeless(XAP_Frame * pFrame);
	virtual void			destroy(void);
	virtual void			activate(void);
	virtual void			notifyActiveFrame(XAP_Frame *pFrame);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);
	static void			autoupdateWC(UT_Worker * pTimer);
	virtual void			set_sensitivity(void);
	virtual void			setUpdateCounter(void);
	// callbacks can fire these events

	virtual void			event_OK(void);
	virtual void			event_Update(void);
	virtual void			event_Checkbox(void);
	virtual void			event_Spin(void);
	virtual void			event_WindowDelete(void);

protected:
#if 0
	// private construction functions
	virtual GtkWidget * _constructWindow(void);
	void				_populateWindowData(void);
	virtual GtkWidget * _constructWindowContents(void);
	void 				_updateWindowData(void);       
	void 				_connectSignals(void);

	// pointers to widgets we need to query/set
	GtkWidget * m_windowMain;
	GtkWidget * m_wContent;
	GtkWidget * m_buttonClose;
	GtkWidget * m_buttonUpdate;
	GtkWidget * m_pTableframe;
	UT_Timer * m_pAutoUpdateWC;
	GtkWidget * m_pAutospin;
	GtkWidget * m_pAutocheck;
	GtkWidget * m_pAutospinlabel;
	GtkAdjustment * m_Spinrange;
	bool m_bAutoWC;
	guint m_Update_rate;

	// Labels for the Word Count data
	GtkWidget * m_labelWCount;
	GtkWidget * m_labelPCount;
	GtkWidget * m_labelCCount;
	GtkWidget * m_labelCNCount;
	GtkWidget * m_labelLCount;	
	GtkWidget * m_labelPgCount;	
	
	// Handshake variables
	bool m_bDestroy_says_stopupdating;
	bool m_bAutoUpdate_happening_now;
#endif
};

#endif /* AP_UNIXDIALOG_WORDCOUNT_H */








