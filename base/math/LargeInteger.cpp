/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/math/LargeInteger.h>
#include <base/math/Math.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0
void LargeIntegerImpl::clear(Words& value) noexcept
{
  fill<Word>(value.begin, value.maximumSize, 0U);
  value.size = 0;
}

void LargeIntegerImpl::assign(Words& dest, const Words& src) noexcept
{
  if (dest.begin == src.begin) { // self assignment
    return;
  }
  BASSERT(dest.maximumSize >= src.maximumSize);
  copy(dest.begin, src.begin, src.size);
}
#endif

void LargeIntegerImpl::clear(Word* value, MemorySize size) noexcept
{
  fill<Word>(value, size, 0U);
}

void LargeIntegerImpl::assign(Word* restrict dest, const Word* restrict src, MemorySize size) noexcept
{
  if (!/*INLINE_ASSERT*/(dest != src)) {
    return; // self assignment - violates restrict
  }
  copy(dest, src, size);
}

void LargeIntegerImpl::assignBit(Word* value, const MemorySize size, const MemorySize bitIndex) noexcept
{
  BASSERT(bitIndex < (size * WORD_BITS));
  fill<Word>(value, size, 0U);
  value[bitIndex / WORD_BITS] = ONE << (bitIndex % WORD_BITS);
}

LargeIntegerImpl::Word LargeIntegerImpl::getBits(const Word* words, MemorySize size, MemorySize bitIndex, unsigned fieldSize) noexcept
{
  BASSERT(fieldSize <= WORD_BITS);
  const MemorySize index = bitIndex / WORD_BITS;
  const Word word = words[index];
  const unsigned int shift = static_cast<unsigned int>(bitIndex - index * WORD_BITS);
  Word result = (word >> shift) & ((ONE << fieldSize) - 1);
  if ((shift + fieldSize) > WORD_BITS) {
    const unsigned int nextBits = shift + fieldSize - WORD_BITS;
    const Word nextWord = ((index + 1) < size) ? words[index + 1] : ZERO; // zeropad
    result |= (nextWord & ((ONE << nextBits) - 1)) << (fieldSize - nextBits);
  }
  return result;
}

bool LargeIntegerImpl::isBitSet(const Word* value, const MemorySize size, const MemorySize bitIndex) noexcept
{
  const auto index = bitIndex / WORD_BITS;
  if (index >= size) {
    return false;
  }
  return value[index] & (ONE << (bitIndex % WORD_BITS));
}

void LargeIntegerImpl::setBit(Word* value, const MemorySize size, const MemorySize bitIndex, const bool bitValue) noexcept
{
  BASSERT(bitIndex < (size * WORD_BITS));
  const MemorySize index = bitIndex / WORD_BITS;
  if (bitValue) {
    value[index] |= ONE << (bitIndex % WORD_BITS);
  } else {
    value[index] &= ~(ONE << (bitIndex % WORD_BITS));
  }
}

bool LargeIntegerImpl::addBit(Word* value, const MemorySize size, const MemorySize bitIndex) noexcept
{
  BASSERT(bitIndex < (size * WORD_BITS));
  value += bitIndex / WORD_BITS;
  Word carrier = ONE << (bitIndex % WORD_BITS);
  for (const Word* end = value + size; value != end; ++value) {
    carrier = Math::addCarry(*value, carrier);
    if (carrier == ZERO) {
      return false;
    }
  }
  return true;
}

void LargeIntegerImpl::leftShift(Word* value, const MemorySize size, const unsigned int shift) noexcept
{
  if (shift == 0) {
    return;
  }

  const unsigned int bitShift = shift % WORD_BITS;
  const unsigned int wordShift = shift / WORD_BITS;

  if (wordShift >= size) {
    fill(value, size, 0U); // mask beginning of value
    return;
  }

  // start from last non-zero value + wordSize (but do not exceed end)
  Word* dest = value + size - 1;
  const Word* src = dest - wordShift;

  if (bitShift != 0) {
    const unsigned int invBitShift = WORD_BITS - bitShift;
    while (src > value) {
      *dest = (*src << bitShift) | (src[-1] >> invBitShift);
      --src;
      --dest;
    }
    *dest = *src << bitShift; // final (shift in zeros)
  } else {
    for (; src >= value; --src, --dest) {
      *dest = *src;
    }
  }
  fill(value, wordShift, 0U); // mask beginning of value
}

// TAG: fix this - prevent overflow
void LargeIntegerImpl::rightShift(Word* value, const MemorySize size, const unsigned int shift) noexcept
{
  if (shift == 0) {
    return;
  }

  unsigned int bitShift = shift % WORD_BITS;
  unsigned int wordShift = shift / WORD_BITS;
  Word* dest = value;
  if (wordShift >= size) {
    fill(value, size, 0U); // mask beginning of value
    return;
  }
  const Word* src = value + wordShift;

  if (bitShift != 0) {
    const unsigned int nextBitShift = WORD_BITS - bitShift; // 0 < nextBitShift < WORD_BITS
    for (const Word* end = value + (size - wordShift - 1); dest != end; ++dest) {
      Word temp = *src >> bitShift;
      ++src;
      temp |= *src << nextBitShift;
      *dest = temp;
    }
    *dest++ = *src >> bitShift;
  } else {
    for (const Word* end = value + (size - wordShift); dest != end; ++src, ++dest) {
      *dest = *src;
    }
  }
  for (const Word* end = value + size; dest != end; ++dest) { // mask end of value
    *dest = 0;
  }
}

bool LargeIntegerImpl::add(Word* value, const MemorySize size, const Word addend) noexcept
{
  Word carrier = addend;
  for (const Word* end = value + size; (value != end) && carrier; ++value) {
    carrier = Math::addCarry(*value, carrier);
  }
  return carrier > 0;
}

bool LargeIntegerImpl::add(Word* restrict value, const Word* restrict addend, const MemorySize size) noexcept
{
  Word carrier = 0;
  for (const Word* end = value + size; value != end; ++value, ++addend) {
    carrier = Math::addCarry(*value, carrier);
    carrier += Math::addCarry(*value, *addend);
  }
  return carrier > 0;
}

bool LargeIntegerImpl::checkAdditionOverflow(const Word* restrict value, const Word* restrict addend, const MemorySize size) noexcept
{
  Word carrier = 0;
  for (const Word* end = value + size; value != end; ++value, ++addend) {
    auto temp = *value;
    carrier = Math::addCarry(temp, carrier);
    carrier += Math::addCarry(temp, *addend);
  }
  return carrier > 0;
}

void LargeIntegerImpl::negate(Word* restrict value, const MemorySize size) noexcept
{
  Word borrower = 0;
  for (const Word* end = value + size; (value != end) /*&& borrower*/; ++value) {
    *value = 0 - *value;
    *value -= borrower;
    borrower = 1;
  }
}

bool LargeIntegerImpl::subtract(Word* restrict value, const MemorySize size, const Word subtrahend) noexcept
{
  Word borrower = subtrahend;
  for (const Word* end = value + size; (value != end) && borrower; ++value) {
    borrower = Math::subtractBorrow(*value, borrower);
  }
  return borrower > 0; // if size == 0 then borrower is subtrahend - not 1 or 0
}

bool LargeIntegerImpl::subtract(Word* restrict value, const Word* restrict subtrahend, const MemorySize size) noexcept
{
  Word borrower = 0;
  for (const Word* end = value + size; value != end; ++value, ++subtrahend) {
    borrower = Math::subtractBorrow(*value, borrower);
    borrower += Math::subtractBorrow(*value, *subtrahend);
  }
  return borrower > 0;
}

bool LargeIntegerImpl::multiply(Word* value, const MemorySize size, const Word multiplicand) noexcept
{
  if (multiplicand == 1) {
    return false;
  } else if (multiplicand == 0) {
    clear(value, size);
    return false;
  }

  // we can do shift instead for power of 2

  const Word* end = value + size;
  Word carrier = 0;
  for (; value != end; ++value) {
    const DoubleWord temp = static_cast<DoubleWord>(*value) * multiplicand; // limit MAX*MAX!
    *value = static_cast<Word>(temp & MAXIMUM);
    carrier = Math::addCarry(*value, carrier); // 1 or 0
    carrier += (temp >> WORD_BITS); // +MAX*MAX/(MAX + 1) => <+MAX => (carrier <= MAX) no overflow
  }
  return carrier > 0;
}

MemorySize LargeIntegerImpl::getSize(const Word* value, const MemorySize size) noexcept
{
  const Word* src = value + size; // start at end of value
  while (src > value) {
    if (*--src != 0) {
      return (src - value) + 1;
    }
  }
  return 0; // all words are zero
}

MemorySize LargeIntegerImpl::getBitSize(const Word* value, const MemorySize size) noexcept
{
  const MemorySize count = getSize(value, size);
  if (count == 0) {
    return 0;
  }
  const auto bit = Math::getHighestBit(value[count - 1]);
  return (count - 1) * WORD_BITS + (bit + 1);
}

bool LargeIntegerImpl::isZero(const Word* value, const MemorySize size) noexcept
{
  for (const Word* end = value + size; value != end; ++value) {
    if (*value != 0) {
      return false;
    }
  }
  return true;
}

bool LargeIntegerImpl::isOne(const Word* value, const MemorySize size) noexcept
{
  if (size == 0) {
    return false;
  }
  if (value[0] != 1) {
    return false;
  }
  return isZero(value + 1, size - 1);
}

bool LargeIntegerImpl::lessThan(const Word* restrict left, const MemorySize size, const Word comparand) noexcept
{
  if (comparand == 0) {
    return false;
  }
  if (size == 0) {
    return true;
  }
  if (left[0] >= comparand) {
    return false;
  }
  return isZero(left + 1, size - 1);
}

bool LargeIntegerImpl::lessThan(const Word* restrict left, const Word* restrict right, const MemorySize size) noexcept
{
  const Word* end = left;
  left += size;
  right += size;
  while (left > end) {
    --left;
    --right;
    if (*left < *right) {
      return true;
    } else if (*left > *right) {
      return false;
    }
  }
  return false; // equal
}

bool LargeIntegerImpl::lessThanEqual(const Word* restrict left, const Word* restrict right, const MemorySize size) noexcept
{
  const Word* end = left;
  left += size;
  right += size;
  while (left > end) {
    --left;
    --right;
    if (*left < *right) {
      return true;
    } else if (*left > *right) {
      return false;
    }
  }
  return true; // equal
}

bool LargeIntegerImpl::equal(const Word* restrict left, MemorySize size, const Word comparand) noexcept
{
  if (size == 0) {
    return comparand == 0;
  }
  if (*left != comparand) {
    return false;
  }
  return isZero(left + 1, size - 1);
}

bool LargeIntegerImpl::equal(const Word* restrict left, const Word* restrict right, const MemorySize size) noexcept
{
  const Word* end = left + size;
  while (left != end) {
    if (*left++ != *right++) {
      return false;
    }
  }
  return true;
}

LargeIntegerImpl::Word LargeIntegerImpl::divide(Word* dividend, const MemorySize size, const Word divisor) noexcept
{
  { // simple case for power of 2
    if (divisor == 0) {
      BASSERT(!"Division by 0");
      return 0;
    }
    const unsigned int shift = Math::getHighestBit(divisor);
    if ((ONE << shift) == divisor) {
      const Word remainder = ((size > 0) ? dividend[0] : 0) & ((ONE << shift) - 1);
      rightShift(dividend, size, shift);
      return remainder;
    }
  }

  Word* word = dividend + size;
  Word remainder = 0;
  while (--word >= dividend) {
    const DoubleWord temp = (static_cast<DoubleWord>(remainder) << WORD_BITS) + *word;
    *word = static_cast<Word>((temp / divisor) & MAXIMUM);
    remainder = temp % divisor; // is it better to do (temp - quotient * divisor)?
  }
  return remainder;
}

LargeIntegerImpl::Word LargeIntegerImpl::remainder(const Word* dividend, const MemorySize size, const Word divisor) noexcept
{
  { // simple case for power of 2
    if (divisor == 0) {
      BASSERT(!"Division by 0");
      return 0;
    }
    const unsigned int shift = Math::getHighestBit(divisor);
    if ((ONE << shift) == divisor) {
      const Word remainder = ((size > 0) ? dividend[0] : 0)& ((ONE << shift) - 1);
      BASSERT(remainder < divisor);
      return remainder;
    }
  }

  const Word* word = dividend + size;
  Word remainder = 0;
  while (--word >= dividend) {
    const DoubleWord temp = (static_cast<DoubleWord>(remainder) << WORD_BITS) + *word;
    remainder = temp % divisor;
  }
  BASSERT(remainder < divisor);
  return remainder;
}



// may remainder be the same as dividend - I think so
void LargeIntegerImpl::divide(
  Word* restrict quotient,
  Word* remainder,
  const Word* dividend,
  const Word* restrict divisor,
  const MemorySize size) noexcept
{
  PrimitiveStackArray<Word> temp(size);

  clear(quotient, size);
  Word* tempDividend = remainder;
  assign(tempDividend, dividend, size);
  assign(&temp[0], divisor, size);

  if (lessThan(tempDividend, divisor, size)) {
    return;
  }

  auto divisorBitSize = getBitSize(divisor, size);
  auto positionOfDivisor = getBitSize(tempDividend, size) - divisorBitSize;
  leftShift(&temp[0], size, static_cast<unsigned int>(positionOfDivisor));

  const MemorySize reducedSize = size;

  while (!lessThan(tempDividend, divisor, reducedSize)) {
    // assert temp >= divisor
    auto newPosition = getBitSize(tempDividend, reducedSize) - divisorBitSize; // 0 <= newPosition < positionOfDivisor
    rightShift(&temp[0], reducedSize, static_cast<unsigned int>(positionOfDivisor - newPosition));
    positionOfDivisor = newPosition;

    if (lessThan(tempDividend, &temp[0], reducedSize)) { // shift = 0 => return value is false
      rightShift(&temp[0], reducedSize, 1);
      --positionOfDivisor;
    }

    addBit(quotient, size, positionOfDivisor); // must be size - is any carrier possible when divisor > 0

    subtract(tempDividend, &temp[0], reducedSize);
  }
  // tempDividend is remainder now
}



LargeInteger::LargeInteger()
{
}

LargeInteger::LargeInteger(const LargeInteger& copy)
  : value(copy.value)
{
}

LargeInteger::LargeInteger(const LargeInteger& copy, MemorySize extend)
{
  auto size = maximum(copy.getSize(), extend);
  value.setSize(size, 0);
  LargeIntegerImpl::assign(toWords(), copy.toWords(), copy.getSize());
}

LargeInteger::LargeInteger(LargeInteger&& move)
  : value(moveObject(move.value))
{
}

LargeInteger& LargeInteger::operator=(const LargeInteger& assign)
{
  value = assign.value;
  return *this;
}

LargeInteger& LargeInteger::operator=(Word assign)
{
  value.setSize(1);
  toWords()[0] = assign;
  return *this;
}

LargeInteger& LargeInteger::operator=(LargeInteger&& assign)
{
  value = moveObject(assign.value);
  return *this;
}

void LargeInteger::setSize(MemorySize size)
{
  value.setSize(size, 0); // unsigned extend
}

void LargeInteger::ensureCapacity(MemorySize capacity)
{
  value.ensureCapacity(capacity);
}

LargeInteger LargeInteger::plus() const noexcept
{
  return *this;
}

LargeInteger LargeInteger::minus() const noexcept
{
  LargeInteger result = *this;
  result.negate();
  return result;
}

LargeInteger& LargeInteger::negate() noexcept
{
  if (getSize() == 0) {
    return *this;
  }
  LargeIntegerImpl::negate(toWords(), getSize());
  return *this;
}

void LargeInteger::extend(MemorySize size) // TAG: we only extend for unsigned for now
{
  if (size > value.getSize()) {
    value.setSize(size, 0);
  }
}

void LargeInteger::signExtend(MemorySize size)
{
  if (size > value.getSize()) {
    bool negative = (value.getSize() > 0) && (value.getElements()[value.getSize() - 1] >> (LargeIntegerImpl::WORD_BITS - 1));
    value.setSize(size, negative ? LargeIntegerImpl::MAXIMUM : 0);
  }
}

void LargeInteger::trim()
{
  auto size = LargeIntegerImpl::getSize(value.getElements(), value.getSize());
  value.setSize(maximum<MemorySize>(size, 1)); // minimum 1 word
}

LargeInteger& LargeInteger::add(const LargeInteger& addend)
{
  const auto size = addend.getSize();
  extend(size);
  bool carry = LargeIntegerImpl::add(toWords(), addend.toWords(), size);
  if (carry) {
    if (getSize() > size) {
      /*carry =*/ LargeIntegerImpl::add(toWords() + size, getSize(), 1);
    }
  }
  return *this;
}

LargeInteger& LargeInteger::subtract(const LargeInteger& subtrahend)
{
  const auto size = subtrahend.getSize();
  extend(size);
  bool borrow = LargeIntegerImpl::subtract(toWords(), subtrahend, static_cast<unsigned int>(size));
  if (borrow) {
    if (getSize() > size) {
      /*borrow =*/ LargeIntegerImpl::subtract(toWords() + size, getSize(), 1);
    }
  }
  return *this;
}

bool LargeInteger::isZero() const noexcept
{
  return LargeIntegerImpl::isZero(toWords(), getSize());
}

LargeIntegerImpl::Word LargeInteger::getBits(unsigned int bitIndex, unsigned int bits) const noexcept
{
  return LargeIntegerImpl::getBits(toWords(), getSize(), bitIndex, bits);
}

bool LargeInteger::isBitSet(unsigned int bit) const noexcept
{
  return LargeIntegerImpl::isBitSet(toWords(), getSize(), bit);
}

void LargeInteger::setBit(unsigned int bit, bool b) noexcept
{
  extend((bit + LargeIntegerImpl::WORD_BITS - 1) / LargeIntegerImpl::WORD_BITS);
  LargeIntegerImpl::setBit(toWords(), getSize(), bit, b);
}

LargeInteger& LargeInteger::operator~() noexcept
{
  for (auto& v : value) {
    v = ~v;
  }
  return *this;
}

bool LargeInteger::operator==(const LargeInteger& comparand) const noexcept
{
  auto s1 = getSize();
  auto s2 = comparand.getSize();
  const Word* c1 = toWords();
  const Word* c2 = comparand.toWords();
  if (s1 <= s2) {
    const auto end = c1 + s1;
    while (c1 != end) {
      if (*c1++ != *c2++) {
        return false;
      }
    }
    return LargeIntegerImpl::isZero(c2, s2 - s1);
  } else {
    const auto end = c2 + s2;
    while (c2 != end) {
      if (*c1++ != *c2++) {
        return false;
      }
    }
    return LargeIntegerImpl::isZero(c1, s1 - s2);
  }
}

bool LargeInteger::operator==(const Word comparand) const noexcept
{
  const auto s1 = getSize();
  if (s1 == 0) {
    return comparand == 0;
  }
  const Word* c1 = toWords();
  if (c1[0] != comparand) {
    return false;
  }
  return LargeIntegerImpl::isZero(c1 + 1, s1 - 1);
}

LargeInteger& LargeInteger::operator&(const LargeInteger& operand) noexcept
{
  // no need to extend since (0 & x) is 0 // extend(operand.getSize());
  auto s1 = getSize();
  auto s2 = operand.getSize();
  Word* c1 = toWords();
  const Word* c2 = operand.toWords();
  if (s1 <= s2) {
    const auto end1 = c1 + s1;
    while (c1 != end1) {
      *c1++ &= *c2++;
    }
    // ignore remaining in c2
  } else {
    const auto end2 = c2 + s2;
    while (c2 != end2) {
      *c1++ &= *c2++;
    }
    const auto end1 = c1 + s1;
    while (c1 != end1) { // fill with zeros
      *c1++ = 0;
    }
  }
  return *this;
}

LargeInteger& LargeInteger::operator|(const LargeInteger& operand) noexcept
{
  // const auto s1_original = getSize();
  const auto s2 = operand.getSize();
  extend(s2);
  Word* c1 = toWords();
  const Word* c2 = operand.toWords();

  const auto end2 = c2 + s2;
  while (c2 != end2) {
    *c1++ |= *c2++;
  }

  // 0 | x = x
  return *this;
}

LargeInteger& LargeInteger::operator^(const LargeInteger& operand) noexcept
{
  // const auto s1_original = getSize();
  const auto s2 = operand.getSize();
  extend(s2);
  Word* c1 = toWords();
  const Word* c2 = operand.toWords();

  const auto end2 = c2 + s2;
  while (c2 != end2) {
    *c1++ ^= *c2++;
  }

  const auto end1 = c1 + s2; // yes s2 since we have extended
  while (c1 != end1) {
    *c1++ ^= 0;
  }

  return *this;
}

bool LargeInteger::operator<(const LargeInteger& comparand) const noexcept
{
  const auto s1 = getSize();
  const auto s2 = comparand.getSize();
  if (s1 <= s2) {
    return LargeIntegerImpl::lessThan(toWords(), comparand.toWords(), s1) &&
      LargeIntegerImpl::isZero(toWords() + s1, s2 - s1);
  } else {
    return LargeIntegerImpl::lessThan(toWords(), comparand.toWords(), s2) &&
      LargeIntegerImpl::isZero(comparand.toWords() + s2, s1 - s2);
  }
}

bool LargeInteger::operator<(const Word comparand) const noexcept
{
  const auto s1 = getSize();
  if (s1 == 0) {
    return 0 < comparand;
  }
  auto c1 = toWords();
  if (c1[0] >= comparand) {
    return false;
  }
  return LargeIntegerImpl::isZero(c1 + 1, s1 - 1);
}

bool LargeInteger::operator<=(const LargeInteger& comparand) const noexcept
{
  const auto s1 = getSize();
  const auto s2 = comparand.getSize();
  if (s1 <= s2) {
    return LargeIntegerImpl::lessThanEqual(toWords(), comparand.toWords(), s1) &&
      LargeIntegerImpl::isZero(toWords() + s1, s2 - s1);
  } else {
    return LargeIntegerImpl::lessThanEqual(toWords(), comparand.toWords(), s2) &&
      LargeIntegerImpl::isZero(comparand.toWords() + s2, s1 - s2);
  }
}

bool LargeInteger::operator<=(const Word comparand) const noexcept
{
  const auto s1 = getSize();
  if (s1 == 0) {
    return 0 <= comparand;
  }
  if (toWords()[0] < comparand) {
    return true;
  }
  return LargeIntegerImpl::isZero(toWords() + 1, s1 - 1);
}

LargeInteger& LargeInteger::operator<<=(const unsigned int shift) noexcept
{
  if (shift == 0) {
    return *this;
  }

  { // TAG: check high bits
    MemorySize newWords = (shift + LargeIntegerImpl::WORD_BITS - 1) / LargeIntegerImpl::WORD_BITS;
    const Word* begin = toWords();
    const Word* src = begin + getSize();
    while ((src != begin) && newWords) {
      if (*--src != 0) {
        break;
      }
      --newWords;
    }
    if (newWords) {
      extend(getSize() + newWords); // no need to extend top zeros
    }
  }
  
  LargeIntegerImpl::leftShift(toWords(), getSize(), shift);
  return *this;
}

LargeInteger& LargeInteger::operator>>=(unsigned int shift) noexcept
{
  LargeIntegerImpl::rightShift(toWords(), getSize(), shift);
  return *this;
}

LargeInteger& LargeInteger::multiply(const unsigned int multiplicand)
{
  const auto size = getSize();
  if (size == 0) {
    return *this; // already 0
  }

  const unsigned int shift = Math::getHighestBit(multiplicand);
  if ((static_cast<LargeIntegerImpl::Word>(1) << shift) == multiplicand) { // power of 2 case
    *this <<= shift;
    return *this;
  }

  auto words = toWords();
  const unsigned int highBit = Math::getHighestBit(toWords()[size - 1]);
  if ((highBit + shift) >= LargeIntegerImpl::WORD_BITS) {
    extend(size + 1); // no need to extend if we dont need more bits
  }
  LargeIntegerImpl::multiply(words, size, multiplicand);
  return *this;
}

unsigned int LargeInteger::divide(const unsigned int divisor)
{
  if (getSize() == 0) {
    return 0;
  }
  if (divisor == 0) {
    *this = 0U;
    return 0;
  }

  return LargeIntegerImpl::divide(toWords(), getSize(), divisor);
}

unsigned int LargeInteger::remainder(const unsigned int divisor) const noexcept
{
  if (getSize() == 0) {
    return 0;
  }
  return LargeIntegerImpl::remainder(toWords(), getSize(), divisor);
}

LargeInteger operator+(const LargeInteger& left, const LargeInteger& right)
{
  LargeInteger result(left, right.getSize());
  result += right;
  return result;
}

LargeInteger operator-(const LargeInteger& left, const LargeInteger& right)
{
  LargeInteger result(left, right.getSize());
  result -= right;
  return result;
}

LargeInteger operator*(const LargeInteger& left, const LargeInteger& right)
{
  LargeInteger result(left, right.getSize());
  result *= right;
  return result;
}

LargeInteger operator*(const unsigned int left, const LargeInteger& right)
{
  LargeInteger result(right);
  result *= left;
  return result;
}

LargeInteger operator*(const LargeInteger& left, const unsigned int right)
{
  LargeInteger result(left);
  result *= right;
  return result;
}

LargeInteger operator/(const LargeInteger& left, const LargeInteger& right)
{
  LargeInteger result(left, right.getSize());
  result /= right;
  return result;
}

LargeInteger operator/(const LargeInteger& left, const unsigned int right)
{
  LargeInteger result(left);
  result /= right;
  return result;
}

LargeInteger operator%(const LargeInteger& left, const LargeInteger& right)
{
  LargeInteger result(left, right.getSize());
  result %= right;
  return result;
}

LargeInteger operator%(const LargeInteger& left, const unsigned int right)
{
  LargeInteger result(left);
  result %= right;
  return result;
}

namespace {

  void reverse(char* first, char* last) noexcept
  {
    while (first < last) {
      swapper(*first++, *last--);
    }
  }

  char* storeDigits(char* dest, const LargeInteger& value, const unsigned int fieldSize, const bool upper = false) noexcept
  {
    BASSERT(fieldSize);
    if (value.getSize() == 0) {
      *dest++ = '0';
      return dest;
    }

    char* first = dest;
    // trim upper zeros
    MemorySize bitIndex = (value.getSize() * LargeIntegerImpl::WORD_BITS - 1) / fieldSize * fieldSize; // start on proper bit offset
    while ((bitIndex >= fieldSize) && !value.getBits(static_cast<unsigned int>(bitIndex), fieldSize)) { // skip top zeros
      bitIndex -= fieldSize;
    }

    for (MemorySize i = 0; i <= bitIndex; i += fieldSize) { // bitIndex is lower bound hence <=
      *dest++ = ASCIITraits::valueToDigit(value.getBits(static_cast<unsigned int>(i), fieldSize), upper); // get digit
    }

    reverse(first, dest - 1); // make high digits first
    return dest;
  }
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const LargeInteger& value)
{
  if (value.getSize() <= 1) {
    return stream << (value & LargeIntegerImpl::MAXIMUM);
  }

  PrimitiveStackArray<char> buffer(4096);

  char* dest = nullptr;
  switch (stream.getBase()) {
  case FormatOutputStream::Symbols::BINARY:
    {
      buffer.resize(maximum<MemorySize>((value.getSize() * LargeIntegerImpl::WORD_BITS + 1 - 1) / 1, 1));
      dest = storeDigits(buffer, value, 1);
      break;
    }
  case FormatOutputStream::Symbols::OCTAL:
    {
      buffer.resize(maximum<MemorySize>((value.getSize() * LargeIntegerImpl::WORD_BITS + 3 - 1) / 3, 1));
      dest = storeDigits(buffer, value, 3);
      break;
    }
  case FormatOutputStream::Symbols::DECIMAL:
    {
      // base 10 => <4 bit
      // we can do better since we just assume many digits - 1000 <= 1024 => 3 digits <= 10 bit
      buffer.resize(maximum<MemorySize>((((value.getSize() * LargeIntegerImpl::WORD_BITS + 10 - 1) / 10) + 2)/3, 1)); // 10 bits - 3 digits
      dest = buffer;

      if (value.getSize() == 0) {
        *dest++ = '0';
        break;
      }

      BASSERT(value >= 0U);
      LargeInteger temp(value); // (value >= 0U) ? value : -value;
      temp.ensureCapacity(temp.getSize());
      temp.trim();

      unsigned int count = (sizeof(LargeIntegerImpl::Word) == 4) ? 10 : 20; // LargeIntegerImpl::MAXIMUM / 10 ^ n => n == 10
      while (temp) {
        const unsigned int remainder = temp.divide(10U);
        *dest++ = ASCIITraits::valueToDigit(remainder); // get digit
        if (!--count) { // get rid of upper zeros
          temp.trim();
          count = (sizeof(LargeIntegerImpl::Word) == 4) ? 10 : 20;
        }
      }

      reverse(buffer, dest - 1); // make high digits first
      break;
    }
  case FormatOutputStream::Symbols::HEXADECIMAL:
    {
      buffer.resize(maximum<MemorySize>((value.getSize() * LargeIntegerImpl::WORD_BITS + 4 - 1) / 4, 1));
      dest = storeDigits(buffer, value, 4, (stream.getFlags() & FormatOutputStream::Symbols::UPPER) != 0);
      break;
    }
  default:
    return stream; // do not do anything if base is unknown
  }

  stream.addIntegerField(buffer, dest - buffer, value < 0U);
  return stream;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(LargeInteger) : public UnitTest {
public:

  TEST_PRIORITY(1); // used for float to string
  TEST_PROJECT("base/math");

  void run() override
  {
    LargeInteger i1;
    TEST_ASSERT(!i1);
    TEST_ASSERT(i1 == 0U);
    TEST_ASSERT(!(i1 != 0U));
    TEST_ASSERT(!(i1 < 0U));
    TEST_ASSERT(i1 <= 0U);
    TEST_ASSERT(!(i1 > 0U));
    TEST_ASSERT(i1 >= 0U);

    // fout << "ZERO: " << i1 << ENDL;

    i1 = 1;
    TEST_ASSERT(i1);
    TEST_ASSERT(i1 == 1U);
    TEST_ASSERT(!(i1 != 1U));
    TEST_ASSERT(!(i1 < 1U));
    TEST_ASSERT(i1 <= 1U);
    TEST_ASSERT(!(i1 > 1U));
    TEST_ASSERT(i1 >= 1U);

    // fout << "ONE: " << i1 << ENDL;

    i1 = 11;
    // fout << "11: " << i1 << ENDL;
    i1 *= 13;
    // fout << "13*11: " << i1 << ENDL;

    for (unsigned int i = 0; i < 17; ++i) {
      i1 <<= 3;
      // fout << "SHIFT LEFT  " << i1 << ENDL;
    }

    for (unsigned int i = 0; i < 17; ++i) {
      i1 >>= 3;
      // fout << "SHIFT RIGHT " << i1 << ENDL;
    }

  }
};

TEST_REGISTER(LargeInteger);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
