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

/**
  Converts between std string and Base strings.
*/

class _COM_AZURE_DEV__BASE__API StdString {
public:

  /** Converts String to UTF-8 string. */
  static std::string toUTF8(const String& s);

  /** Converts WideString to UTF-8 String. */
  static std::string toUTF8(const WideString& s);

  /** Converts const wchar* to UTF-8 string. */
  static std::string toUTF8(const wchar* s);

  /** Converts const wchar* to UTF-8 string. */
  static std::string toUTF8(const wchar* s, MemorySize length);

  /** Converts wstring to UTF-8 string. */
  static std::string toUTF8(const std::wstring& s);



  /** Converts String to wstring. */
  static std::wstring toWide(const String& s);

  /** Converts String to wstring. */
  static std::wstring toWide(const WideString& s);

  /** Converts UTF-8 (const char*) to wstring. */
  static std::wstring toWide(const char* s);

  /** Converts UTF-8 (const char*) to wstring. */
  static std::wstring toWide(const char* s, MemorySize length);

  /** Converts UTF-8 string to wstring. */
  static std::wstring toWide(const std::string& s);
  
  
  
  static String getString(const std::string& string);
  static String getString(const std::wstring& string);
  static WideString getWideString(const std::string& string);
  static WideString getWideString(const std::wstring& string);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
