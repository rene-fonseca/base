/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__SHORT_INTEGER_H
#define _DK_SDU_MIP__BASE__SHORT_INTEGER_H

#include <base/Object.h>
#include <limits.h>

/**
  Signed and short integer (16 bits or more).

  @author René Møller Fonseca
  @version 1.0
*/

class ShortInteger : public Object {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the maximum value. */
  static const short int MAXIMUM = SHRT_MAX;
  /** Specifies the minimum value. */
  static const short int MINIMUM = SHRT_MIN;
protected:

  /** The value. */
  short int value;
public:

  /**
    Initializes the short integer as zero.
  */
  inline ShortInteger() throw() : value(0) {}

  /**
    Initializes the short integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  inline ShortInteger(short int value) throw() : value(value) {}

  /**
    Copy constructor. Initializes a new ShortInteger from other ShortInteger object.
  */
  inline ShortInteger(const ShortInteger& copy) throw() : value(copy.value) {}

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
  inline short int getValue() const throw() {
    return value;
  }

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(short int value) throw() {
    this->value = value;
  }

  /**
    Casts integer to native type.
  */
  inline operator short int() const throw() {
    return value;
  }
};

#endif
