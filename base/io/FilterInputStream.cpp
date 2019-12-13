/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FilterInputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FilterInputStream::FilterInputStream(
  InputStream& i) : in(i) {
}

unsigned int FilterInputStream::available() const {
  return in.available();
}

void FilterInputStream::close() {
  in.close();
}

unsigned int FilterInputStream::read(
  uint8* buffer, unsigned int size, bool nonblocking) {
  return in.read(buffer, size, nonblocking);
}

unsigned int FilterInputStream::skip(unsigned int count)
{
  return in.skip(count);
}

void FilterInputStream::wait() const
{
  in.wait();
}

bool FilterInputStream::wait(unsigned int timeout) const
{
  return in.wait(timeout);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
