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

#define DLGHACK
#ifdef DLGHACK			// see bottom of file for an apology
#ifdef WIN32
#include <windows.h>	// needs to be first
#endif
#ifdef LINUX
#include <gtk/gtk.h>
#endif
#endif /* DLGHACK */

#include <stdlib.h>
#include <string.h>
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ev_EditMethod.h"
#include "fv_View.h"
#include "fl_DocLayout.h"
#include "pd_Document.h"
#include "gr_Graphics.h"
#include "ap_App.h"
#include "ap_Frame.h"
#include "ap_EditMethods.h"


#ifdef DLGHACK
typedef enum _dlg_Answer { dlg_OK, dlg_CANCEL, dlg_YES, dlg_NO } dlg_Answer;
typedef enum _dlg_Buttons { dlg_O, dlg_OC, dlg_YN, dlg_YNC } dlg_Buttons;

dlg_Answer _askUser(AP_Frame * pFrame, const char * szQ, dlg_Buttons b, int defButton);
char * _promptFile(AP_Frame * pFrame, UT_Bool bSaveAs);
char * _promptFile(AP_Frame * pFrame, UT_Bool bSaveAs, char * pSuggestedName);
UT_Bool _chooseFont(AP_Frame * pFrame, FV_View * pView);
UT_Bool _printDoc(AP_Frame * pFrame, FV_View * pView);
#endif /* DLGHACK */


/*****************************************************************/
/*****************************************************************/

/* abbreviations:
**   BOL    beginning of line
**   EOL    end of line
**   BOW    beginning of word
**   EOW    end of word
**   BOW    beginning of sentence
**   EOS    end of sentence
**   BOB    beginning of block
**   EOB    end of block
**   BOD    beginning of document
**   EOD    end of document
**
**   warpInsPt    warp insertion point
**   extSel       extend selection
**   del          delete
**   bck          backwards
**   fwd          forwards
**/

class ap_EditMethods
{
public:
	static EV_EditMethod_Fn scrollPageDown;
	static EV_EditMethod_Fn scrollPageUp;
	static EV_EditMethod_Fn scrollPageLeft;
	static EV_EditMethod_Fn scrollPageRight;
	static EV_EditMethod_Fn scrollLineDown;
	static EV_EditMethod_Fn scrollLineUp;
	static EV_EditMethod_Fn scrollLineLeft;
	static EV_EditMethod_Fn scrollLineRight;
	static EV_EditMethod_Fn scrollToTop;
	static EV_EditMethod_Fn scrollToBottom;

	static EV_EditMethod_Fn warpInsPtToXY;
	static EV_EditMethod_Fn warpInsPtLeft;
	static EV_EditMethod_Fn warpInsPtRight;
	static EV_EditMethod_Fn warpInsPtBOL;
	static EV_EditMethod_Fn warpInsPtEOL;
	static EV_EditMethod_Fn warpInsPtBOW;
	static EV_EditMethod_Fn warpInsPtEOW;
	static EV_EditMethod_Fn warpInsPtBOS;
	static EV_EditMethod_Fn warpInsPtEOS;
	static EV_EditMethod_Fn warpInsPtBOB;
	static EV_EditMethod_Fn warpInsPtEOB;
	static EV_EditMethod_Fn warpInsPtBOD;
	static EV_EditMethod_Fn warpInsPtEOD;
	static EV_EditMethod_Fn warpInsPtPrevLine;
	static EV_EditMethod_Fn warpInsPtNextLine;

	static EV_EditMethod_Fn extSelToXY;
	static EV_EditMethod_Fn extSelLeft;
	static EV_EditMethod_Fn extSelRight;
	static EV_EditMethod_Fn extSelBOL;
	static EV_EditMethod_Fn extSelEOL;
	static EV_EditMethod_Fn extSelBOW;
	static EV_EditMethod_Fn extSelEOW;
	static EV_EditMethod_Fn extSelBOS;
	static EV_EditMethod_Fn extSelEOS;
	static EV_EditMethod_Fn extSelBOB;
	static EV_EditMethod_Fn extSelEOB;
	static EV_EditMethod_Fn extSelBOD;
	static EV_EditMethod_Fn extSelEOD;
	static EV_EditMethod_Fn extSelPrevLine;
	static EV_EditMethod_Fn extSelNextLine;
	static EV_EditMethod_Fn extSelPageDown;
	static EV_EditMethod_Fn extSelPageUp;
	static EV_EditMethod_Fn selectAll;
	static EV_EditMethod_Fn selectWord;
	static EV_EditMethod_Fn selectLine;
	static EV_EditMethod_Fn selectBlock;

	static EV_EditMethod_Fn singleClick;
	static EV_EditMethod_Fn doubleClick;

	static EV_EditMethod_Fn delLeft;
	static EV_EditMethod_Fn delRight;
	static EV_EditMethod_Fn delBOL;
	static EV_EditMethod_Fn delEOL;
	static EV_EditMethod_Fn delBOW;
	static EV_EditMethod_Fn delEOW;
	static EV_EditMethod_Fn delBOS;
	static EV_EditMethod_Fn delEOS;
	static EV_EditMethod_Fn delBOB;
	static EV_EditMethod_Fn delEOB;
	static EV_EditMethod_Fn delBOD;
	static EV_EditMethod_Fn delEOD;

	static EV_EditMethod_Fn insertData;
	static EV_EditMethod_Fn insertTab;
	static EV_EditMethod_Fn insertSoftBreak;
	static EV_EditMethod_Fn insertParagraphBreak;
	static EV_EditMethod_Fn insertLineBreak;
	static EV_EditMethod_Fn insertPageBreak;
	static EV_EditMethod_Fn insertColumnBreak;

	static EV_EditMethod_Fn insertSpace;
	static EV_EditMethod_Fn insertNBSpace;

	static EV_EditMethod_Fn insFmtFaceTimes; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtFaceCourier; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtFaceArial; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtSize08; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtSize10; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtSize12; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtSize14; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtSize16; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtSize24; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtSize36; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtColorBlack; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtColorRed; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtColorGreen; // TODO we need a better way of doing this
	static EV_EditMethod_Fn insFmtColorBlue; // TODO we need a better way of doing this

	// TODO add functions for all of the standard menu commands.
	// TODO here are a few that i started.

	static EV_EditMethod_Fn fileNew;
	static EV_EditMethod_Fn fileOpen;
	static EV_EditMethod_Fn fileSave;
	static EV_EditMethod_Fn fileSaveAs;
	static EV_EditMethod_Fn print;

	static EV_EditMethod_Fn undo;
	static EV_EditMethod_Fn redo;
	static EV_EditMethod_Fn cut;
	static EV_EditMethod_Fn copy;
	static EV_EditMethod_Fn paste;
	static EV_EditMethod_Fn find;
	static EV_EditMethod_Fn go;
	static EV_EditMethod_Fn replace;

	static EV_EditMethod_Fn dlgFont;
	static EV_EditMethod_Fn toggleBold;
	static EV_EditMethod_Fn toggleItalic;
	static EV_EditMethod_Fn toggleUline;
	static EV_EditMethod_Fn toggleStrike;
	static EV_EditMethod_Fn togglePlain;

	static EV_EditMethod_Fn alignLeft;
	static EV_EditMethod_Fn alignCenter;
	static EV_EditMethod_Fn alignRight;
	static EV_EditMethod_Fn alignJustify;

	static EV_EditMethod_Fn singleSpace;
	static EV_EditMethod_Fn middleSpace;
	static EV_EditMethod_Fn doubleSpace;

	static EV_EditMethod_Fn activateWindow_1;
	static EV_EditMethod_Fn activateWindow_2;
	static EV_EditMethod_Fn activateWindow_3;
	static EV_EditMethod_Fn activateWindow_4;
	static EV_EditMethod_Fn activateWindow_5;
	static EV_EditMethod_Fn activateWindow_6;
	static EV_EditMethod_Fn activateWindow_7;
	static EV_EditMethod_Fn activateWindow_8;
	static EV_EditMethod_Fn activateWindow_9;
	static EV_EditMethod_Fn moreWindowsDlg;

	static EV_EditMethod_Fn newWindow;
	static EV_EditMethod_Fn cycleWindows;
	static EV_EditMethod_Fn cycleWindowsBck;
	static EV_EditMethod_Fn closeWindow;
	static EV_EditMethod_Fn querySaveAndExit;

	// Test routines

	static EV_EditMethod_Fn Test_Dump;
};

/*****************************************************************/
/*****************************************************************/

#define _D_				EV_EMT_REQUIREDATA
#define _M_				EV_EMT_ALLOWMULTIPLIER
#define _DM_			EV_EMT_REQUIREDATA | EV_EMT_ALLOWMULTIPLIER

#define F(fn)			ap_EditMethods::fn
#define N(fn)			#fn
#define NF(fn)			N(fn), F(fn)

static EV_EditMethod s_arrayEditMethods[] =
{
	EV_EditMethod(NF(scrollPageDown),		_M_,	""),
	EV_EditMethod(NF(scrollPageUp),			_M_,	""),
	EV_EditMethod(NF(scrollPageLeft),		_M_,	""),
	EV_EditMethod(NF(scrollPageRight),		_M_,	""),
	EV_EditMethod(NF(scrollLineDown),		_M_,	""),
	EV_EditMethod(NF(scrollLineUp),			_M_,	""),
	EV_EditMethod(NF(scrollLineLeft),		_M_,	""),
	EV_EditMethod(NF(scrollLineRight),		_M_,	""),
	EV_EditMethod(NF(scrollToTop),			_M_,	""),
	EV_EditMethod(NF(scrollToBottom),		_M_,	""),

	EV_EditMethod(NF(warpInsPtToXY),		_M_,	""),
	EV_EditMethod(NF(warpInsPtLeft),		_M_,	""),
	EV_EditMethod(NF(warpInsPtRight),		_M_,	""),
	EV_EditMethod(NF(warpInsPtBOL),			_M_,	""),
	EV_EditMethod(NF(warpInsPtEOL),			_M_,	""),
	EV_EditMethod(NF(warpInsPtBOW),			_M_,	""),
	EV_EditMethod(NF(warpInsPtEOW),			_M_,	""),
	EV_EditMethod(NF(warpInsPtBOS),			_M_,	""),
	EV_EditMethod(NF(warpInsPtEOS),			_M_,	""),
	EV_EditMethod(NF(warpInsPtBOB),			_M_,	""),
	EV_EditMethod(NF(warpInsPtEOB),			_M_,	""),
	EV_EditMethod(NF(warpInsPtBOD),			_M_,	""),
	EV_EditMethod(NF(warpInsPtEOD),			_M_,	""),
	EV_EditMethod(NF(warpInsPtPrevLine),	_M_,	""),
	EV_EditMethod(NF(warpInsPtNextLine),	_M_,	""),

	EV_EditMethod(NF(extSelToXY),			_M_,	""),
	EV_EditMethod(NF(extSelLeft),			_M_,	""),
	EV_EditMethod(NF(extSelRight),			_M_,	""),
	EV_EditMethod(NF(extSelBOL),			_M_,	""),
	EV_EditMethod(NF(extSelEOL),			_M_,	""),
	EV_EditMethod(NF(extSelBOW),			_M_,	""),
	EV_EditMethod(NF(extSelEOW),			_M_,	""),
	EV_EditMethod(NF(extSelBOS),			_M_,	""),
	EV_EditMethod(NF(extSelEOS),			_M_,	""),
	EV_EditMethod(NF(extSelBOB),			_M_,	""),
	EV_EditMethod(NF(extSelEOB),			_M_,	""),
	EV_EditMethod(NF(extSelBOD),			_M_,	""),
	EV_EditMethod(NF(extSelEOD),			_M_,	""),
	EV_EditMethod(NF(extSelPrevLine),		_M_,	""),
	EV_EditMethod(NF(extSelNextLine),		_M_,	""),
	EV_EditMethod(NF(extSelPageDown),		_M_,	""),
	EV_EditMethod(NF(extSelPageUp),			_M_,	""),
	EV_EditMethod(NF(selectAll),			_M_,	""),
	EV_EditMethod(NF(selectWord),			_M_,	""),
	EV_EditMethod(NF(selectLine),			_M_,	""),
	EV_EditMethod(NF(selectBlock),			_M_,	""),

	EV_EditMethod(NF(singleClick),			_M_,	""),
	EV_EditMethod(NF(doubleClick),			_M_,	""),

	EV_EditMethod(NF(delLeft),				_M_,	""),
	EV_EditMethod(NF(delRight),				_M_,	""),
	EV_EditMethod(NF(delBOL),				_M_,	""),
	EV_EditMethod(NF(delEOL),				_M_,	""),
	EV_EditMethod(NF(delBOW),				_M_,	""),
	EV_EditMethod(NF(delEOW),				_M_,	""),
	EV_EditMethod(NF(delBOS),				_M_,	""),
	EV_EditMethod(NF(delEOS),				_M_,	""),
	EV_EditMethod(NF(delBOB),				_M_,	""),
	EV_EditMethod(NF(delEOB),				_M_,	""),
	EV_EditMethod(NF(delBOD),				_M_,	""),
	EV_EditMethod(NF(delEOD),				_M_,	""),

	EV_EditMethod(NF(insertData),			_DM_,	""),
	EV_EditMethod(NF(insertTab),			_M_,	""),
	EV_EditMethod(NF(insertSoftBreak),		_M_,	""),
	EV_EditMethod(NF(insertParagraphBreak),	_M_,	""),
	EV_EditMethod(NF(insertLineBreak),		_M_,	""),
	EV_EditMethod(NF(insertPageBreak),		_M_,	""),
	EV_EditMethod(NF(insertColumnBreak),	_M_,	""),

	EV_EditMethod(NF(insertSpace),			_M_,	""),
	EV_EditMethod(NF(insertNBSpace),		_M_,	""),

	EV_EditMethod(NF(insFmtFaceTimes),		_M_,	""),
	EV_EditMethod(NF(insFmtFaceCourier),	_M_,	""),
	EV_EditMethod(NF(insFmtFaceArial),		_M_,	""),
	EV_EditMethod(NF(insFmtSize08),			_M_,	""),
	EV_EditMethod(NF(insFmtSize10),			_M_,	""),
	EV_EditMethod(NF(insFmtSize12),			_M_,	""),
	EV_EditMethod(NF(insFmtSize14),			_M_,	""),
	EV_EditMethod(NF(insFmtSize16),			_M_,	""),
	EV_EditMethod(NF(insFmtSize24),			_M_,	""),
	EV_EditMethod(NF(insFmtSize36),			_M_,	""),
	EV_EditMethod(NF(insFmtColorBlack),		_M_,	""),
	EV_EditMethod(NF(insFmtColorRed),		_M_,	""),
	EV_EditMethod(NF(insFmtColorGreen),		_M_,	""),
	EV_EditMethod(NF(insFmtColorBlue),		_M_,	""),

	EV_EditMethod(NF(fileNew),				_M_,	""),
	EV_EditMethod(NF(fileOpen),				_M_,	""),
	EV_EditMethod(NF(fileSave),				_M_,	""),
	EV_EditMethod(NF(fileSaveAs),			_M_,	""),
	EV_EditMethod(NF(print),				_M_,	""),

	EV_EditMethod(NF(undo),					_M_,	""),
	EV_EditMethod(NF(redo),					_M_,	""),
	EV_EditMethod(NF(cut),					_M_,	""),
	EV_EditMethod(NF(copy),					_M_,	""),
	EV_EditMethod(NF(paste),				_M_,	""),
	EV_EditMethod(NF(find),					_M_,	""),
	EV_EditMethod(NF(go),					_M_,	""),
	EV_EditMethod(NF(replace),				_M_,	""),

	EV_EditMethod(NF(dlgFont),				0,		""),
	EV_EditMethod(NF(toggleBold),			0,		""),
	EV_EditMethod(NF(toggleItalic),			0,		""),
	EV_EditMethod(NF(toggleUline),			0,		""),
	EV_EditMethod(NF(toggleStrike),			0,		""),
	EV_EditMethod(NF(togglePlain),			0,		""),

	EV_EditMethod(NF(alignLeft),			0,		""),
	EV_EditMethod(NF(alignCenter),			0,		""),
	EV_EditMethod(NF(alignRight),			0,		""),
	EV_EditMethod(NF(alignJustify),			0,		""),

	EV_EditMethod(NF(singleSpace),			0,		""),
	EV_EditMethod(NF(middleSpace),			0,		""),
	EV_EditMethod(NF(doubleSpace),			0,		""),

	EV_EditMethod(NF(activateWindow_1),		0,		""),
	EV_EditMethod(NF(activateWindow_2),		0,		""),
	EV_EditMethod(NF(activateWindow_3),		0,		""),
	EV_EditMethod(NF(activateWindow_4),		0,		""),
	EV_EditMethod(NF(activateWindow_5),		0,		""),
	EV_EditMethod(NF(activateWindow_6),		0,		""),
	EV_EditMethod(NF(activateWindow_7),		0,		""),
	EV_EditMethod(NF(activateWindow_8),		0,		""),
	EV_EditMethod(NF(activateWindow_9),		0,		""),
	EV_EditMethod(NF(moreWindowsDlg),		0,		""),

	EV_EditMethod(NF(newWindow),			_M_,	""),
	EV_EditMethod(NF(cycleWindows),			_M_,	""),
	EV_EditMethod(NF(cycleWindowsBck),		_M_,	""),
	EV_EditMethod(NF(closeWindow),			_M_,	""),
	EV_EditMethod(NF(querySaveAndExit),		_M_,	""),

	EV_EditMethod(NF(Test_Dump),			_M_,	"")
};

#define NrElements(a)	((sizeof(a)/sizeof(a[0])))


EV_EditMethodContainer * AP_GetEditMethods(void)
{
	// Construct a container for all of the methods this application
	// knows about.

	return new EV_EditMethodContainer(NrElements(s_arrayEditMethods),s_arrayEditMethods);
}

#undef _D_
#undef _M_
#undef _DM_
#undef F
#undef N
#undef NF

/*****************************************************************/
/*****************************************************************/

#define F(fn)		ap_EditMethods::fn
#define Defun(fn)	UT_Bool F(fn)(FV_View*   pView,   EV_EditMethodCallData *   pCallData  )
#define Defun0(fn)	UT_Bool F(fn)(FV_View* /*pView*/, EV_EditMethodCallData * /*pCallData*/)
#define Defun1(fn)	UT_Bool F(fn)(FV_View*   pView,   EV_EditMethodCallData * /*pCallData*/)
#define EX(fn)		F(fn)(pView, pCallData)


Defun1(scrollPageDown)
{
	pView->cmdScroll(DG_SCROLLCMD_PAGEDOWN);

	return UT_TRUE;
}

Defun1(scrollPageUp)
{
	pView->cmdScroll(DG_SCROLLCMD_PAGEUP);

	return UT_TRUE;
}

Defun1(scrollPageLeft)
{
	pView->cmdScroll(DG_SCROLLCMD_PAGELEFT);

	return UT_TRUE;
}

Defun1(scrollPageRight)
{
	pView->cmdScroll(DG_SCROLLCMD_PAGERIGHT);

	return UT_TRUE;
}

Defun1(scrollLineDown)
{
	pView->cmdScroll(DG_SCROLLCMD_LINEDOWN);

	return UT_TRUE;
}

Defun1(scrollLineUp)
{
	pView->cmdScroll(DG_SCROLLCMD_LINEUP);

	return UT_TRUE;
}

Defun1(scrollLineLeft)
{
	pView->cmdScroll(DG_SCROLLCMD_LINELEFT);

	return UT_TRUE;
}

Defun1(scrollLineRight)
{
	pView->cmdScroll(DG_SCROLLCMD_LINERIGHT);

	return UT_TRUE;
}

Defun1(scrollToTop)
{
	pView->cmdScroll(DG_SCROLLCMD_TOTOP);

	return UT_TRUE;
}

Defun1(scrollToBottom)
{
	pView->cmdScroll(DG_SCROLLCMD_TOBOTTOM);

	return UT_TRUE;
}

Defun(singleClick)
{
	UT_Bool bRes = UT_TRUE;

	// NOTE: context-free binding mechanism ==> we need this extra layer
	if (pView->isLeftMargin(pCallData->m_xPos, pCallData->m_yPos))
		bRes = EX(selectLine);
	else
		bRes = EX(warpInsPtToXY);

	return bRes;
}

Defun(doubleClick)
{
	UT_Bool bRes = UT_TRUE;

	// NOTE: context-free binding mechanism ==> we need this extra layer
	if (pView->isLeftMargin(pCallData->m_xPos, pCallData->m_yPos))
		bRes = EX(selectBlock);
	else
		bRes = EX(selectWord);

	return bRes;
}

Defun(warpInsPtToXY)
{
	pView->warpInsPtToXY(pCallData->m_xPos, pCallData->m_yPos);

	return UT_TRUE;
}

Defun1(warpInsPtLeft)
{
	pView->cmdCharMotion(UT_FALSE,1);
	return UT_TRUE;
}

Defun1(warpInsPtRight)
{
	pView->cmdCharMotion(UT_TRUE,1);
	return UT_TRUE;
}

Defun1(warpInsPtBOL)
{
	pView->moveInsPtTo(FV_DOCPOS_BOL);
	return UT_TRUE;
}

Defun1(warpInsPtEOL)
{
	pView->moveInsPtTo(FV_DOCPOS_EOL);
	return UT_TRUE;
}

Defun1(warpInsPtBOW)
{
	pView->moveInsPtTo(FV_DOCPOS_BOW);
	return UT_TRUE;
}

Defun1(warpInsPtEOW)
{
	pView->moveInsPtTo(FV_DOCPOS_EOW);
	return UT_TRUE;
}

Defun0(warpInsPtBOS)
{
	return UT_TRUE;
}

Defun0(warpInsPtEOS)
{
	return UT_TRUE;
}

Defun1(warpInsPtBOB)
{
	pView->moveInsPtTo(FV_DOCPOS_BOB);
	return UT_TRUE;
}

Defun1(warpInsPtEOB)
{
	pView->moveInsPtTo(FV_DOCPOS_EOB);
	return UT_TRUE;
}

Defun1(warpInsPtBOD)
{
	pView->moveInsPtTo(FV_DOCPOS_BOD);
	return UT_TRUE;
}

Defun1(warpInsPtEOD)
{
	pView->moveInsPtTo(FV_DOCPOS_EOD);
	return UT_TRUE;
}

Defun1(warpInsPtPrevLine)
{
	pView->warpInsPtNextPrevLine(UT_FALSE);

	return UT_TRUE;
}

Defun1(warpInsPtNextLine)
{
	pView->warpInsPtNextPrevLine(UT_TRUE);

	return UT_TRUE;
}

Defun(extSelToXY)
{
	pView->extSelToXY(pCallData->m_xPos, pCallData->m_yPos);
	return UT_TRUE;
}
Defun1(extSelLeft)
{
	pView->extSelHorizontal(UT_FALSE,1);
	return UT_TRUE;
}

Defun1(extSelRight)
{
	pView->extSelHorizontal(UT_TRUE,1);
	return UT_TRUE;
}

Defun1(extSelBOL)
{
	pView->extSelTo(FV_DOCPOS_BOL);
	return UT_TRUE;
}

Defun1(extSelEOL)
{
	pView->extSelTo(FV_DOCPOS_EOL);
	return UT_TRUE;
}

Defun1(extSelBOW)
{
	pView->extSelTo(FV_DOCPOS_BOW);
	return UT_TRUE;
}

Defun1(extSelEOW)
{
	pView->extSelTo(FV_DOCPOS_EOW);
	return UT_TRUE;
}

Defun0(extSelBOS)
{
	return UT_TRUE;
}

Defun0(extSelEOS)
{
	return UT_TRUE;
}

Defun1(extSelBOB)
{
	pView->extSelTo(FV_DOCPOS_BOB);
	return UT_TRUE;
}

Defun1(extSelEOB)
{
	pView->extSelTo(FV_DOCPOS_EOB);
	return UT_TRUE;
}

Defun1(extSelBOD)
{
	pView->extSelTo(FV_DOCPOS_BOD);
	return UT_TRUE;
}

Defun1(extSelEOD)
{
	pView->extSelTo(FV_DOCPOS_EOD);
	return UT_TRUE;
}

Defun1(extSelPrevLine)
{
	pView->extSelNextPrevLine(UT_FALSE);

	return UT_TRUE;
}

Defun1(extSelNextLine)
{
	pView->extSelNextPrevLine(UT_TRUE);

	return UT_TRUE;
}

Defun0(extSelPageDown)
{
	return UT_TRUE;
}

Defun0(extSelPageUp)
{
	return UT_TRUE;
}

Defun1(selectAll)
{
	pView->moveInsPtTo(FV_DOCPOS_BOD);
	pView->extSelTo(FV_DOCPOS_EOD);
	return UT_TRUE;
}

Defun(selectWord)
{
	pView->cmdSelect(pCallData->m_xPos, pCallData->m_yPos, FV_DOCPOS_BOW, FV_DOCPOS_EOW);
	return UT_TRUE;
}

Defun(selectLine)
{
	pView->cmdSelect(pCallData->m_xPos, pCallData->m_yPos, FV_DOCPOS_BOL, FV_DOCPOS_EOL);
	return UT_TRUE;
}

Defun(selectBlock)
{
	pView->cmdSelect(pCallData->m_xPos, pCallData->m_yPos, FV_DOCPOS_BOB, FV_DOCPOS_EOB);
	return UT_TRUE;
}

Defun1(delLeft)
{
	pView->cmdCharDelete(UT_FALSE,1);
	return UT_TRUE;
}

Defun1(delRight)
{
	pView->cmdCharDelete(UT_TRUE,1);
	return UT_TRUE;
}

Defun1(delBOL)
{
	pView->delTo(FV_DOCPOS_BOL);
	return UT_TRUE;
}

Defun1(delEOL)
{
	pView->delTo(FV_DOCPOS_EOL);
	return UT_TRUE;
}

Defun1(delBOW)
{
	pView->delTo(FV_DOCPOS_BOW);
	return UT_TRUE;
}

Defun1(delEOW)
{
	pView->delTo(FV_DOCPOS_EOW);
	return UT_TRUE;
}

Defun0(delBOS)
{
	return UT_TRUE;
}

Defun0(delEOS)
{
	return UT_TRUE;
}

Defun1(delBOB)
{
	pView->delTo(FV_DOCPOS_BOB);
	return UT_TRUE;
}

Defun1(delEOB)
{
	pView->delTo(FV_DOCPOS_EOB);
	return UT_TRUE;
}

Defun1(delBOD)
{
	pView->delTo(FV_DOCPOS_BOD);
	return UT_TRUE;
}

Defun1(delEOD)
{
	pView->delTo(FV_DOCPOS_EOD);
	return UT_TRUE;
}

Defun(insertData)
{
	pView->cmdCharInsert(pCallData->m_pData,pCallData->m_dataLength);
	return UT_TRUE;
}

Defun0(insertTab)
{
	return UT_TRUE;
}

Defun0(insertSoftBreak)
{
	return UT_TRUE;
}

Defun1(insertParagraphBreak)
{
	pView->insertParagraphBreak();

	return UT_TRUE;
}

Defun0(insertLineBreak)
{
	return UT_TRUE;
}

Defun0(insertPageBreak)
{
	return UT_TRUE;
}

Defun0(insertColumnBreak)
{
	return UT_TRUE;
}

Defun1(insertSpace)
{
	UT_UCSChar sp = 0x0020;
	pView->cmdCharInsert(&sp,1);
	return UT_TRUE;
}

Defun1(insertNBSpace)
{
	UT_UCSChar sp = 0x00a0;				// decimal 160 is NBS
	pView->cmdCharInsert(&sp,1);
	return UT_TRUE;
}

Defun1(insFmtFaceTimes)
{
	const XML_Char * properties[] =	{ "font-family", "Times New Roman", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtFaceCourier)
{
	const XML_Char * properties[] =	{ "font-family", "Courier New", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtFaceArial)
{
	const XML_Char * properties[] =	{ "font-family", "Arial", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtSize08)
{
	const XML_Char * properties[] =	{ "font-size", "8pt", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtSize10)
{
	const XML_Char * properties[] =	{ "font-size", "10pt", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtSize12)
{
	const XML_Char * properties[] =	{ "font-size", "12pt", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtSize14)
{
	const XML_Char * properties[] =	{ "font-size", "14pt", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtSize16)
{
	const XML_Char * properties[] =	{ "font-size", "16pt", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtSize24)
{
	const XML_Char * properties[] =	{ "font-size", "24pt", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtSize36)
{
	const XML_Char * properties[] =	{ "font-size", "36pt", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}

Defun1(insFmtColorBlack)
{
	const XML_Char * properties[] =	{ "color", "000000", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtColorRed)
{
	const XML_Char * properties[] =	{ "color", "ff0000", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtColorGreen)
{
	const XML_Char * properties[] =	{ "color", "00ff00", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}
Defun1(insFmtColorBlue)
{
	const XML_Char * properties[] =	{ "color", "0000ff", 0};
	pView->setCharFormat(properties);
	return UT_TRUE;
}

Defun1(fileNew)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);
	AP_App * pApp = pFrame->getApp();
	UT_ASSERT(pApp);

	AP_Frame * pNewFrame = pApp->newFrame();

	if (pNewFrame)
		pFrame = pNewFrame;

	UT_Bool bRet = pFrame->loadDocument(NULL);

	if (pNewFrame)
		pNewFrame->show();

	return bRet;
}

Defun1(fileOpen)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);
	UT_Bool bRes = UT_TRUE;

#ifdef DLGHACK
	char * pNewFile = _promptFile(pFrame, UT_FALSE);
#else
	char * pNewFile = NULL;
#endif /* DLGHACK */

	if (pNewFile)
	{
		UT_DEBUGMSG(("fileOpen: loading [%s]\n",pNewFile));
		AP_App * pApp = pFrame->getApp();
		UT_ASSERT(pApp);
		AP_Frame * pNewFrame = NULL;

		// see if requested file is already open in another frame
		UT_sint32 ndx = pApp->findFrame(pNewFile);

		if (ndx < 0)
		{
			// nope, make a new one
			pNewFrame = pApp->newFrame();
		}
		else
		{
			// yep, reuse it
			pNewFrame = pApp->getFrame(ndx);
			UT_ASSERT(pNewFrame);

			pNewFrame->raise();

#ifdef DLGHACK
			char buf[256];	// TODO: could overflow until we get path out of filename

			sprintf(buf, "Revert to saved copy of %s?", pNewFrame->getFilename());
			dlg_Answer ans = _askUser(pNewFrame, buf, dlg_YN, 0);

			if (ans == dlg_NO)
			{
				// never mind
				free(pNewFile);
				return UT_FALSE;
			}
#endif /* DLGHACK */
		}

		if (pNewFrame)
			pFrame = pNewFrame;

		bRes = pFrame->loadDocument(pNewFile);

		// HACK: at least make something show
		if (!bRes)
			bRes = pFrame->loadDocument(NULL);

		if (pNewFrame)
			pNewFrame->show();

		free(pNewFile);
	}

	return bRes;
}

Defun(fileSave)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);

	// can only save without prompting if filename already known
	if (!pFrame->getFilename())
		return EX(fileSaveAs);

	pView->cmdSave();

	if (pFrame->getViewNumber() > 0)
	{
		AP_App * pApp = pFrame->getApp();
		UT_ASSERT(pApp);

		pApp->updateClones(pFrame);
	}

	return UT_TRUE;
}

Defun1(fileSaveAs)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);

#ifdef DLGHACK
	char * pNewFile = _promptFile(pFrame, UT_TRUE);
#else
	char * pNewFile = NULL;
#endif /* DLGHACK */

	if (pNewFile)
	{
		UT_DEBUGMSG(("fileSaveAs: saving as [%s]\n",pNewFile));
		pView->cmdSaveAs(pNewFile);
		free(pNewFile);

		if (pFrame->getViewNumber() > 0)
		{
			AP_App * pApp = pFrame->getApp();
			UT_ASSERT(pApp);

			pApp->updateClones(pFrame);
		}
	}

	return UT_TRUE;
}

Defun1(print)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);
#ifdef DLGHACK
	_printDoc(pFrame, pView);
#endif /* DLGHACK */
	return UT_TRUE;
}

Defun1(undo)
{
	pView->cmdUndo(1);
	return UT_TRUE;
}

Defun1(redo)
{
	pView->cmdRedo(1);
	return UT_TRUE;
}

Defun0(cut)
{
	return UT_TRUE;
}

Defun0(copy)
{
	return UT_TRUE;
}

Defun0(paste)
{
	return UT_TRUE;
}

Defun0(find)
{
	return UT_TRUE;
}

Defun0(go)
{
	return UT_TRUE;
}

Defun0(replace)
{
	return UT_TRUE;
}

Defun1(newWindow)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);

	return (pFrame->cloneFrame() ? UT_TRUE : UT_FALSE);
}

static UT_Bool _activateWindow(FV_View* pView, UT_uint32 ndx)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);
	AP_App * pApp = pFrame->getApp();
	UT_ASSERT(pApp);

	UT_ASSERT(ndx > 0);
	UT_ASSERT(ndx <= pApp->getFrameCount());

	AP_Frame * pSelFrame = pApp->getFrame(ndx - 1);

	if (pSelFrame)
		pSelFrame->raise();

	return UT_TRUE;
}

Defun1(activateWindow_1)
{
	return _activateWindow(pView, 1);
}
Defun1(activateWindow_2)
{
	return _activateWindow(pView, 2);
}
Defun1(activateWindow_3)
{
	return _activateWindow(pView, 3);
}
Defun1(activateWindow_4)
{
	return _activateWindow(pView, 4);
}
Defun1(activateWindow_5)
{
	return _activateWindow(pView, 5);
}
Defun1(activateWindow_6)
{
	return _activateWindow(pView, 6);
}
Defun1(activateWindow_7)
{
	return _activateWindow(pView, 7);
}
Defun1(activateWindow_8)
{
	return _activateWindow(pView, 8);
}
Defun1(activateWindow_9)
{
	return _activateWindow(pView, 9);
}
Defun0(moreWindowsDlg)
{
	return UT_TRUE;
}

Defun1(cycleWindows)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);
	AP_App * pApp = pFrame->getApp();
	UT_ASSERT(pApp);

	UT_sint32 ndx = pApp->findFrame(pFrame);
	UT_ASSERT(ndx >= 0);

	if (ndx < (UT_sint32) pApp->getFrameCount() - 1)
		ndx++;
	else
		ndx = 0;

	AP_Frame * pSelFrame = pApp->getFrame(ndx);

	if (pSelFrame)
		pSelFrame->raise();

	return UT_TRUE;
}

Defun1(cycleWindowsBck)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);
	AP_App * pApp = pFrame->getApp();
	UT_ASSERT(pApp);

	UT_sint32 ndx = pApp->findFrame(pFrame);
	UT_ASSERT(ndx >= 0);

	if (ndx > 0)
		ndx--;
	else
		ndx = pApp->getFrameCount() - 1;

	AP_Frame * pSelFrame = pApp->getFrame(ndx);

	if (pSelFrame)
		pSelFrame->raise();

	return UT_TRUE;
}

static void _reallyExit(void)
{
#ifdef DLGHACK
#ifdef WIN32
	PostQuitMessage (0);
#endif
#ifdef LINUX
	gtk_main_quit();	// what miguel uses
//	exit(0);			// what Andy had
#endif
#endif /* DLGHACK */
}

Defun(closeWindow)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);
	AP_App * pApp = pFrame->getApp();
	UT_ASSERT(pApp);

	// is this the last view on a dirty document?
	if ((pFrame->getViewNumber() == 0) &&
		(pFrame->isDirty()))
	{
#ifdef DLGHACK
		char buf[256];	// TODO: could overflow until we get path out of filename

		sprintf(buf, "Save changes to %s?", pFrame->getTitle(200));
		dlg_Answer ans = _askUser(pFrame, buf, dlg_YNC, 0);

		if (ans == dlg_YES)
		{
			// save it first
			UT_Bool bRet = EX(fileSave);

			if (!bRet)
			{
				/* TODO */
			}
		}
		else if (ans == dlg_CANCEL)
		{
			// don't close
			return UT_FALSE;
		}
#endif /* DLGHACK */
	}

	// are we the last window?
	if (1 >= pApp->getFrameCount())
	{
		_reallyExit();
	}

	// nuke the window
	pFrame->close();
	pApp->forgetFrame(pFrame);
	delete pFrame;

	return UT_TRUE;
}

Defun(querySaveAndExit)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);
	AP_App * pApp = pFrame->getApp();
	UT_ASSERT(pApp);

	if (1 < pApp->getFrameCount())
	{
#ifdef DLGHACK
		dlg_Answer ans = _askUser(pFrame, "Close all windows and exit?", dlg_YN, 1);

		if (ans == dlg_NO)
		{
			// never mind
			return UT_FALSE;
		}
#endif /* DLGHACK */
	}

	UT_Bool bRet = UT_TRUE;
	UT_uint32 ndx = pApp->getFrameCount();

	// loop over windows, but stop if one can't close
	while (bRet && ndx > 0)
	{
		AP_Frame * f = pApp->getFrame(ndx - 1);
		UT_ASSERT(f);
		pView = f->getCurrentView();
		UT_ASSERT(pView);

		bRet = EX(closeWindow);

		ndx--;
	}

	if (bRet)
	{
		// TODO: this shouldn't be necessary, but just in case
		_reallyExit();
	}

	return bRet;
}

Defun1(dlgFont)
{
	AP_Frame * pFrame = (AP_Frame *) pView->getParentData();
	UT_ASSERT(pFrame);

#ifdef DLGHACK
	return _chooseFont(pFrame, pView);
#else
	return UT_TRUE;
#endif /* DLGHACK */
}

static UT_Bool _toggleSpan(FV_View * pView, const XML_Char * prop, const XML_Char * vOn, const XML_Char * vOff, UT_Bool bMultiple=UT_FALSE)
{
	const XML_Char * props_out[] =	{ NULL, NULL, 0};

	// get current font info from pView
	const XML_Char ** props_in = NULL;
	const XML_Char * s;

	if (!pView->getCharFormat(&props_in))
		return UT_FALSE;

	props_out[0] = prop;
	props_out[1] = vOn;		// be optimistic

	XML_Char * buf = NULL;

	s = UT_getAttribute(prop, props_in);
	if (s)
	{
		if (bMultiple)
		{
			// some properties have multiple values
			XML_Char*	p = strstr(s, vOn);

			if (p)
			{
				// yep...
				if (strstr(s, vOff))
					UT_ASSERT(UT_SHOULD_NOT_HAPPEN);

				// ... take it out
				int len = strlen(s);
				buf = (XML_Char *) calloc(len, sizeof(XML_Char *));

				strncpy(buf, s, p - s);
				strcat(buf, s + (p - s) + strlen(vOn));

				// now see if anything's left
				XML_Char * q;
				UT_cloneString(q, buf);

				if (q && strtok(q, " "))
					props_out[1] = buf;		// yep, use it
				else
					props_out[1] = vOff;	// nope, clear it

				free(q);
			}
			else
			{
				// nope...
				if (UT_stricmp(s, vOff))
				{
					// ...put it in by appending to current contents
					int len = strlen(s) + strlen(vOn) + 2;
					buf = (XML_Char *) calloc(len, sizeof(XML_Char *));

					strcpy(buf, s);
					strcat(buf, " ");
					strcat(buf, vOn);

					props_out[1] = buf;
				}
			}
		}
		else
		{
			if (0 == UT_stricmp(s, vOn))
				props_out[1] = vOff;
		}
	}
	free(props_in);

	// set it either way
	pView->setCharFormat(props_out);

	if (buf)
		free(buf);

	return UT_TRUE;
}

Defun1(toggleBold)
{
	return _toggleSpan(pView, "font-weight", "bold", "normal");
}

Defun1(toggleItalic)
{
	return _toggleSpan(pView, "font-style", "italic", "normal");
}

Defun1(toggleUline)
{
	return _toggleSpan(pView, "text-decoration", "underline", "none", UT_TRUE);
}

Defun1(toggleStrike)
{
	return _toggleSpan(pView, "text-decoration", "line-through", "none", UT_TRUE);
}

Defun0(togglePlain)
{
	// TODO: remove all character-level formatting
	// HYP: explicitly delete it, to get back to defaults, styles
	return UT_TRUE;
}

Defun1(alignLeft)
{
	const XML_Char * properties[] =	{ "text-align", "left", 0};
	pView->setBlockFormat(properties);
	return UT_TRUE;
}

Defun1(alignCenter)
{
	const XML_Char * properties[] =	{ "text-align", "center", 0};
	pView->setBlockFormat(properties);
	return UT_TRUE;
}

Defun1(alignRight)
{
	const XML_Char * properties[] =	{ "text-align", "right", 0};
	pView->setBlockFormat(properties);
	return UT_TRUE;
}

Defun1(alignJustify)
{
	const XML_Char * properties[] =	{ "text-align", "justify", 0};
	pView->setBlockFormat(properties);
	return UT_TRUE;
}

Defun1(singleSpace)
{
	const XML_Char * properties[] =	{ "line-height", "1.0", 0};
	pView->setBlockFormat(properties);
	return UT_TRUE;
}

Defun1(middleSpace)
{
	const XML_Char * properties[] =	{ "line-height", "1.5", 0};
	pView->setBlockFormat(properties);
	return UT_TRUE;
}

Defun1(doubleSpace)
{
	const XML_Char * properties[] =	{ "line-height", "2.0", 0};
	pView->setBlockFormat(properties);
	return UT_TRUE;
}

Defun1(Test_Dump)
{
	pView->Test_Dump();
	return UT_TRUE;
}

/*****************************************************************/
/*****************************************************************/

#ifdef DLGHACK

/*
	Having these platform-specific IFDEFs here is a gruesome HACK.
	The only excuse is that it makes things easier while we figure
	out a better scheme for calling platform-specific dialogs from
	XP code.
*/

/*****************************************************************/
/*****************************************************************/

#ifdef WIN32
#include "ap_Win32App.h"
#include "ap_Win32Frame.h"
#include "gr_Win32Graphics.h"

char * _promptFile(AP_Frame * pFrame, UT_Bool bSaveAs)
{
	return _promptFile(pFrame, bSaveAs, NULL);
}

char * _promptFile(AP_Frame * pFrame, UT_Bool bSaveAs, char * pSuggestedName)
{
	AP_Win32Frame * pWin32Frame = static_cast<AP_Win32Frame *>(pFrame);
	HWND hwnd = pWin32Frame->getTopLevelWindow();

	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for filename

	if( pSuggestedName )
		strcpy(szFile, pSuggestedName);
	else
		szFile[0] = 0;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;

	// display the appropriate dialog box.
	if (bSaveAs)
	{
		if (GetSaveFileName(&ofn)==TRUE)
			return strdup(szFile);
	}
	else
	{
		ofn.Flags |= OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn)==TRUE)
			return strdup(szFile);
	}

	DWORD err = CommDlgExtendedError();
	UT_DEBUGMSG(("Didn't get a file: reason=0x%x\n", err));
	UT_ASSERT(!err);

	return NULL;
}

UT_Bool _chooseFont(AP_Frame * pFrame, FV_View * pView)
{
	/*
	   WARNING: any changes to this function should be closely coordinated
	   with the equivalent logic in Win32Graphics::FindFont()
	*/
	AP_Win32Frame * pWin32Frame = static_cast<AP_Win32Frame *>(pFrame);
	AP_Win32App * pWin32App = static_cast<AP_Win32App *>(pFrame->getApp());
	HWND hwnd = pWin32Frame->getTopLevelWindow();

	CHOOSEFONT cf;				// common dialog box structure
	LOGFONT lf;					// logical font structure

	// Initialize CHOOSEFONT
	memset(&cf, 0, sizeof(CHOOSEFONT));
	memset(&lf, 0, sizeof(lf));
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hwnd;
	cf.lpLogFont = &lf;
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;
	cf.hInstance = pWin32App->getInstance();

	// get current font info from pView
	const XML_Char ** props_in = NULL;
	const XML_Char * s;
	DG_Graphics * pG = pView->getLayout()->getGraphics();

	if (!pView->getCharFormat(&props_in))
		return UT_FALSE;

	// TODO: what about generic family names?
	s = UT_getAttribute("font-family", props_in);
	if (s)
		strcpy(lf.lfFaceName, s);
	else
		cf.Flags |= CF_NOFACESEL;

	s = UT_getAttribute("font-size", props_in);
	if (s)
		lf.lfHeight = -(pG->convertDimension(s));
	else
		cf.Flags |= CF_NOSIZESEL;

	s = UT_getAttribute("font-weight", props_in);
	if (s)
	{
		if (0 == UT_stricmp(s, "bold"))
			lf.lfWeight = 700;
	}
	else
		cf.Flags |= CF_NOSTYLESEL;

	s = UT_getAttribute("font-style", props_in);
	if (s)
	{
		if (0 == UT_stricmp(s, "italic"))
			lf.lfItalic = TRUE;
	}
	else
		cf.Flags |= CF_NOSTYLESEL;

	s = UT_getAttribute("text-decoration", props_in);
	if (s)
	{
		XML_Char*	p = strdup(s);
		UT_ASSERT(p);
		XML_Char*	q = strtok(p, " ");

		while (q)
		{
			if (0 == UT_stricmp(q, "underline"))
				lf.lfUnderline = TRUE;

			else if (0 == UT_stricmp(q, "line-through"))
				lf.lfStrikeOut = TRUE;

			q = strtok(NULL, " ");
		}

		free(p);
	}

	s = UT_getAttribute("color", props_in);
	if (s)
	{
		UT_RGBColor c;
		UT_parseColor(s, c);
		DWORD rgbCurrent;		// current text color

		rgbCurrent = RGB(c.m_red, c.m_grn, c.m_blu);

		cf.rgbColors = rgbCurrent;
	}

	UT_Bool bClobberXLFD = UT_FALSE;

	s = UT_getAttribute("font-xlfd", props_in);
	if (s)
	{
		bClobberXLFD = UT_TRUE;
	}

	free(props_in);

	// raise the dialog
	if (ChooseFont(&cf)==TRUE)
	{
		int i = 0;

		// currently a maximum of eight simultaneous properties
		const XML_Char * props_out[] = {
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL,	// XLFD space, set to null when saving any properties
			0 };

		if (lf.lfFaceName)
		{
			props_out[i] = "font-family";
			props_out[i+1] = lf.lfFaceName;
			i += 2;
		}

		char buf_size[5];

		if (cf.iPointSize)
		{
			sprintf(buf_size, "%dpt", cf.iPointSize/10);
			props_out[i] = "font-size";
			props_out[i+1] = buf_size;
			i += 2;
		}

		if (cf.nFontType & BOLD_FONTTYPE)
		{
			props_out[i] = "font-weight";
			props_out[i+1] ="bold";
			i += 2;
		}
		else if (cf.nFontType & REGULAR_FONTTYPE)
		{
			props_out[i] = "font-weight";
			props_out[i+1] ="normal";
			i += 2;
		}

		if (cf.nFontType & ITALIC_FONTTYPE)
		{
			props_out[i] = "font-style";
			props_out[i+1] ="italic";
			i += 2;
		}
		else
		{
			props_out[i] = "font-style";
			props_out[i+1] ="normal";
			i += 2;
		}

		if ((lf.lfUnderline == TRUE) &&
			(lf.lfStrikeOut == TRUE))
		{
			props_out[i] = "text-decoration";
			props_out[i+1] ="underline line-through";
			i += 2;
		}
		else if (lf.lfUnderline == TRUE)
		{
			props_out[i] = "text-decoration";
			props_out[i+1] ="underline";
			i += 2;
		}
		else if (lf.lfStrikeOut == TRUE)
		{
			props_out[i] = "text-decoration";
			props_out[i+1] ="line-through";
			i += 2;
		}
		else
		{
			props_out[i] = "text-decoration";
			props_out[i+1] ="none";
			i += 2;
		}

		char buf_color[6];

		sprintf(buf_color, "%02x%02x%02x", GetRValue(cf.rgbColors), GetGValue(cf.rgbColors), GetBValue(cf.rgbColors));

		props_out[i] = "color";
		props_out[i+1] = buf_color;
		i += 2;

		if (bClobberXLFD)
		{
			/*
			  XLFD is set to something bogus on Windows so that X will
			  force it self to read from the "updated" CSS-style
			  properties first.
			*/
			props_out[i] = "font-xlfd";
			props_out[i+1] = "hack";
		}

		pView->setCharFormat(props_out);

		return UT_TRUE;
	}

	DWORD err = CommDlgExtendedError();
	UT_DEBUGMSG(("Didn't get a font: reason=0x%x\n", err));
	UT_ASSERT(!err);

	return UT_FALSE;
}

dlg_Answer _askUser(AP_Frame * pFrame, const char * szQ, dlg_Buttons b, int defButton)
{
	dlg_Answer ans = dlg_OK;

	AP_App * pApp = pFrame->getApp();
	UT_ASSERT(pApp);

	const char * szCaption = pApp->getApplicationTitleForTitleBar();

	AP_Win32Frame * pWin32Frame = static_cast<AP_Win32Frame *>(pFrame);
	HWND hwnd = pWin32Frame->getTopLevelWindow();
	UINT flags = (b == dlg_O ? MB_ICONASTERISK : MB_ICONQUESTION);

	switch (b)
	{
	case dlg_O:
		flags |= MB_OK;
		break;

	case dlg_OC:
		flags |= MB_OKCANCEL;
		break;

	case dlg_YN:
		flags |= MB_YESNO;
		break;

	case dlg_YNC:
		flags |= MB_YESNOCANCEL;
		break;

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}

	switch (defButton)
	{
	case 0:
		flags |= MB_DEFBUTTON1;
		break;

	case 1:
		flags |= MB_DEFBUTTON2;
		break;

	case 2:
		flags |= MB_DEFBUTTON3;
		break;

	case 3:
		flags |= MB_DEFBUTTON4;
		break;

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}

	int res = MessageBox(hwnd, szQ, szCaption, flags);

	switch (res)
	{
	case IDCANCEL:
		ans = dlg_CANCEL;
		break;

	case IDNO:
		ans = dlg_NO;
		break;

	case IDOK:
		ans = dlg_OK;
		break;

	case IDYES:
		ans = dlg_YES;
		break;

	case IDABORT:
	case IDIGNORE:
	case IDRETRY:
	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}

	return ans;
}

// TODO: figure out what can be shared here and move it up
UT_Bool _printDoc(AP_Frame * pFrame, FV_View * pView)
{
	AP_Win32Frame * pWin32Frame = static_cast<AP_Win32Frame *>(pFrame);
	HWND hwnd = pWin32Frame->getTopLevelWindow();
	FL_DocLayout* pLayout = pView->getLayout();
	PD_Document * doc = pLayout->getDocument();
	int nPagesInDoc = pLayout->countPages();

	// init print dialog structure
	PRINTDLG pd;
	memset(&pd, 0, sizeof(PRINTDLG));
	pd.lStructSize = sizeof(PRINTDLG);
	pd.hwndOwner = hwnd;
	pd.nFromPage = pd.nMinPage = 1;
	pd.nToPage = pd.nMaxPage = nPagesInDoc;
	pd.Flags = PD_RETURNDC | PD_NOSELECTION;

	// open dialog
	if(!PrintDlg(&pd))
		return UT_FALSE;

	// init doc info struct
	DOCINFO di;
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = doc->getFilename();
	di.lpszOutput = NULL;
	// create a new graphics using this docinfo
	Win32Graphics* ppG = new Win32Graphics(pd.hDC, &di);

	// Create a new layout using the printer's graphics and format it
	FL_DocLayout* pDL = new FL_DocLayout(doc, ppG);
	pDL->formatAll();

	// Create the new view for the printer
	FV_View* pV = new FV_View(NULL, pDL);	// TODO: fix first arg?

	// page range implementation
	WORD nFromPage, nToPage;
	if(pd.Flags & PD_PAGENUMS)
	{
		nFromPage = pd.nFromPage;
		nToPage = pd.nToPage;
		// The dialog takes care of page range bounds issues
	}
	else	// print whole document
	{
		nFromPage = 1;
		nToPage = pDL->countPages();
	}

	// init some dg_DrawArgs for startPage
	dg_DrawArgs da;
	da.pG = NULL;
	da.width = pDL->getWidth();
	da.height = pDL->getHeight();

	ppG->startPrint();
	for(int i = nFromPage -1; i < nToPage; i++)	//page numbers are zero based
	{
		ppG->startPage(doc->getFilename(), i, TRUE, da.width,da.height);
		pV->draw(i, &da);
	}
	ppG->endPrint();

	// clean up
	delete pV;
	delete pDL;
	delete ppG;
	DeleteDC(pd.hDC);
	// free any memory allocated by StartDoc
	if(pd.hDevMode != NULL)
		GlobalFree(pd.hDevMode);
	if(pd.hDevNames != NULL)
		GlobalFree(pd.hDevNames);

	return UT_TRUE;
}

#endif /* WIN32 */


/*****************************************************************/
/*****************************************************************/

#ifdef LINUX

#include "ap_UnixApp.h"
#include "ap_UnixFrame.h"
#include "ps_Graphics.h"

static void set_ok (GtkWidget * /*widget*/, UT_Bool *dialog_result)
{
	*dialog_result = TRUE;
	gtk_main_quit();
}

char * _promptFile(AP_Frame *pFrame, UT_Bool bSaveAs)
{
	return _promptFile(pFrame, bSaveAs, NULL);
}

char * _promptFile(AP_Frame * /*pFrame*/, UT_Bool bSaveAs, char * pSuggestedName)
{
	GtkFileSelection *pFS;
	UT_Bool accepted = FALSE;
	char * fileName = NULL;

	pFS = (GtkFileSelection *)gtk_file_selection_new(bSaveAs ? "Save file" : "Open File");

	/* Connect the signals for Ok and Cancel */
	gtk_signal_connect(GTK_OBJECT(pFS->ok_button), "clicked",
					   GTK_SIGNAL_FUNC(set_ok), &accepted);
	gtk_signal_connect(GTK_OBJECT(pFS->cancel_button), "clicked",
					   GTK_SIGNAL_FUNC(gtk_main_quit), NULL);

	// Do we really want to position at the cursor?
	//gtk_window_position(GTK_WINDOW(pFS), GTK_WIN_POS_MOUSE);

	gtk_file_selection_hide_fileop_buttons(pFS);
	// fill in suggested filename
	if(pSuggestedName)
	{
		gtk_file_selection_set_filename(pFS, pSuggestedName);
	}

	/* Run the dialog */
	gtk_widget_show(GTK_WIDGET(pFS));
	gtk_grab_add(GTK_WIDGET(pFS));
	gtk_main();

	if (accepted)
	{
		UT_cloneString(fileName, gtk_file_selection_get_filename(pFS));
	}

	gtk_widget_destroy (GTK_WIDGET(pFS));

	return fileName;
}

UT_Bool _chooseFont(AP_Frame * pFrame, FV_View * pView)
{
	GtkFontSelectionDialog * cf;
	UT_Bool accepted = FALSE;
	gchar * selectedFont = NULL;

	// TODO move this title to resources?
	cf = (GtkFontSelectionDialog *) gtk_font_selection_dialog_new("Font Selection");

    // Connect the signals to the buttons
	gtk_signal_connect(GTK_OBJECT(cf->ok_button),
					   "clicked",
					   GTK_SIGNAL_FUNC(set_ok),
					   &accepted);
	gtk_signal_connect(GTK_OBJECT(cf->cancel_button),
					   "clicked",
					   GTK_SIGNAL_FUNC(gtk_main_quit),
					   NULL);
	// TIP: bind something (null at least) to "destroy" or you'll
	// never get out of gtk_main();
	gtk_signal_connect(GTK_OBJECT(cf),
					   "destroy",
					   GTK_SIGNAL_FUNC(NULL),
					   NULL);
	// Do we really want to position a new window at the cursor?
	//gtk_window_position(GTK_WINDOW(cf), GTK_WIN_POS_MOUSE);


	// We're now ready to query the view for its properties, to set
	// the font dialog's pop-up appearance to match.
	const XML_Char ** props_in = NULL;
	const XML_Char * s;
	DG_Graphics * pG = pView->getLayout()->getGraphics();

	if (!pView->getCharFormat(&props_in))
		return UT_FALSE;

	// TODO set the proper length.
	// This length is completely arbitrary.  If you know the max X font
	// descriptor length, please set this array to that size and check
	// accordingly.
	gchar * fontString = new gchar[1024];
	fontString[0] = NULL;

	/*
	  logic note: if we have an XLFD, we use it to hint to the GTK+
	  dialog.  If we don't have one, we build a substitute from the
	  CSS-style attributes.
	*/
	s = UT_getAttribute("font-xlfd", props_in);
	if (s)
	{
		if (UT_stricmp(s, ""))
			strcpy(fontString, s);
	}
	else
	{
		/* This is where the hairy code is */

		// we don't have a "foundry" attribute to match X's, fake it
		strcat(fontString, "-*");

		// family is *,[name]
		s = UT_getAttribute("font-family", props_in);
		if (s)
		{
			if (!UT_stricmp(s, ""))
			{
				// this satisfies the condition the view doesn't have
				// a family set
				strcat(fontString, "-");
				strcat(fontString, "Times");
			}
			else
			{
				strcat(fontString, "-");
				strcat(fontString, s);
			}
		}
		else
			// this is kind of redundant, fix it
			strcat(fontString, "-*");

		// weight is *,black,bold,demibold,medium,regular
		s = UT_getAttribute("font-weight", props_in);
		if (s)
		{
			if (!UT_stricmp(s, "bold"))
				strcat(fontString, "-bold");
			else
				strcat(fontString, "-medium");
		}
		else
			strcat(fontString, "-*");

		// slant is *,i,o,r
		s = UT_getAttribute("font-style", props_in);
		if (s)
		{
			if (!UT_stricmp(s, "italic"))
				strcat(fontString, "-i");
			else
				strcat(fontString, "-r");
		}
		else
			strcat(fontString, "-*");

		// sWidth
		strcat(fontString, "-*");
		// adStyle
		strcat(fontString, "-*");

		// pxlsz (we use points)
		strcat(fontString, "-*");

		// point size
		s = UT_getAttribute("font-size", props_in);
		if (s)
		{
			strcat(fontString, "-");
			char fontSize[5];
			snprintf(fontSize, 5, "%d", (pG->convertDimension(s)) * 10);
			UT_DEBUGMSG(("font is size %s\n", fontSize));
			strcat(fontString, fontSize);
		}
		else
			// is this the default size?
			strcat(fontString, "-12");

		// Fill in the last 6 attributes we don't touch
		strcat(fontString, "-*");
		strcat(fontString, "-*");
		strcat(fontString, "-*");
		strcat(fontString, "-*");
		strcat(fontString, "-*");
		strcat(fontString, "-*");

		UT_DEBUGMSG(("Priming with XLFD: [%s]\n", fontString));

		// TODO make this work
/*
  s = UT_getAttribute("text-decoration", props_in);
  if (s)
  {
  XML_Char*	p = strdup(s);
  UT_ASSERT(p);
  XML_Char*	q = strtok(p, " ");

  while (q)
  {
  if (0 == UT_stricmp(q, "underline"))
  lf.lfUnderline = TRUE;

  else if (0 == UT_stricmp(q, "line-through"))
  lf.lfStrikeOut = TRUE;

  q = strtok(NULL, " ");
  }

  free(p);
  }
*/

		// TODO color in X, GTK font dialog doesn't support
		// color, we do that seperately.
/*
  s = UT_getAttribute("color", props_in);
  if (s)
  {
  UT_RGBColor c;
  UT_parseColor(s, c);
  DWORD rgbCurrent;		// current text color

  rgbCurrent = RGB(c.m_red, c.m_grn, c.m_blu);

  cf.rgbColors = rgbCurrent;
  }
*/
	}
	if (!gtk_font_selection_dialog_set_font_name(cf, fontString))
	{
		// We couldn't set the name, which means we don't have it
		// TODO maybe do substitution, does it matter?
		UT_DEBUGMSG(("Couldn't hint to font selection dialog to use XLFD.  "
					 "This XLFD is probably not valid.\n"));
	}

	// Set up a nice sample string
	gchar * sampleString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijlkmnopqrstuvwxyz";
	gtk_font_selection_dialog_set_preview_text(cf, (const gchar *) sampleString);

	/* Run the dialog */
	gtk_widget_show(GTK_WIDGET(cf));
	gtk_grab_add(GTK_WIDGET(cf));
	gtk_main();

	if (accepted)
	{

		selectedFont = gtk_font_selection_dialog_get_font_name(cf);

		if (selectedFont)
		{

			UT_DEBUGMSG(("Font selection returned [%s].\n\n", selectedFont));

			// *************************************************************
			// BIG ASS HACK ALERT - we can't just access the props throgh
			// and ever-incrementing [i] index, since it doesn't match
			// the order in which we tokenize our X string!  I'm using simple
			// numbers, but as the props may change, we should really
			// use enumerated constants.
			// *************************************************************

			// currently a maximum of six simultaneous properties
			const XML_Char * props_out[] = {
				NULL, NULL, 	// 0,1   font family
				NULL, NULL, 	// 2,3   point size
				NULL, NULL, 	// 4,5   weight
				NULL, NULL, 	// 6,7   slant
				NULL, NULL, 	// 8,9   decoration (strike or underline)
				NULL, NULL, 	// 10,11 xlfd
				NULL, NULL,		// 12,13 color (not used here)
				0 };

			// we always store the XLFD
			props_out[10] = "font-xlfd";
			props_out[11] = selectedFont;

			// duplicate and tokenize the XLFD
			gchar * cloneString = strdup(selectedFont);
			UT_ASSERT(cloneString);

			// first call gets foundry, which we ignore
			gchar * token = strtok(cloneString, "-");

			// font family name
			if ((token = strtok(NULL, "-")))
			{
				props_out[0] = "font-family";
				props_out[1] = token;
			}

			/* weight (X has lots of defined weights, we just
			   cast them to bold or normal for now)
			*/
			if ((token = strtok(NULL, "-")))
			{
				if (!UT_stricmp(token, "black"))
				{
					props_out[4] = "font-weight";
					props_out[5] = "bold";
				}
				else if (!UT_stricmp(token, "bold"))
				{
					props_out[4] = "font-weight";
					props_out[5] = "bold";
				}
				else if (!UT_stricmp(token, "demibold"))
				{
					props_out[4] = "font-weight";
					props_out[5] = "bold";
				}
				else if (!UT_stricmp(token, "medium"))
				{
					props_out[4] = "font-weight";
					props_out[5] = "normal";
				}
				else if (!UT_stricmp(token, "regular"))
				{
					props_out[4] = "font-weight";
					props_out[5] = "normal";
				}
				else
				{
					props_out[4] = "font-weight";
					props_out[5] = "normal";
				}
			}

			// slant (X has i,o,r, which we cast to italic or normal)
			if ((token = strtok(NULL, "-")))
			{
				if (!UT_stricmp(token, "i"))
				{
					props_out[6] = "font-style";
					props_out[7] ="italic";
				}
				else // o and r
				{
					props_out[6] = "font-style";
					props_out[7] ="normal";
				}
			}

// TODO: make these work
/*
   if ((lf.lfUnderline == TRUE) &&
   (lf.lfStrikeOut == TRUE))
   {
   props_out[i] = "text-decoration";
   props_out[i+1] ="underline line-through";
   i += 2;
   }
*/
			// sWidth
			strtok(NULL, "-");
			// adStyle
			strtok(NULL, "-");
			// pxlStyle
			strtok(NULL, "-");

			// point size
			char buf_size[5];
			if ((token = strtok(NULL, "-")))
			{
				sprintf(buf_size, "%dpt", (atoi(token)/10));
				props_out[2] = "font-size";
				props_out[3] = buf_size;
			}

			// dummies
			props_out[8] = "text_decoration";
			props_out[9] = "none";

			char buf_color[6];
			sprintf(buf_color, "%02x%02x%02x", 0, 0, 0);
			props_out[12] = "color";
			props_out[13] = buf_color;

			// apply changes
			pView->setCharFormat(props_out);

			if (cloneString)
				free(cloneString);
		}
	}

	gtk_widget_destroy (GTK_WIDGET(cf));

	delete fontString;
	return UT_TRUE;

}

static void message_box_ok_clicked(GtkWidget * widget, dlg_Answer * answer)
{
	*answer = dlg_OK;
	gtk_main_quit();
}
static void message_box_cancel_clicked(GtkWidget * widget, dlg_Answer * answer)
{
	*answer = dlg_CANCEL;
	gtk_main_quit();
}
static void message_box_yes_clicked(GtkWidget * widget, dlg_Answer * answer)
{
	*answer = dlg_YES;
	gtk_main_quit();
}
static void message_box_no_clicked(GtkWidget * widget, dlg_Answer * answer)
{
	*answer = dlg_NO;
	gtk_main_quit();
}

dlg_Answer _askUser(AP_Frame * pFrame, const char * szQ, dlg_Buttons b, int defButton)
{
	AP_App * pApp = pFrame->getApp();
	UT_ASSERT(pApp);

	const char * szCaption = pApp->getApplicationTitleForTitleBar();

	// New GTK+ dialog window
	GtkWidget * dialog_window = gtk_dialog_new();
	gtk_signal_connect_after (GTK_OBJECT (dialog_window),
							  "destroy",
							  GTK_SIGNAL_FUNC(message_box_cancel_clicked),
							  NULL);

	gtk_window_set_title (GTK_WINDOW (dialog_window), szCaption);
	gtk_container_border_width (GTK_CONTAINER (dialog_window), 0);
	gtk_widget_set_usize (dialog_window, 400, 110);

	// Add our label string to the dialog in the message area
	GtkWidget * label = gtk_label_new (szQ);
	gtk_misc_set_padding (GTK_MISC (label), 10, 10);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->vbox),
						label, TRUE, TRUE, 0);
	gtk_widget_show (label);

	// Build all the buttons, regardless of whether they're
	// used.  This is much easier than creating the ones we know we
	// need.  Trust me.
	dlg_Answer answer;
	GtkWidget * ok_button;
	GtkWidget * cancel_button;
	GtkWidget * yes_button;
	GtkWidget * no_button;

	// OK
	ok_button = gtk_button_new_with_label ("OK");
	gtk_signal_connect (GTK_OBJECT (ok_button),
						"clicked",
						GTK_SIGNAL_FUNC (message_box_ok_clicked),
						&answer);
	GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);
	// Cancel
	cancel_button = gtk_button_new_with_label ("Cancel");
	gtk_signal_connect (GTK_OBJECT (cancel_button),
						"clicked",
						GTK_SIGNAL_FUNC (message_box_cancel_clicked),
						&answer);
	GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);
	// Yes
	yes_button = gtk_button_new_with_label ("Yes");
	gtk_signal_connect (GTK_OBJECT (yes_button),
						"clicked",
						GTK_SIGNAL_FUNC (message_box_yes_clicked),
						&answer);
	GTK_WIDGET_SET_FLAGS (yes_button, GTK_CAN_DEFAULT);
	// No
	no_button = gtk_button_new_with_label ("No");
	gtk_signal_connect (GTK_OBJECT (no_button),
						"clicked",
						GTK_SIGNAL_FUNC (message_box_no_clicked),
						&answer);
	GTK_WIDGET_SET_FLAGS (no_button, GTK_CAN_DEFAULT);

	switch (b)
	{
	case dlg_O:
		// OK
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
							ok_button, TRUE, TRUE, 0);
		if (defButton == 0)
			gtk_widget_grab_default (ok_button);
		gtk_widget_show (ok_button);

		break;

	case dlg_OC:
		// OK
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
							ok_button, TRUE, TRUE, 0);
		if (defButton == 0)
			gtk_widget_grab_default (ok_button);
		gtk_widget_show (ok_button);
		// Cancel
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
							cancel_button, TRUE, TRUE, 0);
		if (defButton == 1)
			gtk_widget_grab_default (cancel_button);
		gtk_widget_show (cancel_button);

		break;

	case dlg_YN:
		// Yes
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
							yes_button, TRUE, TRUE, 0);
		if (defButton == 0)
			gtk_widget_grab_default (yes_button);
		gtk_widget_show (yes_button);
		// No
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
							no_button, TRUE, TRUE, 0);
		if (defButton == 1)
			gtk_widget_grab_default (no_button);
		gtk_widget_show (no_button);

		break;

	case dlg_YNC:
		// Yes
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
							yes_button, TRUE, TRUE, 0);
		if (defButton == 0)
			gtk_widget_grab_default (yes_button);
		gtk_widget_show (yes_button);
		// No
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
							no_button, TRUE, TRUE, 0);
		if (defButton == 1)
			gtk_widget_grab_default (no_button);
		gtk_widget_show (no_button);
		// Cancel
		gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area),
							cancel_button, TRUE, TRUE, 0);
		if (defButton == 2)
			gtk_widget_grab_default (cancel_button);
		gtk_widget_show (cancel_button);

		break;

	default:
		UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}

	gtk_grab_add(GTK_WIDGET(dialog_window));
	gtk_widget_show (dialog_window);

	// TODO maybe add some key bindings so that escape will cancel, etc.
	gtk_main();

	gtk_widget_destroy(label);
	gtk_widget_destroy(ok_button);
	gtk_widget_destroy(cancel_button);
	gtk_widget_destroy(yes_button);
	gtk_widget_destroy(no_button);
	gtk_widget_destroy(GTK_WIDGET(dialog_window));

	// answer should be set by the appropriate callback
	return answer;
}

UT_Bool _printDoc(AP_Frame * pFrame, FV_View * pView)
{
	FL_DocLayout* pLayout = pView->getLayout();
	PD_Document * doc = pLayout->getDocument();

	char* title = (char *) doc->getFilename();	// TODO get a real title

	// get the filename with no path
	char* lastslash = strrchr(title, '/');
	lastslash++;
	char *filename;
	filename = (char *) malloc( (strlen(lastslash) + 4) * sizeof(char *) );
	strcpy(filename, lastslash);
	strcat(filename, ".ps");

	char * pNewFile = _promptFile(pFrame, UT_TRUE, filename);

	// create a new graphics
	PS_Graphics* ppG = new PS_Graphics(pNewFile, title, "AbiWord");
	UT_ASSERT(ppG);

	// Create a new layout using the printer's graphics and format it
	FL_DocLayout* pDL = new FL_DocLayout(doc, ppG);
	pDL->formatAll();
	int nPagesInDoc = pDL->countPages();

	// Create the new view for the printer
	FV_View* pV = new FV_View(NULL, pDL);	// TODO: fix first arg?

	dg_DrawArgs da;
	da.pG = NULL;
	// TODO -- really need actual page width/height for each page.
	da.xoff = da.yoff = 0;
	da.width = pDL->getWidth()/nPagesInDoc;
	da.height = pDL->getHeight()/nPagesInDoc;

	ppG->startPrint();

	for(int i = 0; i < nPagesInDoc; i++)	//page numbers are zero based
	{
		ppG->startPage(doc->getFilename(), i, TRUE,da.width, da.height);
		pV->draw(i, &da);
	}
	ppG->endPrint();

	free(filename);
	return UT_TRUE;
}

#endif /* LINUX */

/*****************************************************************/
/*****************************************************************/

#endif /* DLGHACK */
