/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Overflow.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Overflow::Overflow() noexcept
{
}

Overflow::Overflow(const char* message) noexcept
  : Exception(message)
{
}

Overflow::Overflow(const Type& type) noexcept
  : Exception(type)
{
}

Overflow::Overflow(const char* message, const Type& type) noexcept
  : Exception(message, type)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
