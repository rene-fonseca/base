/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__NAMED_NODE_MAP_H
#define _DK_SDU_MIP__BASE_XML__NAMED_NODE_MAP_H

#include <base/Object.h>
#include <base/xml/Node.h>
#include <base/xml/DOMException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class ShadowNode {
  friend class Node;
private:

  void* context;
public:

  ShadowNode(void* _context) throw() : context(_context) {
  }
};

/**
  DOM named node map.
  
  @short DOM named node map.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class NamedNodeMap : public Object {
public:
  
  ShadowNode getNamedItem(const String& name) throw(DOMException);
  
  ShadowNode setNamedItem(ShadowNode node) throw(DOMException);
  
  ShadowNode removeNamedItem(const String& name) throw(DOMException);
  
  /**
    Returns the number of nodes in the map.
  */
  unsigned int getSize() const throw();
  
  /**
    Returns the node at the specified index.
  */
  ShadowNode getItem(unsigned int index) throw(DOMException);
  
  ShadowNode getNamedItemNS(
    const String& namespaceURI, const String& localName) throw();
  
  ShadowNode setNamedItemNS(ShadowNode node) throw(DOMException);
  
  ShadowNode removeNamedItemNS(
    const String& namespaceURI, const String& localName) throw(DOMException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
