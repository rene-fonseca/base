/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileDescriptor.h"

FileDescriptor::FileDescriptor(int handle) {
  this->handle = handle;
}

char FileDescriptor::read() throw() {
  char buffer;
  read(handle, &buffer, sizeof(buffer));
  return buffer;
}

FileDescriptor::read(char* buffer, unsigned int size) {
  read(handle, buffer, size);
}

FileDescriptor::~FileDescriptor() {
}

