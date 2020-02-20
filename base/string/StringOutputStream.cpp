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
#include <base/concurrency/ThreadLocalContext.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// #define _COM_AZURE_DEV__BASE__REUSE_BUFFER

StringOutputStreamWrapper::StringOutputStreamWrapper() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__REUSE_BUFFER)
  if (auto tlc = Thread::getLocalContext()) {
    swapper(string, tlc->stringOutputStream);
    BASSERT(!string);
#if 0
    if (string.getCapacity() > 0) {
      static unsigned int count = 0;
      ++count;
      fout << "StringOutputStreamWrapper:: reuse=" << count << ENDL;
    }
#endif
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
  flush(); // we must empty buffered data
  closed = false;
  string.forceToLength(0); // clear() switches to default empty string!
}

unsigned int StringOutputStreamWrapper::write(
  const uint8* buffer,
  unsigned int size,
  bool nonblocking)
{
  bassert(!closed, IOException("Output stream is closed.", this));
  if (size > 0) {
    string.append(ConstSpan<char>(reinterpret_cast<const char*>(buffer), size));
  }
  return size;
}

StringOutputStreamWrapper::~StringOutputStreamWrapper()
{
#if defined(_COM_AZURE_DEV__BASE__REUSE_BUFFER)
  if (!string.isMultiReferenced() &&
      (string.getLength() < StringOutputStream::DEFAULT_CAPACITY) && // exclude terminator
      (string.getCapacity() == StringOutputStream::DEFAULT_CAPACITY)) { // avoid any resize
    if (auto tlc = Thread::getLocalContext()) {
#if 0
      static unsigned int count = 0;
      ++count;
      fout << "StringOutputStreamWrapper(): STORE FOR REUSE:" << count << ENDL;
#endif
      string.forceToLength(0); // clear() switches to default empty string!
      swapper(string, tlc->stringOutputStream);
      return;
    }
  }
#endif
}



StringOutputStream::StringOutputStream()
  : FormatOutputStream(stream)
{
  stream.ensureCapacity(DEFAULT_CAPACITY); // TAG: what is a good default
}

StringOutputStream::StringOutputStream(MemorySize capacity)
  : FormatOutputStream(stream)
{
  // do not reuse buffer in this case
  // String buffer(capacity);
  // stream.setBuffer(buffer);
  stream.ensureCapacity(capacity);
}

void StringOutputStream::ensureCapacity(MemorySize capacity)
{
  stream.ensureCapacity(capacity);
}

void StringOutputStream::flush()
{
  FormatOutputStream::flush(); // must be first
  stream.flush();
}

const String& StringOutputStream::getString() const
{
  // flush() should be considered cache so constness could still be ok here - yet ugly
  // what is the alternative - mutable all the way through?
  StringOutputStream* sos = const_cast<StringOutputStream*>(this);
  sos->FormatOutputStream::flush(); // must be first
  sos->stream.flush();
  
  return stream.getString();
}

String StringOutputStream::toString()
{
  FormatOutputStream::flush(); // must be first
  stream.flush();
  
  // does NOT copy capacity
  String result = stream.getString().copy(); // we want to preserve ownership in stream
  stream.restart();
  return result;
}

void StringOutputStream::restart()
{
  flush();
  stream.restart();
}

StringOutputStream::~StringOutputStream()
{
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(StringOutputStream) : public UnitTest {
public:

  TEST_PRIORITY(40);
  TEST_PROJECT("base/string");

  void run() override
  {
    StringOutputStream sos;
    sos << "Hello, World!";
    String s = sos; // restarts stream
    TEST_ASSERT(s == "Hello, World!");
    TEST_ASSERT(!sos.getString());

    sos << 123;
    sos << 123U;
    sos << 123.123;
    sos << L"Hello, World!";
    sos << true;
    sos << false;
  }
};

TEST_REGISTER(StringOutputStream);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
