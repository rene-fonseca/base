/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileOutputStream.h"
#include <fcntl.h>

FileOutputStream::FileOutputStream(const char* name) throw(FileNotFound) {
  int handle = open(name, 0);
  if (handle == -1) {
    throw FileNotFound("Unable to open file.");
  }
  fd = new FileDescriptor(handle);
}

FileOutputStream::FileOutputStream(int handle) throw(BindException) {
  fd = new FileDescriptor(handle);
}

void FileOutputStream::close() throw(IOException) {
  fd->close();
}

void FileOutputStream::flush() throw(IOException) {
  fd->flush();
}

void FileOutputStream::write(char value) throw(IOException) {
}

void FileOutputStream::write(const char* buffer, unsigned int size) throw(IOException) {
}

FileOutputStream::~FileOutputStream() {
  delete fd;
}
