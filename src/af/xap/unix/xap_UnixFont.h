/* AbiSource Application Framework
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


#ifndef AP_UNIXFONT_H
#define AP_UNIXFONT_H

#include <fstream.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>

#include "ut_types.h"
#include "ut_vector.h"

/*****************************************************************/

class AP_UnixFont
{
public:

	typedef enum
	{
		STYLE_NORMAL = 0,
		STYLE_BOLD,
		STYLE_ITALIC,
		STYLE_BOLD_ITALIC,
		STYLE_LAST	// this must be last
	} style;

	
	AP_UnixFont(void);
	~AP_UnixFont(void);

	UT_Bool 				openFileAs(const char * fontfile,
									   const char * metricfile,
									   const char * xlfd,
									   AP_UnixFont::style s);
	const char * 			getName(void);
	AP_UnixFont::style		getStyle(void);
	const char * 			getFontfile(void);
	const char * 			getMetricfile(void);
	const char * 			getXLFD(void);
	
	UT_Bool					openPFA(void);
	char					getPFAChar(void);
	UT_Bool					closePFA(void);	

	const char * 			getFontKey(void);
	GdkFont *				getGdkFont(UT_uint16 pointsize);

protected:

	struct allocFont
	{
		UT_uint16			pointSize;
		GdkFont *			gdkFont;
	};

	void					_makeFontKey();
	char * 					m_fontKey;

	// a cache of GdkFont * at a given size
	UT_Vector				m_allocFonts;
	
	// expand this data to account for other attributes of a
	// font, like character set or special decorations.
	char * 					m_name;
	AP_UnixFont::style		m_style;
	char * 					m_xlfd;

	char * 					m_fontfile;
	char *					m_metricfile;

	ifstream * 				m_PFAFile;
};

#endif /* AP_UNIXFONT_H */
