/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__INVALID_NODE_H
#define _DK_SDU_MIP__BASE_COLLECTION__INVALID_NODE_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception specifies that a node of a collection is invalid.

  @author René Møller Fonseca
  @version 1.0
*/

class InvalidNode : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  InvalidNode();

  /**
    Initializes the exception object.

    @param message The message.
  */
  InvalidNode(const char* message);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
