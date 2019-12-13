/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/NetworkException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

NetworkException::NetworkException() noexcept
{
}

NetworkException::NetworkException(const char* message) noexcept
  : IOException(message)
{
}
  
NetworkException::NetworkException(const Type& type) noexcept
  : IOException(type)
{
}
  
NetworkException::NetworkException(const char* message, const Type& type) noexcept
  : IOException(message, type)
{
}

NetworkException::NetworkException(const char* message, const Type& type, unsigned int cause) noexcept
  : IOException(message, type)
{
  setCause(cause);
}

NetworkException::NetworkException(const char* message, const Type& type, unsigned int error, int) noexcept
  : IOException(message, type)
{
  setError(error);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
