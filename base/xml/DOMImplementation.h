/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__DOM_IMPLEMENTATION_H
#define _DK_SDU_MIP__BASE_XML__DOM_IMPLEMENTATION_H

#include <base/Object.h>
#include <base/xml/Document.h>
#include <base/xml/DocumentType.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Document Object Model (DOM) implementation.
  
  @short DOM implementation.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class DOMImplementation : public Object {
public:

  enum Flag {
    SUBSTITUTE_ENTITIES = 1,
    DETECT_IDS = 2,
    COMPLETE_ATTRIBUTE_LISTS = 4
  };
  
  enum Mode {
    PARSING,
    VALIDATING,
    RECOVERING
  };
  
  /**
    Initializes DOM implementation.
  */
  inline DOMImplementation() throw() {
  }
  
  inline DOMImplementation(const DOMImplementation& copy) throw() {
  }

  inline DOMImplementation& operator=(const DOMImplementation& eq) throw() {
    return *this;
  }
  
  /**
    Returns true if the feature is available.
  */
  bool hasFeature(const String& name, const String& version) throw();

  /**
    Creates an empty document.
  */
  Document createDocument(const String& version) throw(DOMException);

  /**
    Creates a document from a URI.

    @param systemId The URI.
  */
  Document createFromURI(const String& systemId) throw(DOMException);
  
  /**
    Creates a document.
  */
  Document createDocument(
    const String& namespaceURI,
    const String& qualifiedName,
    const DocumentType& doctype) throw(DOMException);

  /**
    Creates the document from a string.
  */
  Document createDocumentFromString(
    const String& value,
    Mode mode = VALIDATING,
    unsigned int flags = DETECT_IDS|COMPLETE_ATTRIBUTE_LISTS)
    throw(DOMException);
  
  /**
    Saves the document to the specified file.

    @param filename The path of the file.
    @param document The document.
    @param indent Specifies whether or not the document should be indented. The
    default is false.
  */
  void saveDocument(
    Document document,
    const String& filename,
    bool indent = false) throw(DOMException);
  
  /**
    Saves the document to the specified file.

    @param filename The path of the file.
    @param document The document.
    @param encoding The encoding.
    @param indent Specifies whether or not the document should be indented. The
    default is false.
  */
  void saveDocument(
    Document document,
    const String& filename,
    const String& encoding,
    bool indent) throw(DOMException);

  /**
    Saves the document to a string.

    @param document The document.
    @param spaces Specifies whether or not formatting spaces should be added.
    The default is false.
  */
  String saveDocumentToMemory(
    Document document, bool spaces = false) throw(DOMException);

  /**
    Saves the document to a string.

    @param document The document.
    @param encoding The encoding.
    @param spaces Specifies whether or not formatting spaces should be added.
    The default is false.
  */
  String saveDocumentToMemory(
    Document document,
    const String& encoding,
    bool spaces = false) throw(DOMException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
