/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FilterInputStream.h"

FilterInputStream::FilterInputStream(InputStream& i) throw(BindException) : in(i) {
}

bool FilterInputStream::atEnd() const throw(IOException) {
  return in.atEnd();
}

unsigned int FilterInputStream::available() const throw(IOException) {
  return in.available();
}

void FilterInputStream::close() throw(IOException) {
  in.close();
}

unsigned int FilterInputStream::read(char* buffer, unsigned int size) throw(IOException) {
  return in.read(buffer, size);
}

unsigned int FilterInputStream::skip(unsigned int count) throw(IOException) {
  return in.skip(count);
}

FilterInputStream::~FilterInputStream() {
}
