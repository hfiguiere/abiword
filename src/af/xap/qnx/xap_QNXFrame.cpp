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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ut_types.h"
#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "xap_ViewListener.h"
#include "xap_QNXApp.h"
#include "xap_QNXFrame.h"
#include "ev_QNXKeyboard.h"
#include "ev_QNXMouse.h"
#include "ev_QNXMenu.h"
#include "ev_QNXToolbar.h"
#include "ev_EditMethod.h"
#include "xav_View.h"
#include "fv_View.h"
#include "xad_Document.h"
#include "ut_qnxHelper.h"
#include "gr_Graphics.h"

//QNX DND
#include "fl_DocLayout.h"
#include "xap_EncodingManager.h"
#include "ie_impexp_Register.h"
#include "ie_imp.h"

/* This is required for dynamic zoom implimentation */
#include "fv_View.h"

//THIS IS BAD!!!
#include "ap_FrameData.h"

/*****************************************************************/

#define REPLACEP(p,q)	do { if (p) delete p; p = q; } while (0)
#define ENSUREP(p)		do { UT_ASSERT(p); if (!p) goto Cleanup; } while (0)

/****************************************************************/
int XAP_QNXFrame::_fe::focus_in_event(PtWidget_t *w, void *data, PtCallbackInfo_t *info) 
{
	XAP_QNXFrame * pFrame = (XAP_QNXFrame *)data;
	UT_ASSERT(pFrame);
	/*
 	pFrame->getCurrentView()->focusChange( gtk_grab_get_current() == NULL || 
											 gtk_grab_get_current() == w ? AV_FOCUS_HERE 
																		 : AV_FOCUS_NEARBY);
	*/
	//I don't understand the AV_FOCUS_HERE vs NEARBY
        if (pFrame->getCurrentView())
            pFrame->getCurrentView()->focusChange(AV_FOCUS_HERE/*AV_FOCUS_NEARBY*/);
					
	return Pt_CONTINUE;
}

int XAP_QNXFrame::_fe::focus_out_event(PtWidget_t *w, void *data, PtCallbackInfo_t *info) 
{
	XAP_QNXFrame * pFrame = (XAP_QNXFrame *)data;
	UT_ASSERT(pFrame);
	if(pFrame->getCurrentView())	
		pFrame->getCurrentView() ->focusChange(AV_FOCUS_NONE);
	return Pt_CONTINUE;
}


int XAP_QNXFrame::_fe::button_press_event(PtWidget_t* w, void *data, PtCallbackInfo_t* info)
{
	XAP_QNXFrame * pQNXFrame = (XAP_QNXFrame *)data;
	AV_View * pView = pQNXFrame->getCurrentView();
	EV_QNXMouse * pQNXMouse = (EV_QNXMouse *) pQNXFrame->getMouse();


	if (pView)
		pQNXMouse->mouseClick(pView,info);
	return 0;
}

int XAP_QNXFrame::_fe::button_release_event(PtWidget_t* w, void *data, PtCallbackInfo_t* info)
{
	XAP_QNXFrame * pQNXFrame = (XAP_QNXFrame *)data;
	AV_View * pView = pQNXFrame->getCurrentView();
	EV_QNXMouse * pQNXMouse = (EV_QNXMouse *) pQNXFrame->getMouse();

	if (pView)
		pQNXMouse->mouseUp(pView,info);
	return 0;
}

int XAP_QNXFrame::_fe::motion_notify_event(PtWidget_t* w, void *data, PtCallbackInfo_t* info)
{
	XAP_QNXFrame * pQNXFrame = (XAP_QNXFrame *)data;
	AV_View * pView = pQNXFrame->getCurrentView();
	EV_QNXMouse * pQNXMouse = (EV_QNXMouse *) pQNXFrame->getMouse();

	if (pView)
		pQNXMouse->mouseMotion(pView, info);
	
	return 0;
}
	
int XAP_QNXFrame::_fe::key_press_event(PtWidget_t* w, void *data, PtCallbackInfo_t* info)
{
	XAP_QNXFrame * pQNXFrame = (XAP_QNXFrame *)data;
	AV_View * pView = pQNXFrame->getCurrentView();
	ev_QNXKeyboard * pQNXKeyboard = (ev_QNXKeyboard *) pQNXFrame->getKeyboard();
		
	if (pView)
		pQNXKeyboard->keyPressEvent(pView, info);

	return 0;
}
	
int XAP_QNXFrame::_fe::resize(PtWidget_t * w, void *data, PtCallbackInfo_t *info)
{
	PtContainerCallback_t *cbinfo = (PtContainerCallback_t *)(info->cbdata);

	XAP_QNXFrame * pQNXFrame = (XAP_QNXFrame *)data;
	AV_View * pView = pQNXFrame->getCurrentView();
	//FV_View * pfView = static_cast<FV_View*>(pView);

	if (pView) {
		UT_DEBUGMSG(("Document Area Resizing to %d,%d %d,%d ",
			cbinfo->new_size.ul.x, cbinfo->new_size.ul.y,
			cbinfo->new_size.lr.x, cbinfo->new_size.lr.y));
		pView->setWindowSize(cbinfo->new_size.lr.x - cbinfo->new_size.ul.x, 
				             cbinfo->new_size.lr.y - cbinfo->new_size.ul.y);
	}

	// Dynamic Zoom Implimentation
	pQNXFrame->updateZoom();

	return Pt_CONTINUE;
}

int XAP_QNXFrame::_fe::window_resize(PtWidget_t * w, void *data, PtCallbackInfo_t *info)
{
	PtContainerCallback_t *cbinfo = (PtContainerCallback_t *)(info->cbdata);

	XAP_QNXApp * pQNXApp = (XAP_QNXApp *)data;

	if (pQNXApp) {
		UT_DEBUGMSG(("Window Resizing to %d,%d %d,%d ",
			cbinfo->new_size.ul.x, cbinfo->new_size.ul.y,
			cbinfo->new_size.lr.x, cbinfo->new_size.lr.y));
		pQNXApp->setGeometry(-1, -1, 
						   cbinfo->new_size.lr.x - cbinfo->new_size.ul.x, 
				           cbinfo->new_size.lr.y - cbinfo->new_size.ul.y);
	}

	return Pt_CONTINUE;
}
		
int XAP_QNXFrame::_fe::window_delete(PtWidget_t *w, void *data, PtCallbackInfo_t *info)
{
	PhWindowEvent_t *winevent = (PhWindowEvent_t *)info->cbdata;

	if(!winevent || winevent->event_f != Ph_WM_CLOSE) {
		return Pt_CONTINUE;
	}

	XAP_QNXFrame * pQNXFrame = (XAP_QNXFrame *)data; 
	XAP_App * pApp = pQNXFrame->getApp();
	UT_ASSERT(pApp);

	const EV_EditMethodContainer * pEMC = pApp->getEditMethodContainer();
	UT_ASSERT(pEMC);
	
	const EV_EditMethod * pEM = pEMC->findEditMethodByName("closeWindowX");
	UT_ASSERT(pEM);

	if (pEM)
	{
		if ((*pEM->getFn())(pQNXFrame->getCurrentView(),NULL))
		{
			//Destroy this window.
			PtDestroyWidget(w);
		}
	}
		
	//Do not destroy this window.
	return Pt_CONTINUE;
}
	
int XAP_QNXFrame::_fe::expose(PtWidget_t * w, PhTile_t * damage)
{
	PtArg_t args[1];
	UT_Rect rClip;
	PhRect_t rect;

   	PhPoint_t pnt;
/*
   	PtSuperClassDraw(PtBasic, w, damage);
*/
   	PtBasicWidgetCanvas(w, &rect);
   	PtWidgetOffset(w, &pnt);
/*
	UT_DEBUGMSG(("-----\nWidget Rect is %d,%d  %d,%d (@ %d,%d)",
			rect.ul.x, rect.ul.y, rect.lr.x, rect.lr.y, pnt.x, pnt.y));
*/
	XAP_QNXFrame *pQNXFrame, **ppQNXFrame = NULL;
	PtSetArg(&args[0], Pt_ARG_USER_DATA, &ppQNXFrame, 0);
	PtGetResources(w, 1, args);
	pQNXFrame = (ppQNXFrame) ? *ppQNXFrame : NULL;

	UT_ASSERT(pQNXFrame);
	
	AV_View * pView = pQNXFrame->getCurrentView();
	if (pView) {
		/*
   		 The first damage rect spans all of the damage areas, so
   		 do it piecewise if we can over the areas that need it.

		 The damage rectangles are provided in the windows'
		 co-ordinates, so make sure to translate them to the
		 widgets co-ordinates when passing to our draw() functions
		*/
/*
 When Abi's Draw routine gets to be faster about not going through
 and calculating areas which don't need to be done, then we can
 actually take advantage of the multiple clip areas.  For now though
 we just do it based on the first clip.
#define MULTIPLE_EXPOSE_EVENTS
*/
		if (damage->next) {
			UT_DEBUGMSG(("Multiple damage rects "));
#if defined(MULTIPLE_EXPOSE_EVENTS) 
			damage = damage->next;
#endif
		}
		while (damage) {
/*
			UT_DEBUGMSG(("Expose Rect is %d,%d  %d,%d ",
			damage->rect.ul.x, damage->rect.ul.y, damage->rect.lr.x, damage->rect.lr.y));
*/
			/* At one point in time this required some fiddling to put it in the widget co-ordinates*/
			rClip.width = (damage->rect.lr.x - damage->rect.ul.x) + 1;
			rClip.height = (damage->rect.lr.y - damage->rect.ul.y) + 1;
			rClip.left = damage->rect.ul.x - pnt.x;
			rClip.top = damage->rect.ul.y - pnt.y;

			UT_DEBUGMSG(("Adjusted Expose Rect %d,%d %d/%d ",
				rClip.left, rClip.top, rClip.width, rClip.height));
				
			//Don't bother setting the clip here, the Graphics routine does it

			//OR: Pass the draw function the clipping rectangle
			//This is preferred since this way the application
			//can optimize their drawing routines as well.
			pView->draw(&rClip);

			//OR: Completely unoptimized
			//pView->draw(NULL);
			//break;

#if defined(MULTIPLE_EXPOSE_EVENTS) 
			damage = damage->next;
#else
			break;
#endif
		}

	}
	UT_DEBUGMSG(("====="));

	return 0;
}
	
int XAP_QNXFrame::_fe::vScrollChanged(PtWidget_t * w, void *data, PtCallbackInfo_t *info)
{
	PtScrollbarCallback_t *sb = (PtScrollbarCallback_t *)info->cbdata;
	XAP_QNXFrame * pQNXFrame = (XAP_QNXFrame *)data;
	AV_View * pView = pQNXFrame->getCurrentView();

	if (pView)
		pView->sendVerticalScrollEvent((UT_sint32) sb->position);
	return 0;
}
	
int XAP_QNXFrame::_fe::hScrollChanged(PtWidget_t * w, void *data, PtCallbackInfo_t *info)
{
	PtScrollbarCallback_t *sb = (PtScrollbarCallback_t *)info->cbdata;
	XAP_QNXFrame * pQNXFrame = (XAP_QNXFrame *)data;
	AV_View * pView = pQNXFrame->getCurrentView();
	
	if (pView)
		pView->sendHorizontalScrollEvent((UT_sint32) sb->position);
	return 0;
}

//QNX DnD
static PtDndFetch_t acceptdata[] = {
	{"text","plain",Ph_TRANSPORT_INLINE,},
};

enum {
PLAIN_TEXT = 0,
};

int XAP_QNXFrame::_fe::dnd(PtWidget_t *w,void *data,PtCallbackInfo_t *info)
{
XAP_QNXFrame *pQNXFrame = (XAP_QNXFrame *)data;
PtDndCallbackInfo_t *dc = (PtDndCallbackInfo_t *)info->cbdata;
IE_Imp *pImp = 0;


if(info->reason_subtype == Ph_EV_DND_ENTER)
{
PtDndSelect(w,acceptdata,1,0,0,info);
return Pt_CONTINUE;
}


if(info->reason_subtype == Ph_EV_DND_DROP)
{
	switch(dc->fetch_index)
	{
		case PLAIN_TEXT:
			AP_FrameData *pFrameData = (AP_FrameData*)pQNXFrame->getFrameData();
			FL_DocLayout *pDocLy = pFrameData->m_pDocLayout;
			FV_View *pView = pDocLy->getView();
			PD_DocumentRange dr(pView->getDocument(),pView->getPoint(),pView->getPoint());
	
			IE_Imp::constructImporter(dr.m_pDoc,0,IE_Imp::fileTypeForSuffix(".txt"),&pImp,0);
			if(pImp)
			{
				const char * szEncoding = XAP_EncodingManager::get_instance()->getNative8BitEncodingName();
				pImp->pasteFromBuffer(&dr,(unsigned char *)dc->data,strlen((const char*)dc->data),szEncoding);
				delete pImp;
				pView->_generalUpdate();
			}
				break;
	
		}
}

return Pt_CONTINUE;
}	
/*****************************************************************/

/*
XAP_QNXFrame::XAP_QNXFrame(XAP_QNXApp * app)
	: XAP_Frame(static_cast<XAP_App *>(app)),
*/
XAP_QNXFrame::XAP_QNXFrame(XAP_QNXApp * app)
	: XAP_Frame((XAP_App *)(app)),
	  m_dialogFactory(this, (XAP_App *)(app))
{
	m_pQNXApp = app;
	m_pQNXMenu = NULL;
	m_pQNXPopup = NULL;
	m_pView = NULL;
}

// TODO when cloning a new frame from an existing one
// TODO should we also clone any frame-persistent
// TODO dialog data ??

/*
XAP_QNXFrame::XAP_QNXFrame(XAP_QNXFrame * f)
	: XAP_Frame(static_cast<XAP_Frame *>(f)),
	  m_dialogFactory(this, static_cast<XAP_App *>(f->m_pQNXApp))
*/
XAP_QNXFrame::XAP_QNXFrame(XAP_QNXFrame * f)
	: XAP_Frame((XAP_Frame *)(f)),
	  m_dialogFactory(this, (XAP_App *)(f->m_pQNXApp))
{
	m_pQNXApp = f->m_pQNXApp;
	m_pQNXMenu = NULL;
	m_pQNXPopup = NULL;
	m_pView = NULL;
}

XAP_QNXFrame::~XAP_QNXFrame(void)
{
	// only delete the things we created...
	
	DELETEP(m_pQNXMenu);
	DELETEP(m_pQNXPopup);
}

bool XAP_QNXFrame::initialize(const char * szKeyBindingsKey, const char * szKeyBindingsDefaultValue,
								  const char * szMenuLayoutKey, const char * szMenuLayoutDefaultValue,
								  const char * szMenuLabelSetKey, const char * szMenuLabelSetDefaultValue,
								  const char * szToolbarLayoutsKey, const char * szToolbarLayoutsDefaultValue,
								  const char * szToolbarLabelSetKey, const char * szToolbarLabelSetDefaultValue)
{
	bool bResult;

	// invoke our base class first.
	
	bResult = XAP_Frame::initialize(szKeyBindingsKey, szKeyBindingsDefaultValue,
									szMenuLayoutKey, szMenuLayoutDefaultValue,
									szMenuLabelSetKey, szMenuLabelSetDefaultValue,
									szToolbarLayoutsKey, szToolbarLayoutsDefaultValue,
									szToolbarLabelSetKey, szToolbarLabelSetDefaultValue);
	UT_ASSERT(bResult);

	// get a handle to our keyboard binding mechanism
	// and to our mouse binding mechanism.

	EV_EditEventMapper * pEEM = getEditEventMapper();
	UT_ASSERT(pEEM);

	m_pKeyboard = new ev_QNXKeyboard(pEEM);
	UT_ASSERT(m_pKeyboard);
	
	m_pMouse = new EV_QNXMouse(pEEM);
	UT_ASSERT(m_pMouse);

	return true;
}

void XAP_QNXFrame::setCursor(GR_Graphics::Cursor c)
{
unsigned short cursor_number=0;

if(getTopLevelWindow() == NULL || (m_iFrameMode != XAP_NormalFrame))
	return;
	switch (c)
	{
	default:
		UT_ASSERT(UT_NOT_IMPLEMENTED);
		/*FALLTHRU*/
	case GR_Graphics::GR_CURSOR_DEFAULT:
		cursor_number = Ph_CURSOR_POINTER;
		break;

	case GR_Graphics::GR_CURSOR_IBEAM:
		cursor_number = Ph_CURSOR_POINTER; //XXX: Wtf is IBEAM ?
		break;

	case GR_Graphics::GR_CURSOR_RIGHTARROW:
		cursor_number = Ph_CURSOR_DRAG_RIGHT;
		break;

	case GR_Graphics::GR_CURSOR_LEFTARROW:
		cursor_number = Ph_CURSOR_DRAG_LEFT; //GDK_ARROW;
		break;

	case GR_Graphics::GR_Graphics::GR_CURSOR_IMAGE:
		cursor_number = Ph_CURSOR_POINTER; //XXX: ???
		break;

	case GR_Graphics::GR_CURSOR_IMAGESIZE_NW:
		cursor_number = Ph_CURSOR_DRAG_TL;
		break;

	case GR_Graphics::GR_CURSOR_IMAGESIZE_N:
		cursor_number = Ph_CURSOR_DRAG_TOP;
		break;

	case GR_Graphics::GR_CURSOR_IMAGESIZE_NE:
		cursor_number = Ph_CURSOR_DRAG_TR;
		break;

	case GR_Graphics::GR_Graphics::GR_CURSOR_IMAGESIZE_E:
		cursor_number = Ph_CURSOR_DRAG_RIGHT;
		break;

	case GR_Graphics::GR_CURSOR_IMAGESIZE_SE:
		cursor_number = Ph_CURSOR_DRAG_BR;
		break;

	case GR_Graphics::GR_CURSOR_IMAGESIZE_S:
		cursor_number = Ph_CURSOR_DRAG_BOTTOM;
		break;

	case GR_Graphics::GR_CURSOR_IMAGESIZE_SW:
		cursor_number = Ph_CURSOR_DRAG_BL;
		break;

	case GR_Graphics::GR_Graphics::GR_CURSOR_IMAGESIZE_W:
		cursor_number = Ph_CURSOR_DRAG_LEFT;
		break;

	case GR_Graphics::GR_CURSOR_LEFTRIGHT:
		cursor_number=Ph_CURSOR_DRAG_HORIZONTAL;
		break;
	case GR_Graphics::GR_CURSOR_UPDOWN:
		cursor_number=Ph_CURSOR_DRAG_VERTICAL;
		break;
	case GR_Graphics::GR_CURSOR_EXCHANGE:
		cursor_number=Ph_CURSOR_POINTER;
		break;
	case GR_Graphics::GR_CURSOR_GRAB:
		cursor_number=Ph_CURSOR_MOVE;
		break;
	case GR_Graphics::GR_CURSOR_LINK:
		cursor_number=Ph_CURSOR_FINGER;
		break;
	case GR_Graphics::GR_CURSOR_WAIT:
		cursor_number= Ph_CURSOR_WAIT;
		break;
	}

	PtSetResource(getTopLevelWindow(),Pt_ARG_CURSOR_TYPE,cursor_number,0);
	PtSetResource(m_wSunkenBox,Pt_ARG_CURSOR_TYPE,cursor_number,0);
	PtSetResource(m_wStatusBar,Pt_ARG_CURSOR_TYPE,cursor_number,0);
}


UT_sint32 XAP_QNXFrame::setInputMode(const char * szName)
{
	UT_sint32 result = XAP_Frame::setInputMode(szName);
	if (result == 1)
	{
		// if it actually changed we need to update keyboard and mouse

		EV_EditEventMapper * pEEM = getEditEventMapper();
		UT_ASSERT(pEEM);

		m_pKeyboard->setEditEventMap(pEEM);
		m_pMouse->setEditEventMap(pEEM);
	}

	return result;
}

PtWidget_t * XAP_QNXFrame::getTopLevelWindow(void) const
{
	return m_wTopLevelWindow;
}

PtWidget_t * XAP_QNXFrame::getVBoxWidget(void) const
{
	return m_wVBox;
}

PtWidget_t * XAP_QNXFrame::getTBGroupWidget(void) const
{
	return m_wTBGroup;
}

GR_Graphics * XAP_QNXFrame::getGraphics() {
	return(((AP_FrameData*)m_pData)->m_pG);
}


XAP_DialogFactory * XAP_QNXFrame::getDialogFactory(void)
{
	return &m_dialogFactory;
}

void XAP_QNXFrame::nullUpdate() const
{
        #define EVENT_SIZE sizeof(PhEvent_t) + 1000
   
	UT_uint32 i =0;
	PhEvent_t *event = (PhEvent_t*)malloc(EVENT_SIZE);
        if (!event) return;

	while(i < 5)
	{
		switch(PhEventPeek(event, EVENT_SIZE))
		{
		case Ph_EVENT_MSG:
			PtEventHandler( event );
			break;
		case -1:
			perror( "PhEventPeek failed" );
			break;
		}
		i++;
	}
        free(event);
}

/*
 This is all based on the GTK version from unix
*/
void XAP_QNXFrame::_createTopLevelWindow(void)
{
	bool bResult;
	PtArg_t args[10];
	int 	n;
	UT_uint32	w, h;
	PhArea_t area;

#define INIT_WIDTH 500
#define INIT_HEIGHT 400
	/*** Create the main window ***/
	//ndim.w = m_geometry.width; ndim.h = m_geometry.height;
	void *data = this;
	area.pos.x = 0; area.pos.y = 0;
	area.size.w = INIT_WIDTH; area.size.h = INIT_HEIGHT;

	//If it is available then get the default geometry
	if(m_pQNXApp && m_pQNXApp->getGeometry(NULL, NULL, &w, &h) == true) {
		area.size.w = w;
		area.size.h = h;
	} 

	m_AvailableArea = area;

	n = 0;
	PtSetArg(&args[n++], Pt_ARG_DIM, &area.size, 0);
	PtSetArg(&args[n++], Pt_ARG_WINDOW_TITLE, m_pQNXApp->getApplicationTitleForTitleBar(), 0);
	PtSetArg(&args[n++], Pt_ARG_USER_DATA, &data, sizeof(this));
	PtSetArg(&args[n++], Pt_ARG_WINDOW_MANAGED_FLAGS, 0, Ph_WM_CLOSE);
	PtSetArg(&args[n++], Pt_ARG_WINDOW_NOTIFY_FLAGS, Ph_WM_CLOSE, Ph_WM_CLOSE);
	/*
	PtSetArg(&args[n++], Pt_ARG_WINDOW_STATE, 0, Ph_WM_STATE_ISFRONT);
	*/
	//m_wTopLevelWindow = PtAppInit(NULL, NULL, NULL, 2, args);
	PtSetParentWidget(NULL);
	m_wTopLevelWindow = PtCreateWidget(PtWindow, NULL /* Use last widget? */, n, args);
	if (!m_wTopLevelWindow) {
		fprintf(stderr, "Can't create top level window \n");
		exit(1);
	}
	//PtAddEventHandler(m_wTopLevelWindow, Ph_EV_KEY, _fe::key_press_event, this);
	PtAddCallback(m_wTopLevelWindow, Pt_CB_GOT_FOCUS, _fe::focus_in_event, this);
	PtAddCallback(m_wTopLevelWindow, Pt_CB_LOST_FOCUS, _fe::focus_out_event, this);
	PtAddCallback(m_wTopLevelWindow, Pt_CB_RESIZE, _fe::window_resize, m_pQNXApp);
	PtAddCallback(m_wTopLevelWindow, Pt_CB_WINDOW, _fe::window_delete, this);

	/* TODO: Menu and the Toolbars all go into the same Toolbar "group" */
	n = 0;
#define _A_TBGRP (Pt_LEFT_ANCHORED_LEFT | Pt_RIGHT_ANCHORED_RIGHT | Pt_TOP_ANCHORED_TOP)
    PtSetArg(&args[n++], Pt_ARG_ANCHOR_FLAGS, _A_TBGRP, _A_TBGRP);
    PtSetArg(&args[n++], Pt_ARG_RESIZE_FLAGS, 0, Pt_RESIZE_X_BITS);
    PtSetArg(&args[n++], Pt_ARG_WIDTH, area.size.w, 0); 
	m_wTBGroup = PtCreateWidget(PtToolbarGroup, m_wTopLevelWindow, n, args);

	/*** Create the menu bars ***/
	m_pQNXMenu = new EV_QNXMenuBar(m_pQNXApp, this, m_szMenuLayoutName, m_szMenuLabelSetName);
	UT_ASSERT(m_pQNXMenu);
	bResult = m_pQNXMenu->synthesizeMenuBar();
	UT_ASSERT(bResult);

	/*** Create the tool bars ***/
	_createToolbars();

	/*** Figure out the height to adjust by ***/
	unsigned short tbheight;	
	UT_QNXGetWidgetArea(m_wTBGroup, NULL, NULL, NULL, &tbheight);
#define PHOTON_TOOLBAR_EXTENT_WEIRDNESS
#if defined(PHOTON_TOOLBAR_EXTENT_WEIRDNESS)
	tbheight += 4; 	
#endif
	m_AvailableArea.pos.y += tbheight;
	m_AvailableArea.size.h -= tbheight;

	// Let the app-specific frame code create the contents of
	// the child area of the window (between the toolbars and
	// the status bar).
	
	/* Peel off some space at the bottom for the status bar */
	m_AvailableArea.size.h -= 24;

	/*** Add the document view ***/	
	m_wSunkenBox = _createDocumentWindow();

	//Add status bars
	m_wStatusBar = _createStatusBarWindow();
	
	//Set the icon for the window
	_setWindowIcon();

	// Set geometry 
	int x,y;
	UT_uint32 width,height;
	UT_uint32 f;

	m_pQNXApp->getWinGeometry(&x,&y,&width,&height,&f);
	
	//Get fall-back defaults from pref
	UT_uint32 pref_flags, pref_width, pref_height;
	UT_sint32 pref_x, pref_y;
	m_pQNXApp->getPrefs()->getGeometry(&pref_x, &pref_y, &pref_width,
										 &pref_height, &pref_flags);
	if (!(f & XAP_QNXApp::GEOMETRY_FLAG_SIZE)
	&& (pref_flags & PREF_FLAG_GEOMETRY_SIZE))
	{
		width = pref_width;
		height = pref_height;
		f |= XAP_QNXApp::GEOMETRY_FLAG_SIZE;
	}
	if (!(f & XAP_QNXApp::GEOMETRY_FLAG_POS)
		&& (pref_flags & PREF_FLAG_GEOMETRY_POS))
	{
		x = pref_x;
		y = pref_y;
	f |= XAP_QNXApp::GEOMETRY_FLAG_POS;
	}
// Set the size if requested.
	if(f & XAP_QNXApp::GEOMETRY_FLAG_SIZE)
	{
			PhDim_t dim;
			dim.w = width;
			dim.h = height;
		
			PtSetResource(m_wTopLevelWindow,Pt_ARG_DIM,&dim,0);		
	}
	//Only set the pos on the first window.
	if(m_pQNXApp->getFrameCount() <= 1)
	{
		if( f & XAP_QNXApp::GEOMETRY_FLAG_POS)
		{
			PhPoint_t pos;
			pos.x=x;
			pos.y=y;
			PtSetResource(m_wTopLevelWindow,Pt_ARG_POS,&pos,0);
		}
	}
	//Remember the settings for next time
	m_pQNXApp->getPrefs()->setGeometry(x,y,width,height,f);

return;
}


bool XAP_QNXFrame::close()
{
	PtDestroyWidget(getTopLevelWindow());
	return true;
}

bool XAP_QNXFrame::raise()
{
	if (getTopLevelWindow())
		PtWindowToFront(getTopLevelWindow());
	return true;
}

bool XAP_QNXFrame::show()
{
	raise();
	return true;
}

bool XAP_QNXFrame::openURL(const char * szURL)
{
	spawnlp(P_NOWAITO,"voyager","voyager","-u",szURL,NULL);
	return true;
}

bool XAP_QNXFrame::updateTitle()
{
	if (!XAP_Frame::updateTitle())
	{
		// no relevant change, so skip it
		return false;
	}

	char buf[256];
	buf[0] = 0;

	const char * szAppName = m_pQNXApp->getApplicationTitleForTitleBar();

	int len = 256 - strlen(szAppName) - 4;
	
	const char * szTitle = getTitle(len);

	sprintf(buf, "%s - %s", szTitle, szAppName);

	PtArg_t args[1];

	PtSetArg(&args[0], Pt_ARG_WINDOW_TITLE, buf, 0); 
	PtSetResources(getTopLevelWindow(), 1, args);
	
	return true;
}

/*****************************************************************/
bool XAP_QNXFrame::runModalContextMenu(AV_View * /* pView */, const char * szMenuName,
										   UT_sint32 x, UT_sint32 y)
{

	bool bResult = true;
	UT_ASSERT(!m_pQNXPopup);

	setPopupDone(0);	
	m_pQNXPopup = new EV_QNXMenuPopup(m_pQNXApp, this, 
									  szMenuName, 
									  m_szMenuLabelSetName);
	if (m_pQNXPopup && m_pQNXPopup->synthesizeMenuPopup())
	{
		PtArg_t	arg;
		PhPoint_t pos;

		PtWidget_t *menu = m_pQNXPopup->getMenuHandle();
		PtRealizeWidget(menu);

		memset(&pos, 0, sizeof(pos));
		PtGetAbsPosition(m_wSunkenBox, &pos.x, &pos.y);
		pos.x += x; pos.y += y;

		PtSetArg(&arg, Pt_ARG_POS, &pos, 0);
		PtSetResources(menu, 1, &arg);

		//Really we need to run this synchronously ... or at least
		//be able to provide some sort of handler that blocks the
		//window and then unblocks it when we are finished with 
		//the menu. This is why we do the "getPopupDone" test, set by the menu
		int level;
		level = PtModalStart();
		while (getPopupDone() == 0) {
			PtProcessEvent();
		}	
		PtModalEnd(MODAL_END_ARG(level));
	}
	DELETEP(m_pQNXPopup);

	return bResult;
}

void XAP_QNXFrame::setTimeOfLastEvent(unsigned int eventTime)
{
//	m_pQNXApp->setTimeOfLastEvent(eventTime);
}

EV_Toolbar * XAP_QNXFrame::_newToolbar(XAP_App *app, XAP_Frame *frame,
					const char *szLayout,
					const char *szLanguage)
{
	return (new EV_QNXToolbar((XAP_QNXApp *)(app), 
							  (XAP_QNXFrame *)(frame), szLayout, szLanguage));
}

EV_Menu* XAP_QNXFrame::getMainMenu()
{
	return m_pQNXMenu;
}
