/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2001, 2003 Hubert Figuiere
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

#ifndef AP_CocoaDialog_Columns_H
#define AP_CocoaDialog_Columns_H

#include "ap_Dialog_Columns.h"
#include "gr_CocoaGraphics.h"

#include "ut_types.h"
#include "ut_string.h"

class XAP_CocoaFrame;
@class AP_CocoaDialog_ColumnsController;

/*****************************************************************/

class AP_CocoaDialog_Columns: public AP_Dialog_Columns
{
public:
	AP_CocoaDialog_Columns(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id dlgid);
	virtual ~AP_CocoaDialog_Columns(void);

	virtual void			runModal(XAP_Frame * pFrame);
	virtual void			enableLineBetweenControl(bool bState = true);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id dlgid);

	// callbacks can fire these events
    void                            doSpaceAfterEntry(void);
	void                            doMaxHeightEntry(const char * s);
    void                            doHeightSpin(void);
	void                            doSpaceAfterSpin(void);
	void                            checkLineBetween(void);
	void							 colNumberChanged(void);
	void                            readSpin(void);
	void                            event_Toggle( UT_uint32 icolumns);
	void                            event_previewExposed(void);
	void			event_OK(void);
	void			event_Cancel(void);

private:
	GR_CocoaGraphics	* 		m_pPreviewWidget;
	AP_CocoaDialog_ColumnsController *m_dlg;
};


@interface AP_CocoaDialog_ColumnsController : NSWindowController <XAP_CocoaDialogProtocol>
{
    IBOutlet NSButton *_cancelBtn;
    IBOutlet NSButton *_lineBetweenBtn;
    IBOutlet NSTextField *_maxColSizeData;
    IBOutlet NSTextField *_maxColSizeLabel;
    IBOutlet NSStepper *_maxColSizeStepper;
    IBOutlet NSTextField *_numColumn2Label;
    IBOutlet NSTextField *_numColumnLabel;
    IBOutlet NSTextField *_numOfColumnData;
    IBOutlet NSStepper *_numOfColumnStepper;
    IBOutlet NSButton *_okBtn;
    IBOutlet NSButton *_oneBtn;
    IBOutlet XAP_CocoaNSView *_preview;
    IBOutlet NSBox *_previewBox;
    IBOutlet NSStepper *_spaceAfterColStepper;
    IBOutlet NSTextField *_spaceAfterColData;
    IBOutlet NSTextField *_spaceAfterColLabel;
    IBOutlet NSButton *_threeBtn;
    IBOutlet NSButton *_twoBtn;
    IBOutlet NSButton *_useRTLBtn;
	AP_CocoaDialog_Columns	*_xap;
}
- (IBAction)cancelAction:(id)sender;
- (IBAction)lineBetweenAction:(id)sender;
- (IBAction)maxColSizeAction:(id)sender;
- (IBAction)maxColSizeStepperAction:(id)sender;
- (IBAction)numOfColAction:(id)sender;
- (IBAction)numOfColStepperAction:(id)sender;
- (IBAction)okAction:(id)sender;
- (IBAction)oneAction:(id)sender;
- (IBAction)spaceAfterColAction:(id)sender;
- (IBAction)spaceAfterColStepperAction:(id)sender;
- (IBAction)threeAction:(id)sender;
- (IBAction)twoAction:(id)sender;

- (UT_uint32)colNum;
- (void)setColNum:(UT_uint32)num;

- (NSString*)spaceAfter;
- (void)setSpaceAfter:(const char *)str;
- (void)setMaxColHeight:(const char *)str;
- (bool)lineBetween;
- (void)setLineBetween:(bool)b;
- (UT_uint32)columnRTLOrder;
- (void)setColumnRTLOrder:(UT_uint32)val;

- (XAP_CocoaNSView*)preview;

@end

#endif /* AP_CocoaDialog_Columns_H */






