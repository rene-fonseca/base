/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileInputStream.h"
#include <fcntl.h>

FileInputStream::FileInputStream(const char* path) throw(FileNotFound) {
  int handle = open(path, 0);
  if (handle == -1) {
    throw FileNotFound("Unable to open file.");
  }
  fd = new FileDescriptor(handle);
}

unsigned int FileInputStream::available() {
  // NOT IMPLEMENTED
  return 0;
}

void FileInputStream::close() throw(IOException) {
  // NOT IMPLEMENTED
}

int FileInputStream::read() throw(IOException) {
  char buffer;
  unsigned int count;
  count = read(&buffer, sizeof(buffer));
  if (count == 0) {
    return -1;
  }
  return buffer;
}

unsigned int FileInputStream::read(char* buffer, unsigned int size) throw(IOException) {
  return fd->read(buffer, size);
}

FileInputStream::~FileInputStream() {
//  close(fd->getHandle());
  delete fd;
}
