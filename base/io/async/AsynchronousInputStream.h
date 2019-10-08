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

#include <base/io/async/AsynchronousException.h>
#include <base/io/async/AsynchronousStream.h>
#include <base/io/async/AsynchronousReadEventListener.h>
#include <base/io/async/AsynchronousReadOperation.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class _DK_SDU_MIP__BASE__API AsynchronousInputStream : public virtual AsynchronousStream {
public:

  /**
    Requests and asynchronous read operation.

    @param buffer The bytes to be read.
    @param bytesToRead The number of bytes to be read.
    @param listener The listener to be notified on completion.
  */
  virtual AsynchronousReadOperation read(
    uint8* buffer,
    unsigned int bytesToRead,
    AsynchronousReadEventListener* listener) throw(AsynchronousException) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
