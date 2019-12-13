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

#include <base/Polymorphic.h>
#include <base/xml/SAXNotRecognizedException.h>
#include <base/xml/SAXNotSupportedException.h>
#include <base/xml/SAXParseException.h>
#include <base/xml/Attributes.h>
#include <base/xml/AttributeDecl.h>
#include <base/xml/ElementDecl.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @short Support for reading an XML document using callbacks.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API XMLReader : public Polymorphic {
  friend class XMLReaderImpl;
public:
  
  /**
    If a SAX application needs information about notations and unparsed
    entities, then the application implements this interface and registers an
    instance with the SAX parser using the parser's setDTDHandler method. The
    parser uses the instance to report notation and unparsed entity
    declarations to the application.
    
    @short Receive notification of basic DTD-related events.
    @ingroup xml
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API DTDHandler {
  public:

    /**
      Receives notification of a comment.
    */
    virtual void comment(const String& data) noexcept;
    
    /**
      Receive notification of a document type declaration.
      
      @param name The name of the attribute.
      @param publicId The public identifier.
      @param systemId The system identifier.
    */
    virtual void startDTD(
      const String& name,
      const String& publicId,
      const String& systemId) noexcept;
    
    /**
      Receive notification of end of document type declaration.
    */
    virtual void endDTD() noexcept;

    /**
      Receives notification of the start of an entity.
    */
    virtual void startEntity(const String& name) noexcept;

    /**
      Receives notification of the end of an entity.
    */
    virtual void endEntity(const String& name) noexcept;
    
    /**
      Receives notification of the start of a CDATA section.
    */
    virtual void startCDATA() noexcept;
    
    /**
      Receives notification of the end of a CDATA section.
    */
    virtual void endCDATA() noexcept;
    
    /**
      Receive notification of an attribute declaration event.
      
      @param name The name of the attribute.
      @param element The element name.
      @param type The element name.
      @param defaultType The default value constraint.
      @param defaultValue The default value.
      @param enumeration The enumeration names if type is
      ATTRIBUTE_ENUMERATION.
    */
    virtual void attributeDecl(
      const String& name,
      const String& element,
      AttributeDecl::ValueType type,
      AttributeDecl::DefaultType defaultType,
      const String& defaultValue,
      const Array<String>& enumeration) noexcept;

    /**
      Receive notification of an element declaration event.
      
      @param name The name of the element.
      @param type The type of the element.
    */
    virtual void elementDecl(
      const String& name,
      ElementDecl::ValueType type) noexcept;
    
    /**
      Receive notification of an internal entity declaration event.
      
      @param name The name of the entity.
      @param text The text.
    */
    virtual void internalEntityDecl(
      const String& name,
      const String& text) noexcept;
    
    /**
      Receive notification of an external entity declaration event.
      
      @param name The name of the entity.
      @param publicId The entities public identifier.
      @param systemId The entities system identifier.
    */
    virtual void externalEntityDecl(
      const String& name,
      const String& publicId,
      const String& systemId) noexcept;
    
//     /**
//       Receive notification of the beginning of an element declaration event.
      
//       @param name The notation name.
//       @param type The type.
//     */
//     virtual void startElementDecl(
//       const String& name) noexcept;
    
//     /**
//       Receive notification of the end of an element declaration event.
      
//       @param name The notation name.
//       @param type The type.
//     */
//     virtual void endElementDecl(
//       const String& name) noexcept;
    
    /**
      Receive notification of a notation declaration event.

      @param name The notation name.
      @param publicId The notation's public identifier.
      @param systemId The notation's system identifier.
    */
    virtual void notationDecl(
      const String& name,
      const String& publicId,
      const String& systemId) noexcept;

    /**
      Receive notification of an unparsed entity declaration event.
      
      @param name The unparsed entity's name.
      @param publicId The entity's public identifier.
      @param systemId The entity's system identifier.
      @param notation The name of the associated notation.
    */
    virtual void unparsedEntityDecl(
      const String& name,
      const String& publicId,
      const String& systemId,
      const String& notationName) noexcept;
  };

  class _COM_AZURE_DEV__BASE__API InputSource {
  private:

    void* context = nullptr;
  public:
    
    inline InputSource() noexcept {
    }

    inline InputSource(void* context) noexcept {
    }
  };
  
  /**
    @short Basic interface for resolving entities.
    @ingroup xml
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API EntityResolver {
  public:

    /**
      Allows the application to resolve external entities.
    */
    virtual InputSource resolveEntity(
      const String& publicId, const String& systemId) throw(SAXException);
  };

  /**
    @short Basic interface for SAX error handlers.
    @ingroup xml
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API ErrorHandler {
  public:

    /**
      Receives notification of a recoverable error.
    */
    virtual void error(const SAXParseException& exception) throw(SAXException);

    /**
      Receives notification of a non-recoverable error.
    */
    virtual void fatalError(
      const SAXParseException& exception) throw(SAXException);

    /**
      Receives notification of a warning.
    */
    virtual void warning(
      const SAXParseException& exception) throw(SAXException);
  };

  class _COM_AZURE_DEV__BASE__API Locator {
  public:
  };
  
  /**
    @short Basic interface for SAX error handlers.
    @ingroup xml
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API ContentHandler {
  public:

    /**
      Receives notification of the beginning of a document.
      
      The SAX parser will invoke this method only once, before any other
      methods in this interface or in DTDHandler (except for
      setDocumentLocator).
    */
    virtual void startDocument() noexcept;
    
    /**
      Receives notification of the end of a document.
      
      The SAX parser will invoke this method only once, and it will be the last
      method invoked during the parse. The parser shall not invoke this method
      until it has either abandoned parsing (because of an unrecoverable error)
      or reached the end of input.
    */
    virtual void endDocument() noexcept;
    
    /**
      Receives notification of the beginning of an element.
    */
    virtual void startElement(
      const String& namespaceURI,
      const String& localName,
      const String& qName,
      const Attributes& attributes) noexcept;
    
    /**
      Receives notification of the end of an element.
    */
    virtual void endElement(
      const String& namespaceURI,
      const String& localName,
      const String& qName) noexcept;
    
    /**
      Receives notification of a processing instruction.
    */
    virtual void processingInstruction(
      const String& target, const String& data) noexcept;
    
    /**
      Receives notification of character data.
    */
    virtual void characters(const String& value) noexcept;
    
    /**
      Receives notification of ignorable whitespace in element content.
    */
    virtual void ignorableWhitespace(const String& value) noexcept;
    
    /**
      Receives notification of a comment.
    */
    virtual void comment(const String& value) noexcept;
    
    /**
      Receives notification of a CDATA block.
    */
    virtual void cdataBlock(const String& value) noexcept;
    
    /**
      Receives notification of a skipped entity.
    */
    virtual void skippedEntity(const String& name) noexcept;
    
    /**
      Receives notification of an entity reference.
    */
    virtual void entityReference(const String& name) noexcept;
    
    /**
      End the scope of a prefix-URI mapping.
    */
    virtual void endPrefixMapping(const String& prefix) noexcept;
    
    /**
      Begin the scope of a prefix-URI Namespace mapping.
    */
    virtual void startPrefixMapping(
      const String& prefix, const String& uri) noexcept;

    /**
      Receives an object for locating the origin of SAX document events.
    */
    virtual void setDocumentLocator(Locator* locator) noexcept;
  };
public:
  
  /**
    Returns the value of the specified feature.
  */
  virtual bool getFeature(const String& name) const
    throw(SAXNotRecognizedException, SAXNotSupportedException) = 0;
  
  /**
    Sets the value of the specified feature.
  */
  virtual void setFeature(const String& name, bool value)
    throw(SAXNotRecognizedException, SAXNotSupportedException) = 0;
  
  /**
    Returns the current content handler.
  */
  virtual ContentHandler* getContentHandler() const noexcept = 0;
  
  /**
    Returns the current DTD handler.
  */
  virtual DTDHandler* getDTDHandler() const noexcept = 0;
  
  /**
    Returns the current entity resolver.
  */
  virtual EntityResolver* getEntityResolver() const noexcept = 0;
  
  /**
    Returns the current error handler.
  */
  virtual ErrorHandler* getErrorHandler() const noexcept = 0;
  
  /**
    Registers a content event handler.
  */
  virtual void setContentHandler(ContentHandler* handler) noexcept = 0;
  
  /**
    Registers a DTD event handler.
  */
  virtual void setDTDHandler(DTDHandler* handler) noexcept = 0;
  
  /**
    Registers an entity resolver.
  */
  virtual void setEntityResolver(EntityResolver* entityResolver) noexcept = 0;
  
  /**
    Registers an error event handler.
  */
  virtual void setErrorHandler(ErrorHandler* handler) noexcept = 0;

  /**
    Returns the validation flag.
  */
  virtual bool getValidation() const noexcept = 0;

  /**
    Sets the validation flag.
  */
  virtual void setVatidation(bool validate) noexcept = 0;

  /**
    Returns true if the document is a standalone document.
  */
  virtual bool isStandalone() const noexcept = 0;
  
  /**
    Terminates the parsing.
  */
  virtual void terminate() noexcept = 0;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
