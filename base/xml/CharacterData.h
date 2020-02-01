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
  DOM character data node.
  
  @short DOM character data node.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API CharacterData : public Node {
  friend class Document;
protected:

  inline CharacterData(void* context) noexcept
    : Node(context)
  {
  }
public:
  
  inline CharacterData(Node node)
    : Node(node)
  {
    NodeType type = node.getType();
    bassert(
      node.isInvalid() || 
      (type == COMMENT_NODE) ||
      (type == TEXT_NODE) ||
      (type == CDATA_SECTION_NODE),
      CastException(this)
    );
  }
  
  /**
    The character data of the node.
  */
  String getData() const;

  /**
    Sets the data.
  */
  void setData(const String& data);
  
  /**
    The number of octets that are available through data and the substringData
    method below.
  */
  unsigned int getLength() const noexcept;
  
  /**
    Extracts a range of octets from the node.
  */
  String substringData(
    unsigned int offset, unsigned int count);
  
  /**
    Append the string to the end of the character data of the node.
  */
  void appendData(const String& value);

  /**
    Inserts a string at the specified octet offset.
  */
  void insertData(unsigned int offset, const String& value);

  /**
    Remove a range of octets from the node.
  */
  void deleteData(unsigned int offset, unsigned int count);

  /**
    Replace the characters starting at the specified 16-bit unit offset with
    the specified string.
  */
  void replaceData(
    unsigned int offset,
    unsigned int count,
    const String& value);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
