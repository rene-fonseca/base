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
  Unsigned integer (32 bits or more).

  @short Unsigned integer.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API UnsignedInteger : public Object {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = false;
  /** Specifies the minimum value. */
  static const unsigned int MINIMUM = PrimitiveTraits<unsigned int>::MINIMUM;
  /** Specifies the maximum value. */
  static const unsigned int MAXIMUM = PrimitiveTraits<unsigned int>::MAXIMUM;

  enum {
    FLAG_ALLOW_SPACES = 1 << 0
  };
  
  enum BaseFlags {
    BIN = 1 << 0, /**< Specifies binary integer base (2). */
    DEC = 1 << 2, /**< Specifies decimal integer base (10). */
    OCT = 1 << 3, /**< Specifies octal integer base (8). */
    HEX = 1 << 4, /**< Specifies hexadecimal integer base (16). */
    PREFIX = 1 << 5, /**< Specifies that prefix is required. */
    ANY=BIN|DEC|OCT|HEX|PREFIX, /**< Specifies any supported base (integer base is derived from prefix). */
    FLAG_DEFAULT = FLAG_ALLOW_SPACES | ANY
  };
protected:

  /** The value. */
  unsigned int value = 0;
public:

  /**
    Returns the value of the integer string representation.

    @param src The string representation.
    @param end The string representation.
    @param accept Specifies the integer bases to accept. Default is ANY.
  */
  static unsigned int parse(const char* src, const char* end, unsigned int flags = FLAG_DEFAULT) throw(InvalidFormat);

  static unsigned int parse(const String& string, unsigned int flags = FLAG_DEFAULT) throw(InvalidFormat) {
    return parse(string.native(), string.native() + string.getLength(), flags);
  }
  
  /**
    Initializes the integer as zero.
  */
  inline UnsignedInteger() noexcept
  {
  }

  /**
    Initializes the integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  UnsignedInteger(unsigned int value) noexcept;

  /**
    Copy constructor. Initializes a new Integer from other Integer object.
  */
  inline UnsignedInteger(const UnsignedInteger& copy) noexcept
    : value(copy.value)
  {
  }

  /**
    Assignment of integer to this integer.
  */
  inline UnsignedInteger& operator=(const UnsignedInteger& assign) noexcept
  {
    value = assign.value;
    return *this;
  }

  /**
    Assignment of native type to this integer.
  */
  inline UnsignedInteger& operator=(unsigned int value) noexcept
  {
    this->value = value;
    return *this;
  }

  /**
    Gets the value of the integer.
  */
  inline unsigned int getValue() const noexcept
  {
    return value;
  }

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(unsigned int value) noexcept
  {
    this->value = value;
  }

  /**
    Casts integer to native type.
  */
  inline operator unsigned int() const noexcept
  {
    return value;
  }
};

inline UnsignedInteger::UnsignedInteger(unsigned int _value) noexcept
  : value(_value)
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
