 
/*
** The contents of this file are subject to the AbiSource Public
** License Version 1.0 (the "License"); you may not use this file
** except in compliance with the License. You may obtain a copy
** of the License at http://www.abisource.com/LICENSE/ 
** 
** Software distributed under the License is distributed on an
** "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
** implied. See the License for the specific language governing
** rights and limitations under the License. 
** 
** The Original Code is AbiWord.
** 
** The Initial Developer of the Original Code is AbiSource, Inc.
** Portions created by AbiSource, Inc. are Copyright (C) 1998 AbiSource, Inc. 
** All Rights Reserved. 
** 
** Contributor(s):
**  
*/


#ifndef DG_LAYOUTVIEW_H
#define DG_LAYOUTVIEW_H

#include "ut_types.h"
#include "dg_DocMarker.h"

class FL_DocLayout;
class FL_BlockLayout;
class FP_Page;
class FP_Run;
class DG_DocBuffer;
class DG_Graphics;

enum
{
	DG_SCROLLCMD_PAGEUP,
	DG_SCROLLCMD_PAGEDOWN,
	DG_SCROLLCMD_LINEUP,
	DG_SCROLLCMD_LINEDOWN,
	DG_SCROLLCMD_PAGERIGHT,
	DG_SCROLLCMD_PAGELEFT,
	DG_SCROLLCMD_LINERIGHT,
	DG_SCROLLCMD_LINELEFT,
	DG_SCROLLCMD_TOTOP,
	DG_SCROLLCMD_TOBOTTOM,
	DG_SCROLLCMD_TOPOSITION
};

class DG_LayoutView
{
public:
	DG_LayoutView(FL_DocLayout*);
	void setXScrollOffset(UT_sint32);
	void setYScrollOffset(UT_sint32);
	void setWindowSize(UT_sint32, UT_sint32);
	void draw();
	void draw(UT_sint32, UT_sint32, UT_sint32, UT_sint32);

	// TODO some of these functions should move into protected
	
	void moveInsPtToBOD();
	void getPageScreenOffsets(FP_Page* pPage, UT_sint32& xoff, UT_sint32& yoff, UT_sint32& width, UT_sint32& height);
	void getPageYOffset(FP_Page* pPage, UT_sint32& yoff);
	void invertBetweenPositions(UT_uint32 left, UT_uint32 right);

	void insertParagraphBreak();
	void insertCharacterFormatting(const XML_Char * properties[]);

	void cmdScroll(UT_sint32 iScrollCmd, UT_uint32 iPos = 0);
	void addScrollListener(void (*pfn)(UT_sint32, UT_sint32));
	void removeScrollListener(void (*pfn)(UT_sint32, UT_sint32));
	void sendScrollEvent(UT_sint32 xoff, UT_sint32 yoff);

	void cmdAlignBlock(UT_uint32 iAlignCmd);

// ----------------------
	void			cmdSelectWord(UT_sint32 xPos, UT_sint32 yPos);
	void			cmdCharMotion(UT_Bool bForward, UT_uint32 count);
	UT_Bool			cmdCharInsert(UT_UCSChar * text, UT_uint32 count);
	void			cmdCharDelete(UT_Bool bForward, UT_uint32 count);
	
	void			warpInsPtToXY(UT_sint32 xPos, UT_sint32 yPos);
	void		    moveInsPtToBOL(void);
	void		    moveInsPtToEOL(void);
	void			warpInsPtNextPrevLine(UT_Bool bNext);
	void			extSelHorizontal(UT_Bool bForward, UT_uint32 count);
	void			extSelToXY(UT_sint32 xPos, UT_sint32 yPos);
	void			extSelNextPrevLine(UT_Bool bNext);

	void			Test_Dump(void);	/* TODO remove this */
// ----------------------
	
protected:
	void			    _moveInsPtNextPrevLine(UT_Bool bNext);

	void 				_findPositionCoords(UT_uint32 pos,
											UT_Bool b,
											UT_uint32& x,
											UT_uint32& y,
											UT_uint32& height,
											FL_BlockLayout** ppBlock,
											FP_Run** ppRun);
	FL_BlockLayout* 	_findBlockWithBufferPosition(UT_uint32 pos);
	UT_Bool				_isSelectionEmpty();
	void				_moveToSelectionEnd(UT_Bool bForward);
	void				_clearSelection(void);
	void				_eraseSelection(void);
	void				_resetSelection(void);
	void				_setSelectionAnchor(void);
	void				_deleteSelection(void);
	UT_Bool				_insertFormatPair(const XML_Char * szName, const XML_Char * properties[]);
	void 				_updateInsertionPoint();
	void 				_xorInsertionPoint();
	void 				_eraseInsertionPoint();
	void				_eraseSelectionOrInsertionPoint();
	void				_drawSelectionOrInsertionPoint();
	void				_xorSelection();
	void				_swapSelectionOrientation(void);
	
	UT_uint32			m_xPoint;
	UT_uint32			m_yPoint;
	UT_uint32			m_iPointHeight;
	
	UT_Bool				m_bPointVisible;
	UT_Bool				m_bInsPointRight;
	
	UT_Bool				m_bSelectionVisible;
	FL_DocLayout*		m_pLayout;
	DG_DocBuffer*		m_pBuffer;
	DG_Graphics*		m_pG;
	UT_sint32			m_xScrollOffset;
	UT_sint32			m_yScrollOffset;
	UT_sint32			m_iWindowHeight;
	UT_sint32			m_iWindowWidth;
	UT_uint32			m_iSelectionAnchor;
	UT_Bool				m_bSelection;
	UT_Vector           m_scrollListeners;
};

#endif /* DG_LAYOUTVIEW_H */
