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

#include <base/xml/CharacterData.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  The Text class inherits from CharacterData and represents the textual content
  (termed character data in XML) of an Element or Attribute. If there is no
  markup inside an element's content, the text is contained in a single
  instance of the Text class that is the only child of the element. If there is
  markup, it is parsed into the information items (elements, comments, etc.)
  and Text nodes that form the list of children of the element.
  
  When a document is first made available via the DOM, there is only one Text
  node for each block of text. Users may create adjacent Text nodes that
  represent the contents of a given element without any intervening markup, but
  should be aware that there is no way to represent the separations between
  these nodes in XML or HTML, so they will not (in general) persist between DOM
  editing sessions. The normalize() method on Node merges any such adjacent
  Text nodes into a single node for each block of text.
  
  @short DOM text node.
  @ingroup xml
  @version 1.0
*/

class Text : public CharacterData {
  friend class Document;
protected:

  inline Text(void* context) throw() : CharacterData(context) {
  }
public:
  
  inline Text(Node node) throw(CastException)
    : CharacterData(node) {
    NodeType type = node.getType();
    bassert(
      node.isInvalid() || (type == TEXT_NODE) || (type == CDATA_SECTION_NODE),
      CastException(this)
    );
  }
  
  inline Text(CharacterData node) throw(CastException)
    : CharacterData(node) {
    NodeType type = node.getType();
    bassert(
      (type == TEXT_NODE) || (type == CDATA_SECTION_NODE),
      CastException(this)
    );
  }
  
  /**
    Breaks this node into two nodes at the specified offset, keeping both in
    the tree as siblings. After being split, this node will contain all the
    content up to the offset point. A new node of the same type, which contains
    all the content at and after the offset point, is returned. If the original
    node had a parent node, the new node is inserted as the next sibling of the
    original node. When the offset is equal to the length of this node, the new
    node has no data.
    
    @param index The octet index (not character index).
  */
  Text splitText(unsigned int index) throw(DOMException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
