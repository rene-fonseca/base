/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_VECTOR_H
#define _BASE_VECTOR_H

#include "base/Object.h"
#include "base/Exception.h"
#include "base/OutOfRange.h"

/**
  Vector class.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE> class Vector : public Object {
public:

  typedef TYPE* Iterator;
  typedef const TYPE* ReadOnlyIterator;
protected:

  /** The elements of the vector. */
  TYPE* elements;
  /** The number of elements in the vector. */
  unsigned int length;
public:

  /** Exception thrown by the Vector class. */
  class VectorException : public Exception {};
  /** Thrown if an operation is given incompatible vectors to work on. */
  class IncompatibleVectors : public VectorException {};

  /**
    Initializes the vector. The elements are not set to zero.

    @param length The number of elements in the vector.
  */
  Vector(unsigned int length) throw();

  /**
    Initialize object.

    @param elements The desired elements of the vector.
    @param length The number of elements in the vector.
  */
  Vector(const TYPE elements[], unsigned int length) throw();

  /**
    Initialize object.

    @param vector The vector to be copied.
  */
  Vector(const Vector& vector) throw();

  /**
    Returns the elements of the vector.
  */
  TYPE* getElements() const throw();

  /**
    Returns the number of elements in the vector.
  */
  unsigned int getLength() const throw();

  /**
    Returns the element at the specified index.

    @param index The index of the desired element.
  */
  TYPE& operator[](unsigned int index) const throw(OutOfRange);

  /**
    Returns the element at the specified index.

    @param index The index of the desired element.
  */
  TYPE& getAt(unsigned int index) const throw(OutOfRange);

  /**
    Sets the element at the specified index.

    @param index The index of the desired element.
    @param value The desired value.
  */
  void setAt(unsigned int index, const TYPE& value) throw(OutOfRange);

  /**
    Initializes the elements of the vector using another vector.

    @param vector The vector containing the desired elements.
  */
  Vector& operator=(const Vector& vector) throw(IncompatibleVectors);

  /**
    Initializes the elements of the vector.

    @param value The desired value of the elements.
  */
  Vector& operator=(const TYPE& value) throw();

  /**
    Adds the specified vector.

    @param vector The vector to be added.
  */
  Vector& add(const Vector& vector) throw(IncompatibleVectors);

  /**
    Adds the specified vector.

    @param vector The vector to be added.
  */
  Vector& operator+=(const Vector& vector) throw(IncompatibleVectors);

  /**
    Adds the specified value to the elements of the vector.

    @param value The value to be added.
  */
  Vector& operator+=(const TYPE& value) throw();

  /**
    Subtracts the specified vector.

    @param vector The vector to be subtracted.
  */
  Vector& subtract(const Vector& vector) throw(IncompatibleVectors);

  /**
    Subtracts the specified vector.

    @param vector The vector to be subtracted.

  */
  Vector& operator-=(const Vector& vector) throw(IncompatibleVectors);

  /**
    Subtracts the specified value to the elements of the vector.

    @param value The value to be subtracted.
  */
  Vector& operator-=(const TYPE& value) throw();

  /**
    Multiplies the specified value to the elements of the vector.

    @param value The value to be multiplied.
  */
  Vector& operator*=(const TYPE& value) throw();

  /**
    Returns the product.

    @param value The value to be multiplied.
  */
  Vector& operator*(const TYPE& value) throw();

  /**
    Returns true if the vectors are equal.

    @param vector Vector to be compared.
  */
  bool operator==(const Vector& vector) const throw(IncompatibleVectors);

  /**
    Returns the dot product of vectors.

    @param vector Vector to be used in the calculation.
  */
  TYPE& dot(const Vector& vector) const throw(IncompatibleVectors);

  /**
    Returns the norm of the vector.
  */
  TYPE& norm() const throw();

  /**
    Destroys the vector.
  */
  ~Vector() throw();
};

typedef Vector<float> VectorOfFloat;
typedef Vector<double> VectorOfDouble;

#endif
