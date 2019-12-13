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

#include <base/xml/Declaration.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Document;

/**
  @short XML entity declaration node.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API EntityDecl : public Declaration {
  friend class Document;
protected:
  
  /**
    Initializes entity declaration.
  */
  inline EntityDecl(void* context) noexcept : Declaration(context) {
  }
public:
  
  inline EntityDecl(Node node) : Declaration(node) {
    bassert(
      node.isInvalid() || (node.getType() == ENTITY_DECL_NODE),
      CastException(this)
    );
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
