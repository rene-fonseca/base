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

#include <base/xml/DOMException.h>
#include <base/xml/Node.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Document;
class Element;

/**
  @short XML attribute.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Attribute : public Node {
  friend class Document;
  friend class Element;
protected:
  
  /**
    Initializes attribute.
  */
  inline Attribute(void* context) throw() : Node(context) {
  }
public:
  
  inline Attribute(Node node) throw(CastException) : Node(node) {
    bassert(
      node.isInvalid() || (node.getType() == ATTRIBUTE_NODE),
      CastException(this)
    );
  }
  
  /**
    Returns the name of the attribute.
  */
  String getName() const throw();
  
  /**
    Returns the value of the attribute.
  */
  String getValue() const throw();

  /**
    Sets the value of the attribute.
  */
  void setValue(const String& value) throw(DOMException);

  /**
    Returns true if the attribute value is explicitly specified in the original
    document.
  */
  bool isSpecified() const throw();
  
  /**
    Returns the owner element.
  */
  Node::ShadowElement getOwnerElement() throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
