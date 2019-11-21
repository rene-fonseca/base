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
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#  include <malloc/malloc.h>
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
#  include <malloc.h>
#endif
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
namespace internal {
  namespace specific {
    extern OperatingSystem::Handle processHeap;
  };
};
#endif // flavor

namespace {

  PreferredAtomicCounter totalResizes;
  PreferredAtomicCounter totalMemory;
}

void* HeapImpl::allocateNoThrow(MemorySize size) noexcept
{
  void* result = nullptr;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  result = static_cast<void*>(::HeapAlloc(internal::specific::processHeap, 0, size));
  if (!result && (size != 0)) { // was memory allocated
    return nullptr;
  }
#else // unix
  result = ::malloc(size);
  if (!result && (size != 0)) { // was memory allocated
    return nullptr;
  }
#endif // flavor
  Profiler::pushObjectCreate(reinterpret_cast<MemorySize>(result), size);
  return result;
}

void* HeapImpl::allocate(MemorySize size) throw(MemoryException)
{
  if (!size) {
    return nullptr;
  }
  
  void* result = nullptr;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  result = static_cast<void*>(::HeapAlloc(internal::specific::processHeap, 0, size));
  if (!result && (size != 0)) { // was memory allocated
    throw bindCause(MemoryException("Unable to allocate heap.", Type::getType<HeapImpl>()), ::GetLastError());
  }
#else // unix
  result = ::malloc(size);
  if (!result && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to allocate heap.", Type::getType<HeapImpl>());
  }
#endif // flavor
  Profiler::pushObjectCreate(reinterpret_cast<MemorySize>(result), size);
  return result;
}

MemorySize HeapImpl::getHeapUsed() noexcept
{
  return 0;
}

void* HeapImpl::resize(void* heap, MemorySize size) throw(MemoryException)
{
  MemorySize originalSize = 0;
  const bool profile = Profiler::isEnabled();
  if (heap && profile) {
    originalSize = getSize(heap);
    if (size != originalSize) {
      Profiler::pushObjectDestroy(reinterpret_cast<MemorySize>(heap), originalSize);
    }
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
  } else if (size)  {
    result = static_cast<void*>(::HeapAlloc(internal::specific::processHeap, 0, size));
  }
  if (!result && (size != 0)) { // was memory allocated
    throw bindCause(MemoryException("Unable to resize heap.", Type::getType<HeapImpl>()), ::GetLastError());
  }
#else // unix
  if (size) {
    result = realloc(heap, size);
  } else {
    free(heap);
    result = nullptr;
  }
  if (!result && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to resize heap.", Type::getType<HeapImpl>());
  }
#endif // flavor
  if (result && profile) {
    Profiler::pushObjectCreate(reinterpret_cast<MemorySize>(result), size);
  }
  return result;
}

void* HeapImpl::tryResize(void* heap, MemorySize size) throw(MemoryException)
{
  if (!heap) {
    return nullptr;
  }
  const bool profile = Profiler::isEnabled();
  const auto originalSize = profile ? getSize(heap) : 0;
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (true) {
    if (size) {
      void* result = static_cast<void*>(::HeapReAlloc(internal::specific::processHeap, HEAP_REALLOC_IN_PLACE_ONLY, heap, size));
      if (result) {
        ++totalResizes;
        totalMemory += size;
        
        if (profile && (size != originalSize)) {
          Profiler::pushObjectDestroy(reinterpret_cast<MemorySize>(heap), originalSize);
          Profiler::pushObjectCreate(reinterpret_cast<MemorySize>(result), size);
        }
      }
      return result;
    } else {
      Profiler::pushObjectDestroy(reinterpret_cast<MemorySize>(heap), originalSize);

      if (!::HeapFree(internal::specific::processHeap, 0, heap)) {
        throw bindCause(MemoryException("Unable to resize heap.", Type::getType<HeapImpl>()), ::GetLastError());
      }
      return nullptr;
    }
  }
#else // unix
  if (size == 0) {
    if (profile) {
      Profiler::pushObjectDestroy(reinterpret_cast<MemorySize>(heap), originalSize);
    }
    free(heap);
  }
  return nullptr;
#endif // flavor
}

void HeapImpl::release(void* heap) throw(MemoryException)
{
  if (heap) {
    if (Profiler::isEnabled()) {
      Profiler::pushObjectDestroy(reinterpret_cast<MemorySize>(heap), getSize(heap));
    }
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
  static MemorySize minimumSize = 0;
  if (minimumSize == 0) {
    if (void* temp = HeapAlloc(internal::specific::processHeap, 0, 1)) {
      minimumSize = HeapSize(internal::specific::processHeap, 0, temp);
      HeapFree(internal::specific::processHeap, 0, temp);
    }
  }
  return minimumSize;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  return malloc_good_size(1);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  static MemorySize minimumSize = 0;
  if (minimumSize == 0) {
    if (void* temp = malloc(1)) {
      minimumSize = malloc_usable_size(temp);
      free(temp);
    }
  }
  return minimumSize;
#else
  return 0;
#endif
}

MemorySize HeapImpl::getSize(void* heap) noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return HeapSize(internal::specific::processHeap, 0, heap);
#else
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  return malloc_size(heap);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  return malloc_usable_size(heap);
#else
  return 0;
#endif
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
    
    PrimitiveArray<double> pa1(1024);

    PrimitiveStackArray<uint64> pa2(512);
    TEST_ASSERT(pa2.isUsingStack());

    PrimitiveStackArray<uint8> pa3(1234);
    pa3.resize(4321);
    TEST_ASSERT(pa3.isUsingHeap());
  }
};

TEST_REGISTER(Heap);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
