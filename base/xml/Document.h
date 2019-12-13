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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Stylesheet;
class Transformer;
class DOMImplementation;

/**
  DOM document.
  
  @short DOM document.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Document : public Node {
  friend class Stylesheet;
  friend class Transformer;
  friend class DOMImplementation;
protected:
  
  /**
    Internal constructor.
  */
  inline Document(void* context) noexcept : Node(context) {
  }
public:  
  
  /**
    Initializes the document as invalid.
  */
  Document() throw(DOMException);
  
  /**
    Initializes the document as invalid.
  */
  inline Document(ShadowDocument document) noexcept : Node(document.getNode()) {
  }
  
  /**
    The Document Type Declaration associated with this document.
  */
  DocumentType getDocumentType() noexcept;
  
  /**
    The DOMImplementation object that handles this document.
  */
  DOMImplementation getImplementation() noexcept;

  /**
    Creates and sets a document type object for this document.
  */
  DocumentType createAndSetDocumentType(
    const String& qualifiedName,
    const String& publicId,
    const String& systemId) throw(DOMException);
  
  /**
    This is a convenience attribute that allows direct access to the child node
    that is the root element of the document.
  */
  Element getDocumentElement() noexcept;
  
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
  Element getElementById(const String& elementId) noexcept;
  
  /**
    Creates a new attribute.
  */
  Attribute createAttribute(
    const String& name, const String& value) throw(DOMException);

  /**
    Imports a node from another document to this document. The returned node
    has no parent. The source node is not altered or removed from the original
    document; this method creates a new copy of the source node.
  */
  Node importNode(Node importedNode, bool deep) throw(DOMException);
  
  /**
    Do XInclude substitution.
  */
  void doXIncludeSubstitution() throw(DOMException);
  
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

  /**
    Returns true if the document is valid.
  */
  bool validate() const throw(DOMException);
  
  /**
    Returns true if the tree is valid.
  */
  bool validate(Node node) const throw(DOMException);
  
  /**
    Destroys the document.
  */
  void destroy() throw(DOMException);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
