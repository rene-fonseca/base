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

#if defined(_DEBUG) // TAG: need test symbol

void test() {
  AtomicCounterU<MemorySize> counter(0);
  ++counter;
  counter++;
  --counter;
  counter--;
  counter += 123;
  counter -= 123;
  MemorySize value = counter;
  counter = 1234;
  counter.exchange(4321);
  MemorySize expected = 0;
  counter.compareAndExchange(expected, 1234);
  counter.compareAndExchangeWeak(expected, 4321);
}

#endif

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
