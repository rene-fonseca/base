/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__XML_DEFAULT_READER_H
#define _DK_SDU_MIP__BASE_XML__XML_DEFAULT_READER_H

#include <base/io/File.h>
#include <base/xml/XMLReader.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class XMLDefaultReaderImpl;

/**
  @short Support for reading an XML document using callbacks.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class XMLDefaultReader : public XMLReader {
  friend class XMLDefaultReaderImpl;
private:
  
  /** Internal parser context. */
  void* context;
  /** The registered content handler. */
  ContentHandler* contentHandler;
  /** The registered DTD handler. */
  DTDHandler* dtdHandler;
  /** The registered entity resolver. */
  EntityResolver* entityResolver;
  /** The registered error handler. */
  ErrorHandler* errorHandler;
  /** Specifies whether or not to validate the document. */
  bool validate;
  /** Recovery flag. */
  bool recover;
  /** Specifies whether or not the parsing should be terminated. */
  bool terminated;
  /** Specifies whether or not the parsing is in progress. */
  bool parsing;
  /** Specifies whether or not the document is standalone. */
  bool standalone;
public:
  
  /**
    Initializes the XML reader.
  */
  XMLDefaultReader() throw();
  
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
    return validate;
  }

  /**
    Sets the validation flag.
  */
  inline void setVatidation(bool validate) throw() {
    this->validate = validate;
  }

  /**
    Returns true if the document is a standalone document.
  */
  inline bool isStandalone() const throw() {
    return standalone;
  }
  
  /**
    Terminates the parsing.
  */
  void terminate() throw();
  
  /**
    Destroys the XML reader.
  */
  ~XMLDefaultReader() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
