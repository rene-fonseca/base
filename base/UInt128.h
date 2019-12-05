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
  128 bit unsigned integer.
*/

class _COM_AZURE_DEV__BASE__API UInt128 : public Object {
public:

  static const UInt128 ZERO;
  static const UInt128 ONE;
  static const UInt128 MAXIMUM;

  // TAG: use __uint128_t if available

  uint128 value;

  static constexpr unsigned int BITS = 128;
  static constexpr unsigned int WORD_BITS = 64;
  typedef uint64 WORD;
  typedef int64 SIGNED_WORD; // TAG: get type from template instead
  static constexpr WORD WORD_MAXIMUM = static_cast<WORD>(-1);

  inline UInt128() noexcept
  {
    value.low = 0;
    value.high = 0;
  }

  inline UInt128(uint128 _value) noexcept
  {
    value = _value;
  }

  inline UInt128(int128 _value) noexcept
  {
    value.low = _value.low;
    value.high = _value.high;
  }

  inline UInt128(uint64 low) noexcept
  {
    value.low = low;
    value.high = 0;
  }

  inline UInt128(uint64 low, uint64 high) noexcept
  {
    value.low = low;
    value.high = high;
  }

  inline UInt128(int64 low) noexcept
  {
    if (low < 0) {
      value.low = low;
      value.high = static_cast<WORD>(-1);
    } else {
      value.low = low;
      value.high = 0;
    }
  }

  inline UInt128(uint32 low) noexcept
  {
    value.low = low;
    value.high = 0;
  }

  inline UInt128(int32 low) noexcept
  {
    if (low < 0) {
      value.low = low;
      value.high = static_cast<WORD>(-1);
    } else {
      value.low = low;
      value.high = 0;
    }
  }

  inline UInt128(uint16 low) noexcept
  {
    value.low = low;
    value.high = 0;
  }

  inline UInt128(int16 low) noexcept
  {
    if (low < 0) {
      value.low = low;
      value.high = static_cast<WORD>(-1);
    } else {
      value.low = low;
      value.high = 0;
    }
  }

  inline UInt128(uint8 low) noexcept
  {
    value.low = low;
    value.high = 0;
  }

  inline UInt128(int8 low) noexcept
  {
    if (low < 0) {
      value.low = low;
      value.high = static_cast<WORD>(-1);
    } else {
      value.low = low;
      value.high = 0;
    }
  }

  inline UInt128(const UInt128& copy) noexcept
    : value(copy.value)
  {
  }

  inline UInt128& operator=(const UInt128& assign) noexcept
  {
    value = assign.value;
    return *this;
  }

  inline UInt128& operator~() noexcept
  {
    value.low = ~value.low;
    value.high = ~value.high;
    return *this;
  }

  inline bool operator==(const UInt128& compare) const noexcept
  {
    return (value.low == compare.value.low) && (value.high == compare.value.high);
  }

  inline bool operator!=(const UInt128& compare) const noexcept
  {
    return (value.low != compare.value.low) || (value.high != compare.value.high);
  }

  inline bool operator<(const UInt128& compare) const noexcept
  {
    if (value.high < compare.value.high) {
      return true;
    } else if (value.high == compare.value.high) {
      if (value.low < compare.value.low) {
        return true;
      }
    }
    return false;
  }

  inline bool operator<=(const UInt128& compare) const noexcept
  {
    if (value.high < compare.value.high) {
      return true;
    } else if (value.high == compare.value.high) {
      if (value.low <= compare.value.low) {
        return true;
      }
    }
    return false;
  }

  inline bool operator>(const UInt128& compare) const noexcept
  {
    return !operator<=(compare);
  }

  inline bool operator>=(const UInt128 compare) const noexcept
  {
    return !operator<(compare);
  }

  inline operator uint128() const noexcept
  {
    return value;
  }

  inline operator uint64() const noexcept
  {
    return value.low;
  }

  inline operator uint32() const noexcept
  {
    return static_cast<uint32>(value.low & 0xffffffff);
  }

  inline operator uint16() const noexcept
  {
    return static_cast<uint16>(value.low & 0xffff);
  }

  inline operator uint8() const noexcept
  {
    return static_cast<uint8>(value.low & 0xff);
  }

  void operator>>=(unsigned int shift) noexcept
  {
    BASSERT(shift <= BITS);
    if (shift >= BITS) {
      value.low = 0;
      value.high = 0;
      return;
    }
    if (shift >= WORD_BITS) {
      value.low = value.high >> (shift - WORD_BITS);
      value.high = 0;
      return;
    }
    value.low >>= shift;
    value.low |= value.high << (WORD_BITS - shift);
    value.high >>= shift;
  }

  void operator<<=(unsigned int shift) noexcept
  {
    BASSERT(shift <= BITS);
    if (shift >= BITS) {
      value.low = 0;
      value.high = 0;
      return;
    }
    if (shift >= WORD_BITS) {
      value.high = value.low >> (shift - WORD_BITS);
      value.low = 0;
      return;
    }
    value.high <<= shift;
    value.high |= value.low >> (WORD_BITS - shift);
    value.low <<= shift;
  }

  inline UInt128& operator^=(const UInt128 a) noexcept
  {
    value.low ^= a.value.low;
    value.high ^= a.value.high;
    return *this;
  }

  inline UInt128& operator&=(const UInt128 a) noexcept
  {
    value.low &= a.value.low;
    value.high &= a.value.high;
    return *this;
  }

  inline UInt128& operator|=(const UInt128 a) noexcept
  {
    value.low |= a.value.low;
    value.high |= a.value.high;
    return *this;
  }

  inline UInt128& operator++() noexcept
  {
    if (value.low == WORD_MAXIMUM) { // carry
      ++value.high;
    }
    ++value.low;
    return *this;
  }

  inline UInt128 operator++(int) const noexcept
  {
    UInt128 result = *this;
    ++result;
    return result;
  }

  inline UInt128& operator--() noexcept
  {
    if (value.low == 0) { // borrow
      --value.high;
    }
    --value.low;
    return *this;
  }

  inline UInt128 operator-() const noexcept
  {
    const bool borrow = 0 < value.low;
    WORD high = -static_cast<SIGNED_WORD>(value.high);
    WORD low = -static_cast<SIGNED_WORD>(value.low);
    if (borrow) {
      high -= 1;
    }
    return UInt128(low, high);
  }

  inline UInt128 operator--(int) const noexcept
  {
    UInt128 result = *this;
    --result;
    return result;
  }

  inline UInt128& operator-=(const UInt128& _a) noexcept
  {
    const uint128& a = _a.value;
    const bool borrow = value.low < a.low;
    value.high -= a.high;
    value.low -= a.low;
    if (borrow) {
      value.high -= 1;
    }
    return *this;
  }

  inline UInt128& operator+=(const UInt128& _a) noexcept
  {
    // C++ extension: alias _a.value a; or using _a.value a;
    const auto& a = _a.value;
    const bool carry = a.low < a.low; // TAG: FIXME
    value.high += a.high;
    value.low += a.low;
    if (carry) {
      value.high += 1;
    }
    return *this;
  }

  inline UInt128& operator*=(const UInt128& _a) noexcept
  {
    const auto& a = _a.value;
    // (a_high + a_low) * (b_high + b_low) <=>
    // (a_high * b_high + a_low * b_high) + (a_high * b_low + a_low * b_low)

    if (a.high == 0) { // common case
      // (a_low * b_high) + (a_low * b_low)
      return *this;
    }

    value.low *= a.low;
    value.high *= a.high;
    // TAG: FIXME
    return *this;
  }

  inline UInt128& operator/=(const UInt128& a) noexcept
  {
    // TAG: FIXME
    return *this;
  }

  inline UInt128 operator!() const noexcept
  {
    return (value.low == 0) && (value.high == 0);
  }

  inline operator bool() const noexcept
  {
    return value.low || value.high;
  }

  inline bool isBitSet(unsigned int bit) const noexcept
  {
    BASSERT(bit < BITS);
    bit &= (BITS - 1);
    if (bit >= WORD_BITS) {
      return (value.high >> (bit - WORD_BITS)) & 0x1;
    }
    return (value.low >> bit) & 0x1;
  }
  
  inline bool isLowBitSet() const noexcept
  {
    return value.low & 0x1; // return isBitSet(0);
  }

  inline bool isHighBitSet() const noexcept
  {
    return (value.high >> (WORD_BITS - 1)) & 0x1; // return isBitSet(BITS - 1);
  }
};

inline UInt128 operator^(const UInt128& a, const UInt128& b) noexcept
{
  return UInt128(a.value.low ^ b.value.low, a.value.high ^ b.value.high);
}

inline UInt128 operator&(const UInt128& a, const UInt128& b) noexcept
{
  return UInt128(a.value.low & b.value.low, a.value.high & b.value.high);
}

inline UInt128 operator|(const UInt128& a, const UInt128& b) noexcept
{
  return UInt128(a.value.low | b.value.low, a.value.high | b.value.high);
}

inline UInt128 operator+(const UInt128& a, const UInt128& b) noexcept
{
  UInt128 result = a;
  result += b;
  return result;
}

inline UInt128 operator-(const UInt128& a, const UInt128& b) noexcept
{
  UInt128 result = a;
  result -= b;
  return result;
}

inline UInt128 operator*(const UInt128& a, const UInt128& b) noexcept
{
  UInt128 result = a;
  result *= b;
  return result;
}

inline UInt128 operator/(const UInt128& a, const UInt128& b) noexcept
{
  UInt128 result = a;
  result /= b;
  return result;
}

inline UInt128 operator%(const UInt128& a, const UInt128& b) noexcept
{
  // TAG: optimize
  UInt128 temp = a;
  temp /= b;
  return a - temp * b;
}

inline FormatOutputStream& operator<<(FormatOutputStream& stream, const UInt128& value)
{
  return stream << value.value;
}

#if 0
template<typename WORD>
class BigUnsignedInteger;
typedef BigUnsignedInteger<UInt128> UInt256;
typedef BigUnsignedInteger<UInt256> UInt512;
typedef BigUnsignedInteger<UInt512> UInt1024;
typedef BigUnsignedInteger<UInt1024> UInt2048;
typedef BigUnsignedInteger<UInt2048> UInt4096;
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
