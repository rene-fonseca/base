/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__NOTATION_H
#define _DK_SDU_MIP__BASE_XML__NOTATION_H

#include <base/xml/Node.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  This class represents a notation declared in the DTD. A notation either
  declares, by name, the format of an unparsed entity (see section 4.7 of the
  XML 1.0 specification [XML]), or is used for formal declaration of processing
  instruction targets (see section 2.6 of the XML 1.0 specification [XML]). The
  getName() method inherited from Node returns the declared name of the
  notation.
  
  A Notation node does not have any parent.
  
  @short DOM notation node.
  @ingroup xml
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Notation : public Node {
  friend class Document;
protected:
  
  inline Notation(void* context) throw() : Node(context) {
  }
public:
  
  inline Notation(Node node) throw(CastException) : Node(node) {
    assert(node.getType() == NOTATION_NODE, CastException(this));
  }
  
  /**
    Returns the public identifier.
  */
  String getPublicId() const throw();
  
  /**
    Returns the private identifier.
  */
  String getPrivateId() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
