/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__CHARACTER_DATA_H
#define _DK_SDU_MIP__BASE_XML__CHARACTER_DATA_H

#include <base/xml/Node.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  DOM character data node.
  
  @short DOM character data node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class CharacterData : public Node {
  friend class Document;
protected:

  inline CharacterData(void* context) throw() : Node(context) {
  }
public:
  
  inline CharacterData(Node node) throw(CastException)
    : Node(node) {
    NodeType type = node.getType();
    assert(
      (type == COMMENT_NODE) ||
      (type == TEXT_NODE) ||
      (type == CDATA_SECTION_NODE),
      CastException(this)
    );
  }
  
  /**
    The character data of the node.
  */
  String getData() const throw(DOMException);

  /**
    Sets the data.
  */
  void setData(const String& data) throw(DOMException);
  
  /**
    The number of octets that are available through data and the substringData
    method below.
  */
  unsigned int getLength() const throw();
  
  /**
    Extracts a range of octets from the node.
  */
  String substringData(
    unsigned int offset, unsigned int count) throw(DOMException);
  
  /**
    Append the string to the end of the character data of the node.
  */
  void appendData(const String& value) throw(DOMException);

  /**
    Inserts a string at the specified octet offset.
  */
  void insertData(
    unsigned int offset, const String& value) throw(DOMException);

  /**
    Remove a range of octets from the node.
  */
  void deleteData(unsigned int offset, unsigned int count) throw(DOMException);

  /**
    Replace the characters starting at the specified 16-bit unit offset with
    the specified string.
  */
  void replaceData(
    unsigned int offset,
    unsigned int count,
    const String& value) throw(DOMException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
