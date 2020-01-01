/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/NullPointer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

NullPointer::NullPointer() noexcept
{
}

NullPointer::NullPointer(const char* message) noexcept
  : MemoryException(message)
{
}

NullPointer::NullPointer(const Type& type) noexcept
  : MemoryException(type)
{
}

NullPointer::NullPointer(const char* message, const Type& type) noexcept
  : MemoryException(message, type)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
