/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/WideStringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void WideStringOutputStreamWrapper::ensureCapacity(MemorySize capacity)
{
  string.ensureCapacity(capacity);
}

void WideStringOutputStreamWrapper::close()
{
  bassert(!closed, IOException("Output stream is closed.", this));
  closed = true;
}

void WideStringOutputStreamWrapper::flush()
{
  bassert(!closed, IOException("Output stream is closed.", this));
}

void WideStringOutputStreamWrapper::restart()
{
  closed = false;
  string.clear();
}

unsigned int WideStringOutputStreamWrapper::write(
  const uint8* buffer,
  unsigned int size,
  bool nonblocking)
{
  bassert(!closed, IOException("Output stream is closed.", this));
  if (size > 0) {
// TAG:    string.append(MemorySpan(buffer, size));
  }
  return size;
}



WideStringOutputStream::WideStringOutputStream()
  : FormatOutputStream(stream)
{
  stream.ensureCapacity(1024);
}

WideStringOutputStream::WideStringOutputStream(
  unsigned int granularity)
  : FormatOutputStream(stream)
{
  stream.ensureCapacity(1024);
}

void WideStringOutputStream::ensureCapacity(MemorySize capacity)
{
  stream.ensureCapacity(capacity);
}

const String& WideStringOutputStream::getString() const noexcept
{
  static String string;
  return string;
// TAG:  return stream.getString();
}

String WideStringOutputStream::toString()
{
  static String string;
  return string;
// TAG: FIXME
#if 0
  flush();
  WideString result = stream.getString().copy(); // we want to preserve ownership in stream
  stream.restart();
  return result;
#endif
  
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
