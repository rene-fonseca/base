/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__PIPE_EXCEPTION_H
#define _DK_SDU_MIP__BASE_IO__PIPE_EXCEPTION_H

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown on pipe object exceptions.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class PipeException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  PipeException() throw() : IOException() {}

  /**
    Initializes the exception object.

    @param message The message.
  */
  PipeException(const char* message) throw() : IOException(message) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
