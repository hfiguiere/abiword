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

#ifndef AP_DIALOG_LISTS_H
#define AP_DIALOG_LISTS_H

#include "xap_Frame.h"
#include "xap_Dialog.h"
#include "xav_View.h"
#include "fl_BlockLayout.h"
#include "fl_AutoNum.h"
#include "fl_AutoLists.h"
#include "pt_Types.h"
#include "xap_Preview.h"

class FV_View;
class fl_AutoNum;
class fl_BlockLayout;
class AP_Preview_Paragraph;
class AP_Dialog_Lists;


class AP_Lists_preview : public XAP_Preview
{
public:

	AP_Lists_preview(GR_Graphics * gc, AP_Dialog_Lists * pLists );
	virtual ~AP_Lists_preview(void);

	// data twiddlers
	void				draw(void);
	AP_Dialog_Lists*	getLists(void);
	void				setData(XML_Char * pszFont,float fAlign,float fIndent);


protected:

	AP_Dialog_Lists*	m_pLists;
	XML_Char			m_pszFont[80];
	float				m_fAlign;
	float				m_fIndent;
	UT_sint32			m_iLine_pos[8];
	UT_sint32			m_iLine_height;
	bool				m_bFirst;
};
		
class AP_Dialog_Lists : public XAP_Dialog_Modeless
{
	friend class AP_Preview_Paragraph;
	friend class AP_Preview_Paragraph_Block;

public:
	AP_Dialog_Lists(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~AP_Dialog_Lists(void);

	// these are used for the Modal version of the dialog called from the
    // styles dialog.
	typedef enum {
		a_OK,
		a_QUIT,
		a_CLOSE
	} tAnswer;

	AP_Dialog_Lists::tAnswer	getAnswer(void) const;
	void						ConstructWindowName(void);
	void						StartList(void);
	void						StopList(void);
	void						Apply(void);
	void						fillDialogFromBlock(void);
	void						fillDialogFromVector(UT_Vector * inVec);
	void						PopulateDialogData(void);
	void						fillFakeLabels(void);
	bool						isLastOnLevel(void);
	XML_Char *					getListStyleString( UT_uint32 iListType);
	UT_uint32					decodeListType(char * listformat);
	UT_sint32					findVecItem(UT_Vector * v, char * key);
	void						fillUncustomizedValues(void);
	UT_uint32					getID(void);
	UT_uint32					getStoredID(void) { return m_iID;}
	fl_AutoNum *				getAutoNum(void);
	fl_BlockLayout *			getBlock(void);
	UT_uint32					getTick(void);
	const UT_Vector *			getOutProps(void) const { return &m_OutProps;}
	void						setTick(UT_uint32 iTick);
	bool						isDirty(void) const {return m_bDirty;}
	void						setDirty(void) {m_bDirty = true;}
	void						clearDirty(void) {m_bDirty = false;}

	bool						setView(FV_View * view);
	FV_View *					getView(void);
	AV_View *					getAvView(void);
	void						setActiveFrame(XAP_Frame *pFrame);
	void						generateFakeLabels(void);
	UT_UCSChar *				getListLabel(UT_sint32 itemNo);
	virtual void 				event_PreviewAreaExposed();
	virtual void 				_createPreviewFromGC(GR_Graphics * gc, UT_uint32 width, UT_uint32 height);
	void						setModal(void) {m_bIsModal = true;}
	bool						isModal(void) const { return m_bIsModal;}
protected:

	// These are the "current use" dialog data items,
	// which are liberally read and set by the
	// accessor methods above.
	FV_View*				m_pView;
	// is this used in a modeless dialog like this?
	//
	// These will all be rationalized after windows and beos get the
	// new dialog

	tAnswer					m_answer;
	char					m_WindowName[100];
	bool					m_isListAtPoint;
	bool					m_previousListExistsAtPoint;
	char					m_curListType[100];
	UT_UCSChar				m_curListLabel[100];
	UT_UCSChar				m_newListLabel[100];
	List_Type				m_newListType;
	XML_Char				m_pszDelim[80];
	XML_Char				m_pszDecimal[80];
	XML_Char				m_pszFont[80];
	float					m_fAlign;
	float					m_fIndent;
	UT_uint32				m_iLevel;
	UT_uint32				m_iStartValue;

	UT_uint32				m_iWidth;
	UT_uint32				m_iHeight;
	UT_uint32				m_iLocalTick;
	UT_uint32				m_curStartValue;
	UT_uint32				m_newStartValue;
	UT_uint32				m_curListLevel;
	UT_uint32				m_newListLevel;
	UT_uint32				m_iID;
	List_Type				m_iListType;

	bool					m_bStartList;
	bool					m_bStopList;
	bool					m_bChangeStartValue;
	bool					m_bresumeList;

	bool					m_bStartNewList;
	bool					m_bApplyToCurrent;
	bool					m_bStartSubList;
	bool					m_bResumeList;
	bool					m_bisCustomized;
	bool					m_bguiChanged;

	AP_Preview_Paragraph*	m_paragraphPreview;
	AP_Lists_preview*		m_pListsPreview;

	fl_Layout*				m_pFakeLayout[4];
	PL_StruxDocHandle		m_pFakeSdh[4];
	fl_AutoNum*				m_pFakeAuto;
	PD_Document *			m_pFakeDoc;
private:
	virtual const XML_Char*	_getDingbatsFontName() const;
	bool					m_bDirty;
	bool					m_bIsModal;
	UT_Vector				m_OutProps;
	UT_String				m_Output[5];
};


#endif /* AP_DIALOG_LISTS_H */






