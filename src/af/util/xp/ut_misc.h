
#ifndef UTMISC_H
#define UTMISC_H

#include "ut_types.h"

class UT_RGBColor;
class UT_Rect;

// ----------------------------------------------------------------
class UT_RGBColor
{
public:
	UT_RGBColor();
	UT_RGBColor(unsigned char, unsigned char, unsigned char);
	
	unsigned char m_red;
	unsigned char m_grn;
	unsigned char m_blu;
};

void UT_parseColor(const char*, UT_RGBColor&);

// ----------------------------------------------------------------
class UT_Rect
{
public:
	UT_Rect();
	void setValues(int, int, int, int);
	
	UT_sint32	left;
	UT_sint32	top;
	UT_sint32	width;
	UT_sint32	height;

protected:
};

#define UT_MAX(A,B)	(((A) > (B)) ? (A) : (B))
#define UT_MIN(A,B)	(((A) < (B)) ? (A) : (B))
#define UT_ABS(A)	( ((A) < 0) ? (-(A)) : (A) )

#endif /* UTMISC_H */
