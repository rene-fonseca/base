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
#include <base/mem/DynamicMemory.h>
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

void* DynamicMemory::allocate(MemorySize size) throw() {
  void* result = nullptr;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  result = static_cast<void*>(::HeapAlloc(internal::specific::processHeap, 0, size));
#else // unix
  result = ::malloc(size); // unspecified behavior if size is 0
#endif // flavor
  return result;
}

bool DynamicMemory::release(void* memory) throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return (memory == 0) || (::HeapFree(internal::specific::processHeap, 0, memory) != 0);
#else // unix
  ::free(memory); // works with 0 pointer
  return true;
#endif // flavor
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
