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
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Notation : public Node { // TAG: should be Declaration
  friend class Document;
protected:
  
  inline Notation(void* context) throw() : Node(context) {
  }
public:
  
  inline Notation(Node node) throw(CastException) : Node(node) {
    bassert(
      node.isInvalid() || (node.getType() == NOTATION_NODE),
      CastException(this)
    );
  }
  
  /**
    Returns the public identifier.
  */
  String getPublicId() const throw();
  
  /**
    Returns the system identifier.
  */
  String getSystemId() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
