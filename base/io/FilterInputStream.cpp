/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FilterInputStream.h"

FilterInputStream::FilterInputStream(InputStream* in) {
  this->in = in;
}

unsigned int FilterInputStream::available() {
  return in->available();
}

int FilterInputStream::read() {
  return in->read();
}

unsigned int FilterInputStream::read(char* buffer, unsigned int size) {
  return in->read(buffer, size);
}

void FilterInputStream::skip(unsigned int count) {
  in->skip(count);
}

FilterInputStream::~FilterInputStream() {
}
