/* AbiWord
 * Copyright (C) 1998,1999 AbiSource, Inc.
 * BIDI Copyright (c) 2001,2002 Tomas Frydrych
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


#include "fb_Alignment.h"
#include "fp_Line.h"
#include "fp_Run.h"
#include "fp_TextRun.h"

#include "ut_debugmsg.h"
#include "ut_assert.h"

/////////////////////////////////////////////////////////////
// Alignment left
/////////////////////////////////////////////////////////////

void fb_Alignment_left::initialize(fp_Line * pLine )
{
		if(pLine->getBlock()->getDominantDirection() == FRIBIDI_TYPE_RTL)
		{
	    	m_iStartPosition = 0 - pLine->calculateWidthOfTrailingSpaces();
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
			m_iStartPositionLayoutUnits = 0 - pLine->calculateWidthOfTrailingSpacesInLayoutUnits();
#endif
	 	}
	 	else
	 	{
	 		m_iStartPosition = 0;
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	 		m_iStartPositionLayoutUnits = 0;
#endif
		}
}

UT_sint32 fb_Alignment_left::getStartPosition()
{
	return m_iStartPosition;
}

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32 fb_Alignment_left::getStartPositionInLayoutUnits()
{
	return m_iStartPositionLayoutUnits;
}
#endif

void fb_Alignment_left::eraseLineFromRun(fp_Line *pLine, UT_uint32 runIndex)
{
	pLine->clearScreenFromRunToEnd(runIndex);
}

/////////////////////////////////////////////////////////////
// Alignment center
/////////////////////////////////////////////////////////////

void fb_Alignment_center::initialize(fp_Line *pLine)
{
	UT_sint32 iWidth = pLine->calculateWidthOfLine();

	UT_sint32 m_iExtraWidth = pLine->getMaxWidth() - iWidth;

	m_startPosition = m_iExtraWidth / 2;
	
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	UT_sint32 iWidthLayoutUnits = pLine->calculateWidthOfLineInLayoutUnits();

	UT_sint32 m_iExtraWidthLayoutUnits = pLine->getMaxWidthInLayoutUnits() - iWidthLayoutUnits;

	m_startPositionLayoutUnits = m_iExtraWidthLayoutUnits / 2;
#endif
}

UT_sint32 fb_Alignment_center::getStartPosition()
{
	return m_startPosition;
}

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32 fb_Alignment_center::getStartPositionInLayoutUnits()
{
	return m_startPositionLayoutUnits;
}
#endif

void fb_Alignment_center::eraseLineFromRun(fp_Line *pLine, UT_uint32 runIndex)
{
	pLine->clearScreen();
}

/////////////////////////////////////////////////////////////
// Alignment right
/////////////////////////////////////////////////////////////

void fb_Alignment_right::initialize(fp_Line *pLine)
{
	UT_sint32 iTrailing = pLine->calculateWidthOfTrailingSpaces();
	UT_sint32 iWidth = pLine->calculateWidthOfLine() - iTrailing;

	m_startPosition = pLine->getMaxWidth() - iWidth;
	
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
    UT_sint32 iTrailingLayoutUnits = pLine->calculateWidthOfTrailingSpacesInLayoutUnits();
	UT_sint32 iWidthLayoutUnits = pLine->calculateWidthOfLineInLayoutUnits() - iTrailingLayoutUnits;

	m_startPositionLayoutUnits = pLine->getMaxWidthInLayoutUnits() - iWidthLayoutUnits;
#endif

	if(pLine->getBlock()->getDominantDirection() == FRIBIDI_TYPE_RTL)
	{
		m_startPosition -= iTrailing;
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
		m_startPositionLayoutUnits -= iTrailingLayoutUnits;
#endif
	}
}

UT_sint32 fb_Alignment_right::getStartPosition()
{
	return m_startPosition;
}

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32 fb_Alignment_right::getStartPositionInLayoutUnits()
{
	return m_startPositionLayoutUnits;
}
#endif

void fb_Alignment_right::eraseLineFromRun(fp_Line *pLine, UT_uint32 runIndex)
{
	pLine->clearScreenFromRunToEnd(runIndex);
}

/////////////////////////////////////////////////////////////
// Alignment justify
/////////////////////////////////////////////////////////////

// If first line in block then spaces at the beginning of the
// line should be included and spaces at the end of the line
// should be excluded.

// If last line in block then no justification is required unless
// this is a RTL dominant block, in which case justify right

// Otherwise spaces at start and end of the lines should be ignored.

void fb_Alignment_justify::initialize(fp_Line *pLine)
{
	if (!pLine->isLastLineInBlock())
	{
	  pLine->resetJustification();

	  UT_sint32 iWidth = pLine->calculateWidthOfLine() - pLine->calculateWidthOfTrailingSpaces();

	  m_iExtraWidth = pLine->getMaxWidth() - iWidth;

	  xxx_UT_DEBUGMSG(("fb_Alignment_justify::initialize (0x%x), iWidth %d, m_iExtraWidth %d\n",this,iWidth,m_iExtraWidth));
	  pLine->distributeJustificationAmongstSpaces(m_iExtraWidth);

	  if(pLine->getBlock()->getDominantDirection() == FRIBIDI_TYPE_RTL)
	  {
	      m_iStartPosition = pLine->getMaxWidth();
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	      m_iStartPositionLayoutUnits = pLine->getMaxWidthInLayoutUnits();
#endif
	  }
	  else
	  {
	      m_iStartPosition = 0;
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	      m_iStartPositionLayoutUnits = 0;
#endif
	  }

#ifndef NDEBUG
	  _confirmJustification(pLine);
#endif

	}
	else if(pLine->getBlock()->getDominantDirection() == FRIBIDI_TYPE_RTL) //this is RTL block, the last line behaves as if right-justified
	{
	    m_iStartPosition = pLine->getMaxWidth();
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	    m_iStartPositionLayoutUnits = pLine->getMaxWidthInLayoutUnits();
#endif
	}
	else
	{
	    xxx_UT_DEBUGMSG(("Justified block, last line, left justified\n"));
	    m_iStartPosition = 0;
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
	    m_iStartPositionLayoutUnits = 0;
#endif
	}
}

UT_sint32 fb_Alignment_justify::getStartPosition()
{
	return m_iStartPosition;
}

#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32 fb_Alignment_justify::getStartPositionInLayoutUnits()
{
	return m_iStartPositionLayoutUnits;
}
#endif

#ifndef NDEBUG

void fb_Alignment_justify::_confirmJustification(fp_Line *pLine)
{
}

#endif /* NDEBUG */

void fb_Alignment_justify::eraseLineFromRun(fp_Line *pLine, UT_uint32 runIndex)
{
	pLine->clearScreenFromRunToEnd(runIndex);
}

