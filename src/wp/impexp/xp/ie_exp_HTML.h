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


#ifndef IE_EXP_HTML_H
#define IE_EXP_HTML_H

#include "ie_exp.h"
#include "pl_Listener.h"
class PD_Document;
class s_HTML_Listener;

// The exporter/writer for HTML

// Not sure if these #defines should go in here or elsewhere...

#define BT_NORMAL	1
#define	BT_HEADING1	2
#define	BT_HEADING2	3
#define	BT_HEADING3	4
#define	BT_BLOCKTEXT	5
#define	BT_PLAINTEXT	6

class IE_Exp_HTML : public IE_Exp
{
public:
	IE_Exp_HTML(PD_Document * pDocument);
	virtual ~IE_Exp_HTML();

	static UT_Bool		RecognizeSuffix(const char * szSuffix);
	static IEStatus		StaticConstructor(PD_Document * pDocument,
										  IE_Exp ** ppie);
	static UT_Bool		GetDlgLabels(const char ** pszDesc,
									 const char ** pszSuffixList,
									 IEFileType * ft);
	static UT_Bool 		SupportsFileType(IEFileType ft);
	
protected:
	virtual IEStatus	_writeDocument(void);
	
	s_HTML_Listener *	m_pListener;
};

#endif /* IE_EXP_HTML_H */
