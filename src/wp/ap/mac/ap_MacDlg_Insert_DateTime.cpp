/* AbiWord
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

#include "ut_string.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

#include "xap_App.h"
#include "xap_MacApp.h"
#include "xap_MacFrame.h"

#include "ap_Strings.h"
#include "ap_Dialog_Id.h"
#include "ap_Dialog_Insert_DateTime.h"
#include "ap_MacDlg_Insert_DateTime.h"

#if 0
class DateTimeWin:public BWindow {
	public:
		DateTimeWin(BMessage *data);
		void SetDlg(AP_MacDialog_Insert_DateTime *brk);
		virtual void DispatchMessage(BMessage *msg, BHandler *handler);
		virtual bool QuitRequested(void);
		
	private:
		AP_MacDialog_Insert_DateTime 	*m_DlgTime;
		int currentSelectionIndex;

		sem_id modalSem;
		status_t WaitForDelete(sem_id deleteSem);
};


status_t DateTimeWin::WaitForDelete(sem_id blocker)
{
	status_t	result;
	thread_id	this_tid = find_thread(NULL);
	BLooper		*pLoop;
	BWindow		*pWin = 0;

	pLoop = BLooper::LooperForThread(this_tid);
	if (pLoop)
		pWin = dynamic_cast<BWindow*>(pLoop);

	// block until semaphore is deleted (modal is finished)
	if (pWin) {
		do {
			// update the window periodically			
			pWin->UpdateIfNeeded();
			result = acquire_sem_etc(blocker, 1, B_TIMEOUT, 10000);
		} while (result != B_BAD_SEM_ID);
	} else {
		do {
			// just wait for exit
			result = acquire_sem(blocker);
		} while (result != B_BAD_SEM_ID);
	}
	return result;
}

DateTimeWin::DateTimeWin(BMessage *data) 
	  :BWindow(data) 
{
	currentSelectionIndex = -1;
} //BreakWin::BreakWin

#include <ListItem.h>
#include <ListView.h>

void DateTimeWin::SetDlg(AP_MacDialog_Insert_DateTime *brk)
{
    int i;
    char szCurrentDateTime[CURRENT_DATE_TIME_SIZE];
    time_t  tim = time(NULL);
    struct tm *pTime = localtime(&tim);
	BListView* formatList;
	
	m_DlgTime = brk;
	
	formatList = (BListView *)FindView("FormatList");
	if(formatList)
	{
	    for (i = 0; InsertDateTimeFmts[i] != NULL; i++)
     	{
    	    strftime(szCurrentDateTime, CURRENT_DATE_TIME_SIZE, InsertDateTimeFmts[i], pTime);
    	    BStringItem* pItem = new BStringItem(szCurrentDateTime);
    	    formatList->AddItem(pItem);//SendMessage(m_hwndFormats, LB_ADDSTRING, 0, (LPARAM)szCurrentDateTime);
    	}
	}

	//	We need to tie up the caller thread for a while ...
	Show();
	
	modalSem = create_sem(0,"datetimesem");
	WaitForDelete(modalSem);
	
	Hide();
}

#include <stdio.h>

void DateTimeWin::DispatchMessage(BMessage *msg, BHandler *handler)
{
	BListView* pSource = NULL;

	switch(msg->what) 
	{
	
	case 'SELS': // Selection changed
			
		// Determine the source.
		if( msg->FindPointer("source" , (void **)&pSource) != B_OK)
			break;
		
		currentSelectionIndex = pSource->CurrentSelection();
		
		break;
		
	case 'EXES': // Close dialog and use current selection.
	
		pSource = (BListView *)FindView("FormatList");		
		currentSelectionIndex = pSource->CurrentSelection();
		 
		if(currentSelectionIndex > 0)
			m_DlgTime->setSelection(currentSelectionIndex);
		else
			m_DlgTime->setAnswer(AP_Dialog_Insert_DateTime::a_CANCEL);
			
		delete_sem(modalSem);
		
		break;

	default:
		BWindow::DispatchMessage(msg, handler);
	}
} 

bool DateTimeWin::QuitRequested()
{
	m_DlgTime->setAnswer(AP_Dialog_Insert_DateTime::a_CANCEL);
	
	delete_sem(modalSem);
	
	return(false);
}

#endif

//////////////////////////////////////////////////////////////////////////////

XAP_Dialog * AP_MacDialog_Insert_DateTime::static_constructor(XAP_DialogFactory * pFactory,
													 XAP_Dialog_Id id)
{
	AP_MacDialog_Insert_DateTime * p = new AP_MacDialog_Insert_DateTime(pFactory,id);
	return p;
}

AP_MacDialog_Insert_DateTime::AP_MacDialog_Insert_DateTime(XAP_DialogFactory * pDlgFactory,
					 XAP_Dialog_Id id) 
		   : AP_Dialog_Insert_DateTime(pDlgFactory,id) { 
} 

AP_MacDialog_Insert_DateTime::~AP_MacDialog_Insert_DateTime(void) {
}

/*****************************************************************/

void AP_MacDialog_Insert_DateTime::runModal(XAP_Frame * pFrame)
{
        /*
	BMessage msg;
	DateTimeWin  *newwin;
	if (RehydrateWindow("DateTime", &msg)) {
                newwin = new DateTimeWin(&msg);
		newwin->SetDlg(this);
		//Take the information here ...
		newwin->Lock();
		newwin->Close();
        } */
}

void AP_MacDialog_Insert_DateTime::setSelection(int selID)
{
if(selID >= 0)
	m_iFormatIndex = selID;
else
	m_iFormatIndex = -1;
	
}


