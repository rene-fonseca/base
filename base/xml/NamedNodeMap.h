/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/xml/Node.h>
#include <base/xml/DOMException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Node;
class DocumentType;

class ShadowNode {
  friend class Node;
private:

  void* context;
  
  inline ShadowNode(void* _context) throw() : context(_context) {
  }
public:
};

/**
  DOM named node map.
  
  @short DOM named node map.
  @ingroup xml
  @version 1.0
*/

class NamedNodeMap : public Object {
  friend class DocumentType;
private:
  
  void* context;
  
  /**
    Initializes the node map.
  */
  inline NamedNodeMap(void* _context) throw() : context(_context) {
  }
public:
  
  /**
    Initializes node map from node map.
  */
  inline NamedNodeMap(const NamedNodeMap& copy) throw()
    : context(copy.context) {
  }
  
  /**
    Assignment of node map by node map.
  */
  inline NamedNodeMap& operator=(const NamedNodeMap& eq) throw() {
    context = eq.context;
    return *this;
  }
  
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
