/* AbiSource Program Utilities
 * Copyright (C) 2001,2002 Francis James Franklin <fjf@alinameridon.com>
 * Copyright (C) 2001,2002 AbiSource, Inc.
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

/* Pseudoheader to include the right XML headers */

#ifndef UTXML_H
#define UTXML_H

/* pre-emptive dismissal; ut_types.h is needed by just about everything,
 * so even if it's commented out in-file that's still a lot of work for
 * the preprocessor to do...
 */
#ifndef UT_TYPES_H
#include "ut_types.h"
#endif
#include "ut_bytebuf.h"

/* XML_Char definition moved to ut_types.h */

ABI_EXPORT char * UT_XML_Decode( const char * inKey );

class ABI_EXPORT UT_XML
{
 public:
  UT_XML();
  ~UT_XML();

  /* Strip "svg:" from "svg:svg" etc. in element names, pass any other namespace indicators
   */
  void setNameSpace (const char * xml_namespace);

 private:
  char * m_chardata_buffer;

  UT_uint32 m_chardata_length;
  UT_uint32 m_chardata_max;

  bool grow (char *& buffer, UT_uint32 & length, UT_uint32 & max, UT_uint32 require);

  bool reset_all ();
  void flush_all ();

  const char * m_namespace;
  int m_nslength;

 public:
  /* Returns true iff the name of the first element is xml_type or opt_namespace:xml_type
   */
  bool sniff (const UT_ByteBuf * pBB, const char * xml_type);
  bool sniff (const char * buffer, UT_uint32 length, const char * xml_type);

 private:
  bool m_bSniffing;
  bool m_bValid;

  const char * m_xml_type;

 public:
  UT_Error parse (const char * szFilename);
  UT_Error parse (const UT_ByteBuf * pBB);
  UT_Error parse (const char * buffer, UT_uint32 length);

 public:
  void stop () { m_bStopped = true; } // call this to stop callbacks and to stop the feed to the parser

 private:
  bool m_bStopped;

 public:
  class ABI_EXPORT Listener
    {
    public:
      virtual ~Listener () {}

      virtual void startElement (const XML_Char * name, const XML_Char ** atts) = 0;
      virtual void endElement (const XML_Char * name) = 0;
      virtual void charData (const XML_Char * buffer, int length) = 0;

    protected:
      Listener () {}
    };

  void setListener (Listener * pListener) { m_pListener = pListener; }

 private:
  Listener * m_pListener;

 public:
  class ABI_EXPORT Reader
    {
    public:
      virtual ~Reader () {}

      virtual bool	openFile (const char * szFilename) = 0;
      virtual UT_uint32	readBytes (char * buffer, UT_uint32 length) = 0;
      virtual void	closeFile (void) = 0;
      
    protected:
      Reader () {}
    };

  void setReader (Reader * pReader) { m_pReader = pReader; }

 private:
  Reader * m_pReader;

 public:
  /* For UT_XML internal use only.
   * 
   * However, it should be possible to set up redirections from one UT_XML into another, if multiple
   * namespaces require it - maybe something like:
   * 
   * UT_XML xhtml;
   * xhtml.setListener (xhtml_listener);
   * xhtml.setNameSpace ("xhtml");
   * 
   * UT_XML svg;
   * svg.setListener (svg_listener);
   * svg.setNameSpace ("svg");
   * svg.redirectNameSpace (&xhtml,"xhtml"); // Not yet implemented...
   * svg.parse (buffer);
   * 
   * or vice versa... ?
   */
  void startElement (const XML_Char * name, const XML_Char ** atts);
  void endElement (const XML_Char * name);
  void charData (const XML_Char * buffer, int length);
};

class DefaultReader : public UT_XML::Reader
{
public:
  DefaultReader ();
  virtual ~DefaultReader ();

  virtual bool      openFile (const char * szFilename);
  virtual UT_uint32 readBytes (char * buffer, UT_uint32 length);
  virtual void      closeFile (void);

private:
  FILE * in;
};

#endif
