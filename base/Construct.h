/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_CONSTRUCT_H
#define _DK_SDU_MIP_BASE_CONSTRUCT_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Thrown on object construction error.

  @author René Møller Fonseca
  @version 1.0
*/

class Construct : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  Construct() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Construct(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
