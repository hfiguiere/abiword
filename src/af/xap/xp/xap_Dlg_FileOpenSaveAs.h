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

#ifndef XAP_DIALOG_FILEOPENSAVEAS_H
#define XAP_DIALOG_FILEOPENSAVEAS_H

#include "xap_Dialog.h"
class XAP_App;

/*****************************************************************
** This is the XAP and XP base-class for the file-open and
** file-save-as dialogs.
*****************************************************************/

class XAP_Dialog_FileOpenSaveAs : public AP_Dialog_AppPersistent
{
public:
	XAP_Dialog_FileOpenSaveAs(AP_DialogFactory * pDlgFactory, AP_Dialog_Id id);
	virtual ~XAP_Dialog_FileOpenSaveAs(void);

	virtual void						useStart(void);
	virtual void						runModal(XAP_Frame * pFrame) = 0;
	virtual void						useEnd(void);

	typedef enum { a_VOID, a_OK, a_CANCEL }	tAnswer;
	
	void								setCurrentPathname(const char * szPathname);
	void								setSuggestFilename(UT_Bool);
	XAP_Dialog_FileOpenSaveAs::tAnswer	getAnswer(void) const;
	const char *						getPathname(void) const;
	
protected:
	char *								m_szPersistPathname;
	char *								m_szInitialPathname;
	char *								m_szFinalPathname;
	UT_Bool								m_bSuggestName;
	XAP_Dialog_FileOpenSaveAs::tAnswer	m_answer;
	XAP_App *							m_pApp;
};

#endif /* XAP_DIALOG_FILEOPENSAVEAS_H */
