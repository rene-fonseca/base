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

#include <base/Primitives.h>
#include <base/mem/Span.h>
#include <base/mem/ReferenceCountedAllocator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Literal;
class WideLiteral;
class String;
class WideString;
class AnyValue;
class FormatOutputStream;

/** Avoid common copying to temporary buffer. */
class _COM_AZURE_DEV__BASE__API UTF8Stringify {
private:

  ConstSpan<char> span;
  Reference<ReferenceCountedAllocator<char> > buffer; // container for String
  union {
    char tiny[8];
    uint64 reserved;
  };

  void setString(const String& src);
  void setAnyValue(const AnyValue& src);
public:

  /** Initialize buffer. */
  UTF8Stringify(char src);

  /** Initialize buffer. */
  UTF8Stringify(wchar src);

  /** Initialize buffer. */
  UTF8Stringify(ucs4 src);

  /** Initialize buffer. */
  UTF8Stringify(const char* src);

  /** Initialize buffer. */
  UTF8Stringify(const Literal& src);

  /** Initialize buffer. */
  UTF8Stringify(const wchar* src);

  /** Initialize buffer. */
  UTF8Stringify(const WideLiteral& src);

  /** Initialize buffer. */
  UTF8Stringify(const ucs4* src);

  /** Initialize buffer. */
  UTF8Stringify(const String& src);

  /** Initialize buffer. */
  UTF8Stringify(const WideString& src);

  /** Initialize buffer. */
  UTF8Stringify(const AnyValue& src);

  /** Initialize buffer. */
  UTF8Stringify(FormatOutputStream& src);

  /** Initialize buffer. */
  template<class TYPE>
  UTF8Stringify(const TYPE& src);
  
  /** Returns the span. */
  inline const ConstSpan<char>& getSpan() const noexcept
  {
    return span;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
