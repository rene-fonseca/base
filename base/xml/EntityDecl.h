/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__ENTITY_DECL_H
#define _DK_SDU_MIP__BASE_XML__ENTITY_DECL_H

#include <base/xml/Declaration.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  @short XML entity declaration node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class EntityDecl : public Declaration {
  friend class Document;
protected:
  
  /**
    Initializes entity declaration.
  */
  inline EntityDecl(void* context) throw() : Declaration(context) {
  }
public:
  
  inline EntityDecl(Node node) throw(CastException) : Declaration(node) {
    assert(
      node.isInvalid() || (node.getType() == ENTITY_DECL_NODE),
      CastException(this)
    );
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
