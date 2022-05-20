/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2001-2022 Hubert Figuiere
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#pragma once

#import <Cocoa/Cocoa.h>

class XAP_CocoaApp;
class XAP_Frame;
class AP_CocoaFrame;
class FL_DocLayout;
class GR_Graphics;

#include "ap_Frame.h"
#include "ie_types.h"
#include "ut_assert.h"


/*****************************************************************/

class AP_CocoaFrame : public AP_Frame
{
public:
	AP_CocoaFrame();
	AP_CocoaFrame(AP_CocoaFrame * f);
	virtual ~AP_CocoaFrame(void);

	virtual bool initialize(XAP_FrameMode frameMode = XAP_NormalFrame) override;
	virtual	XAP_Frame* cloneFrame(void) override;

	virtual void setXScrollRange(void) override;
	virtual void setYScrollRange(void) override;
	virtual void				translateDocumentToScreen(UT_sint32 &x, UT_sint32 &y);
	virtual void setStatusMessage(const char* szMsg) override;

	virtual void toggleRuler(bool bRulerOn) override;
	virtual void toggleTopRuler(bool bRulerOn) override;
	virtual void toggleLeftRuler(bool bRulerOn) override;
	virtual void toggleBar(UT_uint32 iBarNb, bool bBarOn) override;
	virtual void toggleStatusBar(bool bStatusBarOn) override;

protected:
	virtual bool _createViewGraphics(GR_Graphics*& pG, UT_uint32 iZoom) override;

	static void					_scrollFuncX(void * pData, UT_sint32 xoff, UT_sint32 xlimit);
	static void					_scrollFuncY(void * pData, UT_sint32 yoff, UT_sint32 ylimit);
	virtual bool _createScrollBarListeners(AV_View * pView, AV_ScrollObj *& pScrollObj,
				       ap_ViewListener *& pViewListener,
				       ap_Scrollbar_ViewListener *& pScrollbarViewListener,
				       AV_ListenerId &lid,
				       AV_ListenerId &lidScrollbarViewListener) override;

	virtual void _bindToolbars(AV_View *pView) override;
	virtual void _setViewFocus(AV_View *pView) override;

	virtual UT_sint32 _getDocumentAreaWidth() override;
	virtual UT_sint32 _getDocumentAreaHeight() override;
	void 						_getHScrollValues (UT_sint32 &min, UT_sint32 &max, UT_sint32 &current);
	void 						_getVScrollValues (UT_sint32 &min, UT_sint32 &max, UT_sint32 &current);
	void 						_setHScrollValues (UT_sint32 min, UT_sint32 max, UT_sint32 current);
	void 						_setVScrollValues (UT_sint32 min, UT_sint32 max, UT_sint32 current);
};
