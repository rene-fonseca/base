/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__XML_PARSER_H
#define _DK_SDU_MIP__BASE_XML__XML_PARSER_H

#include <base/Object.h>
#include <base/io/File.h>
#include <base/xml/SAXNotRecognizedException.h>
#include <base/xml/SAXNotSupportedException.h>
#include <base/xml/SAXParseException.h>
#include <base/xml/Attributes.h>
#include <base/xml/AttributeDecl.h>
#include <base/xml/ElementDecl.h>
#include <base/collection/Map.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Support for reading an XML document using callbacks.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class XMLReader : public Object {
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
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class DTDHandler {
  public:

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
      const Array<String>& enumeration) throw();

    /**
      Receive notification of an element declaration event.
      
      @param name The name of the element.
      @param type The type of the element.
    */
    virtual void elementDecl(
      const String& name,
      ElementDecl::ValueType type) throw();
    
    // internalEntityDecl, externalEntityDecl
    
    /**
      Receive notification of an entity declaration event.
    */
    virtual void entityDecl(
      const String& element,
      const String& name) throw();
    
    /**
      Receive notification of the beginning of an element declaration event.
      
      @param name The notation name.
      @param type The type.
    */
    virtual void startElementDecl(
      const String& name) throw();
    
    /**
      Receive notification of the end of an element declaration event.
      
      @param name The notation name.
      @param type The type.
    */
    virtual void endElementDecl(
      const String& name) throw();
    
    /**
      Receive notification of a notation declaration event.

      @param name The notation name.
      @param publicId The notation's public identifier.
      @param systemId The notation's system identifier.
    */
    virtual void notationDecl(
      const String& name,
      const String& publicId,
      const String& systemId) throw();

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
      const String& notationName) throw();
  };

  /**
    @short Basic interface for resolving entities.
    @ingroup xml
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class EntityResolver {
  public:

    /**
      Allows the application to resolve external entities.
    */
    // virtual InputSource resolveEntity(const String& publicId, const String& systemId) throw(SAXException);
  };

  /**
    @short Basic interface for SAX error handlers.
    @ingroup xml
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class ErrorHandler {
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

  class Locator {
  public:
  };
  
  /**
    @short Basic interface for SAX error handlers.
    @ingroup xml
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class ContentHandler {
  public:

    /**
      Receives notification of the beginning of a document.
      
      The SAX parser will invoke this method only once, before any other
      methods in this interface or in DTDHandler (except for
      setDocumentLocator).
    */
    virtual void startDocument() throw();
    
    /**
      Receives notification of the end of a document.
      
      The SAX parser will invoke this method only once, and it will be the last
      method invoked during the parse. The parser shall not invoke this method
      until it has either abandoned parsing (because of an unrecoverable error)
      or reached the end of input.
    */
    virtual void endDocument() throw();
    
    /**
      Receives notification of the beginning of an element.
    */
    virtual void startElement(
      const String& name,
      const Attributes& attributes) throw();
    
    /**
      Receives notification of the end of an element.
    */
    virtual void endElement(const String& name) throw();

    /**
      Receives notification of a processing instruction.
    */
    virtual void processingInstruction(
      const String& target, const String& data) throw();
    
    /**
      Receives notification of character data.
    */
    virtual void characters(const String& value) throw();
    
    /**
      Receives notification of ignorable whitespace in element content.
    */
    virtual void ignorableWhitespace(const String& value) throw();
    
    /**
      Receives notification of a comment.
    */
    virtual void comment(const String& value) throw();
    
    /**
      Receives notification of a CDATA block.
    */
    virtual void cdataBlock(const String& value) throw();
    
    /**
      Receives notification of a skipped entity.
    */
    virtual void skippedEntity(const String& name) throw();
    
    /**
      Receives notification of an entity reference.
    */
    virtual void entityReference(const String& name) throw();
    
    /**
      End the scope of a prefix-URI mapping.
    */
    virtual void endPrefixMapping(const String& prefix) throw();
    
    /**
      Begin the scope of a prefix-URI Namespace mapping.
    */
    virtual void startPrefixMapping(
      const String& prefix, const String& uri) throw();

    /**
      Receives an object for locating the origin of SAX document events.
    */
    virtual void setDocumentLocator(Locator* locator) throw();
  };
private:
  
  // TAG: need automation pointer for event handlers (weak handle)
  // EventHandler<ErrorHandler> errorHandler;
  /** The registered content handler. */
  ContentHandler* contentHandler;
  /** The registered DTD handler. */
  DTDHandler* dtdHandler;
  /** The registered entity resolver. */
  EntityResolver* entityResolver;
  /** The registered error handler. */
  ErrorHandler* errorHandler;
  /** Specifies whether or not to validate the document. */
  bool validation;
  /** Recovery flag. */
  bool recovery;
  /** Specifies whether or not the parsing should be terminated. */
  bool terminated;
public:
  
  /**
    Initializes the XML reader.
  */
  XMLReader() throw();
  
  /**
    Returns the value of the specified feature.
  */
  bool getFeature(const String& name) const
    throw(SAXNotRecognizedException, SAXNotSupportedException);
  
  /**
    Sets the value of the specified feature.
  */
  void setFeature(const String& name, bool value)
    throw(SAXNotRecognizedException, SAXNotSupportedException);
  
  /**
    Returns the current content handler.
  */
  inline ContentHandler* getContentHandler() const throw() {
    return contentHandler;
  }
  
  /**
    Returns the current DTD handler.
  */
  inline DTDHandler* getDTDHandler() const throw() {
    return dtdHandler;
  }
  
  /**
    Returns the current entity resolver.
  */
  inline EntityResolver* getEntityResolver() const throw() {
    return entityResolver;
  }
  
  /**
    Returns the current error handler.
  */
  ErrorHandler* getErrorHandler() const throw() {
    return errorHandler;
  }
  
  /**
    Parses the specified file.
    
    @param file The file to be parsed.
    @param uri The uri used to resolve entities. Default is improper string.
  */
  void parse(
    File file, const String& uri = String()) throw(IOException, SAXException);
  
  /**
    Parses the specified buffer.
    
    @param buffer The buffer to be parsed.
    @param size The size of the buffer.
    @param uri The uri used to resolve entities. Default is improper string.
  */
  void parse(
    const char* buffer,
    unsigned int size,
    const String& uri) throw(SAXException);  
  
  /**
    Parses an XML document from a system identifier (URI).
    
    @param systemId The identifier of the document.
  */
  void parse(const String& systemId) throw(SAXException);
  
  /**
    Registers a content event handler.
  */
  inline void setContentHandler(ContentHandler* handler) throw() {
    contentHandler = handler;
  }
  
  /**
    Registers a DTD event handler.
  */
  inline void setDTDHandler(DTDHandler* handler) throw() {
    dtdHandler = handler;
  }
  
  /**
    Registers an entity resolver.
  */
  inline void setEntityResolver(EntityResolver* entityResolver) throw() {
    entityResolver = entityResolver;
  }
  
  /**
    Registers an error event handler.
  */
  inline void setErrorHandler(ErrorHandler* handler) throw() {
    errorHandler = handler;
  }

  /**
    Returns the validation flag.
  */
  inline bool getValidation() const throw() {
    return validation;
  }

  /**
    Sets the validation flag.
  */
  inline void setVatidation(bool value) throw() {
    validation = value;
  }
  
  /**
    Terminates the parsing.
  */
  inline void terminate() throw() {
    terminated = true;
  }
  
  /**
    Destroys the XML parser.
  */
  ~XMLReader() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
