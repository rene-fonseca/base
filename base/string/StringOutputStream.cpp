/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/StringOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void StringOutputStreamWrapper::close() throw(IOException) {
  assert(!closed, IOException("Output stream is closed"));
  closed = true;
}

void StringOutputStreamWrapper::flush() throw(IOException) {
  assert(!closed, IOException("Output stream is closed"));
}

unsigned int StringOutputStreamWrapper::write(const char* buffer, unsigned int size, bool nonblocking) throw(IOException) {
  assert(!closed, IOException("Output stream is closed"));
  string.append(buffer, size);
  return size;
}



StringOutputStream::StringOutputStream() throw(BindException) :
  FormatOutputStream(stream) {
  stream.setGranularity(DEFAULT_GRANULARITY);
}

StringOutputStream::StringOutputStream(unsigned int granularity) throw(BindException) :
  FormatOutputStream(stream) {
  stream.setGranularity(granularity);
}

unsigned int StringOutputStream::getGranularity() const throw() {
  return stream.getGranularity();
}

void StringOutputStream::setGranularity(unsigned int granularity) throw() {
  stream.setGranularity(granularity);
}

String StringOutputStream::getString() const throw() {
  return stream.getString();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
