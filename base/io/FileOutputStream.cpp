/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileOutputStream.h"
#include <fcntl.h>

FileOutputStream::FileOutputStream(const String& p, unsigned int flags, unsigned int permissions) throw(FileNotFound) :
  path(p) {

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
    throw FileNotFound("Unable to open file.");
  }
  setHandle(handle);
}

FileOutputStream::~FileOutputStream() {
}
