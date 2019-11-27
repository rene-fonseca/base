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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
