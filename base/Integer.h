/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__INTEGER_H
#define _DK_SDU_MIP__BASE__INTEGER_H

#include "Object.h"
#include "OutOfRange.h"
#include "string/String.h"
#include "string/InvalidFormat.h"
#include "string/StringOutputStream.h"
#include <limits.h>

/**
  Signed integer (32 bits or more).

  @author René Møller Fonseca
  @version 1.0
*/

class Integer : public Object, public Copyable {
public:

  /** True if the integer type is signed. */
  static const bool SIGNED = true;
  /** Specifies the minimum value. */
  static const int MINIMUM = INT_MIN;
  /** Specifies the maximum value. */
  static const int MAXIMUM = INT_MAX;
protected:

  /** The value. */
  int val;
public:

  /**
    Initializes the integer as zero.
  */
  inline Integer() throw() : val(0) {};

  /**
    Initializes the integer with the specified value. Implicit initialization allowed.

    @param value The desired value.
  */
  inline Integer(int value) throw() : val(value) {};

  /**
    Copy constructor. Initializes a new Integer from other Integer object.
  */
  Integer(const Integer& copy) throw(); // use default copy object policy

  /**
    Assignment operator.
  */
  Integer& operator=(int value) const throw(); // use default assignment object policy

  /**
    Gets the value of the integer.
  */
  inline int getValue() const throw() {return val;};

  /**
    Sets the value of the integer.

    @param value The desired value.
  */
  inline void setValue(int value) throw() {val = value;};

  /**
    Casts integer to native type.
  */
  inline operator int() const throw() {return val;};
};

#endif
