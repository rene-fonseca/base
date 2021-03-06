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

#include <base/string/FormatOutputStream.h>
#include <base/math/Vector2D.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A three-dimensional vector represented by X, Y, and Z coordinates.

  @short Three-dimensional vector.
  @ingroup math
  @version 1.1
*/

template<class TYPE>
class Vector3D {
protected:

  /** X coordinate. */
  TYPE x = 0;
  /** Y coordinate. */
  TYPE y = 0;
  /** Z coordinate. */
  TYPE z = 0;
public:

  /**
    Initializes vector as origin (0, 0, 0).
  */
  inline Vector3D() noexcept
  {
  }

  /**
    Initializes vector from the specified coordinates.

    @param x The desired X coordinate.
    @param y The desired Y coordinate.
    @param z The desired Z coordinate.
  */
  Vector3D(const TYPE& x, const TYPE& y, const TYPE& z) noexcept;
  
  /**
    Initializes vector by copying from the specified vector.

    @param vector The desired vector.
  */
  inline Vector3D(const Vector3D& copy) noexcept
    : x(copy.x), y(copy.y), z(copy.z)
  {
  }

  /**
    Assignment of this vector from vector.
  */
  inline Vector3D& operator=(const Vector3D& assign) noexcept
  {
    x = assign.x; // no need to protect against self-assignment
    y = assign.y;
    z = assign.z;
    return *this;
  }

  /**
    Returns the square of the modulus of the vector.
  */
  inline TYPE getSqrModulus() const noexcept
  {
    return x*x + y*y + z*z;
  }

  /**
    Returns the modulus of the vector.
  */
  inline TYPE getModulus() const noexcept
  {
    return Math::sqrt(getSqrModulus());
  }

  /**
    Returns the angle of this vector in the X-Y plane.
  */
  inline TYPE getXYAngle() const noexcept
  {
    return Math::atan2(y, x);
  }

  /**
    Returns the angle between this vector and the X-Y plane.
  */
  inline TYPE getZAngle() const noexcept
  {
    return Math::atan2(z, getModulus());
  }

  /**
    Returns the X coordinate of the vector.
  */
  inline TYPE getX() const noexcept
  {
    return x;
  }

  /**
    Returns the Y coordinate of the vector.
  */
  inline TYPE getY() const noexcept
  {
    return y;
  }

  /**
    Returns the Z coordinate of the vector.
  */
  inline TYPE getZ() const noexcept
  {
    return z;
  }

  /**
    Sets the X coordinate of the vector.

    @param x The desired X coordinate.
  */
  inline void setX(const TYPE& x) noexcept
  {
    this->x = x;
  }

  /**
    Sets the Y coordinate of the vector.

    @param y The desired Y coordinate.
  */
  inline void setY(const TYPE& y) noexcept
  {
    this->y = y;
  }

  /**
    Sets the Z coordinate of the vector.

    @param z The desired Z coordinate.
  */
  inline void setZ(const TYPE& z) noexcept
  {
    this->z = z;
  }

  /**
    Sets values less than the specified value to zero.
  */
  Vector3D& zeroAdjust(const TYPE& zero) noexcept
  {
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
  inline bool isZero() const noexcept
  {
    return (x == TYPE(0)) && (y == TYPE(0) && (z == TYPE(0)));
  }

  /**
    Returns true if the length of this vector is greater zero.
  */
  inline bool isProper() const noexcept
  {
    return (x != TYPE(0)) || (y != TYPE(0) || (z != TYPE(0)));
  }

  /**
    Returns true if this vector is orthogonal with the specified vector.
  */
  inline bool isOrthogonal(const Vector3D& value) const noexcept
  {
    return dot(value) == 0;
  }

  /**
    Returns true if this vector is parallel with the specified vector.
  */
  inline bool isParallel(const Vector3D& value) const noexcept
  {
    return cross(value).isZero();
  }

  /**
    Unary plus.
  */
  Vector3D plus() const noexcept
  {
    return Vector3D(*this);
  }

  /**
    Unary minus.
  */
  Vector3D minus() const noexcept
  {
    return Vector3D(*this).negate();
  }

  /**
    Negates this vector.
  */
  Vector3D& negate() noexcept
  {
    x = -x;
    y = -y;
    z = -z;
    return *this;
  }

  /**
    Adds the specified vector to this vector.
  */
  Vector3D& add(const Vector3D& value) noexcept
  {
    x += value.x;
    y += value.y;
    z += value.z;
    return *this;
  }

  /**
    Subtracts the specified vector from this vector.
  */
  Vector3D& subtract(const Vector3D& value) noexcept
  {
    x -= value.x;
    y -= value.y;
    z -= value.z;
    return *this;
  }

  /**
    Multiplies this vector with the specified value.
  */
  Vector3D& multiply(const TYPE& value) noexcept
  {
    x *= value;
    y *= value;
    z *= value;
    return *this;
  }

  /**
    Divides this vector with the specified value.
  */
  Vector3D& divide(const TYPE& value) noexcept
  {
    x /= value;
    y /= value;
    z /= value;
    return *this;
  }

  /**
    Returns the dot product of this vector and the specified vector.
  */
  inline TYPE dot(const Vector3D& value) const noexcept
  {
    return x * value.x + y * value.y + z * value.z;
  }

  /**
    Returns the cross product of this vector and the specified vector.
  */
  Vector3D cross(const Vector3D& value) const noexcept
  {
    return Vector3D(y * value.z - z * value.y, z * value.x - x * value.z, x * value.y - y * value.x);
  }

  /**
    Returns the angle between this vector and the specified vector.
  */
  TYPE getAngle(const Vector3D& value) const noexcept
  {
    // TAG: use return acos(temp);
    TYPE temp = dot(value)/Math::sqrt(getSqrModulus() * value.getSqrModulus());
    return Math::atan2(Math::sqrt(1 - temp * temp), temp);
  }

  /**
    Returns the projection of this vector onto the specified vector.
  */
  Vector3D getProjection(const Vector3D& value) const noexcept
  {
    return value * dot(value)/value.getSqrModulus();
  }




  /**
    Returns true if the vectors are equal.

    @param vector Vector to be compared.
  */
  inline bool operator==(const Vector3D& value) const noexcept
  {
    return (x == value.x) && (y == value.y) && (z == value.z);
  }

  /**
    Adds the specified vector from this vector.

    @param value The value to be added.
  */
  inline Vector3D& operator+=(const Vector3D& value) noexcept
  {
    return add(value);
  }

  /**
    Subtracts the specified vector from this vector.

    @param value The value to be subtracted.
  */
  inline Vector3D& operator-=(const Vector3D& value) noexcept
  {
    return subtract(value);
  }

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  inline Vector3D& operator*=(const TYPE& value) noexcept
  {
    return multiply(value);
  }

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  inline Vector3D& operator/=(const TYPE& value) noexcept
  {
    return divide(value);
  }

  /**
    Unary plus.
  */
  inline Vector3D operator+() const noexcept
  {
    return plus();
  }

  /**
    Unary minus.
  */
  inline Vector3D operator-() const noexcept
  {
    return minus();
  }
};

template<class TYPE>
inline Vector3D<TYPE>::Vector3D(const TYPE& _x, const TYPE& _y, const TYPE& _z) noexcept
  : x(_x), y(_y), z(_z)
{
}

/**
  Returns the sum of the vectors.

  @relates Vector3D
*/
template<class TYPE>
inline Vector3D<TYPE> operator+(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) noexcept
{
  return Vector3D<TYPE>(left).add(right);
}

/**
  Returns the difference of the vectors.

  @relates Vector3D
*/
template<class TYPE>
inline Vector3D<TYPE> operator-(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) noexcept
{
  return Vector3D<TYPE>(left).subtract(right);
}

/**
  Returns the product of the vector and the value.

  @relates Vector3D
*/
template<class TYPE>
inline Vector3D<TYPE> operator*(const Vector3D<TYPE>& left, const TYPE& right) noexcept
{
  return Vector3D<TYPE>(left).multiply(right);
}

/**
  Returns the product of the vector and the value.

  @relates Vector3D
*/
template<class TYPE>
inline Vector3D<TYPE> operator*(const TYPE& left, const Vector3D<TYPE>& right) noexcept
{
  return Vector3D<TYPE>(right).multiply(left);
}

/**
  Returns the result of the vector divided by the value.

  @relates Vector3D
*/
template<class TYPE>
inline Vector3D<TYPE> operator/(const Vector3D<TYPE>& left, const TYPE& right) noexcept
{
  return Vector3D<TYPE>(left).divide(right);
}

/**
  Returns the dot product of the two vectors.

  @relates Vector3D
*/
template<class TYPE>
inline TYPE dot(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) noexcept
{
  return left.dot(right);
}

/**
  Returns the cross product of the two vectors.

  @relates Vector3D
*/
template<class TYPE>
inline Vector3D<TYPE> cross(const Vector3D<TYPE>& left, const Vector3D<TYPE>& right) noexcept
{
  return left.cross(right);
}

/**
  Writes a string representation of a Vector3D object to a format stream. The
  format is "(x; y; z)".

  @relates Vector3D
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector3D<TYPE>& value)
{
  return stream << '('
                << value.getX() << ';'
                << value.getY() << ';'
                << value.getZ()
                << ')';
}

template<class TYPE>
class IsUninitializeable<Vector3D<TYPE> > : public IsUninitializeable<TYPE> {
};

typedef Vector3D<float> fvector3;
typedef Vector3D<double> dvector3;
typedef Vector3D<long double> ldvector3;

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_TEMPLATE(Vector3D)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
