/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/ReadWriteLockException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ReadWriteLockException::ReadWriteLockException() noexcept
{
}

ReadWriteLockException::ReadWriteLockException(const char* message) noexcept
  : LockException(message)
{
}

ReadWriteLockException::ReadWriteLockException(const Type& type) noexcept
  : LockException(type)
{
}

ReadWriteLockException::ReadWriteLockException(const char* message, const Type& type) noexcept
  : LockException(message, type)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
