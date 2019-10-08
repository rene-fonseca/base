/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/MemoryException.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Signed large integer.
  @ingroup mathematics
  @version 1.0
*/

class LargeInteger {
private:

  /** The value. */
  unsigned int* value = nullptr;
  /** The size of the integer. */
  unsigned int size = 0;
public:

  inline LargeInteger() throw() {
  }
  
  inline LargeInteger(const LargeInteger& copy) throw()
    : value(copy.value), size(copy.size) {
  }
  
  inline LargeInteger& operator=(const LargeInteger& value) throw() {
    if (this != &value) {
      // ...
    }
    return *this;
  }
  
  /**
    Unary plus.
  */
  LargeInteger plus() const throw();
  
  /**
    Unary minus.
  */
  LargeInteger minus() const throw();
  
  /**
    Negates this vector.
  */
  LargeInteger& negate() throw();
  
  /**
    Adds the specified vector to this vector.

    @param value The vector to be added.
  */
  LargeInteger& add(const LargeInteger& value) throw();
  
  /**
    Subtracts the specified vector from this vector.

    @param vector The vector to be subtracted.
  */
  LargeInteger& subtract(const LargeInteger& value) throw();
  
  /**
    Multiplies this vector with the specified value.
    
    @param value The multiplicator.
  */
  LargeInteger& multiply(int value) throw();

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  LargeInteger& divide(int value) throw();

  /**
    Negates the specified vector and stores the result in this vector.
  */
  LargeInteger& negate(const LargeInteger& value) throw();



  // Operator section



  /**
    Returns true if the vectors are equal.

    @param vector LargeInteger to be compared.
  */
  bool operator==(const LargeInteger& vector) const throw();
  
  bool operator!=(const LargeInteger& vector) const throw();
  
  bool operator<(const LargeInteger& vector) const throw();
  
  bool operator<=(const LargeInteger& vector) const throw();
  
  bool operator>(const LargeInteger& vector) const throw();
  
  bool operator>=(const LargeInteger& vector) const throw();

  LargeInteger& operator&(const LargeInteger& value) throw();
  LargeInteger& operator|(const LargeInteger& value) throw();
  LargeInteger& operator~() throw();
  
  LargeInteger& operator<<=(unsigned int value) throw();
  LargeInteger& operator>>=(unsigned int value) throw();
  
  /**
    Adds the specified vector from this vector.

    @param value The value to be added.
  */
  inline LargeInteger& operator+=(const LargeInteger& value) throw() {
    return add(value);
  }

  /**
    Subtracts the specified vector from this vector.

    @param value The value to be subtracted.
  */
  inline LargeInteger& operator-=(const LargeInteger& value) throw() {
    return subtract(value);
  }

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  inline LargeInteger& operator*=(int value) throw() {
    return multiply(value);
  }

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  inline LargeInteger& operator/=(int value) throw() {
    return divide(value);
  }

  /**
    Unary plus.
  */
  inline LargeInteger operator+() const throw() {
    return plus();
  }

  /**
    Unary minus.
  */
  inline LargeInteger operator-() const throw() {
    return minus();
  }
  
  
  
  /**
    Returns the product of the vector and the value.
  */
  friend LargeInteger operator*(
    const LargeInteger& left, int right) throw(MemoryException);

  /**
    Returns the product of the vector and the value.
  */
  friend LargeInteger operator*(
    int left, const LargeInteger& right) throw(MemoryException);

  /**
    Returns the result of the vector divided by the value.
  */
  friend LargeInteger operator/(
    const LargeInteger& left, int right) throw(MemoryException);
  
  /**
    Writes a string representation of a vector object to a format stream.
  */
  friend FormatOutputStream& operator<<(
    FormatOutputStream& stream, const LargeInteger& value) throw(IOException);
};

/**
  Returns the product of the vector and the value.
*/
LargeInteger operator*(
  const LargeInteger& left, int right) throw(MemoryException);

/**
  Returns the product of the vector and the value.
*/
LargeInteger operator*(
  int left, const LargeInteger& right) throw(MemoryException);

/**
  Returns the result of the vector divided by the value.
*/
LargeInteger operator/(
  const LargeInteger& left, int right) throw(MemoryException);

/**
  Writes a string representation of a LargeInteger object to a format stream.
  
  @relates LargeInteger
*/
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const LargeInteger& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
