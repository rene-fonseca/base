/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__UNEXPECTED_FAILURE_H
#define _DK_SDU_MIP__BASE__UNEXPECTED_FAILURE_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown on unexpected errors (this is an internal error).

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class UnexpectedFailure : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  UnexpectedFailure() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  UnexpectedFailure(const char* message) throw();

  UnexpectedFailure(Type type) throw() : Exception(type) {}
  
  UnexpectedFailure(const char* message, Type type) throw() : Exception(message, type) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
