/* AbiSource Application Framework
 * Copyright (C) 1998-2000 AbiSource, Inc.
 * Copyright (C) 2001-2022 Hubert Figuière
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

#import <Cocoa/Cocoa.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#include "ut_types.h"
#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_files.h"
#include "ut_sleep.h"
#include "xap_ViewListener.h"
#include "xap_CocoaApp.h"
#include "xap_CocoaFrame.h"
#include "xap_CocoaTimer.h"
#include "ev_CocoaKeyboard.h"
#include "ev_CocoaMouse.h"
#include "ev_CocoaMenuBar.h"
#include "ev_CocoaMenuPopup.h"
#include "ev_CocoaToolbar.h"
#include "ev_EditMethod.h"
#include "xav_View.h"
#include "fv_View.h"
#include "xad_Document.h"
#include "gr_CocoaGraphics.h"
#include "gr_Painter.h"
#include "gr_CoreGraphicsUtils.h"


@implementation XAP_CocoaNSView
- (id)initWith:(XAP_Frame *)frame andFrame:(NSRect)windowFrame andName:(NSString*)name
{
	UT_DEBUGMSG (("Cocoa: @XAP_CocoaNSView initWith:Frame\n"));
	UT_ASSERT (frame);
	if (![super initWithFrame:windowFrame]) {
		return nil;
	}
    m_name = [name retain];
	m_pFrame = frame;
	m_pGR = nullptr;
	m_drawable = nullptr;
	m_layer = nullptr;
	m_layer_needs_resize = false;
	if (frame) {
		[[NSNotificationCenter defaultCenter] addObserver:self
						selector:@selector(hasBeenResized:)
						name:NSViewFrameDidChangeNotification object:self];
	}
	return self;
}

- (void)dealloc
{
	[_eventDelegate release];
	[[NSNotificationCenter defaultCenter] removeObserver:self];
    [m_name release];
	if (m_layer) {
		::CGLayerRelease(m_layer);
	}
	[super dealloc];
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (BOOL)becomeFirstResponder
{
	if (m_pFrame) {
		UT_ASSERT (m_pFrame);
		
		if (m_pFrame->getCurrentView()) {
			m_pFrame->getCurrentView()->focusChange(AV_FOCUS_HERE);
		}
		UT_DEBUGMSG(("became first responder!\n"));
		return YES;
	}
	UT_DEBUGMSG (("refused to become first responder, because gr_Graphics is not setup.\n"));
	return NO;
}

- (XAP_Frame *)xapFrame
{
	return m_pFrame;
}

- (void)setGraphics:(GR_CocoaGraphics *)gr
{
	m_pGR = gr;
}

@synthesize in_draw_rect = _in_draw_rect;
@synthesize drawable = m_drawable;
@synthesize drawingLayer = m_layer;

- (CGLayerRef)makeDrawingLayer
{
	if (m_layer_needs_resize) {
		CGLayerRelease(m_layer);
		m_layer = nil;
	}
	if (m_layer == nil) {
		NSGraphicsContext* context = nil;

		context = [NSGraphicsContext graphicsContextWithWindow:self.window];
		if (!context && _in_draw_rect) {
			context = NSGraphicsContext.currentContext;
		}
		//UT_ASSERT(context);
		if (!context) {
			return nullptr;
		}
		m_layer = ::CGLayerCreateWithContext(context.CGContext, self.bounds.size, nullptr);
		m_layer_needs_resize = false;
	}
	return m_layer;
}

/// Draw the layer in the current context.
- (void)drawLayer
{
	if (m_layer) {
		NSGraphicsContext* gc = [NSGraphicsContext currentContext];
		if (gc) {
			CGContextRef context = gc.CGContext;
			CGSize layerSize = ::CGLayerGetSize(m_layer);
			GR_CGStateSave state(context);
			::CGContextScaleCTM(context, 1.0, -1.0);
			::CGContextTranslateCTM(context, 0, -self.bounds.size.height);
			NSLog(@"layer size W=%lf, H=%lf", layerSize.width, layerSize.height);
			::CGContextDrawLayerAtPoint(context, CGPointMake(0, 0), m_layer);
		}
		if (m_layer_needs_resize) {
			[self makeDrawingLayer];
		} else {
			CGContextRef context = ::CGLayerGetContext(m_layer);
			GR_CGStateSave state(context);
			CGColorRef white = ::CGColorGetConstantColor(kCGColorWhite);
			::CGContextSetFillColorWithColor(context, white);
			::CGContextFillRect(context, {{0.0, 0.0}, ::CGLayerGetSize(m_layer)});
		}
	}
}

- (void)drawRect:(NSRect)aRect
{
	_in_draw_rect = true;
	if (!m_layer) {
		[self makeDrawingLayer];
	}
	if (m_pGR) {
		[self drawLayer];

		UT_RGBColor clr;
		GR_Painter painter(m_pGR);

		GR_CocoaGraphics::_utNSColorToRGBColor([NSColor redColor], clr);
		m_pGR->fillRect (clr, aRect.origin.x, aRect.origin.y, aRect.size.width, aRect.size.height);

		/*  Because of the way we convert from local to display units for scrolling and back again for expose
		 *  events, there can be a sizeable discrepancy (in local units) between internal events and the
		 *  reported exposed area. I am therefore marking an extra 1-pixel border around the exposed display area.
		 */
		aRect.origin.x    += -1.0;
		aRect.origin.y    += -1.0;
		aRect.size.width  +=  2.0;
		aRect.size.height +=  2.0;

		if (m_drawable) {
			m_drawable->drawImmediate();
		} else {
			m_pGR->_callUpdateCallback(&aRect);
		}
	}
	_in_draw_rect = false;
}

/*!
	Cocoa overridden method.

	\return NO. Coordinates are still upside down, but we'll reverse
	the offscreen instead.
*/
- (BOOL)isFlipped
{
	return NO; //GR_CocoaGraphics::_isFlipped();
}

- (BOOL)isOpaque
{
	return YES;
}

/*
- (XAP_Frame *)_getOwnerFrame
{
	id controller = [[self window] delegate];
	if ([controller isKindOfClass:[XAP_CocoaFrameController class]])
	{
		XAP_Frame* frame = [(XAP_CocoaFrameController*)controller frameImpl]->getFrame();
		return frame;
	}
	NSLog (@"-[_getOwnerFrame] could find owner frame");
	return nullptr;
}
*/

- (void)resetCursorRects
{
	[self addCursorRect:[self visibleRect] cursor:_cursor];
	//[_cursor setOnMouseEntered:YES];
}

- (void)setCursor:(NSCursor*)cursor
{
	[_cursor release];
	_cursor = [cursor retain];
}

- (void)setEventDelegate:(NSObject <XAP_MouseEventDelegate>*)delegate
{
	[_eventDelegate release];
	_eventDelegate = [delegate retain];
}

- (void)hasBeenResized:(NSNotification*)notif
{
	UT_UNUSED(notif);
	if (m_pGR && m_pFrame) {
		AV_View* pView = m_pFrame->getCurrentView();
		NSRect rect = self.bounds;
		if (pView && !pView->isLayoutFilling()) {
			pView->setWindowSize((UT_sint32)rint(rect.size.width), (UT_sint32)rint(rect.size.height));
			m_pFrame->quickZoom(); // was update zoom
		}
	}
	if (m_layer) {
		m_layer_needs_resize = true;
	}
}



- (NSObject <XAP_MouseEventDelegate>*)eventDelegate
{
	return _eventDelegate;
}


- (void)mouseDown:(NSEvent *)theEvent
{
	if (NSTextInputContext* inputContext = [NSTextInputContext currentInputContext]) {
		[inputContext.client unmarkText];
	}
	[_eventDelegate mouseDown:theEvent from:self];
}
- (void)rightMouseDown:(NSEvent *)theEvent
{
	if (NSTextInputContext* inputContext = [NSTextInputContext currentInputContext]) {
		[inputContext.client unmarkText];
	}
	[_eventDelegate mouseDown:theEvent from:self];
}
- (void)otherMouseDown:(NSEvent *)theEvent
{
	if (NSTextInputContext* inputContext = [NSTextInputContext currentInputContext]) {
		[inputContext.client unmarkText];
	}
	[_eventDelegate mouseDown:theEvent from:self];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
	[_eventDelegate mouseDragged:theEvent from:self];
}
- (void)rightMouseDragged:(NSEvent *)theEvent
{
	[_eventDelegate mouseDragged:theEvent from:self];
}
- (void)otherMouseDragged:(NSEvent *)theEvent
{
	[_eventDelegate mouseDragged:theEvent from:self];
}

- (void)mouseUp:(NSEvent *)theEvent
{
	[_eventDelegate mouseUp:theEvent from:self];
}
- (void)rightMouseUp:(NSEvent *)theEvent
{
	[_eventDelegate mouseUp:theEvent from:self];
}
- (void)otherMouseUp:(NSEvent *)theEvent
{
	[_eventDelegate mouseUp:theEvent from:self];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
	if(!m_pFrame) {
		return;
	}
	AV_View *pAV_View = m_pFrame->getCurrentView ();

	UT_sint32 deltaX = (UT_sint32)rint([theEvent deltaX]);
	UT_sint32 deltaY = (UT_sint32)rint([theEvent deltaY]);

	if (deltaX < 0)
		pAV_View->cmdScroll(AV_SCROLLCMD_LINERIGHT, m_pGR->tlu(-30*deltaX));
	if (deltaX > 0)
		pAV_View->cmdScroll(AV_SCROLLCMD_LINELEFT,  m_pGR->tlu( 30*deltaX));

	if (deltaY > 0)
		pAV_View->cmdScroll(AV_SCROLLCMD_LINEUP,    m_pGR->tlu( 30*deltaY));
	if (deltaY < 0)
		pAV_View->cmdScroll(AV_SCROLLCMD_LINEDOWN,  m_pGR->tlu(-30*deltaY));
}

@end
