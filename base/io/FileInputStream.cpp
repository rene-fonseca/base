/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

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

unsigned int FileInputStream::available() const throw(IOException) {
  struct stat status;
  off_t position;
  if ((::fstat(getHandle(), &status) != 0) || ((position = ::lseek(getHandle(), 0, SEEK_CUR)) == -1)) {
    throw IOException("Unable to get available bytes");
  }
  return status.st_size - position;
}

const String<>& FileInputStream::getPath() const throw() {
  return path;
}

void FileInputStream::wait() const throw(IOException) {
  // now what
}

bool FileInputStream::wait(unsigned int timeout) const throw(IOException) {
  // now what
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const FileInputStream& value) {
  return stream << "class/FileInputStream{"
                << "path=" << value.getPath() << ";"
                << "fd=" << value.getHandle()
                << "}";
  return stream;
}
