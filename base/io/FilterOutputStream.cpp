/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FilterOutputStream.h"

FilterOutputStream::FilterOutputStream(OutputStream* out) {
  this->out = out;
}

void FilterOutputStream::close() throw(IOException) {
  out->close();
}

void FilterOutputStream::flush() throw(IOException) {
  out->flush();
}

void FilterOutputStream::write(char value) throw(IOException) {
  out->write(value);
}

void FilterOutputStream::write(char* buffer, unsigned int size) throw(IOException) {
  out->write(buffer, size);
}

FilterOutputStream::~FilterOutputStream() {
}
