/* -*- c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */
/* AbiSource Program Utilities
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

#include <Appearance.h>
#include <ControlDefinitions.h>

#include <Controls.h>

#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ev_MacToolbar.h"
#include "xap_MacApp.h"
#include "xap_MacFrame.h"
#include "xap_MacToolbar_Control.h"
#include "ev_Toolbar_Actions.h"
#include "ev_Toolbar_Layouts.h"
#include "ev_Toolbar_Labels.h"
#include "ev_Toolbar_Control.h"
#include "ev_EditEventMapper.h"
#include "xap_MacTlbr_Icons.h"
#include "ev_MacTlbr_ViewListener.h"
#include "xav_View.h"


/*****************************************************************/

EV_MacToolbar::EV_MacToolbar(XAP_MacApp * pMacApp, XAP_MacFrame * pMacFrame,
								 const char * szToolbarLayoutName,
								 const char * szToolbarLabelSetName)
	: EV_Toolbar(pMacApp->getEditMethodContainer(),
				 szToolbarLayoutName,
				 szToolbarLabelSetName)
{
	m_pMacApp = pMacApp;
	m_pMacFrame = pMacFrame;
	m_pViewListener = NULL;
	m_lid = 0;							// view listener id
	m_MacWindow = pMacFrame->getMacWindow ();
	m_hMainControl = NULL;
}

EV_MacToolbar::~EV_MacToolbar(void)
{
    _releaseListener();
	//	UT_VECTOR_PURGEALL(ControlHandle, m_vecToolbarWidgets);
}

bool EV_MacToolbar::toolbarEvent(XAP_Toolbar_Id id,
									  UT_UCSChar * pData,
									  UT_uint32 dataLength)
{
	// user selected something from this toolbar.
	// invoke the appropriate function.
	// return true iff handled.

	const EV_Toolbar_ActionSet * pToolbarActionSet = m_pMacApp->getToolbarActionSet();
	UT_ASSERT(pToolbarActionSet);

	const EV_Toolbar_Action * pAction = pToolbarActionSet->getAction(id);
	if (!pAction)
		return false;

	AV_View * pView = m_pMacFrame->getCurrentView();

#if 0
	// make sure we ignore presses on "down" group buttons
	if (pAction->getItemType() == EV_TBIT_GroupButton)
	{
		const char * szState = 0;
		EV_Toolbar_ItemState tis = pAction->getToolbarItemState(pView,&szState);

		if (EV_TIS_ShouldBeToggled(tis))
		{
			// if this assert fires, you got a click while the button is down
			// if your widget set won't let you prevent this, handle it here
			UT_ASSERT(UT_TODO);
			
			// can safely ignore this event
			return true;
		}
	}
#endif 

	const char * szMethodName = pAction->getMethodName();
	if (!szMethodName)
		return false;
	
	const EV_EditMethodContainer * pEMC = m_pMacApp->getEditMethodContainer();
	UT_ASSERT(pEMC);

	EV_EditMethod * pEM = pEMC->findEditMethodByName(szMethodName);
	UT_ASSERT(pEM);						// make sure it's bound to something

	invokeToolbarMethod(pView,pEM,pData,dataLength);
	return true;
}

/*****************************************************************/

bool EV_MacToolbar::synthesize(void)
{
	OSErr err;
	ControlHandle rootControl;
	XAP_MacToolbar_Control *appWideControl;
	Rect toolbarRect;
	const UInt16 BTN_WIDTH = XAP_MacToolbar_Control::getButtonWidth ();
	const UInt16 BTN_HEIGHT = XAP_MacToolbar_Control::getButtonHeight ();
	const UInt16 BTN_SPACE = XAP_MacToolbar_Control::getButtonSpace ();

    short btnX = BTN_SPACE;
	appWideControl = ((XAP_MacApp*)m_pMacFrame->getApp())->getToolbarControl();
    WindowPtr owningWin = appWideControl->getWindow();
    UT_ASSERT (owningWin);
	// create a toolbar from the info provided.

	UT_DEBUGMSG(("EV_MacToolbar::synthesize() called !\n"));
	const EV_Toolbar_ActionSet * pToolbarActionSet = m_pMacApp->getToolbarActionSet();
	UT_ASSERT(pToolbarActionSet);
	
	UT_uint32 nrLabelItemsInLayout = m_pToolbarLayout->getLayoutItemCount();
	UT_ASSERT(nrLabelItemsInLayout > 0);

	XAP_Toolbar_ControlFactory * pFactory = m_pMacApp->getControlFactory();
	UT_ASSERT(pFactory);
    
    Rect btnRect;

    /* create the toolbar */
	appWideControl->requestToolbarRect (toolbarRect);
	err = ::GetRootControl(owningWin, &rootControl);
	UT_ASSERT (err == noErr);
	if (err) {
		UT_DEBUGMSG(("GetRootControl failed: %d\n", err));
	}
	::CreateWindowHeaderControl (owningWin, &toolbarRect, false, &m_hMainControl);
	UT_ASSERT (m_hMainControl);
	::HideControl (m_hMainControl);
	err = ::EmbedControl (m_hMainControl, rootControl);
	UT_ASSERT (err == noErr);
	if (err) {
		UT_DEBUGMSG(("EmbedControl failed: %d\n", err));
	}

   	////////////////////////////////////////////////////////////////
	// get toolbar button appearance from the preferences
	////////////////////////////////////////////////////////////////
	

#if 0
	const XML_Char * szValue = NULL;
	m_pUnixApp->getPrefsValue(XAP_PREF_KEY_ToolbarAppearance,&szValue);
	UT_ASSERT((szValue) && (*szValue));

    // handle toolbar style later.
	GtkToolbarStyle style = GTK_TOOLBAR_ICONS;
	if (UT_XML_stricmp(szValue,"icon")==0)
		style = GTK_TOOLBAR_ICONS;
	else if (UT_XML_stricmp(szValue,"text")==0)
		style = GTK_TOOLBAR_TEXT;
	else if (UT_XML_stricmp(szValue,"both")==0)
		style = GTK_TOOLBAR_BOTH;
#endif

	for (UT_uint32 k=0; (k < nrLabelItemsInLayout); k++)
	{
        ControlHandle control;
		ControlButtonContentInfo info;
		PicHandle iconHandle;
        const char * szToolTip;
        Rect btnRect;
		bool	bFoundIcon;
        
		EV_Toolbar_LayoutItem * pLayoutItem = m_pToolbarLayout->getLayoutItem(k);
		UT_ASSERT(pLayoutItem);

		XAP_Toolbar_Id id = pLayoutItem->getToolbarId();
		EV_Toolbar_Action * pAction = pToolbarActionSet->getAction(id);
		UT_ASSERT(pAction);
		EV_Toolbar_Label * pLabel = m_pToolbarLabelSet->getLabel(id);
		UT_ASSERT(pLabel);
        
		switch (pLayoutItem->getToolbarLayoutFlags()) {
		case EV_TLF_Normal:
            control = NULL;
            szToolTip = pLabel->getToolTip();
			if (!szToolTip || !*szToolTip)
				szToolTip = pLabel->getStatusMsg();
                
   			switch (pAction->getItemType()) {
			case EV_TBIT_ColorFore:
			case EV_TBIT_ColorBack:
				UT_DEBUGMSG(("TODO: Hey Mac needs some tender love and care and a colour selector! \n"));
				//UT_ASSERT(UT_NOT_IMPLEMENTED);
				UT_DEBUGMSG(("TODO: Handle the colour selector case \n"));
				break;

			case EV_TBIT_PushButton:
                // get pixmap
				bFoundIcon = m_pMacToolbarIcons->getBitmapForIcon(pLabel->getIconName(),
												  &iconHandle);
				UT_ASSERT(bFoundIcon);
                // build control
                btnRect.top = toolbarRect.top + BTN_SPACE;
                btnRect.left = btnX;
                btnRect.bottom = btnRect.top + BTN_HEIGHT;
                btnRect.right = btnX + BTN_WIDTH;
                
				xxx_UT_DEBUGMSG (("Toolbar: new push button at %d, %d, %d, %d\n", btnRect.top, btnRect.left, btnRect.bottom, btnRect.right));
                control = ::NewControl (owningWin, &btnRect, "\p", true, 0, 0, 1, kControlBevelButtonNormalBevelProc, 0);
				UT_ASSERT (control);
				err = ::EmbedControl (control, m_hMainControl);
				UT_ASSERT (err == noErr);
				// set its icon
				info.contentType = kControlContentPictHandle;
				info.u.picture = iconHandle;
				// find why the iconHandle is bogus and makes Carbon crashing
				err = ::SetControlData (control, kControlEntireControl, kControlBevelButtonContentTag, 
				                      sizeof (info), &info);
				UT_ASSERT (err == noErr);
				err = ::SetControlCommandID (control, id);
				UT_ASSERT (err == noErr);
				btnX += BTN_WIDTH + BTN_SPACE;
                break;
			case EV_TBIT_ToggleButton:
			case EV_TBIT_GroupButton:
                // get pixmap
				bFoundIcon = m_pMacToolbarIcons->getBitmapForIcon(pLabel->getIconName(),
												  &iconHandle);
				UT_ASSERT(bFoundIcon);
                // build control
                btnRect.top = toolbarRect.top + BTN_SPACE;
                btnRect.left = btnX;
                btnRect.bottom = btnRect.top + BTN_HEIGHT;
                btnRect.right = btnX + BTN_WIDTH;
                
				xxx_UT_DEBUGMSG (("Toolbar: new group button at %d, %d, %d, %d\n", btnRect.top, btnRect.left, btnRect.bottom, btnRect.right));
                control = ::NewControl (owningWin, &btnRect, "\p", true, 0, 0, 1, kControlBevelButtonNormalBevelProc, 0);
				err = ::EmbedControl (control, m_hMainControl);
				UT_ASSERT (err == noErr);
               				// set its icon
				info.contentType = kControlContentPictHandle;
				info.u.picture = iconHandle;
				// find why the iconHandle is bogus and makes Carbon crashing
				err = SetControlData (control, kControlEntireControl, kControlBevelButtonContentTag, 
				                      sizeof (info), &info);
				UT_ASSERT (err == noErr);
				err = ::SetControlCommandID (control, id);
				UT_ASSERT (err == noErr);
				btnX += BTN_WIDTH + BTN_SPACE;
                break;
			case EV_TBIT_EditText:
				break;
					
			case EV_TBIT_DropDown:
				break;

			case EV_TBIT_ComboBox:
			{
				EV_Toolbar_Control * pControl = pFactory->getControl(this, id);
				UT_ASSERT(pControl);
				int iWidth = 100;

				if (pControl)
				{
					iWidth = pControl->getPixelWidth();
				}

                //
                // Really special as Combo Box does NOT exists in MacOS.
                // Use popup menu instead. This will be the same as combo boxes are not editable in AbiWord
                btnRect.top = toolbarRect.top + BTN_SPACE;
                btnRect.left = btnX;
                btnRect.bottom = btnRect.top + BTN_HEIGHT;
                btnRect.right = btnX + iWidth;
                
				static short menuID = 10000;
				menuID++;						//FIXME be smarter on this
				MenuHandle theMenu = NewMenu(menuID, "\pFonts");
				::InsertMenu (theMenu, -1);
				// TODO actually use popup.
				control = ::NewControl(owningWin, &btnRect, "\p", true, menuID,
							kControlBehaviorCommandMenu + kControlContentTextOnly, 0,
							kControlBevelButtonNormalBevelProc, 0);
                // control = ::NewControl (owningWin, &btnRect, "\p", true, 0, 0, 1, kControlBevelButtonSmallBevelProc, 0);
				err = ::EmbedControl (control, m_hMainControl);
				UT_ASSERT (err == noErr);
				err = ::SetControlCommandID (control, id);
				UT_ASSERT (err == noErr);
				btnX += iWidth + BTN_SPACE;
				if (pControl)
				{
					pControl->populate();

					const UT_Vector * v = pControl->getContents();
					UT_ASSERT(v);

					if (v)
					{
						UT_uint32 items = v->getItemCount();
						for (UT_uint32 m=0; m < items; m++)
						{
							char * sz = (char *)v->getNthItem(m);
							//GtkWidget * li = gtk_list_item_new_with_label(sz);
							//gtk_widget_show(li);
							//gtk_container_add (GTK_CONTAINER(GTK_COMBO(comboBox)->list), li);
						}
					}
				}
                break;
			}
            case EV_TBIT_StaticLabel:
				// TODO do these...
				UT_DEBUGMSG (("Unimplemented Static label\n"));
				break;
					
			case EV_TBIT_Spacer:
				// TODO really put some visual spacer. Purely cosmetic.
				xxx_UT_DEBUGMSG (("Putting spacer\n"));
				btnX += BTN_SPACE * 2;
				break;
					
			case EV_TBIT_BOGUS:
			default:
				UT_ASSERT(0);
				break;
			}
            m_vecToolbarWidgets.addItem (control);
            break;
        case EV_TLF_Spacer:
            // offset the buttons.
            m_vecToolbarWidgets.addItem (NULL);
            break;
        default:
            UT_ASSERT (0);
        }
    }
	return true;
}


bool EV_MacToolbar::bindListenerToView(AV_View * pView)
{
    _releaseListener();
	
	m_pViewListener = new EV_MacToolbar_ViewListener(this,pView);
	UT_ASSERT(m_pViewListener);

	bool bResult = pView->addListener(static_cast<AV_Listener *>(m_pViewListener),&m_lid);
	UT_ASSERT(bResult);

	if (pView->isDocumentPresent())
		refreshToolbar(pView, AV_CHG_ALL);
    return bResult;
}

// FIXIT: move to XP
void EV_MacToolbar::_releaseListener(void) 
{
	if (!m_pViewListener)
		return;
	DELETEP(m_pViewListener);
	m_pViewListener = NULL;
}



WindowPtr EV_MacToolbar::getWindow(void) const
{
	return m_MacWindow;
}

bool EV_MacToolbar::refreshToolbar(AV_View * pView, AV_ChangeMask mask)
{
	// make the toolbar reflect the current state of the document
	// at the current insertion point or selection.
	
	const EV_Toolbar_ActionSet * pToolbarActionSet = m_pMacApp->getToolbarActionSet();
	UT_ASSERT(pToolbarActionSet);
	
	UT_uint32 nrLabelItemsInLayout = m_pToolbarLayout->getLayoutItemCount();
	for (UT_uint32 k=0; (k < nrLabelItemsInLayout); k++)
	{
		EV_Toolbar_LayoutItem * pLayoutItem = m_pToolbarLayout->getLayoutItem(k);
		UT_ASSERT(pLayoutItem);

		XAP_Toolbar_Id id = pLayoutItem->getToolbarId();
		EV_Toolbar_Action * pAction = pToolbarActionSet->getAction(id);
		UT_ASSERT(pAction);

		AV_ChangeMask maskOfInterest = pAction->getChangeMaskOfInterest();
		if ((maskOfInterest & mask) == 0)					// if this item doesn't care about
			continue;										// changes of this type, skip it...

		switch (pLayoutItem->getToolbarLayoutFlags())
		{
		case EV_TLF_Normal:
			{
				_refreshItem(pView, pAction, k);
			}
			break;
			
		case EV_TLF_Spacer:
			break;
			
		default:
			UT_ASSERT(0);
			break;
		}
	}

	return true;
}

bool EV_MacToolbar::_refreshID(XAP_Toolbar_Id id)
{
	const EV_Toolbar_ActionSet * pToolbarActionSet = m_pMacApp->getToolbarActionSet();
	UT_ASSERT(pToolbarActionSet);

	EV_Toolbar_Action * pAction = pToolbarActionSet->getAction(id);
	UT_ASSERT(pAction);

	AV_View * pView = m_pMacFrame->getCurrentView();
	UT_ASSERT(pView);

	return _refreshItem(pView, pAction, id);
}

bool EV_MacToolbar::_refreshItem(AV_View * pView, const EV_Toolbar_Action * pAction, UT_uint32 k)
{
    bool bGrayed;
    bool bToggled;
    OSStatus err;
	ControlHandle	control;
	const char * szState = 0;
	EV_Toolbar_ItemState tis = pAction->getToolbarItemState(pView,&szState);

    switch (pAction->getItemType()) {
        case EV_TBIT_PushButton:
        case EV_TBIT_ComboBox:
            bGrayed = EV_TIS_ShouldBeGray(tis);

            control = (ControlHandle) m_vecToolbarWidgets.getNthItem(k);
            UT_ASSERT(control != NULL);
                
            // Disable/enable toolbar item
            if (bGrayed) {
                err = DeactivateControl (control);
            }
            else {
                err = ActivateControl (control);
            }
                
            //UT_DEBUGMSG(("refreshToolbar: PushButton [%s] is %s\n",
            //			 m_pToolbarLabelSet->getLabel(id)->getToolbarLabel(),
            //			 ((bGrayed) ? "disabled" : "enabled")));
            break;
        
        case EV_TBIT_ToggleButton:
        case EV_TBIT_GroupButton:
        {
            bGrayed = EV_TIS_ShouldBeGray(tis);
            bToggled = EV_TIS_ShouldBeToggled(tis);

            control = (ControlHandle) m_vecToolbarWidgets.getNthItem(k);
            UT_ASSERT(control);

            // Block the signal, throw the toggle event
            SetControlValue (control, (bToggled ? 1 : 0));

            // Disable/enable toolbar item
            if (bGrayed) {
                err = DeactivateControl (control);
            }
            else {
                err = ActivateControl (control);
            }
                
            //UT_DEBUGMSG(("refreshToolbar: ToggleButton [%s] is %s and %s\n",
            //			 m_pToolbarLabelSet->getLabel(id)->getToolbarLabel(),
            //			 ((bGrayed) ? "disabled" : "enabled"),
            //			 ((bToggled) ? "pressed" : "not pressed")));
        }
        break;

        case EV_TBIT_EditText:
            break;
        case EV_TBIT_DropDown:
            break;
        case EV_TBIT_StaticLabel:
            break;
        case EV_TBIT_Spacer:
            break;
        case EV_TBIT_BOGUS:
            break;
        default:
            UT_ASSERT(0);
            break;
    }

	return true;
}

bool EV_MacToolbar::getToolTip(long lParam)
{
        UT_ASSERT (UT_NOT_IMPLEMENTED); 

#if 0
	UT_ASSERT(lParam);
	LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT) lParam;

	// who's asking?
	UINT idButton = lpttt->hdr.idFrom;
	XAP_Toolbar_Id id = ItemIdFromWmCommand(idButton);
	
	EV_Toolbar_Label * pLabel = m_pToolbarLabelSet->getLabel(id);
	if (!pLabel)
		return false;

	// ok, gotcha
	const char * szToolTip = pLabel->getToolTip();
	if (!szToolTip || !*szToolTip)
	{
		szToolTip = pLabel->getStatusMsg();
	}

	// here 'tis
	strncpy(lpttt->lpszText, szToolTip, 80);
#endif // 0

	return true;
}

