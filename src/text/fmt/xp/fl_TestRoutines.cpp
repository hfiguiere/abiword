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

#if defined(PT_TEST) || defined(FMT_TEST) || defined(UT_TEST)

#include <stdio.h>
#include <string.h>
#include "ut_test.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"

#include "fp_Page.h"
#include "fp_Run.h"
#include "fl_BlockLayout.h"
#include "fl_DocLayout.h"
#include "fl_SectionLayout.h"

FL_DocLayout* FL_DocLayout::m_pDocLayout = NULL;

/*!
  Dump AbiWord backend's internal structures
  This function is for use in GDB (and other debuggers allowing
  interaction with the program being debugged).
  Type this to print out the structures from the GDB command line:
   (gdb) p __dump()
*/
void __dump(void)
{
	FL_DocLayout::m_pDocLayout->__dump(stdout);
	FL_DocLayout::m_pDocLayout->getDocument()->__dump(stdout);
}

/*!
  Dump AbiWord backend's internal fmt structures
  \see __dump
*/
void __dump_fmt(void)
{
	FL_DocLayout::m_pDocLayout->__dump(stdout);
}

/*!
  Dump AbiWord backend's internal pt structures
  \see __dump
*/
void __dump_pt(void)
{
	FL_DocLayout::m_pDocLayout->getDocument()->__dump(stdout);
}

/*!
  Dump pages and sections contained in this FL_DocLayout
  \param fp File where the dump should be written to
*/
void FL_DocLayout::__dump(FILE * fp) const
{
	int count = m_vecPages.getItemCount();

	fprintf(fp,"FL_DocLayout::__dump(0x%p) contains %d pages.\n", 
			(void*)this, m_vecPages.getItemCount());
	for (int i=0; i<count; i++)
	{
		fp_Page* p = (fp_Page*) m_vecPages.getNthItem(i);
		p->__dump(fp);
	}

	fprintf(fp,"FL_DocLayout::__dump(0x%p) sections:\n",(void*)this);
	for (fl_SectionLayout * psl=getFirstSection(); (psl); psl=psl->getNext())
	{
		psl->__dump(fp);
	}
}

/*!
  Dump sections contained in this fl_SectionLayout
  \param fp File where the dump should be written to
*/
void fl_SectionLayout::__dump(FILE * fp) const
{
	fprintf(fp,"Section: 0x%p [type %d]\n",(void*)this,getType());
	for (fl_BlockLayout * pBL=getFirstBlock(); (pBL); pBL=pBL->getNext())
		pBL->__dump(fp);
}

/*!
  Dump runs contained in this fl_BlockLayout
  \param fp File where the dump should be written to
*/
void fl_BlockLayout::__dump(FILE * fp) const
{
	fprintf(fp,"  Block: 0x%p [sdh 0x%p]\n",(void*)this,(void*)m_sdh);
	for (fp_Run* pRun=m_pFirstRun; (pRun); pRun=pRun->getNext())
		pRun->__dump(fp);
}

#endif
