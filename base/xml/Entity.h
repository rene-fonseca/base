/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__ENTITY_H
#define _DK_SDU_MIP__BASE_XML__ENTITY_H

#include <base/xml/Node.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  This class represents an entity, either parsed or unparsed, in an XML
  document. Note that this models the entity itself not the entity declaration.
  Entity declaration modeling has been left for a later level of the DOM
  specification.
  
  @short DOM entity node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Entity : public Node {
  friend class Document;
protected:
  
  inline Entity(void* context) throw() : Node(context) {
  }
public:
  
  inline Entity(Node node) throw(CastException) : Node(node) {
    assert(node.getType() == ENTITY_NODE, CastException(this));
  }
  
  /**
    The public identifier associated with the entity, if specified. If the
    public identifier was not specified, this is an improper string.
  */
  String getPublicId() const throw();
  
  /**
    The system identifier associated with the entity, if specified. If the
    system identifier was not specified, this is an improper string.
  */
  String getPrivateId() const throw();
  
  /**
    For unparsed entities, the name of the notation for the entity. For parsed
    entities, this is an improper string.
  */
  String getNotationName() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
