/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/DynamicMemory.h>
#include <base/mem/Heap.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void* DynamicMemory::allocate(MemorySize size) noexcept
{
  return Heap::allocateNoThrow<uint8>(size);
}

void DynamicMemory::release(void* memory) noexcept
{
  Heap::release(memory);
}

#if 0
void* DynamicMemory::operator new(MemorySize size, void* place) noexcept
{
  BASSERT(size > 0);
  return place;
}

void* DynamicMemory::operator new[](MemorySize size, void* place) noexcept
{
  BASSERT(size > 0);
  return place;
}
#endif

void* DynamicMemory::operator new(MemorySize size)
{
  void* result = Heap::allocateNoThrow<uint8>(size);
  if (!result) {
    _throw MemoryException();
  }
  return result;
}

void DynamicMemory::operator delete(void* memory)
{
  Heap::release(memory);
}

void* DynamicMemory::operator new[](MemorySize size)
{
  void* result = Heap::allocateNoThrow<uint8>(size);
  if (!result) {
    _throw MemoryException();
  }
  return result;
}

void DynamicMemory::operator delete[](void* memory)
{
  Heap::release(memory);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
