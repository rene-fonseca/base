/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATH__VECTOR3D_H
#define _DK_SDU_MIP__BASE_MATH__VECTOR3D_H

#include <base/Object.h>
#include <base/string/FormatOutputStream.h>

/**
  A three-dimensional vector represented by X, Y, and Z coordinates.

  @short Three-dimensional vector.
  @author Morten Baun Møller and René Møller Fonseca
  @version 1.01
*/

template<class TYPE>
class Vector3D : public Object {
protected:

  /** X coordinate. */
  TYPE x;
  /** Y coordinate. */
  TYPE y;
  /** Z coordinate. */
  TYPE z;
public:

  /**
    Initializes vector as origin (0, 0, 0).
  */
  Vector3D() throw();

  /**
    Initializes vector from the specified coordinates.

    @param xx The desired X coordinate.
    @param yy The desired Y coordinate.
    @param zz The desired Z coordinate.
  */
  Vector3D(const TYPE& xx, const TYPE& yy, const TYPE& zz) throw();

  /**
    Initializes vector by copying from the specified vector.

    @param vector The desired vector.
  */
  Vector3D(const Vector3D& copy) throw();

  /**
    Assignment of this vector from vector.
  */
  Vector3D& operator=(const Vector3D& eq) throw();

  /**
    Returns the modulus of the vector.
  */
  TYPE getModulus() const throw();

  /**
    Returns the angle of this vector in the X-Y plane.
  */
  TYPE getXYAngle() const throw();

  /**
    Returns the angle between this vector and the X-Y plane.
  */
  TYPE getZAngle() const throw();

  /**
    Returns the X coordinate of the vector.
  */
  TYPE getX() const throw();

  /**
    Returns the Y coordinate of the vector.
  */
  TYPE getY() const throw();

  /**
    Returns the Z coordinate of the vector.
  */
  TYPE getZ() const throw();

  /**
    Sets the X coordinate of the vector.

    @param x The desired X coordinate.
  */
  void setX(const TYPE& x) throw();

  /**
    Sets the Y coordinate of the vector.

    @param y The desired Y coordinate.
  */
  void setY(const TYPE& y) throw();

  /**
    Sets the Z coordinate of the vector.

    @param z The desired Z coordinate.
  */
  void setZ(const TYPE& z) throw();

  /**
    Sets values less than the specified value to zero.
  */
  Vector3D& zeroAdjust(const TYPE& zero) throw();

  /**
    Returns true if the vectors are equal.
  */
  bool isEqual(const Vector3D& value) const throw();

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
  bool isOrthogonal(const Vector3D& value) const throw();

  /**
    Returns true if this vector is parallel with the specified vector.
  */
  bool isParallel(const Vector3D& value) const throw();

  /**
    Unary plus.
  */
  Vector3D plus() const throw();

  /**
    Unary minus.
  */
  Vector3D minus() const throw();

  /**
    Negates this vector.
  */
  Vector3D& negate() throw();

  /**
    Adds the specified vector to this vector.
  */
  Vector3D& add(const Vector3D& value) throw();

  /**
    Subtracts the specified vector from this vector.
  */
  Vector3D& subtract(const Vector3D& value) throw();

  /**
    Multiplies this vector with the specified value.
  */
  Vector3D& multiply(const TYPE& value) throw();

  /**
    Divides this vector with the specified value.
  */
  Vector3D& divide(const TYPE& value) throw();

  /**
    Returns the dot product of this vector and the specified vector.
  */
  TYPE dot(const Vector3D& value) const throw();

  /**
    Returns the cross product of this vector and the specified vector.
  */
  Vector3D cross(const Vector3D& value) const throw();

  /**
    Returns the angle between this vector and the specified vector.
  */
  TYPE getAngle(const Vector3D& value) const throw();

  /**
    Returns the projection of this vector onto the specified vector.
  */
  Vector3D getProjection(const Vector3D& value) const throw();




  /**
    Returns true if the vectors are equal.

    @param vector Vector to be compared.
  */
  inline bool operator==(const Vector3D& value) const throw() {return isEqual(value);}

  /**
    Adds the specified vector from this vector.

    @param value The value to be added.
  */
  inline Vector3D& operator+=(const Vector3D& value) throw() {return add(value);}

  /**
    Subtracts the specified vector from this vector.

    @param value The value to be subtracted.
  */
  inline Vector3D& operator-=(const Vector3D& value) throw() {return subtract(value);}

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  inline Vector3D& operator*=(const TYPE& value) throw() {return multiply(value);}

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  inline Vector3D& operator/=(const TYPE& value) throw() {return divide(value);}

  /**
    Unary plus.
  */
  inline Vector3D operator+() const throw() {return plus();}

  /**
    Unary minus.
  */
  inline Vector3D operator-() const throw() {return minus();}



  /**
    Returns the product of the vector and the value.
  */
  friend Vector3D operator* <>(const Vector3D& left, const TYPE& right) throw();

  /**
    Returns the product of the vector and the value.
  */
  friend Vector3D operator* <>(const TYPE& left, const Vector3D& right) throw();

  /**
    Returns the result of the vector divided by the value.
  */
  friend Vector3D operator/ <>(const Vector3D& left, const TYPE& right) throw();

  /**
    Writes a string representation of a Vector3D object to a format stream. The format is "(x, y, z)".
  */
  friend FormatOutputStream& operator<< <>(FormatOutputStream& stream, const Vector3D& value);
};

template<class TYPE>
Vector3D<TYPE> operator*(const Vector3D<TYPE>& left, const TYPE& right) throw();

template<class TYPE>
Vector3D<TYPE> operator*(const TYPE& left, const Vector3D<TYPE>& right) throw();

template<class TYPE>
Vector3D<TYPE> operator/(const Vector3D<TYPE>& left, const TYPE& right) throw();

/**
  Returns the dot product of the two vectors.
*/
template<class TYPE>
TYPE dot(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) throw();

/**
  Returns the cross product of the two vectors.
*/
template<class TYPE>
Vector3D<TYPE> cross(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) throw();

/**
  Writes a string representation of a Vector3D object to a format stream. The format is "(x, y, z)".
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector3D<TYPE>& value);

#endif
