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
class StringOutputStream;

/** Avoid common copying to temporary buffer. */
class _COM_AZURE_DEV__BASE__API UTF8Stringify {
private:

  ConstSpan<char> span;
  Reference<ReferenceCountedAllocator<char> > buffer; // container for String
  char tiny[3 * 8] = {0}; // TAG: disable clear for release build
  
  void setString(const StringOutputStream& src);

  /** Returns true if the tiny buffer is used. */
  inline bool isUsingTiny() const noexcept
  {
    return (span.begin() >= tiny) && (span.end() <= (tiny + getArraySize(tiny)));
  }
public:

  /** Initialize buffer. */
  UTF8Stringify();

  /** Initialize buffer. */
  UTF8Stringify(NullPtr);

  /** Initialize buffer. */
  UTF8Stringify(bool src);

  /** Initialize buffer. */
  UTF8Stringify(int value);
  
  /** Initialize buffer. */
  UTF8Stringify(unsigned int value);
  
  /** Initialize buffer. */
  UTF8Stringify(int64 value);
  
  /** Initialize buffer. */
  UTF8Stringify(uint64 value);

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

  /** Initialize buffer. Include StringOutputStream to get implementation. */
  template<class TYPE>
  UTF8Stringify(const TYPE& src);

  /** Initialize buffer. */
  UTF8Stringify(UTF8Stringify&& move);

  /** Initialize buffer. */
  UTF8Stringify& operator=(UTF8Stringify&& assign);

  /** Forces ownership of the data. */
  void ensureOwnership();

  /** Returns the span. */
  inline const ConstSpan<char>& getSpan() const noexcept
  {
    return span;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
