/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "OutputStream.h"

void OutputStream::close() throw(IOException) {
}

void OutputStream::flush() throw(IOException) {
}

void OutputStream::write(const char* buffer, unsigned int size) throw(IOException) {
  const char* head = buffer;
  while (size--) {
    write(*head);
    ++head;
  }
}
