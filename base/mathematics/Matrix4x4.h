/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__MATRIX4X4_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__MATRIX4X4_H

#include <base/string/FormatOutputStream.h>
#include <base/Functor.h>
#include <base/mathematics/Vector3D.h>
#include <base/mathematics/Quaternion.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/** Sums the absolute value of the elements in a sequence. */
template<class TYPE, class RESULT = TYPE>
class AbsoluteSum : public UnaryOperation<TYPE, RESULT> {
protected:
  
  RESULT result;
public:
  
  inline AbsoluteSum() throw()
    : result(0) {
  }
  
  inline void operator()(const TYPE& value) throw() {
    result += absolute(value);
  }
  
  inline RESULT getResult() const throw() {
    return result;
  }
};

/**
  A 4 by 4 matrix with elements storage organized in row major and column minor.

  @short 4 by 4 matrix.
  @ingroup mathematics
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Matrix4x4 {
protected:

  /** The elements of the matrix (row by column). */
  TYPE elements[4][4];
public:

  /**
    Construct an unitialized matrix.
  */
  inline Matrix4x4() throw() {
  }
  
  /**
    Initializes matrix by copying from other matrix.
  */
  inline Matrix4x4(const Matrix4x4& cpy) throw() {
    // no need to protect against self-assignment (possible overlap)
    copy<TYPE>(Cast::pointer<TYPE*>(elements), Cast::pointer<const TYPE*>(cpy.elements), 16);
  }

  /**
    Assignment of matrix to matrix.
  */
  inline Matrix4x4& operator=(const Matrix4x4& eq) throw() {
    // no need to protect against self-assignment (possible overlap)
    copy<TYPE>(Cast::pointer<TYPE*>(elements), Cast::pointer<const TYPE*>(eq.elements), 16);
    return *this;
  }

  inline TYPE getAt(unsigned int row, unsigned int column) const throw() {
    // TAG: check row and column
    return elements[row][column];
  }

  inline void setAt(unsigned int row, unsigned int column, const TYPE& value) throw() {
    // TAG: check row and column
    elements[row][column] = value;
  }
  
  inline TYPE* getElements() throw() {
    return reinterpret_cast<TYPE*>(&elements);
  }

  inline const TYPE* getElements() const throw() {
    return reinterpret_cast<const TYPE*>(&elements);
  }

  bool isEqual(const Matrix4x4& value) const throw() {
    return equal(elements, value.elements, 16);
  }

  /**
    Returns true if the length of this vector is zero.
  */
  bool isZero() const throw() {
    return !findPredicate(elements, 16, bind2Second(NotEqual<TYPE>(), TYPE(0)));
  }

  bool isDiagonal() const throw() {
    const TYPE zero(0);
    return (elements[0][1] == zero) && (elements[0][2] == zero) && (elements[0][3] == zero) &&
      (elements[1][0] == zero) && (elements[1][2] == zero) && (elements[1][3] == zero) &&
      (elements[2][0] == zero) && (elements[2][1] == zero) && (elements[2][3] == zero) &&
      (elements[3][0] == zero) && (elements[3][1] == zero) && (elements[3][2] == zero);
  }
  
  bool isSymmetric() const throw() {
    return (elements[0][1] == elements[1][0]) && (elements[0][2] == elements[2][0]) && (elements[0][3] == zelements[3][0]) &&
      (elements[1][2] == elements[2][1]) && (elements[1][3] == elements[3][1]) &&
      (elements[2][3] == elements[3][2]);
  }

  /**
    Returns the L1-norm of the matrix.
  */
  TYPE getL1Norm() const throw() {
    AbsoluteSum<TYPE> absoluteSum;
    forEach(elements, 16, absoluteSum);
    return absoluteSum.getResult();
  }
  
  /**
    Returns the square of the Hilbert-Schmidt norm of the matrix.
  */
  TYPE getSquareHSNorm() const throw() {
    SquareSum<TYPE> squareSum;
    forEach(elements, 16, squareSum);
    return squareSum.getResult();
  }

  /**
    Returns the infinity norm of the matrix.
  */
  TYPE getInfinityNorm() const throw() {
    Maximum<TYPE> maximum;
    forEach(elements, 16, maximum);
    return maximum.getResult();
  }

  /**
    Returns the determinant of the matrix.
  */
  TYPE getDeterminant() const throw() {
    const TYPE* src = elements;
    
    // 2x2 determinants for the last two rows
    TYPE det01 = src[2*4+0] * src[3*4+1] - src[2*4+1] * src[3*4+0];
    TYPE det02 = src[2*4+0] * src[3*4+2] - src[2*4+2] * src[3*4+0];
    TYPE det12 = src[2*4+1] * src[3*4+2] - src[2*4+2] * src[3*4+1];
    TYPE det13 = src[2*4+1] * src[3*4+3] - src[2*4+3] * src[3*4+1];
    TYPE det23 = src[2*4+2] * src[3*4+3] - src[2*4+3] * src[3*4+2];
    TYPE det30 = src[2*4+3] * src[3*4+0] - src[2*4+0] * src[3*4+3];

    // 3x3 determinants for the last three rows
    TYPE det0 = (src[1*4+1] * det23 - src[1*4+2] * det13 + src[1*4+3] * det12);
    TYPE det1 = -(src[1*4+0] * det23 + src[1*4+2] * det30 + src[1*4+3] * det02);
    TYPE det2 = (src[1*4+0] * det13 + src[1*4+1] * det30 + src[1*4+3] * det01);
    TYPE det3 = -(src[1*4+0] * det12 - src[1*4+1] * det02 + src[1*4+2] * det01);

    return src[0*4+0] * det0 + src[0*4+1] * det1 + src[0*4+2] * det2 + src[0*4+3] * det3;
  }

  /**
    Inverses the specified matrix.
  */
  Matrix4x4& inverse(const Matrix4x4& value) throw() {
    TYPE* dest = getElements();
    const TYPE* src = value.getElements();
    
    // 2x2 determinants for the last two rows
    TYPE det01 = src[2*4+0] * src[3*4+1] - src[2*4+1] * src[3*4+0];
    TYPE det02 = src[2*4+0] * src[3*4+2] - src[2*4+2] * src[3*4+0];
    TYPE det12 = src[2*4+1] * src[3*4+2] - src[2*4+2] * src[3*4+1];
    TYPE det13 = src[2*4+1] * src[3*4+3] - src[2*4+3] * src[3*4+1];
    TYPE det23 = src[2*4+2] * src[3*4+3] - src[2*4+3] * src[3*4+2];
    TYPE det30 = src[2*4+3] * src[3*4+0] - src[2*4+0] * src[3*4+3];

    dest[0] = (src[1*4+1] * det23 - src[1*4+2] * det13 + src[1*4+3] * det12);
    dest[1] = -(src[1*4+0] * det23 + src[1*4+2] * det30 + src[1*4+3] * det02);
    dest[2] = (src[1*4+0] * det13 + src[1*4+1] * det30 + src[1*4+3] * det01);
    dest[3] = -(src[1*4+0] * det12 - src[1*4+1] * det02 + src[1*4+2] * det01);

    TYPE determinant = src[0*4+0] * dest[0] + src[0*4+1] * dest[1] + src[0*4+2] * dest[2] + src[0*4+3] * dest[3];

    // TAG: should we raise an exception if singular
    if (determinant != TYPE(0)) { // check for singularity
      TYPE inverseDeterminant = TYPE(1)/determinant;
      dest[0*4+0] *= inverseDeterminant;
      dest[0*4+1] *= inverseDeterminant;
      dest[0*4+2] *= inverseDeterminant;
      dest[0*4+3] *= inverseDeterminant;
      
      dest[1*4+0] = -(src[0*4+1] * det23 - src[0*4+2] * det13 + src[0*4+3] * det12) * inverseDeterminant;
      dest[1*4+1] = (src[0*4+0] * det23 + src[0*4+2] * det30 + src[0*4+3] * det02) * inverseDeterminant;
      dest[1*4+2] = -(src[0*4+0] * det13 + src[0*4+1] * det30 + src[0*4+3] * det01) * inverseDeterminant;
      dest[1*4+3] = (src[0*4+0] * det12 - src[0*4+1] * det02 + src[0*4+2] * det01) * inverseDeterminant;
      
      // 2x2 determinants for the first two rows
      det01 = src[0*4+0] * src[1*4+1] - src[0*4+1] * src[1*4+0];
      det02 = src[0*4+0] * src[1*4+2] - src[0*4+2] * src[1*4+0];
      det12 = src[0*4+1] * src[1*4+2] - src[0*4+2] * src[1*4+1];
      det13 = src[0*4+1] * src[1*4+3] - src[0*4+3] * src[1*4+1];
      det23 = src[0*4+2] * src[1*4+3] - src[0*4+3] * src[1*4+2];
      det30 = src[0*4+3] * src[1*4+0] - src[0*4+0] * src[1*4+3];
      
      dest[2*4+0] = (src[3*4+1] * det23 - src[3*4+2] * det13 + src[3*4+3] * det12) * inverseDeterminant;
      dest[2*4+1] = -(src[3*4+0] * det23 + src[3*4+2] * det30 + src[3*4+3] * det02) * inverseDeterminant;
      dest[2*4+2] = (src[3*4+0] * det13 + src[3*4+1] * det30 + src[3*4+3] * det01) * inverseDeterminant;
      dest[2*4+3] = -(src[3*4+0] * det12 - src[3*4+1] * det02 + src[3*4+2] * det01) * inverseDeterminant;
      dest[3*4+0] = -(src[2*4+1] * det23 - src[2*4+2] * det13 + src[2*4+3] * det12) * inverseDeterminant;
      dest[3*4+1] = (src[2*4+0] * det23 + src[2*4+2] * det30 + src[2*4+3] * det02) * inverseDeterminant;
      dest[3*4+2] = -(src[2*4+0] * det13 + src[2*4+1] * det30 + src[2*4+3] * det01) * inverseDeterminant;
      dest[3*4+3] = (src[2*4+0] * det12 - src[2*4+1] * det02 + src[2*4+2] * det01) * inverseDeterminant;
    }
    return *this;
  }

  /**
    Inverses the matrix.
  */
  Matrix4x4& inverse() throw() {
    Matrix4x4 temp(*this);
    inverse(temp);
    return *this;
  }
  
  /**
    Unary plus.
  */
  Matrix4x4 plus() const throw() {
    return Matrix4x4(*this);
  }

  /**
    Unary minus.
  */
  Matrix4x4 minus() const throw() {
    return Matrix4x4(*this).negate();
  }

  /**
    Negates the quaternion.
  */
  Matrix4x4 negate() throw() {
    transform(elements, 16, Negate<TYPE>());
    return *this;
  }

  /**
    Adds the specified vector to this vector.
  */
  Matrix4x4& add(const Matrix4x4& value) throw() {
    transformByBinary(elements, value.getElements(), 16, Add<TYPE>());
    return *this;
  }

  /**
    Subtracts the specified vector from this vector.
  */
  Matrix4x4& subtract(const Matrix4x4& value) throw() {
    transformByBinary(elements, value.getElements(), 16, Subtract<TYPE>());
    return *this;
  }

  /**
    Multiplies the matrix elements with the specified value.
  */
  Matrix4x4& multiply(const TYPE& value) throw() {
    transform(elements, 16, bind2Second(Multiply<TYPE>(), value));
    return *this;
  }

  /**
    Multiplies the matrix with the specified quaternion.
  */
  Quaternion<TYPE> multiply(const Quaternion<TYPE>& value) throw() {
    return Quaternion<TYPE>(
      elements[0][0] * value.getX() + elements[0][1] * value.getY() + elements[0][2] * value.getZ() + elements[0][3] * value.getW(),
      elements[1][0] * value.getX() + elements[1][1] * value.getY() + elements[1][2] * value.getZ() + elements[1][3] * value.getW(),
      elements[2][0] * value.getX() + elements[2][1] * value.getY() + elements[2][2] * value.getZ() + elements[2][3] * value.getW(),
      elements[3][0] * value.getX() + elements[3][1] * value.getY() + elements[3][2] * value.getZ() + elements[3][3] * value.getW()
    );
  }

  /**
    Multiplies the matrix elements with the specified matrix.
  */
  Matrix4x4 multiply(const Matrix4x4& value) throw() {
    Matrix4x4 result;
    TYPE* dest = Cast::pointer<TYPE*>(result.elements);
    for (int r = 3; r >= 0; --r) {
      for (int c = 3; c >= 0; --c) {
        *dest-- = elements[r][0] * value.elements[0][c] +
          elements[r][1] * value.elements[1][c] +
          elements[r][2] * value.elements[2][c] +
          elements[r][3] * value.elements[3][c];
      }
    }
    return result;
  }

  /**
    Returns an estimate for the deviation from the zero matrix. The estimate is
    calculated as the sum of all elements of the L1-norm.
  */
  TYPE deviationFromZero() const throw() {
    AbsoluteSum<TYPE> absoluteSum;
    forEach(getElements(), 16, absoluteSum);
    return absoluteSum.getResult();
  }

  /**
    Returns an estimate for the deviation from the identity matrix. The estimate
    is calulated as the sum of all elements of the L1-norm of the difference
    between this matrix and the identity matrix.
  */
  TYPE deviationFromIdentity() const throw() {
    TYPE result = 0;
    const TYPE* src = getElements();
    result += absolute(*src - 1);
    result += absolute(*++src);
    result += absolute(*++src);
    result += absolute(*++src);
    result += absolute(*++src);
    result += absolute(*++src - 1);
    result += absolute(*++src);
    result += absolute(*++src);
    result += absolute(*++src);
    result += absolute(*++src);
    result += absolute(*++src - 1);
    result += absolute(*++src);
    result += absolute(*++src);
    result += absolute(*++src);
    result += absolute(*++src);
    result += absolute(*++src - 1);
    return result;
  }

  /**
    Fills the last row with the vector [0 0 0 1] and leaves the other elements unchanged.
  */
  void forceHomogeneous() throw() {
    // TAG: any deviation from 0 0 0 1 should be feed back into the other elements
    elements[3][0] = TYPE(0);
    elements[3][1] = TYPE(0);
    elements[3][2] = TYPE(0);
    elements[3][3] = TYPE(1);
  }
  
  /**
    Divides this matrix elements with the specified value.
  */
  Matrix4x4& divide(const TYPE& value) throw() {
    transform(elements, 16, bind2Second(Divide<TYPE>(), value));
    return *this;
  }

  Matrix4x4& transpose() throw() {
    swap(elements[0][1], elements[1][0]);
    swap(elements[0][2], elements[2][0]);
    swap(elements[0][3], elements[3][0]);
    swap(elements[1][2], elements[2][1]);
    swap(elements[1][3], elements[3][1]);
    swap(elements[2][3], elements[3][2]);
  }
  
  Matrix4x4& transpose(const Matrix4x4& value) throw() {
    elements[0][0] = value.elements[0][0];
    elements[1][0] = value.elements[0][1];
    elements[2][0] = value.elements[0][2];
    elements[3][0] = value.elements[0][3];
    elements[0][1] = value.elements[1][0];
    elements[1][1] = value.elements[1][1];
    elements[2][1] = value.elements[1][2];
    elements[3][1] = value.elements[1][3];
    elements[0][2] = value.elements[2][0];
    elements[1][2] = value.elements[2][1];
    elements[2][2] = value.elements[2][2];
    elements[3][2] = value.elements[2][3];
    elements[0][3] = value.elements[3][0];
    elements[1][3] = value.elements[3][1];
    elements[2][3] = value.elements[3][2];
    elements[3][3] = value.elements[3][3];
    return *this;
  }
  
  /**
    Returns true if the vectors are equal.

    @param vector Vector to be compared.
  */
  inline bool operator==(const Matrix4x4& value) const throw() {
    return isEqual(value);
  }

  /**
    Adds the specified vector from this vector.

    @param value The value to be added.
  */
  inline Matrix4x4& operator+=(const Matrix4x4& value) throw() {
    return add(value);
  }

  /**
    Subtracts the specified vector from this vector.

    @param value The value to be subtracted.
  */
  inline Matrix4x4& operator-=(const Matrix4x4& value) throw() {
    return subtract(value);
  }

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  inline Matrix4x4& operator*=(const TYPE& value) throw() {
    return multiply(value);
  }

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  inline Matrix4x4& operator/=(const TYPE& value) throw() {
    return divide(value);
  }

  /**
    Unary plus.
  */
  inline Matrix4x4 operator+() const throw() {
    return plus();
  }

  /**
    Unary minus.
  */
  inline Matrix4x4 operator-() const throw() {
    return minus();
  }
};

/**
  Returns the sum of the vectors.
*/
template<class TYPE>
inline Matrix4x4<TYPE> operator+(const Matrix4x4<TYPE>& left, const Matrix4x4<TYPE>& right) throw() {
  return Matrix4x4<TYPE>(left).add(right);
}

/**
  Returns the difference of the vectors.
*/
template<class TYPE>
inline Matrix4x4<TYPE> operator-(const Matrix4x4<TYPE>& left, const Matrix4x4<TYPE>& right) throw() {
  return Matrix4x4<TYPE>(left).subtract(right);
}

/**
  Returns the product of the matrix and the value.
*/
template<class TYPE>
inline Matrix4x4<TYPE> operator*(const Matrix4x4<TYPE>& left, const TYPE& right) throw() {
  return Matrix4x4<TYPE>(left).multiply(right);
}

/**
  Returns the product of the matrix and the value.
*/
template<class TYPE>
inline Matrix4x4<TYPE> operator*(const TYPE& left, const Matrix4x4<TYPE>& right) throw() {
  return Matrix4x4<TYPE>(right).multiply(left);
}

/**
  Returns the product of the matrices.
*/
template<class TYPE>
inline Matrix4x4<TYPE> operator*(const Matrix4x4<TYPE>& left, const Matrix4x4<TYPE>& right) throw() {
  return Matrix4x4<TYPE>(left).multiply(right);
}

/**
  Returns the result of the vector divided by the value.
*/
template<class TYPE>
inline Matrix4x4<TYPE> operator/(const Matrix4x4<TYPE>& left, const TYPE& right) throw() {
  return Matrix4x4<TYPE>(left).divide(right);
}

/**
  Writes a string representation of a Matrix4x4 object to a format stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Matrix4x4<TYPE>& value) throw(IOException) {
  const TYPE* src = value.getElements()--;
  stream << '[';
  for (unsigned int i = 4; i > 0; --i) {
    stream << '[' << *++src << ' ' << *++src << ' ' << *++src << ' ' << *++src << ']';
  }
  stream << ']';
  return stream;
}

/**
  Returns a homogeneous rotation matrix corresponding to the specified angle of rotation (in radians) around the X-axis.
*/
template<class TYPE>
Matrix4x4<TYPE> getXRotation(TYPE angle) throw() {
  Matrix4x4<TYPE> result;
  TYPE* dest = result.getElements();
  const TYPE zero(0);
  const TYPE one(1);
  const TYPE ca = Math::cos(angle);
  const TYPE sa = Math::sin(angle);
  
  *dest++ = one;
  *dest++ = zero;
  *dest++ = zero;
  *dest++ = zero;
  
  *dest++ = zero;
  *dest++ = ca;
  *dest++ = -sa;
  *dest++ = zero;
  
  *dest++ = zero;
  *dest++ = sa;
  *dest++ = ca;
  *dest++ = zero;

  *dest++ = zero;
  *dest++ = zero;
  *dest++ = zero;
  *dest++ = one;  
  return result;
}

/**
  Returns a homogeneous rotation matrix corresponding to the specified angle of rotation (in radians) around the Y-axis.
*/
template<class TYPE>
Matrix4x4<TYPE> getYRotation(TYPE angle) throw() {
  Matrix4x4<TYPE> result;
  TYPE* dest = result.getElements();
  const TYPE zero(0);
  const TYPE one(1);
  const TYPE ca = Math::cos(angle);
  const TYPE sa = Math::sin(angle);
  
  *dest++ = ca;
  *dest++ = zero;
  *dest++ = -sa;
  *dest++ = zero;
  
  *dest++ = zero;
  *dest++ = one;
  *dest++ = zero;
  *dest++ = zero;
  
  *dest++ = sa;
  *dest++ = zero;
  *dest++ = ca;
  *dest++ = zero;

  *dest++ = zero;
  *dest++ = zero;
  *dest++ = zero;
  *dest++ = one;  
  return result;
}

/**
  Returns a homogeneous rotation matrix corresponding to the specified angle of rotation (in radians) around the Z-axis.
*/
template<class TYPE>
Matrix4x4<TYPE> getZRotation(TYPE angle) throw() {
  Matrix4x4<TYPE> result;
  TYPE* dest = result.getElements();
  const TYPE zero(0);
  const TYPE one(1);
  const TYPE ca = Math::cos(angle);
  const TYPE sa = Math::sin(angle);
  
  *dest++ = ca;
  *dest++ = -sa;
  *dest++ = zero;
  *dest++ = zero;
  
  *dest++ = sa;
  *dest++ = ca;
  *dest++ = zero;
  *dest++ = zero;
  
  *dest++ = zero;
  *dest++ = zero;
  *dest++ = one;
  *dest++ = zero;

  *dest++ = zero;
  *dest++ = zero;
  *dest++ = zero;
  *dest++ = one;  
  return result;
}

/**
  Returns a homogeneous rotation matrix corresponding to the specified Euler angles.
*/
template<class TYPE>
Matrix4x4<TYPE> getXYZRotation(const Vector3D<TYPE> rotation) throw() {
  const TYPE cx = Math::cos(rotation.getX());
  const TYPE sx = Math::sin(rotation.getX());
  const TYPE cy = Math::cos(rotation.getY());
  const TYPE sy = Math::sin(rotation.getY());
  const TYPE cz = Math::cos(rotation.getZ());
  const TYPE sz = Math::sin(rotation.getZ());
  const TYPE cxsy = cx * sy;
  const TYPE sxsy = sx * sy;

  Matrix4x4<TYPE> result;
  TYPE* dest = result.getElements();
  
  *dest++ = cy * cz;
  *dest++ = -cy * sz;
  *dest++ = -sy;
  *dest++ = TYPE(0);

  *dest++ = -sxsy * cz + cx * sz;
  *dest++ = sxsy * sz + cx * cz;
  *dest++ = -sx * cy;
  *dest++ = TYPE(0);

  *dest++ = cxsy * cz + sx * sz;
  *dest++ = -cxsy * sz + sx * cz;
  *dest++ = cx * cy;
  *dest++ = TYPE(0);
  
  *dest++ = TYPE(0);
  *dest++ = TYPE(0);
  *dest++ = TYPE(0);
  *dest++ = TYPE(1);
  
  return result;
}

/**
  Returns a homogeneous rotation matrix corresponding to the specified quaternion.
*/
template<class TYPE>
Matrix4x4<TYPE> getQRotation(const Quaternion<TYPE>& quaternion) throw() {
  const TYPE xx = quaternion.getX() * quaternion.getX();
  const TYPE xy = quaternion.getX() * quaternion.getY();
  const TYPE xz = quaternion.getX() * quaternion.getZ();
  const TYPE xw = quaternion.getX() * quaternion.getW();
  const TYPE yy = quaternion.getX() * quaternion.getX();
  const TYPE yz = quaternion.getY() * quaternion.getZ();
  const TYPE yw = quaternion.getY() * quaternion.getW();
  const TYPE zz = quaternion.getZ() * quaternion.getZ();
  const TYPE zw = quaternion.getZ() * quaternion.getW();
  
  Matrix4x4<TYPE> result;
  TYPE* dest = result.getElements();

  *dest++ = 1 - 2 * (yy + zz);
  *dest++ = 2 * (xy - zw);
  *dest++ = 2 * (xz + yw);
  *dest++ = TYPE(0);
  
  *dest++ = 2 * (xy + zw);
  *dest++ = 1 - 2 * (xx + zz);
  *dest++ = 2 * (yz - xw);
  *dest++ = TYPE(0);
  
  *dest++ = 2 * (xz - yw);
  *dest++ = 2 * (yz + xw);
  *dest++ = 1 - 2 * (xx + yy);
  *dest++ = TYPE(0);
  
  *dest++ = TYPE(0);
  *dest++ = TYPE(0);
  *dest++ = TYPE(0);
  *dest++ = TYPE(1);
  
  return result;
}

/**
  Returns the product of the matrix and quaternion.
*/
template<class TYPE>
inline Quaternion<TYPE> operator*(const Matrix4x4<TYPE>& left, const Quaternion<TYPE>& right) throw() {
  return Matrix4x4<TYPE>(left).multiply(right);
}

template<class TYPE>
class Uninitializeable<Matrix4x4<TYPE> > {
public:

  static const bool IS_UNINITIALIZEABLE = Uninitializeable<TYPE>::IS_UNINITIALIZEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
