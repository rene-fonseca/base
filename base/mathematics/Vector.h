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
#include <base/OutOfDomain.h>
#include <base/OutOfRange.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/Reference.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/mem/AllocatorEnumeration.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Vector implementation.
  
  @short Vector.
  @ingroup mathematics
  @version 1.10
*/

template<class TYPE> class Vector : public Object {
public:

  typedef typename ReferenceCountedAllocator<TYPE>::Iterator Iterator;
  typedef typename ReferenceCountedAllocator<TYPE>::ReadIterator ReadIterator;
  typedef typename ReferenceCountedAllocator<TYPE>::Enumerator Enumerator;
  typedef typename ReferenceCountedAllocator<TYPE>::ReadEnumerator ReadEnumerator;

  /*
    Reference to an element within a vector.
  */
  class Element {
    friend class Vector;
  private:
    
    Vector& vector; // use reference to avoid 'copy on write'
    unsigned int index = 0;
    Element(const Element& copy) throw();
    Element& operator=(const Element& eq) throw();
    
    inline Element(Vector& _vector, unsigned int _index) throw()
      : vector(_vector), index(_index) {
    }
  public:
    
    inline Element& operator=(const TYPE& value) throw(OutOfRange) {
      vector.setAt(index, value);
      return *this;
    }
    
    inline operator const TYPE&() const throw(OutOfRange) {
      return vector.getAt(index);
    }
  };
protected:

  /**
    The elements of the vector stored in an array. The array is guarantied to
    be non-empty when the vector object has been initialized.
  */
  Reference<ReferenceCountedAllocator<TYPE> > elements;

  /**
    Returns the elements of the vector for modification.
  */
  inline TYPE* getElements() throw(MemoryException) {
    if (elements.isMultiReferenced()) { // do we have the elements for our self
      elements = new ReferenceCountedAllocator<TYPE>(*elements); // make copy of the elements
    }
    return elements->getElements();
  }

  /**
    Returns the elements of the vector for read-only.
  */
  inline const TYPE* getElements() const throw() {
    return elements->getElements();
  }

  /**
    Returns the elements of the vector for read-only.
  */
  inline const TYPE* getReadOnlyElements() const throw() {
    return elements->getElements();
  }

  /**
    Sets the size of the vector. Only invocated by constructors.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    elements = new ReferenceCountedAllocator<TYPE>(size);
  }
public:

  /** Gets the size of the vector. */
  inline unsigned int getSize() const throw() {
    return elements->getSize();
  }
public:

  /**
    @short Exception raised by the Vector class.
  */
  class VectorException : public Exception {
  };
  
  /**
    Raised if an operation is given incompatible vectors to work on.
    
    @short Incompatible vector exception.
  */
  class IncompatibleVectors : public VectorException {
  };
  
  /**
    Initializes vector of the specified size. The elements are not initialized.

    @param size The number of elements in the vector. Must be non zero.
  */
  explicit Vector(unsigned int size) throw(OutOfDomain);

  /**
    Initializes vector from the specified array.

    @param elements The desired elements.
    @param size The number of elements in the array.
  */
  Vector(const TYPE elements[], unsigned int size) throw(OutOfDomain);

  /**
    Initializes vector from other vector.

    @param copy The vector to be copied.
  */
  inline Vector(const Vector& copy) throw() : elements(copy.elements) {
  }

  /**
    Assigns vector to this vector.

    @param eq The vector containing the desired elements.
  */
  Vector& operator=(const Vector& eq) throw(MemoryException);



  /**
    Returns the first element of the allocator as a modifying array.
  */
  inline Iterator getBeginIterator() throw() {
    return elements->getBeginIterator();
  }

  /**
    Returns the end of the allocator as a modifying array.
  */
  inline Iterator getEndIterator() throw() {
    return elements->getEndIterator();
  }

  /**
    Returns the first element of the allocator as a non-modifying array.
  */
  inline ReadIterator getBeginIterator() const throw() {
    return elements->getBeginIterator();
  }

  /**
    Returns the end of the allocator as a non-modifying array.
  */
  inline ReadIterator getEndIterator() const throw() {
    return elements->getEndIterator();
  }

  /**
    Returns a modifying enumerator of the array.
  */
  inline Enumerator getEnumerator() throw() {
    return elements->getEnumerator();
  }

  /**
    Returns a non-modifying enumerator of the array.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return elements->getReadEnumerator();
  }



  // Elements access section



  /**
    Returns the element at the specified index.

    @param index The index of the desired element.
  */
  const TYPE& getAt(unsigned int index) const throw(OutOfRange);

  /**
    Sets the element at the specified index.

    @param index The index of the desired element.
    @param value The desired value.
  */
  void setAt(unsigned int index, const TYPE& value) throw(OutOfRange);

  /**
    Returns the element at the specified index.

    @param index The index of the desired element.
  */
  inline Element operator[](unsigned int index) throw(OutOfRange) {
    return Element(*this, index);
  }



  /**
    Unary plus.
  */
  Vector plus() const throw();

  /**
    Unary minus.
  */
  Vector minus() const throw();

  /**
    Sets this vector to the zero vector.
  */
  Vector& clear() throw();

  /**
    Negates this vector.
  */
  Vector& negate() throw();

  /**
    Adds the specified vector to this vector.

    @param value The vector to be added.
  */
  Vector& add(const Vector& value) throw(IncompatibleVectors);

  /**
    Subtracts the specified vector from this vector.

    @param vector The vector to be subtracted.
  */
  Vector& subtract(const Vector& value) throw(IncompatibleVectors);

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  Vector& multiply(const TYPE& value) throw();

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  Vector& divide(const TYPE& value) throw();

  /**
    Negates the specified vector and stores the result in this vector.
  */
  Vector& negate(const Vector& value) throw();

  /**
    Returns the dot product of this vector with itself.
  */
  TYPE dotdot() const throw();

  /**
    Returns the L2-norm of the vector.
  */
  TYPE norm() const throw();



  // Operator section



  /**
    Returns true if the vectors are equal.

    @param vector Vector to be compared.
  */
  bool operator==(const Vector& vector) const throw(IncompatibleVectors);

  /**
    Adds the specified vector from this vector.

    @param value The value to be added.
  */
  inline Vector& operator+=(const Vector& value) throw(IncompatibleVectors) {
    return add(value);
  }

  /**
    Subtracts the specified vector from this vector.

    @param value The value to be subtracted.
  */
  inline Vector& operator-=(const Vector& value) throw(IncompatibleVectors) {
    return subtract(value);
  }

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  inline Vector& operator*=(const TYPE& value) throw() {
    return multiply(value);
  }

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  inline Vector& operator/=(const TYPE& value) throw() {
    return divide(value);
  }

  /**
    Unary plus.
  */
  inline Vector operator+() const throw() {
    return plus();
  }

  /**
    Unary minus.
  */
  inline Vector operator-() const throw() {
    return minus();
  }



  // Friend section



#if 0
/**
    Returns the product of the vector and the value.
  */
  friend Vector operator* <>(const Vector& left, const TYPE& right) throw(MemoryException);

  /**
    Returns the product of the vector and the value.
  */
  friend Vector operator* <>(const TYPE& left, const Vector& right) throw(MemoryException);

  /**
    Returns the result of the vector divided by the value.
  */
  friend Vector operator/ <>(const Vector& left, const TYPE& right) throw(MemoryException);

  /**
    Returns the dot product of the two vectors.
  */
  friend TYPE dot<>(const Vector<TYPE>& left, const Vector<TYPE>& right) throw();

  /**
    Writes a string representation of a vector object to a format stream.
  */
  friend FormatOutputStream& operator<< <>(FormatOutputStream& stream, const Vector<TYPE>& value) throw(IOException);
#endif
};

/**
  Returns the product of the vector and the value.
*/
template<class TYPE>
Vector<TYPE> operator*(const Vector<TYPE>& left, const TYPE& right) throw(MemoryException);

/**
  Returns the product of the vector and the value.
*/
template<class TYPE>
Vector<TYPE> operator*(const TYPE& left, const Vector<TYPE>& right) throw(MemoryException);

/**
  Returns the result of the vector divided by the value.
*/
template<class TYPE>
Vector<TYPE> operator/(const Vector<TYPE>& left, const TYPE& right) throw(MemoryException);

/**
  Returns the dot product of the two vectors.
*/
template<class TYPE>
TYPE dot(const Vector<TYPE>& left, const Vector<TYPE>& right) throw();

/**
  Writes a string representation of a vector object to a format stream.

  @relates Vector
*/
template<class TYPE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Vector<TYPE>& value) throw(IOException);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
