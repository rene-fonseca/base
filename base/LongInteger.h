/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__LONG_INTEGER_H
#define _DK_SDU_MIP__BASE__LONG_INTEGER_H

#include "Object.h"

/**
  Signed and long integer (64 bits or more).

  @author René Møller Fonseca
  @version 1.0
*/

class LongInteger : public Object, public Copyable {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the maximum value. */
  static const long long MAXIMUM = 9223372036854775807LL;
  /** Specifies the minimum value. */
  static const long long MINIMUM = (-9223372036854775807LL - 1LL);
protected:

  /** The value. */
  long long val;
public:

  /**
    Initializes the long integer as zero.
  */
  inline LongInteger() throw() : val(0) {};

  /**
    Initializes the long integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  inline LongInteger(long long value) throw() : val(value) {}

  /**
    Copy constructor. Initializes a new LongInteger from other LongInteger object.
  */
  LongInteger(const LongInteger& copy) throw(); // use default copy object policy

  /**
    Assignment operator.
  */
  const LongInteger& operator=(long long value) const throw(); // use default assignment object policy

  /**
    Gets the value of the integer.
  */
  inline long long getValue() const throw() {return val;}

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(long long value) throw() {val = value;}

  /**
    Casts integer to native type.
  */
  inline operator long long() const throw() {return val;}
};

#endif
