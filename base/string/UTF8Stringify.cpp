/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/UTF8Stringify.h>
#include <base/Literal.h>
#include <base/WideLiteral.h>
#include <base/string/String.h>
#include <base/string/Unicode.h>
#include <base/AnyValue.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

UTF8Stringify::UTF8Stringify(char src)
{
  reserved = 0;
  tiny[0] = src;
  span = ConstSpan<char>(tiny, 1);
}

UTF8Stringify::UTF8Stringify(wchar src)
{
  reserved = 0;
  ucs4 ch = src;
  MemorySize length = Unicode::writeUTF8(reinterpret_cast<uint8*>(tiny), ch);
  BASSERT(length <= getArraySize(tiny));
  span = ConstSpan<char>(tiny, length);
/*
  String temp(src);
  buffer = temp.getContainer();
  span = temp.getSpan();
*/
}

UTF8Stringify::UTF8Stringify(ucs4 src)
{
  reserved = 0;
  MemorySize length = Unicode::writeUTF8(reinterpret_cast<uint8*>(tiny), src);
  BASSERT(length <= getArraySize(tiny));
  span = ConstSpan<char>(tiny, length);
/*
  String temp(src);
  buffer = temp.getContainer();
  span = temp.getSpan();
*/
}

UTF8Stringify::UTF8Stringify(const char* src)
  : span(src, getNullTerminatedLength(src))
{
}

UTF8Stringify::UTF8Stringify(const Literal& src)
  : span(src.getValue(), src.getLength())
{
}

UTF8Stringify::UTF8Stringify(const wchar* src)
{
  String temp(src);
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(const WideLiteral& src)
{
  String temp(src.getValue(), src.getLength());
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(const ucs4* src)
{
  String temp(src);
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(const String& src)
{
  String temp(src);
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(const WideString& src)
{
  String temp(src);
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(const AnyValue& src)
{
  String temp(src.getString());
  buffer = temp.getContainer();
  span = temp.getSpan();
}

UTF8Stringify::UTF8Stringify(FormatOutputStream& src)
{
  String temp(src.toString());
  buffer = temp.getContainer();
  span = temp.getSpan();
}

void UTF8Stringify::setString(const String& src)
{
  buffer = src.getContainer();
  span = src.getSpan();
}

// TAG: avoid dependency on AnyValue - by add format functions
#if 0
String toString(int value)
{
}
#endif

void UTF8Stringify::setAnyValue(const AnyValue& src)
{
  String temp(src.getString());
  buffer = temp.getContainer();
  span = temp.getSpan();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
