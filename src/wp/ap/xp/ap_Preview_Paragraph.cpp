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

#include <stdlib.h>
#include <stdio.h>

#include "ut_debugmsg.h"
#include "ut_assert.h"
#include "ut_types.h"
#include "ut_string.h"

#include "xap_Preview.h"
#include "ap_Preview_Paragraph.h"

/************************************************************************/

// all of these are measured in pixels

#define DEFAULT_TOP_MARGIN 5

#define DEFAULT_LEFT_STOP 20
#define DEFAULT_RIGHT_STOP 20

#define DEFAULT_BEFORE_SPACING 0
#define DEFAULT_AFTER_SPACING 0
#define DEFAULT_LINE_SPACING 0

AP_Preview_Paragraph_Block::AP_Preview_Paragraph_Block(UT_RGBColor & clr,
													   GR_Graphics * gc,
													   AP_Dialog_Paragraph::tAlignState m_align,
													   UT_uint32 fontHeight)
{
	UT_ASSERT(gc);
	
	m_clr.m_red = clr.m_red;
	m_clr.m_grn = clr.m_grn;
	m_clr.m_blu = clr.m_blu;

	m_gc = gc;

	m_firstLineLeftStop = DEFAULT_LEFT_STOP;
	m_leftStop = DEFAULT_LEFT_STOP;
	m_rightStop = DEFAULT_RIGHT_STOP;

	m_beforeSpacing = DEFAULT_BEFORE_SPACING;
	m_afterSpacing = DEFAULT_AFTER_SPACING;
	m_lineSpacing = DEFAULT_LINE_SPACING;

	m_align = AP_Dialog_Paragraph::align_LEFT;
	m_indent = AP_Dialog_Paragraph::indent_NONE;
	m_spacing = AP_Dialog_Paragraph::spacing_SINGLE;
	
	m_fontHeight = fontHeight;
}

AP_Preview_Paragraph_Block::~AP_Preview_Paragraph_Block()
{
	// clear out our vector by freeing ONLY the first
	// word.  All the pointers in the vector point
	// to different offsets of one piece of memory allocated
	// all at once.
	if (m_words.getItemCount() > 0)
	{
		UT_UCSChar * word = (UT_UCSChar *) m_words.getNthItem(0);
		FREEP(word);
	}
}

void AP_Preview_Paragraph_Block::setText(const UT_UCSChar * text)
{
	UT_ASSERT(text);

	// clear the words vector by freeing ONLY the first
	// word (see the destructor for notes)
	if (m_words.getItemCount() > 0)
	{
		UT_UCSChar * word = (UT_UCSChar *) m_words.getNthItem(0);
		FREEP(word);
		m_words.clear();
	}

	// clear the widths vector (has no memory at each item)
	m_widths.clear();
	
	// dup the string for harmful chunkification
	UT_UCSChar * clone = NULL;
	UT_UCS_cloneString(&clone, text);
	
	UT_UCSChar * i = clone;

	while (*i != 0)
	{
		// for each space
		if (*i == ' ')
		{
			// terminate this word
			*i = 0;

			// add clone item
			m_words.addItem(clone);

			// measure clone item
			m_widths.addItem((void *) m_gc->measureString(clone, 0, UT_UCS_strlen(clone), NULL));

			// advance clone pointer for new word
			clone = i + 1;
		}
		i++;
	}

	// add last word
	m_words.addItem(clone);
	// measure last word
	m_widths.addItem((void *) m_gc->measureString(clone, 0, UT_UCS_strlen(clone), NULL));
}

// ignores NULL parameters, otherwise scales dimensioned strings into
// pixel constants for drawing this block.  It's 72/2, since we'll
// be converting all units to inches, then to points (72 dpi),
// and cutting in half (/2) for mini-view

#define DIMENSION_INCH_SCALE_FACTOR	36

#define STORE_CONVERTED(m, v) \
            if (v) m = (UT_uint32) (UT_convertToInches(v) * DIMENSION_INCH_SCALE_FACTOR);

void AP_Preview_Paragraph_Block::setFormat(AP_Dialog_Paragraph::tAlignState align,
										   const XML_Char * firstLineIndent,
										   AP_Dialog_Paragraph::tIndentState indent,
										   const XML_Char * leftMargin,
										   const XML_Char * rightMargin,
										   const XML_Char * beforeSpacing,
										   const XML_Char * afterSpacing,
										   const XML_Char * lineSpacing,
										   AP_Dialog_Paragraph::tSpacingState spacing)
{	
	m_align = align;

	if (firstLineIndent)
	{
		m_indent = indent;
		switch (m_indent)
		{
		case AP_Dialog_Paragraph::indent_FIRSTLINE:
			m_firstLineLeftStop = m_leftStop + ((UT_uint32) UT_convertToInches(firstLineIndent)
												* DIMENSION_INCH_SCALE_FACTOR);
			break;
		case AP_Dialog_Paragraph::indent_HANGING:
			m_firstLineLeftStop = m_leftStop - ((UT_uint32) UT_convertToInches(firstLineIndent)
												* DIMENSION_INCH_SCALE_FACTOR);
			break;
		case AP_Dialog_Paragraph::indent_NONE:
			m_firstLineLeftStop = m_leftStop;
			break;
		default:
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		}
	}
	
	STORE_CONVERTED(m_firstLineLeftStop, firstLineIndent);

	
	STORE_CONVERTED(m_leftStop, leftMargin);
	STORE_CONVERTED(m_rightStop, rightMargin);
	STORE_CONVERTED(m_beforeSpacing, beforeSpacing);
	STORE_CONVERTED(m_afterSpacing, afterSpacing);

	if (lineSpacing)
	{
		m_spacing = spacing;
		switch (m_spacing)
		{
		case AP_Dialog_Paragraph::spacing_SINGLE:
			m_lineSpacing = 0;
			break;
		case AP_Dialog_Paragraph::spacing_ONEANDHALF:
			m_lineSpacing = (UT_uint32) ((double) m_fontHeight * 0.5);
			break;
		case AP_Dialog_Paragraph::spacing_DOUBLE:
			m_lineSpacing = m_fontHeight;
			break;
		case AP_Dialog_Paragraph::spacing_ATLEAST:
			// TODO : is this right?  I think we just use what they asked for
			// TODO : unless it's less than the height of a line
			if (((UT_uint32) UT_convertToInches(lineSpacing) * DIMENSION_INCH_SCALE_FACTOR) > m_fontHeight)
				m_lineSpacing = ((UT_uint32) UT_convertToInches(lineSpacing)
								 * DIMENSION_INCH_SCALE_FACTOR);
			break;
		case AP_Dialog_Paragraph::spacing_EXACTLY:
			// TODO : is this right?  I think we just use the spacing they asked
			// TODO : for.  If it's less than one line height, oh well.
			m_lineSpacing = ((UT_uint32) UT_convertToInches(lineSpacing)
								 * DIMENSION_INCH_SCALE_FACTOR);
		case AP_Dialog_Paragraph::spacing_MULTIPLE:
			m_lineSpacing = m_fontHeight * ((UT_uint32) UT_convertDimensionless(lineSpacing) - 1);
			break;
		}
	}

}
		

/************************************************************************/

// TODO : LOCALIZE THESE!

#define STRING_PREVIOUS_BLOCK "Previous Paragraph Previous Paragraph Previous Paragraph Previous Paragraph Previous Paragraph Previous Paragraph Previous Paragraph"
#define STRING_FOLLOWING_BLOCK "Following Paragraph Following Paragraph Following Paragraph Following Paragraph Following Paragraph Following Paragraph Following Paragraph"

AP_Preview_Paragraph::AP_Preview_Paragraph(GR_Graphics * gc,
										   const UT_UCSChar * text,
										   AP_Dialog_Paragraph * dlg)
	: XAP_Preview(gc)
{
	UT_ASSERT(text && dlg);

	m_dlg = dlg;
	m_font = NULL;
	m_fontHeight = 0;
	
	m_y = DEFAULT_TOP_MARGIN;
		
	m_clrWhite = new UT_RGBColor(255,255,255);
	m_clrBlack = new UT_RGBColor(0,0,0);
	m_clrGray = new UT_RGBColor(192,192,192);

	// initialize font to start measuring with for following setText calls
	_loadDrawFont();
	
	{
		// this block is a dummy block
		m_previousBlock = new AP_Preview_Paragraph_Block(*m_clrGray,
														 m_gc,
														 AP_Dialog_Paragraph::align_LEFT,
														 m_fontHeight);
		m_previousBlock->setFormat(AP_Dialog_Paragraph::align_LEFT,
								   NULL,
								   AP_Dialog_Paragraph::indent_NONE,
								   NULL,NULL,NULL,NULL,NULL,
								   AP_Dialog_Paragraph::spacing_SINGLE);
	}
	
	{
		// this block is our ACTIVE block
		m_activeBlock = new AP_Preview_Paragraph_Block(*m_clrBlack,
													   m_gc,
													   AP_Dialog_Paragraph::align_LEFT,
													   m_fontHeight);
		// read these from the dialog's members
		m_activeBlock->setFormat((AP_Dialog_Paragraph::tAlignState) dlg->_getMenuItemValue(AP_Dialog_Paragraph::id_MENU_ALIGNMENT),
								 dlg->_getSpinItemValue(AP_Dialog_Paragraph::id_SPIN_SPECIAL_INDENT),
								 (AP_Dialog_Paragraph::tIndentState) dlg->_getMenuItemValue(AP_Dialog_Paragraph::id_MENU_SPECIAL_INDENT),
								 dlg->_getSpinItemValue(AP_Dialog_Paragraph::id_SPIN_LEFT_INDENT),
								 dlg->_getSpinItemValue(AP_Dialog_Paragraph::id_SPIN_RIGHT_INDENT),								 
								 dlg->_getSpinItemValue(AP_Dialog_Paragraph::id_SPIN_BEFORE_SPACING),
								 dlg->_getSpinItemValue(AP_Dialog_Paragraph::id_SPIN_AFTER_SPACING),
								 dlg->_getSpinItemValue(AP_Dialog_Paragraph::id_SPIN_SPECIAL_SPACING),
								 (AP_Dialog_Paragraph::tSpacingState) dlg->_getMenuItemValue(AP_Dialog_Paragraph::id_MENU_SPECIAL_SPACING));
	}
	
	{
		// another dummy block
		m_followingBlock = new AP_Preview_Paragraph_Block(*m_clrGray,
														  m_gc,
														  AP_Dialog_Paragraph::align_LEFT,
														  m_fontHeight);
		m_followingBlock->setFormat(AP_Dialog_Paragraph::align_LEFT,
									NULL,
									AP_Dialog_Paragraph::indent_NONE,
									NULL,NULL,NULL,NULL,NULL,
									AP_Dialog_Paragraph::spacing_SINGLE);
	}
	
	// TODO : LOCALIZE THESE!

	UT_UCSChar * tmp = NULL;

	UT_UCS_cloneString_char(&tmp, STRING_PREVIOUS_BLOCK);
	m_previousBlock->setText(tmp);
	FREEP(tmp);
	
	m_activeBlock->setText(text);

	UT_UCS_cloneString_char(&tmp, STRING_FOLLOWING_BLOCK);	
	m_followingBlock->setText(tmp);
	FREEP(tmp);
}

AP_Preview_Paragraph::~AP_Preview_Paragraph()
{
	DELETEP(m_font);

	DELETEP(m_clrWhite);
	DELETEP(m_clrBlack);
	DELETEP(m_clrGray);

	DELETEP(m_previousBlock);
	DELETEP(m_activeBlock);
	DELETEP(m_followingBlock);	
}

void AP_Preview_Paragraph::draw(void)
{
	UT_ASSERT(m_gc);

	// paint white background (Word 97's background is always white, the
	// text is always black, and the font is always the same size, regardless
	// of the current document's real block formatting).
	_drawPageBackground();

	// draw the three paragraphs
	_appendBlock(m_previousBlock);
	_appendBlock(m_activeBlock);
	_appendBlock(m_followingBlock);

	m_y = DEFAULT_TOP_MARGIN;
}

UT_Bool AP_Preview_Paragraph::_loadDrawFont(void)
{
	// we draw at 5 points in this preview
	GR_Font * font = m_gc->findFont("Times New Roman", "normal", "", "normal", "", "6pt");
	
	if (font)
	{
		REPLACEP(m_font, font);
		m_gc->setFont(m_font);
		m_fontHeight = m_gc->getFontHeight();
		return UT_TRUE;
	}
	else
		return UT_FALSE;
}

void AP_Preview_Paragraph::_drawPageBackground(void)
{
	// clear area
	m_gc->fillRect(*m_clrWhite, 0, 0, getWindowWidth(), getWindowHeight());

	// draw a black one pixel border
	m_gc->setColor(*m_clrBlack);
	m_gc->drawLine(0, 0, getWindowWidth(), 0);
	m_gc->drawLine(getWindowWidth() - 1, 0, getWindowWidth() - 1, getWindowHeight());
	m_gc->drawLine(getWindowWidth() - 1, getWindowHeight() - 1, 0, getWindowHeight() - 1);
	m_gc->drawLine(0, getWindowHeight() - 1, 0, 0);
}

// width of space character in pixels
#define SPACE_CHAR_WIDTH 3

void AP_Preview_Paragraph::_appendBlock(AP_Preview_Paragraph_Block * block)
{
	UT_ASSERT(block);

	UT_uint32 ypre = 0;
	UT_uint32 ypost = 0;
	
	UT_uint32 wordCounter = 0;
	UT_uint32 wordCount = block->m_words.getItemCount();
		
	m_gc->setColor(block->m_clr);

	{
		// is there any "at least" or "exactly" spacing for this block?
		// we have to land the baseline in the right spot then
		switch (block->m_spacing)
		{
		case AP_Dialog_Paragraph::spacing_SINGLE:
		case AP_Dialog_Paragraph::spacing_ONEANDHALF:
		case AP_Dialog_Paragraph::spacing_DOUBLE:
		case AP_Dialog_Paragraph::spacing_MULTIPLE:
			ypost = block->m_lineSpacing;
			break;
		case AP_Dialog_Paragraph::spacing_ATLEAST:
		case AP_Dialog_Paragraph::spacing_EXACTLY:
			ypre = block->m_lineSpacing;
			break;
		default:
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		}
	}
	
	// start y at m_y;
	UT_uint32 y = m_y;

	// handle any spacing before first block
	y += ypre;
	// draw first line
	wordCounter += _appendLine(&block->m_words,
							   &block->m_widths,
							   0,
							   block->m_firstLineLeftStop,
							   block->m_rightStop,
							   block->m_align,
							   y);
	y += block->m_fontHeight;
	// handle any spacing after first block
	y += ypost;
	
	// handle all other lines until out of words
	while (wordCounter < wordCount)
	{
		// handle any spacing before first block
		y += ypre;
		wordCounter += _appendLine(&block->m_words,
								   &block->m_widths,
								   wordCounter,
								   block->m_leftStop,
								   block->m_rightStop,
								   block->m_align,
								   y);
		y += block->m_fontHeight;
		// handle any spacing after first block
		y += ypost;
	}

	// record the changes
	m_y = y;
}

// returns number of words it plotted
UT_uint32 AP_Preview_Paragraph::_appendLine(UT_Vector * words,
											UT_Vector * widths,
											UT_uint32 startWithWord,
											UT_uint32 left,
											UT_uint32 right,
											AP_Dialog_Paragraph::tAlignState align,
											UT_uint32 y)
{

	UT_ASSERT(words && widths);

	UT_uint32 i = 0;
	UT_uint32 totalWords = words->getItemCount();
	
	UT_uint32 pixelsForThisLine = 0;
	
	// max length of first line is the diff between the (special)
	// left stop and the (normal) right stop
	UT_uint32 maxPixelsForFirstLine = getWindowWidth() - left - right;

	// negative or zero makes no sense
	UT_ASSERT(maxPixelsForFirstLine > 0);

	i = startWithWord;

	// while we're not out of words AND have space, try to pack more onto this line
	// NOTE : we don't evaluate space widths in the while() condition so we don't
	// NOTE : wrap on one (which would be silly)
	while ((i < totalWords) &&
		   (pixelsForThisLine + (UT_uint32) widths->getNthItem(i) <= maxPixelsForFirstLine))
	{
		pixelsForThisLine += (UT_uint32) widths->getNthItem(i) + SPACE_CHAR_WIDTH;
		i++;
	}

	// we have "i" words to plot on this line, and they will take pixelsForThisLine space

	// TODO : reworkd following code to remove this variable for more speed
	UT_uint32 willDrawAt = left;
		
	// obey alignment requests
	switch(align)
	{
	case AP_Dialog_Paragraph::align_RIGHT:
		// for right, we just draw at the difference in spaces added onto the first line stop.
		willDrawAt = left + (maxPixelsForFirstLine - pixelsForThisLine);
		break;
	case AP_Dialog_Paragraph::align_CENTERED:
		// for centered, we split the difference 
		willDrawAt = left + (maxPixelsForFirstLine - pixelsForThisLine) / 2;
		break;
	case AP_Dialog_Paragraph::align_JUSTIFIED:
		// TODO : IMPLEMENT THIS ONE!  Will probably require a reworking of the next
		// TODO : for() loop (that plots words with fixed space of SPACE_CHAR_WIDTH)
	default:
		// aligh_LEFT is caught here
		break;
	}

	UT_uint32 k;
	for (k = startWithWord; k < i; k++)
	{
		m_gc->drawChars((UT_UCSChar *) words->getNthItem(k), 0,
						UT_UCS_strlen((UT_UCSChar *) words->getNthItem(k)), willDrawAt, y);
		willDrawAt += (UT_uint32) widths->getNthItem(k) + SPACE_CHAR_WIDTH;
	}

	// return number of words drawn
	return k - startWithWord;
}

	
#if 0	
	// starting at m_x and m_y, we flow text until we hit the margins,
	// then wrap and start over again at m_y + lineheight
	while (wordCounter < wordCount)
	{
		UT_UCSChar * word = (UT_UCSChar *) words->getNthItem(wordCounter);
		UT_ASSERT(word);

		// TODO : maybe move this strlen to the block's setText() member to
		// TODO : speed things up
		wordLen = UT_UCS_strlen(word);

		wordWidth = 

		// if it won't fit, wrap the Y coordinate before plotting
		if ((wordWidth + m_x) >= (getWindowWidth() - m_defaultRightMargin))
		{
			m_y += m_fontHeight + 1;
			m_x = m_defaultLeftMargin;
		}

		// plot the text
		m_gc->setColor((UT_RGBColor &) block->getColor());
		m_gc->drawChars(word, 0, wordLen, m_x, m_y);
		m_x += wordWidth;

		// print a space as 3 pixels
		m_x += 3;
		
		// next word
		wordCounter++;
	}

	// always drop down one more line to start next paragraph
	m_y += m_fontHeight + 1;

	// start next at margin
	m_x = m_defaultLeftMargin;
#endif
