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

#include <base/io/async/AsynchronousException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Asynchronous stream.
  
  @short Asynchronous stream.
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AsynchronousStream {
public:

  /**
    Inserts an explicit tests for any pending asynchronous operation
    completions. Relinquishes the currently executing thread voluntarily.
  */
  static bool asyncTest() noexcept;

  /**
    Aborts any pending asynchronous operations (read as well as write) issued by
    the executing thread.
  */
  //virtual void asyncCancel() throw(AsynchronousException) = 0;

  virtual ~AsynchronousStream() noexcept(false);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
