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

#include <base/xml/DocumentType.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Document;

/**
  @short XML declaration node.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Declaration : public Node {
  friend class Document;
protected:
  
  /**
    Initializes declaration node.
  */
  inline Declaration(void* context) noexcept : Node(context) {
  }
public:
  
  inline Declaration(Node node) : Node(node) {
    NodeType type = node.getType();
    bassert(
      node.isInvalid() ||
      (type == ATTRIBUTE_DECL_NODE) ||
      (type == ELEMENT_DECL_NODE) ||
      (type == ENTITY_DECL_NODE),
      CastException(this)
    );
  }
  
  /**
    Returns the document type node of this declaration.
    TAG: need Declaration node for all declarations
  */
  DocumentType getDocumentType() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
