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

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <stdlib.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
namespace internal {
  namespace specific {
    extern OperatingSystem::Handle processHeap;
  };
};
#endif // flavor

void* HeapImpl::allocate(unsigned int size) throw(MemoryException)
{
  void* result = nullptr;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  result = static_cast<void*>(::HeapAlloc(internal::specific::processHeap, 0, size));
  if ((!result) && (size != 0)) { // was memory allocated
    throw bindCause(MemoryException("Unable to allocate heap", Type::getType<HeapImpl>()), ::GetLastError());
  }
#else // unix
  result = ::malloc(size);
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to allocate heap", Type::getType<HeapImpl>());
  }
#endif // flavor
  return result;
}

void* HeapImpl::resize(void* heap, unsigned int size) throw(MemoryException)
{
  void* result = nullptr;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // is serialization enabled for the heap object returned by GetProcessHeap
  if (heap) {
    if (size) {
      result = static_cast<void*>(::HeapReAlloc(internal::specific::processHeap, 0, heap, size));
    } else {
      if (!::HeapFree(internal::specific::processHeap, 0, heap)) {
        throw bindCause(MemoryException("Unable to resize heap", Type::getType<HeapImpl>()), ::GetLastError());
      }
      result = nullptr;
    }
  } else {
    result = static_cast<void*>(::HeapAlloc(internal::specific::processHeap, 0, size));
  }
  if ((!result) && (size != 0)) { // was memory allocated
    throw bindCause(MemoryException("Unable to resize heap", Type::getType<HeapImpl>()), ::GetLastError());
  }
#else // unix
  result = realloc(heap, size);
  if ((!result) && (size != 0)) { // was memory allocated
    throw MemoryException("Unable to resize heap", Type::getType<HeapImpl>());
  }
#endif // flavor
  return result;
}

void* HeapImpl::tryResize(void* heap, unsigned int size) throw(MemoryException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (heap) {
    if (size) {
      return static_cast<void*>(::HeapReAlloc(internal::specific::processHeap, HEAP_REALLOC_IN_PLACE_ONLY, heap, size));
    } else {
      if (!::HeapFree(internal::specific::processHeap, 0, heap)) {
        throw bindCause(MemoryException("Unable to resize heap", Type::getType<HeapImpl>()), ::GetLastError());
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
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::HeapFree(internal::specific::processHeap, 0, heap)) {
    throw bindCause(MemoryException("Unable to release heap", Type::getType<HeapImpl>()), ::GetLastError());
  }
#else // unix
  free(heap);
#endif // flavor
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
