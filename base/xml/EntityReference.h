/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__ENTITY_REFERENCE_H
#define _DK_SDU_MIP__BASE_XML__ENTITY_REFERENCE_H

#include <base/xml/Node.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  EntityReference nodes may be inserted into the structure model when an entity
  reference is in the source document, or when the user wishes to insert an
  entity reference. Note that character references and references to predefined
  entities are considered to be expanded by the HTML or XML processor so that
  characters are represented by their Unicode equivalent rather than by an
  entity reference. Moreover, the XML processor may completely expand
  references to entities while building the structure model, instead of
  providing EntityReference objects. If it does provide such objects, then for
  a given EntityReference node, it may be that there is no Entity node
  representing the referenced entity. If such an Entity exists, then the
  subtree of the EntityReference node is in general a copy of the Entity node
  subtree. However, this may not be true when an entity contains an unbound
  namespace prefix. In such a case, because the namespace prefix resolution
  depends on where the entity reference is, the descendants of the
  EntityReference node may be bound to different namespace URIs.
  
  As for Entity nodes, EntityReference nodes and all their descendants are
  read-only.
  
  @short DOM entity reference node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class EntityReference : public Node {
  friend class Document;
protected:
  
  inline EntityReference(void* context) throw() : Node(context) {
  }
public:
  
  inline EntityReference(Node node) throw(CastException) : Node(node) {
    assert(node.getType() == ENTITY_REFERENCE_NODE, CastException(this));
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
