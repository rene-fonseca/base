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
#include <base/mem/Heap.h>
#include <base/OperatingSystem.h>
#include <base/concurrency/AtomicCounter.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <stdlib.h>
#  include <malloc/malloc.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
namespace internal {
  namespace specific {
    extern OperatingSystem::Handle processHeap;
  };
};
#endif // flavor

void* HeapImpl::allocate(MemorySize size) throw(MemoryException)
{
  Profiler::pushObjectCreate(size);
  
  void* result = nullptr;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  result = static_cast<void*>(::HeapAlloc(internal::specific::processHeap, 0, size));
  if ((!result) && (size != 0)) { // was memory allocated
    throw bindCause(MemoryException("Unable to allocate heap.", Type::getType<HeapImpl>()), ::GetLastError());
  }
#else // unix
  result = ::malloc(size);
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to allocate heap.", Type::getType<HeapImpl>());
  }
#endif // flavor
  return result;
}

void* HeapImpl::resize(void* heap, MemorySize size) throw(MemoryException)
{
  if (heap) {
    auto originalSize = getSize(heap);
    if (size != originalSize) {
      Profiler::pushObjectDestroy(originalSize);
      Profiler::pushObjectCreate(size);
    }
  } else {
    Profiler::pushObjectCreate(size);
  }

  void* result = nullptr;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // is serialization enabled for the heap object returned by GetProcessHeap
  if (heap) {
    if (size) {
      // do we get inplace resize for free or do we have to do this explicitly
      result = static_cast<void*>(::HeapReAlloc(internal::specific::processHeap, 0, heap, size));
    } else {
      if (!::HeapFree(internal::specific::processHeap, 0, heap)) {
        throw bindCause(MemoryException("Unable to resize heap.", Type::getType<HeapImpl>()), ::GetLastError());
      }
      result = nullptr;
    }
  } else {
    result = static_cast<void*>(::HeapAlloc(internal::specific::processHeap, 0, size));
  }
  if ((!result) && (size != 0)) { // was memory allocated
    throw bindCause(MemoryException("Unable to resize heap.", Type::getType<HeapImpl>()), ::GetLastError());
  }
#else // unix
  if (size) {
    result = realloc(heap, size);
  } else {
    free(heap);
    result = nullptr;
  }
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to resize heap.", Type::getType<HeapImpl>());
  }
#endif // flavor
  return result;
}

namespace {

  PreferredAtomicCounter totalResizes;
  PreferredAtomicCounter totalMemory;
}

void* HeapImpl::tryResize(void* heap, MemorySize size) throw(MemoryException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (heap) {
    auto originalSize = Profiler::isEnabled() ? getSize(heap) : 0;
    if (size) {
      void* result = static_cast<void*>(::HeapReAlloc(internal::specific::processHeap, HEAP_REALLOC_IN_PLACE_ONLY, heap, size));
      if (result) {
        ++totalResizes;
        totalMemory += size;
        
        if (size != originalSize) {
          Profiler::pushObjectDestroy(originalSize);
          Profiler::pushObjectCreate(size);
        }
      }
      return result;
    } else {
      Profiler::pushObjectDestroy(originalSize);

      if (!::HeapFree(internal::specific::processHeap, 0, heap)) {
        throw bindCause(MemoryException("Unable to resize heap.", Type::getType<HeapImpl>()), ::GetLastError());
      }
      return nullptr;
    }
  } else {
    return nullptr;
  }
#else // unix
  return nullptr;
#endif // flavor
}

void HeapImpl::release(void* heap) throw(MemoryException)
{
  if (heap) {
    Profiler::pushObjectDestroy(getSize(heap));
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::HeapFree(internal::specific::processHeap, 0, heap)) {
    throw bindCause(MemoryException("Unable to release heap.", Type::getType<HeapImpl>()), ::GetLastError());
  }
#else // unix
  free(heap);
#endif // flavor
}

MemorySize HeapImpl::getMinimumSize() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: implement
  return 0;
#else
  return malloc_good_size(1);
#endif
}

MemorySize HeapImpl::getSize(void* heap) noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: implement
  return 0;
#else
  return malloc_size(heap);
#endif
}

void Heap::onLeak(const Type* type, void* buffer, MemorySize size)
{
  static MemorySize total = 0;
  total += size;
  if (type) {
    // TAG: count types separately
  }
  // BASSERT(!"Leaked memory detected.");
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Heap) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_PROJECT("base/mem");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    const MemorySize minimumSize = Heap::getMinimumSize();
    double* heap1 = Heap::allocate<double>(1234);
    TEST_ASSERT(heap1);
    heap1 = Heap::resize(heap1, 2345);
    MemorySize size = Heap::getSize(heap1);
    TEST_ASSERT(size >= 2345);
    unsigned int* heap2 = Heap::resize<unsigned int>(nullptr, 9876);
    TEST_ASSERT(heap2);
    heap1 = Heap::resize(heap1, 3456);
    TEST_ASSERT(heap1);
    heap1 = Heap::resize(heap1, 321);
    TEST_ASSERT(heap1);
    Heap::release(heap1);
    heap2 = Heap::resize(heap2, 0); // release
    TEST_ASSERT(!heap2);
    // Heap::release(heap2);
    bool* heap3 = Heap::resize<bool>(nullptr, 0);
    TEST_ASSERT(!heap3);
  }
};

TEST_REGISTER(Heap);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
