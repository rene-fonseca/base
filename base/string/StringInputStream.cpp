/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/StringInputStream.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

StringInputStream::StringInputStream(String& _string) throw(BindException)
  : string(_string) {
  index = 0;
  eof = false;
  closed = false;
}

unsigned int StringInputStream::available() throw(IOException) {
  assert(!closed, IOException(this));
  return string.getLength() - index;
}

void StringInputStream::close() throw(IOException) {
  assert(!closed, IOException(this));
  string = String();
  closed = true;
}

unsigned int StringInputStream::read(char* buffer, unsigned int size) throw(IOException) {
  assert(!closed, IOException(this));
  if (index >= string.getLength()) {
    assert(!eof, EndOfFile(this));
    eof = true;
    return 0;
  }
  if (size >= string.getLength() - index) {
    size = string.getLength() - index;
  }
  copy<char>(buffer, string.getElements(), size);
  return size;
}

unsigned int StringInputStream::skip(unsigned int count) throw(IOException) {
  assert(!closed, IOException(this));
  if (index >= string.getLength()) {
    assert(!eof, EndOfFile(this));
    eof = true;
    return 0;
  }
  if (count >= string.getLength() - index) {
    count = string.getLength() - index;
  }
  index += count;
  return count;
}

StringInputStream::~StringInputStream() throw(IOException) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
