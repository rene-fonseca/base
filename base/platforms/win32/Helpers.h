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

  // TAG: avoid any allocation if we are mapping from wchar*/wstring
  std::wstring buffer;
public:

  /** Empty string. */
  inline OSString() noexcept {
  }

  /** UTF-8. */
  inline OSString(const String& s) noexcept : buffer(toWide(s)) {
  }

  /** UTF-8. */
  inline OSString(const std::string& s) noexcept : buffer(toWide(s)) {
  }

  /** UTF-8. */
  inline OSString(const char* s) noexcept : buffer(toWide(s)) {
  }

  inline operator wchar* () noexcept {
    return const_cast<wchar*>(buffer.c_str());
  }

  inline operator const wchar* () const noexcept {
    return buffer.c_str();
  }

  inline bool empty() const noexcept {
    return buffer.empty();
  }

  inline size_t size() const noexcept {
    return buffer.size();
  }

  inline size_t getLength() const noexcept {
    return buffer.size();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
