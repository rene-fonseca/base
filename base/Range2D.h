/***************************************************************************
    begin       : Fri May 12 2000
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
  Range2D();

  /**
    Initialize object. The range is copied from the specified range.

    @param range The desired range.
  */
  Range2D(const Range2D &range);

  /**
    Initialize object.

    @param minimum The desired minimum value.
    @param maximum The desired maximum value.
  */
  Range2D(float minimum, float maximum);

  /**
    Sets the range.

    @param range The desired range.
  */
  void assign(const Range2D &range);

  /**
    Sets the minimum and maximum values.

    @param minimum The desired minimum value.
    @param maximum The desired maximum value.
  */
  void assign(float minimum, float maximum);

  /**
    Returns true if the value is above the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isAbove(float value) const;

  /**
    Returns true if the value is below the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isBelow(float value) const;

  /**
    Returns true if the value is outside the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isOutside(float value) const;

  /**
    Returns true if the value is within the range specified by minimum and maximum.

    @param value The value to be checked.
  */
  bool isWithin(float value) const;

  /**
    Returns the minimum value.
  */
  float getMinimum() const;

  /**
    Returns the maximum value.
  */
  float getMaximum() const;

  /**
    Sets the minimum value.

    @param minimum The desired minimum value.
  */
  void setMinimum(float minimum);

  /**
    Sets the maximum value.

    @param maximum The desired maximum value.
  */
  void setMaximum(float maximum);

  /**
    Writes a string representation of this object to a stream. This method is intended to be used for debugging purposes.
  */
  void toStream(ostream& stream) const;
};

#endif
