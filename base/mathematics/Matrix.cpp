/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Matrix.h"

template Matrix<float>;
template Matrix<double>;

template<class TYPE> void Matrix<TYPE>::adjustDimension(unsigned int rows, unsigned int columns) {
  unsigned int newSize = rows * columns;

  if (size != newSize) {
    delete[] elements;
    elements = new TYPE[newSize];
  }

  this->rows = rows;
  this->columns = columns;
  size = newSize;
  dimension.assign(rows, columns);
}

template<class TYPE> Matrix<TYPE>::Matrix() throw() {
  dimension.assign(0, 0);
  rows = dimension.getWidth();
  columns = dimension.getHeight();
  size = dimension.getSize();

  elements = new TYPE[size];
}

template<class TYPE> Matrix<TYPE>::Matrix(const Dimension& dimension) throw(OutOfDomain) {
  if (dimension.getSize() == 0) {
    throw OutOfDomain();
  }

  this->dimension = dimension;
  rows = dimension.getWidth();
  columns = dimension.getHeight();
  size = dimension.getSize();

  elements = new TYPE[size];
}

template<class TYPE> Matrix<TYPE>::Matrix(TYPE elements[], const Dimension& dimension) throw(OutOfDomain) {
  if (dimension.getSize() == 0) {
    throw OutOfDomain();
  }

  this->dimension = dimension;
  rows = dimension.getWidth();
  columns = dimension.getHeight();
  size = dimension.getSize();

  elements = new TYPE[size];

  /* Copy elements. */
  for (unsigned int i = 0; i < size; i++) {
    this->elements[i] = elements[i];
  }
}

template<class TYPE> Matrix<TYPE>::Matrix(const Matrix& matrix) throw() {
  this->dimension = matrix.dimension;
  rows = dimension.getWidth();
  columns = dimension.getHeight();
  size = dimension.getSize();

  elements = new TYPE[size];

  /* Copy elements. */
  for (unsigned int i = 0; i < size; i++) {
    this->elements[i] = matrix.elements[i];
  }
}

template<class TYPE> Matrix<TYPE>::Matrix(const Vector<TYPE>& diagonal) throw() {
  this->dimension = Dimension(diagonal.getLength(), diagonal.getLength());
  rows = dimension.getWidth();
  columns = dimension.getHeight();
  size = dimension.getSize();

  elements = new TYPE[size];

  SimpleRandomIterator<TYPE> dest; // = getIteratorOfRowByColumn();
  while (dest.hasNext()) {
    *dest = 0;
    ++dest;
  }

//  RandomIterator<TYPE>* src = diagonal.getIterator();
//  ReadOnlyRandomIterator<TYPE>* src = diagonal.getReadOnlyIterator();
  float* src = NULL;
  dest = getIteratorOfDiagonal();
  while (dest.hasNext()) {
    *dest = *src;
    ++src;
    ++dest;
  }
}

template<class TYPE> RandomIterator<TYPE> Matrix<TYPE>::getIteratorOfRow(unsigned int row) throw(OutOfBounds) {
  validateRow();
  return SimpleRandomIterator<TYPE>(elements[getIndexOfElement(row, 0)], columns);
}

template<class TYPE> ReadOnlyRandomIterator<TYPE> Matrix<TYPE>::getIteratorOfRow(unsigned int row) const throw(OutOfBounds) {
  validateRow();
  return SimpleRandomIterator<TYPE, ReadOnlyIteratorTraits<TYPE> >(elements[getIndexOfElement(row, 0)], columns);
}

template<class TYPE> RandomIterator<TYPE> Matrix<TYPE>::getIteratorOfColumn(unsigned int column) throw(OutOfBounds) {
  validateColumn();
  return EquidistantRandomIterator<TYPE>(elements[getIndexOfElement(0, column)], rows, columns);
}

template<class TYPE> ReadOnlyRandomIterator<TYPE> Matrix<TYPE>::getIteratorOfColumn(unsigned int column) const throw(OutOfBounds) {
  validateColumn();
  return EquidistantRandomIterator<TYPE, ReadOnlyIteratorTraits<TYPE> >(elements[getIndexOfElement(0, column)], rows, columns);
}

template<class TYPE> RandomIterator<TYPE> Matrix<TYPE>::getIteratorOfDiagonal() throw() {
  return EquidistantRandomIterator<TYPE>(elements[getIndexOfElement(0, 0)], minimum(rows, columns), columns + 1);
}

template<class TYPE> ReadOnlyRandomIterator<TYPE> Matrix<TYPE>::getIteratorOfDiagonal() const throw() {
  return EquidistantRandomIterator<TYPE, ReadOnlyIteratorTraits<TYPE> >(elements[getIndexOfElement(0, 0)], minimum(rows, columns), columns + 1);
}

template<class TYPE> RandomIterator<TYPE> Matrix<TYPE>::getIteratorOfRowByColumn() throw() {
  return SimpleRandomIterator<TYPE>(elements[getIndexOfElement(0, 0)], size);
}

template<class TYPE> ReadOnlyRandomIterator<TYPE> Matrix<TYPE>::getIteratorOfRowByColumn() const throw() {
  return SimpleRandomIterator<TYPE, ReadOnlyIteratorTraits<TYPE> >(elements[getIndexOfElement(0, 0)], size);
}

template<class TYPE> Dimension Matrix<TYPE>::getDimension() const throw() {
  return Dimension(dimension);
}

template<class TYPE> TYPE& Matrix<TYPE>::at(unsigned int row, unsigned int column) throw(OutOfBounds) {
  validateElement();
  return elements[row * columns + column];
}

template<class TYPE> bool Matrix<TYPE>::operator==(const Matrix& matrix) throw(IncompatibleMatrices) {
  if (dimension != matrix.dimension) {
    throw IncompatibleMatrices();
  }

  TYPE* a = elements;
  TYPE* b = matrix.elements;
  unsigned int count = size;

  while (count--) {
    if (*(a++) != *(b++)) { /* Check if elements are different - problem if real numbers */
      return false;
    }
  }
  return true;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::operator=(const TYPE& value) throw() {
  TYPE* a = elements;
  unsigned int count = size;

  while (count--) {
    *(a++) = value;
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::operator=(const Matrix& matrix) throw() {
  if (matrix.size != size) {
    delete[] elements;

    elements = new TYPE[matrix.size];
  }
  *dimension = *matrix.dimension;
  rows = dimension->getWidth();
  columns = dimension->getHeight();
  size = dimension->getSize();

  TYPE* a = elements;
  TYPE* b = matrix.elements;
  unsigned int count = size;

  while (count--) {
    *(a++) = *(b++);
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::operator+() throw() {
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::operator+=(const Matrix& matrix) throw(IncompatibleMatrices) {
  if (dimension == matrix.dimension) {
    throw IncompatibleMatrices();
  }

  TYPE* a = elements;
  TYPE* b = matrix.elements;
  unsigned int count = size;

  while (count--) {
    *(a++) += *(b++);
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::operator-() throw() {
  TYPE* a = elements;
  unsigned int count = size;

  while (count--) {
    *a = -(*a);
    a++;
  }

  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::operator-=(const Matrix& matrix) throw(IncompatibleMatrices) {
  if (dimension == matrix.dimension) {
    throw IncompatibleMatrices();
  }

  TYPE* a = elements;
  TYPE* b = matrix.elements;
  unsigned int count = size;

  while (count--) {
    *(a++) -= *(b++);
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::operator*=(const TYPE& value) throw() {
  TYPE* a = elements;
  unsigned int count = size;

  while (count--) {
    *(a++) *= value;
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::operator/=(const TYPE& value) throw() {
  TYPE* a = elements;
  unsigned int count = size;

  while (count--) {
    *(a++) /= value;
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::identity() throw(NotSquare) {
  if (!isSquare()) {
    throw NotSquare();
  }

  TYPE* element = elements;

  for (unsigned int row = 0; row < rows; row++) {
    for (unsigned int column = 0; column < columns; column++) {
      *(element++) = (row == column) ? 1 : 0;
    }
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::add(const Matrix& a, const Matrix& b) throw(IncompatibleMatrices) {
  if ((dimension != a.dimension) || (dimension != b.dimension)) {
    throw IncompatibleMatrices();
  }

  TYPE* result = elements;
  TYPE* A = a.elements;
  TYPE* B = b.elements;
  unsigned int count = size;

  while (count--) {
    *(result++) = *(A++) + *(B++);
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::subtract(const Matrix& a, const Matrix& b) throw(IncompatibleMatrices) {
  if ((dimension != a.dimension) || (dimension != b.dimension)) {
    throw IncompatibleMatrices();
  }

  TYPE* result = elements;
  TYPE* A = a.elements;
  TYPE* B = b.elements;
  unsigned int count = size;

  while (count--) {
    *(result++) = *(A++) - *(B++);
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::multiply(const Matrix<TYPE>& a, const Matrix<TYPE>& b) throw(IncompatibleMatrices) {
  if ((a.columns != b.rows) || (rows != a.rows) || (columns != b.columns)) {
    throw IncompatibleMatrices();
  }

  unsigned int length = b.rows; /* If A=[k,l] and B=[l,m] then common = l */
  unsigned int size = b.size;
  TYPE* result = elements;
  TYPE* A = a.elements; /* a[0,0] */
  TYPE* B = b.elements; /* b[0,0] */
  TYPE sum;

  unsigned int k = rows;
  while (k-- > 0) { /* Rows of result matrix */

    unsigned int m = columns;
    while (m-- > 0) { /* Columns of result matrix */

      sum = 0;
      unsigned int l = length;
      while (l-- > 0) {
        sum += *A * *B;
        A++; /* Go to next column [r,c]=[r,c+1] */
        B += columns; /* Go to next row [r,c]=[r+1,c] */
      }
      *(result++) = sum;

      A -= length; /* Go to first column of current row */
      B -= size; /* Go to first row of current column */
      B++; /* Go to the next column of the current row */

    }

    A += length; /* Go to the first column of the next row */
    B -= length; /* Go to b[0,0] */

  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::multiply(const Matrix<TYPE>& a, const Vector<TYPE>& b) throw(MatrixException) {
  if ((a.columns != b.length) || (rows != a.rows) || (columns != 1)) {
    throw MatrixException();
  }

  unsigned int length = b.length;
  TYPE* result = elements;
  TYPE* A = a.elements; /* a[0,0] */
  TYPE* B = b.elements; /* The first element */
  TYPE sum;

  unsigned int k = rows;
  while (k-- > 0) { /* Rows of result matrix */

    sum = 0;
    l = length;
    while (l-- > 0) {
      sum += *A * *B;
      A++; /* Go to next column [r,c]=[r,c+1] */
      B++; /* Go to next row */
    }
    *(result++) = sum;

    B -= length; /* Go to first element */
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::multiply(const Vector<TYPE>& a, const Matrix<TYPE>& b) throw(MatrixException) {
  if ((a.length != b.rows) || (columns != b.columns) || (rows != 1)) {
    throw MatrixException();
  }

  unsigned int length = a.length;
  TYPE* result = elements;
  TYPE* A = a.elements; /* The first element */
  TYPE* B = b.elements; /* b[0,0] */
  TYPE sum;

  unsigned int m = columns;
  while (m-- > 0) { /* Columns of result matrix */

    sum = 0;
    l = length;
    while (l-- > 0) {
      sum += *A * *B;
      A++; /* Go to next element */
      B += columns; /* Go to next row [r,c]=[r+1,c] */
    }
    *(result++) = sum;

    A -= length; /* Go to first element */
    B -= size; /* Go to first row of current column */
    B++; /* Go to the next column of the current row */
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::multiply(const Vector<TYPE>& a, const Vector<TYPE>& b) throw(MatrixException) {
  if ((rows != a.length) || (columns != b.length)) {
    throw MatrixException();
  }

  TYPE* result = elements;
  TYPE* A = a.elements; /* a[0,0] */
  TYPE* B = b.elements; /* The first element */

  unsigned int k = rows;
  while (k-- > 0) { /* Rows of result matrix */

    unsigned int l = columns;
    while (l-- > 0) { /* Columns of result matrix */
      *(result++) = *A * *B;
      B++; /* Go to next element */
    }

    A++; /* Go to next element */
    B -= length; /* Go to first element */
  }

  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::operator*(const TYPE& value) throw() {
  Matrix<TYPE> result = Matrix(this);
  result *= value;
  return result;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::operator*(const Matrix& matrix) throw() {
  Matrix<TYPE> result = Matrix(Dimension(rows, matrix.columns));
  result.multiply(this, matrix);
  return result;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::transpose(const Matrix& matrix) throw() {
  if ((rows != matrix.columns) || (columns != matrix.rows)) {
    adjustDimension(matrix.columns, matrix.rows);
  }

  TYPE* a = elements;
  TYPE* b = matrix.elements;

  unsigned int r = rows;
  while (r-- > 0) {
    unsigned int c = columns;
    while (c-- > 0) { /* Copy column of b to row of a */
      *(a++) = *b;
      b += rows; /* Go to the next row */
    }
    b -= size;
    b++; /* Go to next column */
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::transpose() const throw() {
  Matrix<TYPE> matrix = Matrix<TYPE>(Dimension(columns, rows));
  matrix.transpose(*this);
  return matrix;
}

template<class TYPE> void Matrix<TYPE>::toStream(ostream& stream) const {
  TYPE* element = elements;

  stream << "[";
  for (unsigned int row = 0; row < rows; row++) {

    stream << "[";
    for (unsigned int column = 0; column < columns; column++) {
      stream << *(element++);
      if (column < (columns - 1)) {
        stream << ";";
      }
    }
    stream << "]";

  }
  stream << "]";
}

template<class TYPE> Matrix<TYPE>::~Matrix() throw() {
  delete[] elements;
}
