/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "InputStream.h"

unsigned int InputStream::available() {
  return 0;
}

void InputStream::close() throw(IOException) {
}

unsigned int InputStream::read(char* buffer, unsigned int size) throw(IOException) {
  char* head = buffer;
  unsigned int count = 0;

  while (count < size) {
    int value = read();
    if (value == -1) { // have we reached the end of the stream
      break;
    }
    *head = value;
    ++head;
    ++count;
  }
  return count;
}

void InputStream::skip(unsigned int count) throw(IOException) {
  while (count) {
    read();
    --count;
  }
}
