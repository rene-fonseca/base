/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mathematics/LargeInteger.h>
#include <base/mathematics/Math.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void LargeIntegerImpl::clear(Word* value, MemorySize size) noexcept
{
  fill<Word>(value, size, 0);
}

void LargeIntegerImpl::assign(Word* restrict dest, const Word* restrict src, MemorySize size) noexcept
{
  if (!INLINE_ASSERT(dest != src)) {
    return; // self assignment - violates restrict
  }
  copy(dest, src, size);
}

void LargeIntegerImpl::setBit(Word* value, MemorySize size, unsigned int bit) noexcept
{
  BASSERT(bit < (size * WORD_BITS));
  fill<Word>(value, size, 0);
  value[bit / WORD_BITS] = static_cast<Word>(1) << (bit % WORD_BITS);
}

bool LargeIntegerImpl::addBit(Word* value, MemorySize size, unsigned int bit) noexcept
{
  BASSERT(bit < (size * WORD_BITS));
  value += bit / WORD_BITS;
  Word carrier = static_cast<Word>(1) << (bit % WORD_BITS);
  for (const Word* end = value + size; value < end; ++value) {
    const DoubleWord temp = static_cast<DoubleWord>(*value) + carrier;
    *value = temp;
    carrier = (temp & (static_cast<DoubleWord>(1) << WORD_BITS)) ? 1 : 0;
    if (carrier == 0) {
      return false;
    }
  }
  return true;
}

void LargeIntegerImpl::leftShift(Word* value, MemorySize size, unsigned int shift) noexcept
{
  unsigned int bitShift = shift % WORD_BITS;
  unsigned int wordShift = shift / WORD_BITS;

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
void LargeIntegerImpl::rightShift(Word* value, MemorySize size, unsigned int shift) noexcept
{
  unsigned int bitShift = shift % WORD_BITS;
  unsigned int wordShift = shift / WORD_BITS;
  const Word* src = value + wordShift;
  Word* dest = value;

  if (bitShift != 0) {
    const unsigned int nextBitShift = WORD_BITS - bitShift; // 0 < nextBitShift < WORD_BITS
    for (const Word* end = value + (size - wordShift - 1); dest < end; ++dest) {
      Word temp = *src >> bitShift;
      ++src;
      temp |= *src << nextBitShift;
      *dest = temp;
    }
    *dest++ = *src >> bitShift;
  } else {
    for (const Word* end = value + (size - wordShift); dest < end; ++src, ++dest) {
      *dest = *src;
    }
  }
  for (const Word* end = value + size; dest < end; ++dest) { // mask end of value
    *dest = 0;
  }
}

bool LargeIntegerImpl::add(Word* value, MemorySize size, Word addend) noexcept
{
  const Word* end = value + size;
  Word carrier = addend;
  for (; (value < end) && carrier; ++value) {
    const bool carry = *value > (MAXIMUM - carrier);
    *value += carrier;
    carrier = carry ? 1 : 0;
  }
  return carrier > 0;
}

bool LargeIntegerImpl::add(Word* restrict value, const Word* restrict addend, MemorySize size) noexcept
{
  const Word* end = value + size;
  Word carrier = 0;
  for (; value < end; ++value, ++addend) {
    // switch single Word implementation
    const DoubleWord temp = static_cast<DoubleWord>(*value) + *addend + carrier;
    *value = static_cast<Word>(temp & MAXIMUM);
    carrier = static_cast<Word>(temp >> WORD_BITS);
  }
  return carrier > 0;
}

bool LargeIntegerImpl::subtract(Word* restrict value, MemorySize size, Word subtrahend) noexcept
{
  const Word* end = value + size;
  Word borrower = subtrahend;
  for (; (value < end) && borrower; ++value) {
    const bool borrow = *value < borrower;
    *value -= borrower;
    borrower = borrow ? 1 : 0;
  }
  return borrower > 0;
}

bool LargeIntegerImpl::subtract(Word* restrict value, const Word* restrict subtrahend, MemorySize size) noexcept
{
  const Word* end = value + size;
  Word borrow = 0;
  for (; value < end; ++value, ++subtrahend) {
    // switch single Word implementation
    BASSERT(borrow <= 1);
    const DoubleWord temp = (static_cast<DoubleWord>(1) << WORD_BITS) + *value - *subtrahend - borrow;
    *value = static_cast<Word>(temp & MAXIMUM);
    borrow = (temp >> WORD_BITS) ? 1 : 0;
  }
  return borrow > 0;
}

bool LargeIntegerImpl::checkOverflow(const Word* restrict left, const Word* restrict right, MemorySize size) noexcept
{
  const Word* end = left + size;
  Word carrier = 0;
  for (; left < end; ++left, ++right) {
    const DoubleWord temp = static_cast<DoubleWord>(*left) + *right + carrier;
    carrier = static_cast<Word>(temp >> WORD_BITS);
  }
  return carrier > 0;
}

bool LargeIntegerImpl::multiply(Word* value, MemorySize size, Word multiplicand) noexcept
{
  const Word* end = value + size;
  Word carrier = 0;
  for (; value < end; ++value) {
    const DoubleWord temp = static_cast<DoubleWord>(*value) * multiplicand + carrier;
    *value = static_cast<Word>(temp);
    carrier = static_cast<Word>(temp >> WORD_BITS);
  }
  return carrier > 0;
}

MemorySize LargeIntegerImpl::getSize(const Word* value, MemorySize size) noexcept
{
  const Word* src = value + size; // start at end of value
  while (src > value) {
    if (*--src != 0) {
      return (src - value) + 1;
    }
  }
  return 0; // all words are zero
}

MemorySize LargeIntegerImpl::getBitSize(const Word* value, MemorySize size) noexcept
{
  const MemorySize count = getSize(value, size);
  if (count == 0) {
    return 0;
  }
  const auto bit = Math::getHighestBit(value[count - 1]);
  return count * WORD_BITS + bit + 1;

#if 0
  for (int bit = WORD_BITS - 1; bit >= 0; --bit) {
    if (temp >> bit) {
      return (src - value) * WORD_BITS + bit + 1;
    }
  }
#endif
}

bool LargeIntegerImpl::isZero(const Word* value, MemorySize size) noexcept
{
  for (const Word* end = value + size; value < end; ++value) {
    if (*value != 0) {
      return false;
    }
  }
  return true;
}

bool LargeIntegerImpl::isOne(const Word* value, MemorySize size) noexcept
{
  if (size == 0) {
    return false;
  }
  if (value[0] != 1) {
    return false;
  }
  return isZero(value + 1, size - 1);
}

bool LargeIntegerImpl::lessThan(const Word* restrict left, MemorySize size, Word comparand) noexcept
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

bool LargeIntegerImpl::lessThan(const Word* restrict left, const Word* restrict right, MemorySize size) noexcept
{
  const Word* end = left;
  left += size;
  right += size;
  while (left > end) {
    --left;
    --right;
    if (*left < *right) {
      return true;
    } else if (*left > * right) {
      return false;
    }
  }
  return false;
}

bool LargeIntegerImpl::lessThanEqual(const Word* restrict left, const Word* restrict right, MemorySize size) noexcept
{
  const Word* end = left;
  left += size;
  right += size;
  while (left > end) {
    --left;
    --right;
    if (*left < *right) {
      return true;
    } else if (*left > * right) {
      return false;
    }
  }
  return true;
}

bool LargeIntegerImpl::equal(const Word* restrict left, const Word* restrict right, MemorySize size) noexcept
{
  const Word* end = left + size;
  while (left < end) {
    if (*left++ != *right++) {
      return false;
    }
  }
  return true;
}

LargeIntegerImpl::Word LargeIntegerImpl::divide(Word* value, MemorySize size, Word divisor) noexcept
{
  Word* word = value + size;
  Word remainder = 0;
  while (--word >= value) {
    const DoubleWord temp = (static_cast<DoubleWord>(remainder) << WORD_BITS) + *word;
    *word = static_cast<Word>((temp / divisor) & MAXIMUM);
    remainder = temp % divisor;
  }
  return remainder;
}

// may remainder be the same as dividend - I think so
void LargeIntegerImpl::divide(
  Word* restrict quotient,
  Word* remainder,
  const Word* dividend,
  const Word* restrict divisor,
  MemorySize size) noexcept
{
  // TAG: if divisior can be in Word then use
  // Word x divide(value, size, divisor);

  PrimitiveArray<Word> temp(size);
  clear(quotient, size);
  Word* tempDividend = remainder;
  assign(tempDividend, dividend, size);
  assign(&temp[0], divisor, size);

  if (lessThan(tempDividend, divisor, size)) {
    return;
  }

  unsigned int divisorBitSize = getBitSize(divisor, size);
  unsigned int positionOfDivisor = getBitSize(tempDividend, size) - divisorBitSize;
  leftShift(&temp[0], size, positionOfDivisor);

  const MemorySize reducedSize = size;

  while (!lessThan(tempDividend, divisor, reducedSize)) {
    // assert temp >= divisor
    unsigned int newPosition = getBitSize(tempDividend, reducedSize) - divisorBitSize; // 0 <= newPosition < positionOfDivisor
    rightShift(&temp[0], reducedSize, positionOfDivisor - newPosition);
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
  : value(std::move(move.value))
{
}

LargeInteger& LargeInteger::operator=(const LargeInteger& assign)
{
  value = assign.value;
  return *this;
}

LargeInteger& LargeInteger::operator=(LargeInteger&& assign)
{
  value = std::move(assign.value);
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
  auto src = value.getElements();
  auto end = src + value.getSize();
  while (src != end) {
    // TAG: FIXME
  }
  return *this;
}

void LargeInteger::extend(MemorySize size) // TAG: we only extend for unsigned for now
{
  if (size > value.getSize()) {
    value.setSize(value.getSize(), 0);
  }
}

void LargeInteger::signExtend(MemorySize size)
{
  if (size > value.getSize()) {
    bool negative = (value.getSize() > 0) && (value.getElements()[value.getSize() - 1] >> (LargeIntegerImpl::WORD_BITS - 1));
    value.setSize(value.getSize(), negative ? LargeIntegerImpl::MAXIMUM : 0);
  }
}

void LargeInteger::trim()
{
  auto size = LargeIntegerImpl::getSize(value.getElements(), value.getSize());
  value.setSize(size);
}

LargeInteger& LargeInteger::add(const LargeInteger& addend)
{
  const auto size = addend.getSize();
  extend(size);
  bool carry = LargeIntegerImpl::add(toWords(), addend.toWords(), size);
  if (carry) {
    if (getSize() > size) {
      carry = LargeIntegerImpl::add(toWords() + size, getSize(), 1);
    }
  }
  return *this;
}

LargeInteger& LargeInteger::subtract(const LargeInteger& subtrahend)
{
  const auto size = subtrahend.getSize();
  extend(size);
  bool borrow = LargeIntegerImpl::subtract(toWords(), subtrahend, size);
  if (borrow) {
    if (getSize() > size) {
      borrow = LargeIntegerImpl::subtract(toWords() + size, getSize(), 1);
    }
  }
  return *this;
}

bool LargeInteger::isZero() const noexcept
{
  return LargeIntegerImpl::isZero(value.getElements(), value.getSize());
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

LargeInteger& LargeInteger::multiply(int value)
{
  BASSERT(!"Not implemented.");
  return *this;
}

LargeInteger& LargeInteger::divide(int value)
{
  BASSERT(!"Not implemented.");
  return *this;
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

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(LargeInteger) : public UnitTest {
public:

  TEST_PRIORITY(1000);
  TEST_PROJECT("base/math");

  void run() override
  {
    LargeInteger i1;
    TEST_ASSERT(i1);
    TEST_ASSERT(i1 == 0U);
    TEST_ASSERT(!(i1 != 0U));
    TEST_ASSERT(!(i1 < 0U));
    TEST_ASSERT(i1 <= 0U);
    TEST_ASSERT(!(i1 > 0U));
    TEST_ASSERT(i1 >= 0U);
  }
};

TEST_REGISTER(LargeInteger);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
