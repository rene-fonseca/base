/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Underflow.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Underflow::Underflow() noexcept
{
}

Underflow::Underflow(const char* message) noexcept
  : Exception(message)
{
}

Underflow::Underflow(const Type& type) noexcept
  : Exception(type)
{
}

Underflow::Underflow(const char* message, const Type& type) noexcept
  : Exception(message, type)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
