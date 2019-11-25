/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/OutOfRange.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#define EXCEPTION_DECLARATION(TYPE, PARENT) \
  TYPE() noexcept; \
  TYPE(const char* message) noexcept; \
  TYPE(const Type& type) noexcept; \
  TYPE(const char* message, const Type& type) noexcept;

#define EXCEPTION_IMPLEMENTATION(TYPE, PARENT) \
  TYPE::TYPE() noexcept {} \
  TYPE::TYPE(const char* message) noexcept : PARENT(message) {} \
  TYPE::TYPE(const Type& type) noexcept : PARENT(type) {} \
  TYPE::TYPE(const char* message, const Type& type) noexcept : PARENT(message, type) {}

OutOfRange::OutOfRange() noexcept
{
}

OutOfRange::OutOfRange(const char* message) noexcept
  : Exception(message)
{
}

OutOfRange::OutOfRange(const Type& type) noexcept
  : Exception(type)
{
}

OutOfRange::OutOfRange(const char* message, const Type& type) noexcept
  : Exception(message, type)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
