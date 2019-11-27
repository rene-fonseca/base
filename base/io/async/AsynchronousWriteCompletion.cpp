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
    flags(copy.flags)
{
}

AsynchronousWriteCompletion& AsynchronousWriteCompletion::operator=(
  const AsynchronousWriteCompletion& assign) throw()
{
  // no need to protect against self-assignment
  buffer = assign.buffer;
  bytesToWrite = assign.bytesToWrite;
  offset = assign.offset;
  bytesWritten = assign.bytesWritten;
  flags = assign.flags;
  return *this;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
