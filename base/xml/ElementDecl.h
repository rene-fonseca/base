/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__ELEMENT_DECL_H
#define _DK_SDU_MIP__BASE_XML__ELEMENT_DECL_H

#include <base/xml/DocumentType.h>
#include <base/xml/Declaration.h>
#include <base/xml/AttributeDecl.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  @short XML element declaration node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ElementDecl : public Declaration {
  friend class Document;
protected:
  
  /**
    Initializes element declaration.
  */
  inline ElementDecl(void* context) throw() : Declaration(context) {
  }
public:

  /** The value type. */
  enum ValueType {
    EMPTY, /**< Empty. */
    ANY, /**< Any. */
    MIXED, /**< Mixed. */
    ELEMENT /**< Element. */
  };
  
  inline ElementDecl(Node node) throw(CastException) : Declaration(node) {
    assert(
      node.isInvalid() || (node.getType() == ELEMENT_DECL_NODE),
      CastException(this)
    );
  }
  
  inline ElementDecl(Declaration node) throw(CastException)
    : Declaration(node) {
    assert(
      node.isInvalid() || (node.getType() == ELEMENT_DECL_NODE),
      CastException(this)
    );
  }

  /**
    Returns the type of the element.
  */
  ValueType getValueType() const throw();

  /**
    Returns true if the element declaration has any attribute declarations.
  */
  bool hasAttributes() const throw();
  
  /**
    Returns the first attribute declaration.
  */
  AttributeDecl getFirstAttribute() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif