/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATH__MATRIX_H
#define _DK_SDU_MIP__BASE_MATH__MATRIX_H

#include "base/Object.h"
#include "base/Dimension.h"
#include "base/OutOfRange.h"
#include "base/OutOfDomain.h"
#include "IncompatibleOperands.h"
#include "base/mathematics/Vector.h"
#include "base/mem/ReferenceCountedObjectPointer.h"
#include "base/mem/Array.h"
#include "base/string/FormatOutputStream.h"


/**
  Matrix implementation.

  @author René Møller Fonseca
  @version 1.10
*/

template<class TYPE> class Matrix : public Object {
protected:

  /**
    The elements of the matrix stored in an array. Guarantied to be non-empty
    when matrix object has been initialized.
  */
  ReferenceCountedObjectPointer<Array<TYPE> > elements;

  /** The number of rows in the matrix. */
  unsigned int rows;
  /** The number of columns in the matrix. */
  unsigned int columns;

  /**
    Returns the elements of the matrix for modification.
  */
  inline TYPE* getElements() throw(MemoryException) {
    if (elements.isMultiReferenced()) { // do we have the elements for our self
      elements = new Array<TYPE>(*elements); // make copy of the elements
    }
    return elements->getElements();
  }

  /**
    Returns the elements of the matrix for read-only.
  */
  inline const TYPE* getReadOnlyElements() const throw() {
    return elements->getElements();
  }

  /**
    Returns true if this matrix and the specified matrix have identical dimensions.
  */
  inline bool areCompatible(const Matrix& value) const throw() {
    return (rows == value.rows) && (columns == value.columns);
  }

  /**
    Sets the size of the matrix. Only invocated by constructors.
  */
  inline void setSize(unsigned int rows, unsigned int columns) throw(MemoryException) {
    this->rows = rows;
    this->columns = columns;
    if ((elements.isNULL()) || (elements.isMultiReferenced())) { // do we have the elements for our self
      elements = new Array<TYPE>(rows * columns);
    } else {
      elements->setSize(rows * columns);
    }
  }

  /**
    Sets the dimension of this matrix to the dimension of the specified matrix.
  */
  inline void setDimension(const Matrix& value) throw(MemoryException) {
    setSize(value.rows, value.columns);
  }

  /**
    Gets the number of elements in the matrix.
  */
  inline unsigned int getSize() const throw() {
    return elements->getSize();
  }

  /** Throws OutOfRange if element coordinate (row, column) is invalid. */
  inline void validateElement(unsigned int row, unsigned int column) const throw(OutOfRange) {
    if ((row >= rows) || (column >= columns)) {
      throw OutOfRange();
    }
  }

  /** Throws OutOfRange if row is invalid. */
  inline void validateRow(unsigned int row) const throw(OutOfRange) {
    if (row >= rows) {
      throw OutOfRange();
    }
  }

  /** Throws OutOfRange if column is invalid. */
  inline void validateColumn(unsigned int column) const throw(OutOfRange) {
    if (column >= columns) {
      throw OutOfRange();
    }
  }

  /** Converts 2D index to 1D index in the element array. Does NOT check boundaries. */
  inline unsigned int getIndexOfElement(unsigned int row, unsigned int column) const throw() {
    return row * columns + column;
  }

public:

  /** Exception thrown by the Matrix class. */
  class MatrixException : public Exception {
  };
  /** Thrown if an operation is given incompatible matrices to work on. */
  class IncompatibleMatrices : public IncompatibleOperands {
  };

  /**
    Initializes a matrix with no elements.
  */
  inline Matrix() throw() : elements(NULL), rows(0), columns(0) {}

  /**
    Initializes matrix with the specified dimension.

    @param dimension The desired dimension of the matrix.
  */
  explicit Matrix(const Dimension& dimension) throw(OutOfDomain);

  /**
    Initializes matrix using the elements of an array. The elements are
    expected to be ordered from left to right and top to bottom.

    @param elements The desired elements of the matrix.
    @param dimension The desired dimension of the matrix.
  */
  Matrix(const TYPE elements[], const Dimension& dimension) throw(OutOfDomain);

  /**
    Initializes matrix as a diagonal matrix with the diagonal elements
    provided by the specified enumerator. Throws 'OutOfDomain' if enumeration
    is empty.

    @param diagonal The enumerator containing the desired diagonal elements.
    @param dimension The desired dimension of the matrix.
  */
  Matrix(Enumeration<TYPE>& diagonal, const Dimension& dimension) throw(OutOfDomain);

  /**
    Initializes matrix from other matrix.

    @param matrix The matrix to be copied.
  */
  inline Matrix(const Matrix& copy) throw() : elements(copy.elements), rows(copy.rows), columns(copy.columns) {}

  /**
    Assigns matrix to this matrix.

    @param eq The matrix containing the desired elements.
  */
  Matrix& operator=(const Matrix& eq) throw(MemoryException);



  // Dimension section



  /**
    Returns the dimension of the matrix.
  */
  inline Dimension getDimension() const throw() {return Dimension(rows, columns);}

  /**
    Returns the number of rows of this matrix.
  */
  inline unsigned int getRows() const throw() {return rows;}

  /**
    Returns the number of columns of this matrix.
  */
  inline unsigned int getColumns() const throw() {return columns;}

  /**
    Returns true if the matrix is a square matrix.

    @return True if the matrix is square.
  */
  inline bool isSquare() const throw() {return (rows == columns);}



  // Elements access section



  /**
    Returns an enumeration of all the elements in the specified row.

    @param row The desired row to enumerate.
  */
  ArrayEnumeration<TYPE> getRowEnumeration(unsigned int row) throw(OutOfRange);

  /**
    Returns a read-only enumeration of all the elements in the specified row.

    @param row The desired row to enumerate.
  */
  ArrayEnumeration<const TYPE> getRowEnumeration(unsigned int row) const throw(OutOfRange);

  /**
    Returns an enumeration of all the elements in the specified column.

    @param column The desired column to enumerate.
  */
  ArrayEnumeration<TYPE> getColumnEnumeration(unsigned int column) throw(OutOfRange);

  /**
    Returns a read-only enumeration of all the elements in the specified column.

    @param column The desired column to enumerate.
  */
  ArrayEnumeration<const TYPE> getColumnEnumeration(unsigned int column) const throw(OutOfRange);

  /**
    Returns an enumeration of all the elements of the diagonal.
  */
  ArrayEnumeration<TYPE> getDiagonalEnumeration() throw(OutOfRange);

  /**
    Returns a read-only enumeration of all the elements of the diagonal.
  */
  ArrayEnumeration<const TYPE> getDiagonalEnumeration() const throw(OutOfRange);

  /**
    Returns the element at the specified position.

    @param row The row of the element.
    @param column The column of the element.
  */
  TYPE getAt(unsigned int row, unsigned int colum) const throw(OutOfRange);

  /**
    Sets the element at the specified position.

    @param row The row of the element.
    @param column The column of the element.
    @param value The desired value.
  */
  void setAt(unsigned int row, unsigned int column, const TYPE& value) throw(OutOfRange);



  // Modification section



  /**
    Unary plus.
  */
  Matrix plus() const throw();

  /**
    Unary minus.
  */
  Matrix minus() const throw();

  /**
    Returns a new matrix that is the transpose of this matrix.
  */
  Matrix transpose() const throw();

  /**
    Sets this matrix to the zero matrix.
  */
  Matrix& clear() throw();

  /**
    Sets this matrix to the identity matrix.
  */
  Matrix& identity() throw();

  /**
    Negates this matrix.
  */
  Matrix& negate() throw();

  /**
    Adds the specified matrix to this matrix.

    @param value The matrix to be added.
  */
  Matrix& add(const Matrix& value) throw(IncompatibleOperands);

  /**
    Subtracts the specified matrix from this matrix.

    @param value The matrix to be subtracted.
  */
  Matrix& subtract(const Matrix& value) throw(IncompatibleOperands);

  /**
    Multiplies this matrix with the specified value.

    @param value The multiplicator.
  */
  Matrix& multiply(const TYPE& value) throw();

  /**
    Divides this matrix with the specified value.

    @param value The divisor.
  */
  Matrix& divide(const TYPE& value) throw();

  /**
    Negates the specified matrix and stores the result in this matrix.
  */
  Matrix& negate(const Matrix& value) throw();

  /**
    Sets this matrix to the sum of the specified matrices.

    @param left Matrix.
    @param right Matrix.
  */
  Matrix& add(const Matrix& left, const Matrix& right) throw(IncompatibleOperands);

  /**
    Sets this matrix to the difference of the specified matrices.

    @param left Matrix.
    @param right Matrix.
  */
  Matrix& subtract(const Matrix& left, const Matrix& right) throw(IncompatibleOperands);

  /**
    Sets this matrix to the product of the specified matrices.

    @param left Matrix.
    @param right Matrix.
  */
  Matrix& multiply(const Matrix<TYPE>& left, const Matrix<TYPE>& right) throw(IncompatibleOperands);

  /**
    Sets this matrix to the product of the operands.

    @param left The left operand.
    @param right The right operand.
  */
  Matrix& multiply(const Matrix& left, const TYPE& right) throw();

  /**
    Sets this matrix to the product of the operands.

    @param left The left operand.
    @param right The right operand.
  */
  Matrix& multiply(const TYPE& left, const Matrix& right) throw();

  /**
    Sets this matrix to the result of the division of the left by right operand.

    @param left The left operand.
    @param right The right operand.
  */
  Matrix& divide(const Matrix& left, const TYPE& right) throw();

  /**
    Sets this matrix to the transpose of the specified matrix.

    @param matrix The matrix to be transposed.
  */
  Matrix& transpose(const Matrix& matrix) throw();



  // Operator section



  /**
    Returns true if the matrices are equal element by element. This member
    should only be used for integer types.

    @param value Matrix to be compared.
  */
  bool operator==(const Matrix& value) const throw(IncompatibleOperands);

  /**
    Adds the specified matrix from this matrix.

    @param value The value to be added.
  */
  inline Matrix& operator+=(const Matrix& value) throw(IncompatibleOperands) {return add(value);}

  /**
    Subtracts the specified matrix from this matrix.

    @param value The value to be subtracted.
  */
  inline Matrix& operator-=(const Matrix& value) throw(IncompatibleOperands) {return subtract(value);}

  /**
    Multiplies this matrix with the specified value.

    @param value The multiplicator.
  */
  inline Matrix& operator*=(const TYPE& value) throw() {return multiply(value);}

  /**
    Divides this matrix with the specified value.

    @param value The divisor.
  */
  inline Matrix& operator/=(const TYPE& value) throw() {return divide(value);}

  /**
    Unary plus.
  */
  inline Matrix operator+() const throw() {return plus();}

  /**
    Unary minus.
  */
  inline Matrix operator-() const throw() {return minus();}



  // Friend section



  /**
    Returns the product of the matrices.
  */
  friend Matrix operator* <>(const Matrix<TYPE>& left, const Matrix<TYPE>& right) throw(MemoryException);

  /**
    Returns the product of the matrix and the value.
  */
  friend Matrix operator* <>(const Matrix& left, const TYPE& right) throw(MemoryException);

  /**
    Returns the product of the matrix and the value.
  */
  friend Matrix operator* <>(const TYPE& left, const Matrix& right) throw(MemoryException);

  /**
    Returns the result of the matrix divided by the value.
  */
  friend Matrix operator/ <>(const Matrix& left, const TYPE& right) throw(MemoryException);

  /**
    Writes a string representation of a matrix object to a format stream.
  */
  friend FormatOutputStream& operator<< <>(FormatOutputStream& stream, const Matrix& value);
};

/**
  Returns the product of the matrices.
*/
template<class TYPE>
Matrix<TYPE> operator*(const Matrix<TYPE>& left, const Matrix<TYPE>& right) throw(MemoryException);

/**
  Returns the product of the matrix and the value.
*/
template<class TYPE>
Matrix<TYPE> operator*(const Matrix<TYPE>& left, const TYPE& right) throw(MemoryException);

/**
  Returns the product of the matrix and the value.
*/
template<class TYPE>
Matrix<TYPE> operator*(const TYPE& left, const Matrix<TYPE>& right) throw(MemoryException);

/**
  Returns the result of the matrix divided by the value.
*/
template<class TYPE>
Matrix<TYPE> operator/(const Matrix<TYPE>& left, const TYPE& right) throw(MemoryException);

/**
  Writes a string representation of a matrix object to a format stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Matrix<TYPE>& value);

#endif
