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

#include <base/string/WideString.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class MemoryStreamImpl;

/**
  Posix formatting.
*/

class _COM_AZURE_DEV__BASE__API Posix {
private:
  
  MemoryStreamImpl* ms = nullptr;
  Posix(const Posix&) = delete;
  Posix& operator=(const Posix&) = delete;
public:

  Posix();
  
  ~Posix();
  
  /** Returns true on success. Use for fast string to double conversion. */
  // will be deprecated once fully implemented
  bool getSeries(const char* src, const char* end, float& d) noexcept;

  /** Returns true on success. Use for fast string to double conversion. */
  // will be deprecated once fully implemented
  bool getSeries(const char* src, const char* end, double& d) noexcept;

  /** Returns true on success. Use for fast string to double conversion. */
  // will be deprecated once fully implemented
  bool getSeries(const char* src, const char* end, long double& d) noexcept;

  /** Returns true on success. */
  static bool toFloat(const char* src, const char* end, float& d) noexcept;

  /** Returns true on success. */
  static bool toDouble(const char* src, const char* end, double& d) noexcept;

  /** Returns true on success. */
  static bool toLongDouble(const char* src, const char* end, long double& d) noexcept;

  /** Returns true on success. */
  static bool toFloat(const wchar* src, const wchar* end, float& d) noexcept;

  /** Returns true on success. */
  static bool toDouble(const wchar* src, const wchar* end, double& d) noexcept;

  /** Returns true on success. */
  static bool toLongDouble(const wchar* src, const wchar* end, long double& d) noexcept;

  /** Returns true on success. */
  static bool toFloat(const ucs4* src, const ucs4* end, float& d) noexcept;

  /** Returns true on success. */
  static bool toDouble(const ucs4* src, const ucs4* end, double& d) noexcept;

  /** Returns true on success. */
  static bool toLongDouble(const ucs4* src, const ucs4* end, long double& d) noexcept;

  /** Returns true on success. */
  static inline bool toFloat(const String& text, float& value) noexcept
  {
    return toFloat(text.native(), text.native() + text.getLength(), value);
  }

  /** Returns true on success. */
  static inline bool toDouble(const String& text, double& value) noexcept
  {
    return toDouble(text.native(), text.native() + text.getLength(), value);
  }

  /** Returns true on success. */
  static inline bool toLongDouble(const String& text, long double& value) noexcept
  {
    return toLongDouble(text.native(), text.native() + text.getLength(), value);
  }

  /** Returns true on success. */
  static inline bool toFloat(const WideString& text, float& value) noexcept
  {
    return toFloat(text.native(), text.native() + text.getLength(), value);
  }

  /** Returns true on success. */
  static inline bool toDouble(const WideString& text, double& value) noexcept
  {
    return toDouble(text.native(), text.native() + text.getLength(), value);
  }

  /** Returns true on success. */
  static inline bool toLongDouble(const WideString& text, long double& value) noexcept
  {
    return toLongDouble(text.native(), text.native() + text.getLength(), value);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
