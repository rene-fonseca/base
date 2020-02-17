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
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A 2D range object is a span from a minimum to a maximum.

  @short 2D range.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Range2D : public virtual Object {
protected:

  /** The minimum value. */
  double minimum = 0;
  /** The maximum value. */
  double maximum = 0;
public:

  /**
    Initializes a Range2D object with the minimum and maximum values set to zero.
  */
  inline Range2D() noexcept
  {
  }

  /**
    Initializes a Range2D object. The range is copied from the specified range.

    @param range The desired range.
  */
  inline Range2D(const Range2D& copy) noexcept
    : minimum(copy.minimum), maximum(copy.maximum)
  {
  }

  /**
    Initializes a Range2D object with the specified minimum and maximum values.

    @param minimum The desired minimum value.
    @param maximum The desired maximum value.
  */
  inline Range2D(double _minimum, double _maximum) noexcept
    : minimum(_minimum), maximum(_maximum)
  {
  }

  /**
    Sets the range.

    @param range The desired range.
  */
  inline Range2D& operator=(const Range2D& assign) noexcept
  {
    minimum = assign.minimum;
    maximum = assign.maximum;
    return *this;
  }

  /**
    Returns true if the value is above the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  inline bool isAbove(double value) const noexcept
  {
    return value > maximum;
  }

  /**
    Returns true if the value is below the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  inline bool isBelow(double value) const noexcept
  {
    return value < minimum;
  }

  /**
    Returns true if the value is outside the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  inline bool isOutside(double value) const noexcept
  {
    return (value < minimum) || (value > maximum);
  }
  
  /**
    Returns true if the value is within the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  inline bool isWithin(double value) const noexcept
  {
    return (value >= minimum) && (value <= maximum);
  }
  
  /**
    Returns the minimum value.
  */
  inline double getMinimum() const noexcept
  {
    return minimum;
  }

  /**
    Returns the maximum value.
  */
  inline double getMaximum() const noexcept
  {
    return maximum;
  }

  /**
    Sets the minimum value.

    @param minimum The desired minimum value.
  */
  inline void setMinimum(double minimum) noexcept
  {
    this->minimum = minimum;
  }

  /**
    Sets the maximum value.

    @param maximum The desired maximum value.
  */
  inline void setMaximum(double maximum) noexcept
  {
    this->maximum = maximum;
  }
};

/**
  Writes a string representation of a Range2D object to a format stream. The format is "(minimum, maximum)".
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Range2D& value);

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE(Range2D)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
