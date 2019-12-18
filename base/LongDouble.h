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
#include <base/FloatingPoint.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class is a wrapper for the long double primitive.
  
  @short Long double.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API LongDouble : public Object {
public:

  /** Minimum normalized positive floating-point number (2^(e_min-1)). */
  static const long double MINIMUM;
  /** Maximum representable finite floating-point number (2^e_max). */
  static const long double MAXIMUM;
  /** Positive infinity. */
  static const long double INFINITY;
  /** Negative infinity. */
  static const long double MINUS_INFINITY;
  /** Quiet NaN. */
  static const long double QUIET_NAN;
  /**
    The difference between 1 and the least value greater than 1 that is
    representable in the given floating point type.
  */
  static const long double EPSILON;
protected:
  
  /** The value. */
  long double value = 0;
public:
  
  /**
    Initializes the value as zero.
  */
  inline LongDouble() noexcept
  {
  }

  /**
    Initializes the long double with the specified value. Implicit
    initialization is allowed.
    
    @param value The desired value.
  */
  inline LongDouble(long double _value) noexcept
    : value(_value)
  {
  }
  
  /**
    Initializes the value by other value.
  */
  inline LongDouble(const LongDouble& copy) noexcept
    : value(copy.value)
  {
  }

  /**
    Assignment of value by value.
  */
  inline LongDouble& operator=(const LongDouble& assign) noexcept
  {
    value = assign.value;
    return *this;
  }

  /**
    Assignment of this value by long double primitive.
  */
  inline LongDouble& operator=(long double value) noexcept
  {
    this->value = value;
    return *this;
  }
  
  /**
    Returns the value of the long double.
  */
  inline long double getValue() const noexcept
  {
    return value;
  }

  /**
    Sets the value of the long double.

    @param value The desired value.
  */
  inline void setValue(long double value) noexcept
  {
    this->value = value;
  }

  /**
    Returns true if the value is negative.
  */
  inline bool isNegative() const noexcept
  {
    return FloatingPoint::ToLongDouble(value).isNegative();
  }

  /**
    Returns the sign.
  */
  inline long double getSign() const noexcept
  {
    return FloatingPoint::ToLongDouble(value).isNegative() ? -1.0L : 1.0L;
  }

  /**
    Returns true if the value is an ordinary number (not infinity or NaN).
  */
  inline bool isOrdinary() const noexcept
  {
    return FloatingPoint::ToLongDouble(value).isOrdinary();
  }
  
  /**
    Returns true if the value is either +INFINITY or -INFINITY.
  */
  inline bool isInfinity() const noexcept
  {
    return FloatingPoint::ToLongDouble(value).isInfinity();
  }
  
  /**
    Returns true if the value is Not a Number (NaN).
  */
  inline bool isNaN() const noexcept
  {
    return FloatingPoint::ToLongDouble(value).isNaN();
  }
  
  /**
    Returns true if the value is a quite NaN. A NaN that behaves predictably
    and does not raise exceptions in arithmetic operations is called a quiet
    NaN.
  */
  inline bool isQuiteNaN() const noexcept
  {
    return FloatingPoint::ToLongDouble(value).isQuiteNaN();
  }
  
  /**
    Returns true if the value is a signaling NaN. A NaN that generally raises
    an exception when encountered as an operand of arithmetic operations is
    called a signaling NaN.
  */
  inline bool isSignalingNaN() const noexcept
  {
    return FloatingPoint::ToLongDouble(value).isSignalingNaN();
  }
  
  /**
    Casts value to the long double primitive.
  */
  inline operator long double() const noexcept
  {
    return value;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
