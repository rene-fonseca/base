/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__SHORT_INTEGER_H
#define _DK_SDU_MIP__BASE__SHORT_INTEGER_H

#include <base/Object.h>
#include <base/Primitives.h>
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Signed and short integer (16 bits or more).

  @short A signed short integer.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class ShortInteger : public Object {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the maximum value. */
  static const short MAXIMUM = PrimitiveTraits<short>::MAXIMUM;
  /** Specifies the minimum value. */
  static const short MINIMUM = PrimitiveTraits<short>::MINIMUM;
protected:

  /** The value. */
  short value;
public:
  
  /**
    Returns the value of the integer string representation.

    @param string The string representation.
    @param withoutSign Effectively prevents signs from being parsed. Default is to allow a single sign.
  */
  static short parse(const String& string, bool withoutSign = false) throw(InvalidFormat);
  
  /**
    Initializes the short integer as zero.
  */
  inline ShortInteger() throw() : value(0) {
  }
  
  /**
    Initializes the short integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  ShortInteger(short value) throw();
  
  /**
    Initializes short integer from string.

    @param string The string.
  */
  inline ShortInteger(const String& string) throw(InvalidFormat)
    : value(parse(string)) {
  }
  
  /**
    Copy constructor. Initializes a new ShortInteger from other ShortInteger object.
  */
  inline ShortInteger(const ShortInteger& copy) throw() : value(copy.value) {
  }

  /**
    Assignment operator.
  */
  inline ShortInteger& operator=(const ShortInteger& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Gets the value of the integer.
  */
  inline short getValue() const throw() {
    return value;
  }

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(short value) throw() {
    this->value = value;
  }

  /**
    Casts integer to native type.
  */
  inline operator short () const throw() {
    return value;
  }
};

inline ShortInteger::ShortInteger(short _value) throw() : value(_value) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
