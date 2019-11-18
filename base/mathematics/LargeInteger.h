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

// ATTENTION: not fully implemented and tested

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Low-level large integer. */
class LargeIntegerImpl {
public:

  typedef uint32 Word;
  typedef uint64 DoubleWord;
  static constexpr unsigned int WORD_SIZE = sizeof(Word);
  static constexpr unsigned int WORD_BITS = WORD_SIZE * 8;
  static constexpr Word ZERO = static_cast<Word>(0);
  static constexpr Word ONE = static_cast<Word>(1);
  static constexpr Word MAXIMUM = static_cast<Word>(0) - 1;

  /** Sets words to 0. */
  static void clear(Word* value, MemorySize size) noexcept;

  /** Assigns words. */
  static void assign(Word* restrict dest, const Word* restrict src, MemorySize size) noexcept;

  /** Assigns bit. */
  static void assignBit(Word* value, MemorySize size, MemorySize bitIndex) noexcept;

  /** Returns bit field. fieldSize must be WORD_SIZE or less. */
  static Word getBits(const Word* words, MemorySize size, MemorySize bitIndex, unsigned int fieldSize) noexcept;

  /** Returns true if the bit is set. */
  static bool isBitSet(const Word* value, MemorySize size, MemorySize bitIndex) noexcept;

  /** Sets the bit. */
  static void setBit(Word* value, MemorySize size, MemorySize bitIndex, bool bitValue) noexcept;

  /** Add the bit. */
  static bool addBit(Word* value, MemorySize size, MemorySize bitIndex) noexcept;

  /** Left shift. */
  static void leftShift(Word* value, MemorySize size, unsigned int shift) noexcept;

  /** Right shift. */
  static void rightShift(Word* value, MemorySize size, unsigned int shift) noexcept;

  /** Add single word. */
  static bool add(Word* value, MemorySize size, Word addend) noexcept;

  /** Add words. */
  static bool add(Word* restrict value, const Word* restrict right, MemorySize size) noexcept;

  /** Returns true if addition causes carrier. */
  static bool checkAdditionOverflow(const Word* restrict left, const Word* restrict right, MemorySize size) noexcept;

  /** Subtract single word. */
  static bool subtract(Word* restrict value, MemorySize size, Word subtrahend) noexcept;

  /** Subtract words. */
  static bool subtract(Word* restrict value, const Word* restrict right, MemorySize size) noexcept;

  /** Multiply word. */
  static bool multiply(Word* value, MemorySize size, Word multiplicand) noexcept;

  static MemorySize getSize(const Word* value, MemorySize size) noexcept;

  static MemorySize getBitSize(const Word* value, MemorySize size) noexcept;

  /** Returns true if zero. */
  static bool isZero(const Word* value, MemorySize size) noexcept;

  /** Returns true if one. */
  static bool isOne(const Word* value, MemorySize size) noexcept;

  /** Returns true if less than. */
  static bool lessThan(const Word* restrict left, MemorySize size, Word comparand) noexcept;

  /** Returns true if less than. */
  static bool lessThan(const Word* restrict left, const Word* restrict right, MemorySize size) noexcept;

  /** Returns true if less than or equal. */
  static bool lessThanEqual(const Word* restrict left, MemorySize size, Word comparand) noexcept;

  /** Returns true if less than or equal. */
  static bool lessThanEqual(const Word* restrict left, const Word* restrict right, MemorySize size) noexcept;

  /** Returns true if equal. */
  static bool equal(const Word* restrict left, MemorySize size, Word comparand) noexcept;

  /** Returns true if equal. */
  static bool equal(const Word* restrict left, const Word* restrict right, MemorySize size) noexcept;

  /** Divides by single word. */
  static Word divide(Word* dividend, MemorySize size, Word divisor) noexcept;

  /** Remainder by single word. Doesnt modify dividend. */
  static Word remainder(const Word* dividend, MemorySize size, Word divisor) noexcept;

  // may remainder be the same as dividend - I think so
  static void divide(
    Word* restrict quotient,
    Word* remainder,
    const Word* dividend,
    const Word* restrict divisor,
    MemorySize size) noexcept;
}; // LargeIntegerImpl



/**
  Large integer.
*/

class _COM_AZURE_DEV__BASE__API LargeInteger {
private:

  typedef LargeIntegerImpl::Word Word;

  /** The value. */
  Allocator<Word> value;

  inline Word* toWords() noexcept
  {
    return value.getElements();
  }

  inline const Word* toWords() const noexcept
  {
    return value.getElements();
  }
public:

  LargeInteger();
  
  LargeInteger(const LargeInteger& copy);

  LargeInteger(const LargeInteger& copy, MemorySize capacity);

  LargeInteger(LargeInteger&& move);

  LargeInteger& operator=(const LargeInteger& assign);

  LargeInteger& operator=(LargeInteger&& assign);

  LargeInteger& operator=(Word assign);

  /** Returns the word size. */
  inline MemorySize getSize() const noexcept
  {
    return value.getSize();
  }

  /** Sets the word size of the integer. Adds zeros when extended. */
  void setSize(MemorySize size);

  /** Sets the word capacity. */
  void ensureCapacity(MemorySize size);

  /** Unsigned extend. */
  void extend(MemorySize size);

  /** Signed extend. */
  void signExtend(MemorySize size);

  /** Removes high zero words. */
  void trim();

  /** Returns true if the integer is zero. */
  bool isZero() const noexcept;

  Word getBits(unsigned int bitIndex, unsigned bitSize) const noexcept;

  /** Returns true if the bit is set. */
  bool isBitSet(unsigned int bit) const noexcept;
  
  /** Sets the bit. */
  void setBit(unsigned int bit, bool b) noexcept;

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
  LargeInteger& multiply(unsigned int value);

  /**
    Divides this integer with the specified divisor.

    @param divisor The divisor.

    @return Returns the remainder.
  */
  unsigned int divide(const unsigned int divisor);

  /**
    Returns the remainder without modification.

    @param value The divisor.
  */
  unsigned int remainder(unsigned int divisor) const noexcept;

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

  bool operator==(const Word value) const noexcept;

  inline bool operator!=(const LargeInteger& value) const noexcept
  {
    return !operator==(value);
  }

  inline bool operator!=(const Word value) const noexcept
  {
    return !operator==(value);
  }

  bool operator<(const LargeInteger& value) const noexcept;

  bool operator<(const Word value) const noexcept;

  bool operator<=(const LargeInteger& value) const noexcept;

  bool operator<=(const Word value) const noexcept;

  inline bool operator>(const LargeInteger& value) const noexcept
  {
    return !operator<=(value);
  }

  inline bool operator>(const Word value) const noexcept
  {
    return !operator<=(value);
  }

  inline bool operator>=(const LargeInteger& value) const noexcept
  {
    return !operator>(value);
  }

  inline bool operator>=(const Word value) const noexcept
  {
    return !operator>(value);
  }

  LargeInteger& operator&(const LargeInteger& value) noexcept;
  
  LargeInteger& operator|(const LargeInteger& value) noexcept;

  LargeInteger& operator^(const LargeInteger& operand) noexcept;

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
  inline LargeInteger& operator*=(unsigned int multiplicand) noexcept
  {
    return multiply(multiplicand);
  }

  /**
    Divides this vector with the specified value.

    @param divisor The divisor.
  */
  inline LargeInteger& operator/=(const unsigned int divisor) noexcept
  {
    divide(divisor);
    return *this;
  }

  /**
    Divides this vector with the specified value.

    @param divisor The divisor.
  */
  inline unsigned int operator%=(const unsigned int divisor) noexcept
  {
    auto result = remainder(divisor);
    *this = result;
    return result;
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

  inline unsigned int operator&(const unsigned int word) const noexcept
  {
    if (getSize() == 0) {
      return 0;
    }
    return toWords()[0] & word;
  }

  /** Returns the integer as a word. Must be MAXIMUM at most. */
  inline Word getAsWord() const noexcept
  {
    if (getSize() == 0) {
      return 0;
    }
    BASSERT(*this <= LargeIntegerImpl::MAXIMUM);
    return toWords()[0];
  }

  /**
    Writes a string representation of a vector object to a format stream.
  */
  _COM_AZURE_DEV__BASE__API friend FormatOutputStream& operator<<(
    FormatOutputStream& stream, const LargeInteger& value) throw(IOException);
};

LargeInteger operator+(const LargeInteger& left, const LargeInteger& right);

LargeInteger operator-(const LargeInteger& left, const LargeInteger& right);

LargeInteger operator*(const LargeInteger& left, const LargeInteger& right);

LargeInteger operator/(const LargeInteger& left, const LargeInteger& right);

LargeInteger operator%(const LargeInteger& left, const LargeInteger& right);

LargeInteger operator%(const LargeInteger& left, unsigned int right);

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
