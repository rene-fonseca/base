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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#define NUMBER_TO_GRAN(NUMBER, GRAN) ((((NUMBER) + (GRAN) - 1)/(GRAN)) * (GRAN))

Buffer::Buffer(MemorySize size, MemorySize granularity)
{
  this->granularity = (granularity > 0) ? granularity : 1;
  this->size = NUMBER_TO_GRAN(size, granularity);
  heap.resize(this->size);
}

Buffer::Buffer(const Buffer& copy)
{
  granularity = copy.granularity;
  size = copy.size;
  heap.resize(size);
  base::copy<uint8>(heap, copy.heap, copy.size);
}

void Buffer::setSize(MemorySize size)
{
  size = NUMBER_TO_GRAN(size, granularity);
  if (size != this->size) { // do we really have to
    this->size = size;
    heap.resize(size);
  }
}

Buffer::~Buffer() noexcept
{
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
