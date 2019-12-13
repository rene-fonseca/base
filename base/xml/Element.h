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

#include <base/xml/Node.h>
#include <base/xml/Attribute.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Document;

/**
  Representation of an element in an XML document.
  
  @short DOM element node.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Element : public Node {
  friend class Document;
protected:
  
  /**
    Internal constructor.
  */
  inline Element(void* context) noexcept : Node(context) {
  }
public:
  
  /**
    Initializes element from node.
  */
  inline Element(const Node& node) : Node(node) {
    bassert(
      node.isInvalid() || (node.getType() == ELEMENT_NODE),
      CastException(this)
    );
  }
  
  /**
    Initializes element.
  */
  inline Element(const Node::ShadowElement& element) noexcept
    : Node(element.getNode()) {
  }
  
  /**
    Returns the target.
  */
  String getTagName() const noexcept;
  
  /**
    Returns the first attribute node of this element.
  */
  Attribute getFirstAttribute() noexcept;
  
  /**
    Retrieves an attribute value by name.
  */
  inline String getAttribute(const String& name) const {
    return getAttribute(name.native());
  }

  String getAttribute(const char* name) const;
  
  /**
    Adds a new attribute.
  */
  inline void setAttribute(
    const String& name, const String& value) {
    setAttribute(name.native(), value.native());
  }

  void setAttribute(
    const char* name, const char* value);
  
  /**
    Removes an attribute by name.
  */
  inline void removeAttribute(const String& name) {
    removeAttribute(name.native());
  }

  void removeAttribute(const char* name);
  
  /**
    Retrieves an attribute node by name.
  */
  inline Attribute getAttributeNode(const String& name) {
    return getAttributeNode(name.native());
  }

  Attribute getAttributeNode(const char* name);
  
  /**
    Adds a new attribute node.
  */
  Attribute setAttributeNode(Attribute attribute);
  
  /**
    Removes the specified attribute node.
  */
  Attribute removeAttributeNode(Attribute attribute);
  
  /**
    Retrieves an Attribute node by local name and namespace URI.
  */
  inline Attribute getAttributeNodeNS(
    const String& namespaceURI,
    const String& localName) {
    return getAttributeNodeNS(namespaceURI.native(), localName.native());
  }

  Attribute getAttributeNodeNS(
    const char* namespaceURI,
    const char* localName);
  
  /**
    Retrieves an attribute value by local name and namespace URI.
  */
  inline String getAttributeNS(
    const String& namespaceURI, const String& localName) {
    return getAttributeNS(namespaceURI.native(), localName.native());
  }

  String getAttributeNS(
    const char* namespaceURI, const char* localName);

  /**
    Adds a new attribute.
  */
  inline void setAttributeNS(
    const String& namespaceURI,
    const String& qualifiedName,
    const String& value) {
    setAttributeNS(namespaceURI.native(), qualifiedName.native(), value.native());
  }

  void setAttributeNS(
    const char* namespaceURI,
    const char* qualifiedName,
    const char* value);
  
  /**
    Removes an attribute by local name and namespace URI.
  */
  inline void removeAttributeNS(
    const String& namespaceURI,
    const String& localName) {
    removeAttributeNS(namespaceURI.native(), localName.native());
  }

  void removeAttributeNS(
    const char* namespaceURI,
    const char* localName);
  
  /**
    Adds a new attribute.
  */
  Attribute setAttributeNodeNS(Attribute attribute);

  /**
    Returns true if the element has any attributes.
  */
  bool hasAttributes() const noexcept;
  
  bool hasAttribute(const char* name) const noexcept;

  /**
    Returns true when an attribute with a given name is specified on this
    element or has a default value, false otherwise.
  */
  inline bool hasAttribute(const String& name) const noexcept {
    return hasAttribute(name.native());
  }

  bool hasAttributeNS(
    const char* namespaceURI, const char* localName) const noexcept;

  /**
    Returns true when an attribute with a given local name and namespace URI is
    specified on this element or has a default value, false otherwise.
  */
  inline bool hasAttributeNS(
    const String& namespaceURI, const String& localName) const noexcept {
    return hasAttributeNS(namespaceURI.native(), localName.native());
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
