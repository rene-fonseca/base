/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mathematics/Vector.h>
#include <base/Functor.h>
#include <math.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

template<class TYPE>
Vector<TYPE>::Vector(unsigned int size) throw(OutOfDomain) {
  if (size < 1) {
    throw OutOfDomain(this);
  }
  setSize(size);
}

template<class TYPE>
Vector<TYPE>::Vector(const TYPE elements[], unsigned int size) throw(OutOfDomain) {
  if (size < 1) {
    throw OutOfDomain(this);
  }
  setSize(size);
  copy<TYPE>(getElements(), elements, getSize());
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::operator=(const Vector& eq) throw(MemoryException) {
  if (&eq != this) { // protect against self assignment
    elements = eq.elements;
  }
  return *this;
}

template<class TYPE>
TYPE Vector<TYPE>::getAt(unsigned int index) const throw(OutOfRange) {
  if (index >= getSize()) {
    throw OutOfRange(this);
  }
  return getReadOnlyElements()[index];
}

template<class TYPE>
void Vector<TYPE>::setAt(unsigned int index, const TYPE& value) throw(OutOfRange) {
  if (index >= getSize()) {
    throw OutOfRange(this);
  }
  getElements()[index] = value;
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::clear() throw() {
  fill(getElements(), getSize(), TYPE(0));
  return *this;
}

template<class TYPE>
Vector<TYPE> Vector<TYPE>::plus() const throw() {
  return Vector(*this);
}

template<class TYPE>
Vector<TYPE> Vector<TYPE>::minus() const throw() {
  return Vector(getSize()).negate(*this);
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::negate() throw() {
  transform(getElements(), getSize(), Negate<TYPE>());
  return *this;
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::add(const Vector<TYPE>& value) throw(IncompatibleVectors) {
  if (value.getSize() != getSize()) {
    throw IncompatibleVectors(this);
  }
  transformByBinary<TYPE>(getElements(), value.getReadOnlyElements(), getSize(), Add<TYPE>());
  return *this;
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::subtract(const Vector<TYPE>& value) throw(IncompatibleVectors) {
  if (value.getSize() != getSize()) {
    throw IncompatibleVectors(this);
  }
  transformByBinary<TYPE>(getElements(), value.getReadOnlyElements(), getSize(), Subtract<TYPE>());
  return *this;
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::multiply(const TYPE& value) throw() {
  transform(getElements(), getSize(), bind2Second(Multiply<TYPE>(), value));
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::divide(const TYPE& value) throw() {
  transform(getElements(), getSize(), bind2Second(Divide<TYPE>(), value));
  return *this;
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::negate(const Vector<TYPE>& value) throw() {
//  setDimension(value);
  transformByUnary(getElements(), value.getReadOnlyElements(), getSize(), Negate<TYPE>());
  return *this;
}

template<class TYPE>
TYPE Vector<TYPE>::dotdot() const throw() {
  SquareSum<TYPE> squareSum;
  forEach(getReadOnlyElements(), getSize(), squareSum);
  return squareSum.getResult();
}

template<class TYPE>
TYPE Vector<TYPE>::norm() const throw() {
  SquareSum<TYPE> squareSum;
  forEach(getReadOnlyElements(), getSize(), squareSum);
  return sqrt(squareSum.getResult());
}

template<class TYPE>
bool Vector<TYPE>::operator==(const Vector& value) const throw(IncompatibleVectors) {
  if (value.getSize() != getSize()) {
    throw IncompatibleVectors(this);
  }
  return equal(getReadOnlyElements(), value.getReadOnlyElements(), getSize());
}

template<class TYPE>
Vector<TYPE> operator*(const Vector<TYPE>& left, const TYPE& right) throw(MemoryException) {
  return Vector<TYPE>(left).multiply(right);
}

template<class TYPE>
Vector<TYPE> operator*(const TYPE& left, const Vector<TYPE>& right) throw(MemoryException) {
  return Vector<TYPE>(right).multiply(left);
}

template<class TYPE>
Vector<TYPE> operator/(const Vector<TYPE>& left, const TYPE& right) throw(MemoryException) {
  return Vector<TYPE>(left).divide(right);
}

template<class TYPE>
TYPE dot(const Vector<TYPE>& left, const Vector<TYPE>& right) throw() {
  if (left.getSize() != right.getSize()) {
    throw Vector<TYPE>::IncompatibleVectors();
  }
  DotProduct<TYPE> dotProduct;
  forEach(left.getReadOnlyElements(), right.getReadOnlyElements(), left.getSize(), dotProduct);
  return dotProduct.getResult();
}

template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector<TYPE>& value) {
  stream << '(';
  const TYPE* element = value.getReadOnlyElements();
  unsigned int count = value.getSize();
  while (--count) { // vector has a size greater than 0 - do for all except last element
    stream << *element << ',';
    ++element;
  }
  stream << *element << ')'; // dump the last element of the vector
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
