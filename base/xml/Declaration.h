/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__DECLARATION_H
#define _DK_SDU_MIP__BASE_XML__DECLARATION_H

#include <base/xml/DocumentType.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  @short XML declaration node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Declaration : public Node {
  friend class Document;
protected:
  
  /**
    Initializes declaration node.
  */
  inline Declaration(void* context) throw() : Node(context) {
  }
public:
  
  inline Declaration(Node node) throw(CastException) : Node(node) {
    NodeType type = node.getType();
    assert(
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
  DocumentType getDocumentType() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
