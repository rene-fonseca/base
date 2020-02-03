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
#include <base/concurrency/ThreadLocalContext.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// we really want native WideString buffer to avoid copy on use
// #define _COM_AZURE_DEV__BASE__REUSE_BUFFER

WideStringOutputStreamWrapper::WideStringOutputStreamWrapper() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__REUSE_BUFFER)
  if (auto tlc = Thread::getLocalContext()) {
    swapper(string, tlc->stringOutputStream);
    BASSERT(!string);
#if 0
    if (string.getCapacity() > 0) {
      static unsigned int count = 0;
      ++count;
      fout << "WideStringOutputStreamWrapper:: reuse=" << count << ENDL;
    }
#endif
  }
#endif
}

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
  // should we allow flush for closed
  bassert(!closed, IOException("Output stream is closed.", this));
}

void WideStringOutputStreamWrapper::restart()
{
  closed = false;
  string.forceToLength(0); // clear() switches to default empty string!
}

unsigned int WideStringOutputStreamWrapper::write(
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

WideStringOutputStreamWrapper::~WideStringOutputStreamWrapper()
{
#if defined(_COM_AZURE_DEV__BASE__REUSE_BUFFER)
  if (!string.isMultiReferenced() &&
      (string.getLength() < StringOutputStream::DEFAULT_CAPACITY) && // exclude terminator
      (string.getCapacity() == StringOutputStream::DEFAULT_CAPACITY)) { // avoid any resize
    if (auto tlc = Thread::getLocalContext()) {
#if 0
      static unsigned int count = 0;
      ++count;
      fout << "WideStringOutputStreamWrapper(): STORE FOR REUSE:" << count << ENDL;
#endif
      string.forceToLength(0); // clear() switches to default empty string!
      swapper(string, tlc->stringOutputStream);
      return;
    }
  }
#endif
}



WideStringOutputStream::WideStringOutputStream()
  : FormatOutputStream(stream)
{
  stream.ensureCapacity(DEFAULT_CAPACITY); // TAG: what is a good default
}

WideStringOutputStream::WideStringOutputStream(MemorySize capacity)
  : FormatOutputStream(stream)
{
  // do not reuse buffer in this case
  // String buffer(capacity);
  // stream.setBuffer(buffer);
  stream.ensureCapacity(capacity);
}

void WideStringOutputStream::ensureCapacity(MemorySize capacity)
{
  stream.ensureCapacity(capacity);
}

const String& WideStringOutputStream::getString() const noexcept
{
  // TAG: check where this is used
  return stream.getString();
}

String WideStringOutputStream::toString() noexcept
{
  flush();
  // does NOT copy capacity
  String result = stream.getString().copy(); // we want to preserve ownership in stream
  stream.restart();
  return result;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
