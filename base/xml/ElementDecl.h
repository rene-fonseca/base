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
#include <base/xml/Declaration.h>
#include <base/xml/AttributeDecl.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Document;

/**
  @short XML element declaration node.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ElementDecl : public Declaration {
  friend class Document;
protected:
  
  /**
    Initializes element declaration.
  */
  inline ElementDecl(void* context) noexcept : Declaration(context) {
  }
public:

  /** The value type. */
  enum ValueType {
    EMPTY, /**< Empty. */
    ANY, /**< Any. */
    MIXED, /**< Mixed. */
    ELEMENT /**< Element. */
  };
  
  /** Content. */
  enum Content {
    CONTENT_PCDATA, /**< PCDATA. */
    CONTENT_ELEMENT, /**< Element. */
    CONTENT_SEQUENCE, /**< Sequence. */
    CONTENT_OR /**< Or. */
  };
  
  /** Content occurrences. */
  enum Occurrence {
    ONCE, /**< Content occurring exactly once. */
    OPTIONAL, /**< Optional content. */
    STAR, /**< Any number of occurrences. */
    PLUS /**< One or more. */
  };
  
  inline ElementDecl(Node node)
    : Declaration(node)
  {
    bassert(
      node.isInvalid() || (node.getType() == ELEMENT_DECL_NODE),
      CastException(this)
    );
  }
  
  inline ElementDecl(Declaration node)
    : Declaration(node)
  {
    bassert(
      node.isInvalid() || (node.getType() == ELEMENT_DECL_NODE),
      CastException(this)
    );
  }

  /**
    Returns the type of the element.
  */
  ValueType getValueType() const;

  /**
    Returns true if the element declaration has any attribute declarations.
  */
  bool hasAttributes() const noexcept;
  
  /**
    Returns the first attribute declaration.
  */
  AttributeDecl getFirstAttribute() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
