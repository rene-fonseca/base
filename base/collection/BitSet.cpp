/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/collection/BitSet.h>
#include <base/string/StringOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void BitSet::zeroExtend(unsigned int size) throw(MemoryException) {
  if (size > this->size) {
    unsigned int count = getNumberOfElements(size);
    if (this->size == 0) {
      this->elements->setSize(count);
      fill<unsigned long>(getElements(), count, 0);
    } else {
      unsigned int oldCount = getNumberOfElements(this->size);
      if (oldCount < count) {
        this->elements->setSize(count);
        fill<unsigned long>(getElements() + oldCount, count - oldCount, 0); // does not overwrite bits in use
      }
    }
    this->size = size;
  }
}

BitSet::BitSet() throw() : elements(new ReferenceCountedCapacityAllocator<unsigned long>()), size(0) {
}

BitSet::BitSet(unsigned int s, bool value) throw() :
  elements(new ReferenceCountedCapacityAllocator<unsigned long>(getNumberOfElements(size))),
  size(s) {
  fill<unsigned long>(getElements(), getNumberOfElements(size), value ? ~0UL : 0);
  reinitialize();
}

BitSet& BitSet::operator=(const BitSet& eq) throw() {
  if (&eq != this) { // protect against self assignment
    elements = eq.elements;
    size = eq.size;
  }
  return *this;
}

bool BitSet::getAt(unsigned int index) const throw(OutOfRange) {
  assert(index < size, OutOfRange(this));
  return (getElements()[getElementIndex(index)]) & getBitMask(index);
}

void BitSet::setAt(unsigned int index, bool value) throw(OutOfRange) {
  assert(index < size, OutOfRange(this));
  if (value) {
    getElements()[getElementIndex(index)] |= getBitMask(index); // set
  } else {
    getElements()[getElementIndex(index)] &= ~getBitMask(index); // reset
  }
}

BitSet& BitSet::set() throw() {
  fill<unsigned long>(getElements(), getNumberOfElements(size), ~(unsigned long)0);
  reinitialize();
  return *this;
}

BitSet& BitSet::set(unsigned int index) throw(OutOfRange) {
  assert(index < size, OutOfRange(this));
  getElements()[getElementIndex(index)] |= getBitMask(index);
  return *this;
}

BitSet& BitSet::reset() throw() {
  fill<unsigned long>(getElements(), getNumberOfElements(size), 0);
  // no need to reinitialize
  return *this;
}

BitSet& BitSet::reset(unsigned int index) throw(OutOfRange) {
  assert(index < size, OutOfRange(this));
  getElements()[getElementIndex(index)] &= ~getBitMask(index);
  return *this;
}

BitSet& BitSet::flip() throw() {
  transform<unsigned long>( // TAG: check this
    getElements(),
    getNumberOfElements(size),
    bind2Second(BitwiseExclusiveOr<unsigned long>(), ~(unsigned long)0)
  );
  reinitialize();
  return *this;
}

BitSet& BitSet::flip(unsigned int index) throw(OutOfRange) {
  assert(index < size, OutOfRange(this));
  getElements()[getElementIndex(index)] ^= getBitMask(index);
  return *this;
}

BitSet& BitSet::operator&=(const BitSet& value) throw() {
  // zeroExtend is not required
  transformByBinary(
    getElements(),
    value.getElements(),
    getNumberOfElements(minimum(size, value.size)),
    BitwiseAnd<unsigned long>()
  );
  return *this;
}

BitSet& BitSet::operator|=(const BitSet& value) throw() {
  zeroExtend(value.getSize());
  transformByBinary(
    getElements(),
    value.getElements(),
    getNumberOfElements(minimum(size, value.size)),
    BitwiseOr<unsigned long>()
  );
  return *this;
}

BitSet& BitSet::operator^=(const BitSet& value) throw() {
  zeroExtend(value.getSize());
  transformByBinary(
    getElements(),
    value.getElements(),
    getNumberOfElements(minimum(size, value.size)),
    BitwiseExclusiveOr<unsigned long>()
  );
  return *this;
}

BitSet& BitSet::operator<<=(unsigned int count) throw() {
  if (count == 0) {
    return *this;
  }

  unsigned int oldSize = size;
  setSize(size + count); // possible copy on write

  unsigned long* writeHead = getElements() + getElementIndex(count);
  const unsigned long* readHead = getElements(); // must be called after possible copy on write
  unsigned int mostSignificant = count % (sizeof(unsigned long) * 8);
  unsigned int leastSignificant = sizeof(unsigned long) * 8 - mostSignificant;

  if (mostSignificant == 0) { // can we do a fast move without binary shifts
    move<unsigned long>(writeHead, readHead, getNumberOfElements(oldSize));
  } else {
    const unsigned long* end = readHead + getNumberOfElements(oldSize);
    unsigned long previous = 0;
    while (readHead <= end) {
      unsigned long temp = *readHead;
      *writeHead = (previous >> leastSignificant) | (temp << mostSignificant);
      previous = temp;
      ++writeHead;
      ++readHead;
    }
  }

  fill<unsigned long>(getElements(), getElementIndex(count), 0); // reinitialize beginning
  return *this;
}

BitSet& BitSet::operator>>=(unsigned int count) throw() {
  if (count == 0) {
    return *this;
  }

  unsigned long* writeHead = getElements(); // possible copy on write
  const unsigned long* readHead = writeHead + getElementIndex(count) + 1;
  unsigned int leastSignificant = count % (sizeof(unsigned long) * 8);
  unsigned int mostSignificant = sizeof(unsigned long) * 8 - leastSignificant;

  if (leastSignificant == 0) { // can we do a fast move without binary shifts
    move<unsigned long>(writeHead, readHead, getNumberOfElements(size));
  } else {
    const unsigned long* end = writeHead + getNumberOfElements(size);
    unsigned long previous = *readHead++;
    while (readHead <= end) { // TAG: possible segmentation fault problem
      unsigned long temp = *readHead;
      *writeHead = (previous >> leastSignificant) | (temp << mostSignificant);
      previous = temp;
      ++writeHead;
      ++readHead;
    }
  }

  setSize(size - count);
  return *this;
}

void BitSet::removeAll() throw() {
  elements = new ReferenceCountedCapacityAllocator<unsigned long>(); // no need to copy
  size = 0;
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const BitSet& value) throw(IOException) {
  StringOutputStream buffer;

  const unsigned long* current = value.getElements() + value.getSize();
  unsigned long count = value.getSize();
  unsigned long mask = 1UL << (value.getSize() % (sizeof(unsigned long) * 8)); // most significant first

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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
