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
#include <base/io/async/AsynchronousStream.h>
#include <base/io/async/AsynchronousWriteEventListener.h>
#include <base/io/async/AsynchronousWriteOperation.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API AsynchronousOutputStream : public virtual AsynchronousStream {
public:

  /**
    Requests and asynchronous write operation.

    @param buffer The bytes to be read.
    @param bytesToWrite The number of bytes to be written.
    @param listener The listener to be notified on completion.
  */
  virtual AsynchronousWriteOperation write(
    const uint8* buffer,
    unsigned int bytesToWrite,
    AsynchronousWriteEventListener* listener) = 0;

  inline ~AsynchronousOutputStream() noexcept(false) { // MSC doesnt inherit noexcept
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
