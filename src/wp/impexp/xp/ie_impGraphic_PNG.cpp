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

#include "ut_string.h"

#include "ie_impGraphic_PNG.h"
#include "fg_GraphicRaster.h"

UT_Bool IE_ImpGraphic_PNG::RecognizeSuffix(const char * szSuffix)
{
	return (UT_stricmp(szSuffix,".png") == 0);
}

UT_Bool IE_ImpGraphic_PNG::GetDlgLabels(const char ** pszDesc,
									   const char ** pszSuffixList,
									   IEGraphicFileType * ft)
{
	*pszDesc = "Portable Network Graphics (.png)";
	*pszSuffixList = "*.png";
	*ft = IEGFT_PNG;
	return UT_TRUE;
}

UT_Bool IE_ImpGraphic_PNG::SupportsFileType(IEGraphicFileType ft)
{
	return (IEGFT_PNG == ft);
}

UT_Error IE_ImpGraphic_PNG::StaticConstructor(IE_ImpGraphic **ppieg)
{
	*ppieg = new IE_ImpGraphic_PNG();
	if (*ppieg == NULL)
	  return UT_IE_NOMEMORY;

	return UT_OK;
}

//  This actually creates our FG_Graphic object for a PNG
UT_Error IE_ImpGraphic_PNG::importGraphic(UT_ByteBuf* pBB, 
										  FG_Graphic ** ppfg)
{
	FG_GraphicRaster *pFGR;

	pFGR = new FG_GraphicRaster();
	if(pFGR == NULL)
		return UT_IE_NOMEMORY;

	if(!pFGR->setRaster_PNG(pBB)) {
		DELETEP(pFGR);
		
		return UT_IE_BOGUSDOCUMENT;
	}

	*ppfg = (FG_Graphic *) pFGR;
	return UT_OK;
}
