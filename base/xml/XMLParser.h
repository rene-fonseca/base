/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

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
  XML parser
  @author René Møller Fonseca
*/

class XMLParser : public Object {
friend class XMLParserImpl;
public:

  /**
    @short Call back interface.
  */
  class Callback : public virtual Object {
  public:

    virtual void startDocument() {}
    virtual void endDocument() {}
    virtual void startElement(const String& n, const XMLPropertyHash& p) {}
    virtual void endElement(const String& n) {}
    virtual void characters(const String& s) {}
    virtual void comment(const String& s) {}
    virtual void warning(const String& s) {}
    virtual void error(const String& s) {}
    virtual void fatalError(const String& s) {}
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
