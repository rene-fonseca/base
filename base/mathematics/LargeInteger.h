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

#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @short Signed large integer.
  @ingroup mathematics
*/

class LargeInteger {
private:

  /** The value. */
  unsigned int* value = nullptr;
  /** The size of the integer. */
  unsigned int size = 0;
public:

  inline LargeInteger() {
  }
  
  inline LargeInteger(const LargeInteger& copy) noexcept
    : value(copy.value), size(copy.size)
  {
  }
  
  inline LargeInteger& operator=(const LargeInteger& value)
  {
    if (this != &value) {
      // ...
    }
    return *this;
  }
  
  /**
    Unary plus.
  */
  LargeInteger plus() const;
  
  /**
    Unary minus.
  */
  LargeInteger minus() const;
  
  /**
    Negates this vector.
  */
  LargeInteger& negate();
  
  /**
    Adds the specified vector to this vector.

    @param value The vector to be added.
  */
  LargeInteger& add(const LargeInteger& value);
  
  /**
    Subtracts the specified vector from this vector.

    @param vector The vector to be subtracted.
  */
  LargeInteger& subtract(const LargeInteger& value);
  
  /**
    Multiplies this vector with the specified value.
    
    @param value The multiplicator.
  */
  LargeInteger& multiply(int value);

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  LargeInteger& divide(int value);

  /**
    Negates the specified vector and stores the result in this vector.
  */
  LargeInteger& negate(const LargeInteger& value);



  // Operator section



  /**
    Returns true if the vectors are equal.

    @param vector LargeInteger to be compared.
  */
  bool operator==(const LargeInteger& vector) const;
  
  bool operator!=(const LargeInteger& vector) const;
  
  bool operator<(const LargeInteger& vector) const;
  
  bool operator<=(const LargeInteger& vector) const;
  
  bool operator>(const LargeInteger& vector) const;
  
  bool operator>=(const LargeInteger& vector) const;

  LargeInteger& operator&(const LargeInteger& value);
  LargeInteger& operator|(const LargeInteger& value);
  LargeInteger& operator~();
  
  LargeInteger& operator<<=(unsigned int value);
  LargeInteger& operator>>=(unsigned int value);
  
  /**
    Adds the specified vector from this vector.

    @param value The value to be added.
  */
  inline LargeInteger& operator+=(const LargeInteger& value)
  {
    return add(value);
  }

  /**
    Subtracts the specified vector from this vector.

    @param value The value to be subtracted.
  */
  inline LargeInteger& operator-=(const LargeInteger& value)
  {
    return subtract(value);
  }

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  inline LargeInteger& operator*=(int value)
  {
    return multiply(value);
  }

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  inline LargeInteger& operator/=(int value)
  {
    return divide(value);
  }

  /**
    Unary plus.
  */
  inline LargeInteger operator+() const
  {
    return plus();
  }

  /**
    Unary minus.
  */
  inline LargeInteger operator-() const
  {
    return minus();
  }
  
  
  
  /**
    Returns the product of the vector and the value.
  */
  friend LargeInteger operator*(const LargeInteger& left, int right);

  /**
    Returns the product of the vector and the value.
  */
  friend LargeInteger operator*(int left, const LargeInteger& right);

  /**
    Returns the result of the vector divided by the value.
  */
  friend LargeInteger operator/(const LargeInteger& left, int right);
  
  /**
    Writes a string representation of a vector object to a format stream.
  */
  _COM_AZURE_DEV__BASE__API friend FormatOutputStream& operator<<(
    FormatOutputStream& stream, const LargeInteger& value) throw(IOException);
};

/**
  Returns the product of the vector and the value.
*/
LargeInteger operator*(const LargeInteger& left, int right);

/**
  Returns the product of the vector and the value.
*/
LargeInteger operator*(int left, const LargeInteger& right);

/**
  Returns the result of the vector divided by the value.
*/
LargeInteger operator/(const LargeInteger& left, int right);

/**
  Writes a string representation of a LargeInteger object to a format stream.
  
  @relates LargeInteger
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(
  FormatOutputStream& stream, const LargeInteger& value) throw(IOException);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
