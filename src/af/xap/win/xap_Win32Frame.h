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


#ifndef XAP_WIN32FRAME_H
#define XAP_WIN32FRAME_H

#include <windows.h>
#include "xap_Frame.h"
#include "ut_vector.h"
#include "xap_Win32DialogFactory.h"
#include "xap_Win32DragAndDrop.h"

class XAP_Win32App;
class ev_Win32Keyboard;
class EV_Win32Mouse;
class EV_Win32MenuBar;
class EV_Win32MenuPopup;

/*****************************************************************
******************************************************************
** This file defines the Win32-platform-specific class for the
** cross-platform application frame.  This is used to hold all
** Win32-specific data.  One of these is created for each top-level
** document window.
******************************************************************
*****************************************************************/

class XAP_Win32Frame : public XAP_Frame
{
public:
	XAP_Win32Frame(XAP_Win32App * app);
	XAP_Win32Frame(XAP_Win32Frame * f);
	virtual ~XAP_Win32Frame(void);

	virtual bool				initialize(const char * szKeyBindingsKey, const char * szKeyBindingsDefaultValue,
										   const char * szMenuLayoutKey, const char * szMenuLayoutDefaultValue,
										   const char * szMenuLabelSetKey, const char * szMenuLabelSetDefaultValue,
										   const char * szToolbarLayoutsKey, const char * szToolbarLayoutsDefaultValue,
										   const char * szToolbarLabelSetKey, const char * szToolbarLabelSetDefaultValue);

	virtual	XAP_Frame *			cloneFrame(void)=0;
	virtual	XAP_Frame *			buildFrame(XAP_Frame * pClone)=0;
	virtual UT_Error			loadDocument(const char * szFilename, int ieft)=0;
	virtual bool				close(void);
	virtual bool				raise(void);
	virtual bool				show(void);
	virtual void setFullScreen(bool isFullScreen) {}
	virtual bool				openURL(const char * szURL);
	virtual bool				updateTitle(void);
	virtual UT_sint32			setInputMode(const char * szName);
	virtual void                nullUpdate () const;
	virtual void                setCursor(GR_Graphics::Cursor c);

	HWND						getTopLevelWindow(void) const;
	HWND						getToolbarWindow(void) const;
	EV_Win32Mouse *				getWin32Mouse(void);
	virtual EV_Menu*			getMainMenu();

	virtual XAP_DialogFactory *	getDialogFactory(void);
	virtual void				setXScrollRange(void)=0;
	virtual void				setYScrollRange(void)=0;
	virtual bool				runModalContextMenu(AV_View * pView, const char * szMenuName,
													UT_sint32 x, UT_sint32 y);
	virtual void				translateDocumentToScreen(UT_sint32 &x, UT_sint32 &y) = 0;
	virtual void				setStatusMessage(const char * szMsg) = 0;

	static bool					RegisterClass(XAP_Win32App * app);

	virtual void				toggleRuler(bool bRulerOn) = 0;	
	virtual void				toggleTopRuler(bool bRulerOn) = 0;
	virtual void				toggleLeftRuler(bool bRulerOn) = 0;
	
protected:
	virtual HWND				_createDocumentWindow(HWND hwndParent,
													  UT_uint32 iLeft, UT_uint32 iTop,
													  UT_uint32 iWidth, UT_uint32 iHeight)=0;
	virtual HWND				_createStatusBarWindow(HWND hwndParent,
													  UT_uint32 iLeft, UT_uint32 iTop,
													  UT_uint32 iWidth)=0;
	void						_createTopLevelWindow(void);
	static LRESULT CALLBACK		_FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

  	virtual EV_Toolbar *		_newToolbar(XAP_App *app, XAP_Frame *frame, const char *, const char *);

	// TODO see why ev_Win32Keyboard has lowercase prefix...
	XAP_Win32App *				m_pWin32App;
	EV_Win32MenuBar *			m_pWin32Menu;
	EV_Win32MenuPopup *			m_pWin32Popup; /* only valid while a context popup is up */
	UT_uint32					m_iBarHeight;
	UT_uint32					m_iStatusBarHeight;
	
	HWND						m_hwndFrame; /* the entire window, menu, toolbar, document, etc. */
	HWND						m_hwndRebar;
	HWND						m_hwndContainer; /* the document and all rulers and scroll bars */
	HWND						m_hwndStatusBar;
	
	AP_Win32DialogFactory		m_dialogFactory;

	UINT						m_mouseWheelMessage;

	/* These 2 variables are used in the frame
	** to remember the last values we used to set
	** the size of client area of the frame (on
	** a resize of the frame window).  That is,
	** the values we used to calculate the layout
	** of the various toolbars, view, and anything
	** else that goes in the frame window.  We do
	** this because Win32 plays funny games with
	** window sizes (NonClient vs Client coordinates).
	** We do this so that we can short-circuit some
	** of the resizing and the resulting flashing.
	*/
	UT_uint32					m_iSizeWidth;
	UT_uint32					m_iSizeHeight;
		
	XAP_Win32DropTarget				m_dropTarget;
};

#endif /* XAP_WIN32FRAME_H */
