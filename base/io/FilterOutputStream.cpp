/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FilterOutputStream.h"

FilterOutputStream::FilterOutputStream(OutputStream* out) {
  this->out = out;
}

void FilterOutputStream::close() {
  out->close();
}

void FilterOutputStream::flush() {
  out->flush();
}

void FilterOutputStream::write(char value) {
  out->write(value);
}

void FilterOutputStream::write(char* buffer, unsigned int size) {
  out->write(buffer, size);
}

FilterOutputStream::~FilterOutputStream() {
}
