/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileInputStream.h"
#include <fcntl.h>

FileInputStream::FileInputStream(const String<>& p, unsigned int flags) throw(FileNotFound) :
  path(p) {
  unsigned int temp = 0;
  if (flags & NONBLOCK) {
    temp |= O_NONBLOCK;
  }
  int handle = ::open(path, temp | O_RDONLY);
  if (handle == -1) {
    throw FileNotFound("Unable to open file for reading.");
  }
  setHandle(handle);
}

const String<>& FileInputStream::getPath() const throw() {
  return path;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileInputStream& value) {
  return stream << "class/FileInputStream{"
                << "path=" << value.getPath() << ";"
                << "fd=" << value.getHandle()
                << "}";
  return stream;
}
