/* AbiWord
 * Copyright (C) 2000 AbiSource, Inc.
 * Copyright (C) 2000, 2001 Frodo Looijaard <frodol@dds.nl>
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

/* This exporter was written by Frodo Looijaard <frodol@dds.nl> */

//  Export Word or TextEd data from a Psion file. 
//  We use libpsiconv for the real work

// To do:
//		Check and fix characters with high ASCII codes
//		Check and fix characters with low ASCII codes.
//		Support styles
//		Check what we need to base our formats on ie. what is the
//		  base paragraph or character layout - probably determined
//		  by style?!?
//		Add support for bullets
//		Better determination of font type (serif, sansserif or nonproportional)
//		Add support for other sections (page-level layout, headers, footers)
//		Add support for objects, fields, format marks


// Search for TODO for more things to do.

#include "ut_string.h"
#include "ut_bytebuf.h"
#include "ut_units.h"
#include "ut_debugmsg.h"
#include "ut_wctomb.h"
#include "ut_assert.h"

#include "pt_Types.h"
#include "pd_Document.h"
#include "pp_AttrProp.h"
#include "px_ChangeRecord.h"
#include "px_CR_Object.h"
#include "px_CR_Span.h"
#include "px_CR_Strux.h"

#include "ie_exp_Psion.h"
#include <psiconv/generate.h>


// We declare listener classes to traverse the internal file easily.
// While we are building paragraphs (and perhaps other things), we
// store the intermediate stuff in the listener. Once we have a
// complete paragraph, we add the data to the referred IE_Exp_Psion class.

class s_Psion_Listener : public PL_Listener
{
public:
	// Constructors and destructor
	s_Psion_Listener(PD_Document * pDocument,
	                 IE_Exp_Psion * pie);
	virtual ~s_Psion_Listener(void);

	// Overriding methods from the base class
	virtual UT_Bool populate(PL_StruxFmtHandle sfh,
	                         const PX_ChangeRecord * pcr);

	virtual UT_Bool populateStrux(PL_StruxDocHandle sdh,
	                              const PX_ChangeRecord * pcr,
	                              PL_StruxFmtHandle * psfh);

	virtual UT_Bool change(PL_StruxFmtHandle sfh,
	                       const PX_ChangeRecord * pcr);

	virtual UT_Bool insertStrux(PL_StruxFmtHandle sfh,
	                            const PX_ChangeRecord * pcr,
	                            PL_StruxDocHandle sdh,
	                            PL_ListenerId lid,
	                            void (* pfnBindHandles)
	                                            (PL_StruxDocHandle sdhNew,
	                                             PL_ListenerId lid,
	                                             PL_StruxFmtHandle sfhNew));

	virtual UT_Bool signal(UT_uint32 iSignal);

	// New public methods
	virtual UT_Bool finishDocument(void);

protected:
	// New data
	PD_Document *m_pDocument;
	IE_Exp_Psion *m_pie;

	UT_ByteBuf m_currentParagraphText;
	psiconv_paragraph_layout m_currentParagraphPLayout;
	psiconv_character_layout m_currentParagraphCLayout;
	UT_Bool m_inParagraph;
	psiconv_in_line_layouts m_currentParagraphInLines;

	// New methods
	UT_Bool _writeText(const UT_UCSChar *p, UT_uint32 inlength,
	                   UT_uint32 *outlength);
	UT_Bool _openParagraph(PT_AttrPropIndex api);
	UT_Bool _closeParagraph(void);
	UT_Bool _addInLine(PT_AttrPropIndex api,UT_uint32 textlen);
	static void _parseColor(const char *input,psiconv_color color);
	static int _hexDigitToDec(char in);
	static void _parseTab(char *input,psiconv_tab tab);
	static UT_Bool _parseTabs(const char *input,psiconv_tab_list tabs);
};


/*****************************************************************/
/*****************************************************************/

IE_Exp_Psion::IE_Exp_Psion(PD_Document *pDocument)
	: IE_Exp(pDocument)
{
	// We can't allocate m_paragraphs here, because we can't have a
	// constructor fail, which might happen on memory shortage.
	m_paragraphs = NULL;
	m_error = 0;
	m_pListener = NULL;
}

IE_Exp_Psion::~IE_Exp_Psion(void)
{
	if (m_paragraphs)
		psiconv_list_free(m_paragraphs);
}

// This function writes a document to file. To do this, we must first
// construct a psiconv_file (a sort of abstract syntax tree of the document)
// then translate it to a psiconv_buffer (the file in Psion format), and
// finally write the contents of this buffer to a file.
// As most of the work is the same for any type of Psion file, we define
// this method here in the base exporter class. The only thing that is
// really different between TextEd and Word files is the psiconv_file;
// so we construct it through the virtual method _createPsionFile.
UT_Error IE_Exp_Psion::_writeDocument(void)
{
	const int MAXBUFLEN = 512;
	char buf[MAXBUFLEN];
	char *pByte;
	unsigned int i;
	UT_Bool bRes;
	int iRes;
	psiconv_file psionfile;
	psiconv_buffer psiondump;

	// We allocate m_paragraphs here and not in the constructor, because
	// we can't fail in a constructor. Note that we assume it is not
	// yet allocated.
	if (!(m_paragraphs = psiconv_list_new(sizeof(struct psiconv_paragraph_s))))
		return UT_IE_NOMEMORY;

	// Create a s_Psion_Listener, and signal it to traverse the document.
	// It will translate the internal AbiWord representation to 
	// psiconv_file fragments in the current exporter object.
	if (!(m_pListener = new s_Psion_Listener(m_pDocument,this)))
		return UT_IE_NOMEMORY;
	bRes = m_pDocument->tellListener(m_pListener);
	if (bRes && !m_error)
		// We must call this by hand, to close any open paragraphs.
		m_pListener->finishDocument();
	delete m_pListener;
	if (!bRes || m_error)
		return UT_IE_COULDNOTWRITE;

	// Create the complete psiconv_file representation
	if (!(psionfile = _createPsionFile()))
		return UT_IE_COULDNOTWRITE;
		
	// Translate the psiconv_file representation to a dump that can be
	// written to file.
	iRes = psiconv_write(&psiondump,psionfile);
	psiconv_free_file(psionfile);
	if (iRes)
		return UT_IE_COULDNOTWRITE;

	// Write the content of psiondump to file. We do this in 
	// MAXBUFFERLEN chunks, because it would be horribly slow if we did it
	// byte by byte. Note that we can't just access the psiconv_buffer,
	// we have to read it byte by byte.
	for (i = 0; i < psiconv_buffer_length(psiondump); i++) {
		if (!(pByte = (char *) psiconv_buffer_get(psiondump,i))) {
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			psiconv_buffer_free(psiondump);
			return UT_IE_COULDNOTWRITE;
		}
		buf[i % MAXBUFLEN] = *pByte;
		if ((i % MAXBUFLEN == MAXBUFLEN-1) || 
		    (i == psiconv_buffer_length(psiondump) - 1)) 
		// Seemingly this function does not return an error code?!?
		write(buf,(i % MAXBUFLEN)+1);
	}

	psiconv_buffer_free(psiondump);
	return UT_OK;
}


/*****************************************************************/
/*****************************************************************/

IE_Exp_Psion_TextEd::IE_Exp_Psion_TextEd(PD_Document * pDocument)
	: IE_Exp_Psion(pDocument)
{
}

IE_Exp_Psion_TextEd::~IE_Exp_Psion_TextEd(void)
{
}

UT_Bool IE_Exp_Psion_TextEd::RecognizeSuffix(const char * szSuffix)
{
	return (UT_stricmp(szSuffix,".psitext") == 0);
}

UT_Error IE_Exp_Psion_TextEd::StaticConstructor(PD_Document * pDocument,
                                                IE_Exp ** ppie)
{
	IE_Exp_Psion_TextEd * p = new IE_Exp_Psion_TextEd(pDocument);
	*ppie = p;
	return UT_OK;
}

UT_Bool IE_Exp_Psion_TextEd::GetDlgLabels(const char ** pszDesc,
                                   const char ** pszSuffixList,
                                   IEFileType * ft)
{
	*pszDesc = "Psion TextEd (.psitext)";
	*pszSuffixList = "*.psitext";
	*ft = IEFT_Psion_TextEd;
	return UT_TRUE;
}


UT_Bool IE_Exp_Psion_TextEd::SupportsFileType(IEFileType ft)
{
	return (IEFT_Psion_TextEd == ft);
}

// Translate the psiconv_file fragments in the exporter object to a
// complete psiconv_file of type psiconv_texted_file.
psiconv_file IE_Exp_Psion_TextEd::_createPsionFile(void)
{
	psiconv_file psionfile;
	psiconv_texted_f textedfile;

	// We base our file on an empty file.
	if (!(psionfile = psiconv_empty_file(psiconv_texted_file)))  
		return NULL;
	// psionfile->file is a void pointer...
	textedfile = (psiconv_texted_f)(psionfile->file);
	// Free the old paragraphs structure and put ours into it. We also
	// set the old pointer to it to NULL, because we don't want to risk
	// double deallocation!
	psiconv_free_text_and_layout(textedfile->texted_sec->paragraphs);
	textedfile->texted_sec->paragraphs = m_paragraphs;
	m_paragraphs = NULL;
	return psionfile;
}


/*****************************************************************/
/*****************************************************************/

IE_Exp_Psion_Word::IE_Exp_Psion_Word(PD_Document * pDocument)
	: IE_Exp_Psion(pDocument)
{
}

IE_Exp_Psion_Word::~IE_Exp_Psion_Word(void)
{
}

UT_Bool IE_Exp_Psion_Word::RecognizeSuffix(const char * szSuffix)
{
	return (UT_stricmp(szSuffix,".psiword") == 0);
}

UT_Error IE_Exp_Psion_Word::StaticConstructor(PD_Document * pDocument,
                                                IE_Exp ** ppie)
{
	IE_Exp_Psion_Word * p = new IE_Exp_Psion_Word(pDocument);
	*ppie = p;
	return UT_OK;
}

UT_Bool IE_Exp_Psion_Word::GetDlgLabels(const char ** pszDesc,
                                   const char ** pszSuffixList,
                                   IEFileType * ft)
{
	*pszDesc = "Psion Word (.psiword)";
	*pszSuffixList = "*.psiword";
	*ft = IEFT_Psion_Word;
	return UT_TRUE;
}


UT_Bool IE_Exp_Psion_Word::SupportsFileType(IEFileType ft)
{
	return (IEFT_Psion_Word == ft);
}

// Translate the psiconv_file fragments in the exporter object to a
// complete psiconv_file of type psiconv_word_file.
psiconv_file IE_Exp_Psion_Word::_createPsionFile(void)
{
	psiconv_file psionfile;
	psiconv_word_f wordfile;

	// We base our file on an empty file.
	if (!(psionfile = psiconv_empty_file(psiconv_word_file))) 
		return NULL;
	// psionfile->file is a void pointer...
	wordfile = (psiconv_word_f)(psionfile->file);
	// Free the old paragraphs structure and put ours into it. We also
	// set the old pointer to it to NULL, because we don't want to risk
	// double deallocation!
	psiconv_free_text_and_layout(wordfile->paragraphs);
	wordfile->paragraphs = m_paragraphs;
	m_paragraphs = NULL;
	return psionfile;
}


/*****************************************************************/
/*****************************************************************/

// The listener uses several psiconv_file fragments while building
// paragraphs (and perhaps other things). We can't allocate them
// here, because we can't fail in a constructor.
s_Psion_Listener::s_Psion_Listener(PD_Document * pDocument,
                                   IE_Exp_Psion * pie)
{
	m_pDocument = pDocument;
	m_pie = pie;
	m_inParagraph = 0;
	m_currentParagraphPLayout = NULL;
	m_currentParagraphCLayout = NULL;
	m_currentParagraphInLines = NULL;
}

// Deallocate those psiconv_file fragments that are currently allocated.
s_Psion_Listener::~s_Psion_Listener(void)
{
	if (m_currentParagraphPLayout)
		psiconv_free_paragraph_layout(m_currentParagraphPLayout);
	if (m_currentParagraphCLayout)
		psiconv_free_character_layout(m_currentParagraphCLayout);
	if (m_currentParagraphInLines)
		psiconv_list_free(m_currentParagraphInLines);
}

// Standard listener callback for in-block data
UT_Bool s_Psion_Listener::populate(PL_StruxFmtHandle /*sfh*/,
                                   const PX_ChangeRecord * pcr)
{
	const PX_ChangeRecord_Span * pcrs;
	PT_BufIndex bi;
	PT_AttrPropIndex api;
	UT_uint32 textlen;
	
	switch(pcr->getType()) {
		case PX_ChangeRecord::PXT_InsertSpan:
			pcrs = static_cast<const PX_ChangeRecord_Span *> (pcr);
			bi = pcrs->getBufIndex();
			if (! _writeText(m_pDocument->getPointer(bi),pcrs->getLength(),
			                 &textlen))
				return UT_FALSE;
			api = pcr->getIndexAP();
			return _addInLine(api,textlen);
		case PX_ChangeRecord::PXT_InsertObject:
			UT_DEBUGMSG(("Insert Object (ignored)\n"));
			return UT_TRUE;
		case PX_ChangeRecord::PXT_InsertFmtMark:
			UT_DEBUGMSG(("Insert Format Mark (ignored)\n"));
			return UT_TRUE;
		default:
			UT_ASSERT(0);
			return UT_FALSE;
	}

}

// Standard listener callback for block and section data
UT_Bool s_Psion_Listener::populateStrux(PL_StruxDocHandle /*sdh*/,
                                        const PX_ChangeRecord * pcr,
                                        PL_StruxFmtHandle * psfh)
{
	UT_ASSERT(pcr->getType() == PX_ChangeRecord::PXT_InsertStrux);
	const PX_ChangeRecord_Strux * pcrx = 
	                        static_cast<const PX_ChangeRecord_Strux *> (pcr);

	switch (pcrx->getStruxType()) {
		case PTX_Block:
			if (!_closeParagraph())
				return UT_FALSE;
			if (!_openParagraph(pcr->getIndexAP()))
				return UT_FALSE;
			return UT_TRUE;
			break;
		case PTX_Section:
			UT_DEBUGMSG(("New section (ignored)\n"));
			if (!_closeParagraph())
				return UT_FALSE;
			return UT_TRUE;
			break;
		default:
			UT_ASSERT(0);
			 return UT_FALSE;
	}
}

// Standard listener callback that should never be called
UT_Bool s_Psion_Listener::change(PL_StruxFmtHandle /*sfh*/,
                                 const PX_ChangeRecord * /*pcr*/)
{
	UT_ASSERT(0);  // this function is not used.
	return UT_FALSE;
}

// Standard listener callback that should never be called
UT_Bool s_Psion_Listener::insertStrux(PL_StruxFmtHandle /*sfh*/,
                const PX_ChangeRecord * /*pcr*/,
                PL_StruxDocHandle /*sdh*/,
                PL_ListenerId /* lid */,
                void (* /*pfnBindHandles*/)(PL_StruxDocHandle /* sdhNew */,
                                            PL_ListenerId /* lid */,
                                            PL_StruxFmtHandle /* sfhNew */))
{
	UT_ASSERT(0);    // this function is not used.
	return UT_FALSE;
}

// Standard listener callback that should never be called
UT_Bool s_Psion_Listener::signal(UT_uint32 /* iSignal */)
{
	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	return UT_FALSE;
}

// Add some text to the current paragraph. Returns the length of the 
// written text in outLength; this may be more or less than inLength,
// because some input characters may need to be represented by more than one
// output character, and because we may ignore some input characters.
UT_Bool s_Psion_Listener::_writeText(const UT_UCSChar *p, UT_uint32 inLength,
                                     UT_uint32 *outLength)
{
	class UT_Wctomb wctomb("CP1252");

	UT_uint32 i;
	char pC[MB_LEN_MAX];
	int mbLen;

	if (outLength)
		*outLength = 0;
	// I am not sure I handle the multi-byte to wide character in charset
	// CP1252 right. Actually, I am pretty sure it is worthless :-( 
	for (i = 0; i < inLength; i++)  {
		if (!wctomb.wctomb(pC,mbLen,(wchar_t) p[i])) {
			pC[0]='?'; // Do we really want this?
			wctomb.initialize();
		}
		// TODO: Check for special ASCII codes (below 32 decimal)
		if ((mbLen ==1) && (pC[0] < 0x20)) {
			// TODO
		}
		if (!m_currentParagraphText.append( (UT_Byte *) pC,mbLen))
			return UT_FALSE;
		if (outLength)
			*outLength += mbLen;
	}
	return UT_TRUE;
}

// Open a new paragraph, and set paragraph-level layout.
UT_Bool s_Psion_Listener::_openParagraph(PT_AttrPropIndex api)
{
	const PP_AttrProp * pAP = NULL;
	const XML_Char* szValue;
	char *tempstr;
	UT_Bool widowsorphans;

	// New paragraph, new text.
	m_currentParagraphText.truncate(0);
	// Set the base layout of this paragraph.
	// TODO: This should be based on the current style!
	if (!(m_currentParagraphPLayout = psiconv_basic_paragraph_layout()))
		return UT_FALSE;
	if (!(m_currentParagraphCLayout = psiconv_basic_character_layout())) {
		psiconv_free_paragraph_layout(m_currentParagraphPLayout);
		return UT_FALSE;
	}
	// New list of in-line layout
	if (!(m_currentParagraphInLines = 
	              psiconv_list_new(sizeof(struct psiconv_in_line_layout_s)))) {
		psiconv_free_character_layout(m_currentParagraphCLayout);
		psiconv_free_paragraph_layout(m_currentParagraphPLayout);
		return UT_FALSE;
	}
	// We need to close the paragraph too.
	m_inParagraph = UT_TRUE;
	
	// Check for all possible paragraph-level layout.
	if (m_pDocument->getAttrProp(api,&pAP) && pAP) {
		if (pAP->getProperty((XML_Char*)"margin-left",szValue))
			m_currentParagraphPLayout->indent_left = 
			              UT_convertToDimension((const char *) szValue,DIM_CM);
		if (pAP->getProperty((XML_Char*)"margin-right",szValue)) 
			m_currentParagraphPLayout->indent_right = 
			              UT_convertToDimension((const char *) szValue,DIM_CM);
		if (pAP->getProperty((XML_Char*)"text-indent",szValue)) 
			m_currentParagraphPLayout->indent_first = 
			              UT_convertToDimension((const char *) szValue,DIM_CM);
		if (pAP->getProperty((XML_Char*)"text-align",szValue)) {
			if (!UT_strcmp((const char *) szValue,"center"))
				m_currentParagraphPLayout->justify_hor = psiconv_justify_centre;
			else if (!UT_strcmp((const char *) szValue,"right"))
				m_currentParagraphPLayout->justify_hor = psiconv_justify_right;
			else if (!UT_strcmp((const char *) szValue,"justify"))
				m_currentParagraphPLayout->justify_hor = psiconv_justify_full;
			else
				m_currentParagraphPLayout->justify_hor = psiconv_justify_left;
		}
		if (pAP->getProperty((XML_Char*)"line-height",szValue)) {
			m_currentParagraphPLayout->linespacing_exact =
			       szValue[strlen((char *)szValue)-1]=='+'?psiconv_bool_false:
			                                               psiconv_bool_true;
			tempstr = UT_strdup((char *)szValue);
			if (!m_currentParagraphPLayout->linespacing_exact)
				tempstr[strlen((char *)szValue)-1] = '\000';
			m_currentParagraphPLayout->linespacing = 
			              UT_convertToDimension(tempstr,DIM_PT);
			free(tempstr);
		}
		if (pAP->getProperty((XML_Char*)"margin-top",szValue)) 
			m_currentParagraphPLayout->space_above = 
			              UT_convertToDimension((const char *) szValue,DIM_PT);
		if (pAP->getProperty((XML_Char*)"margin-bottom",szValue)) 
			m_currentParagraphPLayout->space_below = 
			              UT_convertToDimension((const char *) szValue,DIM_PT);
		if (pAP->getProperty((XML_Char*)"keep-together",szValue)) { 
			if (!UT_strcmp((const char *) szValue,"yes"))
				m_currentParagraphPLayout->keep_together = psiconv_bool_true;
			else
				m_currentParagraphPLayout->keep_together = psiconv_bool_false;
		}
		if (pAP->getProperty((XML_Char*)"keep-with-next",szValue)) {
			if (!UT_strcmp((const char *) szValue,"yes"))
				m_currentParagraphPLayout->keep_with_next = psiconv_bool_true;
			else
				m_currentParagraphPLayout->keep_with_next = psiconv_bool_false;
		}
		// Set widowsorphans if either widows or orphans is set and 
		// unequal to "0".
		widowsorphans = UT_FALSE;
		if (pAP->getProperty((XML_Char*)"widows",szValue)) 
			widowsorphans |= UT_strcmp((const char *) szValue,"0");
		if (pAP->getProperty((XML_Char*)"orphans",szValue)) 
			widowsorphans |= UT_strcmp((const char *) szValue,"0");
		m_currentParagraphPLayout->no_widow_protection = 
		                   widowsorphans?psiconv_bool_false:psiconv_bool_true;
		if (pAP->getProperty((XML_Char*)"default-tab-interval",szValue))  
			m_currentParagraphPLayout->tabs->normal = 
			              UT_convertToDimension((const char *) szValue,DIM_CM);
		if (pAP->getProperty((XML_Char*)"tabstops",szValue))  
			if (!_parseTabs((char *)szValue,
			                m_currentParagraphPLayout->tabs->extras)) {
				psiconv_free_character_layout(m_currentParagraphCLayout);
				psiconv_free_paragraph_layout(m_currentParagraphPLayout);
				psiconv_list_free(m_currentParagraphInLines);
				return UT_FALSE;
			}
		// TODO: Bullets
	}

	return UT_TRUE;
}

// Close a paragraph, and write the result to the exporter object.
UT_Bool s_Psion_Listener::_closeParagraph(void)
{
	struct psiconv_paragraph_s para;
	// It is safe to call us when we are not actually in a paragraph;
	// we will just nop.
	if (m_inParagraph) {
		// Append the final \000
		if (!m_currentParagraphText.append((const UT_Byte *) "",1))
			goto ERROR1;
		// We need to get a copy of the paragraph text, because 
		// m_currentParagraphText will be refilled.
		if (!(para.text =
	                UT_strdup((char *) m_currentParagraphText.getPointer(0))))
			goto ERROR1;
		// Get the base paragraph and character layout.
		para.base_character = m_currentParagraphCLayout;
		m_currentParagraphCLayout = NULL;
		para.base_paragraph = m_currentParagraphPLayout;
		m_currentParagraphPLayout = NULL;
		// TODO: Set the real style here.
		para.base_style = 0;
		// TODO: Set the in-block character layout here.
		para.in_lines = m_currentParagraphInLines;
		m_currentParagraphInLines = 0;
		// We don't do replacements, because I do not really understand
		// the Psion-side of it yet.
		if (!(para.replacements = 
		            psiconv_list_new(sizeof(struct psiconv_replacement_s))))
			goto ERROR2;
		// Add to the main paragraph list in the exporter object..
		if (psiconv_list_add(m_pie->m_paragraphs,&para))
			goto ERROR3;
		
		m_inParagraph = UT_FALSE;
	}
	return UT_TRUE;

// Cleanup if something goes wrong halfway-through.
ERROR3:
	psiconv_list_free(para.replacements);
ERROR2:
	psiconv_list_free(para.in_lines);
	psiconv_free_paragraph_layout(para.base_paragraph);
	psiconv_free_character_layout(para.base_character);
	free(para.text);
ERROR1:
	return UT_FALSE;
}

// Add current in-line layout
UT_Bool s_Psion_Listener::_addInLine(PT_AttrPropIndex api,UT_uint32 textlen)
{
	const PP_AttrProp * pAP = NULL;
	const XML_Char* szValue;
	struct psiconv_in_line_layout_s curInLine;
	char *tempstr;

	if (!m_inParagraph)
		return UT_FALSE;

	curInLine.length = textlen;
	if (!(curInLine.layout =
	                psiconv_clone_character_layout(m_currentParagraphCLayout)))
		return UT_FALSE;

	if (m_pDocument->getAttrProp(api,&pAP) && pAP) {
		if (pAP->getProperty((XML_Char*)"font-family",szValue)) {
			tempstr = curInLine.layout->font->name;
			if (!(curInLine.layout->font->name = 
			                               UT_strdup((const char*) szValue))) {
				curInLine.layout->font->name = tempstr;
				goto ERROR;
			}
			free(tempstr);
			// There may be a good general way to do this, but I do
			// not know how.
			if (strstr(curInLine.layout->font->name,"Courier") ||
			    strstr(curInLine.layout->font->name,"Mono"))
				curInLine.layout->font->screenfont = psiconv_font_nonprop;
			else if (strstr(curInLine.layout->font->name,"Arial") ||
			         strstr(curInLine.layout->font->name,"Goth") ||
			         strstr(curInLine.layout->font->name,"Helvetic") ||
			         strstr(curInLine.layout->font->name,"Univers") ||
			         strstr(curInLine.layout->font->name,"Sans"))
				curInLine.layout->font->screenfont = psiconv_font_sansserif;
			else
				curInLine.layout->font->screenfont = psiconv_font_serif;
		}
		if (pAP->getProperty((XML_Char*)"font-size",szValue))
			curInLine.layout->font_size = 
			              UT_convertToDimension((const char *) szValue,DIM_PT);
		if (pAP->getProperty((XML_Char*)"font-weight",szValue)) {
			if (!UT_strcmp((const char *) szValue,"bold"))
				curInLine.layout->bold = psiconv_bool_true;
			else
				curInLine.layout->bold = psiconv_bool_false;
		}
		if (pAP->getProperty((XML_Char*)"font-style",szValue)) {
			if (!UT_strcmp((const char *) szValue,"italic"))
				curInLine.layout->italic = psiconv_bool_true;
			else
				curInLine.layout->italic = psiconv_bool_false;
		}
		if (pAP->getProperty((XML_Char*)"text-decoration",szValue)) {
			if (strstr((const char *) szValue,"underline")) 
				curInLine.layout->underline = psiconv_bool_true;
			else
				curInLine.layout->underline = psiconv_bool_false;
			if (strstr((const char *) szValue,"line-through")) 
				curInLine.layout->strikethrough = psiconv_bool_true;
			else
				curInLine.layout->strikethrough = psiconv_bool_false;
		}
		if (pAP->getProperty((XML_Char*)"text-position",szValue)) {
			if (!UT_strcmp((const char *) szValue,"superscript"))
				curInLine.layout->super_sub = psiconv_superscript;
			else if (!UT_strcmp((const char *) szValue,"subscript"))
				curInLine.layout->super_sub = psiconv_subscript;
			else
				curInLine.layout->super_sub = psiconv_normalscript;
		}
		if (pAP->getProperty((XML_Char*)"color",szValue)) 
			_parseColor((char *) szValue,curInLine.layout->color);
		if (pAP->getProperty((XML_Char*)"bgcolor",szValue)) 
			_parseColor((char *) szValue,curInLine.layout->back_color);
	}
	if (psiconv_list_add(m_currentParagraphInLines,&curInLine))
		goto ERROR;
	return UT_TRUE;
ERROR:
	psiconv_free_character_layout(curInLine.layout);
	return UT_FALSE;
}

// Close remaining paragraphs and sections. We could do this in the
// destructor, except that it may not be safe to call these routines
// if something went wrong half-way through.
UT_Bool s_Psion_Listener::finishDocument(void)
{
	return _closeParagraph();
}

int s_Psion_Listener::_hexDigitToDec(char in)
{
	switch(in) {
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
		default: return 0;
	}
}

void s_Psion_Listener::_parseColor(const char *input,psiconv_color color)
{
	color->red = _hexDigitToDec(input[0])*16+_hexDigitToDec(input[1]);
	color->green = _hexDigitToDec(input[2])*16+_hexDigitToDec(input[3]);
	color->blue = _hexDigitToDec(input[4])*16+_hexDigitToDec(input[5]);
}

// Clobbers *input!
void s_Psion_Listener::_parseTab(char *input,psiconv_tab tab)
{
	char *slash;
	slash = index(input,'/');
	tab->kind = psiconv_tab_left;
	if (slash) {
		if (*(slash+1) == 'R')
			tab->kind = psiconv_tab_right;
		else if (*(slash+1) == 'C')
			tab->kind = psiconv_tab_centre;
		*slash = '\000';
	}
	tab->location = UT_convertToDimension(input,DIM_CM);
}


// Parse tabs into input.
UT_Bool s_Psion_Listener::_parseTabs(const char *input,psiconv_tab_list tabs)
{
	const char *currentPos;
	const char *nextPos;
	char *copy;
	struct psiconv_tab_s tab;

	currentPos = input;
	while (*currentPos != '\000') {
		nextPos = index(currentPos,',');
		if (!nextPos)
			nextPos = index(currentPos,'\000');
		if (!(copy = (char *) malloc(nextPos - currentPos + 1)))
			return UT_FALSE;
		memcpy(copy,currentPos,nextPos-currentPos);
		copy[nextPos - currentPos] = '\000';
		_parseTab(copy,&tab);
		free(copy);
		if (psiconv_list_add(tabs,&tab))
			return UT_FALSE;
		currentPos = nextPos;
		while ((*currentPos == ',' || *currentPos == ' '))
			currentPos++;
	}
	return UT_TRUE;
}
