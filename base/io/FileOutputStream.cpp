/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileOutputStream.h>
#include <fcntl.h>

FileOutputStream::FileOutputStream(const String<>& p, unsigned int flags, unsigned int permissions) throw(FileNotFound) :
  FileDescriptorOutputStream(), path(p) {

  unsigned int temp = 0;
  if (flags & CREATE) {
    temp |= O_CREAT;
  }
  if (flags & TRUNCATE) {
    temp |= O_TRUNC;
  }
  if (flags & APPEND) {
    temp |= O_APPEND;
  }
  if (flags & NONBLOCK) {
    temp |= O_NONBLOCK;
  }
  if (flags & SYNC) {
    temp |= O_SYNC;
  }

  int handle = ::open(path, temp | O_WRONLY, permissions);
  if (handle == -1) {
    throw FileNotFound("Unable to open file");
  }
  setHandle(handle);
}

const String<>& FileOutputStream::getPath() const throw() {
  return path;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileOutputStream& value) {
  return stream << "class/FileOutputStream{"
                << "path=" << value.getPath() << ";"
                << "fd=" << value.getHandle()
                << "}";
  return stream;
}
