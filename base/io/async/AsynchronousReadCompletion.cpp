/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/async/AsynchronousReadCompletion.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

AsynchronousReadCompletion::AsynchronousReadCompletion(
  const AsynchronousReadCompletion& copy) noexcept 
  : buffer(copy.buffer),
    bytesToRead(copy.bytesToRead),
    offset(copy.offset),
    bytesRead(copy.bytesRead),
    flags(copy.flags) {
}

AsynchronousReadCompletion& AsynchronousReadCompletion::operator=(
  const AsynchronousReadCompletion& assign) noexcept
{
  // no need to protect against self-assignment
  buffer = assign.buffer;
  bytesToRead = assign.bytesToRead;
  offset = assign.offset;
  bytesRead = assign.bytesRead;
  flags = assign.flags;
  return *this;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
