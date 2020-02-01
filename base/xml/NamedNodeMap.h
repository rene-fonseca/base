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

#include <base/Object.h>
#include <base/xml/Node.h>
#include <base/xml/DOMException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Node;
class DocumentType;

class _COM_AZURE_DEV__BASE__API ShadowNode {
  friend class Node;
private:

  void* context = nullptr;
  
  inline ShadowNode(void* _context) noexcept : context(_context) {
  }
public:
};

/**
  DOM named node map.
  
  @short DOM named node map.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API NamedNodeMap : public Object {
  friend class DocumentType;
private:
  
  void* context = nullptr;
  
  /**
    Initializes the node map.
  */
  inline NamedNodeMap(void* _context) noexcept
    : context(_context)
  {
  }
public:
  
  /**
    Initializes node map from node map.
  */
  inline NamedNodeMap(const NamedNodeMap& copy) noexcept
    : context(copy.context)
  {
  }
  
  /**
    Assignment of node map by node map.
  */
  inline NamedNodeMap& operator=(const NamedNodeMap& assign) noexcept
  {
    context = assign.context;
    return *this;
  }
  
  ShadowNode getNamedItem(const String& name);
  
  ShadowNode setNamedItem(ShadowNode node);
  
  ShadowNode removeNamedItem(const String& name);
  
  /**
    Returns the number of nodes in the map.
  */
  unsigned int getSize() const noexcept;
  
  /**
    Returns the node at the specified index.
  */
  ShadowNode getItem(unsigned int index);
  
  ShadowNode getNamedItemNS(const String& namespaceURI, const String& localName) noexcept;
  
  ShadowNode setNamedItemNS(ShadowNode node);
  
  ShadowNode removeNamedItemNS(const String& namespaceURI, const String& localName);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
