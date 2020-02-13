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

#include <base/string/StringOutputStream.h>
#include <base/string/UTF8Stringify.h>
#include <base/AnyValue.h>

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

  /**
    Simple string substitution.
  */
  static String substImpl(const UTF8Stringify& text, const UTF8Stringify* args, MemorySize numberOfArgs);

  /** Substitutes the given string with the arguments. */
  template<typename... ARGS>
  static String subst(const UTF8Stringify& text, ARGS&&... args)
  {
    const UTF8Stringify strings[] = { UTF8Stringify(std::forward<ARGS>(args))... }; // has use-local on buffer
    return substImpl(text, strings, getArraySize(strings));
  }

  /** The maximum supported arguments. */
  static constexpr unsigned int MAX_ARGS = 64;

#if 0
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
#endif

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

#if 0
  /** String substitution. */
  static String subst(const String& text, const std::initializer_list<const char*>& list);
  /** String substitution. */
  static String subst(const String& text, const std::initializer_list<Literal>& list);
  /** String substitution. */
  static String subst(const String& text, const std::initializer_list<String>& list);
#endif

  /** String substitution implementation. */
  class _COM_AZURE_DEV__BASE__API Subst {
  private:

    const ConstSpan<char> text;
    const ConstSpan<char>* args = nullptr;
    MemorySize numberOfArgs = 0;
  public:

    /** Initializes substitution. */
    // Subst(const UTF8Stringify& text, const UTF8Stringify* args, MemorySize numberOfArgs);

    /** Initializes substitution. */
    Subst(const ConstSpan<char>& text, const ConstSpan<char>* args, MemorySize numberOfArgs);

    /** Returns the number of arguments. */
    inline MemorySize getNumberOfArgs() const noexcept
    {
      return numberOfArgs;
    }

    /** Returns the argument. */
    inline const ConstSpan<char>& getArg(MemorySize i) const
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

/** Write to format stream. */
FormatOutputStream& operator<<(FormatOutputStream& stream, const Format::Subst& subst);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
