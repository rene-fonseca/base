/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "OutputStream.h"

void OutputStream::close() {
}

void OutputStream::flush() {
}

void OutputStream::write(char* buffer, unsigned int size) {
  char* head = buffer;
  while (size--) {
    write(*head);
    ++head;
  }
}
