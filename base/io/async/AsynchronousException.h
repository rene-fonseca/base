/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_EXCEPTION_H
#define _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_EXCEPTION_H

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Asynchronous exception.

  @ingroup exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AsynchronousException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  AsynchronousException() throw();

  /**
    Initializes exception object as unspecified.

    @param message The message.
  */
  AsynchronousException(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
