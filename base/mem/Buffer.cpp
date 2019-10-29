/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/mem/Buffer.h>
#include <base/UnitTest.h>
#include <string.h>
#include <stdlib.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#define NUMBER_TO_GRAN(NUMBER, GRAN) ((((NUMBER) + (GRAN) - 1)/(GRAN)) * (GRAN))

Buffer::Buffer(MemorySize size, MemorySize granularity) throw(MemoryException)
{
  this->granularity = (granularity > 0) ? granularity : 1;
  this->size = NUMBER_TO_GRAN(size, granularity);
  if (this->size > 0) {
    bytes = static_cast<uint8*>(malloc(this->size));
  }
  if ((bytes == nullptr) && (this->size != 0)) { // was memory allocated
    throw MemoryException(this);
  }
}

Buffer::Buffer(const Buffer& copy) throw(MemoryException)
{
  granularity = copy.granularity;
  size = copy.size;
  bytes = static_cast<uint8*>(malloc(size));
  if ((bytes == nullptr) && (size != 0)) { // was memory allocated
    throw MemoryException(this);
  }
  memcpy(bytes, copy.bytes, size); // copy buffer
}

void Buffer::setSize(MemorySize size) throw(MemoryException)
{
  size = NUMBER_TO_GRAN(size, granularity);
  if (size != this->size) { // do we really have to
    this->size = size;
    if (size == 0) {
      if (bytes) {
        ::free(bytes);
        bytes = nullptr;
      }
      return;
    }
    uint8* result = static_cast<uint8*>(realloc(bytes, size));
    if ((result == nullptr) && (size > 0)) { // was memory allocated
      throw MemoryException(this);
    }
    bytes = result;
  }
}

Buffer::~Buffer() throw() {
  if (bytes) {
    ::free(bytes);
  }
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Buffer) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base/mem");
  TEST_TIMEOUT_MS(30 * 1000);
  // TAG: TEST_LIMIT_PROCESSING_TIME(1000);
  // TAG: TEST_LIMIT_MEMORY(1024);
  // TAG: TEST_LIMIT_LEAK_MEMORY(1024);

  void run() override
  {
    Buffer b1;
    auto s = b1.getSize();
    TEST_ASSERT(s == 0);
    auto b = b1.getBytes();
    TEST_ASSERT(b == nullptr);
    
    Buffer b2(8 * 1024);
    TEST_ASSERT(b2.getSize() == (8 * 1024));
    fill<uint8>(b2.getBytes(), b2.getSize(), 0x3e);
    
    b2.setSize(7 * 1024);
    bool wrong = false;
    for (auto i : b2) {
      if (i != 0x3e) {
        wrong = true;
        break;
      }
    }
    TEST_ASSERT(!wrong);
    
    auto b3 = b2;

    b2.setSize(10 * 1024);
    wrong = false;
    for (MemorySize i = 0; i < (7 * 1024); ++i) { // make sure data is preserved
      if (b2[i] != 0x3e) {
        wrong = true;
        break;
      }
    }
    TEST_ASSERT(!wrong);

    b2.setSize(0);
    TEST_ASSERT(b2.getBytes() == nullptr);
  }
};

TEST_REGISTER(Buffer);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
