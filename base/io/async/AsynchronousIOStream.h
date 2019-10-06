/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/io/async/AsynchronousException.h>
#include <base/io/async/AsynchronousReadOperation.h>
#include <base/io/async/AsynchronousWriteOperation.h>
#include <base/io/async/AsynchronousReadEventListener.h>
#include <base/io/async/AsynchronousWriteEventListener.h>
#include <base/io/async/AsynchronousStream.h>
#include <base/OperatingSystem.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

typedef AsynchronousWriteOperation A;

/**
  Asynchronous IO stream.

  @author Rene Fonseca
*/
class AsynchronousIOStream : public virtual AsynchronousStream {
  friend class AsynchronousReadOperation;
  friend class AsynchronousReadContext;
  friend class AsynchronousWriteOperation;
  friend class AsynchronousWriteContext;
protected:

  /**
    Returns the operating system handle to the stream.
  */
  virtual OperatingSystem::Handle getHandle() const throw() = 0;
public:

  /**
    Requests and asynchronous read operation.

    @param buffer The bytes to be read.
    @param bytesToRead The number of bytes to be read.
    @param offset The offset.
    @param listener The event listener.
  */
  virtual AsynchronousReadOperation read(
    uint8* buffer,
    unsigned int bytesToRead,
    unsigned long long offset,
    AsynchronousReadEventListener* listener) throw(AsynchronousException) = 0;

  /**
    Requests and asynchronous write operation.

    @param buffer The bytes to be written.
    @param bytesToWrite The number of bytes to be written.
    @param offset The offset.
    @param listener The event listener.
  */
  virtual AsynchronousWriteOperation write(
    const uint8* buffer,
    unsigned int bytesToWrite,
    unsigned long long offset,
    AsynchronousWriteEventListener* listener) throw(AsynchronousException) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
