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

void FilterInputStream::close() throw(IOException) {
  in->close();
}

int FilterInputStream::read() throw(IOException) {
  return in->read();
}

unsigned int FilterInputStream::read(char* buffer, unsigned int size) throw(IOException) {
  return in->read(buffer, size);
}

void FilterInputStream::skip(unsigned int count) throw(IOException) {
  in->skip(count);
}

FilterInputStream::~FilterInputStream() {
}
