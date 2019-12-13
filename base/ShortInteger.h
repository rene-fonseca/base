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
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Signed and short integer (16 bits or more).

  @short A signed short integer.
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API ShortInteger : public Object {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the maximum value. */
  static const short MAXIMUM = PrimitiveTraits<short>::MAXIMUM;
  /** Specifies the minimum value. */
  static const short MINIMUM = PrimitiveTraits<short>::MINIMUM;
protected:

  /** The value. */
  short value = 0;
public:
  
  enum {
    FLAG_ALLOW_SPACES = 1 << 0,
    FLAG_ALLOW_SIGN = 1 << 1,
    FLAG_DEFAULT = FLAG_ALLOW_SPACES | FLAG_ALLOW_SIGN
  };

  /**
    Returns the value of the integer string representation.

    @param src The string representation.
    @param end The string representation.
    @param flags The parsing flags.
  */
  static short parse(const char* src, const char* end, unsigned int flags = FLAG_DEFAULT);

  /**
    Returns the value of the integer string representation.

    @param string The string representation.
    @param flags The parsing flags.
  */
  static inline short parse(const String& string, unsigned int flags = FLAG_DEFAULT)
  {
    return parse(string.native(), string.native() + string.getLength(), flags);
  }
  
  /**
    Initializes the short integer as zero.
  */
  inline ShortInteger() noexcept
  {
  }
  
  /**
    Initializes the short integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  ShortInteger(short value) noexcept;
  
  /**
    Copy constructor. Initializes a new ShortInteger from other ShortInteger object.
  */
  inline ShortInteger(const ShortInteger& copy) noexcept
    : value(copy.value)
  {
  }

  /**
    Assignment operator.
  */
  inline ShortInteger& operator=(const ShortInteger& assign) noexcept
  {
    value = assign.value;
    return *this;
  }

  /**
    Gets the value of the integer.
  */
  inline short getValue() const noexcept
  {
    return value;
  }

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(short value) noexcept
  {
    this->value = value;
  }

  /**
    Casts integer to native type.
  */
  inline operator short () const noexcept
  {
    return value;
  }
};

inline ShortInteger::ShortInteger(short _value) noexcept
  : value(_value)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
