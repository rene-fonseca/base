/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__SHORT_INTEGER_H
#define _DK_SDU_MIP__BASE__SHORT_INTEGER_H

#include "Object.h"
#include <limits.h>

/**
  Signed and short integer (16 bits or more).

  @author René Møller Fonseca
  @version 1.0
*/

class ShortInteger : public Object, public Copyable {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the maximum value. */
  static const short int MAXIMUM = SHRT_MAX;
  /** Specifies the minimum value. */
  static const short int MINIMUM = SHRT_MIN;
protected:

  /** The value. */
  int val;
public:

  /**
    Initializes the short integer as zero.
  */
  inline ShortInteger() throw() : val(0) {};

  /**
    Initializes the short integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  inline ShortInteger(short int value) throw() : val(value) {};

  /**
    Copy constructor. Initializes a new ShortInteger from other ShortInteger object.
  */
  ShortInteger(const ShortInteger& copy) throw(); // use default copy object policy

  /**
    Assignment operator.
  */
  ShortInteger& operator=(short int value) const throw(); // use default assignment object policy

  /**
    Gets the value of the integer.
  */
  inline short int getValue() const throw() {return val;};

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(short int value) throw() {val = value;};

  /**
    Casts integer to native type.
  */
  inline operator short int() const throw() {return val;};
};

#endif
