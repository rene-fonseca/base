/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATH__MATRIX_H
#define _DK_SDU_MIP__BASE_MATH__MATRIX_H

#include "base/Object.h"
#include "base/Dimension.h"
#include "base/OutOfBounds.h"
#include "base/OutOfDomain.h"
#include "base/RandomIterator.h"
#include "base/mathematics/Vector.h"
#include "base/iteration/SimpleRandomIterator.h"
#include "base/iteration/EquidistantRandomIterator.h"
#include "base/mem/ReferenceCountedObjectPointer.h"
#include "base/string/FormatOutputStream.h"

/**
  This class encapsulates matrix manipulations.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE> class Matrix : public Object {
public:

  /** Iterators. */
  typedef SimpleRandomIterator<TYPE> RowIterator;
  typedef ReadOnlySimpleRandomIterator<TYPE> ReadOnlyRowIterator;
  typedef EquidistantRandomIterator<TYPE> ColumnIterator;
  typedef ReadOnlyEquidistantRandomIterator<TYPE> ReadOnlyColumnIterator;
  typedef EquidistantRandomIterator<TYPE> DiagonalIterator;
  typedef ReadOnlyEquidistantRandomIterator<TYPE> ReadOnlyDiagonalIterator;
  typedef SimpleRandomIterator<TYPE> RowByColumnIterator;
  typedef ReadOnlySimpleRandomIterator<TYPE> ReadOnlyRowByColumnIterator;
  typedef EquidistantRandomIterator<TYPE> ColumnByRowIterator;
  typedef ReadOnlyEquidistantRandomIterator<TYPE> ReadOnlyColumnByRowIterator;
protected:

  class MatrixBuffer : public ReferenceCountedObject {
  public:
/*    MatrixBuffer(unsigned int elementSize);
    MatrixBuffer(unsigned int elementSize, unsigned int rows, unsigned int columns);
    MatrixBuffer(const MatrixBuffer& copy);
    void resize(unsigned int rows, unsigned int columns);
    TYPE* getElements();*/
  };

  /** Reference counted buffer holding the elements of the matrix. */
  ReferenceCountedObjectPointer<MatrixBuffer> internal;

  /** The elements of the matrix. */
  TYPE* elements;
  /** The number of rows in the matrix. */
  unsigned int rows;
  /** The number of columns in the matrix. */
  unsigned int columns;
  /** The number of elements in the matrix. */
  unsigned int size;
  /** The dimension of the matrix. */
  Dimension dimension; // not needed

  /** Used to reallocate memory for elements if the matrix changes its dimension. */
  void adjustDimension(unsigned int rows, unsigned int columns);

  /** Throws OutOfBounds if element coordinate (row, column) is invalid. */
  inline void validateElement(unsigned int row, unsigned int column) const throw(OutOfBounds) {if ((row >= rows) || (column >= columns)) throw OutOfBounds();}

  /** Throws OutOfBounds if row is invalid. */
  inline void validateRow(unsigned int row) const throw(OutOfBounds) {if (row >= rows) throw OutOfBounds();}

  /** Throws OutOfBounds if column is invalid. */
  inline void validateColumn(unsigned int column) const throw(OutOfBounds) {if (column >= columns) throw OutOfBounds();}

  /** Converts 2D index to 1D index in the element array. Does NOT check boundaries. */
  inline unsigned int getIndexOfElement(unsigned int row, unsigned int column) const throw() {return row * columns + column;}

public:

  /** Exception thrown by the Matrix class. */
  class MatrixException : public Exception {
  };
  /** Thrown if an operation is given incompatible operands to work on. */
  class IncompatibleOperands : public MatrixException {
  };
  /** Thrown if an operation is given incompatible matrices to work on. */
  class IncompatibleMatrices : public IncompatibleOperands {
  };
  /** Thrown if matrix is not square as required by an operation. */
  class NotSquare : public MatrixException {
  };

  /**
    Initializes a matrix with no elements.
  */
  Matrix() throw();

  /**
    Initializes the matrix.

    @param dimension The desired dimension of the matrix.
  */
  Matrix(const Dimension& dimension) throw(OutOfDomain);

  /**
    Initialize object using the elements of an array.

    @param elements The desired elements of the matrix.
    @param dimension The desired dimension of the matrix.
  */
  Matrix(TYPE elements[], const Dimension& dimension) throw(OutOfDomain);

  /**
    Initialize object by copying a matrix.

    @param matrix The matrix to be copied.
  */
  Matrix(const Matrix& matrix) throw();

  /**
    Initialize matrix as an diagonal matrix.

    @param vector The desired diagonal vector of the matrix.
  */
  Matrix(const Vector<TYPE>& diagonal) throw();

  /**
    Returns an iterator of a single row of the matrix.

    @param row Specifies the row to be returned.
  */
  RowIterator<TYPE> getIteratorOfRow(unsigned int row) throw(OutOfBounds);

  /**
    Returns a read-only iterator of a single row of the matrix.

    @param row Specifies the row to be returned.
  */
  ReadOnlyRowIterator<TYPE> getIteratorOfRow(unsigned int row) const throw(OutOfBounds);

  /**
    Returns an iterator of a single column of the matrix.

    @param column Specifies the column to be returned.
  */
  ColumnIterator<TYPE> getIteratorOfColumn(unsigned int column) throw(OutOfBounds);

  /**
    Returns a read-only iterator of a single column of the matrix.

    @param column Specifies the column to be returned.
  */
  ColumnIterator<TYPE, ReadOnly> getIteratorOfColumn(unsigned int column) const throw(OutOfBounds);

  /**
    Returns an iterator of the diagonal elements of the matrix.
  */
  DiagonalIterator<TYPE> getIteratorOfDiagonal() throw();

  /**
    Returns a read-only iterator of the diagonal elements of the matrix.
  */
  ReadOnlyDiagonalIterator<TYPE> getIteratorOfDiagonal() const throw();

  /**
    Returns an iterator of all the elements of the matrix.
  */
  RowByColumnIterator<TYPE> getIteratorOfRowByColumn() throw();

  /**
    Returns a read-only iterator of all the elements of the matrix.
  */
  ReadOnlyRowByColumnIterator<TYPE> getIteratorOfRowByColumn() const throw();

//  RandomIterator<TYPE> getIteratorOfColumnByRow() throw();

//  ReadOnlyRandomIterator<TYPE> getIteratorOfColumnByRow() const throw();

  /**
    Returns the dimension of the matrix.
  */
  Dimension getDimension() const throw();

  /**
    Returns true if the matrix is a square matrix.

    @return True if the matrix is square.
  */
  inline bool isSquare() const throw() {return (rows == columns);}

  /**
    Returns the element at the specified row and column. Throws OutOfBounds if indices specifies element not within the matrix.

    @param row The row of the element.
    @param column The column of the element.
  */
  TYPE& at(unsigned int row, unsigned int column) throw(OutOfBounds);

  /**
    Returns true if the matrices are equal.
  */
  bool operator==(const Matrix& matrix) throw(IncompatibleMatrices);

  /**
    Assigns value to the elements of the matrix.

    @param value The desired value.
  */
  Matrix& operator=(const TYPE& value) throw();

  /**
    Assigns the matrix.

    @param matrix The desired matrix.
  */
  Matrix& operator=(const Matrix& matrix) throw();

  /**
    Unary plus.
    Problem? Return new object.
  */
  Matrix& operator+() throw();

  /**
    Add matrix.
  */
  Matrix& operator+=(const Matrix& matrix) throw(IncompatibleMatrices);

  /**
    Unary minus.
    Problem? Return new object.
  */
  Matrix& operator-() throw();

  /**
    Subtract matrix.
  */
  Matrix& operator-=(const Matrix& matrix) throw(IncompatibleMatrices);

  /**
    Multiply with value.
  */
  Matrix& operator*=(const TYPE& value) throw();

  /**
    Divide by value.
  */
  Matrix& operator/=(const TYPE& value) throw();

  /**
    Initializes the matrix as the identity matrix.
  */
  Matrix& identity() throw(NotSquare);

  /**
    Replaces the matrix with the sum of the specified matrices.

    @param a Matrix.
    @param b Matrix.
  */
  Matrix& add(const Matrix& a, const Matrix& b) throw(IncompatibleMatrices);

  /**
    Replaces the matrix with the difference of the specified matrices.

    @param a Matrix.
    @param b Matrix.
  */
  Matrix& subtract(const Matrix& a, const Matrix& b) throw(IncompatibleMatrices);

  /**
    Replaces the matrix with the product of the specified matrices.

    @param a Matrix.
    @param b Matrix.
  */
  Matrix& multiply(const Matrix<TYPE>& a, const Matrix<TYPE>& b) throw(IncompatibleMatrices);

  /**
    Replaces the matrix with the product of the specified matrices.

    @param a Matrix.
    @param b Vector treated as a (n x 1) matrix.
  */
  Matrix& multiply(const Matrix<TYPE>& a, const Vector<TYPE>& b) throw(MatrixException);

  /**
    Replaces the matrix with the product of the specified operands.

    @param a Vector treated as a (1 x n) matrix.
    @param b Matrix.
  */
  Matrix& multiply(const Vector<TYPE>& a, const Matrix<TYPE>& b) throw(MatrixException);

  /**
    Replaces the matrix (n x m) with the product of the specified vectors.

    @param a Vector treated as a (n x 1) matrix.
    @param b Vector treated as a (1 x m) matrix.
  */
  Matrix& multiply(const Vector<TYPE>& a, const Vector<TYPE>& b) throw(MatrixException);

  /**
    Returns the product.

    @param value Value.
  */
  Matrix& operator*(const TYPE& value) throw();

  /**
    Returns the product.

    @param matrix Matrix.
  */
  Matrix& operator*(const Matrix& matrix) throw();

  /**
    Replaces the matrix with the transpose of the specified matrix.

    @param matrix The matrix to be transposed.
  */
  Matrix& transpose(const Matrix& matrix) throw();

  /**
    Returns the transpose of the matrix.
  */
  Matrix& transpose() const throw();

  /**
    Destroys the matrix.
  */
  ~Matrix() throw();

  friend FormatOutputStream& operator<< <>(FormatOutputStream& stream, const Matrix& value);
};

/**
  Writes a string representation of a Matrix object to a format stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Matrix<TYPE>& value);

typedef Matrix<float> MatrixOfFloat;
typedef Matrix<double> MatrixOfDouble;

#endif
