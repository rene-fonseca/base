/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__VECTOR3D_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__VECTOR3D_H

#include <base/string/FormatOutputStream.h>
#include <base/mathematics/Vector2D.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A three-dimensional vector represented by X, Y, and Z coordinates.

  @short Three-dimensional vector.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.01
*/

template<class TYPE>
class Vector3D {
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
  inline Vector3D() throw() : x(0), y(0), z(0) {}

  /**
    Initializes vector from the specified coordinates.

    @param x The desired X coordinate.
    @param y The desired Y coordinate.
    @param z The desired Z coordinate.
  */
  Vector3D(const TYPE& x, const TYPE& y, const TYPE& z) throw();
  
  /**
    Initializes vector by copying from the specified vector.

    @param vector The desired vector.
  */
  inline Vector3D(const Vector3D& copy) throw() : x(copy.x), y(copy.y), z(copy.z) {}

  /**
    Assignment of this vector from vector.
  */
  inline Vector3D& operator=(const Vector3D& eq) throw() {
    x = eq.x; // no need to protect against self-assignment
    y = eq.y;
    z = eq.z;
    return *this;
  }

  /**
    Returns the square of the modulus of the vector.
  */
  inline TYPE getSqrModulus() const throw() {return x*x + y*y + z*z;}

  /**
    Returns the modulus of the vector.
  */
  inline TYPE getModulus() const throw() {
    return sqrt(getSqrModulus());
  }

  /**
    Returns the angle of this vector in the X-Y plane.
  */
  inline TYPE getXYAngle() const throw() {
    return atan2(y, x);
  }

  /**
    Returns the angle between this vector and the X-Y plane.
  */
  inline TYPE getZAngle() const throw() {
    return atan2(z, getModulus());
  }

  /**
    Returns the X coordinate of the vector.
  */
  inline TYPE getX() const throw() {return x;}

  /**
    Returns the Y coordinate of the vector.
  */
  inline TYPE getY() const throw() {return y;}

  /**
    Returns the Z coordinate of the vector.
  */
  inline TYPE getZ() const throw() {return z;}

  /**
    Sets the X coordinate of the vector.

    @param x The desired X coordinate.
  */
  inline void setX(const TYPE& x) throw() {this->x = x;}

  /**
    Sets the Y coordinate of the vector.

    @param y The desired Y coordinate.
  */
  inline void setY(const TYPE& y) throw() {this->y = y;}

  /**
    Sets the Z coordinate of the vector.

    @param z The desired Z coordinate.
  */
  inline void setZ(const TYPE& z) throw() {this->z = z;}

  /**
    Sets values less than the specified value to zero.
  */
  Vector3D& zeroAdjust(const TYPE& zero) throw() {
    if (x < zero) {
      x = TYPE(0);
    }
    if (y < zero) {
      y = TYPE(0);
    }
    if (z < zero) {
      z = TYPE(0);
    }
    return *this;
  }

  /**
    Returns true if the length of this vector is zero.
  */
  inline bool isZero() const throw() {
    return (x == TYPE(0)) && (y == TYPE(0) && (z == TYPE(0)));
  }

  /**
    Returns true if the length of this vector is greater zero.
  */
  inline bool isProper() const throw() {
    return (x != TYPE(0)) || (y != TYPE(0) || (z != TYPE(0)));
  }

  /**
    Returns true if this vector is orthogonal with the specified vector.
  */
  inline bool isOrthogonal(const Vector3D& value) const throw() {
    return dot(value) == 0;
  }

  /**
    Returns true if this vector is parallel with the specified vector.
  */
  inline bool isParallel(const Vector3D& value) const throw() {
    return cross(value).isZero();
  }

  /**
    Unary plus.
  */
  Vector3D plus() const throw() {
    return Vector3D(*this);
  }

  /**
    Unary minus.
  */
  Vector3D minus() const throw() {
    return Vector3D(*this).negate();
  }

  /**
    Negates this vector.
  */
  Vector3D& negate() throw() {
    x = -x;
    y = -y;
    z = -z;
    return *this;
  }

  /**
    Adds the specified vector to this vector.
  */
  Vector3D& add(const Vector3D& value) throw() {
    x += value.x;
    y += value.y;
    z += value.z;
    return *this;
  }

  /**
    Subtracts the specified vector from this vector.
  */
  Vector3D& subtract(const Vector3D& value) throw() {
    x -= value.x;
    y -= value.y;
    z -= value.z;
    return *this;
  }

  /**
    Multiplies this vector with the specified value.
  */
  Vector3D& multiply(const TYPE& value) throw() {
    x *= value;
    y *= value;
    z *= value;
    return *this;
  }

  /**
    Divides this vector with the specified value.
  */
  Vector3D& divide(const TYPE& value) throw() {
    x /= value;
    y /= value;
    z /= value;
    return *this;
  }

  /**
    Returns the dot product of this vector and the specified vector.
  */
  inline TYPE dot(const Vector3D& value) const throw() {
    return x * value.x + y * value.y + z * value.z;
  }

  /**
    Returns the cross product of this vector and the specified vector.
  */
  Vector3D cross(const Vector3D& value) const throw() {
    return Vector3D(y * value.z - z * value.y, z * value.x - x * value.z, x * value.y - y * value.x);
  }

  /**
    Returns the angle between this vector and the specified vector.
  */
  TYPE getAngle(const Vector3D& value) const throw() {
    // TAG: use return acos(temp);
    TYPE temp = dot(value)/sqrt(getSqrModulus() * value.getSqrModulus());
    return atan2(sqrt(1 - temp * temp), temp);
  }

  /**
    Returns the projection of this vector onto the specified vector.
  */
  Vector3D getProjection(const Vector3D& value) const throw() {
    return value * dot(value)/value.getSqrModulus();
  }




  /**
    Returns true if the vectors are equal.

    @param vector Vector to be compared.
  */
  inline bool operator==(const Vector3D& value) const throw() {
    return (x == value.x) && (y == value.y) && (z == value.z);
  }

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
};

template<class TYPE>
inline Vector3D<TYPE>::Vector3D(const TYPE& _x, const TYPE& _y, const TYPE& _z) throw() : x(_x), y(_y), z(_z) {
}

/**
  Returns the sum of the vectors.
*/
template<class TYPE>
inline Vector3D<TYPE> operator+(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) throw() {
  return Vector3D<TYPE>(left).add(right);
}

/**
  Returns the difference of the vectors.
*/
template<class TYPE>
inline Vector3D<TYPE> operator-(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) throw() {
  return Vector3D<TYPE>(left).subtract(right);
}

/**
  Returns the product of the vector and the value.
*/
template<class TYPE>
inline Vector3D<TYPE> operator*(const Vector3D<TYPE>& left, const TYPE& right) throw() {
  return Vector3D<TYPE>(left).multiply(right);
}

/**
  Returns the product of the vector and the value.
*/
template<class TYPE>
inline Vector3D<TYPE> operator*(const TYPE& left, const Vector3D<TYPE>& right) throw() {
  return Vector3D<TYPE>(right).multiply(left);
}

/**
  Returns the result of the vector divided by the value.
*/
template<class TYPE>
inline Vector3D<TYPE> operator/(const Vector3D<TYPE>& left, const TYPE& right) throw() {
  return Vector3D<TYPE>(left).divide(right);
}

/**
  Returns the dot product of the two vectors.
*/
template<class TYPE>
inline TYPE dot(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) throw() {
  return left.dot(right);
}

/**
  Returns the cross product of the two vectors.
*/
template<class TYPE>
inline Vector3D<TYPE> cross(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) throw() {
  return left.cross(right);
}

/**
  Writes a string representation of a Vector3D object to a format stream. The format is "(x; y; z)".
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector3D<TYPE>& value) {
  return stream << '(' << value.getX() << ';' << value.getY() << ';' << value.getZ() << ')';
}

template<class TYPE>
class Relocateable<Vector3D<TYPE> > {
public:

  static const bool IS_RELOCATEABLE = Relocateable<TYPE>::IS_RELOCATEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
