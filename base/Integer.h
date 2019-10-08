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
  Signed integer (32 bits or more).

  @short Signed integer.
  @version 1.2
*/

class _DK_SDU_MIP__BASE__API Integer : public Object {
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

  /**
    Returns the value of the integer string representation.

    @param string The string representation.
    @param withoutSign Effectively prevents signs from being parsed. Default is to allow a single sign.
  */
  static int parse(const String& string, bool withoutSign = false) throw(InvalidFormat);
  
  /**
    Initializes the integer as zero.
  */
  inline Integer() throw() {
  }

  /**
    Initializes the integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  Integer(int value) throw();

  /**
    Initializes integer from string.

    @param string The string.
  */
  inline Integer(const String& string) throw(InvalidFormat)
    : value(parse(string)) {
  }

  /**
    Copy constructor. Initializes a new Integer from other Integer object.
  */
  inline Integer(const Integer& copy) throw() : value(copy.value) {
  }

  /**
    Assignment of integer to this integer.
  */
  inline Integer& operator=(const Integer& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Assignment of native type to this integer.
  */
  inline Integer& operator=(int value) throw() {
    this->value = value;
    return *this;
  }

  /**
    Gets the value of the integer.
  */
  inline int getValue() const throw() {
    return value;
  }

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(int value) throw() {
    this->value = value;
  }

  /**
    Casts integer to native type.
  */
  inline operator int() const throw() {
    return value;
  }
};

inline Integer::Integer(int _value) throw()
  : value(_value) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
