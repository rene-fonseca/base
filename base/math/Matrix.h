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

#include <base/Object.h>
#include <base/Dimension.h>
#include <base/OutOfRange.h>
#include <base/OutOfDomain.h>
#include <base/math/IncompatibleOperands.h>
#include <base/math/Vector.h>
#include <base/mem/Reference.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Matrix implementation.

  @short Matrix
  @ingroup math
  @version 1.10
*/

template<class TYPE>
class Matrix : public Object {
public:

  class RowReference;

  /**
    Reference to an element within a matrix.

    @short Reference to an element within a matrix.
  */
  class ElementReference {
    friend class RowReference;
  private:
    
    Matrix& matrix; // use reference to avoid 'copy on write'
    unsigned int row = 0;
    unsigned int column = 0;

    inline ElementReference(
      Matrix& _matrix, unsigned int _row, unsigned int _column) noexcept
      : matrix(_matrix),
        row(_row),
        column(_column) {
    }
  public:

    inline ElementReference& operator=(bool value) {
      matrix.setAt(row, column, value); return *this;
    }
    
    inline operator TYPE() const {
      return matrix.getAt(row, column);
    }
  };

  /**
    Reference to a row within a matrix.

    @short Reference to a row within a matrix.
  */
  class RowReference {
    friend class Matrix;
  private:
    
    Matrix& matrix; // use reference to avoid 'copy on write'
    unsigned int row = 0;
    inline RowReference(Matrix& _matrix, unsigned int _row) noexcept
      : matrix(_matrix),
        row(_row) {
    }
  public:

    inline ElementReference operator[](unsigned int column) {
      return ElementReference(matrix, row, column);
    }
  };

  class Row {
  public:

    typedef typename ReferenceCountedAllocator<TYPE>::Iterator Iterator;
    typedef typename ReferenceCountedAllocator<TYPE>::ReadIterator ReadIterator;
    typedef typename ReferenceCountedAllocator<TYPE>::Enumerator Enumerator;
    typedef typename ReferenceCountedAllocator<TYPE>::ReadEnumerator ReadEnumerator;

//    /**
//      Enumeration of all the elements of a matrix.
//    */
//    class Enumeration : public AllocatorEnumeration<TYPE, TYPE&, TYPE*> {
//    public:
//
//      /**
//        Initializes an enumeration of all the elements of the specified matrix.
//
//        @param matrix The matrix being enumerated.
//      */
//      Enumeration(Matrix& matrix) noexcept
//        : AllocatorEnumeration<TYPE, TYPE&, TYPE*>(matrix.getElements(), matrix.getElements() + matrix.getSize()) {
//      }
//    };
//
//    /**
//      Non-modifying enumeration of all the elements of a matrix.
//    */
//    class ReadOnlyEnumeration : public AllocatorEnumeration<TYPE, const TYPE&, const TYPE*> {
//    public:
//
//      /**
//        Initializes a non-modifying enumeration of all the elements of the specified matrix.
//
//        @param matrix The matrix being enumerated.
//      */
//      ReadOnlyEnumeration(const Matrix& matrix) noexcept
//        : AllocatorEnumeration<TYPE, const TYPE&, const TYPE*>(matrix.getElements(), matrix.getElements() + matrix.getSize()) {
//      }
//    };
  }; // end of Row namespace

//  friend class Row::Enumerator;
//  friend class Row::ReadOnlyEnumerator;
protected:

  /**
    The elements of the matrix stored in an array. The array is guarantied to
    be non-empty when the matrix object has been initialized.
  */
  Reference<ReferenceCountedAllocator<TYPE> > elements;
  
  /** The number of rows in the matrix. */
  unsigned int rows = 0;
  /** The number of columns in the matrix. */
  unsigned int columns = 0;
  
  /**
    Returns the elements of the matrix for modification. May copy all elements
    of the matrix.
  */
  inline TYPE* getMutableElements() {
    if (elements.isMultiReferenced()) { // do we have the elements for our self
      elements = new ReferenceCountedAllocator<TYPE>(*elements);
    }
    return elements->getElements();
  }
  
  inline TYPE* getElements() {
    if (elements.isMultiReferenced()) { // do we have the elements for our self
      elements = new ReferenceCountedAllocator<TYPE>(*elements);
    }
    return elements->getElements();
  }
  
  /**
    Returns the elements of the matrix for read-only.
  */
  inline const TYPE* getReadOnlyElements() const noexcept {
    return elements->getElements();
  }

  inline const TYPE* getElements() const noexcept {
    return elements->getElements();
  }

  /**
    Returns true if this matrix and the specified matrix have identical
    dimensions.
  */
  inline bool isCompatible(const Matrix& value) const noexcept {
    return (rows == value.rows) && (columns == value.columns);
  }

  /**
    Sets the size of the matrix. Only invocated by constructors.
  */
  inline void setSize(
    unsigned int rows,
    unsigned int columns) {
    this->rows = rows;
    this->columns = columns;
    // do we have the elements for our self
    if ((!elements.isValid()) || (elements.isMultiReferenced())) {
      elements = new ReferenceCountedAllocator<TYPE>(rows * columns);
    } else {
      elements->setSize(rows * columns);
    }
  }

  /**
    Sets the dimension of this matrix to the dimension of the specified matrix.
  */
  inline void setDimension(const Matrix& value)
  {
    setSize(value.rows, value.columns);
  }

  /**
    Gets the number of elements in the matrix.
  */
  inline MemorySize getSize() const noexcept
  {
    return elements->getSize();
  }

  /**
    Raises OutOfRange if element coordinate (row, column) is invalid.
  */
  inline void validateElement(
    unsigned int row,
    unsigned int column) const
  {
    if ((row >= rows) || (column >= columns)) {
      _throw OutOfRange();
    }
  }

  /**
    Raises OutOfRange if row is invalid.
  */
  inline void validateRow(unsigned int row) const {
    if (row >= rows) {
      _throw OutOfRange();
    }
  }

  /**
    Raises OutOfRange if column is invalid.
  */
  inline void validateColumn(unsigned int column) const {
    if (column >= columns) {
      _throw OutOfRange();
    }
  }
  
  /**
    Converts 2D index to 1D index in the element array. Does NOT check
    boundaries.
  */
  inline unsigned int getIndexOfElement(
    unsigned int row,
    unsigned int column) const noexcept {
    return row * columns + column;
  }
public:
  
  /** @short Exception raised by the Matrix class. */
  class MatrixException : public Exception {
  public:
    
    _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
  };
  
  /**
    @short Raised if an operation is given incompatible matrices to work on.
  */
  class IncompatibleMatrices : public IncompatibleOperands {
  public:
    
    _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
  };
  
  /** @short Raised by some operations if matrix is non-square. */
  class NotSquare : public MatrixException {
  public:
    
    _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
  };
  
  /** @short Raised by some operations if matrix is singular. */
  class Singular : public MatrixException {
  public:
    
    _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
  };

  /**
    Initializes a matrix with no elements.
  */
  inline Matrix() noexcept {
    setSize(0, 0);
  }
  
  /**
    Initializes matrix with the specified dimension.

    @param dimension The desired dimension of the matrix.
  */
  explicit Matrix(const Dimension& dimension);
  
  /**
    Initializes matrix using the elements of an array. The elements are
    expected to be ordered from left to right and top to bottom.

    @param elements The desired elements of the matrix.
    @param dimension The desired dimension of the matrix.
  */
  Matrix(const TYPE elements[], const Dimension& dimension);
  
  /**
    Initializes matrix as a diagonal matrix with the diagonal elements
    provided by the specified enumerator. Raises OutOfDomain if enumeration
    is empty.

    @param diagonal The enumerator containing the desired diagonal elements.
    @param dimension The desired dimension of the matrix.
  */
  Matrix(
    Enumerator<ReadEnumeratorTraits<TYPE> >& diagonal,
    const Dimension& dimension);
  
  /**
    Initializes matrix from other matrix.

    @param matrix The matrix to be copied.
  */
  inline Matrix(const Matrix& copy) noexcept
    : elements(copy.elements), rows(copy.rows), columns(copy.columns) {
  }

  /**
    Assigns matrix to this matrix.

    @param eq The matrix containing the desired elements.
  */
  Matrix& operator=(const Matrix& assign) {
    this->elements = assign.elements;
    this->rows = assign.rows;
    this->columns = assign.columns;
    return *this;
  }



  // Dimension section



  /**
    Returns the dimension of the matrix.
  */
  inline Dimension getDimension() const noexcept {
    return Dimension(rows, columns);
  }

  /**
    Returns the number of rows of this matrix.
  */
  inline unsigned int getRows() const noexcept {
    return rows;
  }

  /**
    Returns the number of columns of this matrix.
  */
  inline unsigned int getColumns() const noexcept {
    return columns;
  }



  // Matrix type



  /**
    Returns true if the matrix is empty (i.e. has no elements).
  */
  inline bool isEmpty() const noexcept {
    return (rows == 0) || (columns == 0);
  }
  
  /**
    Returns true if the matrix is a square matrix.
  */
  inline bool isSquare() const noexcept {
    return (rows == columns);
  }

  /**
    Returns true if the matrices are equal.
  */
  bool isEqual(const Matrix& value) const noexcept;

  /**
    Returns true if the matrix is the zero matrix.

    @return false if matrix contains no elements.
  */
  bool isZero() const noexcept;

  /**
    Returns true if this matrix is a diagonal matrix.

    @return false if matrix is not square or contains no elements.
  */
  bool isDiagonal() const noexcept;

  /**
    Returns true if this matrix is lower triangular (i.e. the matrix has no
    non-zero values above the diagonal line).

    @return false if matrix is not square or contains no elements.
  */
  bool isLowerTriangular() const noexcept;

  /**
    Returns true if this matrix is upper triangular (i.e. the matrix has no
    non-zero values below the diagonal line).

    @return false if matrix is not square or contains no elements.
  */
  bool isUpperTriangular() const noexcept;

  /**
    Returns true if this matrix is symmetric.

    @return false if matrix is not square or contains no elements.
  */
  bool isSymmetric() const noexcept;
  
  
  
  /**
    Returns the Hilbert-Schmidt norm of the matrix.
  */
  TYPE getNorm() const noexcept;

  /**
    Returns the determinant of this matrix.
  */
  TYPE getDeterminant() const;
  
  /**
    Returns the minor of this matrix.

    @param row The row to be discarded.
    @param column The column to be discarded.
  */
  Matrix getMinor(
    unsigned int row,
    unsigned int column) const;
  
  /**
    Returns the cofactor of this matrix for the specified element.

    @param row The row of the element.
    @param column The column of the element.
  */
  TYPE getCofactor(
    unsigned int row,
    unsigned int column) const;
  
  /**
    Returns the adjoint of this matrix.
  */
  Matrix getAdjoint() const;

  /**
    Returns the solution(s) for the linear system(s).

    @param value The constant column vector(s).
  */
  Matrix solve(
    const Matrix<TYPE>& value) const;
  


  // Elements access section



  /**
    Returns an enumeration of all the elements in the specified row.

    @param row The desired row to enumerate.
  */
//  ArrayEnumeration<TYPE> getRowEnumeration(unsigned int row);

  /**
    Returns a read-only enumeration of all the elements in the specified row.

    @param row The desired row to enumerate.
  */
//  ArrayEnumeration<const TYPE> getRowEnumeration(unsigned int row) const;

  /**
    Returns an enumeration of all the elements in the specified column.

    @param column The desired column to enumerate.
  */
//  ArrayEnumeration<TYPE> getColumnEnumeration(unsigned int column);

  /**
    Returns a read-only enumeration of all the elements in the specified column.

    @param column The desired column to enumerate.
  */
//  ArrayEnumeration<const TYPE> getColumnEnumeration(unsigned int column) const;

  /**
    Returns an enumeration of all the elements of the diagonal.
  */
//  ArrayEnumeration<TYPE> getDiagonalEnumeration();

  /**
    Returns a read-only enumeration of all the elements of the diagonal.
  */
//  ArrayEnumeration<const TYPE> getDiagonalEnumeration() const;

  /**
    Returns the element at the specified position.

    @param row The row of the element.
    @param column The column of the element.
  */
  const TYPE& getAt(unsigned int row, unsigned int colum) const;
  
  /**
    Sets the element at the specified position.

    @param row The row of the element.
    @param column The column of the element.
    @param value The desired value.
  */
  void setAt(
    unsigned int row,
    unsigned int column, const TYPE& value);
  
  /**
    Returns the element at the specified position.
  */
  inline const TYPE& operator()(
    unsigned int row,
    unsigned int column) const {
    return getAt(row, column);
  }
  
  /**
    Returns a reference to the specified row.
  */
  inline RowReference operator[](unsigned int row) {
    return RowReference(*this, row);
  }



  // Modification section



  /**
    Unary plus.
  */
  Matrix plus() const noexcept;

  /**
    Unary minus.
  */
  Matrix minus() const noexcept;

  /**
    Unary inversion.
  */
  Matrix invert() const;

  /**
    Returns a new matrix that is the transpose of this matrix.
  */
  Matrix transpose() const noexcept;



  /**
    Sets this matrix to the zero matrix.
  */
  Matrix& clear();

  /**
    Sets this matrix to the identity matrix.
  */
  Matrix& identity();

  /**
    Pivot transformation of this matrix.
  */
//  Matrix& pivot(unsigned int row, unsigned int column);

  /**
    Solves the equations.
  */
//  Matrix solve(const Matrix& value) const;
  
  /**
    Sets the values of this matrix that exceed the specified value to zero (0).

    @param zero Specifies the zero limit.
  */
  Matrix& zeroAdjust(const TYPE& zero);
  
  /**
    Does a row echelon transformation of this matrix.
  */
  Matrix& rowEchelon();
  
  /**
    Does a reduced row echelon transformation of this matrix for the specified
    element. The element value is transformed to 1 and the other values of the
    column are transformed to 0.

    @param row The row of the element.
    @param column The column of the element.
  */
  Matrix& pivot(
    unsigned int row,
    unsigned int column);

  /**
    Throws away any elements that exceed the specified dimension.

    @param rows The desired number of row.
    @param columns The desired number of columns.
  */
  Matrix& clip(
    unsigned int rows,
    unsigned int columns);

  /**
    Negates this matrix.
  */
  Matrix& negate();
  
  /**
    Adds the specified matrix to this matrix.

    @param value The matrix to be added.
  */
  Matrix& add(
    const Matrix& value);
  
  /**
    Subtracts the specified matrix from this matrix.

    @param value The matrix to be subtracted.
  */
  Matrix& subtract(
    const Matrix& value);

  /**
    Multiplies this matrix with the specified value.

    @param value The multiplicator.
  */
  Matrix& multiply(const TYPE& value);
  
  /**
    Divides this matrix with the specified value.

    @param value The divisor.
  */
  Matrix& divide(const TYPE& value);

  /**
    Negates the specified matrix and stores the result in this matrix.
  */
  Matrix& negate(const Matrix& value);
  
  /**
    Sets this matrix to the sum of the specified matrices.

    @param left Matrix.
    @param right Matrix.
  */
  Matrix& add(
    const Matrix& left,
    const Matrix& right);
  
  /**
    Sets this matrix to the difference of the specified matrices.

    @param left Matrix.
    @param right Matrix.
  */
  Matrix& subtract(
    const Matrix& left,
    const Matrix& right);
  
  /**
    Sets this matrix to the product of the specified matrices.

    @param left Matrix.
    @param right Matrix.
  */
  Matrix& multiply(
    const Matrix<TYPE>& left,
    const Matrix<TYPE>& right);
  
  /**
    Sets this matrix to the product of the operands.

    @param left The left operand.
    @param right The right operand.
  */
  Matrix& multiply(
    const Matrix& left, const TYPE& right);
  
  /**
    Sets this matrix to the product of the operands.

    @param left The left operand.
    @param right The right operand.
  */
  Matrix& multiply(
    const TYPE& left, const Matrix& right);
  
  /**
    Sets this matrix to the result of the division of the left by right
    operand.
    
    @param left The left operand.
    @param right The right operand.
  */
  Matrix& divide(const Matrix& left, const TYPE& right);
  
  /**
    Sets this matrix to the transpose of the specified matrix.

    @param matrix The matrix to be transposed.
  */
  Matrix& transpose(const Matrix& matrix);



  // Operator section



  /**
    Returns true if the matrices are equal element by element. This member
    should only be used for integer types.

    @param value Matrix to be compared.
  */
  inline bool operator==(const Matrix& value) const noexcept {
    return isEqual(value);
  }

  /**
    Adds the specified matrix from this matrix.

    @param value The value to be added.
  */
  inline Matrix& operator+=(
    const Matrix& value) {
    return add(value);
  }

  /**
    Subtracts the specified matrix from this matrix.

    @param value The value to be subtracted.
  */
  inline Matrix& operator-=(
    const Matrix& value) {
    return subtract(value);
  }

  /**
    Multiplies this matrix with the specified value.

    @param value The multiplicator.
  */
  inline Matrix& operator*=(const TYPE& value) {
    return multiply(value);
  }

  /**
    Divides this matrix with the specified value.

    @param value The divisor.
  */
  inline Matrix& operator/=(const TYPE& value) {
    return divide(value);
  }

  /**
    Unary plus.
  */
  inline Matrix operator+() const {
    return plus();
  }

  /**
    Unary minus.
  */
  inline Matrix operator-() const {
    return minus();
  }



  // Friend section


#if 0
  /**
    Returns the product of the matrices.
  */
  friend Matrix operator* <>(
    const Matrix& left, const Matrix& right);

  /**
    Returns the product of the matrix and the value.
  */
  friend Matrix operator* <>(
    const Matrix& left, const TYPE& right);

  /**
    Returns the product of the value and matrix.
  */
  friend Matrix operator* <>(
    const TYPE& left, const Matrix& right);

  /**
    Returns the result of matrix divided by matrix.
  */
  friend Matrix operator/ <>(
    const Matrix& left, const Matrix& right);

  /**
    Returns the result of matrix divided by value.
  */
  friend Matrix operator/ <>(
    const Matrix& left, const TYPE& right);

  /**
    Returns the result of value divided by matrix.
  */
  friend Matrix operator/ <>(
    const TYPE& left, const Matrix& right);

  /**
    Writes a string representation of a matrix object to a format stream.
  */
  friend FormatOutputStream& operator<< <>(
    FormatOutputStream& stream, const Matrix& value);
#endif
};

/**
  @relates Matrix
*/
template<class TYPE>
bool operator==(const Matrix<TYPE>& left, const Matrix<TYPE>& right) noexcept;

/**
  @relates Matrix
*/
template<class TYPE>
Matrix<TYPE> operator+(
  const Matrix<TYPE>& left, const Matrix<TYPE>& right);

/**
  @relates Matrix
*/
template<class TYPE>
Matrix<TYPE> operator-(
  const Matrix<TYPE>& left, const Matrix<TYPE>& right);

/**
  @relates Matrix
*/
template<class TYPE>
Matrix<TYPE> operator*(
  const Matrix<TYPE>& left, const Matrix<TYPE>& right);

/**
  @relates Matrix
*/
template<class TYPE>
Matrix<TYPE> operator*(
  const Matrix<TYPE>& left, const TYPE& right);

/**
  @relates Matrix
*/
template<class TYPE>
Matrix<TYPE> operator*(
  const TYPE& left, const Matrix<TYPE>& right);

/**
  @relates Matrix
*/
template<class TYPE>
Matrix<TYPE> operator/(
  const Matrix<TYPE>& left, const Matrix<TYPE>& right);

/**
  @relates Matrix
*/
template<class TYPE>
Matrix<TYPE> operator/(
  const Matrix<TYPE>& left, const TYPE& right);

/**
  @relates Matrix
*/
template<class TYPE>
Matrix<TYPE> operator/(
  const TYPE& left, const Matrix<TYPE>& right);

/**
  Writes a string representation of a matrix object to a format stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Matrix<TYPE>& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
