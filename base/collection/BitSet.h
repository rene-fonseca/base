/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__BIT_SET_H
#define _DK_SDU_MIP__BASE_COLLECTION__BIT_SET_H

#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/collection/Collection.h>
#include <base/collection/Enumeration.h>
#include <base/collection/InvalidEnumeration.h>
#include <base/OutOfRange.h>
#include <base/string/FormatOutputStream.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Bit set. Ordered sequence of boolean values. Offen used to represent a set of flags.

  @short Set of bits.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class BitSet : public Collection {
public:

  /**
    Reference to a single bit within a BitSet.
  */
  class Reference {
  private:
    friend class BitSet;
    BitSet& bitset; // use reference to avoid 'copy on write'
    unsigned int index;
    Reference(const Reference& copy); // prohibit default copy initialization
    Reference& operator=(const Reference& eq); // prohibit default assignment
    inline Reference(BitSet& b, unsigned int i) : bitset(b), index(i) {}
  public:
    inline Reference& operator=(bool value) throw(OutOfRange) {bitset.setAt(index, value); return *this;}
    inline operator bool() throw(OutOfRange) {return bitset.getAt(index);}
  };
private:

  /** The elements of the bit set. */
  ReferenceCountedObjectPointer<ReferenceCountedCapacityAllocator<unsigned long> > elements;
  /** The number of bits in the bit set. */
  unsigned int size;
protected:

  /**
    Returns the number of required elements to hold the specified number of bits.
  */
  static inline unsigned int getNumberOfElements(unsigned int size) throw() {
    return (size + sizeof(unsigned long) * 8 - 1)/(sizeof(unsigned long) * 8);
  }

  /**
    Returns the index of the internal element holding the bit at the specified index.
  */
  static inline unsigned int getElementIndex(unsigned int index) throw() {
    return index/(sizeof(unsigned long) * 8);
  }

  /**
    Returns the bit mask for the specified index.
  */
  static inline unsigned long getBitMask(unsigned int index) throw() {
    return ((unsigned long)1) << (index % (sizeof(unsigned long) * 8));
  }

  /**
    Returns the elements of the internal array for modifying access.
  */
  inline unsigned long* getElements() throw(MemoryException) {
    elements.copyOnWrite();
    return elements->getElements();
  }

  /**
    Returns the elements of the internal array for non-modifying access.
  */
  inline const unsigned long* getElements() const throw() {
    return elements->getElements();
  }

  /**
    Zero-extends the bit set to the specified size.
  */
  void zeroExtend(unsigned int size) throw(MemoryException);

  /**
    Resets any unused bits.
  */
  inline void reinitialize() throw() {
    unsigned long mask = getBitMask(size) - 1; // TAG: big endian problem
    if ((size > 0) && (mask != 0)) { // are some of the bits unused
      getElements()[getNumberOfElements(size) - 1] &= mask; // reset unused bits
    }
  }

  /**
    Sets the number of bits in the bit set.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    elements->setSize(getNumberOfElements(size));
    reinitialize();
  }
public:

  /**
    Initializes an empty bit set.
  */
  BitSet() throw();

  /**
    Initializes array with the specified number of elements.

    @param size The initial number of bits.
    @param value The initial state of the bits.
  */
  BitSet(unsigned int size, bool value) throw();

  /**
    Initializes bit set from other bit set.
  */
  BitSet(const BitSet& copy) throw(MemoryException) : elements(copy.elements), size(copy.size) {}

  /**
    Assignment of bit set to bit set.
  */
  BitSet& operator=(const BitSet& eq) throw();

  /**
    Returns the number of bit in the bit set.
  */
  inline unsigned int getSize() const throw() {return size;}

  /**
    Returns true if the bit set is empty.
  */
  inline bool isEmpty() const throw() {return size == 0;}

  /**
    Returns the bit state at the specified index. Throws 'OutOfRange' if the index is invalid.

    @param index The index of the element.
  */
  bool getAt(unsigned int index) const throw(OutOfRange);

  /**
    Sets the bit state at the specified index. Throws 'OutOfRange' if the index is invalid.

    @param index The index of the element.
    @param value The desired value.
  */
  void setAt(unsigned int index, bool value) throw(OutOfRange);

  /**
    Sets (sets to true) all the states of the bit set.
  */
  BitSet& set() throw();

  /**
    Sets (sets to true) the state at the specified index. Throws 'OutOfRange' if the index is invalid.
  */
  BitSet& set(unsigned int index) throw(OutOfRange);

  /**
    Resets (set to false) all the states of the bit set.
  */
  BitSet& reset() throw();

  /**
    Resets (sets to false) the state at the specified index. Throws 'OutOfRange' if the index is invalid.
  */
  BitSet& reset(unsigned int index) throw(OutOfRange);

  /**
    Inverts all the states of the bit set.
  */
  BitSet& flip() throw();

  /**
    Inverts the state at the specified index. Throws 'OutOfRange' if the index is invalid.
  */
  BitSet& flip(unsigned int index) throw(OutOfRange);

  /**
    Binary AND of the bit sets.
  */
  BitSet& operator&=(const BitSet& value) throw();

  /**
    Binary OR of the bit sets.
  */
  BitSet& operator|=(const BitSet& value) throw();

  /**
    Binary EXCLUSIVE OR of the bit sets.
  */
  BitSet& operator^=(const BitSet& value) throw();

  /**
    Binary left shift.

    @param count The number of bits to shift.
  */
  BitSet& operator<<=(unsigned int count) throw();

  /**
    Binary right shift.

    @param count The number of bits to shift.
  */
  BitSet& operator>>=(unsigned int count) throw();

  /**
    Returns an inverted bit set.
  */
  inline BitSet operator~() const throw() {
    return BitSet(*this).flip();
  }

  /**
    Returns a left-shifted bit set.
  */
  inline BitSet operator<<(unsigned int count) const throw() {
    return BitSet(*this) <<= count;
  }

  /**
    Returns a right-shifted bit set.
  */
  inline BitSet operator>>(unsigned int count) const throw() {
    return BitSet(*this) >>= count;
  }

  /**
    Returns reference to the bit at the specified index. Throws 'OutOfRange' if the index is invalid.

    @param index The index of the element.
  */
  inline Reference operator[](unsigned int index) throw(OutOfRange) {
    return Reference(*this, index);
  }

  /**
    Returns the bit at the specified index. Throws 'OutOfRange' if the index is invalid.

    @param index The index of the element.
  */
  inline bool operator[](unsigned int index) const throw(OutOfRange) {
    return getAt(index);
  }

  /**
    Removes all the bits.
  */
  void removeAll() throw();

  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const BitSet& value);
};

/**
  Writes a string representation of a bit set to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const BitSet& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
