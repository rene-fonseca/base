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

class _COM_AZURE_DEV__BASE__API LargeInteger {
private:

  /** The value. */
  Allocator<unsigned int> value;
public:

  LargeInteger();
  
  LargeInteger(const LargeInteger& copy);
  
  LargeInteger(LargeInteger&& move);

  LargeInteger& operator=(const LargeInteger& assign);

  LargeInteger& operator=(LargeInteger&& assign);

  inline MemorySize getSize() const noexcept
  {
    return value.getSize();
  }

  void setSize(MemorySize size);

  void extend(MemorySize size);

  void trim();

  bool isZero() const noexcept;
  
  /**
    Unary plus.
  */
  LargeInteger plus() const noexcept;
  
  /**
    Unary minus.
  */
  LargeInteger minus() const noexcept;
  
  /**
    Negates this vector.
  */
  LargeInteger& negate() noexcept;
  
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

    @param value LargeInteger to be compared.
  */
  bool operator==(const LargeInteger& value) const noexcept;
  
  inline bool operator!=(const LargeInteger& value) const noexcept
  {
    return !operator==(value);
  }
  
  bool operator<(const LargeInteger& value) const noexcept;
  
  bool operator<=(const LargeInteger& value) const noexcept;
  
  inline bool operator>(const LargeInteger& value) const noexcept
  {
    return !operator<=(value);
  }
  
  inline bool operator>=(const LargeInteger& value) const noexcept
  {
    return !operator>(value);
  }

  LargeInteger& operator&(const LargeInteger& value) noexcept;
  
  LargeInteger& operator|(const LargeInteger& value) noexcept;
  
  LargeInteger& operator~() noexcept;
  
  LargeInteger& operator<<=(unsigned int value) noexcept;
  
  LargeInteger& operator>>=(unsigned int value) noexcept;
  
  /**
    Adds the specified vector from this vector.

    @param value The value to be added.
  */
  inline LargeInteger& operator+=(const LargeInteger& value) noexcept
  {
    return add(value);
  }

  /**
    Subtracts the specified vector from this vector.

    @param value The value to be subtracted.
  */
  inline LargeInteger& operator-=(const LargeInteger& value) noexcept
  {
    return subtract(value);
  }

  /**
    Multiplies this vector with the specified value.

    @param value The multiplicator.
  */
  inline LargeInteger& operator*=(int value) noexcept
  {
    return multiply(value);
  }

  /**
    Divides this vector with the specified value.

    @param value The divisor.
  */
  inline LargeInteger& operator/=(int value) noexcept
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

  inline bool operator!() const noexcept
  {
    return isZero();
  }

  inline operator bool() const noexcept
  {
    return !isZero();
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
