/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/StringInputStream.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

StringInputStream::StringInputStream(String<>& s) throw(BindException) : str(s) {
  index = 0;
  eof = false;
  closed = false;
}

unsigned int StringInputStream::available() throw(IOException) {
  if (closed) {
    throw IOException();
  }
  return str.length() - index;
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
  if (index >= str.length()) {
    if (eof) {
      throw EndOfFile();
    }
    eof = true;
    return 0;
  }
  if (size >= str.length() - index) {
    size = str.length() - index;
  }
  copy<char>(buffer, str.getBytes(), size);
  return size;
}

unsigned int StringInputStream::skip(unsigned int count) throw(IOException) {
  if (closed) {
    throw IOException();
  }
  if (index >= str.length()) {
    if (eof) {
      throw EndOfFile();
    }
    eof = true;
    return 0;
  }
  if (count >= str.length() - index) {
    count = str.length() - index;
  }
  index += count;
  return count;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
