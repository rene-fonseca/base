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
#include <base/FloatingPoint.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class is a wrapper for the float primitive.
  
  @short Float.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Float : public Object {
public:

  /** Minimum normalized positive floating-point number (2^(e_min-1)). */
  static constexpr float MINIMUM = FloatingPoint::ToFloat::MINIMUM;
  /** Maximum representable finite floating-point number (2^e_max). */
  static const float MAXIMUM; // TAG: = FloatingPoint::ToFloat::MAXIMUM;
  /**
    The difference between 1 and the least value greater than 1 that is
    representable in the given floating point type.
  */
  static constexpr float EPSILON = FloatingPoint::ToFloat::EPSILON;
protected:
  
  /** The value. */
  float value = 0;
public:
  
  /**
    Initializes the value as zero.
  */
  inline Float() throw() {
  }

  /**
    Initializes the float with the specified value. Implicit initialization is
    allowed.

    @param value The desired value.
  */
  inline Float(float _value) throw() : value(_value) {
  }
  
  /**
    Initializes the value by other value.
  */
  inline Float(const Float& copy) throw() : value(copy.value) {
  }

  /**
    Assignment of value by value.
  */
  inline Float& operator=(const Float& eq) throw() {
    value = eq.value;
    return *this;
  }

  /**
    Assignment of this value by float primitive.
  */
  inline Float& operator=(float value) throw() {
    this->value = value;
    return *this;
  }
  
  /**
    Returns the value of the float.
  */
  inline float getValue() const throw() {
    return value;
  }

  /**
    Sets the value of the float.

    @param value The desired value.
  */
  inline void setValue(float value) throw() {
    this->value = value;
  }

  /**
    Returns true if the value is negative.
  */
  inline bool isNegative() const throw() {
    return FloatingPoint::ToFloat(value).isNegative();
  }
  
  /**
    Returns true if the value is an ordinary number (not infinity or NaN).
  */
  inline bool isOrdinary() const throw() {
    return FloatingPoint::ToFloat(value).isOrdinary();
  }
  
  /**
    Returns true if the value is either +INFINITY or -INFINITY.
  */
  inline bool isInfinity() const throw() {
    return FloatingPoint::ToFloat(value).isInfinity();
  }
  
  /**
    Returns true if the value is Not a Number (NaN).
  */
  inline bool isNaN() const throw() {
    return FloatingPoint::ToFloat(value).isNaN();
  }
  
  /**
    Returns true if the value is a quite NaN. A NaN that behaves predictably
    and does not raise exceptions in arithmetic operations is called a quiet
    NaN.
  */
  inline bool isQuiteNaN() const throw() {
    return FloatingPoint::ToFloat(value).isQuiteNaN();
  }
  
  /**
    Returns true if the value is a signaling NaN. A NaN that generally raises
    an exception when encountered as an operand of arithmetic operations is
    called a signaling NaN.
  */
  inline bool isSignalingNaN() const throw() {
    return FloatingPoint::ToFloat(value).isSignalingNaN();
  }
  
  /**
    Casts value to float primitive.
  */
  inline operator float() const throw() {
    return value;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
