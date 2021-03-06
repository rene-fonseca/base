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
#include <base/math/Math.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A two-dimensional vector represented by X and Y coordinates.

  @short Two-dimensional vector.
  @ingroup math
  @version 1.1
*/

template<class TYPE>
class Vector2D {
protected:

  /** The X coordinate. */
  TYPE x = 0;
  /** The Y coordinate. */
  TYPE y = 0;
public:

  /**
    Initializes vector as the origin (0,0).
  */
  inline Vector2D() noexcept {
  }

  /**
    Initializes vector from the specified coordinates.

    @param x The desired X coordinate.
    @param y The desired Y coordinate.
  */
  Vector2D(const TYPE& x, const TYPE& y) noexcept;

  /**
    Initializes vector by copying from the specified vector.

    @param copy The desired vector.
  */
  inline Vector2D(const Vector2D& copy) noexcept
    : x(copy.x), y(copy.y) {
  }

  /**
    Assignment of this vector from vector.
  */
  inline Vector2D& operator=(const Vector2D& assign) noexcept {
    x = assign.x; // no need to protect against self-assignment
    y = assign.y;
    return *this;
  }

  /**
    Returns the square of the modulus of the vector.
  */
  inline TYPE getSqrModulus() const noexcept {
    return x * x + y * y;
  }

  /**
    Returns the modulus of the vector.
  */
  inline TYPE getModulus() const noexcept {
    return Math::sqrt(getSqrModulus());
  }

  /**
    Returns the angle of the vector.
  */
  inline TYPE getAngle() const noexcept {
    return Math::atan2(y, x);
  }

  /**
    Returns the X coordinate.
  */
  inline TYPE getX() const noexcept {
    return x;
  }

  /**
    Returns the Y coordinate.
  */
  inline TYPE getY() const noexcept {
    return y;
  }

  /**
    Sets the X coordinate.

    @param x The desired X coordinate.
  */
  inline void setX(const TYPE& x) noexcept {
    this->x = x;
  }

  /**
    Sets the Y coordinate.

    @param y The desired Y coordinate.
  */
  inline void setY(const TYPE& y) noexcept {
    this->y = y;
  }

  /**
    Sets values less than the specified value to zero.
  */
  Vector2D& zeroAdjust(const TYPE& zero) noexcept {
    if (x < zero) {
      x = TYPE(0);
    }
    if (y < zero) {
      y = TYPE(0);
    }
    return *this;
  }

  /**
    Returns true if the length of this vector is zero.
  */
  inline bool isZero() const noexcept {
    return (x == TYPE(0)) && (y == TYPE(0));
  }

  /**
    Returns true if the length of this vector is greater zero.
  */
  inline bool isProper() const noexcept {
    return (x != TYPE(0)) || (y != TYPE(0));
  }

  /**
    Returns true if this vector is orthogonal with the specified vector.
  */
  inline bool isOrthogonal(const Vector2D& value) const noexcept {
    return dot(value) == 0;
  }

  /**
    Returns true if this vector is parallel with the specified vector.
  */
  inline bool isParallel(const Vector2D& value) const noexcept {
    return determinant(value) == 0;
  }

  /**
    Unary plus.
  */
  inline Vector2D plus() const noexcept {
    return Vector2D(*this);
  }

  /**
    Unary minus.
  */
  inline Vector2D minus() const noexcept {
    return Vector2D(*this).negate();
  }

  /**
    Negates this vector.
  */
  inline Vector2D& negate() noexcept {
    x = -x;
    y = -y;
    return *this;
  }

  /**
    Adds the specified vector to this vector.
  */
  inline Vector2D& add(const Vector2D& value) noexcept {
    x += value.x;
    y += value.y;
    return *this;
  }

  /**
    Subtracts the specified vector from this vector.
  */
  inline Vector2D& subtract(const Vector2D& value) noexcept {
    x -= value.x;
    y -= value.y;
    return *this;
  }

  /**
    Multiplies this vector with the specified value.
  */
  inline Vector2D& multiply(const TYPE& value) noexcept {
    x *= value;
    y *= value;
    return *this;
  }

  /**
    Divides this vector with the specified value.
  */
  inline Vector2D& divide(const TYPE& value) noexcept {
    x /= value;
    y /= value;
    return *this;
  }

  /**
    Returns the dot product of this vector and the specified vector.
  */
  inline TYPE dot(const Vector2D& value) const noexcept {
    return x * value.x + y * value.y;
  }

  /**
    Returns the determinant of this vector and the specified vector.
  */
  inline TYPE determinant(const Vector2D& value) const noexcept {
    return x * value.y - y * value.x;
  }

  /**
    Returns the angle between this vector and the specified vector.
  */
  TYPE getAngle(const Vector2D& value) const noexcept {
    TYPE temp = dot(value)/Math::sqrt(getSqrModulus() * value.getSqrModulus());
    return Math::atan2(Math::sqrt(1 - temp * temp), temp);
  }

  /**
    Returns the projection of this vector onto the specified vector.
  */
  inline Vector2D getProjection(const Vector2D& value) const noexcept {
    return value * dot(value)/value.getSqrModulus();
  }



  /**
    Returns true if the vectors are equal.

    @param vector Vector to be compared.
  */
  inline bool operator==(const Vector2D& value) const noexcept {
    return (x == value.x) && (y == value.y);
  }

  /**
    Adds the specified vector from this vector.

    @param value The value to be added.
  */
  inline Vector2D& operator+=(const Vector2D& value) noexcept {
    return add(value);
  }

  /**
    Subtracts the specified vector from this vector.

    @param value The value to be subtracted.
  */
  inline Vector2D& operator-=(const Vector2D& value) noexcept {
    return subtract(value);
  }

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  inline Vector2D& operator*=(const TYPE& value) noexcept {
    return multiply(value);
  }

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  inline Vector2D& operator/=(const TYPE& value) noexcept {
    return divide(value);
  }

  /**
    Unary plus.
  */
  inline Vector2D operator+() const noexcept {
    return plus();
  }

  /**
    Unary minus.
  */
  inline Vector2D operator-() const noexcept {
    return minus();
  }


#if 0
  /**
    Returns the product of the vector and the value.
  */
  friend Vector2D operator* <>(const Vector2D& left, const TYPE& right) noexcept;

  /**
    Returns the product of the vector and the value.
  */
  friend Vector2D operator* <>(const TYPE& left, const Vector2D& right) noexcept;

  /**
    Returns the result of the vector divided by the value.
  */
  friend Vector2D operator/ <>(const Vector2D& left, const TYPE& right) noexcept;
#endif
};

template<class TYPE>
inline Vector2D<TYPE>::Vector2D(const TYPE& _x, const TYPE& _y) noexcept
  : x(_x), y(_y) {
}

/**
  Returns the sum of the vectors.

  @relates Vector2D
*/
template<class TYPE>
inline Vector2D<TYPE> operator+(
  const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) noexcept {
  return Vector2D<TYPE>(left).add(right);
}

/**
  Returns the difference of the vectors.

  @relates Vector2D
*/
template<class TYPE>
inline Vector2D<TYPE> operator-(
  const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) noexcept {
  return Vector2D<TYPE>(left).subtract(right);
}

/**
  Returns the product of the vector and the value.

  @relates Vector2D
*/
template<class TYPE>
inline Vector2D<TYPE> operator*(
  const Vector2D<TYPE>& left, const TYPE& right) noexcept {
  return Vector2D<TYPE>(left).multiply(right);
}

/**
  Returns the product of the vector and the value.

  @relates Vector2D
*/
template<class TYPE>
inline Vector2D<TYPE> operator*(
  const TYPE& left, const Vector2D<TYPE>& right) noexcept {
  return Vector2D<TYPE>(right).multiply(left);
}

/**
  Returns the ratio of the vector and the value.

  @relates Vector2D
*/
template<class TYPE>
inline Vector2D<TYPE> operator/(
  const Vector2D<TYPE>& left, const TYPE& right) noexcept {
  return Vector2D<TYPE>(left).divide(right);
}

/**
  Returns the dot product of the two vectors.

  @relates Vector2D
*/
template<class TYPE>
inline TYPE dot(const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) noexcept {
  return left.dot(right);
}

/**
  Returns the determinant of the two vectors.

  @relates Vector2D
*/
template<class TYPE>
inline TYPE determinant(
  const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) noexcept {
  return left.determinant(right);
}

/**
  Writes a string representation of a Vector2D object to a format stream. The
  format is "(x; y)".

  @relates Vector2D
*/
template<class TYPE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Vector2D<TYPE>& value)
{
  return stream << '(' << value.getX() << ';' << value.getY() << ')';
}

template<class TYPE>
class IsUninitializeable<Vector2D<TYPE> > : public IsUninitializeable<TYPE> {
};

typedef Vector2D<float> fvector2;
typedef Vector2D<double> dvector2;
typedef Vector2D<long double> ldvector2;

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_TEMPLATE(Vector2D)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
