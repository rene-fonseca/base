/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileNotFound.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FileNotFound::FileNotFound() noexcept
{
}

FileNotFound::FileNotFound(const char* message) noexcept
  : FileException(message)
{
}

FileNotFound::FileNotFound(const Type& type) noexcept
  : FileException(type)
{
}

FileNotFound::FileNotFound(const char* message, const Type& type) noexcept
  : FileException(message, type)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
