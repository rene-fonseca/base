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
#include <base/mem/Heap.h>
#include <base/Primitives.h>
#include <base/Base.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void* DebugDynamicMemory::allocate(MemorySize size) noexcept
{
  MemorySize newSize = sizeof(Descriptor) + sizeof(unsigned int) * ((size+sizeof(unsigned int)-1)/sizeof(unsigned int) + PREFIX_WORDS + SUFFIX_WORDS);
  if (newSize > PrimitiveTraits<unsigned int>::MAXIMUM) {
    return 0;
  }

  uint8* result = Heap::allocateNoThrow<uint8>(newSize);
  if (!result) {
    return nullptr;
  }
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

  ++currentAllocations;
  
  return user;
}

void DebugDynamicMemory::release(void* memory) throw(MemoryCorruption)
{
  if (!memory) {
    return;
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

  unsigned int size = 0; // size of user block
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
  
  --currentAllocations;
  
  Heap::release(memory);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
