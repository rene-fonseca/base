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

void StringOutputStreamWrapper::ensureCapacity(MemorySize capacity)
{
  string.ensureCapacity(capacity);
}

void StringOutputStreamWrapper::close() throw(IOException)
{
  bassert(!closed, IOException("Output stream is closed", this));
  closed = true;
}

void StringOutputStreamWrapper::flush() throw(IOException)
{
  bassert(!closed, IOException("Output stream is closed", this));
}

unsigned int StringOutputStreamWrapper::write(
  const uint8* buffer,
  unsigned int size,
  bool nonblocking) throw(IOException)
{
  bassert(!closed, IOException("Output stream is closed", this));
  if (size > 0) {
    string.append(MemorySpan(buffer, size));
  }
  return size;
}



StringOutputStream::StringOutputStream() throw(BindException) :
  FormatOutputStream(stream)
{
  stream.setGranularity(DEFAULT_GRANULARITY);
}

StringOutputStream::StringOutputStream(
  unsigned int granularity) throw(BindException)
  : FormatOutputStream(stream)
{
  stream.setGranularity(granularity);
}

void StringOutputStream::ensureCapacity(MemorySize capacity)
{
  stream.ensureCapacity(capacity);
}

unsigned int StringOutputStream::getGranularity() const noexcept
{
  return stream.getGranularity();
}

void StringOutputStream::setGranularity(unsigned int granularity) throw()
{
  stream.setGranularity(granularity);
}

const String& StringOutputStream::getString() const noexcept
{
  return stream.getString();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
