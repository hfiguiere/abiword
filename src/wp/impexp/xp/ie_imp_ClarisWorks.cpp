/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 1998-2000 Hubert Figuiere
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ut_types.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "ut_string.h"
#include "ie_imp_ClarisWorks.h"
#include "pd_Document.h"
#include "ut_growbuf.h"

/*****************************************************************/
/* Generic IO                                                    */
/*****************************************************************/


/*****************************************************************/
/*****************************************************************/

/*
  Import ClarisWorks/AppleWorks file
*/

static const char PROPS_XML_ATTR_NAME[] = "PROPS";

// char attributes
static const char FONT_FAMILY[] = "font-family:%s;";

static const char FONT_STYLE[] = "font-style:%s;";
static const char FONT_STYLE_ITALIC[] = "italic";

static const char FONT_WEIGHT[] = "font-weight:%s;";
static const char FONT_WEIGHT_BOLD[] = "bold";

static const char FONT_SIZE[] = "font-size:%spt;";

enum {
    CW_HANDLED_VERSION = 5
};


/*****************************************************************/
/*****************************************************************/

#define X_CleanupIfError(ies,exp)	do { if (((ies)=(exp)) != UT_OK) goto Cleanup; } while (0)


UT_Error IE_Imp_ClarisWorks::importFile(const char * szFilename)
{
    FILE *fp = fopen(szFilename, "r");
    if (!fp)
    {
        UT_DEBUGMSG(("Could not open file %s\n",szFilename));
        return UT_IE_FILENOTFOUND;
    }
    
    UT_Error iestatus;
    
    X_CleanupIfError(iestatus,_writeHeader(fp));
    X_CleanupIfError(iestatus,_parseFile(fp));
    
    iestatus = UT_OK;

 Cleanup:
    fclose(fp);
    return iestatus;
}

#undef X_CleanupIfError

/*****************************************************************/
/*****************************************************************/

IE_Imp_ClarisWorks::~IE_Imp_ClarisWorks()
{
}

IE_Imp_ClarisWorks::IE_Imp_ClarisWorks(PD_Document * pDocument)
    : IE_Imp(pDocument)
{
}

/*****************************************************************/
/*****************************************************************/

#define X_ReturnIfFail(exp,ies)		do { UT_Bool b = (exp); if (!b) return (ies); } while (0)
#define X_ReturnNoMemIfError(exp)	X_ReturnIfFail(exp,UT_IE_NOMEMORY)

UT_Error IE_Imp_ClarisWorks::_writeHeader(FILE * /* fp */)
{
    X_ReturnNoMemIfError(m_pDocument->appendStrux(PTX_Section, NULL));
    
    return UT_OK;
}

UT_Error IE_Imp_ClarisWorks::_parseFile(FILE * fp)
{
    unsigned char buf [128];   // general purpose buffer (128 bytes, no more)
    unsigned long offset;
    UT_GrowBuf gbBlock(1024);
    UT_Bool bEatLF = UT_FALSE;
    UT_Bool bEmptyFile = UT_TRUE;
    unsigned char c;
    
    
    // read the final header to get the ETBL offset
    fseek (fp, -24, SEEK_END);
    
   fread (&buf, sizeof (char), 4, fp);
   if (strncmp ((char *)buf, "ETBL", 4) != 0) 
   {
       // ERROR !
       UT_DEBUGMSG(("ETBL marker not found!\n"));
   }
   fread (&offset, sizeof (offset), 1, fp);
   {
       if (offset >= ftell (fp)) 
       {
           // ERROR again !
           UT_DEBUGMSG(("incorrect offset for ETBL struct !\n"));
       }
   }
   
   // moving to ETBL 
   fseek (fp, offset, SEEK_SET);
   
   fread (&buf, sizeof (char), 4, fp);
   if (strncmp ((char *)buf, "ETBL", 4) != 0) 
   {
       // ERROR !
       UT_DEBUGMSG(("ETBL marker from ETBL not found!\n"));
   }
   fread (&offset, sizeof (offset), 1, fp);
   {
       if (offset >= ftell (fp)) 
       {
           // ERROR again !
           UT_DEBUGMSG(("incorrect offset for  struct !\n"));
       }
   }
   
   
   while (fread(&c, 1, sizeof(c), fp) > 0)
   {
       switch (c)
       {
           
       case 0x0D:// ClarisWorks uses 0x0D as an EOL, like on the Macintosh
           
           // start a paragraph and emit any text that we
           // have accumulated.
           X_ReturnNoMemIfError(m_pDocument->appendStrux(PTX_Block, NULL));
           bEmptyFile = UT_FALSE;
           if (gbBlock.getLength() > 0)
           {
               X_ReturnNoMemIfError(m_pDocument->appendSpan(gbBlock.getPointer(0), gbBlock.getLength()));
               gbBlock.truncate(0);
           }
           break;
           
       default:
           bEatLF = UT_FALSE;
           
           // deal with plain character.
           // this cast is OK.  we have US-ASCII (actually Latin-1) character
           // data, so we can do this.
           
           // TODO consider scanning for UTF8...
           
           UT_UCSChar uc = (UT_UCSChar) c;
           X_ReturnNoMemIfError(gbBlock.ins(gbBlock.getLength(),&uc,1));
           break;
       }
   } 
   
   if (gbBlock.getLength() > 0 || bEmptyFile)
   {
       // if we have text left over (without final CR/LF),
       // or if we read an empty file,
       // create a paragraph and emit the text now.
       X_ReturnNoMemIfError(m_pDocument->appendStrux(PTX_Block, NULL));
       if (gbBlock.getLength() > 0)
           X_ReturnNoMemIfError(m_pDocument->appendSpan(gbBlock.getPointer(0), gbBlock.getLength()));
   }
   
   
   return UT_OK;
}



#undef X_ReturnNoMemIfError
#undef X_ReturnIfFail


/*****************************************************************/
/*****************************************************************/

//
// What is this for ? I don't know...
// FIXME
//
void IE_Imp_ClarisWorks::pasteFromBuffer(PD_DocumentRange * pDocRange,
                                     unsigned char * pData, UT_uint32 lenData)
{
	UT_ASSERT(m_pDocument == pDocRange->m_pDoc);
	UT_ASSERT(pDocRange->m_pos1 == pDocRange->m_pos2);

	UT_GrowBuf gbBlock(1024);
	UT_Bool bEatLF = UT_FALSE;
	UT_Bool bSuppressLeadingParagraph = UT_TRUE;
	UT_Bool bInColumn1 = UT_TRUE;
	unsigned char * pc;

	PT_DocPosition dpos = pDocRange->m_pos1;
	
	for (pc=pData; (pc<pData+lenData); pc++)
	{
		unsigned char c = *pc;
		
		switch (c)
		{
		case '\r':
		case '\n':
			if ((c == '\n') && bEatLF)
			{
				bEatLF = UT_FALSE;
				break;
			}

			if (c == '\r')
			{
				bEatLF = UT_TRUE;
			}
			
			// we interprete either CRLF, CR, or LF as a paragraph break.
			
			if (gbBlock.getLength() > 0)
			{
				// flush out what we have
				m_pDocument->insertSpan(dpos, gbBlock.getPointer(0), gbBlock.getLength());
				dpos += gbBlock.getLength();
                                gbBlock.truncate(0);
			}
			bInColumn1 = UT_TRUE;
			break;

		default:
			bEatLF = UT_FALSE;
			if (bInColumn1 && !bSuppressLeadingParagraph)
			{
				m_pDocument->insertStrux(dpos,PTX_Block);
				dpos++;
			}
			
			// deal with plain character.
			// this cast is OK.  we have US-ASCII (actually Latin-1) character
			// data, so we can do this.
						UT_UCSChar uc = (UT_UCSChar) c;
			gbBlock.ins(gbBlock.getLength(),&uc,1);

			bInColumn1 = UT_FALSE;
			bSuppressLeadingParagraph = UT_FALSE;
			break;
		}
	} 

	if (gbBlock.getLength() > 0)
	{
		// if we have text left over (without final CR/LF),
		m_pDocument->insertSpan(dpos, gbBlock.getPointer(0), gbBlock.getLength());
		dpos += gbBlock.getLength();
	}

	return;
}

/*****************************************************************/
/*****************************************************************/

UT_Bool IE_Imp_ClarisWorks::RecognizeContents(const char * szBuf, UT_uint32 iNumbytes)
{
    if (iNumbytes >= 8) 
    {
        if ((szBuf[4] == 'B') && (szBuf[5] == 'O') && (szBuf [6] == 'B')
            && (szBuf [7] == 'O'))
        {
            if (szBuf [0] == CW_HANDLED_VERSION) 
            {
                return (UT_TRUE);
            }
            else 
            {
                UT_DEBUGMSG (("%s,%d: Mismatch version.\n",__FILE__,__LINE__));
            }
        }
    
    }

    return(UT_FALSE);
}

UT_Bool IE_Imp_ClarisWorks::RecognizeSuffix(const char * szSuffix)
{
   return (UT_stricmp(szSuffix,".cwk") == 0);
}

UT_Error IE_Imp_ClarisWorks::StaticConstructor(PD_Document * pDocument,
                                           IE_Imp ** ppie)
{
   IE_Imp_ClarisWorks * p = new IE_Imp_ClarisWorks(pDocument);
   *ppie = p;
   return UT_OK;
}

UT_Bool	IE_Imp_ClarisWorks::GetDlgLabels(const char ** pszDesc,
                                     const char ** pszSuffixList,
                                     IEFileType * ft)
{
   *pszDesc = "ClarisWorks/AppleWorks 5 (.cwk)";
   *pszSuffixList = "*.cwk";
   *ft = IEFT_ClarisWorks;
   return UT_TRUE;
}

UT_Bool IE_Imp_ClarisWorks::SupportsFileType(IEFileType ft)
{
   return (IEFT_ClarisWorks == ft);
}




