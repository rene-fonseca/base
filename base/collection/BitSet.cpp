/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/BitSet.h>
#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void BitSet::zeroExtend(MemorySize size)
{
  if (size > this->size) {
    unsigned int count = getNumberOfElements(size);
    if (this->size == 0) {
      this->elements->setSize(count);
      fill<unsigned long>(getElements(), count, 0);
    } else {
      unsigned int oldCount = getNumberOfElements(this->size);
      if (oldCount < count) {
        this->elements->setSize(count);
        // does not overwrite bits in use
        fill<unsigned long>(getElements() + oldCount, count - oldCount, 0);
      }
    }
    this->size = size;
  }
}

BitSet::BitSet()
  : elements(new ReferenceCountedCapacityAllocator<unsigned long>()), size(0)
{
}

BitSet::BitSet(unsigned int _size, bool value)
  : elements(
      new ReferenceCountedCapacityAllocator<unsigned long>(
        getNumberOfElements(_size),
        ReferenceCountedCapacityAllocator<unsigned long>::DEFAULT_GRANULARITY
      )
    ),
    size(_size)
{
  fill<unsigned long>(
    getElements(),
    getNumberOfElements(size),
    value ? ~0UL : 0UL
  );
  reinitialize();
}

bool BitSet::getAt(unsigned int index) const throw(OutOfRange)
{
  if (!(index < size)) {
    throw OutOfRange(this);
  }
  return getElements()[getElementIndex(index)] & getBitMask(index);
}

void BitSet::setAt(unsigned int index, bool value) throw(OutOfRange)
{
  if (!(index < size)) {
    throw OutOfRange(this);
  }
  if (value) {
    getElements()[getElementIndex(index)] |= getBitMask(index); // set
  } else {
    getElements()[getElementIndex(index)] &= ~getBitMask(index); // reset
  }
}

BitSet& BitSet::set() noexcept
{
  fill<unsigned long>(getElements(), getNumberOfElements(size), ~0UL);
  reinitialize();
  return *this;
}

BitSet& BitSet::set(unsigned int index) throw(OutOfRange)
{
  if (!(index < size)) {
    throw OutOfRange(this);
  }
  getElements()[getElementIndex(index)] |= getBitMask(index);
  return *this;
}

BitSet& BitSet::reset() noexcept
{
  fill<unsigned long>(getElements(), getNumberOfElements(size), 0);
  // no need to reinitialize
  return *this;
}

BitSet& BitSet::reset(unsigned int index) throw(OutOfRange) {
  if (!(index < size)) {
    throw OutOfRange(this);
  }
  getElements()[getElementIndex(index)] &= ~getBitMask(index);
  return *this;
}

BitSet& BitSet::flip() noexcept
{
  transform<unsigned long>( // TAG: check this
    getElements(),
    getNumberOfElements(size),
    bind2Second(BitwiseExclusiveOr<unsigned long>(), ~0UL)
  );
  reinitialize();
  return *this;
}

BitSet& BitSet::flip(unsigned int index) throw(OutOfRange)
{
  if (!(index < size)) {
    throw OutOfRange(this);
  }
  getElements()[getElementIndex(index)] ^= getBitMask(index);
  return *this;
}

BitSet& BitSet::operator&=(const BitSet& value) noexcept
{
  // zeroExtend is not required
  transformByBinary(
    getElements(),
    value.getElements(),
    getNumberOfElements(minimum(size, value.size)),
    BitwiseAnd<unsigned long>()
  );
  return *this;
}

BitSet& BitSet::operator|=(const BitSet& value) noexcept
{
  zeroExtend(value.getSize());
  transformByBinary(
    getElements(),
    value.getElements(),
    getNumberOfElements(minimum(size, value.size)),
    BitwiseOr<unsigned long>()
  );
  return *this;
}

BitSet& BitSet::operator^=(const BitSet& value) noexcept
{
  zeroExtend(value.getSize());
  transformByBinary(
    getElements(),
    value.getElements(),
    getNumberOfElements(minimum(size, value.size)),
    BitwiseExclusiveOr<unsigned long>()
  );
  return *this;
}

BitSet& BitSet::operator<<=(unsigned int shift) noexcept
{
  if (shift == 0) {
    return *this;
  }
  
  const unsigned int bitShift = shift % (sizeof(unsigned long) * 8);
  const unsigned int wordShift = shift/(sizeof(unsigned long) * 8);
  const unsigned int wordSize = getNumberOfElements(size);
  unsigned long* begin = getElements();
  
  // start from last non-zero value + wordSize (but do not exceed end)
  unsigned long* dest = begin + wordSize - 1;
  const unsigned long* src = dest - wordShift;
  
  if (bitShift == 0) {
    while (src >= begin) {
      *dest-- = *src--;
    }
  } else {
    unsigned int invBitShift = (sizeof(unsigned long) * 8) - bitShift;
    while (src > begin) {
      *dest = (*src << bitShift) | (src[-1] >> invBitShift);
      --src;
      --dest;
    }
    *dest = *src << bitShift; // final (shift in zeros)
  }
  fill<unsigned long>(begin, wordShift, 0UL); // mask beginning of value
  return *this;
}

BitSet& BitSet::operator>>=(unsigned int shift) noexcept {
  if (shift == 0) {
    return *this;
  }
  
  const unsigned int bitShift = shift % (sizeof(unsigned long) * 8);
  const unsigned int wordShift = shift/(sizeof(unsigned long) * 8);
  
  unsigned long* dest = getElements(); // possible copy on write
  const unsigned long* src = dest + wordShift; // getElementIndex(shift);
  const unsigned int wordSize = getNumberOfElements(size);
  
  if (bitShift == 0) { // can we do a fast move without binary shifts
    move<unsigned long>(dest, src, wordSize - wordShift);
    dest += wordSize - wordShift;
  } else {
    unsigned int nextBitShift = (sizeof(unsigned long) * 8) - bitShift; // 0 < nextBitShift < (sizeof(unsigned long) * 8)
    for (const unsigned long* end = dest + (wordSize - wordShift - 1); dest != end; ++dest) {
      unsigned int temp = *src >> bitShift;
      ++src;
      temp |= *src << nextBitShift;
      *dest = temp;
    }
    *dest++ = *src >> bitShift;
  }
  fill<unsigned long>(dest, wordShift, 0UL); // setSize(size - shift);
  return *this;
}

void BitSet::removeAll() noexcept {
  elements = new ReferenceCountedCapacityAllocator<unsigned long>(); // no need to copy
  size = 0;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const BitSet& value) throw(IOException) {
  StringOutputStream buffer;
  
  const unsigned long* current = value.getElements() + value.getSize();
  unsigned long count = value.getSize();
  // most significant first
  unsigned long mask = 1UL << (value.getSize() % (sizeof(unsigned long) * 8));
  
  while (count) {
    unsigned long value = *current;
    --current;
    while (mask) { // continue until all bits have been written
      buffer << ((value & mask) ? '1' : '0');
      mask >>= 1;
      --count;
    }
    mask = 1UL << (sizeof(unsigned long) * 8 - 1); // most significant first
  }
  return stream << buffer.getString();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
