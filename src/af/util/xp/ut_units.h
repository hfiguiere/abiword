/* AbiSource Program Utilities
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



#ifndef UT_UNITS_H
#define UT_UNITS_H

/* pre-emptive dismissal; ut_types.h is needed by just about everything,
 * so even if it's commented out in-file that's still a lot of work for
 * the preprocessor to do...
 */
#ifndef UT_TYPES_H
#include "ut_types.h"
#endif

class GR_Graphics;

UT_BEGIN_EXTERN_C

#define UT_PAPER_UNITS_PER_INCH				100
#define UT_LAYOUT_UNITS						1440

#define UT_DEV_UNITS(x) ((x) >> 10)
#define UT_LOG_UNITS(x) ((x) << 10)


/* helper macros for variable conversions from our logical to device
   units */
#ifndef USE_LAYOUT_UNITS

/* these two macros are for conversion of function parameters:
       int foo(int a, int b)
       {
         _UUD(a);
         _UUD(b);

         // some processing
         ...
       }
*/
#if 1
#define _UUL(x) x = UT_LOG_UNITS(x)
#define _UUD(x) x = UT_DEV_UNITS(x)

/* these two macros are for converison in assignments:
       int a = _UL(b);
*/
#define _UL(x) UT_LOG_UNITS(x)
#define _UD(x) UT_DEV_UNITS(x)
#else
#define _UUL(x)
#define _UUD(x)
#define _UL(x) x
#define _UD(x) x
#endif

#else

#define _UUL(x)
#define _UUD(x)
#define _UL(x) x
#define _UD(x) x

#endif


typedef enum _ut_dimension { DIM_IN, DIM_CM, DIM_MM, DIM_PI, DIM_PT, DIM_PX, DIM_PERCENT, DIM_none } UT_Dimension;

/*
 *	DIM_IN := inches
 *	DIM_CM := centimeters
 *	DIM_MM := millimeters
 *	DIM_PI := picometers
 *	DIM_PT := points
 *	DIM_PX := pixels
 */

double UT_convertToInches(const char* s);
double UT_convertDimToInches (double f, UT_Dimension dim);
ABI_EXPORT double UT_convertDimensions(double f, UT_Dimension from, UT_Dimension to);
ABI_EXPORT double UT_convertToPoints(const char* s);
ABI_EXPORT double UT_convertToDimension(const char* s, UT_Dimension dim);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32 UT_convertToLayoutUnits(const char* s);
UT_sint32 UT_convertSizeToLayoutUnits(double Value, UT_Dimension dim);
#endif
double UT_convertDimensionless(const char * sz);
double UT_convertInchesToDimension(double inches, UT_Dimension dim);

UT_sint32 UT_paperUnits(const char * sz);
double    UT_inchesFromPaperUnits(UT_sint32 iPaperUnits);
UT_sint32 UT_paperUnitsFromInches(double dInches);
UT_sint32 UT_docUnitsFromPaperUnits(GR_Graphics * pG, UT_sint32 iPaperUnits);
#if !defined(WITH_PANGO) && defined(USE_LAYOUT_UNITS)
UT_sint32 UT_layoutUnitsFromPaperUnits(UT_sint32 iPaperUnits);
UT_sint32 UT_paperUnitsFromLayoutUnits(UT_sint32 iLayoutUnits);
#endif

const char * UT_incrementDimString(const char * dimString, double inc);
const char * UT_multiplyDimString(const char * dimString, double mult);
UT_Dimension UT_determineDimension(const char * sz, UT_Dimension fallback = DIM_IN);
ABI_EXPORT const char * UT_dimensionName(UT_Dimension dim);
const char * UT_convertInchesToDimensionString(UT_Dimension, double valueInInches, const char * szPrecision = NULL);
const char * UT_formatDimensionString(UT_Dimension, double value, const char * szPrecision = NULL);
const char * UT_reformatDimensionString(UT_Dimension dim, const char *sz, const char * szPrecision = NULL);
ABI_EXPORT const char * UT_convertToDimensionlessString(double value, const char * szPrecision = NULL);
const char * UT_formatDimensionedValue(double value, const char * szUnits, const char * szPrecision = NULL);

bool UT_hasDimensionComponent(const char * sz);


UT_END_EXTERN_C

#endif /* UT_UNITS_H */
