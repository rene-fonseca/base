/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Document Object Model (DOM) implementation.
  
  @short DOM implementation.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API DOMImplementation : public Object {
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

  class _COM_AZURE_DEV__BASE__API Interface {
  };

  class _COM_AZURE_DEV__BASE__API EventListener : public Interface {
  public:
  };
  
  class _COM_AZURE_DEV__BASE__API DocumentEvent : public Interface {
  public:

    /**
      Create a new event object of the specified type.
    */
    virtual DOMEvent createEvent(const String& type) = 0;
  };

  class _COM_AZURE_DEV__BASE__API EventException : public Exception {
  public:
    
    _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
  };
  
  class _COM_AZURE_DEV__BASE__API EventTarget : public Interface {
  public:
    
    virtual void addEventListener(const String& type,
                                  EventListener* listener,
                                  bool useCapture) noexcept = 0;
    
    virtual void removeEventListener(const String& type,
                                     EventListener* listener,
                                     bool useCapture) noexcept = 0;
    
    virtual bool dispatchEvent(
      const DOMEvent& event) = 0;
  };
  
  /**
    Initializes DOM implementation.
  */
  inline DOMImplementation() noexcept {
  }
  
  inline DOMImplementation(const DOMImplementation& copy) noexcept {
  }

  inline DOMImplementation& operator=(const DOMImplementation& assign) noexcept {
    return *this;
  }
  
  /**
    Returns true if the feature is available.
  */
  bool hasFeature(const String& name, const String& version) noexcept;

  /**
    Creates an enmpty DOM Document object.
    
    @param version The XML version. The default is "1.0".
  */
  Document createDocument(
    const String& version = "1.0");
  
  /**
    Creates a DOM Document object of the specified type.
    
    @param doctype The document type.
    @param version The XML version. The default is "1.0".
  */
  Document createDocument(
    DocumentType doctype,
    const String& version = "1.0");

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
    const String& version = "1.0");
  
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
   ;

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
   ;

  /**
    Creates an empty DocumentType node.
  */
  DocumentType createDocumentType(
    const String& qualifiedName,
    const String& publicId,
    const String& systemId);
  
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
    bool indent = false);
  
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
    bool indent);

  /**
    Saves the document to a string.

    @param document The document.
    @param spaces Specifies whether or not formatting spaces should be added.
    The default is false.
  */
  String saveDocumentToMemory(
    Document document, bool spaces = false);

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
    bool spaces = false);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
