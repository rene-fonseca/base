/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__LONG_INTEGER_H
#define _DK_SDU_MIP__BASE__LONG_INTEGER_H

#include <base/Object.h>

/**
  Signed and long integer (64 bits or more).

  @author René Møller Fonseca
  @version 1.0
*/

class LongInteger : public virtual Object {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the maximum value. */
  static const long long MAXIMUM = 9223372036854775807LL;
  /** Specifies the minimum value. */
  static const long long MINIMUM = (-9223372036854775807LL - 1LL);
protected:

  /** The value. */
  long long value;
public:

  /**
    Initializes the long integer as zero.
  */
  inline LongInteger() throw() : value(0) {}

  /**
    Initializes the long integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  inline LongInteger(long long value) throw() : value(value) {}

  /**
    Initializes integer by copying from other integer.
  */
  inline LongInteger(const LongInteger& copy) throw() : value(copy.value) {}

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

#endif
