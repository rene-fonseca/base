/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__XML_PARSER_H
#define _DK_SDU_MIP__BASE_XML__XML_PARSER_H

#include <base/Object.h>
#include <base/xml/XMLException.h>
#include <base/xml/XMLNode.h>
#include <base/xml/XMLProperty.h>
#include <base/collection/Map.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

typedef Map<String, XMLProperty> XMLPropertyHash;

/**
  XML parser.
  
  @short XML parser.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class XMLParser : public Object {
  friend class XMLParserImpl;
public:

  /**
    @short XML parser call back interface.
  */
  class Callback {
  public:

    virtual void startDocument() throw() {
    }
    
    virtual void endDocument() throw() {
    }
    
    virtual void startElement(const String& n, const XMLPropertyHash& p) throw() {
    }
    
    virtual void endElement(const String& n) throw() {
    }
    
    virtual void characters(const String& s) throw() {
    }
    
    virtual void comment(const String& s) throw() {
    }
    
    virtual void warning(const String& s) throw() {
    }
    
    virtual void error(const String& s) throw() {
    }
    
    virtual void fatalError(const String& s) throw() {
    }
  };

private:

  /** Holds the callback object. */
  Callback* callback;
  /** Undefined context data (do not manipulate). */
  void* context;
protected:

  /** Prepares the XML parser for a new document. */
  void prepare() throw(XMLException);
  /** Releases resources used by XML parser. */
  void release() throw();
public:

  /**
    Initializes XML parser.

    @param callback The desired callback object.
  */
  XMLParser(Callback* callback) throw(XMLException);

  /**
    Parses the specified string.

    @param value The string to be parsed.
  */
  void parse(const char* value, unsigned int size) throw(XMLException);

  /**
    Terminates the parsing.
  */
  void terminate() throw();

  /**
    Destroys the XML parser.
  */
  ~XMLParser() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
