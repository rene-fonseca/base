/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__ELEMENT_H
#define _DK_SDU_MIP__BASE_XML__ELEMENT_H

#include <base/xml/Node.h>
#include <base/xml/Attribute.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  Representation of an element in an XML document.
  
  @short DOM element node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Element : public Node {
  friend class Document;
protected:
  
  /**
    Internal constructor.
  */
  inline Element(void* context) throw() : Node(context) {
  }
public:
  
  /**
    Initializes element from node.
  */
  inline Element(const Node& node) throw(CastException) : Node(node) {
    assert(node.getType() == ELEMENT_NODE, CastException(this));
  }
  
  /**
    Initializes element.
  */
  inline Element(const Node::ShadowElement& element) throw()
    : Node(element.getNode()) {
  }
  
  /**
    Returns the target.
  */
  String getTagName() const throw();
  
  /**
    Retrieves an attribute value by name.
  */
  String getAttribute(const String& name) const throw(DOMException);
  
  /**
    Adds a new attribute.
  */
  void setAttribute(
    const String& name, const String& value) throw(DOMException);
  
  /**
    Removes an attribute by name.
  */
  void removeAttribute(const String& name) throw(DOMException);
  
  /**
    Retrieves an attribute node by name.
  */
  Attribute getAttributeNode(const String& name) throw(DOMException);
  
  /**
    Adds a new attribute node.
  */
  Attribute setAttributeNode(Attribute attribute) throw(DOMException);
  
  /**
    Removes the specified attribute node.
  */
  Attribute removeAttributeNode(Attribute attribute) throw(DOMException);
  
  // NodeList getElementsByTagName(const String& name) throw();

  // NodeList getElementsByTagNameNS(
  //   const String& namespaceURI,
  //   const String& localName) throw();
  
  /**
    Retrieves an Attribute node by local name and namespace URI.
  */
  Attribute getAttributeNodeNS(
    const String& namespaceURI,
    const String& localName) throw(DOMException);
  
  /**
    Retrieves an attribute value by local name and namespace URI.
  */
  String getAttributeNS(
    const String& namespaceURI, const String& localName) throw(DOMException);
  
  /**
    Adds a new attribute.
  */
  void setAttributeNS(
    const String& namespaceURI,
    const String& qualifiedName,
    const String& value) throw(DOMException);
  
  /**
    Removes an attribute by local name and namespace URI.
  */
  void removeAttributeNS(
    const String& namespaceURI,
    const String& localName) throw(DOMException);
  
  /**
    Adds a new attribute.
  */
  Attribute setAttributeNodeNS(Attribute attribute) throw(DOMException);
  
  /**
    Returns true when an attribute with a given name is specified on this
    element or has a default value, false otherwise.
  */
  bool hasAttribute(const String& name) const throw();
  
  /**
    Returns true when an attribute with a given local name and namespace URI is
    specified on this element or has a default value, false otherwise.
  */
  bool hasAttributeNS(
    const String& namespaceURI, const String& localName) const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
