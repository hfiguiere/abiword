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

#ifndef AP_QNXDIALOG_MERGECELLS_H
#define AP_QNXDIALOG_MERGECELLS_H

#include "ap_Dialog_MergeCells.h"

class XAP_QNXFrame;

/*****************************************************************/

class AP_QNXDialog_MergeCells: public AP_Dialog_MergeCells
{
public:
	AP_QNXDialog_MergeCells(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~AP_QNXDialog_MergeCells(void);

	virtual void			runModeless(XAP_Frame * pFrame);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);

	// callbacks can fire these events
	virtual void			event_Close(void);
	virtual void            setSensitivity(AP_Dialog_MergeCells::mergeWithCell mergeThis, bool bsens);
	virtual void            destroy(void);
	virtual void            activate(void);
	virtual void            notifyActiveFrame(XAP_Frame * pFrame);
protected:
	typedef enum
	{
	    BUTTON_CLOSE
	} ResponseId ;
private:
	PtWidget_t * _constructWindow(void);
	PtWidget_t *m_windowMain;
	PtWidget_t *m_MergeRight,*m_MergeLeft,*m_MergeAbove,*m_MergeBelow;
};

#endif /* AP_QNXDIALOG_MERGECELLS_H */
