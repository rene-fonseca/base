/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/MultibyteException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

MultibyteException::MultibyteException() noexcept
{
}

MultibyteException::MultibyteException(const char* message) noexcept
  : Exception(message)
{
}

MultibyteException::MultibyteException(const Type& type) noexcept
  : Exception(type)
{
}
  
MultibyteException::MultibyteException(const char* message, const Type& type) noexcept
  : Exception(message, type)
{
}

MultibyteException::MultibyteException(const char* message, const Type& type, unsigned int cause, MemoryDiff octetIndex, MemoryDiff suboctetIndex) noexcept
  : Exception(message, type)
{
  setCause(cause);
  setOctetIndex(octetIndex);
  setSuboctetIndex(suboctetIndex);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
