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

StringOutputStreamWrapper::StringOutputStreamWrapper() noexcept
{
  // TAG: we want to reuse the last String for the current Thread - needs testing
#if 0
  if (auto tlc = Thread::getLocalContext()) {
    swapper(string, tlc->string);
  }
#endif
}

void StringOutputStreamWrapper::ensureCapacity(MemorySize capacity)
{
  string.ensureCapacity(capacity);
}

void StringOutputStreamWrapper::close()
{
  bassert(!closed, IOException("Output stream is closed.", this));
  closed = true;
}

void StringOutputStreamWrapper::flush()
{
  // should we allow flush for closed
  bassert(!closed, IOException("Output stream is closed.", this));
}

void StringOutputStreamWrapper::restart()
{
  closed = false;
  string.clear();
}

unsigned int StringOutputStreamWrapper::write(
  const uint8* buffer,
  unsigned int size,
  bool nonblocking)
{
  bassert(!closed, IOException("Output stream is closed.", this));
  if (size > 0) {
    string.append(MemorySpan(buffer, size));
  }
  return size;
}

StringOutputStreamWrapper::~StringOutputStreamWrapper()
{
  // TAG: store String for reuse
#if 0
  if (auto tlc = Thread::getLocalContext()) {
    string.clear();
    swapper(string, tlc->string);
  }
#endif
}



StringOutputStream::StringOutputStream()
  : FormatOutputStream(stream)
{
  stream.ensureCapacity(DEFAULT_CAPACITY);
}

StringOutputStream::StringOutputStream(MemorySize capacity)
  : FormatOutputStream(stream)
{
  stream.ensureCapacity(capacity);
}

void StringOutputStream::ensureCapacity(MemorySize capacity)
{
  stream.ensureCapacity(capacity);
}

const String& StringOutputStream::getString() const noexcept
{
  return stream.getString();
}

String StringOutputStream::toString() noexcept
{
  // TAG: it could be useful to NOT copy if temporary string
  flush();
  // does NOT copy capacity
  String result = stream.getString().copy(); // we want to preserve ownership in stream
  stream.restart();
  return result;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
