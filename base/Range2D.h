/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _RANGE2D_H
#define _RANGE2D_H

#include "Object.h"

/**
  2D range.

  @author René Møller Fonseca
  @version 1.0
*/

class Range2D : public Object {
protected:

  /** The minimum value. */
  float minimum;
  /** The maximum value. */
  float maximum;
public:

  /**
    Initialize object. The minimum and maximum value are set to zero.
  */
  Range2D() throw();

  /**
    Initialize object. The range is copied from the specified range.

    @param range The desired range.
  */
  Range2D(const Range2D& range) throw();

  /**
    Initialize object.

    @param minimum The desired minimum value.
    @param maximum The desired maximum value.
  */
  Range2D(float minimum, float maximum) throw();

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
  void assign(float minimum, float maximum) throw();

  /**
    Returns true if the value is above the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isAbove(float value) const throw();

  /**
    Returns true if the value is below the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isBelow(float value) const throw();

  /**
    Returns true if the value is outside the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isOutside(float value) const throw();

  /**
    Returns true if the value is within the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isWithin(float value) const throw();

  /**
    Returns the minimum value.
  */
  float getMinimum() const throw();

  /**
    Returns the maximum value.
  */
  float getMaximum() const throw();

  /**
    Sets the minimum value.

    @param minimum The desired minimum value.
  */
  void setMinimum(float minimum) throw();

  /**
    Sets the maximum value.

    @param maximum The desired maximum value.
  */
  void setMaximum(float maximum) throw();

  /**
    Writes a string representation of this object to stream.
  */
  ostream& toString(ostream& stream) const;
};

TOSTRING(Range2D);

#endif
