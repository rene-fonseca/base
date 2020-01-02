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

#include <base/AnyValue.h>
#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Alias for StringOutputStream for shorthand formatting. */
typedef StringOutputStream format;

/**
  String formatting support.

  String result = Format::subst(MESSAGE("My name is %1 %2."), "John", "Doe");

  You can do inline value formatting like this (hint format() is a StringOutputStream):

  String result = Format::subst(
    MESSAGE("My value is %1."),
    format() << HEX << myValue, format() << ZEROPAD << setWidth(8) << 5432
  );

  You can also reuse to format object as a shorthand because each argument triggers automatic restart of the stream.
  format f;
  String result = Format::subst(
    MESSAGE("My value is %1 and %2."),
    f << HEX << myValue,
    f << ZEROPAD << setWidth(8) << 5432
  );
*/

class _COM_AZURE_DEV__BASE__API Format {
public:

  /** The maximum supported arguments. */
  static constexpr unsigned int MAX_ARGS = 16;
  
  /**
    Simple string substitution.

    String text = Format::subst(MESSAGE("My name is %1 and my last name is %2."), {"John", "Doe"});
  */
  static String subst(const String& text);
  static String subst(const String& text, const String& a);
  static String subst(const String& text, const String& a, const String& b);
  static String subst(const String& text, const String& a, const String& b, const String& c);
  static String subst(const String& text, const String& a, const String& b, const String& c, const String& d);
  static String subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e);
  static String subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f);
  static String subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g);
  static String subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h);
  static String subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h, const String& i);
  static String subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h, const String& i, const String& j);
  static String subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h, const String& i, const String& j, const String& k);
  static String subst(const String& text, const String& a, const String& b, const String& c, const String& d, const String& e, const String& f, const String& g, const String& h, const String& i, const String& j, const String& k, const String& l);
  static String subst(const String& text, const AnyValue& a);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h, const AnyValue& i);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h, const AnyValue& i, const AnyValue& j);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h, const AnyValue& i, const AnyValue& j, const AnyValue& k);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h, const AnyValue& i, const AnyValue& j, const AnyValue& k, const AnyValue& l);

#if 0 // used for older GCC
  static inline String subst(const char* text)
  {
    return subst(String(text));
  }

  static inline String subst(const char* text, const String& a)
  {
    return subst(String(text), a);
  }
  
  static inline String subst(const char* text, const String& a, const String& b)
  {
    return subst(String(text), a, b);
  }
#endif

  static String subst(const String& text, std::initializer_list<const char*> list);
  static String subst(const String& text, std::initializer_list<Literal> list);

  /** String substitution implementation. */
  class _COM_AZURE_DEV__BASE__API Subst {
  private:

    const String& text;
    const MemorySpan* args = nullptr;
    MemorySize numberOfArgs = 0;
  public:

    Subst(const String& text, const MemorySpan* args, MemorySize numberOfArgs);

    inline MemorySize getNumberOfArgs() const noexcept
    {
      return numberOfArgs;
    }

    inline const MemorySpan& getArg(MemorySize i) const
    {
      if (!INLINE_ASSERT(i < numberOfArgs)) {
        _throw OutOfRange();
      }
      return args[i];
    }

    /** Returns the substituted string. */
    String format() const;

    /** Write to stream. */
    FormatOutputStream& operator<<(FormatOutputStream& stream) const;
  };
};

FormatOutputStream& operator<<(FormatOutputStream& stream, const Format::Subst& subst);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
