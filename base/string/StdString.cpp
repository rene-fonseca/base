/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/StdString.h>
#include <locale>
#include <codecvt>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

std::string StdString::toUTF8(const String& s)
{
  return std::string(s.getElements(), s.getLength());
}

std::string StdString::toUTF8(const WideString& s)
{
  std::string result;
  if (s.isEmpty()) {
    return result;
  }
  const MemoryDiff length = Unicode::UCS4ToUTF8(nullptr, s.getElements(), s.getLength());
  if (length < 0) {
    BASSERT(!"Invalid UCS4 string.");
    return result; // throw
  }
  result.resize(length);
  Unicode::UCS4ToUTF8(reinterpret_cast<uint8*>(&result[0]), s.getElements(), s.getLength());
  return result;
}

std::string StdString::toUTF8(const std::wstring& s)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.to_bytes(s.c_str(), s.c_str() + s.size());
}

std::string StdString::toUTF8(const wchar* s)
{
  if (s) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
    return convert.to_bytes(s);
  } else {
    return std::string();
  }
}

std::string StdString::toUTF8(const wchar* s, MemorySize length)
{
  if (s) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
    return convert.to_bytes(s, s + length);
  } else {
    return std::string();
  }
}

std::wstring StdString::toWide(const String& s)
{
#if 1
  std::wstring result;
  if (s.isEmpty()) {
    return result;
  }
  const MemoryDiff length = Unicode::UTF8ToWChar(nullptr, s.getBytes(), s.getLength());
  if (length < 0) {
    BASSERT(!"Invalid UTF-8 string.");
    return result; // throw
  }
  result.resize(length);
  Unicode::UTF8ToWChar(&result[0], s.getBytes(), s.getLength());
  return result;
#else
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.from_bytes(s.getElements(), s.getElements() + s.getLength());
#endif
}

std::wstring StdString::toWide(const WideString& s)
{
  std::wstring result;
  if (s.isEmpty()) {
    return result;
  }
  const MemoryDiff length = Unicode::UCS4ToWChar(nullptr, s.getElements(), s.getLength());
  if (length < 0) {
    BASSERT(!"Invalid UCS4 string.");
    return result; // throw
  }
  result.resize(length);
  Unicode::UCS4ToWChar(&result[0], s.getElements(), s.getLength());
  return result;
}

std::wstring StdString::toWide(const char* s)
{
#if 1
  std::wstring result;
  if (!s) {
    return result;
  }
  const MemorySize nativeLength = getNullTerminatedLength(s);
  if (!nativeLength) {
    return result;
  }
  MemoryDiff length = Unicode::UTF8ToWChar(nullptr, reinterpret_cast<const uint8*>(s), nativeLength);
  if (length < 0) {
    BASSERT(!"Invalid UTF-8 string.");
    return result; // throw
  }
  result.resize(length);
  Unicode::UTF8ToWChar(&result[0], reinterpret_cast<const uint8*>(s), nativeLength);
  return result;
#else
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.from_bytes(s);
#endif
}

std::wstring StdString::toWide(const char* s, MemorySize size)
{
  std::wstring result;
  if (!s || (size == 0)) {
    return result;
  }
  MemoryDiff length = Unicode::UTF8ToWChar(nullptr, reinterpret_cast<const uint8*>(s), size);
  if (length < 0) {
    BASSERT(!"Invalid UTF-8 string.");
    return result; // throw
  }
  result.resize(length);
  Unicode::UTF8ToWChar(&result[0], reinterpret_cast<const uint8*>(s), size);
  return result;
}

std::wstring StdString::toWide(const std::string& s)
{
#if 1
  std::wstring result;
  if (s.empty()) {
    return result;
  }
  MemoryDiff length = Unicode::UTF8ToWChar(nullptr, reinterpret_cast<const uint8*>(s.c_str()), s.size());
  if (length < 0) {
    BASSERT(!"Invalid UTF-8 string.");
    return result; // throw
  }
  result.resize(length);
  Unicode::UTF8ToWChar(&result[0], reinterpret_cast<const uint8*>(s.c_str()), s.size());
  return result;
#else
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
  return convert.from_bytes(s.c_str(), s.c_str() + s.size());
#endif
}

String StdString::getString(const std::string& string)
{
  return String(string.c_str(), string.size());
}

String StdString::getString(const std::wstring& string)
{
  return String(string.c_str(), string.size());
}

WideString StdString::getWideString(const std::string& string)
{
  return WideString(string.c_str(), string.size());
}

WideString StdString::getWideString(const std::wstring& string)
{
  return WideString(string.c_str(), string.size());
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
