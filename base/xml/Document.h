/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__DOCUMENT_H
#define _DK_SDU_MIP__BASE_XML__DOCUMENT_H

#include <base/Object.h>
#include <base/xml/Attribute.h>
#include <base/xml/CDATASection.h>
#include <base/xml/Comment.h>
#include <base/xml/DocumentFragment.h>
#include <base/xml/DocumentType.h>
#include <base/xml/DOMImplementation.h>
#include <base/xml/Element.h>
#include <base/xml/EntityReference.h>
#include <base/xml/Node.h>
#include <base/xml/ProcessingInstruction.h>
#include <base/xml/Text.h>
#include <base/mem/Reference.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Stylesheet;
class Transformer;
class DOMImplementation;

/**
  DOM document.
  
  @short DOM document.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Document : public Object {
  friend class Stylesheet;
  friend class Transformer;
  friend class DOMImplementation;
private:

  class DocumentImpl : public ReferenceCountedObject {
  private:
    
    /** Context. */
    void* context;
  public:
    
    /**
      Initializes invalid document.
    */
    DocumentImpl() throw();
    
    /**
      Initializes document.
    */
    DocumentImpl(void* context) throw();
    
    /**
      Returns the context.
    */
    inline void* getContext() const throw() {
      return context;
    }
    
    /**
      Destroys the document.
    */
    ~DocumentImpl() throw();
  };
  
  /**
    The document.
  */
  Reference<DocumentImpl> document;
public:

  // sources: File, String, URI, ...
  // static Document parseFile(File file) throw(IOException, DOMException);
  // static Document parseString(const String& document) throw(DOMException);
  
  /**
    Initializes the document as invalid.
  */
  Document() throw(DOMException);

  /**
    Creates a new document with the speicified version.

    @param version The version of XML (e.g. "1.0").
  */
  void create(const String& version) throw(DOMException);

  /**
    The Document Type Declaration associated with this document.
  */
  DocumentType getDocumentType() throw();
  
  /**
    The DOMImplementation object that handles this document.
  */
  DOMImplementation getImplementation() throw();
  
  /**
    This is a convenience attribute that allows direct access to the child node
    that is the root element of the document.
  */
  Element getDocumentElement() throw();
  
  /**
    Creates an Attribute of the given name.
  */
  Attribute createAttribute(const String& name) throw(DOMException);

  /**
    Creates an Attribute of the given qualified name and namespace URI.
  */
  Attribute createAttributeNS(
    const String& namespaceURI,
    const String& qualifiedName) throw(DOMException);

  /**
    Creates an element of the type specified.
  */
  Element createElement(const String& name) throw(DOMException);
  
  /**
    Creates an element of the given qualified name and namespace URI.
  */
  Element createElementNS(
    const String& namespaceURI,
    const String& qualifiedName) throw(DOMException);

  /**
    Creates an empty DocumentFragment node.
  */
  DocumentFragment createDocumentFragment() throw(DOMException);

  /**
    Creates a Text node given the specified string.
  */
  Text createText(const String& data) throw(DOMException);

  /**
    Creates a Comment node given the specified string.
  */
  Comment createComment(const String& data) throw(DOMException);
  
  /**
    Creates a CDATASection node whose value is the specified string.
  */
  CDATASection createCDATASection(const String& data) throw(DOMException);

  /**
    Creates a ProcessingInstruction node given the specified target and data
    strings.
  */
  ProcessingInstruction createProcessingInstruction(
    const String& target,
    const String& data) throw(DOMException);  
  
  /**
    Creates an EntityReference node.
  */
  EntityReference createEntityReference(
    const String& name) throw(DOMException);
  
  /**
    Returns the Element whose ID is given by elementId.
  */
  Element getElementById(const String& elementId) throw();
  
  /**
    Creates a new attribute.
  */
  Attribute createAttribute(
    const String& name, const String& value) throw(DOMException);

  /**
    Imports a node from another document to this document. The returned node has
    no parent. The source node is not altered or removed from the original
    document; this method creates a new copy of the source node.
  */
  Node importNode(Node importedNode, bool deep) throw(DOMException);
  
  /**
    Initializes the document from a file.

    @param filename The name of the file.
  */
  Document(const String& filename) throw(DOMException);
  
  /**
    Initializes document from other document.
  */
  inline Document(const Document& copy) throw()
    : document(copy.document) {
  }
  
  /**
    Assignment of document by document.
  */
  inline Document& operator=(const Document& eq) throw() {
    document = eq.document;
    return *this;
  }
  
  /**
    XInclude process.
  */
  void doXIncludeProcess() throw(); // FIXME
  
  /**
    Save to document to the specified file.
    
    @param filename The name of the file.
  */
  void save(const String& filename) throw(IOException);

  // bool isStandalone() const throw();
  // String getVersion() const throw();
  // String getEncoding() const throw();
  // String getURI() const throw();
  
  // String getDocumentType() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
