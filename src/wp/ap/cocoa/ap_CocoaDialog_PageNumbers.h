/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2001 Hubert Figuiere
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

#ifndef AP_COCOADIALOG_PAGENUMBERS_H
#define AP_COCOADIALOG_PAGENUMBERS_H

#include "ut_types.h"
#include "ap_Dialog_PageNumbers.h"

class XAP_CocoaFrame;
class GR_CocoaGraphics;

class AP_CocoaDialog_PageNumbers : public AP_Dialog_PageNumbers
{
 public:
  AP_CocoaDialog_PageNumbers(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
  virtual ~AP_CocoaDialog_PageNumbers(void);

  virtual void runModal(XAP_Frame * pFrame);

  static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);

  void event_OK(void);
  void event_Cancel(void);
  void event_WindowDelete(void);
  void event_PreviewExposed(void);
  void event_AlignChanged(AP_Dialog_PageNumbers::tAlign);
  void event_HdrFtrChanged(AP_Dialog_PageNumbers::tControl);

 protected:

  // private construction functions
#if 0
  virtual GtkWidget * _constructWindow (void);
  virtual void _constructWindowContents (GtkWidget * container);
  virtual void _connectSignals (void);

  // caches of the last known values for alignment and hdr/footer/both
  AP_Dialog_PageNumbers::tAlign m_recentAlign;
  AP_Dialog_PageNumbers::tControl m_recentControl;

  GtkWidget * m_buttonOK;
  GtkWidget * m_buttonCancel;
  GtkWidget * m_window;

  GtkWidget * m_previewArea;

  GtkWidget * m_combo1;
  GtkWidget * m_combo2;
#endif

  GR_CocoaGraphics * m_unixGraphics;
};

#endif /* AP_COCOADIALOG_PAGENUBMERS_H */
