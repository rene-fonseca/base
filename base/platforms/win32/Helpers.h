/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  OSString for mapping strings to the OS specific string.
*/
class OSString {
private:

  ToWCharString buffer;
public:

  /** Empty string. */
  inline OSString() noexcept
  {
  }

  /** UTF-8. */
  inline OSString(const String& s) noexcept
    : buffer(s)
  {
  }

  inline OSString(const WideString& s) noexcept
    : buffer(s)
  {
  }

  /** UTF-8. */
  inline OSString(const std::string& s) noexcept
    : buffer(s)
  {
  }

  /** UTF-8. */
  inline OSString(const char* s) noexcept
    : buffer(s)
  {
  }

  inline operator wchar* () noexcept
  {
    return const_cast<wchar*>(buffer.native());
  }

  inline operator const wchar* () const noexcept
  {
    return buffer;
  }

  inline bool empty() const noexcept
  {
    return buffer.getLength() == 0;
  }

  inline size_t size() const noexcept
  {
    return buffer.getLength();
  }

  inline size_t getLength() const noexcept
  {
    return buffer.getLength();
  }

  inline operator bool() const noexcept
  {
    return buffer.getLength() > 0;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
