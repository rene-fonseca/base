/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FilterOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FilterOutputStream::FilterOutputStream(OutputStream& o) throw(BindException) : out(o) {
}

void FilterOutputStream::close() throw(IOException) {
  out.close();
}

void FilterOutputStream::flush() throw(IOException) {
  out.flush();
}

unsigned int FilterOutputStream::write(const char* buffer, unsigned int size) throw(IOException) {
  return out.write(buffer, size);
}

FilterOutputStream::~FilterOutputStream() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
