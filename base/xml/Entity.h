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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Document;

/**
  This class represents an entity, either parsed or unparsed, in an XML
  document. Note that this models the entity itself not the entity declaration.
  Entity declaration modeling has been left for a later level of the DOM
  specification.
  
  @short DOM entity node.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Entity : public Node {
  friend class Document;
protected:
  
  inline Entity(void* context) noexcept : Node(context) {
  }
public:
  
  inline Entity(Node node) throw(CastException) : Node(node) {
    bassert(
      node.isInvalid() || (node.getType() == ENTITY_NODE),
      CastException(this)
    );
  }
  
  /**
    The public identifier associated with the entity, if specified. If the
    public identifier was not specified, this is an improper string.
  */
  String getPublicId() const noexcept;
  
  /**
    The system identifier associated with the entity, if specified. If the
    system identifier was not specified, this is an improper string.
  */
  String getSystemId() const noexcept;
  
  /**
    For unparsed entities, the name of the notation for the entity. For parsed
    entities, this is an improper string.
  */
  String getNotationName() const noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
