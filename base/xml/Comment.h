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
  This class inherits from CharacterData and represents the content of a
  comment, i.e., all the characters between the starting '<!--' and ending
  '-->'. Note that this is the definition of a comment in XML, and, in
  practice, HTML, although some HTML tools may implement the full SGML comment
  structure.
  
  @short DOM comment node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Comment : public CharacterData {
  friend class Document;
protected:
  
  inline Comment(void* context) throw() : CharacterData(context) {
  }
public:
  
  inline Comment(Node node) throw(CastException)
    : CharacterData(node) {
    bassert(
      node.isInvalid() || (node.getType() == COMMENT_NODE),
      CastException(this)
    );
  }
  
  inline Comment(CharacterData node) throw(CastException)
    : CharacterData(node) {
    bassert(
      node.isInvalid() || (node.getType() == COMMENT_NODE),
      CastException(this)
    );
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
