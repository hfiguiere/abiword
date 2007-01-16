/* AbiSource Application Framework
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

#ifndef XAP_QNXDIALOG_INSERT_SYMBOL_H
#define XAP_QNXDIALOG_INSERT_SYMBOL_H

#include "xap_Dlg_Insert_Symbol.h"

#define DEFAULT_QNX_SYMBOL_FONT "Symbol"

class XAP_QNXFrame;

/*****************************************************************/

class XAP_QNXDialog_Insert_Symbol: public XAP_Dialog_Insert_Symbol
{
public:
	XAP_QNXDialog_Insert_Symbol(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~XAP_QNXDialog_Insert_Symbol(void);

	//DEPRECATED
	virtual void			runModal(XAP_Frame * pFrame);

	virtual void			runModeless(XAP_Frame * pFrame);
	virtual void			notifyActiveFrame(XAP_Frame *pFrame);
	virtual void			notifyCloseFrame(XAP_Frame *pFrame);
	virtual void			destroy(void);
	virtual void            activate(void);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);

	// callbacks can fire these events

	virtual void			event_OK(void);
	virtual void			event_Cancel(void);
	virtual void			SymbolMap_exposed( void);
	virtual void			Symbolarea_exposed( void);
	virtual void			SymbolMap_clicked(PtCallbackInfo_t * event);
	virtual void			CurrentSymbol_clicked(PtCallbackInfo_t *event);
	virtual void			Key_Pressed(void * e);
	virtual void            New_Font( void);
	virtual void			event_WindowDelete(void);

	
protected:

	GR_QNXGraphics	* 	m_qnxGraphics;
	GR_QNXGraphics *    m_qnxarea;

	// private construction functions
	PtWidget_t * _constructWindow(void);

	// pointers to widgets we need to query/set
	PtWidget_t * m_windowMain;

	PtWidget_t * m_SymbolMap;

	PtWidget_t * m_areaCurrentSym;
	PtWidget_t * m_fontcombo;
	
	PtWidget_t * m_buttonOK;
	PtWidget_t * m_buttonCancel;

	int       done;
};

#endif /* XAP_QNXDIALOG_INSERT_SYMBOL_H */










