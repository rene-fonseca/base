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
#include <base/math/Vector3D.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A quaternion.

  @short Quaternion.
  @ingroup math
  @version 1.0
*/

template<class TYPE>
class Quaternion {
protected:

  /** X coordinate. */
  TYPE x = 0;
  /** Y coordinate. */
  TYPE y = 0;
  /** Z coordinate. */
  TYPE z = 0;
  /** w coordinate. */
  TYPE w = 0;
public:

  /**
    Initializes quaternion as origin (0, 0, 0, 0).
  */
  inline Quaternion() noexcept {
  }

  /**
    Initializes quaternion from the specified coordinates.

    @param xyz The desired X, Y, Z coordinates.
    @param w The desired W coordinate.
  */
  inline Quaternion(const Vector3D<TYPE>& xyz, const TYPE& _w) noexcept
    : x(xyz.getX()), y(xyz.getY()), z(xyz.getZ()), w(_w)
  {
  }

  /**
    Initializes quaternion from the specified coordinates.

    @param x The desired X coordinate.
    @param y The desired Y coordinate.
    @param z The desired Z coordinate.
    @param w The desired W coordinate.
  */
  Quaternion(const TYPE& x, const TYPE& y, const TYPE& z, const TYPE& w) noexcept;
  
  /**
    Initializes quaternion by copying from other quaternion.

    @param copy The desired quaternion.
  */
  inline Quaternion(const Quaternion& copy) noexcept
    : x(copy.x), y(copy.y), z(copy.z), w(copy.w)
  {
  }

  /**
    Assignment of quaternion to quaternion.
  */
  inline Quaternion& operator=(const Quaternion& assign) noexcept
  {
    x = assign.x; // no need to protect against self-assignment
    y = assign.y;
    z = assign.z;
    w = assign.w;
    return *this;
  }

  /**
    Returns the X coordinate of the quaternion.
  */
  inline TYPE getX() const noexcept
  {
    return x;
  }

  /**
    Returns the Y coordinate of the quaternion.
  */
  inline TYPE getY() const noexcept
  {
    return y;
  }

  /**
    Returns the Z coordinate of the quaternion.
  */
  inline TYPE getZ() const noexcept
  {
    return z;
  }

  /**
    Returns the W coordinate of the quaternion.
  */
  inline TYPE getW() const noexcept
  {
    return w;
  }
  
  /**
    Sets the X coordinate of the quaternion.

    @param x The desired X coordinate.
  */
  inline void setX(const TYPE& x) noexcept
  {
    this->x = x;
  }

  /**
    Sets the Y coordinate of the quaternion.

    @param y The desired Y coordinate.
  */
  inline void setY(const TYPE& y) noexcept
  {
    this->y = y;
  }

  /**
    Sets the Z coordinate of the quaternion.

    @param z The desired Z coordinate.
  */
  inline void setZ(const TYPE& z) noexcept
  {
    this->z = z;
  }

  /**
    Sets the W coordinate of the quaternion.

    @param w The desired W coordinate.
  */
  inline void setW(const TYPE& w) noexcept
  {
    this->w = w;
  }

  /**
    Sets values less than the specified value to zero.
  */
  Quaternion& zeroAdjust(const TYPE& zero) noexcept
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
    if (w < zero) {
      w = TYPE(0);
    }
    return *this;
  }

  /**
    Returns true if the length of this quaternion is zero.
  */
  inline bool isZero() const noexcept
  {
    const TYPE zero(0);
    return (x == zero) && (y == zero) && (z == zero) && (w == zero);
  }

  /**
    Returns true if the length of this quaternion is greater zero.
  */
  inline bool isProper() const noexcept
  {
    const TYPE zero(0);
    return (x != zero) || (y != zero) || (z != zero) || (w != zero);
  }

  /**
    Unary plus.
  */
  Quaternion plus() const noexcept
  {
    return Quaternion(*this);
  }

  /**
    Unary minus.
  */
  Quaternion minus() const noexcept
  {
    return Quaternion(*this).negate();
  }

  /**
    Negates the quaternion.
  */
  Quaternion& negate() noexcept
  {
    x = -x;
    y = -y;
    z = -z;
    w = -w;
    return *this;
  }

  /**
    Adds the specified quaternion to this quaternion.
  */
  Quaternion& add(const Quaternion& value) noexcept
  {
    x += value.x;
    y += value.y;
    z += value.z;
    w += value.w;
    return *this;
  }

  /**
    Subtracts the specified quaternion from this quaternion.
  */
  Quaternion& subtract(const Quaternion& value) noexcept
  {
    x -= value.x;
    y -= value.y;
    z -= value.z;
    w -= value.w;
    return *this;
  }

  /**
    Multiplies this quaternion with the specified value.
  */
  Quaternion& multiply(const TYPE& value) noexcept
  {
    x *= value;
    y *= value;
    z *= value;
    w *= value;
    return *this;
  }

  /**
    Divides this quaternion with the specified value.
  */
  Quaternion& divide(const TYPE& value) noexcept
  {
    TYPE temp = TYPE(1)/value;
    x *= temp;
    y *= temp;
    z *= temp;
    w *= temp;
    return *this;
  }

  /**
    Returns true if the quaternions are equal.

    @param value Quaternion to be compared.
  */
  inline bool operator==(const Quaternion& value) const noexcept
  {
    return (x == value.x) && (y == value.y) && (z == value.z) && (w == value.w);
  }

  /**
    Adds the specified quaternion to this quaternion.

    @param value The value to be added.
  */
  inline Quaternion& operator+=(const Quaternion& value) noexcept
  {
    return add(value);
  }

  /**
    Subtracts the specified quaternion from this quaternion.

    @param value The value to be subtracted.
  */
  inline Quaternion& operator-=(const Quaternion& value) noexcept
  {
    return subtract(value);
  }

  /**
    Multiplies this quaternion with the specified value.

    @param value The multiplicator.
  */
  inline Quaternion& operator*=(const TYPE& value) noexcept
  {
    return multiply(value);
  }

  /**
    Divides this quaternion with the specified value.

    @param value The divisor.
  */
  inline Quaternion& operator/=(const TYPE& value) noexcept
  {
    return divide(value);
  }

  /**
    Unary plus.
  */
  inline Quaternion operator+() const noexcept
  {
    return plus();
  }

  /**
    Unary minus.
  */
  inline Quaternion operator-() const noexcept
  {
    return minus();
  }
};

template<class TYPE>
inline Quaternion<TYPE>::Quaternion(
  const TYPE& _x, const TYPE& _y, const TYPE& _z, const TYPE& _w) noexcept
  : x(_x), y(_y), z(_z), w(_w)
{
}

/**
  Returns the sum of the quaternion.

  @relates Quaternion
*/
template<class TYPE>
inline Quaternion<TYPE> operator+(
  const Quaternion<TYPE>& left, const Quaternion<TYPE>& right) noexcept
{
  return Quaternion<TYPE>(left).add(right);
}

/**
  Returns the difference of the quaternion.

  @relates Quaternion
*/
template<class TYPE>
inline Quaternion<TYPE> operator-(
  const Quaternion<TYPE>& left, const Quaternion<TYPE>& right) noexcept
{
  return Quaternion<TYPE>(left).subtract(right);
}

/**
  Returns the product of the quaternion and the value.

  @relates Quaternion
*/
template<class TYPE>
inline Quaternion<TYPE> operator*(const Quaternion<TYPE>& left, const TYPE& right) noexcept
{
  return Quaternion<TYPE>(left).multiply(right);
}

/**
  Returns the product of the quaternion and the value.

  @relates Quaternion
*/
template<class TYPE>
inline Quaternion<TYPE> operator*(const TYPE& left, const Quaternion<TYPE>& right) noexcept
{
  return Quaternion<TYPE>(right).multiply(left);
}

/**
  Returns the result of the quaternion divided by the value.

  @relates Quaternion
*/
template<class TYPE>
inline Quaternion<TYPE> operator/(const Quaternion<TYPE>& left, const TYPE& right) noexcept
{
  return Quaternion<TYPE>(left).divide(right);
}

/**
  Returns the dot product of the two quaternions.

  @relates Quaternion
*/
template<class TYPE>
inline TYPE dot(const Quaternion<TYPE>& left, const Quaternion<TYPE>& right) noexcept
{
  return left.getX() * right.getX() + left.getY() * right.getY() + left.getZ() * right.getZ() +
    left.getW() * right.getW();
}

/**
  Writes a string representation of a Quaternion object to a format stream. The
  format is "(x; y; z; w)".

  @relates Quaternion
*/
template<class TYPE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Quaternion<TYPE>& value)
{
  return stream << '(' << value.getX() << ';'
                << value.getY() << ';'
                << value.getZ() << ';'
                << value.getW() << ')';
}

template<class TYPE>
class IsUninitializeable<Quaternion<TYPE> > : public IsUninitializeable<TYPE> {
};

typedef Quaternion<float> fquaternion;
typedef Quaternion<double> dquaternion;
typedef Quaternion<long double> ldquaternion;

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_TEMPLATE(Quaternion)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
