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

#ifndef AP_COCOADIALOG_REPLACE_H
#define AP_COCOADIALOG_REPLACE_H

#include "ap_Dialog_Replace.h"

class XAP_CocoaFrame;

/*****************************************************************/

class AP_CocoaDialog_Replace: public AP_Dialog_Replace
{
public:
	AP_CocoaDialog_Replace(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~AP_CocoaDialog_Replace(void);


	virtual void			runModal(XAP_Frame * pFrame){};
	virtual void			runModeless(XAP_Frame * pFrame);
	virtual void			notifyActiveFrame(XAP_Frame *pFrame);
	virtual void			notifyCloseFrame(XAP_Frame *pFrame){};
	virtual void			destroy(void);
	virtual void			activate(void);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);

	// callbacks can fire these events
	virtual void			event_Find(void);
	virtual void			event_Replace(void);
	virtual void			event_ReplaceAll(void);
	virtual void			event_MatchCaseToggled(void);	
	virtual void			event_Cancel(void);
	virtual void			event_WindowDelete(void);

protected:
#if 0
	// private construction functions
	virtual GtkWidget * _constructWindow(void);
	void		_populateWindowData(void);
	void 		_storeWindowData(void);

	// pointers to widgets we need to query/set
	GtkWidget * m_windowMain;

	GtkWidget * m_entryFind;
	GtkWidget * m_entryReplace;
	GtkWidget * m_checkbuttonMatchCase;

	GtkWidget * m_buttonFindNext;
	GtkWidget * m_buttonReplace;
	GtkWidget * m_buttonReplaceAll;

	GtkWidget * m_buttonCancel;
#endif
};

#endif /* AP_COCOADIALOG_REPLACE_H */































