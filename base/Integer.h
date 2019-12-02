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

#include <base/Object.h>
#include <base/Primitives.h>
#include <base/string/WideString.h>
#include <base/string/InvalidFormat.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Signed integer (32 bits or more).

  @short Signed integer.
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API Integer : public Object {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the minimum value. */
  static const int MINIMUM = PrimitiveTraits<int>::MINIMUM;
  /** Specifies the maximum value. */
  static const int MAXIMUM = PrimitiveTraits<int>::MAXIMUM;
protected:

  /** The value. */
  int value = 0;
public:

  enum {
    FLAG_ALLOW_SPACES = 1 << 0,
    FLAG_ALLOW_SIGN = 1 << 1,
    FLAG_DEFAULT = FLAG_ALLOW_SPACES | FLAG_ALLOW_SIGN
  };

  /**
    Returns the value of the integer string representation.

    @param string The string representation.
    @param flags The parsing flags.
  */
  static int parse(const char* src, const char* end, unsigned int flags = FLAG_DEFAULT) throw(InvalidFormat);

  /**
    Returns the value of the integer string representation.

    @param string The string representation.
    @param withoutSign Effectively prevents signs from being parsed. Default is to allow a single sign.
  */
  static inline int parse(const String& string, unsigned int flags = FLAG_DEFAULT) throw(InvalidFormat)
  {
    return parse(string.native(), string.native() + string.getLength(), flags);
  }
  
  /**
    Returns the value of the integer string representation.

    @param string The string representation.
    @param flags The parsing flags.
  */
  static int parse(const wchar* src, const wchar* end, unsigned int flags = FLAG_DEFAULT) throw(InvalidFormat);

  /**
    Returns the value of the integer string representation.

    @param string The string representation.
    @param flags The parsing flags.
  */
  static int parse(const ucs4* src, const ucs4* end, unsigned int flags = FLAG_DEFAULT) throw(InvalidFormat);

  /**
    Returns the value of the integer string representation.

    @param string The string representation.
    @param withoutSign Effectively prevents signs from being parsed. Default is to allow a single sign.
  */
  static inline int parse(const WideString& string, unsigned int flags = FLAG_DEFAULT) throw(InvalidFormat)
  {
    return parse(string.native(), string.native() + string.getLength(), flags);
  }

  /**
    Initializes the integer as zero.
  */
  inline Integer() noexcept
  {
  }

  /**
    Initializes the integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  Integer(int value) noexcept;

  /**
    Copy constructor. Initializes a new Integer from other Integer object.
  */
  inline Integer(const Integer& copy) noexcept
    : value(copy.value)
  {
  }

  /**
    Assignment of integer to this integer.
  */
  inline Integer& operator=(const Integer& assign) noexcept
  {
    value = assign.value;
    return *this;
  }

  /**
    Assignment of native type to this integer.
  */
  inline Integer& operator=(int value) noexcept
  {
    this->value = value;
    return *this;
  }

  /**
    Gets the value of the integer.
  */
  inline int getValue() const noexcept
  {
    return value;
  }

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(int value) noexcept
  {
    this->value = value;
  }

  /**
    Casts integer to native type.
  */
  inline operator int() const noexcept
  {
    return value;
  }
};

inline Integer::Integer(int _value) noexcept
  : value(_value)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
