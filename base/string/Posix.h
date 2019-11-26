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
#include <sstream>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Posix formatting.

  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Posix {
private:
  
  class _COM_AZURE_DEV__BASE__API MemoryBuffer : public std::streambuf {
  public:

    MemoryBuffer(const char* src, const char* end);

    void reset(const char* src, const char* end);
  };

  class _COM_AZURE_DEV__BASE__API MemoryStream : virtual public MemoryBuffer, public std::istream {
  public:

    MemoryStream(const char* src = nullptr, const char* end = nullptr);
    
    void setLocale(const std::locale& _Loc);
  };

  MemoryStream ms;
public:

  Posix();

  /** Returns true on success. Use for fast string to double conversion. */
  bool getSeries(const char* src, const char* end, float& d);

  /** Returns true on success. Use for fast string to double conversion. */
  bool getSeries(const char* src, const char* end, double& d);

  /** Returns true on success. Use for fast string to double conversion. */
  bool getSeries(const char* src, const char* end, long double& d);

  /** Returns true on success. */
  static bool toFloat(const char* src, const char* end, float& d);
  static bool toDouble(const char* src, const char* end, double& d);
  static bool toLongDouble(const char* src, const char* end, long double& d);

  /** Returns true on success. */
  static inline bool toFloat(const std::string& text, float& value)
  {
    return toFloat(text.c_str(), text.c_str() + text.size(), value);
  }

  /** Returns true on success. */
  static inline bool toFloat(const String& text, float& value)
  {
    return toFloat(text.native(), text.native() + text.getLength(), value);
  }

  /** Returns true on success. */
  static inline bool toDouble(const std::string& text, double& value)
  {
    return toDouble(text.c_str(), text.c_str() + text.size(), value);
  }

  /** Returns true on success. */
  static inline bool toDouble(const String& text, double& value)
  {
    return toDouble(text.native(), text.native() + text.getLength(), value);
  }

  /** Returns true on success. */
  static inline bool toLongDouble(const std::string& text, long double& value)
  {
    return toLongDouble(text.c_str(), text.c_str() + text.size(), value);
  }

  /** Returns true on success. */
  static inline bool toLongDouble(const String& text, long double& value)
  {
    return toLongDouble(text.native(), text.native() + text.getLength(), value);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
