/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FilterOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FilterOutputStream::FilterOutputStream(
  OutputStream& o) : out(o) {
}

void FilterOutputStream::close() {
  out.close();
}

void FilterOutputStream::flush() {
  out.flush();
}

unsigned int FilterOutputStream::write(
  const uint8* buffer,
  unsigned int size,
  bool nonblocking) {
  return out.write(buffer, size, nonblocking);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
