/* -*- c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */
/* AbiSource Application Framework
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 1999 John Brewer DBA Jera Design
 * Copyright (C) 2000-2001 Hubert Figuiere
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


#ifndef XAP_MACFRAME_H
#define XAP_MACFRAME_H

/* Mac headers */
#include <MacWindows.h>
#include <MacTypes.h>
#include <Controls.h>

#include "xap_Frame.h"
#include "ut_vector.h"
#include "xap_MacDialogFactory.h"
class XAP_MacApp;
class ev_MacKeyboard;
class EV_MacMouse;
class EV_MacMenu;

/*****************************************************************
******************************************************************
** This file defines the Mac-platform-specific class for the
** cross-platform application frame.  This is used to hold all
** Mac-specific data.  One of these is created for each top-level
** document window.
******************************************************************
*****************************************************************/

#define XAP_MACFRAME_WINDOW_KIND 128

class XAP_MacFrame : public XAP_Frame
{
public:
	XAP_MacFrame(XAP_MacApp * app);
	XAP_MacFrame(XAP_MacFrame * f);
	virtual ~XAP_MacFrame(void);

	virtual bool initialize(const char * szKeyBindingsKey, 
				  const char * szKeyBindingsDefaultValue,
				  const char * szMenuLayoutKey, 
				  const char * szMenuLayoutDefaultValue,
				  const char * szMenuLabelSetKey, 
				  const char * szMenuLabelSetDefaultValue,
				  const char * szToolbarLayoutsKey, 
				  const char * szToolbarLayoutsDefaultValue,
				  const char * szToolbarLabelSetKey, 
				  const char * szToolbarLabelSetDefaultValue);

	virtual	XAP_Frame *			cloneFrame(void);
	virtual	XAP_Frame *			buildFrame(XAP_Frame * pClone);
	virtual UT_Error			loadDocument(const char * szFilename,  int ieft)=0;
	virtual bool				close(void);
	virtual bool				raise(void);
	virtual bool				show(void);
	virtual void setFullScreen(bool isFullScreen) {}
	virtual bool				openURL(const char * szURL);
	virtual bool				updateTitle(void);
	virtual UT_sint32			setInputMode(const char * szName);
	virtual void                            nullUpdate () const {}
	virtual void                setCursor(GR_Graphics::Cursor c) {}


	virtual XAP_DialogFactory *	getDialogFactory(void);
	virtual void				setXScrollRange(void);
	virtual void				setYScrollRange(void);
	virtual bool 				runModalContextMenu(AV_View * pView, const char * szMenuName, UT_sint32 x, UT_sint32 y);
	
	virtual void				setStatusMessage(const char * szMsg) { UT_ASSERT (UT_NOT_IMPLEMENTED); };

	virtual void				toggleRuler(bool bRulerOn) { UT_ASSERT (UT_NOT_IMPLEMENTED); };
	virtual void				toggleBar(UT_uint32 iBarNb, bool bBarOn) { UT_ASSERT (UT_NOT_IMPLEMENTED); };
	virtual void				toggleStatusBar(bool bStatusBarOn) { UT_ASSERT (UT_NOT_IMPLEMENTED); };
	virtual bool				getBarVisibility(UT_uint32 iBarNb) { UT_ASSERT (UT_NOT_IMPLEMENTED); };


	virtual void				queue_resize() { UT_ASSERT (UT_NOT_IMPLEMENTED); };

	EV_MacMenu					*getMenu () { return m_pMacMenu; };

    virtual bool				_macUpdate(void);
    virtual bool				_macGrow(void);
	UT_uint32					_getToolbarCount (void) { return m_vecToolbarLayoutNames.getItemCount(); };
	WindowPtr					getMacWindow (void) { UT_ASSERT (m_MacWindow != NULL); return m_MacWindow; } ;
	GrafPtr						getMacWindowPort (void) { UT_ASSERT (m_MacWindowPort != NULL); return m_MacWindowPort; } ;
    void						_getStatusBarRect (Rect & rect) { rect = m_placardRect; };
	ControlHandle               _getRootControl ()
		{ return m_rootControl; };
	short                       _getVisibleRgnTop ()
		{ return m_visibleRgnTop; };
	void                        _setVisibleRgnTop (short top);
	virtual EV_Menu*			getMainMenu();
#if defined(USE_CARBON_EVENTS)
	static pascal OSStatus HandleCarbonWindowEvent (EventHandlerCallRef nextHandler, 
	                                          EventRef theEvent, void* userData);
#endif

protected:
	virtual EV_Toolbar *		_newToolbar(XAP_App *app, XAP_Frame *frame, const char *, const char *);

	void						_createTopLevelWindow(void);
    virtual void				_createStatusBar(void) = 0;
	void						_calcVertScrollBarRect(Rect & rect);
	void						_calcHorizScrollBarRect(Rect & rect);
	void						_calcPlacardRect ();
	virtual void				_createDocumentWindow(void) = 0;
private:
	WindowPtr					m_MacWindow;
    GrafPtr						m_MacWindowPort;
//	Rect 						m_winBounds;
    AP_MacDialogFactory			m_dialogFactory;
        
    ev_MacKeyboard				*m_pKeyboard;
    EV_MacMenu					*m_pMacMenu;
    EV_MacMouse					*m_pMouse;
	void						MacWindowInit ();

	short                       m_visibleRgnTop;
	ControlHandle               m_rootControl;
	Rect						m_placardRect;
};

#endif /* XAP_MACFRAME_H */
