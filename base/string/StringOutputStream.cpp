/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void StringOutputStreamWrapper::close() throw(IOException) {
  bassert(!closed, IOException("Output stream is closed", this));
  closed = true;
}

void StringOutputStreamWrapper::flush() throw(IOException) {
  bassert(!closed, IOException("Output stream is closed", this));
}

unsigned int StringOutputStreamWrapper::write(
  const uint8* buffer,
  unsigned int size,
  bool nonblocking) throw(IOException) {
  bassert(!closed, IOException("Output stream is closed", this));
  string.append(NativeString(Cast::pointer<const char*>(buffer)), size);
  return size;
}



StringOutputStream::StringOutputStream() throw(BindException) :
  FormatOutputStream(stream) {
  stream.setGranularity(DEFAULT_GRANULARITY);
}

StringOutputStream::StringOutputStream(
  unsigned int granularity) throw(BindException)
  : FormatOutputStream(stream) {
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
