/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__RANGE2D_H
#define _DK_SDU_MIP__BASE__RANGE2D_H

#include <base/Object.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A 2D range object is a span from a minimum to a maximum.

  @short 2D range.
  @author René Møller Fonseca
  @version 1.0
*/

class Range2D : public virtual Object {
protected:

  /** The minimum value. */
  double minimum;
  /** The maximum value. */
  double maximum;
public:

  /**
    Initializes a Range2D object with the minimum and maximum values set to zero.
  */
  Range2D() throw();

  /**
    Initializes a Range2D object. The range is copied from the specified range.

    @param range The desired range.
  */
  Range2D(const Range2D& range) throw();

  /**
    Initializes a Range2D object with the specified minimum and maximum values.

    @param minimum The desired minimum value.
    @param maximum The desired maximum value.
  */
  Range2D(double minimum, double maximum) throw();

  /**
    Sets the range.

    @param range The desired range.
  */
  void assign(const Range2D &range) throw();

  /**
    Sets the minimum and maximum values.

    @param minimum The desired minimum value.
    @param maximum The desired maximum value.
  */
  void assign(double minimum, double maximum) throw();

  /**
    Returns true if the value is above the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isAbove(double value) const throw();

  /**
    Returns true if the value is below the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isBelow(double value) const throw();

  /**
    Returns true if the value is outside the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isOutside(double value) const throw();

  /**
    Returns true if the value is within the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isWithin(double value) const throw();

  /**
    Returns the minimum value.
  */
  double getMinimum() const throw();

  /**
    Returns the maximum value.
  */
  double getMaximum() const throw();

  /**
    Sets the minimum value.

    @param minimum The desired minimum value.
  */
  void setMinimum(double minimum) throw();

  /**
    Sets the maximum value.

    @param maximum The desired maximum value.
  */
  void setMaximum(double maximum) throw();

  /**
    Writes a string representation of a Range2D object to a format stream. The format is "(minimum,maximum)".
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const Range2D& value);
};

/**
  Writes a string representation of a Range2D object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Range2D& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
