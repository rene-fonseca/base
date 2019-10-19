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
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Format.
*/

class _COM_AZURE_DEV__BASE__API Format {
public:

  /**
    Simple string substitution.

    String text = Format::subst(MESSAGE("My name is %1 and my last name is %2."), "John", "Doe");
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
  static String subst(const String& text, const AnyValue& a);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g);
  static String subst(const String& text, const AnyValue& a, const AnyValue& b, const AnyValue& c, const AnyValue& d, const AnyValue& e, const AnyValue& f, const AnyValue& g, const AnyValue& h);
  static String subst(const String& text, std::initializer_list<const char*> list);

  /** String substitution implementation. */
  class _COM_AZURE_DEV__BASE__API Subst {
  private:

    const String& text;
    const String** args = nullptr; // TAG: use MemorySpan for args
    const MemorySpan* args2 = nullptr;
    unsigned int numberOfArgs = 0;
  public:

    Subst(const String& text, const String** args, unsigned int numberOfArgs);

    Subst(const String& text, const MemorySpan* args, unsigned int numberOfArgs);

    inline unsigned int getNumberOfArgs() const noexcept
    {
      return numberOfArgs;
    }

    inline const String& getArg(unsigned int i) const
    {
      if (!INLINE_ASSERT(i < numberOfArgs)) {
        throw OutOfRange();
      }
      return *args[i];
    }

    // TAG: use MemorySpan for args
    inline MemorySpan getArgBytes(unsigned int i) const
    {
      if (!INLINE_ASSERT(i < numberOfArgs)) {
        throw OutOfRange();
      }
      const String& text = *args[i];
      return MemorySpan(text.getElements(), text.getEnd());
    }

    /** Returns the substituted string. */
    String format() const;

    /** Write to stream. */
    FormatOutputStream& operator<<(FormatOutputStream& stream) const;
  };
};

FormatOutputStream& operator<<(FormatOutputStream& stream, const Format::Subst& subst);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
