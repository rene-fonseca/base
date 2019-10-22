/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/AtomicCounter.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0 && defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD)
template _COM_AZURE_DEV__BASE__API class AtomicCounter<char>;
template _COM_AZURE_DEV__BASE__API class AtomicCounter<short>;
template _COM_AZURE_DEV__BASE__API class AtomicCounter<int>;
template _COM_AZURE_DEV__BASE__API class AtomicCounter<long>;
template _COM_AZURE_DEV__BASE__API class AtomicCounter<long long>;
template _COM_AZURE_DEV__BASE__API class AtomicCounter<MemoryDiff>;

template _COM_AZURE_DEV__BASE__API class AtomicCounterU<uint8>;
template _COM_AZURE_DEV__BASE__API class AtomicCounterU<uint16>;
template _COM_AZURE_DEV__BASE__API class AtomicCounterU<uint32>;
template _COM_AZURE_DEV__BASE__API class AtomicCounterU<uint64>;
template _COM_AZURE_DEV__BASE__API class AtomicCounterU<MemorySize>;
#endif

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(AtomicCounter) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    AtomicCounterU<MemorySize> counter(0);
    TEST_ASSERT(static_cast<MemorySize>(counter) == 0);
    ++counter;
    TEST_ASSERT(static_cast<MemorySize>(counter) == 1);
    counter++;
    TEST_ASSERT(static_cast<MemorySize>(counter) == 2);
    --counter;
    TEST_ASSERT(static_cast<MemorySize>(counter) == 1);
    counter--;
    TEST_ASSERT(static_cast<MemorySize>(counter) == 0);
    counter += 123;
    TEST_ASSERT(static_cast<MemorySize>(counter) == 123);
    counter -= 123;
    TEST_ASSERT(static_cast<MemorySize>(counter) == 0);
    MemorySize value = counter;
    counter = 1234;
    counter.exchange(4321);
    TEST_ASSERT(static_cast<MemorySize>(counter) == 4321);
    MemorySize expected = 4321;
    counter.compareAndExchange(expected, 1234);
    TEST_ASSERT(static_cast<MemorySize>(counter) == 1234);
    expected = 9876;
    counter.compareAndExchangeWeak(expected, 4321);
    TEST_ASSERT(static_cast<MemorySize>(expected) == 1234);
  }
};

TEST_REGISTER(AtomicCounter);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
