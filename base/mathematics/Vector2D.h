/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATH__VECTOR2D_H
#define _DK_SDU_MIP__BASE_MATH__VECTOR2D_H

#include "base/Object.h"
#include "base/string/FormatOutputStream.h"

/**
  A two-dimensional vector represented by X and Y coordinates.

  @short Two-dimensional vector.
  @author René Møller Fonseca
  @version 1.01
*/

template<class TYPE>
class Vector2D : public Object {
protected:

  /** The X coordinate. */
  TYPE x;
  /** The Y coordinate. */
  TYPE y;
public:

  /**
    Initializes vector as the origin (0,0).
  */
  Vector2D() throw();

  /**
    Initializes vector by copying the coordinates from the specified vector.

    @param vector The desired vector.
  */
  Vector2D(const Vector2D& vector) throw();

  /**
    Initializes vector from the specified coordinates.

    @param x The desired X coordinate.
    @param y The desired Y coordinate.
  */
  Vector2D(const TYPE& x, const TYPE& y) throw();

  /**
    Assignment of vector to vector.

    @param vector The desired vector.
  */
  void assign(const Vector2D& vector) throw();

  /**
    Sets the X and Y coordinates of the vector.

    @param x The desired X coordinate.
    @param y The desired Y coordinate.
  */
  void assign(const TYPE& x, const TYPE& y) throw();

  /**
    Returns the angle of the vector.
  */
  TYPE getAngle() const throw();

  /**
    Returns the modulus of the vector.
  */
  TYPE getModulus() const throw();

  /**
    Returns the X coordinate.
  */
  TYPE getX() const throw();

  /**
    Returns the Y coordinate.
  */
  TYPE getY() const throw();

  /**
    Sets the X coordinate.

    @param x The desired X coordinate.
  */
  void setX(const TYPE& x) throw();

  /**
    Sets the Y coordinate.

    @param y The desired Y coordinate.
  */
  void setY(const TYPE& y) throw();

  /**
    Writes a string representation of a Vector2D object to a format stream. The format is "(x, y)".
  */
  friend FormatOutputStream& operator<< <>(FormatOutputStream& stream, const Vector2D& value);
};

/**
  Writes a string representation of a Vector2D object to a format stream. The format is "(x, y)".
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector2D<TYPE>& value);

/** 2D vector of float. */
typedef Vector2D<float> Float2D;

/** 2D vector of double. */
typedef Vector2D<double> Double2D;

/** 2D vector of float holding the origin. */
extern const Float2D zeroFloat2D;

/** 2D vector of double holding the origin. */
extern const Double2D zeroDouble2D;

#endif
