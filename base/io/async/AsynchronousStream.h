/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_STREAM_H
#define _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_STREAM_H

#include <base/io/async/AsynchronousException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Asynchronous stream.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AsynchronousStream {
public:

  /**
    Inserts an explicit tests for any pending asynchronous operation
    completions. Relinquishes the currently executing thread voluntarily.
  */
  static bool asyncTest() throw();

  /**
    Aborts any pending asynchronous operations (read as well as write) issued by
    the calling thread.
  */
  //virtual void asyncCancel() throw(AsynchronousException) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
