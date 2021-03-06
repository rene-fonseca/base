/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/CommunicationsException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

CommunicationsException::CommunicationsException() noexcept
  : IOException()
{
}

CommunicationsException::CommunicationsException(const char* message) noexcept
  : IOException(message)
{
}

CommunicationsException::CommunicationsException(const Type& type) noexcept
  : IOException(type)
{
}
  
CommunicationsException::CommunicationsException(const char* message, const Type& type) noexcept
  : IOException(message, type)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
