/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__LONG_DOUBLE_H
#define _DK_SDU_MIP__BASE__LONG_DOUBLE_H

#include <base/Object.h>
#include <base/FloatingPoint.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is a wrapper for the long double primitive.
  
  @short Long double.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class LongDouble : public Object {
public:

  /** Minimum normalized positive floating-point number (2^(e_min-1)). */
  static const long double MINIMUM = FloatingPoint::ToLongDouble::MINIMUM;
  /** Maximum representable finite floating-point number (2^e_max). */
  static const long double MAXIMUM = FloatingPoint::ToLongDouble::MAXIMUM;
  /**
    The difference between 1 and the least value greater than 1 that is
    representable in the given floating point type.
  */
  static const long double EPSILON = FloatingPoint::ToLongDouble::EPSILON;
protected:
  
  /** The value. */
  long double value;
public:
  
  /**
    Initializes the value as zero.
  */
  inline LongDouble() throw() : value(0) {
  }

  /**
    Initializes the long double with the specified value. Implicit
    initialization is allowed.
    
    @param value The desired value.
  */
  inline LongDouble(long double _value) throw() : value(_value) {
  }
  
  /**
    Initializes the value by other value.
  */
  inline LongDouble(const LongDouble& copy) throw() : value(copy.value) {
  }

  /**
    Assignment of value by value.
  */
  inline LongDouble& operator=(const LongDouble& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Assignment of this value by long double primitive.
  */
  inline LongDouble& operator=(long double value) throw() {
    this->value = value;
    return *this;
  }
  
  /**
    Returns the value of the long double.
  */
  inline long double getValue() const throw() {
    return value;
  }

  /**
    Sets the value of the long double.

    @param value The desired value.
  */
  inline void setValue(long double value) throw() {
    this->value = value;
  }

  /**
    Returns true if the value is negative.
  */
  inline bool isNegative() const throw() {
    return FloatingPoint::ToLongDouble(value).isNegative();
  }
  
  /**
    Returns true if the value is an ordinary number (not infinity or NaN).
  */
  inline bool isOrdinary() const throw() {
    return FloatingPoint::ToLongDouble(value).isOrdinary();
  }
  
  /**
    Returns true if the value is either +INFINITY or -INFINITY.
  */
  inline bool isInfinity() const throw() {
    return FloatingPoint::ToLongDouble(value).isInfinity();
  }
  
  /**
    Returns true if the value is Not a Number (NaN).
  */
  inline bool isNaN() const throw() {
    return FloatingPoint::ToLongDouble(value).isNaN();
  }
  
  /**
    Returns true if the value is a quite NaN. A NaN that behaves predictably
    and does not raise exceptions in arithmetic operations is called a quiet
    NaN.
  */
  inline bool isQuiteNaN() const throw() {
    return FloatingPoint::ToLongDouble(value).isQuiteNaN();
  }
  
  /**
    Returns true if the value is a signaling NaN. A NaN that generally raises
    an exception when encountered as an operand of arithmetic operations is
    called a signaling NaN.
  */
  inline bool isSignalingNaN() const throw() {
    return FloatingPoint::ToLongDouble(value).isSignalingNaN();
  }
  
  /**
    Casts value to the long double primitive.
  */
  inline operator long double() const throw() {
    return value;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
