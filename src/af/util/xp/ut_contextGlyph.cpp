/* AbiSource Program Utilities
 * Copyright (C) 2001 AbiSource, Inc.
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
#include <string.h>
#include "ut_contextGlyph.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"
#include "xap_App.h"
#include "xap_Prefs_SchemeIds.h"

struct LigatureData
{
	UT_UCSChar code;
	UT_UCSChar next;
	//UT_UCSChar prev;
};

// these tables have to be sorted by the first two numbers !!!

// in the ligature table, use 1 to indicate that no ligature
// glyph exists
static Letter s_ligature[] =
{
	// Latin
	/*
	{0x0066, 0x0066, 0xFB00, 0xFB00, 0xFB00, 0xFB00},
	{0x0066, 0x0069, 0xFB01, 0xFB01, 0xFB01, 0xFB01},
	{0x0066, 0x006C, 0xFB02, 0xFB02, 0xFB02, 0xFB02},
	{0x017F, 0x0074, 0xFB05, 0xFB05, 0xFB05, 0xFB05},
	*/

	// Greek
	{0x0391, 0x0301, 0x0386, 0x0386, 0x0386, 0x0386},
	{0x0395, 0x0301, 0x0388, 0x0388, 0x0388, 0x0388},
	{0x0397, 0x0301, 0x0389, 0x0389, 0x0389, 0x0389},
	{0x0399, 0x0301, 0x038a, 0x038a, 0x038a, 0x038a},
	{0x0399, 0x0308, 0x03aa, 0x03aa, 0x03aa, 0x03aa},
	{0x039F, 0x0301, 0x038c, 0x038c, 0x038c, 0x038c},
	{0x03A5, 0x0301, 0x038e, 0x038e, 0x038e, 0x038e},
	{0x03A5, 0x0308, 0x03ab, 0x03ab, 0x03ab, 0x03ab},
	{0x03A9, 0x0301, 0x038f, 0x038f, 0x038f, 0x038f},
	{0x03B1, 0x0301, 0x03ac, 0x03ac, 0x03ac, 0x03ac},
	{0x03B5, 0x0301, 0x03ad, 0x03ad, 0x03ad, 0x03ad},
	{0x03B7, 0x0301, 0x03ae, 0x03ae, 0x03ae, 0x03ae},
	{0x03B8, 0x0308, 0x03ca, 0x03ca, 0x03ca, 0x03ca},
	{0x03B9, 0x0301, 0x03af, 0x03af, 0x03af, 0x03af},
	{0x03BF, 0x0301, 0x03cc, 0x03cc, 0x03cc, 0x03cc},
	{0x03C5, 0x0301, 0x03cd, 0x03cd, 0x03cd, 0x03cd},
	{0x03C5, 0x0308, 0x03cb, 0x03cb, 0x03cb, 0x03cb},
	{0x03C9, 0x0301, 0x03ce, 0x03ce, 0x03ce, 0x03ce},
	{0x03CA, 0x0301, 0x0390, 0x0390, 0x0390, 0x0390},
	{0x03CB, 0x0301, 0x03b0, 0x03b0, 0x03b0, 0x03b0},
 
	// Hebrew
	{0x05D0, 0x05B7, 0xFB2E, 0xFB2E, 0xFB2E, 0xFB2E},
	{0x05D0, 0x05B8, 0xFB2F, 0xFB2F, 0xFB2F, 0xFB2F},
	{0x05D0, 0x05BC, 0xFB30, 0xFB30, 0xFB30, 0xFB30},
	{0x05D1, 0x05BC, 0xFB31, 0xFB31, 0xFB31, 0xFB31},
	{0x05D2, 0x05BC, 0xFB32, 0xFB32, 0xFB32, 0xFB32},
	{0x05D3, 0x05BC, 0xFB33, 0xFB33, 0xFB33, 0xFB33},
	{0x05D4, 0x05BC, 0xFB34, 0xFB34, 0xFB34, 0xFB34},
	{0x05D5, 0x05B9, 0xFB4B, 0xFB4B, 0xFB4B, 0xFB4B},
	{0x05D5, 0x05BC, 0xFB35, 0xFB35, 0xFB35, 0xFB35},
	{0x05D6, 0x05BC, 0xFB36, 0xFB36, 0xFB36, 0xFB36},
	{0x05D8, 0x05BC, 0xFB38, 0xFB38, 0xFB38, 0xFB38},
	/*{0x05D9, 0x05B4, 0xFB1D, 0xFB1D, 0xFB1D, 0xFB1D},*/ //not found in MS fonts
	{0x05D9, 0x05BC, 0xFB39, 0xFB39, 0xFB39, 0xFB39},
	{0x05DA, 0x05BC, 0xFB3B, 0xFB3B, 0xFB3A, 0xFB3A},
	{0x05DB, 0x05BC, 0xFB3B, 0xFB3B, 0xFB3A, 0xFB3A},
	{0x05DC, 0x05BC, 0xFB3C, 0xFB3C, 0xFB3C, 0xFB3C},
	{0x05DE, 0x05BC, 0xFB3E, 0xFB3E, 1, 1},
	{0x05E0, 0x05BC, 0xFB40, 0xFB40, 1, 1},
	{0x05E1, 0x05BC, 0xFB41, 0xFB41, 0xFB41, 0xFB41},
	{0x05E3, 0x05BC, 0xFB44, 0xFB44, 0xFB43, 0xFB43},
	{0x05E4, 0x05BC, 0xFB44, 0xFB44, 0xFB43, 0xFB43},
	{0x05E6, 0x05BC, 0xFB46, 0xFB46, 1, 1},
	{0x05E7, 0x05BC, 0xFB47, 0xFB47, 0xFB47, 0xFB47},
	{0x05E8, 0x05BC, 0xFB48, 0xFB48, 0xFB48, 0xFB48},
	{0x05E9, 0x05BC, 0xFB49, 0xFB49, 0xFB49, 0xFB49},
	{0x05E9, 0x05C1, 0xFB2A, 0xFB2A, 0xFB2A, 0xFB2A},
	{0x05E9, 0x05C2, 0xFB2B, 0xFB2B, 0xFB2B, 0xFB2B},
	{0x05EA, 0x05BC, 0xFB4A, 0xFB4A, 0xFB4A, 0xFB4A},
	
	
	
	// Arabic
	{0x0644, 0x0622, 1, 1, 0xFEF6, 0xFEf5},
	{0x0644, 0x0623, 1, 1, 0xFEF8, 0xFEF7},
	{0x0644, 0x0625, 1, 1, 0xFEFA, 0xFEF9},
	{0x0644, 0x0627, 1, 1, 0xFEFC, 0xFEFB},

	// Hebrew
	{0xFB2A, 0x05BC, 0xFB2C, 0xFB2C, 0xFB2C, 0xFB2C},
	{0xFB2B, 0x05BC, 0xFB2D, 0xFB2D, 0xFB2D, 0xFB2D},
	{0xFB49, 0x05C1, 0xFB2C, 0xFB2C, 0xFB2C, 0xFB2C},
	{0xFB49, 0x05C2, 0xFB2D, 0xFB2D, 0xFB2D, 0xFB2D},
 
};

static Letter s_lig_rev[NrElements(s_ligature)];

// when adding entries to this table, make sure that you update
// the HEBREW_START and HEBREW_END macros that follow
static Letter s_table[] =
{
	// code_low, code_high, intial, medial, final, stand-alone
 
	// Greek
	{0x03C2, 0x03C3, 0x03C3, 0x03C3, 0x03C2, 0x03C2},
	
	// Hebrew letters
	// the following macro defines how many entries in this table
	// precede the Hebrew section
#define HEBREW_START 1
	{0x05DA, 0x05DB, 0x05DB, 0x05DB, 0x05DA, 0x05DA},
	{0x05DD, 0x05DE, 0x05DE, 0x05DE, 0x05DD, 0x05DD},
	{0x05DF, 0x05E0, 0x05E0, 0x05E0, 0x05DF, 0x05DF},
	{0x05E3, 0x05E4, 0x05E4, 0x05E4, 0x05E3, 0x05E3},
	{0x05E5, 0x05E6, 0x05E6, 0x05E6, 0x05E5, 0x05E5},
	// the following macro defines the index of the last entry in
	// the Hebrew section of the table
#define HEBREW_END 5
	// Arabic
	{0x0621, 0x0621, 0x0621, 0x0621, 0x0621, 0xFE80},
	{0x0622, 0x0622, 0x0622, 0x0622, 0xFE82, 0xFE81},
	{0x0623, 0x0623, 0x0623, 0x0623, 0xFE84, 0xFE83},
	{0x0624, 0x0624, 0x0624, 0x0624, 0xFE86, 0xFE85},
	{0x0625, 0x0625, 0x0625, 0x0625, 0xFE88, 0xFE87},
	{0x0626, 0x0626, 0xFE8B, 0xFE8C, 0xFE8A, 0xFE89},
	{0x0627, 0x0627, 0x0627, 0x0627, 0xFE8E, 0xFE8D},
	{0x0628, 0x0628, 0xFE91, 0xFE92, 0xFE90, 0xFE8F},
	{0x0629, 0x0629, 0x0629, 0x0629, 0xFE94, 0xFE93},
	{0x062a, 0x062a, 0xFE97, 0xFE98, 0xFE96, 0xFE95},
	{0x062b, 0x062b, 0xFE9B, 0xFE9C, 0xFE9A, 0xFE99},
	{0x062c, 0x062c, 0xFE9F, 0xFEA0, 0xFE9E, 0xFE9D},
	{0x062d, 0x062d, 0xFEA3, 0xFEA4, 0xFEA2, 0xFEA1},
	{0x062e, 0x062e, 0xFEA7, 0xFEA8, 0xFEA6, 0xFEA5},
	{0x062f, 0x062f, 0x062F, 0x062F, 0xFEAA, 0xFEA9},
	{0x0630, 0x0630, 0x0630, 0x0630, 0xFEAC, 0xFEAB},
	{0x0631, 0x0631, 0x0631, 0x0631, 0xFEAE, 0xFEAD},
	{0x0632, 0x0632, 0x0632, 0x0632, 0xFEB0, 0xFEAF},
	{0x0633, 0x0633, 0xFEB3, 0xFEB4, 0xFEB2, 0xFEB1},
	{0x0634, 0x0634, 0xFEB7, 0xFEB8, 0xFEB6, 0xFEB5},
	{0x0635, 0x0635, 0xFEBB, 0xFEBC, 0xFEBA, 0xFEB9},
	{0x0636, 0x0636, 0xFEBF, 0xFEC0, 0xFEBE, 0xFEBD},
	{0x0637, 0x0637, 0xFEC3, 0xFEC4, 0xFEC2, 0xFEC1},
	{0x0638, 0x0638, 0xFEC7, 0xFEC8, 0xFEC6, 0xFEC5},
	{0x0639, 0x0639, 0xFECB, 0xFECC, 0xFECA, 0xFEC9},
	{0x063a, 0x063a, 0xFECF, 0xFED0, 0xFECE, 0xFECD},
	{0x0641, 0x0641, 0xFED3, 0xFED4, 0xFED2, 0xFED1},
	{0x0642, 0x0642, 0xFED7, 0xFED8, 0xFED6, 0xFED5},
	{0x0643, 0x0643, 0xFEDB, 0xFEDC, 0xFEDA, 0xFED9},
	{0x0644, 0x0644, 0xFEDF, 0xFEE0, 0xFEDE, 0xFEDD},
	{0x0645, 0x0645, 0xFEE3, 0xFEE4, 0xFEE2, 0xFEE1},
	{0x0646, 0x0646, 0xFEE7, 0xFEE8, 0xFEE6, 0xFEE5},
	{0x0647, 0x0647, 0xFEEB, 0xFEEC, 0xFEEA, 0xFEE9},
	{0x0648, 0x0648, 0x0648, 0x0648, 0xFEEE, 0xFEED},
	{0x0649, 0x0649, 0x0649, 0x0649, 0xFEF0, 0xFEEF},
	{0x064a, 0x064a, 0xFEF3, 0xFEF4, 0xFEF2, 0xFEF1},
 
	/* the following characters are not found in Arabic Presentation forms B
		and so we will leave it for now -- most of these are ligatures
		
	{0x0660, 0x0660, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0661, 0x0661, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0662, 0x0662, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0663, 0x0663, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0664, 0x0664, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0665, 0x0665, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0666, 0x0666, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0667, 0x0667, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0668, 0x0668, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0669, 0x0669, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x066a, 0x066a, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x066b, 0x066b, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x066c, 0x066c, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x066d, 0x066d, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0671, 0x0671, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0672, 0x0672, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0673, 0x0673, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0674, 0x0674, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0675, 0x0675, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0676, 0x0676, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0677, 0x0677, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0678, 0x0678, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0679, 0x0679, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x067a, 0x067a, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x067b, 0x067b, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x067c, 0x067c, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x067d, 0x067d, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x067e, 0x067e, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x067f, 0x067f, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0680, 0x0680, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0681, 0x0681, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0682, 0x0682, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0683, 0x0683, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0684, 0x0684, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0685, 0x0685, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0686, 0x0686, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0687, 0x0687, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0688, 0x0688, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0689, 0x0689, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x068a, 0x068a, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x068b, 0x068b, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x068c, 0x068c, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x068d, 0x068d, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x068e, 0x068e, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x068f, 0x068f, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0690, 0x0690, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0691, 0x0691, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0692, 0x0692, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0693, 0x0693, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0694, 0x0694, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0695, 0x0695, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0696, 0x0696, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0697, 0x0697, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0698, 0x0698, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x0699, 0x0699, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x069a, 0x069a, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x069b, 0x069b, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x069c, 0x069c, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x069d, 0x069d, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x069e, 0x069e, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x069f, 0x069f, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06a0, 0x06a0, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06a1, 0x06a1, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06a2, 0x06a2, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06a3, 0x06a3, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06a4, 0x06a4, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06a5, 0x06a5, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06a6, 0x06a6, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06a7, 0x06a7, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06a8, 0x06a8, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06a9, 0x06a9, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06aa, 0x06aa, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06ab, 0x06ab, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06ac, 0x06ac, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06ad, 0x06ad, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06ae, 0x06ae, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06af, 0x06af, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06b0, 0x06b0, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06b1, 0x06b1, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06b2, 0x06b2, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06b3, 0x06b3, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06b4, 0x06b4, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06b5, 0x06b5, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06b6, 0x06b6, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06b7, 0x06b7, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06b8, 0x06b8, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06b9, 0x06b9, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06ba, 0x06ba, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06bb, 0x06bb, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06bc, 0x06bc, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06bd, 0x06bd, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06be, 0x06be, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06bf, 0x06bf, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06c0, 0x06c0, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06c1, 0x06c1, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06c2, 0x06c2, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06c3, 0x06c3, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06c4, 0x06c4, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06c5, 0x06c5, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06c6, 0x06c6, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06c7, 0x06c7, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06c8, 0x06c8, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06c9, 0x06c9, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06ca, 0x06ca, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06cb, 0x06cb, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06cc, 0x06cc, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06cd, 0x06cd, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06ce, 0x06ce, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06cf, 0x06cf, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06d0, 0x06d0, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06d1, 0x06d1, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06d2, 0x06d2, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06d3, 0x06d3, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06d4, 0x06d4, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06d5, 0x06d5, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06e5, 0x06e5, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06e6, 0x06e6, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06f0, 0x06f0, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06f1, 0x06f1, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06f2, 0x06f2, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06f3, 0x06f3, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06f4, 0x06f4, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06f5, 0x06f5, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06f6, 0x06f6, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06f7, 0x06f7, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06f8, 0x06f8, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06f9, 0x06f9, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06fa, 0x06fa, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06fb, 0x06fb, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06fc, 0x06fc, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06fd, 0x06fd, 0xFE, 0xFE, 0xFE, 0xFE},
	{0x06fe, 0x06fe, 0xFE, 0xFE, 0xFE, 0xFE},
	*/
};

static UCSRange s_ignore[] =
{
	// Hebrew overstriking characters
	{0x0591,0x05A1},
	{0x05A3,0x05B9},
	{0x05BB,0x05BD},
	{0x05BF,0x05BF},
	{0x05C1,0x05C2},
	{0x05C4,0x05C4},
	
	// Arabic overstriking characters
	{0x064B,0x0655},
	{0x06D6,0x06E8},
	{0x06EA,0x06ED}
};

// comparison function for bsearch
static int s_comp(const void *a, const void *b)
{
	const UT_UCSChar* A = (const UT_UCSChar*)a;
	const Letter* B = (const Letter*)b;
	
	if(*A < B->code_low)
		return -1;
	if(*A > B->code_high)
		return 1;
		
	return 0;
	
}

static int s_comp_ignore(const void *a, const void *b)
{
	const UT_UCSChar* A = (const UT_UCSChar*)a;
	const UCSRange* B = (const UCSRange*)b;
	if(*A < B->low)
		return -1;
	if(*A > B->high)
		return 1;
	
	return 0;
}

// this function is special in that we want it to find our ligature glyph
// if our charcter is the first part of the ligature or if it is the second
// part of the ligature; we can determine which one it is later by comparing
// our character to the two chars that makeup the ligature.

static int s_comp_lig(const void *a, const void *b)
{
	const LigatureData * A = (const LigatureData*)a;
	const Letter * B = (const Letter*)b;
	
	int ret = (int) A->code - (int) B->code_low;
	if(!ret)
		ret = (int) A->next - (int) B->code_high;

	return ret; 
}

static int s_comp_lig2(const void *a, const void *b)
{
	const LigatureData * A = (const LigatureData*)a;
	const Letter * B = (const Letter*)b;
	
	int ret = (int) A->code - (int) B->code_high;
	if(!ret)
		ret = (int) A->next - (int) B->code_low;

	return ret;
}


static int s_comp_qlig(const void *a, const void *b)
{
	const Letter *A = (const Letter*)a;
	const Letter *B = (const Letter*)b;

	int ret = (int) A->code_high - (int) B->code_high;
	if(!ret)
		return (int) A->code_low - (int) B->code_low;

	return ret;
}

Letter* UT_contextGlyph::s_pGlyphTable = &s_table[0];
UCSRange * UT_contextGlyph::s_pIgnore = &s_ignore[0];
Letter* UT_contextGlyph::s_pLigature = &s_ligature[0];
Letter* UT_contextGlyph::s_pLigRev = &s_lig_rev[0];
bool UT_contextGlyph::s_bInit = false;
UT_uint32 UT_contextGlyph::s_iGlyphTableSize = sizeof(s_table);

UT_contextGlyph::UT_contextGlyph()
{
	if(!s_bInit)
	{
		bool bHebrewContextGlyphs = false;
		XAP_App::getApp()->getPrefsValueBool((XML_Char*)XAP_PREF_KEY_UseHebrewContextGlyphs, &bHebrewContextGlyphs);
		UT_DEBUGMSG(("UT_contextGlyph Constructor: bHebrewContextGlyphs %d\n",bHebrewContextGlyphs));
		if(!bHebrewContextGlyphs)
		{
			s_iGlyphTableSize -= (HEBREW_END - HEBREW_START + 1) * sizeof(Letter);
			memcpy(&s_table[HEBREW_START], &s_table[HEBREW_END + 1],s_iGlyphTableSize - HEBREW_START);
		}

		memcpy(s_pLigRev,s_pLigature, sizeof(s_ligature));
		qsort(s_pLigRev,NrElements(s_lig_rev), sizeof(Letter),s_comp_qlig);
		s_bInit = true;
	}
}

inline GlyphContext UT_contextGlyph::_evalGlyphContext(const UT_UCSChar* code, const UT_UCSChar * prev, const UT_UCSChar * next) const
{
	xxx_UT_DEBUGMSG(("UT_contextGlyph::_evalGlyphContext: code 0x%x, prev 0x%x [*0x%x], next 0x%x [*0x%x]\n",
				*code, prev, *prev, next, *next ));
				
	if((!next || !*next) && (!prev || !*prev))
		return GC_ISOLATE;
	
	bool bPrevWD;
	bool bNextWD;
	
	if((!next || !*next) && prev && *prev)
	{
		bPrevWD = UT_isWordDelimiter(*prev, *code);
		if(bPrevWD)
			return GC_ISOLATE;
		else
			return GC_FINAL;
	}
		
	// no-next has been trapped above, now we can
	//check if next is not a character that is to be ignored
	const UT_UCSChar *myNext = next;
	
	while(*myNext && bsearch((void*)myNext, (void*)s_pIgnore, NrElements(s_ignore),sizeof(UCSRange),s_comp_ignore))
		myNext++;
  
	const UT_UCSChar myNextNext = (*myNext && *(myNext+1)) ?  *(myNext+1) : UCS_SPACE;

	xxx_UT_DEBUGMSG(("UT_contextGlyph::_evalGlyphContext: myNext 0x%x\n", *myNext));

	if((!prev || !*prev) && !*myNext)
		return GC_ISOLATE;
		
	if((!prev || !*prev) && *myNext)
	{
		bNextWD = UT_isWordDelimiter(*myNext, myNextNext);
		if(bNextWD)
			return GC_ISOLATE;
		else
			return GC_INITIAL;
	}
		
	if(*prev && !*myNext)
	{
		bPrevWD = UT_isWordDelimiter(*prev, *code);
		if(bPrevWD)
			return GC_ISOLATE;
		else
			return GC_FINAL;
		
	}
	xxx_UT_DEBUGMSG(("UT_contextGlyph::_evalGlyphContext: code 0x%x, prev 0x%x, myNext 0x%x, myNextNext 0x%x\n",*code,*prev, *myNext,myNextNext));
	bPrevWD = UT_isWordDelimiter(*prev, *code);
	bNextWD = UT_isWordDelimiter(*myNext, myNextNext);
	
	// if both are not , then medial form is needed
	if(!bPrevWD && !bNextWD)
		return GC_MEDIAL;
	
	// if only *next is, than final form is needed
	if(bNextWD)
		return GC_FINAL;
		
	// if *prev is, the initial form is needed
	if(bPrevWD)
		return GC_INITIAL;
		
	// if we got here, both are delimiters, which means stand alone form is needed
	return GC_ISOLATE;
		
}
/*
	code - pointer to the character to interpret
	prev - pointer to the character before code
	next - NULL-terminated string of characters that follow
	
	returns the glyph to be drawn
*/
UT_UCSChar UT_contextGlyph::getGlyph(const UT_UCSChar * code,
									 const UT_UCSChar * prev,
									 const UT_UCSChar * next) const
{
	
	UT_ASSERT(code);
	
	// first, decide if this is a part of a ligature
	// first check for a ligature form
	LigatureData Lig;
	Letter *pL = 0;
	bool bIsSecond = false;
	GlyphContext context = GC_NOT_SET;

	Lig.next = next ? *next : 0;
	Lig.code = *code;
		
	pL = (Letter*) bsearch((void*)&Lig, (void*)s_pLigature, NrElements(s_ligature),sizeof(Letter),s_comp_lig);

	if(pL)
	{
		next++;
		xxx_UT_DEBUGMSG(("UT_contextGlyph::getGlyph: char 0x%x, 1st part of ligature\n", *code));			
	}
	else
	{
		Lig.next = prev ? *prev : 0;
		pL = (Letter*) bsearch((void*)&Lig, (void*)s_pLigRev, NrElements(s_lig_rev),sizeof(Letter),s_comp_lig2);
		if(pL)
		{
			xxx_UT_DEBUGMSG(("UT_contextGlyph::getGlyph: char 0x%x, 2nd part of ligature\n", *code));
			bIsSecond = true;
		}
	}

	// if this is a ligature, handle it
	if(pL)
	{
		context = bIsSecond ? _evalGlyphContext(prev, prev+1, next)
											: _evalGlyphContext(code, prev, next);
		UT_UCSChar glyph = 0;
		switch (context)
		{
			case GC_INITIAL:
				glyph = pL->initial;
				break;
			case GC_MEDIAL:
				glyph = pL->medial;
				break;
			case GC_FINAL:
				glyph = pL->final;
				break;
			case GC_ISOLATE:
				glyph = pL->alone;
				break;
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		}
		
		UT_DEBUGMSG(("UT_contextGlyph::getGlyph: ligature (%d), glyph 0x%x\n", bIsSecond, glyph));
		if(!bIsSecond && glyph != 1) //first part of a ligature
			return glyph;
		if(bIsSecond && glyph != 1)
		{
			// a special ligature glyph was used, map this to a 0-width non breaking space
			return 0xFEFF;
		}

		// if we got here, the glyph was 1, which means this form is to be just
		// treated as an ordinary letter, also if this was a first part of the ligature
		// we already know its context, but not if it was a second part of lig.
	}


	// if we have no pL we are dealing with an ordinary letter
	pL = (Letter*) bsearch((void*)code, (void*)s_pGlyphTable, s_iGlyphTableSize/sizeof(Letter),sizeof(Letter),s_comp);
	
	// if we still have no pL, it means the letter has only one form
	// so we return it back
	if(!pL)
		return *code;

	if(context == GC_NOT_SET || bIsSecond)
		context = _evalGlyphContext(code, prev, next);
		
	switch (context)
	{
		case GC_INITIAL:
			return pL->initial;
		case GC_MEDIAL:
			return pL->medial;
		case GC_FINAL:
			return pL->final;
		case GC_ISOLATE:
			return pL->alone;
		default:
			UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	}
	
	return 0;
}

void UT_contextGlyph::renderString(const UT_UCSChar * src,
							UT_UCSChar *dest,
							UT_uint32 len,
							const UT_UCSChar * prev,
							const UT_UCSChar * next) const
{
	UT_ASSERT(src);
	UT_ASSERT(dest);
	
	const UT_UCSChar * src_ptr = src;
	UT_UCSChar * dst_ptr = dest;
	const UT_UCSChar * next_ptr;
	UT_UCSChar prev_tmp[2] = {0,0};
	UT_UCSChar next_tmp[CONTEXT_BUFF_SIZE + 1] = {0,0,0,0,0,0};
	const UT_UCSChar * prev_ptr;
	UT_UCSChar glyph = 0;
	
	for(UT_uint32 i = 0; i < len; i++, src_ptr++)	
	{
		// first, decide if this is a part of a ligature
		// first check for a ligature form
		LigatureData Lig;
		Letter *pL = 0;
		bool bIsSecond = false;
		GlyphContext context = GC_NOT_SET;

		//get the current context
		if(len > CONTEXT_BUFF_SIZE && i < len - CONTEXT_BUFF_SIZE)
			next_ptr = src_ptr + 1;
		else
		{
			next_ptr = next_tmp;
			UT_uint32 j;
			for(j = 0; j < len - i - 1 && j < CONTEXT_BUFF_SIZE; j++)
				next_tmp[j] = *(src_ptr + 1 + j);
			for(; j < CONTEXT_BUFF_SIZE; j++)
				next_tmp[j] = *(next + (j + i + 1 - len));
		}
		
		if(i == 0)
			prev_ptr = prev;
		else if(i == 1)
		{
			prev_tmp[0] = *src;
			prev_tmp[1] = *prev;
			prev_ptr = prev_tmp;
		}
		else
		{
			prev_tmp[0] = *(src_ptr - 1);
			prev_tmp[1] = *(src_ptr - 2);
			//no need to set prev_ptr, since this has been done when i == 1
		}
			
		Lig.next = next_ptr ? *next_ptr : 0;
		Lig.code = *src_ptr;
		
		pL = (Letter*) bsearch((void*)&Lig, (void*)s_pLigature, NrElements(s_ligature),sizeof(Letter),s_comp_lig);

		if(pL)
		{
			// we need the context of the whole pair not just of this character
			next_ptr++;
			xxx_UT_DEBUGMSG(("UT_contextGlyph::getGlyph: char 0x%x, 1st part of ligature\n", *code));			
		}
		else
		{
			//we only check that this is not a second part of a ligature for the
			//first character, for the rest we will handle that in the previous
			//cycle of the loop
			if(i == 0)
			{
				Lig.next = prev ? *prev : 0;
				pL = (Letter*) bsearch((void*)&Lig, (void*)s_pLigRev, NrElements(s_lig_rev),sizeof(Letter),s_comp_lig2);
				if(pL)
				{
					xxx_UT_DEBUGMSG(("UT_contextGlyph::getGlyph: char 0x%x, 2nd part of ligature\n", *code));
					bIsSecond = true;
				}
			}
		}

		// if this is a ligature, handle it
		if(pL)
		{
			context = bIsSecond ? _evalGlyphContext(prev, prev+1, next_ptr)
											: _evalGlyphContext(src_ptr, prev_ptr, next_ptr);
			switch (context)
			{
				case GC_INITIAL:
					glyph = pL->initial;
					break;
				case GC_MEDIAL:
					glyph = pL->medial;
					break;
				case GC_FINAL:
					glyph = pL->final;
					break;
				case GC_ISOLATE:
					glyph = pL->alone;
					break;
				default:
					UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
			}
		
			UT_DEBUGMSG(("UT_contextGlyph::getGlyph: ligature (%d), glyph 0x%x\n", bIsSecond, glyph));
			if(!bIsSecond && glyph != 1) //first part of a ligature
			{
				// we set both this and the next char if we can
				*dst_ptr++ = glyph;
				if(i < len - 1)
				{
					*dst_ptr++ = 0xFEFF;
					src_ptr++;
					i++;
					continue;
				}
				continue;
			}
			else if(bIsSecond && glyph != 1)
			{
				// a special ligature glyph was used, map this to a 0-width non breaking space
				*dst_ptr++ = 0xFEFF;
				continue;
			}

			// if we got here, the glyph was 1, which means this form is to be just
			// treated as an ordinary letter, also if this was a first part of the ligature
			// we already know its context, but not if it was a second part of lig.
		}


		// if we have no pL we are dealing with an ordinary letter
		pL = (Letter*) bsearch((void*)src_ptr, (void*)s_pGlyphTable, s_iGlyphTableSize/sizeof(Letter),sizeof(Letter),s_comp);

		// if we still have no pL, it means the letter has only one form
		// so we return it back
		if(!pL)
		{
			*dst_ptr++ = *src_ptr;
			continue;
		}

		if(context == GC_NOT_SET || bIsSecond)
			context = _evalGlyphContext(src_ptr, prev_ptr, next_ptr);
		
		switch (context)
		{
			case GC_INITIAL:
				glyph = pL->initial;
				break;
			case GC_MEDIAL:
				glyph = pL->medial;
				break;
			case GC_FINAL:
				glyph = pL->final;
				break;
			case GC_ISOLATE:
				glyph = pL->alone;
				break;
			default:
				UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
		}
	
		*dst_ptr++ = glyph;
	}		

}
