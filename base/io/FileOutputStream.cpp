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

FileOutputStream::~FileOutputStream() {
  delete fd;
}
