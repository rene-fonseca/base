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

#include <base/Base.h>
#include <sstream>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Posix formatting.

  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Posix {
private:
  
  class MemoryBuffer : public std::streambuf {
  public:

    MemoryBuffer(const char* src, const char* end);

    void reset(const char* src, const char* end);
  };

  class MemoryStream : virtual public MemoryBuffer, public std::istream {
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
  static bool toDouble(const char* src, const char* end, double& d);

  /** Returns true on success. */
  static inline bool toDouble(const std::string& text, double& d)
  {
    return toDouble(text.c_str(), text.c_str() + text.size(), d);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
