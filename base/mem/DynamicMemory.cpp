/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/mem/DynamicMemory.h>
#include <base/OperatingSystem.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <stdlib.h>
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
class DynamicMemoryImpl {
public:

  static OperatingSystem::Handle processHeap;
};

// must be initialized very early
OperatingSystem::Handle DynamicMemoryImpl::processHeap = ::GetProcessHeap();
#endif

void* DynamicMemory::allocate(unsigned int size) throw() {
  void* result;
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  result = static_cast<void*>(::HeapAlloc(DynamicMemoryImpl::processHeap, 0, size));
#else // unix
  result = malloc(size); // unspecified behavior if size is 0
#endif // flavour
  return result;
}

bool DynamicMemory::release(void* memory) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return (memory == 0) || (::HeapFree(DynamicMemoryImpl::processHeap, 0, memory) != 0);
#else // unix
  free(memory); // works with 0 pointer
  return true;
#endif // flavour
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
