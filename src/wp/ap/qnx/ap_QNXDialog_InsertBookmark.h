/* AbiWord
 * Copyright (C) 2000 AbiSource, Inc.
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

#ifndef AP_QNXDIALOG_INSERTBOOKMARK_H
#define AP_QNXDIALOG_INSERTBOOKMARK_H

#include "ap_Dialog_InsertBookmark.h"
#include <Pt.h>

class XAP_QNXFrame;

/*****************************************************************/

class AP_QNXDialog_InsertBookmark: public AP_Dialog_InsertBookmark
{
public:
	AP_QNXDialog_InsertBookmark(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~AP_QNXDialog_InsertBookmark(void);

	virtual void			runModal(XAP_Frame * pFrame);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);
  void 					event_OK(void);
  void 					event_Cancel(void);
  void 					event_Delete(void);	
	int 					done;
protected:

	PtWidget_t *	_constructWindow(void);
	void				_setCombo(void);

	PtWidget_t *m_comboBox;
};

#endif /* AP_QNXDIALOG_STUB_H */
