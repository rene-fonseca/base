/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/xml/Document.h>
#include <base/xml/DocumentType.h>
#include <base/xml/DOMEvent.h>
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
    WARNINGS = 1,
    PEDANTIC = 2,
    SUBSTITUTE_ENTITIES = 4,
    DETECT_IDS = 8,
    COMPLETE_ATTRIBUTE_LISTS = 16
  };
  
  enum Mode {
    PARSING, /**< Parsing without validation. */
    VALIDATING, /**< Parsing with validation. */
    RECOVERING /**< Recovering mode. */
  };

  class Interface {
  };

  class EventListener : public Interface {
  public:
  };
  
  class DocumentEvent : public Interface {
  public:

    /**
      Create a new event object of the specified type.
    */
    virtual DOMEvent createEvent(const String& type) throw(DOMException) = 0;
  };

  class EventException : public Exception {
  public:
  };
  
  class EventTarget : public Interface {
  public:
    
    virtual void addEventListener(const String& type,
                                  EventListener* listener,
                                  bool useCapture) throw() = 0;
    
    virtual void removeEventListener(const String& type,
                                     EventListener* listener,
                                     bool useCapture) throw() = 0;
    
    virtual bool dispatchEvent(
      const DOMEvent& event) throw(EventException) = 0;
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
    Creates an enmpty DOM Document object.
    
    @param version The XML version. The default is "1.0".
  */
  Document createDocument(
    const String& version = "1.0") throw(DOMException);
  
  /**
    Creates a DOM Document object of the specified type.
    
    @param doctype The document type.
    @param version The XML version. The default is "1.0".
  */
  Document createDocument(
    DocumentType doctype,
    const String& version = "1.0") throw(DOMException);

  /**
    Creates a DOM Document object of the specified type with its document
    element.
    
    @param namespaceURI The namespace. May be improper.
    @param qualifiedName The qualified name of the document element.
    @param doctype The document type.
    @param version The XML version. The default is "1.0".
  */
  Document createDocument(
    const String& namespaceURI,
    const String& qualifiedName,
    DocumentType doctype,
    const String& version = "1.0") throw(DOMException);
  
  /**
    Creates a document from a URI.

    @param systemId The URI.
    @param mode The mode.
    @param flags The flags.
  */
  Document createFromURI(
    const String& systemId,
    Mode mode = VALIDATING,
    unsigned int flags = WARNINGS|PEDANTIC|DETECT_IDS|COMPLETE_ATTRIBUTE_LISTS)
    throw(DOMException);  

  /**
    Creates the document from a string.

    @param value The string.
    @param mode The mode.
    @param flags The flags.
  */
  Document createDocumentFromString(
    const String& value,
    Mode mode = VALIDATING,
    unsigned int flags = WARNINGS|PEDANTIC|DETECT_IDS|COMPLETE_ATTRIBUTE_LISTS)
    throw(DOMException);

  /**
    Creates an empty DocumentType node.
  */
  DocumentType createDocumentType(
    const String& qualifiedName,
    const String& publicId,
    const String& systemId) throw(DOMException);
  
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
