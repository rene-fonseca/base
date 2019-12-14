/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/AmbiguousRegistration.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

AmbiguousRegistration::AmbiguousRegistration() noexcept
{
}

AmbiguousRegistration::AmbiguousRegistration(const char* message) noexcept
  : Exception(message)
{
}

AmbiguousRegistration::AmbiguousRegistration(const Type& type) noexcept
  : Exception(type)
{
}

AmbiguousRegistration::AmbiguousRegistration(const char* message, const Type& type) noexcept
  : Exception(message, type)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
