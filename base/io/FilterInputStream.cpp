/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FilterInputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FilterInputStream::FilterInputStream(
  InputStream& i) throw(BindException) : in(i) {
}

unsigned int FilterInputStream::available() const throw(IOException) {
  return in.available();
}

void FilterInputStream::close() throw(IOException) {
  in.close();
}

unsigned int FilterInputStream::read(
  uint8* buffer, unsigned int size, bool nonblocking) throw(IOException) {
  return in.read(buffer, size, nonblocking);
}

unsigned int FilterInputStream::skip(unsigned int count) throw(IOException) {
  return in.skip(count);
}

void FilterInputStream::wait() const throw(IOException) {
  in.wait();
}

bool FilterInputStream::wait(unsigned int timeout) const throw(IOException) {
  return in.wait(timeout);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
