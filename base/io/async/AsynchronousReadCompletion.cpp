/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/async/AsynchronousReadCompletion.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

AsynchronousReadCompletion::AsynchronousReadCompletion(const AsynchronousReadCompletion& copy) throw() 
  : bytesToRead(copy.bytesToRead),
    buffer(copy.buffer),
    offset(copy.offset),
    bytesRead(copy.bytesRead),
    flags(copy.flags) {
}

AsynchronousReadCompletion& AsynchronousReadCompletion::operator=(const AsynchronousReadCompletion& eq) throw() {
  // no need to protect against self-assignment
  bytesToRead = eq.bytesToRead;
  buffer = eq.buffer;
  offset = eq.offset;
  bytesRead = eq.bytesRead;
  flags = eq.flags;
  return *this;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
