/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATH__VECTOR2D_H
#define _DK_SDU_MIP__BASE_MATH__VECTOR2D_H

#include <base/Object.h>
#include <base/string/FormatOutputStream.h>

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
    Initializes vector from the specified coordinates.

    @param xx The desired X coordinate.
    @param yy The desired Y coordinate.
  */
  Vector2D(const TYPE& xx, const TYPE& yy) throw();

  /**
    Initializes vector by copying from the specified vector.

    @param copy The desired vector.
  */
  Vector2D(const Vector2D& copy) throw();

  /**
    Assignment of this vector from vector.
  */
  Vector2D& operator=(const Vector2D& eq) throw();

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
    Sets values less than the specified value to zero.
  */
  Vector2D& zeroAdjust(const TYPE& zero) throw();

  /**
    Returns true if the vectors are equal.
  */
  bool isEqual(const Vector2D& value) const throw();

  /**
    Returns true if the length of this vector is zero.
  */
  bool isZero() const throw();

  /**
    Returns true if the length of this vector is greater zero.
  */
  bool isProper() const throw();

  /**
    Returns true if this vector is orthogonal with the specified vector.
  */
  bool isOrthogonal(const Vector2D& value) const throw();

  /**
    Returns true if this vector is parallel with the specified vector.
  */
  bool isParallel(const Vector2D& value) const throw();

  /**
    Unary plus.
  */
  Vector2D plus() const throw();

  /**
    Unary minus.
  */
  Vector2D minus() const throw();

  /**
    Negates this vector.
  */
  Vector2D& negate() throw();

  /**
    Adds the specified vector to this vector.
  */
  Vector2D& add(const Vector2D& value) throw();

  /**
    Subtracts the specified vector from this vector.
  */
  Vector2D& subtract(const Vector2D& value) throw();

  /**
    Multiplies this vector with the specified value.
  */
  Vector2D& multiply(const TYPE& value) throw();

  /**
    Divides this vector with the specified value.
  */
  Vector2D& divide(const TYPE& value) throw();

  /**
    Returns the dot product of this vector and the specified vector.
  */
  TYPE dot(const Vector2D& value) const throw();

  /**
    Returns the determinant of this vector and the specified vector.
  */
  TYPE determinant(const Vector2D& value) const throw();

  /**
    Returns the angle between this vector and the specified vector.
  */
  TYPE getAngle(const Vector2D& value) const throw();

  /**
    Returns the projection of this vector onto the specified vector.
  */
  Vector2D getProjection(const Vector2D& value) const throw();




  /**
    Returns true if the vectors are equal.

    @param vector Vector to be compared.
  */
  inline bool operator==(const Vector2D& value) const throw() {return isEqual(value);}

  /**
    Adds the specified vector from this vector.

    @param value The value to be added.
  */
  inline Vector2D& operator+=(const Vector2D& value) throw() {return add(value);}

  /**
    Subtracts the specified vector from this vector.

    @param value The value to be subtracted.
  */
  inline Vector2D& operator-=(const Vector2D& value) throw() {return subtract(value);}

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  inline Vector2D& operator*=(const TYPE& value) throw() {return multiply(value);}

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  inline Vector2D& operator/=(const TYPE& value) throw() {return divide(value);}

  /**
    Unary plus.
  */
  inline Vector2D operator+() const throw() {return plus();}

  /**
    Unary minus.
  */
  inline Vector2D operator-() const throw() {return minus();}



  /**
    Returns the product of the vector and the value.
  */
  friend Vector2D operator* <>(const Vector2D& left, const TYPE& right) throw();

  /**
    Returns the product of the vector and the value.
  */
  friend Vector2D operator* <>(const TYPE& left, const Vector2D& right) throw();

  /**
    Returns the result of the vector divided by the value.
  */
  friend Vector2D operator/ <>(const Vector2D& left, const TYPE& right) throw();

  /**
    Writes a string representation of a Vector2D object to a format stream. The format is "(x, y)".
  */
  friend FormatOutputStream& operator<< <>(FormatOutputStream& stream, const Vector2D& value);
};

template<class TYPE>
Vector2D<TYPE> operator*(const Vector2D<TYPE>& left, const TYPE& right) throw();

template<class TYPE>
Vector2D<TYPE> operator*(const TYPE& left, const Vector2D<TYPE>& right) throw();

template<class TYPE>
Vector2D<TYPE> operator/(const Vector2D<TYPE>& left, const TYPE& right) throw();

/**
  Returns the dot product of the two vectors.
*/
template<class TYPE>
TYPE dot(const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) throw();

/**
  Returns the determinant of the two vectors.
*/
template<class TYPE>
TYPE determinant(const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) throw();

/**
  Writes a string representation of a Vector2D object to a format stream. The format is "(x, y)".
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector2D<TYPE>& value);

#endif
