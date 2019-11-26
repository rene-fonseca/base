/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/InvalidContainer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

InvalidContainer::InvalidContainer() noexcept
{
}

InvalidContainer::InvalidContainer(const char* message) noexcept
  : ContainerException(message)
{
}

InvalidContainer::InvalidContainer(const Type& type) noexcept
  : ContainerException(type)
{
}

InvalidContainer::InvalidContainer(const char* message, const Type& type) noexcept
  : ContainerException(message, type)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
