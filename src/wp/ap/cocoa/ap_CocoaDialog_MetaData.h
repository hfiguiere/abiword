/* AbiWord
 * Copyright (C) 2000 AbiSource, Inc.
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

#ifndef AP_COCOADIALOG_METADATA_H
#define AP_COCOADIALOG_METADATA_H

#include "ap_Dialog_MetaData.h"

class XAP_CocoaFrame;

@interface AP_CocoaDialog_MetadataController : NSWindowController
{
    IBOutlet NSTextField *m_authorData;
    IBOutlet NSTextField *m_authorLabel;
    IBOutlet NSTextField *m_categoryData;
    IBOutlet NSTextField *m_categoryLabel;
    IBOutlet NSTextField *m_coAuthorData;
    IBOutlet NSTextField *m_coAuthorLabel;
    IBOutlet NSTextField *m_coverageData;
    IBOutlet NSTextField *m_coverageLabel;
    IBOutlet NSTextField *m_descriptionData;
    IBOutlet NSTextField *m_descriptionLabel;
    IBOutlet NSTextField *m_keywordData;
    IBOutlet NSTextField *m_keywordLabel;
    IBOutlet NSTextField *m_languageData;
    IBOutlet NSTextField *m_languageLabel;
    IBOutlet NSTextField *m_publisherData;
    IBOutlet NSTextField *m_publisherLabel;
    IBOutlet NSTextField *m_relationData;
    IBOutlet NSTextField *m_relationLabel;
    IBOutlet NSTextField *m_rightsData;
    IBOutlet NSTextField *m_rightsLabel;
    IBOutlet NSTextField *m_sourceData;
    IBOutlet NSTextField *m_sourceLabel;
    IBOutlet NSTextField *m_subjectData;
    IBOutlet NSTextField *m_subjectLabel;
    IBOutlet NSTextField *m_titleData;
    IBOutlet NSTextField *m_titleLabel;
}
- (IBAction)cancelBtnAction:(id)sender;
- (IBAction)fieldEdited:(id)sender;
- (IBAction)okBtnAction:(id)sender;
@end

/*****************************************************************/

class AP_CocoaDialog_MetaData: public AP_Dialog_MetaData
{
public:
	AP_CocoaDialog_MetaData(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id dlgid);
	virtual ~AP_CocoaDialog_MetaData(void);

	virtual void			runModal(XAP_Frame * pFrame);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id dlgid);
	
protected:

 private:
	AP_CocoaDialog_MetadataController * m_dlg;
};

#endif /* AP_COCOADIALOG_METADATA_H */
