/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/Allocator.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__INSTANTIATE_CONTAINER_COMMON_TYPE_LIMITED(Allocator)

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Allocator) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_PROJECT("base/mem");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  
  void run() override
  {
    Allocator<uint8> a1;
    TEST_ASSERT(!a1.getElements());
    Allocator<uint8> a2(1023);
    for (auto& v : a2) {
      v = 0xfe;
    }
    a1 = a2;
    TEST_ASSERT(a1.getElements() != a2.getElements());
    Allocator<uint8> a3(a2);

    a1 = moveObject(a3);
    // TEST_ASSERT(a1.getElements() != a3.getElements());

    a2.getBeginIterator()[1023 - 1] = 0x07;
    a2.setSize(12345, 0x3f);
    TEST_ASSERT(a2.getSize() == 12345);
    TEST_ASSERT(a2.getBeginIterator()[1023 - 1] == 0x07);
    TEST_ASSERT(a2.getBeginIterator()[1023] == 0x3f);
    TEST_ASSERT(a2.getBeginReadIterator()[a2.getSize() - 1] == 0x3f);
    a2.ensureCapacity(23456);
    a2.setSize(345);
    auto size = a2.getSize();
    TEST_ASSERT(size == 345);
    auto capacity = a2.getCapacity();
    TEST_ASSERT(capacity > 0);
    auto garbageBytes = a2.getGarbageByteSize();
    TEST_ASSERT(garbageBytes > 0);
    MemorySize garbage = a2.garbageCollect();
    TEST_ASSERT(garbage <= 1024*1024);
    // TEST_ASSERT(garbage > 0);
    a2.clear();
    TEST_ASSERT(a2.getSize() == 0);
  }
};

TEST_REGISTER(Allocator);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
