/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Matrix.h"
#include "base/Functor.h"

template Matrix<float>;
template Matrix<double>;
template Matrix<long double>;

template<class TYPE>
 Matrix<TYPE>::Matrix(const Dimension& dimension) throw(OutOfDomain) {
  if (dimension.getSize() < 1) {
    throw OutOfDomain();
  }
  setSize(dimension.getHeight(), dimension.getWidth());
}

template<class TYPE>
Matrix<TYPE>::Matrix(const TYPE elements[], const Dimension& dimension) throw(OutOfDomain) {
  if (dimension.getSize() < 1) {
    throw OutOfDomain();
  }
  setSize(dimension.getHeight(), dimension.getWidth());
  copyArray<TYPE>(getElements(), elements, getSize());
}

template<class TYPE>
Matrix<TYPE>::Matrix(Enumeration<TYPE>& diagonal, const Dimension& dimension) throw(OutOfDomain) {
  if (dimension.getSize() < 1) {
    throw OutOfDomain();
  }
  setSize(dimension.getHeight(), dimension.getWidth());
  identity();
  unsigned int i = 0;
  while (diagonal.hasNext()) {
    setAt(i, i, *diagonal.next());
    ++i;
  }
}



template<class TYPE>
TYPE Matrix<TYPE>::getAt(unsigned int row, unsigned int column) const throw(OutOfRange) {
  validateElement(row, column);
  return getReadOnlyElements()[row * getColumns() + column];
}

template<class TYPE>
void Matrix<TYPE>::setAt(unsigned int row, unsigned int column, const TYPE& value) throw(OutOfRange) {
  validateElement(row, column);
  getElements()[row * getColumns() + column] = value;
}



template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::clear() throw() {
  fill(getElements(), getSize(), TYPE(0));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::identity() throw() {
  TYPE* element = getElements();
  for (unsigned int row = 0; row < rows; row++) {
    for (unsigned int column = 0; column < columns; column++) {
      *element = (row == column) ? 1 : 0;
      ++element;
    }
  }
  return *this;
}

template<class TYPE>
Matrix<TYPE> Matrix<TYPE>::plus() const throw() {
  return Matrix(*this);
}

template<class TYPE>
Matrix<TYPE> Matrix<TYPE>::minus() const throw() {
  return Matrix(getDimension()).negate(*this);
}

template<class TYPE>
Matrix<TYPE> Matrix<TYPE>::transpose() const throw() {
  return Matrix(getDimension()).transpose(*this);
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::negate() throw() {
  transform(getElements(), getSize(), Negate<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::add(const Matrix<TYPE>& value) throw(IncompatibleOperands) {
  if (!areCompatible(value)) {
    throw IncompatibleOperands();
  }
  transformByBinary(getElements(), getReadOnlyElements(), getSize(), Add<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::subtract(const Matrix<TYPE>& value) throw(IncompatibleOperands) {
  if (!areCompatible(value)) {
    throw IncompatibleOperands();
  }
  transformByBinary(getElements(), getReadOnlyElements(), getSize(), Subtract<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::multiply(const TYPE& value) throw() {
  transform(getElements(), getSize(), bind2Second(Multiply<TYPE>(), value));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::divide(const TYPE& value) throw() {
  transform(getElements(), getSize(), bind2Second(Divide<TYPE>(), value));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::negate(const Matrix<TYPE>& value) throw() {
  setDimension(value);
  transformByUnary(getElements(), value.getReadOnlyElements(), getSize(), Negate<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::add(const Matrix& left, const Matrix& right) throw(IncompatibleOperands) {
  if (!left.areCompatible(right)) {
    throw IncompatibleOperands();
  }
  setDimension(left);
  transformByBinary(getElements(), left.getReadOnlyElements(), right.getReadOnlyElements(), getSize(), Add<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::subtract(const Matrix& left, const Matrix& right) throw(IncompatibleOperands) {
  if (!left.areCompatible(right)) {
    throw IncompatibleOperands();
  }
  setDimension(left);
  transformByBinary(getElements(), left.getReadOnlyElements(), right.getReadOnlyElements(), getSize(), Add<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::multiply(const Matrix<TYPE>& left, const Matrix<TYPE>& right) throw(IncompatibleOperands) {
  if (left.columns != right.rows) {
    throw IncompatibleOperands();
  }
  setSize(left.rows, right.columns);

  unsigned int length = right.rows; // If A=[k,l] and B=[l,m] then common = l
  unsigned int size = right.getSize();
  TYPE* result = getElements();
  const TYPE* A = left.getReadOnlyElements(); // a[0,0]
  const TYPE* B = right.getReadOnlyElements(); // b[0,0]
  TYPE sum;

  unsigned int k = rows;
  while (k-- > 0) { // Rows of result matrix

    unsigned int m = columns;
    while (m-- > 0) { // Columns of result matrix

      sum = 0;
      unsigned int l = length;
      while (l-- > 0) {
        sum += *A * *B;
        A++; // Go to next column [r,c]=[r,c+1]
        B += columns; // Go to next row [r,c]=[r+1,c]
      }
      *result = sum;
      ++result;

      A -= length; // Go to first column of current row
      B -= size; // Go to first row of current column
      B++; // Go to the next column of the current row

    }

    A += length; // Go to the first column of the next row
    B -= length; // Go to b[0,0]

  }
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::multiply(const Matrix<TYPE>& left, const TYPE& right) throw() {
  setDimension(left);
  transformByUnary(getElements(), left.getReadOnlyElements(), getSize(), bind2Second(Multiply<TYPE>(), right));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::multiply(const TYPE& left, const Matrix<TYPE>& right) throw() {
  setDimension(right);
  transformByUnary(getElements(), right.getReadOnlyElements(), getSize(), bind2First(Multiply<TYPE>(), left));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::divide(const Matrix<TYPE>& left, const TYPE& right) throw() {
  setDimension(left);
  transformByUnary(getElements(), left.getReadOnlyElements(), getSize(), bind2Second(Divide<TYPE>(), right));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::transpose(const Matrix& value) throw() {
  setSize(value.columns, value.rows);

  TYPE* dest = getElements();
  const TYPE* src = value.getReadOnlyElements();
  unsigned int size = elements->getSize();

  unsigned int r = rows;
  while (r-- > 0) {
    unsigned int c = columns;
    while (c-- > 0) { // Copy column to row
      *dest = *src;
      ++dest;
      src += rows; // Go to the next row
    }
    src -= size;
    ++src; // Go to next column
  }
  return *this;
}

template<class TYPE>
bool Matrix<TYPE>::operator==(const Matrix& value) const throw(IncompatibleOperands) {
  if (!areCompatible(value)) {
    throw IncompatibleOperands();
  }
  return equal(getReadOnlyElements(), value.getReadOnlyElements(), getSize());
}

template<class TYPE>
Matrix<TYPE> operator*(const Matrix<TYPE>& left, const Matrix<TYPE>& right) throw(MemoryException) {
  return Matrix<TYPE>().multiply(left, right);
}

template<class TYPE>
Matrix<TYPE> operator*(const Matrix<TYPE>& left, const TYPE& right) throw(MemoryException) {
  return Matrix<TYPE>().multiply(left, right);
}

template<class TYPE>
Matrix<TYPE> operator*(const TYPE& left, const Matrix<TYPE>& right) throw(MemoryException) {
  return Matrix<TYPE>().multiply(left, right);
}

template<class TYPE>
Matrix<TYPE> operator/(const Matrix<TYPE>& left, const TYPE& right) throw(MemoryException) {
  return Matrix<TYPE>().divide(left, right);
}

template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Matrix<TYPE>& value) {
  const TYPE* element = getReadOnlyElements();
  stream << '[';
  for (unsigned int row = 0; row < value.rows; ++row) {
    stream << '[';
    for (unsigned int column = 0; column < value.columns; ++column) {
      stream << *element;
      ++element;
      if (column < (columns - 1)) {
        stream << ',';
      }
    }
    stream << ']';
  }
  stream << ']';
  return stream;
}

/*
template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::multiply(const Matrix<TYPE>& a, const Vector<TYPE>& b) throw(MatrixException) {
  if ((a.columns != b.length) || (rows != a.rows) || (columns != 1)) {
    throw MatrixException();
  }

  unsigned int length = b.length;
  TYPE* result = elements;
  TYPE* A = a.elements; // a[0,0]
  TYPE* B = b.elements; // The first element
  TYPE sum;

  unsigned int k = rows;
  while (k-- > 0) { // Rows of result matrix

    sum = 0;
    l = length;
    while (l-- > 0) {
      sum += *A * *B;
      A++; // Go to next column [r,c]=[r,c+1]
      B++; // Go to next row
    }
    *(result++) = sum;

    B -= length; // Go to first element
  }
  return *this;
}

template<class TYPE> Matrix<TYPE>& Matrix<TYPE>::multiply(const Vector<TYPE>& a, const Matrix<TYPE>& b) throw(MatrixException) {
  if ((a.length != b.rows) || (columns != b.columns) || (rows != 1)) {
    throw MatrixException();
  }

  unsigned int length = a.length;
  TYPE* result = elements;
  TYPE* A = a.elements; // The first element
  TYPE* B = b.elements; // b[0,0]
  TYPE sum;

  unsigned int m = columns;
  while (m-- > 0) { // Columns of result matrix

    sum = 0;
    l = length;
    while (l-- > 0) {
      sum += *A * *B;
      A++; // Go to next element
      B += columns; // Go to next row [r,c]=[r+1,c]
    }
    *(result++) = sum;

    A -= length; // Go to first element
    B -= size; // Go to first row of current column
    B++; // Go to the next column of the current row
  }
  return *this;
}
*/
int main() {
  Matrix<double> a;
  Matrix<double> b;
  Matrix<double> c(a);
  c = a * b * 4.;
  return 0;
}
