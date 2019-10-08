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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Signed and long integer (64 bits or more).

  @short A signed long integer
  @version 1.1
*/

class _DK_SDU_MIP__BASE__API LongInteger : public virtual Object {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the maximum value. */
  static const long long MAXIMUM = PrimitiveTraits<long long>::MAXIMUM;
  /** Specifies the minimum value. */
  static const long long MINIMUM = PrimitiveTraits<long long>::MINIMUM;
protected:

  /** The value. */
  long long value = 0;
public:
  
  /**
    Returns the value of the integer string representation.

    @param string The string representation.
    @param withoutSign Effectively prevents signs from being parsed. Default is to allow a single sign.
  */
  static long long parse(const String& string, bool withoutSign = false) throw(InvalidFormat);
  
  /**
    Initializes the long integer as zero.
  */
  inline LongInteger() throw() {
  }
  
  /**
    Initializes the long integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  LongInteger(long long value) throw();
  
  /**
    Initializes long integer from string.
    
    @param string The string.
  */
  inline LongInteger(const String& string) throw(InvalidFormat)
    : value(parse(string)) {
  }
  
  /**
    Initializes integer by copying from other integer.
  */
  inline LongInteger(const LongInteger& copy) throw() : value(copy.value) {
  }
  
  /**
    Assignment of integer to this integer.
  */
  inline LongInteger& operator=(const LongInteger& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Gets the value of the integer.
  */
  inline long long getValue() const throw() {
    return value;
  }

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(long long value) throw() {
    this->value = value;
  }

  /**
    Casts integer to native type.
  */
  inline operator long long() const throw() {
    return value;
  }
};

inline LongInteger::LongInteger(long long _value) throw() : value(_value) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
