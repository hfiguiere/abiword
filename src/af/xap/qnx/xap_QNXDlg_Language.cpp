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

#include <stdlib.h>
#include <stdio.h>

#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_types.h"

#include "xap_App.h"
#include "xap_QNXApp.h"
#include "xap_QNXFrameImpl.h"
#include "xap_Frame.h"

#include "xap_Strings.h"
#include "xap_Dialog_Id.h"
#include "xap_QNXDlg_Language.h"
#include "ut_qnxHelper.h"
#include "ut_Xpm2Bitmap.h"
#include "xap_QNXToolbar_Icons.h"

/*****************************************************************/

XAP_Dialog * XAP_QNXDialog_Language::static_constructor(XAP_DialogFactory * pFactory,
													 XAP_Dialog_Id id)
{
	XAP_QNXDialog_Language * p = new XAP_QNXDialog_Language(pFactory,id);
	return p;
}

XAP_QNXDialog_Language::XAP_QNXDialog_Language(XAP_DialogFactory * pDlgFactory,
											   XAP_Dialog_Id id)
	: XAP_Dialog_Language(pDlgFactory,id)
{
	m_pLanguageList = NULL;
}

XAP_QNXDialog_Language::~XAP_QNXDialog_Language(void)
{
}

static int s_delete_clicked(PtWidget_t *w, void *data, PtCallbackInfo_t *info)
{
	XAP_QNXDialog_Language *dlg = (XAP_QNXDialog_Language *)data;
	dlg->setAnswer(XAP_Dialog_Language::a_CANCEL);
	return Pt_CONTINUE;
}

static int s_ok_clicked(PtWidget_t *w, void *data, PtCallbackInfo_t *info)
{
	XAP_QNXDialog_Language *dlg = (XAP_QNXDialog_Language *)data;
	dlg->setAnswer(XAP_Dialog_Language::a_OK);
	return Pt_CONTINUE;
}

static int s_cancel_clicked(PtWidget_t *w, void *data, PtCallbackInfo_t *info)
{
	XAP_QNXDialog_Language *dlg = (XAP_QNXDialog_Language *)data;
	dlg->setAnswer(XAP_Dialog_Language::a_CANCEL);
	return Pt_CONTINUE;
}


void XAP_QNXDialog_Language::runModal(XAP_Frame * pFrame)
{
	UT_ASSERT(pFrame);

	XAP_QNXFrameImpl * pQNXFrameImpl = (XAP_QNXFrameImpl*)pFrame->getFrameImpl();
	PtWidget_t *parent =	pQNXFrameImpl->getTopLevelWindow();	
	UT_ASSERT(parent);

	PtSetParentWidget(parent);

	// build the dialog
	PtWidget_t * window = constructWindow();
	UT_ASSERT(window);
	connectFocus(window, pFrame);

	UT_QNXCenterWindow(parent, window);
	UT_QNXBlockWidget(parent, 1);
	
	// Run the dialog
	PtRealizeWidget(window);

	int count;
	count = PtModalStart();
	done = 0;
	while(!done) {
		PtProcessEvent();
	}
	PtModalEnd(MODAL_END_ARG(count));

	m_bChangedLanguage = false;

	if (m_answer == XAP_Dialog_Language::a_OK) {
		PtTreeItem_t *titem =	PtTreeGetCurrent(m_pLanguageList);

			if (!m_pLanguage || UT_stricmp(m_pLanguage, titem->string)) {
				_setLanguage(titem->string);
				m_bChangedLanguage = true;
			}
	}

	UT_QNXBlockWidget(parent, 0);
	PtDestroyWidget(window);
}

PtWidget_t * XAP_QNXDialog_Language::constructWindow(void)
{
	const XAP_StringSet * pSS = m_pApp->getStringSet();
	PtWidget_t *windowLangSelection;
	PtWidget_t *vboxMain;
	PtWidget_t *vboxOuter;

	PtWidget_t *hboxBut;
	PtWidget_t *buttonOK;
	PtWidget_t *buttonCancel;

	PtArg_t args[10];
	int n;

	n = 0;
PtSetArg(&args[n++], Pt_ARG_WINDOW_TITLE, _(XAP,DLG_ULANG_LangTitle), 0);
    PtSetArg(&args[n++], Pt_ARG_WINDOW_RENDER_FLAGS, 0, ABI_MODAL_WINDOW_RENDER_FLAGS);
    PtSetArg(&args[n++], Pt_ARG_WINDOW_MANAGED_FLAGS, 0, ABI_MODAL_WINDOW_MANAGE_FLAGS);
	windowLangSelection = PtCreateWidget(PtWindow, NULL, n, args);
	PtAddCallback(windowLangSelection, Pt_CB_WINDOW_CLOSING, s_delete_clicked, this);

	n = 0;
	PtSetArg(&args[n++], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_VERTICAL, 0);
	PtSetArg(&args[n++], Pt_ARG_MARGIN_HEIGHT, ABI_MODAL_MARGIN_SIZE, 0); 
	PtSetArg(&args[n++], Pt_ARG_MARGIN_WIDTH, ABI_MODAL_MARGIN_SIZE, 0); 
	PtSetArg(&args[n++], Pt_ARG_GROUP_SPACING_Y, ABI_MODAL_MARGIN_SIZE, 0); 
	PtSetArg(&args[n++], Pt_ARG_GROUP_FLAGS, 
					Pt_GROUP_EQUAL_SIZE_HORIZONTAL,
					Pt_GROUP_EQUAL_SIZE_HORIZONTAL); 
	vboxOuter = PtCreateWidget(PtGroup, windowLangSelection, n, args);


	n = 0;
	PtSetArg(&args[n++], Pt_ARG_GROUP_ORIENTATION, Pt_GROUP_VERTICAL, 0);
	PtSetArg(&args[n++], Pt_ARG_GROUP_FLAGS, 
				Pt_GROUP_STRETCH_VERTICAL | Pt_GROUP_STRETCH_HORIZONTAL,
				Pt_GROUP_STRETCH_VERTICAL | Pt_GROUP_STRETCH_HORIZONTAL);
		PtSetArg(&args[n++],Pt_ARG_HEIGHT,2 * ABI_DEFAULT_BUTTON_WIDTH,0);
		vboxMain =  PtCreateWidget(PtGroup, vboxOuter, n, args);
		pretty_group(vboxMain, _(XAP,DLG_ULANG_LangLabel));

	n = 0;
	PtSetArg(&args[n++],Pt_ARG_TREE_FLAGS,Pt_FALSE,Pt_TREE_SHOW_CONNECTORS);
	m_pLanguageList = PtCreateWidget(PtTree, vboxMain, n, args);

/* Add bitmap into PtTree */
	PhImage_t *pImage = NULL;
	short img,img_id;
	AP_QNXToolbar_Icons icon;

	pImage = icon.getPixmapForIcon("SPELLCHECK");

	img_id=	PtTreeAddImages(m_pLanguageList,pImage,1);

	
	UT_Vector *pVec = getAvailableDictionaries();
	UT_uint32 nItems = pVec->getItemCount();
	
	UT_uint32 k;
	for (k = 0; k < m_iLangCount; k++) {
		const char *item = (const char *) m_ppLanguages[k];
		img=-1;
		for(UT_uint32 i = 0; i < nItems; i++)
		{	
			const char *dic = (const char *) pVec->getNthItem(i);
			if(strcmp(dic,m_ppLanguagesCode[k]) == 0)
			{
				img=img_id;
				break;
			}
		}
		PtTreeItem_t *titem_old=0;
	
		PtTreeItem_t *titem = PtTreeAllocItem(m_pLanguageList,item,img,img);
		if(!titem_old)	PtTreeAddFirst(m_pLanguageList, titem,NULL);
		else PtTreeAddAfter(m_pLanguageList,titem,titem_old);
		if(!UT_stricmp(item,m_pLanguage))
			PtTreeGoto(m_pLanguageList,titem);
		titem_old = titem;
		img = -1;
	}
	FREEP(pImage);
	
	/* Buttons along the bottom */
	n = 0;
	hboxBut =  PtCreateWidget(PtGroup, vboxOuter, n, args);

	n = 0;
	PtSetArg(&args[n++], Pt_ARG_WIDTH, ABI_DEFAULT_BUTTON_WIDTH, 0);
PtSetArg(&args[n++], Pt_ARG_TEXT_STRING, _(XAP,DLG_OK), 0);
	buttonOK = PtCreateWidget(PtButton, hboxBut, n, args);
	PtAddCallback(buttonOK, Pt_CB_ACTIVATE, s_ok_clicked, this);

	n = 0;
	PtSetArg(&args[n++], Pt_ARG_WIDTH, ABI_DEFAULT_BUTTON_WIDTH, 0);
PtSetArg(&args[n++], Pt_ARG_TEXT_STRING, _(XAP,DLG_Cancel), 0);
	buttonCancel = PtCreateWidget(PtButton, hboxBut, n, args);
	PtAddCallback(buttonCancel, Pt_CB_ACTIVATE, s_cancel_clicked, this);

	return windowLangSelection;
}
	
void XAP_QNXDialog_Language::setAnswer(XAP_Dialog_Language::tAnswer ans) { 
	if(!done++) { 
		m_answer = ans; 
	}
}


