/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATH__VECTOR_H
#define _DK_SDU_MIP__BASE_MATH__VECTOR_H

#include <base/Object.h>
#include <base/OutOfDomain.h>
#include <base/OutOfRange.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/mem/AllocatorEnumeration.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Vector implementation.

  @author René Møller Fonseca
  @version 1.10
*/

template<class TYPE> class Vector : public Object {
public:

  /**
    Reference to an element within a vector.
  */
  class Reference {
  private:
    friend class Vector;
    Vector& vector; // use reference to avoid 'copy on write'
    unsigned int index;
    Reference(const Reference& copy); // prohibit default copy initialization
    Reference& operator=(const Reference& eq); // prohibit default assignment
    Reference(Vector& v, unsigned int i) throw() : vector(v), index(i) {}
  public:
    inline Reference& operator=(const TYPE& value) throw(OutOfRange) {vector.setAt(index, value); return *this;}
    inline operator TYPE() const throw(OutOfRange) {return vector.getAt(index);}
  };

  /**
    Enumeration of all the elements of a vector.
  */
  class Enumeration : public AllocatorEnumeration<TYPE, TYPE&, TYPE*> {
  public:

    /**
      Initializes an enumeration of all the elements of the specified vector.

      @param vector The vector being enumerated.
    */
    Enumeration(Vector& vector) throw() :
      AllocatorEnumeration<TYPE, TYPE&, TYPE*>(vector.getElements(), vector.getElements() + vector.getSize()) {}
  };

  /**
    Non-modifying enumeration of all the elements of a vector.
  */
  class ReadOnlyEnumeration : public AllocatorEnumeration<TYPE, const TYPE&, const TYPE*> {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the specified vector.

      @param vector The vector being enumerated.
    */
    ReadOnlyEnumeration(const Vector& vector) throw() :
      AllocatorEnumeration<TYPE, const TYPE&, const TYPE*>(vector.getElements(), vector.getElements() + vector.getSize()) {}
  };
protected:

  /**
    The elements of the vector stored in an array. The array is guarantied to
    be non-empty when the vector object has been initialized.
  */
  ReferenceCountedObjectPointer<ReferenceCountedAllocator<TYPE> > elements;

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
  inline unsigned int getSize() const throw() {return elements->getSize();};
public:

  /** Exception thrown by the Vector class. */
  class VectorException : public Exception {
  };

  /** Thrown if an operation is given incompatible vectors to work on. */
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
  inline Vector(const Vector& copy) throw() : elements(copy.elements) {};

  /**
    Assigns vector to this vector.

    @param eq The vector containing the desired elements.
  */
  Vector& operator=(const Vector& eq) throw(MemoryException);



  // Elements access section



  /**
    Returns the element at the specified index.

    @param index The index of the desired element.
  */
  TYPE getAt(unsigned int index) const throw(OutOfRange);

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
  inline Reference operator[](unsigned int index) throw(OutOfRange) {
    return Reference(*this, index);
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
    Returns the norm of this vector.
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
  inline Vector& operator+=(const Vector& value) throw(IncompatibleVectors) {return add(value);}

  /**
    Subtracts the specified vector from this vector.

    @param value The value to be subtracted.
  */
  inline Vector& operator-=(const Vector& value) throw(IncompatibleVectors) {return subtract(value);};

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  inline Vector& operator*=(const TYPE& value) throw() {return multiply(value);};

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  inline Vector& operator/=(const TYPE& value) throw() {return divide(value);};

  /**
    Unary plus.
  */
  inline Vector operator+() const throw() {return plus();}

  /**
    Unary minus.
  */
  inline Vector operator-() const throw() {return minus();}



  // Friend section



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
  friend FormatOutputStream& operator<< <>(FormatOutputStream& stream, const Vector<TYPE>& value);
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
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Vector<TYPE>& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
