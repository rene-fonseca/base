/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__VECTOR2D_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__VECTOR2D_H

#include <base/string/FormatOutputStream.h>
#include <base/mathematics/Math.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A two-dimensional vector represented by X and Y coordinates.

  @short Two-dimensional vector.
  @ingroup mathematics
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.01
*/

template<class TYPE>
class Vector2D {
protected:

  /** The X coordinate. */
  TYPE x;
  /** The Y coordinate. */
  TYPE y;
public:

  /**
    Initializes vector as the origin (0,0).
  */
  inline Vector2D() throw() : x(0), y(0) {}

  /**
    Initializes vector from the specified coordinates.

    @param x The desired X coordinate.
    @param y The desired Y coordinate.
  */
  Vector2D(const TYPE& x, const TYPE& y) throw();

  /**
    Initializes vector by copying from the specified vector.

    @param copy The desired vector.
  */
  inline Vector2D(const Vector2D& copy) throw() : x(copy.x), y(copy.y) {}

  /**
    Assignment of this vector from vector.
  */
  inline Vector2D& operator=(const Vector2D& eq) throw() {
    x = eq.x; // no need to protect against self-assignment
    y = eq.y;
    return *this;
  }

  /**
    Returns the square of the modulus of the vector.
  */
  inline TYPE getSqrModulus() const throw() {
    return x * x + y * y;
  }

  /**
    Returns the modulus of the vector.
  */
  inline TYPE getModulus() const throw() {
    return Math::sqrt(getSqrModulus());
  }

  /**
    Returns the angle of the vector.
  */
  inline TYPE getAngle() const throw() {
    return Math::atan2(y, x);
  }

  /**
    Returns the X coordinate.
  */
  inline TYPE getX() const throw() {return x;}

  /**
    Returns the Y coordinate.
  */
  inline TYPE getY() const throw() {return y;}

  /**
    Sets the X coordinate.

    @param x The desired X coordinate.
  */
  inline void setX(const TYPE& x) throw() {this->x = x;}

  /**
    Sets the Y coordinate.

    @param y The desired Y coordinate.
  */
  inline void setY(const TYPE& y) throw() {this->y = y;}

  /**
    Sets values less than the specified value to zero.
  */
  Vector2D& zeroAdjust(const TYPE& zero) throw() {
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
  inline bool isZero() const throw() {
    return (x == TYPE(0)) && (y == TYPE(0));
  }

  /**
    Returns true if the length of this vector is greater zero.
  */
  inline bool isProper() const throw() {
    return (x != TYPE(0)) || (y != TYPE(0));
  }

  /**
    Returns true if this vector is orthogonal with the specified vector.
  */
  inline bool isOrthogonal(const Vector2D& value) const throw() {
    return dot(value) == 0;
  }

  /**
    Returns true if this vector is parallel with the specified vector.
  */
  inline bool isParallel(const Vector2D& value) const throw() {
    return determinant(value) == 0;
  }

  /**
    Unary plus.
  */
  inline Vector2D plus() const throw() {
    return Vector2D(*this);
  }

  /**
    Unary minus.
  */
  inline Vector2D minus() const throw() {
    return Vector2D(*this).negate();
  }

  /**
    Negates this vector.
  */
  inline Vector2D& negate() throw() {
    x = -x;
    y = -y;
    return *this;
  }

  /**
    Adds the specified vector to this vector.
  */
  inline Vector2D& add(const Vector2D& value) throw() {
    x += value.x;
    y += value.y;
    return *this;
  }

  /**
    Subtracts the specified vector from this vector.
  */
  inline Vector2D& subtract(const Vector2D& value) throw() {
    x -= value.x;
    y -= value.y;
    return *this;
  }

  /**
    Multiplies this vector with the specified value.
  */
  inline Vector2D& multiply(const TYPE& value) throw() {
    x *= value;
    y *= value;
    return *this;
  }

  /**
    Divides this vector with the specified value.
  */
  inline Vector2D& divide(const TYPE& value) throw() {
    x /= value;
    y /= value;
    return *this;
  }

  /**
    Returns the dot product of this vector and the specified vector.
  */
  inline TYPE dot(const Vector2D& value) const throw() {
    return x * value.x + y * value.y;
  }

  /**
    Returns the determinant of this vector and the specified vector.
  */
  inline TYPE determinant(const Vector2D& value) const throw() {
    return x * value.y - y * value.x;
  }

  /**
    Returns the angle between this vector and the specified vector.
  */
  TYPE getAngle(const Vector2D& value) const throw() {
    TYPE temp = dot(value)/Math::sqrt(getSqrModulus() * value.getSqrModulus());
    return Math::atan2(Math::sqrt(1 - temp * temp), temp);
  }

  /**
    Returns the projection of this vector onto the specified vector.
  */
  inline Vector2D getProjection(const Vector2D& value) const throw() {
    return value * dot(value)/value.getSqrModulus();
  }



  /**
    Returns true if the vectors are equal.

    @param vector Vector to be compared.
  */
  inline bool operator==(const Vector2D& value) const throw() {
    return (x == value.x) && (y == value.y);
  }

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
};

template<class TYPE>
inline Vector2D<TYPE>::Vector2D(const TYPE& _x, const TYPE& _y) throw() : x(_x), y(_y) {}

template<class TYPE>
inline Vector2D<TYPE> operator+(const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) throw() {
  return Vector2D<TYPE>(left).add(right);
}

template<class TYPE>
inline Vector2D<TYPE> operator-(const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) throw() {
  return Vector2D<TYPE>(left).subtract(right);
}

template<class TYPE>
inline Vector2D<TYPE> operator*(const Vector2D<TYPE>& left, const TYPE& right) throw() {
  return Vector2D<TYPE>(left).multiply(right);
}

template<class TYPE>
inline Vector2D<TYPE> operator*(const TYPE& left, const Vector2D<TYPE>& right) throw() {
  return Vector2D<TYPE>(right).multiply(left);
}

template<class TYPE>
inline Vector2D<TYPE> operator/(const Vector2D<TYPE>& left, const TYPE& right) throw() {
  return Vector2D<TYPE>(left).divide(right);
}

/**
  Returns the dot product of the two vectors.
*/
template<class TYPE>
inline TYPE dot(const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) throw() {
  return left.dot(right);
}

/**
  Returns the determinant of the two vectors.
*/
template<class TYPE>
inline TYPE determinant(const Vector2D<TYPE>& left, const Vector2D<TYPE>& right) throw() {
  return left.determinant(right);
}

/**
  Writes a string representation of a Vector2D object to a format stream. The format is "(x; y)".
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector2D<TYPE>& value) {
  return stream << '(' << value.getX() << ';' << value.getY() << ')';
}

template<class TYPE>
class Relocateable<Vector2D<TYPE> > {
public:

  static const bool IS_RELOCATEABLE = Relocateable<TYPE>::IS_RELOCATEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
