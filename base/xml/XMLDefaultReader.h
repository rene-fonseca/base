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

#include <base/io/File.h>
#include <base/xml/XMLReader.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class XMLDefaultReaderImpl;

/**
  @short Support for reading an XML document using callbacks.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API XMLDefaultReader : public XMLReader {
  friend class XMLDefaultReaderImpl;
private:
  
  /** Internal parser context. */
  void* context = nullptr;
  /** The registered content handler. */
  ContentHandler* contentHandler = nullptr;
  /** The registered DTD handler. */
  DTDHandler* dtdHandler = nullptr;
  /** The registered entity resolver. */
  EntityResolver* entityResolver = nullptr;
  /** The registered error handler. */
  ErrorHandler* errorHandler = nullptr;
  /** Specifies whether or not to validate the document. */
  bool validate = false;
  /** Recovery flag. */
  bool recover = false;
  /** Specifies whether or not the parsing should be terminated. */
  bool terminated = false;
  /** Specifies whether or not the parsing is in progress. */
  bool parsing = false;
  /** Specifies whether or not the document is standalone. */
  bool standalone = false;
public:
  
  /**
    Initializes the XML reader.
  */
  XMLDefaultReader() noexcept;
  
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
  inline ContentHandler* getContentHandler() const noexcept {
    return contentHandler;
  }
  
  /**
    Returns the current DTD handler.
  */
  inline DTDHandler* getDTDHandler() const noexcept {
    return dtdHandler;
  }
  
  /**
    Returns the current entity resolver.
  */
  inline EntityResolver* getEntityResolver() const noexcept {
    return entityResolver;
  }
  
  /**
    Returns the current error handler.
  */
  ErrorHandler* getErrorHandler() const noexcept {
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
  inline void setContentHandler(ContentHandler* handler) noexcept {
    contentHandler = handler;
  }
  
  /**
    Registers a DTD event handler.
  */
  inline void setDTDHandler(DTDHandler* handler) noexcept {
    dtdHandler = handler;
  }
  
  /**
    Registers an entity resolver.
  */
  inline void setEntityResolver(EntityResolver* _entityResolver) noexcept {
    entityResolver = _entityResolver;
  }
  
  /**
    Registers an error event handler.
  */
  inline void setErrorHandler(ErrorHandler* handler) noexcept {
    errorHandler = handler;
  }

  /**
    Returns the validation flag.
  */
  inline bool getValidation() const noexcept {
    return validate;
  }

  /**
    Sets the validation flag.
  */
  inline void setVatidation(bool validate) noexcept {
    this->validate = validate;
  }

  /**
    Returns true if the document is a standalone document.
  */
  inline bool isStandalone() const noexcept {
    return standalone;
  }
  
  /**
    Terminates the parsing.
  */
  void terminate() noexcept;
  
  /**
    Destroys the XML reader.
  */
  ~XMLDefaultReader() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
