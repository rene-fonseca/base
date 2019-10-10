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
#include <base/mem/DebugDynamicMemory.h>
#include <base/Primitives.h>
#include <base/OperatingSystem.h>
#include <base/Base.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <stdlib.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
namespace internal {
  namespace specific {
    extern OperatingSystem::Handle processHeap;
  };
};
#endif // flavor

void* DebugDynamicMemory::allocate(unsigned int size) throw() {
  unsigned long long newSize = sizeof(Descriptor) + sizeof(unsigned int) * ((size+sizeof(unsigned int)-1)/sizeof(unsigned int) + PREFIX_WORDS + SUFFIX_WORDS);
  if (newSize > PrimitiveTraits<unsigned int>::MAXIMUM) {
    return 0;
  }

  unsigned int* result = nullptr;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  result = static_cast<unsigned int*>(::HeapAlloc(internal::specific::processHeap, 0, newSize));
#else // unix
  result = static_cast<unsigned int*>(malloc(newSize)); // unspecified behavior if size is 0
#endif // flavor  
  //assert(result % sizeof(unsigned int) == 0, MemoryCorruption(Type::getType<DebugDynamicMemory>()));

  Descriptor* descriptor = Cast::pointer<Descriptor*>(result);
  descriptor->magic = 0; //&allocate;
  descriptor->allocated = true;
  descriptor->size = size;
  
  unsigned int* prefix = Cast::pointer<unsigned int*>(Cast::pointer<char*>(result) + sizeof(descriptor));
  unsigned int* user = prefix + PREFIX_WORDS;

  // fill prefix
  {
    unsigned int* dest = prefix;
    const unsigned int* end = user;
    for (; dest < end; ++dest) {
      *dest = Cast::getOffset(dest);
    }
  }
  
  // fill suffix
  {
    unsigned int* dest = user + size/sizeof(unsigned int);
    if ((size % sizeof(unsigned int)) != 0) {
      *dest = Cast::getOffset(dest);
    }
    const unsigned int* end = dest + SUFFIX_WORDS;
    for (; dest < end; ++dest) {
      *dest = Cast::getOffset(dest);
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
  bassert(
    (Cast::getOffset(memory) % sizeof(unsigned int)) == 0,
    MemoryCorruption(Type::getType<DebugDynamicMemory>())
  );
  unsigned int* prefix = static_cast<unsigned int*>(memory) - PREFIX_WORDS;

  // check prefix words
  {
    const unsigned int* src = prefix;
    const unsigned int* end = src + PREFIX_WORDS;
    for (; src < end; ++src) {
      if (*src != (Cast::getOffset(src))) {
        throw MemoryCorruption(Type::getType<DebugDynamicMemory>());
      }
    }
  }

  unsigned int size; // size of user block
  Descriptor* descriptor = Cast::pointer<Descriptor*>(Cast::pointer<char*>(prefix) - sizeof(Descriptor));
  
  // check descriptor
  {
    bassert(
      descriptor->magic == 0/*&allocate*/,
      MemoryCorruption(Type::getType<DebugDynamicMemory>())
    );
    spinLock.exclusiveLock(); // prevent multiple deletions
    unsigned int allocated = descriptor->allocated;
    descriptor->allocated = false;
    spinLock.releaseLock();
    bassert(allocated, MemoryCorruption(Type::getType<DebugDynamicMemory>()));
    size = descriptor->size;
  }
  
  // check suffix words
  {
    const unsigned int* src = prefix + PREFIX_WORDS + size/sizeof(unsigned int);
    if (size % sizeof(unsigned int) != 0) {
      unsigned int mask = static_cast<unsigned int>(-1);
      for (unsigned int i = 0; i < (size % sizeof(unsigned int)); ++i) {
        Cast::pointer<char*>(&mask)[i] = 0; // filter out dont cares
      }
      bassert(
        (*src & mask) == (Cast::getOffset(src) & mask),
        MemoryCorruption(Type::getType<DebugDynamicMemory>())
      );
      ++src;
    }
    const unsigned int* end = src + SUFFIX_WORDS;
    for (; src < end; ++src) {
      if (*src != Cast::getOffset(src)) {
        throw MemoryCorruption(Type::getType<DebugDynamicMemory>());
      }
    }
  }
  
  spinLock.exclusiveLock();
  --currentAllocations;
  spinLock.releaseLock();

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return ::HeapFree(internal::specific::processHeap, 0, memory);
#else // unix
  ::free(memory); // works with 0 pointer
  return true;
#endif // flavor
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
