/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/math/Matrix.h>
#include <base/math/Math.h>
#include <base/Functor.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template _COM_AZURE_DEV__BASE__API class Matrix<float>;
template _COM_AZURE_DEV__BASE__API class Matrix<double>;
template _COM_AZURE_DEV__BASE__API class Matrix<long double>;

template<class TYPE>
class MultiplySubtract : public BinaryOperation<TYPE, TYPE, TYPE> {
protected:
  
  TYPE value;
public:
  
  inline MultiplySubtract(const TYPE& _value) noexcept : value(_value) {
  }
  
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {
    return left - value * right;
  }
};

template<class TYPE>
class ZeroAdjust : public UnaryOperation<TYPE, TYPE> {
protected:
  
  TYPE zero;
public:
  
  inline ZeroAdjust(const TYPE& _zero) : zero(_zero) {
  }
  
  inline TYPE operator()(const TYPE& value) const throw() {
    return (value > zero) ? value : TYPE(0);
  }
};

template<class TYPE>
Matrix<TYPE>::Matrix(const Dimension& dimension) throw(OutOfDomain) {
  bassert(dimension.isProper(), OutOfDomain());
  setSize(dimension.getHeight(), dimension.getWidth());
}

template<class TYPE>
Matrix<TYPE>::Matrix(const TYPE elements[], const Dimension& dimension) throw(OutOfDomain) {
  bassert(dimension.isProper(), OutOfDomain());
  setSize(dimension.getHeight(), dimension.getWidth());
  copy<TYPE>(getMutableElements(), elements, getSize());
}

template<class TYPE>
Matrix<TYPE>::Matrix(Enumerator<ReadEnumeratorTraits<TYPE> >& diagonal, const Dimension& dimension) throw(OutOfDomain) {
  bassert(dimension.isProper(), OutOfDomain());
  setSize(dimension.getHeight(), dimension.getWidth());
  identity();
  unsigned int i = 0;
  while (diagonal.hasNext()) {
    setAt(i, i, *diagonal.next());
    ++i;
  }
}



template<class TYPE>
const TYPE& Matrix<TYPE>::getAt(unsigned int row, unsigned int column) const throw(OutOfRange) {
  validateElement(row, column);
  return getReadOnlyElements()[row * getColumns() + column];
}

template<class TYPE>
void Matrix<TYPE>::setAt(unsigned int row, unsigned int column, const TYPE& value) throw(OutOfRange) {
  validateElement(row, column);
  getMutableElements()[row * getColumns() + column] = value;
}



template<class TYPE>
bool Matrix<TYPE>::isEqual(const Matrix& value) const throw() {
  if (!isCompatible(value)) {
    return false;
  }
  return equal(getReadOnlyElements(), value.getReadOnlyElements(), getSize());
}

template<class TYPE>
bool Matrix<TYPE>::isZero() const throw() {
  return !findPredicate(getReadOnlyElements(), getSize(), bind2Second(NotEqual<TYPE>(), TYPE(0)));
}

template<class TYPE>
bool Matrix<TYPE>::isDiagonal() const throw() {
  if (!isSquare()) {
    return false;
  }

  const TYPE* current = getReadOnlyElements();
  const TYPE* last = current + getSize() - 1;

  while (current < last) { // have we reached the last element
    ++current; // just skip the diagonal element
    if (findPredicate(current, columns, bind2Second(NotEqual<TYPE>(), TYPE(0)))) {
      return false;
    }
    current += columns; // go to next row
  }
  return true;
}

template<class TYPE>
bool Matrix<TYPE>::isLowerTriangular() const throw() {
  if (!isSquare()) {
    return false;
  }

  const TYPE* upper = getReadOnlyElements() + columns; // end of upper triangle (first row)

  for (unsigned int inUpper = columns - 1; inUpper > 0; --inUpper) {
    if (findPredicate(upper - inUpper, inUpper, bind2Second(NotEqual<TYPE>(), TYPE(0)))) {
      return false;
    }
    upper += columns; // go to end of next row
  }
  return true;
}

template<class TYPE>
bool Matrix<TYPE>::isUpperTriangular() const throw() {
  if (!isSquare()) {
    return false;
  }

  const TYPE* lower = getReadOnlyElements(); // first element in lower triangle

  for (unsigned int inLower = 0; inLower < columns; ++inLower) {
    if (findPredicate(lower, inLower, bind2Second(NotEqual<TYPE>(), TYPE(0)))) {
      return false;
    }
    lower += columns; // go to next row
  }
  return true;
}

template<class TYPE>
bool Matrix<TYPE>::isSymmetric() const throw() {
  if (!isSquare()) {
    return false;
  }

  const TYPE* upper = getReadOnlyElements(); // first diagonal element
  const TYPE* lower = upper + getSize() - 1; // after last element of previous column
  unsigned int inUpper = columns; // number of elements in upper triangle on the current row
  unsigned int inLowerAndDiagonal = 0; // number of elements in the lower triangle on the row + diag elem
  unsigned int size = getSize();

  while (inUpper > 0) { // have we reached the last element
    --inUpper; // there is one less element in this row than the previous row
    ++inLowerAndDiagonal;

    // skip elements of opposite triangle and diagonal
    upper += inLowerAndDiagonal; // go to first element to be compared
    lower -= size - 1 - columns * inLowerAndDiagonal; // go to first element to be compared
    unsigned int elems = inUpper; // number of elements in lower and upper triangle to be compared

    while (elems) {
      if (*lower != *upper) {
        return false;
      }
      ++upper; // go to next element in row
      lower += columns; // go to next element in column
      --elems;
    }
  }
  return true;
}



template<class TYPE>
TYPE Matrix<TYPE>::getNorm() const throw() {
  SquareSum<TYPE> squareSum;
  forEach(getReadOnlyElements(), getSize(), squareSum);
  return Math::sqrt(squareSum.getResult());
}

template<class TYPE>
TYPE Matrix<TYPE>::getDeterminant() const throw(NotSquare, MemoryException) {
  bassert(isSquare(), NotSquare());

  Matrix temp(*this); // make a copy of this matrix
  TYPE* elements = temp.getMutableElements();
  TYPE result = TYPE(1);

  for (unsigned int diagonal = 0; diagonal < rows; ++diagonal) { // for all diagonal elements do

    // find the row with the maximum value in the column (diagonal) - from and including row (diagonal)
    const TYPE* current = elements + getIndexOfElement(diagonal, diagonal);
    unsigned int maximumRow = diagonal;
    TYPE maximumValue = TYPE(0);
    for (unsigned int row = diagonal; row < rows; ++row) {
      TYPE temp;
      if ((temp = absolute<TYPE>(*current)) > maximumValue) {
        maximumValue = temp;
        maximumRow = row;
      }
      current += columns; // go to element in next row
    }

    if (maximumValue == TYPE(0)) {
      return 0; // determinant is zero
    }

    if (maximumRow != diagonal) { // do we have to swap rows
      swap( // do not swap elements to the left of the diagonal
        elements + getIndexOfElement(diagonal, diagonal),
        elements + getIndexOfElement(maximumRow, diagonal),
        columns - diagonal
      );
      result = -result;
    }

    result *= maximumValue; // update determinant with diagonal

    // make row additions on submatrix (beginning at next diagonal)
    for (unsigned int row = diagonal + 1; row < rows; ++row) {
      transformByBinary(
        elements + getIndexOfElement(row, diagonal + 1),
        elements + getIndexOfElement(diagonal, diagonal + 1),
        columns - diagonal - 1, // columns - diagonal - 1 >= 0
        MultiplySubtract<TYPE>(elements[getIndexOfElement(row, diagonal)]/maximumValue)
      );
    }
  }

  return result;
}

template<class TYPE>
Matrix<TYPE> Matrix<TYPE>::getMinor(unsigned int row, unsigned int column) const throw(OutOfRange, NotSquare, MemoryException) {
  validateElement(row, column);
  bassert(isSquare(), NotSquare());

  Matrix temp(Dimension(rows - 1, columns - 1));

  const TYPE* src = getReadOnlyElements();
  TYPE* dest = temp.getMutableElements();

  for (unsigned int r = 0; r < rows; ++r) {
    if (r != row) {
      for (unsigned int c = 0; c < columns; ++c) {
        if (c != column) {
          *dest = *src;
          ++dest;
        }
        ++src;
      }
    } else {
      src += columns; // skip row
    }
  }

  return temp;
}

template<class TYPE>
TYPE Matrix<TYPE>::getCofactor(unsigned int row, unsigned int column) const throw(OutOfRange, NotSquare, MemoryException) {
  TYPE result = getMinor(row, column).getDeterminant();
  if ((row + column) % 2 == 0) {
    result = -result;
  }
  return result;
}

template<class TYPE>
Matrix<TYPE> Matrix<TYPE>::getAdjoint() const throw(NotSquare, MemoryException) {
  bassert(isSquare(), NotSquare());

  Matrix result(Dimension(rows, columns));
  TYPE* current = result.getMutableElements();

  for (unsigned int row = 0; row < rows; ++row) {
    for (unsigned int column = 0; column < columns; ++column) {
      *current = getCofactor(column, row); // does not throw OutOfRange - 'cause column and row are valid
      ++current;
    }
  }

  return result;
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
Matrix<TYPE> Matrix<TYPE>::invert() const throw(NotSquare, Singular, MemoryException) {
  bassert(isSquare(), NotSquare());

  Matrix result(getDimension());
  result.identity();
  Matrix temp(*this); // make copy of this matrix
  TYPE* tmpArray = temp.getMutableElements();
  TYPE* resArray = result.getMutableElements();

  for (unsigned int diagonal = 0; diagonal < rows; ++diagonal) { // for all diagonal elements do

    // find the row with the maximum value in the column (diagonal) - from row (diagonal) only
    const TYPE* current = tmpArray + getIndexOfElement(diagonal, diagonal);
    unsigned int maximumRow = diagonal;
    TYPE maximumValue = TYPE(0);
    for (unsigned int row = diagonal; row < rows; ++row) {
      TYPE temp;
      if ((temp = absolute<TYPE>(*current)) > maximumValue) {
        maximumValue = temp;
        maximumRow = row;
      }
      current += columns; // go to element in next row
    }

    bassert(maximumValue != TYPE(0), Singular());

    if (maximumRow != diagonal) { // do we have to swap rows
      swap( // only swap from current column
        tmpArray + getIndexOfElement(diagonal, diagonal),
        tmpArray + getIndexOfElement(maximumRow, diagonal),
        columns - diagonal
      );
      swap( // swap all row
        resArray + getIndexOfElement(diagonal, 0),
        resArray + getIndexOfElement(maximumRow, 0),
        columns
      );
    }

    // make diagonal value 1
    const TYPE factor = TYPE(1)/maximumValue;
    transform(tmpArray + getIndexOfElement(diagonal, diagonal), columns - diagonal, bind2Second(Multiply<TYPE>(), factor));
    transform(resArray + getIndexOfElement(diagonal, 0), columns, bind2Second(Multiply<TYPE>(), factor));

    // make row additions
    for (unsigned int row = 0; row < rows; ++row) {
      if (row != diagonal) {
        transformByBinary(
          tmpArray + getIndexOfElement(row, diagonal + 1),
          tmpArray + getIndexOfElement(diagonal, diagonal + 1),
          columns - diagonal - 1,
          MultiplySubtract<TYPE>(tmpArray[getIndexOfElement(row, diagonal)])
        );
        transformByBinary(
          resArray + getIndexOfElement(row, 0),
          resArray + getIndexOfElement(diagonal, 0),
          columns,
          MultiplySubtract<TYPE>(tmpArray[getIndexOfElement(row, diagonal)])
        );
      }
    }
  }

  return result;
}

template<class TYPE>
Matrix<TYPE> Matrix<TYPE>::transpose() const throw() {
  return Matrix(Dimension(columns, rows)).transpose(*this);
}



template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::clear() throw(MemoryException) {
  fill(getMutableElements(), getSize(), TYPE(0));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::identity() throw(MemoryException) {
  TYPE* current = getMutableElements(); // the first element of the matrix
  const TYPE* last = current + getSize() - 1; // the last element of the matrix

  while (current < last) {
    *current = TYPE(1); // set diagonal
    ++current; // go to non-diagonal
    fill(current, columns, TYPE(0));
    current += columns;
  }
  *current = TYPE(1); // set the last diagonal element

  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::zeroAdjust(const TYPE& zero) throw(MemoryException) {
  transform(getMutableElements(), getSize(), ZeroAdjust<TYPE>(zero));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::rowEchelon() throw(MemoryException) {
  TYPE* elements = getMutableElements();

  unsigned int row = 0;
  for (unsigned int column = 0; column < columns; ++column) { // for all columns do

    // find the row with the maximum value in the column - from and including row
    const TYPE* current = elements + getIndexOfElement(row, column);
    unsigned int maximumRow = row;
    TYPE maximumValue = TYPE(0);
    for (unsigned int r = row; r < rows; ++r) {
      TYPE temp;
      if ((temp = absolute<TYPE>(*current)) > maximumValue) {
        maximumValue = temp;
        maximumRow = r;
      }
      current += columns; // go to element in next row
    }

    if (maximumValue == TYPE(0)) {
      continue; // go to next column - stay at the current row
    }

    if (maximumRow != row) { // do we have to swap rows
      // no need to swap elements to the left of the current column - they are all zero at this point
      swap(
        elements + getIndexOfElement(row, column),
        elements + getIndexOfElement(maximumRow, column),
        columns - column
      );
    }

    // make row additions under the current element - elements to the left are zero
    for (unsigned int r = row + 1; r < rows; ++r) {
      transformByBinary(
        elements + getIndexOfElement(r, column),
        elements + getIndexOfElement(row, column),
        columns - column, // columns - column > 0
        MultiplySubtract<TYPE>(elements[getIndexOfElement(r, column)]/maximumValue)
      );
    }

    ++row;
  }

  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::pivot(unsigned int row, unsigned int column) throw(OutOfRange, Singular, MemoryException) {
  validateElement(row, column);
  TYPE* elements = getMutableElements();
  TYPE value = elements[getIndexOfElement(row, column)];

  bassert(value != TYPE(0), Singular());

  // make value of specified element 1
  transform(
    elements + getIndexOfElement(row, 0), // go to row of specified element
    columns,
    bind2Second(Multiply<TYPE>(), TYPE(1)/value)
  );

  // make other values in specified column 0
  for (unsigned r = 0; r < rows; ++r) {
    if (r != row) {
      transformByBinary(
        elements + getIndexOfElement(r, 0), // beginning of current row
        elements + getIndexOfElement(row, 0), // beginning of pivot row
        columns,
        MultiplySubtract<TYPE>(elements[getIndexOfElement(r, column)])
      );
    }
  }
  return *this;
}

template<class TYPE>
Matrix<TYPE> Matrix<TYPE>::solve(const Matrix<TYPE>& value) const throw(IncompatibleOperands, Singular) {
  // back substitution would be more efficient

  bassert(isSquare() && (columns == value.rows), IncompatibleOperands("Unable to solve matrix equations"));

  Matrix temp(*this); // make copy of this matrix
  Matrix result(value);
  TYPE* tmpArray = temp.getMutableElements();
  TYPE* resArray = result.getMutableElements();

  for (unsigned int diagonal = 0; diagonal < rows; ++diagonal) { // for all diagonal elements do

    // find the row with the maximum value in the column (diagonal) - from row (diagonal) only
    const TYPE* current = tmpArray + getIndexOfElement(diagonal, diagonal);
    unsigned int maximumRow = diagonal;
    TYPE maximumValue = TYPE(0);
    for (unsigned int row = diagonal; row < rows; ++row) {
      TYPE temp;
      if ((temp = absolute<TYPE>(*current)) > maximumValue) {
        maximumValue = temp;
        maximumRow = row;
      }
      current += columns; // go to element in next row
    }

    bassert(maximumValue != TYPE(0), Singular());

    if (maximumRow != diagonal) { // do we have to swap rows
      swap( // only swap from current column
        tmpArray + getIndexOfElement(diagonal, diagonal),
        tmpArray + getIndexOfElement(maximumRow, diagonal),
        columns - diagonal
      );
      swap( // swap all row
        resArray + getIndexOfElement(diagonal, 0),
        resArray + getIndexOfElement(maximumRow, 0),
        columns
      );
    }

    // make diagonal value 1
    const TYPE factor = TYPE(1)/maximumValue;
    transform(tmpArray + getIndexOfElement(diagonal, diagonal), columns - diagonal, bind2Second(Multiply<TYPE>(), factor));
    transform(resArray + getIndexOfElement(diagonal, 0), columns, bind2Second(Multiply<TYPE>(), factor));

    // make row additions
    for (unsigned int row = 0; row < rows; ++row) {
      if (row != diagonal) {
        transformByBinary(
          tmpArray + getIndexOfElement(row, diagonal + 1),
          tmpArray + getIndexOfElement(diagonal, diagonal + 1),
          columns - diagonal - 1,
          MultiplySubtract<TYPE>(tmpArray[getIndexOfElement(row, diagonal)])
        );
        transformByBinary(
          resArray + getIndexOfElement(row, 0),
          resArray + getIndexOfElement(diagonal, 0),
          columns,
          MultiplySubtract<TYPE>(tmpArray[getIndexOfElement(row, diagonal)])
        );
      }
    }
  }

  return result;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::clip(unsigned int rows, unsigned int columns) throw(OutOfRange, MemoryException)
{
  if (!((rows <= this->rows) && (columns <= this->columns))) {
    throw OutOfRange();
  }
  
  TYPE* elements = getMutableElements();
  const TYPE* src = elements + this->columns - columns; // first row is ok
  TYPE* dest = elements + columns; // first row is ok

  for (unsigned int r = 1; r < rows; ++r) {
    copy(dest, src, columns);
    src += this->columns - columns;
    dest += columns;
  }

  setSize(rows, columns); // reduce size of matrix but do not modify elements
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::negate() throw(MemoryException) {
  transform(getMutableElements(), getSize(), Negate<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::add(const Matrix<TYPE>& value) throw(IncompatibleOperands, MemoryException) {
  bassert(isCompatible(value), IncompatibleOperands());
  transformByBinary(getMutableElements(), getReadOnlyElements(), getSize(), Add<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::subtract(const Matrix<TYPE>& value) throw(IncompatibleOperands, MemoryException) {
  bassert(isCompatible(value), IncompatibleOperands());
  transformByBinary(getMutableElements(), getReadOnlyElements(), getSize(), Subtract<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::multiply(const TYPE& value) throw(MemoryException) {
  transform(getMutableElements(), getSize(), bind2Second(Multiply<TYPE>(), value));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::divide(const TYPE& value) throw(MemoryException) {
  transform(getMutableElements(), getSize(), bind2Second(Multiply<TYPE>(), TYPE(1)/value));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::negate(const Matrix<TYPE>& value) throw(MemoryException) {
  setDimension(value);
  transformByUnary(getMutableElements(), value.getReadOnlyElements(), getSize(), Negate<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::add(const Matrix& left, const Matrix& right) throw(IncompatibleOperands, MemoryException) {
  bassert(left.isCompatible(right), IncompatibleOperands());
  setDimension(left);
  transformByBinary(getMutableElements(), left.getReadOnlyElements(), right.getReadOnlyElements(), getSize(), Add<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::subtract(const Matrix& left, const Matrix& right) throw(IncompatibleOperands, MemoryException) {
  bassert(left.isCompatible(right), IncompatibleOperands());
  setDimension(left);
  transformByBinary(getMutableElements(), left.getReadOnlyElements(), right.getReadOnlyElements(), getSize(), Add<TYPE>());
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::multiply(const Matrix<TYPE>& left, const Matrix<TYPE>& right) throw(IncompatibleOperands, MemoryException) {
  bassert(left.columns == right.rows, IncompatibleOperands());
  setSize(left.rows, right.columns);

  unsigned int length = right.rows; // If A=[k,l] and B=[l,m] then common = l
  unsigned int size = right.getSize();
  TYPE* result = getMutableElements();
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
Matrix<TYPE>& Matrix<TYPE>::multiply(const Matrix<TYPE>& left, const TYPE& right) throw(MemoryException) {
  setDimension(left);
  transformByUnary(getMutableElements(), left.getReadOnlyElements(), getSize(), bind2Second(Multiply<TYPE>(), right));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::multiply(const TYPE& left, const Matrix<TYPE>& right) throw(MemoryException) {
  setDimension(right);
  transformByUnary(getMutableElements(), right.getReadOnlyElements(), getSize(), bind2First(Multiply<TYPE>(), left));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::divide(const Matrix<TYPE>& left, const TYPE& right) throw(MemoryException) {
  setDimension(left);
  transformByUnary(getMutableElements(), left.getReadOnlyElements(), getSize(), bind2Second(Divide<TYPE>(), right));
  return *this;
}

template<class TYPE>
Matrix<TYPE>& Matrix<TYPE>::transpose(const Matrix& value) throw(MemoryException) {
  setSize(value.columns, value.rows);

  TYPE* dest = getMutableElements();
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
bool operator==(const Matrix<TYPE>& left, const Matrix<TYPE>& right) noexcept
{
  return left.isEqual(right);
}

template<class TYPE>
Matrix<TYPE> operator+(const Matrix<TYPE>& left, const Matrix<TYPE>& right) throw(MemoryException)
{
  return Matrix<TYPE>(left.getDimension()).add(left, right);
}

template<class TYPE>
Matrix<TYPE> operator-(const Matrix<TYPE>& left, const Matrix<TYPE>& right) throw(MemoryException)
{
  return Matrix<TYPE>(left.getDimension()).subtract(left, right);
}

template<class TYPE>
Matrix<TYPE> operator*(const Matrix<TYPE>& left, const Matrix<TYPE>& right) throw(MemoryException)
{
  return Matrix<TYPE>(Dimension(left.rows, right.columns)).multiply(left, right);
}

template<class TYPE>
Matrix<TYPE> operator*(const Matrix<TYPE>& left, const TYPE& right) throw(MemoryException)
{
  return Matrix<TYPE>(left.getDimension()).multiply(left, right);
}

template<class TYPE>
Matrix<TYPE> operator*(const TYPE& left, const Matrix<TYPE>& right) throw(MemoryException)
{
  return Matrix<TYPE>(right.getDimension()).multiply(left, right);
}

template<class TYPE>
Matrix<TYPE> operator/(const Matrix<TYPE>& left, const Matrix<TYPE>& right) throw(MemoryException)
{
  // right is a square matrix
  return Matrix<TYPE>(Dimension(left.rows, right.rows)).multiply(left, right.invert());
}

template<class TYPE>
Matrix<TYPE> operator/(const Matrix<TYPE>& left, const TYPE& right) throw(MemoryException)
{
  return Matrix<TYPE>(left.getDimension()).multiply(left, TYPE(1)/right);
}

template<class TYPE>
Matrix<TYPE> operator/(const TYPE& left, const Matrix<TYPE>& right) throw(MemoryException)
{
  return Matrix<TYPE>(right.getDimension()).multiply(left, right.invert());
}

template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Matrix<TYPE>& value)
{
  FormatOutputStream::PushContext push(stream);
  const TYPE* element = value.getReadOnlyElements();
  stream << '[';
  for (unsigned int row = 0; row < value.getRows(); ++row) {
    stream << '[';
    for (unsigned int column = 0; column < value.getColumns(); ++column) {
      stream << *element;
      ++element;
      if (column < (value.getColumns() - 1)) {
        stream << ',';
      }
    }
    stream << ']';
  }
  stream << ']';
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
