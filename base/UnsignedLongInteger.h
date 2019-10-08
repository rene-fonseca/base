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
#include <base/string/FormatInputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Unsigned long integer (64 bits or more). This class maps onto the
  unsigned long long primitive. There is no wrapper class available for the
  long primitive.

  @short An unsigned long integer
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API UnsignedLongInteger : public Object {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = false;
  /** Specifies the minimum value. */
  static const unsigned long long MINIMUM = PrimitiveTraits<unsigned long long>::MINIMUM;
  /** Specifies the maximum value. */
  static const unsigned long long MAXIMUM = PrimitiveTraits<unsigned long long>::MAXIMUM;

  enum BaseOptions {
    BIN = 1, /**< Specifies binary integer base (2). */
    DEC = 2, /**< Specifies decimal integer base (10). */
    OCT = 4, /**< Specifies octal integer base (8). */
    HEX = 8, /**< Specifies hexadecimal integer base (16). */
    PREFIX = 16, /**< Specifies that prefix is required. */
    ANY=BIN|DEC|OCT|HEX|PREFIX /**< Specifies any supported base (integer base is derived from prefix). */
  };
protected:

  /** The value. */
  unsigned long long value = 0;
public:

  /**
    Returns the value of the integer string representation.

    @param str The string representation.
    @param accept Specifies the integer bases to accept. Default is ANY.
  */
  static unsigned long long parse(const String& str, unsigned int accept = ANY) throw(InvalidFormat);

  /**
    Initializes the integer as zero.
  */
  inline UnsignedLongInteger() throw() {
  }

  /**
    Initializes the integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  UnsignedLongInteger(unsigned long long value) throw();

  /**
    Initializes the integer from the specified string representation.

    @param str The string representation.
    @param accept Specifies the bases to accept. Default is ANY.
  */
  inline UnsignedLongInteger(const String& str, unsigned long long accept = ANY) throw(InvalidFormat) : value(parse(str, accept)) {
  }

  /**
    Copy constructor. Initializes a new Integer from other Integer object.
  */
  inline UnsignedLongInteger(const UnsignedLongInteger& copy) throw()
    : value(copy.value) {
  }

  /**
    Assignment of integer to this integer.
  */
  inline UnsignedLongInteger& operator=(const UnsignedLongInteger& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Assignment of native type to this integer.
  */
  inline UnsignedLongInteger& operator=(unsigned long long value) throw() {
    this->value = value;
    return *this;
  }

  /**
    Gets the value of the integer.
  */
  inline unsigned long long getValue() const throw() {
    return value;
  }

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(unsigned long long value) throw() {
    this->value = value;
  }

  /**
    Casts integer to native type.
  */
  inline operator unsigned long long() const throw() {
    return value;
  }
};

inline UnsignedLongInteger::UnsignedLongInteger(unsigned long long _value) throw()
  : value(_value) {
}

/**
  Reads an unsigned long long from the specified format input stream.
*/
FormatInputStream& operator>>(FormatInputStream& stream, unsigned long long& value) throw(InvalidFormat, IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
