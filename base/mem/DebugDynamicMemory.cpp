/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Primitives.h>
#include <base/Base.h>
#include <base/mem/DebugDynamicMemory.h>
#include <base/OperatingSystem.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <stdlib.h>
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

SpinLock DebugDynamicMemory::spinLock; // init before all other objects

class DynamicMemoryImpl {
public:

  static OperatingSystem::Handle processHeap;
};

class DebugDynamicMemoryImpl {
public:

  DebugDynamicMemoryImpl() throw() {
    DebugDynamicMemory::currentAllocations = 0;
  }
  
  ~DebugDynamicMemoryImpl() throw() {
    
    DebugDynamicMemory::currentAllocations;
    
    // output currentAllocations
  }
};

void* DebugDynamicMemory::allocate(unsigned int size) throw() {
  unsigned long long newSize = sizeof(Descriptor) + sizeof(unsigned int) * ((size+sizeof(unsigned int)-1)/sizeof(unsigned int) + PREFIX_WORDS + SUFFIX_WORDS);
  if (newSize > UnsignedInt::MAXIMUM) {
    return 0;
  }

  unsigned int* result;
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  result = static_cast<unsigned int*>(::HeapAlloc(DynamicMemoryImpl::processHeap, 0, newSize));
#else // unix
  result = static_cast<unsigned int*>(malloc(newSize)); // unspecified behavior if size is 0
#endif // flavour  
  //assert(result % sizeof(unsigned int) == 0, MemoryCorruption());

  Descriptor* descriptor = pointer_cast<Descriptor*>(result);
  descriptor->magic = 0; //&allocate;
  descriptor->allocated = true;
  descriptor->size = size;
  
  unsigned int* prefix = pointer_cast<unsigned int*>(pointer_cast<char*>(result) + sizeof(descriptor));
  unsigned int* user = prefix + PREFIX_WORDS;

  // fill prefix
  {
    unsigned int* dest = prefix;
    const unsigned int* end = user;
    for (; dest < end; ++dest) {
      *dest = pointer_cast<char*>(dest) - static_cast<char*>(0);
    }
  }
  
  // fill suffix
  {
    unsigned int* dest = user + size/sizeof(unsigned int);
    if ((size % sizeof(unsigned int)) != 0) {
      *dest = pointer_cast<char*>(dest) - static_cast<char*>(0);
    }
    const unsigned int* end = dest + SUFFIX_WORDS;
    for (; dest < end; ++dest) {
      *dest = pointer_cast<char*>(dest) - static_cast<char*>(0);
    }
  }

  spinLock.exclusiveLock();
  ++currentAllocations;
  spinLock.releaseLock();
  
  return user;
}

bool DebugDynamicMemory::release(void* memory) throw(MemoryCorruption) {
  if (!memory) {
    return true;
  }
  
  // check alignment
  assert((pointer_cast<char*>(memory) - static_cast<char*>(0)) % sizeof(unsigned int) == 0, MemoryCorruption());
  unsigned int* prefix = static_cast<unsigned int*>(memory) - PREFIX_WORDS;

  // check prefix words
  {
    const unsigned int* src = prefix;
    const unsigned int* end = src + PREFIX_WORDS;
    for (; src < end; ++src) {
      if (*src != (pointer_cast<const char*>(src) - static_cast<const char*>(0))) {
        throw MemoryCorruption();
      }
    }
  }

  unsigned int size; // size of user block
  Descriptor* descriptor = pointer_cast<Descriptor*>(pointer_cast<char*>(prefix) - sizeof(Descriptor));
  
  // check descriptor
  {
    assert(descriptor->magic == 0/*&allocate*/, MemoryCorruption());
    spinLock.exclusiveLock(); // prevent multiple deletions
    unsigned int allocated = descriptor->allocated;
    descriptor->allocated = false;
    spinLock.releaseLock();
    assert(allocated, MemoryCorruption());
    size = descriptor->size;
  }
  
  // check suffix words
  {
    const unsigned int* src = prefix + PREFIX_WORDS + size/sizeof(unsigned int);
    if (size % sizeof(unsigned int) != 0) {
      unsigned int mask = static_cast<unsigned int>(-1);
      for (unsigned int i = 0; i < (size % sizeof(unsigned int)); ++i) {
        pointer_cast<char*>(&mask)[i] = 0; // filter out dont cares
      }
      assert((*src & mask) == ((pointer_cast<const char*>(src) - static_cast<const char*>(0)) & mask), MemoryCorruption());
      ++src;
    }
    const unsigned int* end = src + SUFFIX_WORDS;
    for (; src < end; ++src) {
      if (*src != (pointer_cast<const char*>(src) - static_cast<const char*>(0))) {
        throw MemoryCorruption();
      }
    }
  }
  
  spinLock.exclusiveLock();
  --currentAllocations;
  spinLock.releaseLock();

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return ::HeapFree(DynamicMemoryImpl::processHeap, 0, memory);
#else // unix
  free(memory); // works with 0 pointer
  return true;
#endif // flavour
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
