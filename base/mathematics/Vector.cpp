/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Vector.h"
#include <math.h>

template<class TYPE> Vector<TYPE>::Vector(unsigned int length) throw() {
  this->length = length;
  elements = new TYPE[length];
}

template<class TYPE> Vector<TYPE>::Vector(const TYPE elements[], unsigned int length) throw() {
  this->length = length;
  elements = new TYPE[length];

  /* Copy the elements. */
  for (unsigned int i = 0; i < length; i++) {
    this->elements[i] = elements[i];
  }
}

template<class TYPE> Vector<TYPE>::Vector(const Vector& vector) throw() {
  this->length = vector.length;
  elements = new TYPE[length];

  /* Copy the elements. */
  for (unsigned int i = 0; i < length; i++) {
    this->elements[i] = vector.elements[i];
  }
}

template<class TYPE> TYPE* Vector<TYPE>::getElements() const throw() {
  return elements;
}

template<class TYPE> unsigned int Vector<TYPE>::getLength() const throw() {
  return length;
}

template<class TYPE> TYPE& Vector<TYPE>::operator[](unsigned int index) const throw(OutOfRange) {
  if (index >= length) {
    throw RangeException();
  }
  return elements[index];
}

template<class TYPE> TYPE& Vector<TYPE>::getAt(unsigned int index) const throw(OutOfRange) {
  if (index >= length) {
    throw RangeException();
  }
  return elements[index];
}

template<class TYPE> void Vector<TYPE>::setAt(unsigned int index, const TYPE& value) throw(OutOfRange) {
  if (index >= length) {
    throw RangeException();
  }
  elements[index] = value;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::operator=(const Vector& vector) throw(IncompatibleVectors) {
  if (length != vector.length) {
    throw IncompatibleVectors();
  }

  TYPE* a = elements;
  TYPE* b = vector.elements;
  unsigned int count = length;

  while (count-- > 0) {
    *(a++) = *(b++);
  }
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::operator=(const TYPE& value) throw() {
  TYPE* result = elements;
  unsigned int count = length;

  while (count-- > 0) {
    *(result++) = value;
  }
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::add(const Vector<TYPE>& vector) throw(IncompatibleVectors) {
  if (length != vector.length) {
    throw IncompatibleVectors();
  }

  TYPE* a = elements;
  TYPE* b = vector.elements;
  unsigned int count = length;

  while (count-- > 0) {
    *(a++) += *(b++);
  }
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::operator+=(const Vector<TYPE>& vector) throw(IncompatibleVectors) {
  if (length != vector.length) {
    throw IncompatibleVectors();
  }

  TYPE* a = elements;
  TYPE* b = vector.elements;
  unsigned int count = length;

  while (count-- > 0) {
    *(a++) += *(b++);
  }
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::operator+=(const TYPE& value) throw() {
  TYPE* a = elements;
  unsigned int count = length;

  while (count-- > 0) {
    *(a++) += value;
  }
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::subtract(const Vector<TYPE>& vector) throw(IncompatibleVectors) {
  if (length != vector.length) {
    throw IncompatibleVectors();
  }

  TYPE* a = elements;
  TYPE* b = vector.elements;
  unsigned int count = length;

  while (count-- > 0) {
    *(a++) -= *(b++);
  }
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::operator-=(const Vector<TYPE>& vector) throw(IncompatibleVectors) {
  if (length != vector.length) {
    throw IncompatibleVectors();
  }

  TYPE* a = elements;
  TYPE* b = vector.elements;
  unsigned int count = length;

  while (count-- > 0) {
    *(a++) -= *(b++);
  }
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::operator-=(const TYPE& value) throw() {
  TYPE* a = elements;
  unsigned int count = length;

  while (count-- > 0) {
    *(a++) -= value;
  }
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::operator*=(const TYPE& value) throw() {
  TYPE* a = elements;
  unsigned int count = length;

  while (count-- > 0) {
    *(a++) *= value;
  }
  return *this;
}

template<class TYPE> Vector<TYPE>& Vector<TYPE>::operator*(const TYPE& value) throw() {
  Vector<TYPE> result = Vector(length);
  TYPE* source = elements;
  TYPE* destination = result.elements;
  unsigned int count = length;

  while (count--) {
    *(destination++) = *(source++) * value;
  }

  return result;
}

template<class TYPE> bool Vector<TYPE>::operator==(const Vector& vector) const throw(IncompatibleVectors) {
  if (length != vector.length) {
    throw IncompatibleVectors();
  }

  TYPE* a = elements;
  TYPE* b = vector.elements;
  unsigned int count = length;

  while (count-- > 0) {
    if (*(a++) != *(b++)) {
      return false;
    }
  }

  return true;
}

template<class TYPE> TYPE& Vector<TYPE>::dot(const Vector<TYPE>& vector) const throw(IncompatibleVectors) {
  if (length != vector.length) {
    throw IncompatibleVectors();
  }

  TYPE result = 0;
  TYPE* a = elements;
  TYPE* b = vector.elements;
  unsigned int count = length;

  while (count-- > 0) {
    result += *(a++) * *(b++);
  }

  return result;
}

template<class TYPE> TYPE& Vector<TYPE>::norm() const throw() {
  TYPE result = 0;
  TYPE* element = elements;
  unsigned int count = length;

  while (count-- > 0) {
    result += *element * *element;
    element++;
  }

  return sqrt(result);
}

template<class TYPE> Vector<TYPE>::~Vector() throw() {
  delete[] elements;
}
