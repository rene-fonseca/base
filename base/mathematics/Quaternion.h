/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__QUATERNION_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__QUATERNION_H

#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A quaternion.

  @short Quaternion.
  @ingroup mathematics
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Quaternion {
protected:

  /** X coordinate. */
  TYPE x;
  /** Y coordinate. */
  TYPE y;
  /** Z coordinate. */
  TYPE z;
  /** w coordinate. */
  TYPE w;
public:

  /**
    Initializes quaternion as origin (0, 0, 0, 0).
  */
  inline Quaternion() throw() : x(0), y(0), z(0), w(0) {
  }

  /**
    Initializes quaternion from the specified coordinates.

    @param x The desired X coordinate.
    @param y The desired Y coordinate.
    @param z The desired Z coordinate.
    @param w The desired W coordinate.
  */
  Quaternion(const TYPE& x, const TYPE& y, const TYPE& z, const TYPE& w) throw();
  
  /**
    Initializes quaternion by copying from other quaternion.

    @param copy The desired quaternion.
  */
  inline Quaternion(const Quaternion& copy) throw()
    : x(copy.x), y(copy.y), z(copy.z), w(copy.w) {
  }

  /**
    Assignment of quaternion to quaternion.
  */
  inline Quaternion& operator=(const Quaternion& eq) throw() {
    x = eq.x; // no need to protect against self-assignment
    y = eq.y;
    z = eq.z;
    w = eq.w;
    return *this;
  }

  /**
    Returns the X coordinate of the quaternion.
  */
  inline TYPE getX() const throw() {
    return x;
  }

  /**
    Returns the Y coordinate of the quaternion.
  */
  inline TYPE getY() const throw() {
    return y;
  }

  /**
    Returns the Z coordinate of the quaternion.
  */
  inline TYPE getZ() const throw() {
    return z;
  }

  /**
    Returns the W coordinate of the quaternion.
  */
  inline TYPE getW() const throw() {
    return w;
  }
  
  /**
    Sets the X coordinate of the quaternion.

    @param x The desired X coordinate.
  */
  inline void setX(const TYPE& x) throw() {
    this->x = x;
  }

  /**
    Sets the Y coordinate of the quaternion.

    @param y The desired Y coordinate.
  */
  inline void setY(const TYPE& y) throw() {
    this->y = y;
  }

  /**
    Sets the Z coordinate of the quaternion.

    @param z The desired Z coordinate.
  */
  inline void setZ(const TYPE& z) throw() {
    this->z = z;
  }

  /**
    Sets the W coordinate of the quaternion.

    @param w The desired W coordinate.
  */
  inline void setW(const TYPE& w) throw() {
    this->w = w;
  }

  /**
    Sets values less than the specified value to zero.
  */
  Quaternion& zeroAdjust(const TYPE& zero) throw() {
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
  inline bool isZero() const throw() {
    const TYPE zero(0);
    return (x == zero) && (y == zero) && (z == zero) && (w == zero);
  }

  /**
    Returns true if the length of this quaternion is greater zero.
  */
  inline bool isProper() const throw() {
    const TYPE zero(0);
    return (x != zero) || (y != zero) || (z != zero) || (w != zero);
  }

  /**
    Unary plus.
  */
  Quaternion plus() const throw() {
    return Quaternion(*this);
  }

  /**
    Unary minus.
  */
  Quaternion minus() const throw() {
    return Quaternion(*this).negate();
  }

  /**
    Negates the quaternion.
  */
  Quaternion& negate() throw() {
    x = -x;
    y = -y;
    z = -z;
    w = -w;
    return *this;
  }

  /**
    Adds the specified quaternion to this quaternion.
  */
  Quaternion& add(const Quaternion& value) throw() {
    x += value.x;
    y += value.y;
    z += value.z;
    w += value.w;
    return *this;
  }

  /**
    Subtracts the specified quaternion from this quaternion.
  */
  Quaternion& subtract(const Quaternion& value) throw() {
    x -= value.x;
    y -= value.y;
    z -= value.z;
    w -= value.w;
    return *this;
  }

  /**
    Multiplies this quaternion with the specified value.
  */
  Quaternion& multiply(const TYPE& value) throw() {
    x *= value;
    y *= value;
    z *= value;
    w *= value;
    return *this;
  }

  /**
    Divides this quaternion with the specified value.
  */
  Quaternion& divide(const TYPE& value) throw() {
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
  inline bool operator==(const Quaternion& value) const throw() {
    return (x == value.x) && (y == value.y) && (z == value.z) && (w == value.w);
  }

  /**
    Adds the specified quaternion to this quaternion.

    @param value The value to be added.
  */
  inline Quaternion& operator+=(const Quaternion& value) throw() {
    return add(value);
  }

  /**
    Subtracts the specified quaternion from this quaternion.

    @param value The value to be subtracted.
  */
  inline Quaternion& operator-=(const Quaternion& value) throw() {
    return subtract(value);
  }

  /**
    Multiplies this quaternion with the specified value.

    @param value The multiplicator.
  */
  inline Quaternion& operator*=(const TYPE& value) throw() {
    return multiply(value);
  }

  /**
    Divides this quaternion with the specified value.

    @param value The divisor.
  */
  inline Quaternion& operator/=(const TYPE& value) throw() {
    return divide(value);
  }

  /**
    Unary plus.
  */
  inline Quaternion operator+() const throw() {
    return plus();
  }

  /**
    Unary minus.
  */
  inline Quaternion operator-() const throw() {
    return minus();
  }
};

template<class TYPE>
inline Quaternion<TYPE>::Quaternion(const TYPE& _x, const TYPE& _y, const TYPE& _z, const TYPE& _w) throw()
  : x(_x), y(_y), z(_z), w(_w) {
}

/**
  Returns the sum of the quaternion.
*/
template<class TYPE>
inline Quaternion<TYPE> operator+(const Quaternion<TYPE>& left, const Quaternion<TYPE>& right) throw() {
  return Quaternion<TYPE>(left).add(right);
}

/**
  Returns the difference of the quaternion.
*/
template<class TYPE>
inline Quaternion<TYPE> operator-(const Quaternion<TYPE>& left, const Quaternion<TYPE>& right) throw() {
  return Quaternion<TYPE>(left).subtract(right);
}

/**
  Returns the product of the quaternion and the value.
*/
template<class TYPE>
inline Quaternion<TYPE> operator*(const Quaternion<TYPE>& left, const TYPE& right) throw() {
  return Quaternion<TYPE>(left).multiply(right);
}

/**
  Returns the product of the quaternion and the value.
*/
template<class TYPE>
inline Quaternion<TYPE> operator*(const TYPE& left, const Quaternion<TYPE>& right) throw() {
  return Quaternion<TYPE>(right).multiply(left);
}

/**
  Returns the result of the quaternion divided by the value.
*/
template<class TYPE>
inline Quaternion<TYPE> operator/(const Quaternion<TYPE>& left, const TYPE& right) throw() {
  return Quaternion<TYPE>(left).divide(right);
}

/**
  Writes a string representation of a Quaternion object to a format stream. The format is "(x; y; z; w)".
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Quaternion<TYPE>& value) throw(IOException) {
  return stream << '(' << value.getX() << ';'
                << value.getY() << ';'
                << value.getZ() << ';'
                << value.getW() << ')';
}

template<class TYPE>
class Relocateable<Quaternion<TYPE> > {
public:

  static const bool IS_RELOCATEABLE = Relocateable<TYPE>::IS_RELOCATEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
