/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "InputStream.h"

void InputStream::close() {
}

unsigned int InputStream::available() {
  return 0;
}

unsigned int InputStream::read(char* buffer, unsigned int size) {
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

void InputStream::skip(unsigned int count) {
  while (count) {
    read();
    --count;
  }
}
