// fp_PageSize.cpp
#include "string.h"

#include "fp_PageSize.h"
#include "ut_units.h"
#include "ut_assert.h"
#include "ut_debugmsg.h"


// This class stores the pagesize in mm. The resoning behind
// that is that mm is at least a derived unit from an ISO standard.
// If anyone think it's more apropriate to express it in meters,
// you're free to change the implementation.

const int cMaxSymbolicLength = 25;

struct private_pagesize_sizes
{
	double w; // width
	double h; // height

	fp_PageSize::Unit u; // unit for all these values
	char name[cMaxSymbolicLength]; // symbolic name

	double l; // left margin
	double r; // right margin
	double t; // top margin
	double b; // bottom margin
};

// all paper sizes with this value set for any of its margins
// means that i don't know what the correct value is. we should
// find these out ASAP! Default to 1inch (25.4mm)
#define MARGIN_UNKNOWN 25.4

const private_pagesize_sizes
pagesizes[fp_PageSize::_last_predefined_pagesize_dont_use_] =
{
	// the A sizes
	{1682.0, 2378.0, fp_PageSize::mm,	"4A", 
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{1189.0, 1682.0, fp_PageSize::mm,	"2A", 
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 841.0, 1189.0, fp_PageSize::mm,	"A0", 
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 594.0,  841.0, fp_PageSize::mm,	"A1",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 420.0,  594.0, fp_PageSize::mm,	"A2",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 297.0,  420.0, fp_PageSize::mm,	"A3", 28.0, 28.0, 28.0, 28.0 },
	{ 210.0,  297.0, fp_PageSize::mm,	"A4", 28.0, 28.0, 28.0, 28.0 },
	{ 148.0,  210.0, fp_PageSize::mm,	"A5", 28.0, 28.0, 28.0, 28.0 },
	{ 105.0,  148.0, fp_PageSize::mm,	"A6",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  74.0,  105.0, fp_PageSize::mm,	"A7",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  52.0,  74.0, fp_PageSize::mm,	"A8",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  37.0,  52.0, fp_PageSize::mm,	"A9",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  26.0,  37.0, fp_PageSize::mm,	"A10",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },

	// the B sizes
	{2000.0, 2828.0, fp_PageSize::mm,	"4B",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{1414.0, 2000.0, fp_PageSize::mm,	"2B",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{1000.0, 1414.0, fp_PageSize::mm,	"B0",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 707.0, 1000.0, fp_PageSize::mm,	"B1",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 500.0,  707.0, fp_PageSize::mm,	"B2",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 353.0,  500.0, fp_PageSize::mm,	"B3",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 250.0,  353.0, fp_PageSize::mm,	"B4", 21.0, 21.0, 21.0, 21.0 },
	{ 176.0,  250.0, fp_PageSize::mm,	"B5", 28.0, 28.0, 28.0, 28.0 },
	{ 125.0,  176.0, fp_PageSize::mm,	"B6",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  88.0,  125.0, fp_PageSize::mm,	"B7",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  62.0,   88.0, fp_PageSize::mm,	"B8",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  44.0,   62.0, fp_PageSize::mm,	"B9",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  31.0,   44.0, fp_PageSize::mm,	"B10",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },

	// the C sizes
	{ 917.0, 1297.0, fp_PageSize::mm,	"C0",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 648.0,  917.0, fp_PageSize::mm,	"C1",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 458.0,  648.0, fp_PageSize::mm,	"C2",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 324.0,  458.0, fp_PageSize::mm,	"C3",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{ 229.0,  324.0, fp_PageSize::mm,	"C4",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	// FIXME: C5 is dealt with below, under envelopes
	// FIXME: should prolly have C6/C5 here too, for completeness
	{ 114.0,  162.0, fp_PageSize::mm,	"C6",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  81.0,  114.0, fp_PageSize::mm,	"C7",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  57.0,   81.0, fp_PageSize::mm,	"C8",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  40.0,   57.0, fp_PageSize::mm,	"C9",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	{  28.0,   40.0, fp_PageSize::mm,	"C10",
	  MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN, MARGIN_UNKNOWN },
	
	// Japanese B sizes
	// FIXME: should prolly have the other Japanese sizes
	{ 182.0,  258.0, fp_PageSize::mm,   "B5-Japan", 28.0, 28.0, 28.0, 28.0},

	// the rest
	{   8.5,   14.0, fp_PageSize::inch,	"Legal", 1.0, 1.0, 1.0, 1.0	},
	{   8.5,   13.0, fp_PageSize::inch,	"Folio", 1.0, 1.0, 1.0, 1.0	},
	{   8.5,   11.0, fp_PageSize::inch,	"Letter", 1.0, 1.0, 1.0, 1.0 },
	{   8.5,    5.5, fp_PageSize::inch, "Half-Letter", 1.0, 1.0, 1.0, 1.0 },
	{   7.5,   10.0, fp_PageSize::inch, "Executive", 1.0, 1.0, 1.0, 1.0 },
	{ 280.1,  267.0, fp_PageSize::mm,   "Tabloid/Ledger", 25.4, 25.4, 25.4, 25.4},
	{  99.0,  191.0, fp_PageSize::mm,   "Monarch", 3.5, 3.5, 3.5, 3.5},
	{ 297.0,  433.0, fp_PageSize::mm,   "SuperB", 28.2, 28.2, 28.2, 28.2},
	{ 105.0,  242.0, fp_PageSize::mm,   "Envelope-Commercial", 1.8, 1.8, 1.8, 1.8},
	{  99.0,  191.0, fp_PageSize::mm,   "Envelope-Monarch", 1.8, 1.8, 1.8, 1.8},
	{ 110.0,  220.0, fp_PageSize::mm,   "Envelope-DL", 1.8, 1.8, 1.8, 1.8},
	{ 162.0,  229.0, fp_PageSize::mm,   "Envelope-C5", 1.8, 1.8, 1.8, 1.8},
	{ 105.0,  148.0, fp_PageSize::mm,   "EuroPostcard", 1.8, 1.8, 1.8, 1.8},
	{   0.0,    0.0, fp_PageSize::mm,	"Custom", 0.0, 0.0, 0.0, 0.0}
};

const double ScaleFactors[fp_PageSize::_last_predefined_unit_dont_use_] =
{
	{ 1.0								},		// mm
	{ 10.0								},		// cm
	{ 25.4								},		// inch
	{ 25.4 / UT_PAPER_UNITS_PER_INCH	},		// PaperUnit
	{ 25.4 / UT_LAYOUT_UNITS			}		// LayoutUnit
};

fp_PageSize::fp_PageSize(Predefined preDef)
{
	m_bisPortrait = true;
	Set(preDef);
	m_scale = 1.0;
}

fp_PageSize::fp_PageSize(const char *name)
{
	m_bisPortrait = true;
	m_scale = 1.0;
	Set(name);
}

fp_PageSize::fp_PageSize(double w, double h, Unit u)
{
	UT_ASSERT(u >= _first_predefined_unit_ && u < _last_predefined_unit_dont_use_);
	m_bisPortrait = true;
	m_scale = 1.0;
	Set(w, h, u);
}

// all Set() calls ultimately go through this function
void fp_PageSize::Set(Predefined preDef, Unit u)
{
	UT_ASSERT(preDef >= _first_predefined_pagesize_ && preDef < _last_predefined_pagesize_dont_use_);
	UT_ASSERT(u >= _first_predefined_unit_ && u <= _last_predefined_unit_dont_use_);

	const private_pagesize_sizes& size = pagesizes[preDef];

	if (u != _last_predefined_unit_dont_use_)
		m_unit = u;
	else
		m_unit = size.u;

	m_iWidth        = size.w * ScaleFactors[m_unit];
	m_iHeight       = size.h * ScaleFactors[m_unit];
	m_iMarginTop    = size.t * ScaleFactors[m_unit];
	m_iMarginBottom = size.b * ScaleFactors[m_unit];
	m_iMarginLeft   = size.l * ScaleFactors[m_unit];
	m_iMarginRight  = size.r * ScaleFactors[m_unit];

	m_predefined = (char *)pagesizes [preDef].name;
}

void fp_PageSize::Set(double w, double h, Unit u)
{
	int i;
	double converted_w, converted_h;

	UT_ASSERT(u >= _first_predefined_unit_ && u < _last_predefined_unit_dont_use_);

	// calculate which predefined this represents

	for (i = _first_predefined_pagesize_; 
		 i < (int)_last_predefined_pagesize_dont_use_; i++)
	{
		if (pagesizes[i].u != u )  // Convert to local defined units and round off
		{
			converted_w = w * ScaleFactors[u]/ScaleFactors[pagesizes[i].u];
			int w_int = (int) (converted_w*10.0);
			if ( converted_w*10 - w_int >= 0.5 ) w_int++;
			converted_w = (double) w_int/10.0;
			converted_h = h * ScaleFactors[u]/ScaleFactors[pagesizes[i].u];
			int h_int = (int) (converted_h*10.0);
			if ( converted_h*10 - h_int >= 0.5 ) h_int++;
			converted_h = (double) h_int/10.0;			
		}
		else
		{
			converted_w = w;
			converted_h = h;
		}

	    if ((pagesizes [i].w == converted_w) && 
			(pagesizes [i].h == converted_h))
		{
			Set(static_cast<Predefined>(i), u);
			break;
		}
		if ((pagesizes [i].h == converted_w) &&
			(pagesizes [i].w == converted_h))
		{
			Set(static_cast<Predefined>(i), u);
			m_bisPortrait = false;
			break;
		}
	}

	// Force Custom Label if nothing else fits
	if ( i == (int)_last_predefined_pagesize_dont_use_ )
	{
		Set (static_cast<Predefined>(i-1), u);
		m_iWidth  = w * ScaleFactors[m_unit];
		m_iHeight = h * ScaleFactors[m_unit];
	}
}

void fp_PageSize::Set(const char *name, Unit u)
{
	UT_ASSERT(u >= _first_predefined_unit_ && u <= _last_predefined_unit_dont_use_);
	UT_DEBUGMSG(("fp_PageSize::Set(\"%s\")\n", (char*) name));

	Set(NameToPredefined(name), u);
}

void fp_PageSize::setPortrait(void)
{
	m_bisPortrait = true;
}

void fp_PageSize::setLandscape(void)
{
	m_bisPortrait = false;
}

double fp_PageSize::Width(Unit u) const
{
	UT_ASSERT(u >= _first_predefined_unit_ && u < _last_predefined_unit_dont_use_);
	if(m_bisPortrait == true)
		return m_scale * m_iWidth / ScaleFactors[u];
	else
		return m_scale * m_iHeight / ScaleFactors[u];
}

double fp_PageSize::Height(Unit u) const
{
	UT_ASSERT(u >= _first_predefined_unit_ && u < _last_predefined_unit_dont_use_);
	if(m_bisPortrait == true)
		return m_scale * m_iHeight / ScaleFactors[u];
	else
		return m_scale * m_iWidth / ScaleFactors[u];
}

double fp_PageSize::MarginTop(Unit u) const
{
	UT_ASSERT(u >= _first_predefined_unit_ && u < _last_predefined_unit_dont_use_);
	if(m_bisPortrait == true)
		return m_scale * m_iMarginTop / ScaleFactors[u];
	else
		return m_scale * m_iMarginRight / ScaleFactors[u];
}

double fp_PageSize::MarginBottom(Unit u) const
{
	UT_ASSERT(u >= _first_predefined_unit_ && u < _last_predefined_unit_dont_use_);
	if(m_bisPortrait == true)
		return m_scale * m_iMarginBottom / ScaleFactors[u];
	else
		return m_scale * m_iMarginLeft / ScaleFactors[u];
}

double fp_PageSize::MarginLeft(Unit u) const
{
	UT_ASSERT(u >= _first_predefined_unit_ && u < _last_predefined_unit_dont_use_);
	if(m_bisPortrait == true)
		return m_scale * m_iMarginLeft / ScaleFactors[u];
	else
		return m_scale * m_iMarginTop / ScaleFactors[u];
}

double fp_PageSize::MarginRight(Unit u) const
{
	UT_ASSERT(u >= _first_predefined_unit_ && u < _last_predefined_unit_dont_use_);
	if(m_bisPortrait == true)
		return m_scale * m_iMarginRight / ScaleFactors[u];
	else
		return m_scale * m_iMarginBottom / ScaleFactors[u];
}

bool fp_PageSize::IsPredefinedName(const char* szPageSizeName)
{
	for (int i = (int)_first_predefined_pagesize_;
		 i < (int)_last_predefined_pagesize_dont_use_; ++i)
	{
		if (!strcmp(pagesizes[i].name, szPageSizeName))
		{
			return true;
		}
	}
	return false;
}

fp_PageSize::Predefined fp_PageSize::NameToPredefined(const char *name)
{
	int preDef = 0;
	// determine the predefined layout the name represents

	if(!name)
	{
	    UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	    return fp_PageSize::Letter;
	}

	for(preDef = (int)_first_predefined_pagesize_;
	    preDef < static_cast<int>(_last_predefined_pagesize_dont_use_);
		preDef++)
	{
		if (0 == strcmp(pagesizes[preDef].name, name)) {
			break;
		}
	}

	if ((preDef >= _first_predefined_pagesize_) && (preDef < _last_predefined_pagesize_dont_use_)) {
		return static_cast<Predefined>(preDef);
	}

	UT_ASSERT(UT_SHOULD_NOT_HAPPEN);
	return fp_PageSize::Letter;
}

const char * fp_PageSize::PredefinedToName(Predefined preDef)
{
	UT_ASSERT((preDef >= _first_predefined_pagesize_) && (preDef < _last_predefined_pagesize_dont_use_));

	return pagesizes[preDef].name;
}
