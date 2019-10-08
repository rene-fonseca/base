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

#include <base/xml/Node.h>
#include <base/CastException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Document;

/**
  The ProcessingInstruction class represents a "processing instruction", used
  in XML as a way to keep processor-specific information in the text of the
  document.
  
  @short DOM processing instruction node.
  @ingroup xml
  @version 1.0
*/

class ProcessingInstruction : public Node {
  friend class Document;
protected:

  inline ProcessingInstruction(void* context) throw() : Node(context) {
  }
public:

  inline ProcessingInstruction(Node node) throw(CastException) : Node(node) {
    bassert(
      node.isInvalid() || (node.getType() == PROCESSING_INSTRUCTION_NODE),
      CastException(this)
    );
  }
  
  /**
    Returns the target of this processing instruction. XML defines this as
    being the first token following the markup that begins the processing
    instruction.
  */
  String getTarget() const throw();
  
  /**
    Returns the content of this processing instruction. This data from the
    first non white space character after the target to the character
    immediately preceding the ?>.
  */
  String getData() const throw();
  
  /**
    Sets the content of this processing instruction.
  */
  void setData(const String& value) throw(DOMException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
