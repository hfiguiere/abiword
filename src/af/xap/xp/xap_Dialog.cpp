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

#include "ap_Dialog.h"

/*****************************************************************/

AP_Dialog::AP_Dialog(AP_DialogFactory * pDlgFactory, AP_Dialog_Id id)
{
	m_pDlgFactory = pDlgFactory;
	m_id = id;
}

AP_Dialog::~AP_Dialog(void)
{
}

AP_Dialog_Id AP_Dialog::getDialogId(void) const
{
	return m_id;
}

/*****************************************************************/

AP_Dialog_NonPersistent::AP_Dialog_NonPersistent(AP_DialogFactory * pDlgFactory, AP_Dialog_Id id)
	: AP_Dialog(pDlgFactory,id)
{
}

AP_Dialog_NonPersistent::~AP_Dialog_NonPersistent(void)
{
}
