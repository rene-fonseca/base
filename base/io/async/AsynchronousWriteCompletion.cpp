/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/async/AsynchronousWriteCompletion.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

AsynchronousWriteCompletion::AsynchronousWriteCompletion(
  const AsynchronousWriteCompletion& copy) throw() 
  : buffer(copy.buffer),
    bytesToWrite(copy.bytesToWrite),
    offset(copy.offset),
    bytesWritten(copy.bytesWritten),
    flags(copy.flags) {
}

AsynchronousWriteCompletion& AsynchronousWriteCompletion::operator=(
  const AsynchronousWriteCompletion& eq) throw() {
  // no need to protect against self-assignment
  buffer = eq.buffer;
  bytesToWrite = eq.bytesToWrite;
  offset = eq.offset;
  bytesWritten = eq.bytesWritten;
  flags = eq.flags;
  return *this;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
