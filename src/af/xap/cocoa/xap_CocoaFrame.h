/* AbiSource Application Framework
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


#ifndef XAP_COCOAFRAME_H
#define XAP_COCOAFRAME_H

#import <AppKit/AppKit.h>

#include "xap_Frame.h"
#include "ut_vector.h"
#include "xap_CocoaDialogFactory.h"

class XAP_CocoaApp;
class EV_CocoaKeyboard;
class EV_CocoaMouse;
class EV_CocoaMenuBar;
class EV_CocoaMenuPopup;

/*****************************************************************
******************************************************************
** This file defines the unix-platform-specific class for the
** cross-platform application frame.  This is used to hold all
** unix-specific data.  One of these is created for each top-level
** document window.
******************************************************************
*****************************************************************/

class XAP_CocoaFrame : public XAP_Frame
{
public:
	XAP_CocoaFrame(XAP_CocoaApp * app);
	XAP_CocoaFrame(XAP_CocoaFrame * f);
	virtual ~XAP_CocoaFrame();

	virtual bool				initialize(const char * szKeyBindingsKey, const char * szKeyBindingsDefaultValue,
										   const char * szMenuLayoutKey, const char * szMenuLayoutDefaultValue,
										   const char * szMenuLabelSetKey, const char * szMenuLabelSetDefaultValue,
										   const char * szToolbarLayoutsKey, const char * szToolbarLayoutsDefaultValue,
										   const char * szToolbarLabelSetKey, const char * szToolbarLabelSetDefaultValue);

	virtual	XAP_Frame *			cloneFrame() = 0;
	virtual UT_Error   			loadDocument(const char * szFilename, int ieft) = 0;
	virtual UT_Error                        loadDocument(const char * szFilename, int ieft, bool createNew) = 0;
	virtual bool				close();
	virtual bool				raise();
	virtual bool				show();
	virtual bool				openURL(const char * szURL);
	virtual bool				updateTitle();
	virtual UT_sint32			setInputMode(const char * szName);
	virtual void                            nullUpdate () const;

	NSWindow *					getTopLevelWindow() const;
	NSView *					getVBoxWidget() const;
	virtual XAP_DialogFactory *	getDialogFactory();
	virtual void				setXScrollRange() = 0;
	virtual void				setYScrollRange() = 0;
	virtual bool				runModalContextMenu(AV_View * pView, const char * szMenuName,
													UT_sint32 x, UT_sint32 y);
	virtual void				translateDocumentToScreen(UT_sint32 &x, UT_sint32 &y) = 0;
	virtual void				setStatusMessage(const char * szMsg) = 0;

	void						setTimeOfLastEvent(unsigned long eventTime);
	
	virtual void				toggleRuler(bool bRulerOn) = 0;
	virtual void				queue_resize();
	virtual EV_Menu*			getMainMenu();
	virtual void                rebuildMenus(void);
    virtual void                rebuildToolbar(UT_uint32 ibar);
protected:
	virtual NSWindow *			_createDocumentWindow() = 0;
	virtual NSView *			_createStatusBarWindow() = 0;
	virtual void				_createTopLevelWindow();
	virtual void				_setWindowIcon() = 0;

	virtual EV_Toolbar *		_newToolbar(XAP_App *app, XAP_Frame *frame, const char *, const char *);

private:
	XAP_CocoaApp *				m_pCocoaApp;
	EV_CocoaMenuBar *			m_pCocoaMenu;
	EV_CocoaMenuPopup *			m_pCocoaPopup; /* only valid while a context popup is up */
	
	NSWindow *					m_wTopLevelWindow;
	NSView *					m_wVBox;
	NSView * 				m_wSunkenBox;
	NSView *					m_wStatusBar;
	unsigned int                       m_iAbiRepaintID;
	AP_CocoaDialogFactory		m_dialogFactory;
};

#endif /* XAP_COCOAFRAME_H */
