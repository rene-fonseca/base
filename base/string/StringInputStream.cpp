/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/StringInputStream.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

StringInputStream::StringInputStream(String& s) throw(BindException) : str(s) {
  index = 0;
  eof = false;
  closed = false;
}

unsigned int StringInputStream::available() throw(IOException) {
  if (closed) {
    throw IOException();
  }
  return str.getLength() - index;
}

void StringInputStream::close() throw(IOException) {
  if (closed) {
    throw IOException();
  }
  str = "";
  closed = true;
}

unsigned int StringInputStream::read(char* buffer, unsigned int size) throw(IOException) {
  if (closed) {
    throw IOException();
  }
  if (index >= str.getLength()) {
    if (eof) {
      throw EndOfFile();
    }
    eof = true;
    return 0;
  }
  if (size >= str.getLength() - index) {
    size = str.getLength() - index;
  }
  copy<char>(buffer, str.getElements(), size);
  return size;
}

unsigned int StringInputStream::skip(unsigned int count) throw(IOException) {
  if (closed) {
    throw IOException();
  }
  if (index >= str.getLength()) {
    if (eof) {
      throw EndOfFile();
    }
    eof = true;
    return 0;
  }
  if (count >= str.getLength() - index) {
    count = str.getLength() - index;
  }
  index += count;
  return count;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
