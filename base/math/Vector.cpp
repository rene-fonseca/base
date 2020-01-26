/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/math/Vector.h>
#include <base/math/Math.h>
#include <base/Functor.h>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// float, long double
// template class _COM_AZURE_DEV__BASE__API Vector<double>;

template<class TYPE>
Vector<TYPE>::Vector(unsigned int size)
{
  if (size < 1) {
    _throw OutOfDomain(this);
  }
  setSize(size);
}

template<class TYPE>
Vector<TYPE>::Vector(
  const TYPE elements[], unsigned int size)
{
  if (size < 1) {
    _throw OutOfDomain(this);
  }
  setSize(size);
  copy<TYPE>(getElements(), elements, getSize());
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::operator=(const Vector& assign)
{
  if (&assign != this) { // protect against self assignment
    elements = assign.elements;
  }
  return *this;
}

template<class TYPE>
const TYPE& Vector<TYPE>::getAt(unsigned int index) const
{
  if (index >= getSize()) {
    _throw OutOfRange(this);
  }
  return getReadOnlyElements()[index];
}

template<class TYPE>
void Vector<TYPE>::setAt(unsigned int index, const TYPE& value)
{
  if (index >= getSize()) {
    _throw OutOfRange(this);
  }
  getElements()[index] = value;
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::clear() noexcept
{
  fill(getElements(), getSize(), TYPE(0));
  return *this;
}

template<class TYPE>
Vector<TYPE> Vector<TYPE>::plus() const noexcept
{
  return Vector(*this);
}

template<class TYPE>
Vector<TYPE> Vector<TYPE>::minus() const noexcept
{
  return Vector(getSize()).negate(*this);
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::negate() noexcept {
  transform(getElements(), getSize(), Negate<TYPE>());
  return *this;
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::add(const Vector<TYPE>& value)
{
  if (value.getSize() != getSize()) {
    _throw IncompatibleVectors(this);
  }
  transformByBinary<TYPE>(getElements(), value.getReadOnlyElements(), getSize(), Add<TYPE>());
  return *this;
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::subtract(const Vector<TYPE>& value)
{
  if (value.getSize() != getSize()) {
    _throw IncompatibleVectors(this);
  }
  transformByBinary<TYPE>(getElements(), value.getReadOnlyElements(), getSize(), Subtract<TYPE>());
  return *this;
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::multiply(const TYPE& value) noexcept
{
  transform(getElements(), getSize(), bind2Second(Multiply<TYPE>(), value));
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::divide(const TYPE& value) noexcept
{
  transform(getElements(), getSize(), bind2Second(Divide<TYPE>(), value));
  return *this;
}

template<class TYPE>
Vector<TYPE>& Vector<TYPE>::negate(const Vector<TYPE>& value) noexcept
{
//  setDimension(value);
  transformByUnary(getElements(), value.getReadOnlyElements(), getSize(), Negate<TYPE>());
  return *this;
}

template<class TYPE>
TYPE Vector<TYPE>::dotdot() const noexcept
{
  SquareSum<TYPE> squareSum;
  forEach(getReadOnlyElements(), getSize(), squareSum);
  return squareSum.getResult();
}

template<class TYPE>
TYPE Vector<TYPE>::norm() const noexcept
{
  SquareSum<TYPE> squareSum;
  forEach(getReadOnlyElements(), getSize(), squareSum);
  return Math::sqrt(squareSum.getResult());
}

template<class TYPE>
bool Vector<TYPE>::operator==(const Vector& value) const
{
  if (value.getSize() != getSize()) {
    _throw IncompatibleVectors(this);
  }
  return equal(getReadOnlyElements(), value.getReadOnlyElements(), getSize());
}

template<class TYPE>
Vector<TYPE> operator*(const Vector<TYPE>& left, const TYPE& right)
{
  return Vector<TYPE>(left).multiply(right);
}

template<class TYPE>
Vector<TYPE> operator*(const TYPE& left, const Vector<TYPE>& right)
{
  return Vector<TYPE>(right).multiply(left);
}

template<class TYPE>
Vector<TYPE> operator/(const Vector<TYPE>& left, const TYPE& right)
{
  return Vector<TYPE>(left).divide(right);
}

template<class TYPE>
TYPE dot(const Vector<TYPE>& left, const Vector<TYPE>& right) noexcept
{
  if (left.getSize() != right.getSize()) {
    _throw Vector<TYPE>::IncompatibleVectors();
  }
  DotProduct<TYPE> dotProduct;
  forEach(left.getReadOnlyElements(), right.getReadOnlyElements(), left.getSize(), dotProduct);
  return dotProduct.getResult();
}

template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector<TYPE>& value)
{
  FormatOutputStream::PushContext push(stream);
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
