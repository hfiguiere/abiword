/* AbiSource Application Framework
 * Copyright (C) 1998-2001 AbiSource, Inc.
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


#ifndef AV_LISTENER_H
#define AV_LISTENER_H

/* pre-emptive dismissal; ut_types.h is needed by just about everything,
 * so even if it's commented out in-file that's still a lot of work for
 * the preprocessor to do...
 */
#ifndef UT_TYPES_H
#include "ut_types.h"
#endif

class AV_View;

typedef UT_uint32 AV_ListenerId;


typedef enum _AV_ListenerType
{
    AV_LISTENER_MENU,
    AV_LISTENER_TOOLBAR,
    AV_LISTENER_LEFTRULER,
    AV_LISTENER_TOPRULER,
    AV_LISTENER_SCROLLBAR,
    AV_LISTENER_VIEW,
    AV_LISTENER_STATUSBAR,
	AV_LISTENER_CARET
} AV_ListenerType;

// TODO how did we fill this mask so fast?
// TODO next it'll need to become a 32-bit mask
typedef UT_uint16 AV_ChangeMask;
#define AV_CHG_NONE			((AV_ChangeMask) 0x0000)
#define AV_CHG_DO			((AV_ChangeMask) 0x0001)		// canDo
#define AV_CHG_DIRTY		((AV_ChangeMask) 0x0002)		// isDirty
#define AV_CHG_EMPTYSEL		((AV_ChangeMask) 0x0004)		// isSelectionEmpty
#define AV_CHG_FILENAME		((AV_ChangeMask) 0x0008)		// getFilename
#define AV_CHG_FMTBLOCK		((AV_ChangeMask) 0x0010)		// getBlockFormat
#define AV_CHG_FMTCHAR		((AV_ChangeMask) 0x0020)		// getCharFormat
#define AV_CHG_CLIPBOARD	((AV_ChangeMask) 0x0040)
#define AV_CHG_PAGECOUNT	((AV_ChangeMask) 0x0080)		// number of pages
#define AV_CHG_WINDOWSIZE	((AV_ChangeMask) 0x0100)
#define AV_CHG_FMTSECTION	((AV_ChangeMask) 0x0200)
#define AV_CHG_COLUMN		((AV_ChangeMask) 0x0400)
#define AV_CHG_INPUTMODE	((AV_ChangeMask) 0x0800)
#define AV_CHG_FMTSTYLE		((AV_ChangeMask) 0x1000)		// getStyle
#define AV_CHG_INSERTMODE	((AV_ChangeMask) 0x2000)
#define AV_CHG_HDRFTR   	((AV_ChangeMask) 0x4000)
#define AV_CHG_DIRECTIONMODE ((AV_ChangeMask)0x4000)
#define AV_CHG_FRAMEDATA	((AV_ChangeMask) 0x8000)		// frame-level preferences (pFrameData)
#define AV_CHG_ALL			((AV_ChangeMask) 0xFFFF)

#define AV_CHG_SAVE			((AV_ChangeMask) (AV_CHG_DO | AV_CHG_DIRTY | AV_CHG_FILENAME))
#define AV_CHG_TYPING		((AV_ChangeMask) (AV_CHG_DO | AV_CHG_DIRTY | AV_CHG_EMPTYSEL | AV_CHG_COLUMN))
#define AV_CHG_MOTION		((AV_ChangeMask) (AV_CHG_EMPTYSEL | AV_CHG_FMTSTYLE | AV_CHG_FMTBLOCK | AV_CHG_FMTSECTION | AV_CHG_FMTCHAR | AV_CHG_COLUMN))
#define AV_CHG_STYLE_PARA	((AV_ChangeMask)  (AV_CHG_FMTBLOCK | AV_CHG_FMTCHAR))
/*
	Various UI elements (title, toolbar, etc.) need to stay in sync with
	the current state of an AV_View.  They can do so by registering
	an AV_Listener with the AV_View in order to be notified of the existence
	of certain changes to the document as they occur.

	Note that these notifications do *not* pass any document state, they
	simply note the existence of a certain category of changes to that state.

	The view will notify each registered listener (in an undefined order).
	When the listener registers, it is provided an ID which may be used
	later to refer to it.
*/

class AV_Listener
{
public:
	virtual bool		notify(AV_View * pView, const AV_ChangeMask mask) = 0;
	virtual AV_ListenerType    getType(void) = 0;
};

#endif /* AV_LISTENER_H */
