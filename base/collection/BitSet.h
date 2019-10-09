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

#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/mem/Reference.h>
#include <base/collection/Collection.h>
#include <base/collection/Enumeration.h>
#include <base/collection/InvalidEnumeration.h>
#include <base/OutOfRange.h>
#include <base/string/FormatOutputStream.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  An ordered sequence of boolean values (bits). The is offen used to represent
  a set of flags. This class does not take up as much memory as Array<bool>.
  
  @short Set of bits collection.
  @ingroup collections
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API BitSet : public Collection {
public:

  class BitSetEnumerator;
  class BitSetReadEnumerator;
  
  /*
    Reference to a bit.
  */
  class BitReference {
    friend class BitSetEnumerator;
  private:
    
    /** The word containing the bit. */
    unsigned long* word = nullptr;
    /** The bit within the word. */
    unsigned long mask = 0;
    
    inline BitReference(unsigned long* _word, unsigned long _mask) throw()
      : word(_word), mask(_mask) {
    }
  public:
    
    inline BitReference(const BitReference& copy) throw()
      : word(copy.word), mask(copy.mask) {
    }
    
    /**
      Assignment of bit.
    */
    inline BitReference& operator=(const BitReference& eq) throw() {
      word = eq.word;
      mask = eq.mask;
      return *this;
    }
    
    /**
      Assignment of bit.
    */
    inline BitReference& operator=(bool value) throw() {
      if (value) {
        *word |= mask;
      } else {
        *word &= ~mask;
      }
      return *this;
    }

    /**
      Logic and operator.
    */
    inline BitReference& operator&=(bool value) throw() {
      if (!value) {
        *word &= ~mask;
      }
      return *this;
    }
    
    /**
      Logic or operator.
    */
    inline BitReference& operator|=(bool value) throw() {
      if (value) {
        *word |= mask;
      }
      return *this;
    }
    
    /**
      Logic xor operator.
    */
    inline BitReference& operator^=(bool value) throw() {
      if (value) {
        *word ^= mask; // flip
      }
      return *this;
    }
    
    /**
      Returns the value of the referenced bit.
    */
    inline operator bool() const throw() {
      return *word & mask;
    }
  };

  /*
    Reference to a bit.
  */
  class BitReadReference {
    friend class BitSetReadEnumerator;
  private:
    
    /** The word containing the bit. */
    const unsigned long* word = nullptr;
    /** The bit within the word. */
    unsigned long mask = 0;
    
    inline BitReadReference(
      const unsigned long* _word, unsigned long _mask) throw()
      : word(_word), mask(_mask) {
    }
  public:
    
    inline BitReadReference(const BitReadReference& copy) throw()
      : word(copy.word), mask(copy.mask) {
    }
    
    /**
      Assignment of bit.
    */
    inline BitReadReference& operator=(const BitReadReference& eq) throw() {
      word = eq.word;
      mask = eq.mask;
      return *this;
    }
    
    /**
      Returns the value of the referenced bit.
    */
    inline operator bool() const throw() {
      return *word & mask;
    }
  };

  /*
    Pointer to a bit.
  */
  class BitPointer {
    friend class BitSetEnumerator;
  private:
    
    /** The bit. */
    BitReference bit;
    
    inline BitPointer(const BitReference& _bit) throw() : bit(_bit) {
    }
  public:
    
    /**
      Returns the reference to the bit.
    */
    inline BitReference operator*() const throw() {
      return bit;
    }
  };
  
  /*
    Pointer to a bit.
  */
  class BitReadPointer {
    friend class BitSetReadEnumerator;
  private:
    
    /** The bit. */
    BitReadReference bit;
    
    inline BitReadPointer(const BitReadReference& _bit) throw() : bit(_bit) {
    }
  public:
    
    /**
      Returns the reference to the bit.
    */
    inline BitReadReference operator*() const throw() {
      return bit;
    }
  };
  
  class EnumeratorTraits {
  public:
    
    typedef EnumeratorTraits SelfEnumeratorTraits;
      
    typedef bool Value;
    typedef const BitReference Reference;
    typedef const BitPointer Pointer;
    typedef unsigned int Distance;
  };

  /**
    Enumerator of elements of a bit set.
    
    @short Non-modifying enumerator of elements of a bit set.
    @version 1.0
  */
  class BitSetEnumerator : public Enumerator<EnumeratorTraits> {
    friend class BitSet;
  protected:
    
    typedef Enumerator<EnumeratorTraits>::Value Value;
    typedef Enumerator<EnumeratorTraits>::Pointer Pointer;
    
    /** The current position in the enumeration. */
    unsigned long* word = nullptr;
    /** The number of bits left. */
    unsigned int count = 0;
    
    /**
      Initializes an enumeration of all the elements of a bit set.
      
      @param word Specifies the beginning of the enumeration.
      @param count The number of bits in the set.
    */
    explicit inline BitSetEnumerator(
      unsigned long* _word, unsigned int _count) throw()
      : word(_word), count(_count) {
    }
  public:
    
    /**
      Initializes enumeration from other enumeration.
    */
    inline BitSetEnumerator(const BitSetEnumerator& copy) throw()
      : word(copy.word), count(copy.count) {
    }
    
    /**
      Returns true if the enumeration still contains elements.
    */
    inline bool hasNext() const throw() {
      return count != 0;
    }
    
    /**
      Returns the next element and advances the position of this enumeration.
    */
    inline Pointer next() throw(EndOfEnumeration) {
      bassert(count != 0, EndOfEnumeration());
      if (count % (sizeof(unsigned long) * 8) == 0) {
        --word;
      }
      --count;
      return BitPointer(
        BitReference(word, 1UL << (count % (sizeof(unsigned long) * 8)))
      );
    }
    
    /**
      Returns true if the enumerations are pointing to the same position.
    */
    inline bool operator==(const BitSetEnumerator& eq) const throw() {
      return (word == eq.word) && (count == eq.count);
    }
    
    /**
      Returns true if the enumerations aren't pointing to the same position.
    */
    inline bool operator!=(const BitSetEnumerator& eq) const throw() {
      return (word != eq.word) || (count != eq.count);
    }
  };
  
  class ReadEnumeratorTraits {
  public:
    
    typedef ReadEnumeratorTraits SelfEnumeratorTraits;
      
    typedef bool Value;
    typedef BitReadReference Reference;
    typedef BitReadPointer Pointer;
    typedef unsigned int Distance;
  };
  
  /**
    Enumerator of elements of a bit set.
    
    @short Non-modifying enumerator of elements of a bit set.
    @version 1.0
  */
  class BitSetReadEnumerator : public Enumerator<ReadEnumeratorTraits> {
    friend class BitSet;
  protected:
    
    typedef Enumerator<ReadEnumeratorTraits>::Value Value;
    typedef Enumerator<ReadEnumeratorTraits>::Pointer Pointer;
    
    /** The current position in the enumeration. */
    const unsigned long* word = nullptr;
    /** The number of bits left. */
    unsigned int count = 0;
    
    /**
      Initializes an enumeration of all the elements of a bit set.
      
      @param word Specifies the beginning of the enumeration.
      @param count The number of bits in the set.
    */
    explicit inline BitSetReadEnumerator(
      const unsigned long* _word, unsigned int _count) throw()
      : word(_word), count(_count) {
    }
  public:
    
    /**
      Initializes enumeration from other enumeration.
    */
    inline BitSetReadEnumerator(const BitSetReadEnumerator& copy) throw()
      : word(copy.word), count(copy.count) {
    }
    
    /**
      Returns true if the enumeration still contains elements.
    */
    inline bool hasNext() const throw() {
      return count != 0;
    }
    
    /**
      Returns the next element and advances the position of this enumeration.
    */
    inline Pointer next() throw(EndOfEnumeration) {
      bassert(count != 0, EndOfEnumeration());
      if (count % (sizeof(unsigned long) * 8) == 0) {
        --word;
      }
      --count;
      return BitReadPointer(
        BitReadReference(word, 1UL << (count % (sizeof(unsigned long) * 8)))
      );
    }
    
    /**
      Returns true if the enumerations are pointing to the same position.
    */
    inline bool operator==(const BitSetReadEnumerator& eq) const throw() {
      return (word == eq.word) && (count == eq.count);
    }
    
    /**
      Returns true if the enumerations aren't pointing to the same position.
    */
    inline bool operator!=(const BitSetReadEnumerator& eq) const throw() {
      return (word != eq.word) || (count != eq.count);
    }
  };

  typedef BitSetEnumerator Enumerator;
  typedef BitSetReadEnumerator ReadEnumerator;
  
  /*
    Reference to a single bit within a BitSet.
  */
  class Element {
    friend class BitSet;
  private:
    
    BitSet& bitset; // use reference to avoid 'copy on write'
    unsigned int index = 0;
    
    Element(const Element& copy) throw();
    Element& operator=(const Element& eq) throw();
    
    inline Element(BitSet& _bitset, unsigned int _index)
      : bitset(_bitset),
        index(_index) {
    }
  public:
    
    inline Element& operator=(bool value) throw(OutOfRange) {
      bitset.setAt(index, value);
      return *this;
    }
    
    inline operator bool() throw(OutOfRange) {
      return bitset.getAt(index);
    }
  };
private:

  /** The elements of the bit set. */
  Reference<ReferenceCountedCapacityAllocator<unsigned long> > elements;
  /** The number of bits in the bit set. */
  unsigned int size = 0;
protected:
  
  /**
    Returns the number of required elements to hold the specified number of
    bits.
  */
  static inline unsigned int getNumberOfElements(unsigned int size) throw() {
    return (size + sizeof(unsigned long) * 8 - 1)/(sizeof(unsigned long) * 8);
  }
  
  /**
    Returns the index of the internal element holding the bit at the specified
    index.
  */
  static inline unsigned int getElementIndex(unsigned int index) throw() {
    return index/(sizeof(unsigned long) * 8);
  }
  
  /**
    Returns the bit mask for the specified index.
  */
  static inline unsigned long getBitMask(unsigned int index) throw() {
    return 1UL << (index % (sizeof(unsigned long) * 8));
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
  BitSet() throw(MemoryException);
  
  /**
    Initializes array with the specified number of elements.

    @param size The initial number of bits.
    @param value The initial state of the bits.
  */
  BitSet(unsigned int size, bool value) throw(MemoryException);
  
  /**
    Initializes bit set from other bit set.
  */
  inline BitSet(const BitSet& copy) throw()
    : elements(copy.elements), size(copy.size) {
  }

  /**
    Assignment of bit set to bit set.
  */
  inline BitSet& operator=(const BitSet& eq) throw() {
    elements = eq.elements;
    size = eq.size;
    return *this;
  }

  /**
    Returns the number of bit in the bit set.
  */
  inline unsigned int getSize() const throw() {
    return size;
  }

  /**
    Returns true if the bit set is empty.
  */
  inline bool isEmpty() const throw() {
    return size == 0;
  }

  /**
    Returns the bit state at the specified index. Raises OutOfRange if the
    index is invalid.
    
    @param index The index of the element.
  */
  bool getAt(unsigned int index) const throw(OutOfRange);

  /**
    Sets the bit state at the specified index. Raises OutOfRange if the index
    is invalid.
    
    @param index The index of the element.
    @param value The desired value.
  */
  void setAt(unsigned int index, bool value) throw(OutOfRange);

  /**
    Sets (sets to true) all the states of the bit set.
  */
  BitSet& set() throw();

  /**
    Sets (sets to true) the state at the specified index. Raises OutOfRange if
    the index is invalid.
  */
  BitSet& set(unsigned int index) throw(OutOfRange);

  /**
    Resets (set to false) all the states of the bit set.
  */
  BitSet& reset() throw();

  /**
    Resets (sets to false) the state at the specified index. Raises OutOfRange
    if the index is invalid.
  */
  BitSet& reset(unsigned int index) throw(OutOfRange);

  /**
    Inverts all the states of the bit set.
  */
  BitSet& flip() throw();

  /**
    Inverts the state at the specified index. Raises OutOfRange if the index is
    invalid.
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
    Returns reference to the bit at the specified index. Raises OutOfRange if
    the index is invalid.

    @param index The index of the element.
  */
  inline Element operator[](unsigned int index) throw(OutOfRange) {
    return Element(*this, index);
  }

  /**
    Returns the bit at the specified index. Raises OutOfRange if the index is
    invalid.

    @param index The index of the element.
  */
  inline bool operator[](unsigned int index) const throw(OutOfRange) {
    return getAt(index);
  }

  /**
    Returns a modifying enumerator of the bit set. The elements are
    enumerated from most significant to the least significant.
  */
  inline Enumerator getEnumerator() throw() {
    return Enumerator(
      elements->getElements() + size/(sizeof(unsigned long) * 8),
      size
    );
  }
  
  /**
    Returns a non-modifying enumerator of the bit set. The elements are
    enumerated from most significant to the least significant.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return ReadEnumerator(
      elements->getElements() + size/(sizeof(unsigned long) * 8),
      size
    );
  }
  
  /**
    Removes all the bits.
  */
  void removeAll() throw();

  friend FormatOutputStream& operator<<(
    FormatOutputStream& stream, const BitSet& value) throw(IOException);
};

/**
  Writes a string representation of a bit set to a format stream.
  
  @relates BitSet
*/
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const BitSet& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
