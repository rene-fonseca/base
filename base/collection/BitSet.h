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

#include <base/mem/ReferenceCountedAllocator.h>
#include <base/mem/Reference.h>
#include <base/collection/Collection.h>
#include <base/collection/Enumeration.h>
#include <base/collection/InvalidEnumeration.h>
#include <base/OutOfRange.h>
#include <base/string/FormatOutputStream.h>
#include <base/Functor.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  An ordered sequence of boolean values (bits). The is offen used to represent
  a set of flags. This class does not take up as much memory as Array<bool>.
  
  @short Set of bits collection.
  @ingroup collections
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API BitSet : public Collection {
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
    
    inline BitReference(unsigned long* _word, unsigned long _mask) noexcept
      : word(_word), mask(_mask)
    {
    }
  public:
    
    inline BitReference() noexcept
    {
    }
    
    inline BitReference(const BitReference& copy) noexcept
      : word(copy.word), mask(copy.mask)
    {
    }
    
    /**
      Assignment of bit.
    */
    inline BitReference& operator=(const BitReference& assign) noexcept
    {
      word = assign.word;
      mask = assign.mask;
      return *this;
    }
    
    /**
      Assignment of bit.
    */
    inline BitReference& operator=(bool value) noexcept
    {
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
    inline BitReference& operator&=(bool value) noexcept
    {
      if (!value) {
        *word &= ~mask;
      }
      return *this;
    }
    
    /**
      Logic or operator.
    */
    inline BitReference& operator|=(bool value) noexcept
    {
      if (value) {
        *word |= mask;
      }
      return *this;
    }
    
    /**
      Logic xor operator.
    */
    inline BitReference& operator^=(bool value) noexcept
    {
      if (value) {
        *word ^= mask; // flip
      }
      return *this;
    }
    
    /**
      Returns the value of the referenced bit.
    */
    inline operator bool() const noexcept
    {
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
    
    inline BitReadReference(const unsigned long* _word, unsigned long _mask) noexcept
      : word(_word), mask(_mask)
    {
    }
  public:
    
    inline BitReadReference() noexcept
    {
    }

    inline BitReadReference(const BitReadReference& copy) noexcept
      : word(copy.word), mask(copy.mask)
    {
    }
    
    /**
      Assignment of bit.
    */
    inline BitReadReference& operator=(const BitReadReference& assign) noexcept
    {
      word = assign.word;
      mask = assign.mask;
      return *this;
    }
    
    /**
      Returns the value of the referenced bit.
    */
    inline operator bool() const noexcept
    {
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
    
    inline BitPointer(const BitReference& _bit) noexcept
      : bit(_bit)
    {
    }
  public:
    
    /**
      Returns the reference to the bit.
    */
    inline BitReference operator*() const noexcept
    {
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
    
    inline BitReadPointer(const BitReadReference& _bit) noexcept
      : bit(_bit)
    {
    }
  public:
    
    /**
      Returns the reference to the bit.
    */
    inline BitReadReference operator*() const noexcept
    {
      return bit;
    }
  };
  
  class EnumeratorTraits {
  public:
    
    typedef EnumeratorTraits SelfEnumeratorTraits;
      
    typedef BitReference Value;
    typedef BitReference Reference;
    typedef BitPointer Pointer;
    typedef MemoryDiff Distance;
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
    typedef Enumerator<EnumeratorTraits>::Reference EnumReference;
    typedef Enumerator<EnumeratorTraits>::Pointer Pointer;
    
    /** The current position in the enumeration. */
    unsigned long* word = nullptr;
    /** The number of bits left. */
    unsigned int count = 0;
    /** Current position. */
    EnumReference current;
    
    /**
      Initializes an enumeration of all the elements of a bit set.
      
      @param word Specifies the beginning of the enumeration.
      @param count The number of bits in the set.
    */
    explicit inline BitSetEnumerator(
      unsigned long* _word, unsigned int _count) noexcept
      : word(_word), count(_count)
    {
    }
  public:
    
    /**
      Initializes enumeration from other enumeration.
    */
    inline BitSetEnumerator(const BitSetEnumerator& copy) noexcept
      : word(copy.word), count(copy.count)
    {
    }
    
    /**
      Returns true if the enumeration still contains elements.
    */
    inline bool hasNext() const noexcept
    {
      return count != 0;
    }
    
    /**
      Returns the next element and advances the position of this enumeration.
    */
    inline EnumReference next()
    {
      if (count == 0) {
        _throw EndOfEnumeration();
      }
      if (count % (sizeof(unsigned long) * 8) == 0) {
        --word;
      }
      --count;

      // update current so we can return by reference
      current = BitReference(word, 1UL << (count % (sizeof(unsigned long) * 8)));
      return current;
    }
    
    /**
      Returns true if the enumerations are pointing to the same position.
    */
    inline bool operator==(const BitSetEnumerator& compare) const noexcept
    {
      return (word == compare.word) && (count == compare.count);
    }
    
    /**
      Returns true if the enumerations aren't pointing to the same position.
    */
    inline bool operator!=(const BitSetEnumerator& compare) const noexcept
    {
      return (word != compare.word) || (count != compare.count);
    }
  };
  
  class ReadEnumeratorTraits {
  public:
    
    typedef ReadEnumeratorTraits SelfEnumeratorTraits;
      
    typedef bool Value;
    typedef BitReadReference Reference;
    typedef BitReadPointer Pointer;
    typedef MemoryDiff Distance;
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
    typedef Enumerator<ReadEnumeratorTraits>::Reference EnumReference;
    typedef Enumerator<ReadEnumeratorTraits>::Pointer Pointer;
    
    /** The current position in the enumeration. */
    const unsigned long* word = nullptr;
    /** The number of bits left. */
    MemorySize count = 0;
    /** Current position. */
    EnumReference current;
    
    /**
      Initializes an enumeration of all the elements of a bit set.
      
      @param word Specifies the beginning of the enumeration.
      @param count The number of bits in the set.
    */
    explicit inline BitSetReadEnumerator(
      const unsigned long* _word, MemorySize _count) noexcept
      : word(_word), count(_count)
    {
    }
  public:
    
    /**
      Initializes enumeration from other enumeration.
    */
    inline BitSetReadEnumerator(const BitSetReadEnumerator& copy) noexcept
      : word(copy.word), count(copy.count)
    {
    }
    
    /**
      Returns true if the enumeration still contains elements.
    */
    inline bool hasNext() const noexcept
    {
      return count != 0;
    }
    
    /**
      Returns the next element and advances the position of this enumeration.
    */
    inline EnumReference next()
    {
      if (count == 0) {
        _throw EndOfEnumeration();
      }
      if (count % (sizeof(unsigned long) * 8) == 0) {
        --word;
      }
      --count;
      current = BitReadReference(word, 1UL << (count % (sizeof(unsigned long) * 8)));
      return current;
    }
    
    /**
      Returns true if the enumerations are pointing to the same position.
    */
    inline bool operator==(const BitSetReadEnumerator& compare) const noexcept
    {
      return (word == compare.word) && (count == compare.count);
    }
    
    /**
      Returns true if the enumerations aren't pointing to the same position.
    */
    inline bool operator!=(const BitSetReadEnumerator& compare) const noexcept
    {
      return (word != compare.word) || (count != compare.count);
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
    MemorySize index = 0;
    
    Element(const Element& copy) noexcept;
    Element& operator=(const Element& assign) noexcept;
    
    inline Element(BitSet& _bitset, MemorySize _index)
      : bitset(_bitset),
        index(_index)
    {
    }
  public:
    
    inline Element& operator=(bool value)
    {
      bitset.setAt(index, value);
      return *this;
    }
    
    inline operator bool()
    {
      return bitset.getAt(index);
    }
  };
private:

  /** The elements of the bit set. */
  Reference<ReferenceCountedAllocator<unsigned long> > elements;
  /** The number of bits in the bit set. */
  MemorySize size = 0;
protected:
  
  /**
    Returns the number of required elements to hold the specified number of
    bits.
  */
  static inline MemorySize getNumberOfElements(MemorySize size) noexcept
  {
    return (size + sizeof(unsigned long) * 8 - 1)/(sizeof(unsigned long) * 8);
  }
  
  /**
    Returns the index of the internal element holding the bit at the specified
    index.
  */
  static inline unsigned int getElementIndex(unsigned int index) noexcept
  {
    return index/(sizeof(unsigned long) * 8);
  }
  
  /**
    Returns the bit mask for the specified index.
  */
  static inline unsigned long getBitMask(unsigned int index) noexcept
  {
    return 1UL << (index % (sizeof(unsigned long) * 8));
  }
  
  /**
    Returns the elements of the internal array for modifying access.
  */
  inline unsigned long* getElements()
  {
    elements.copyOnWrite();
    return elements->getElements();
  }
  
  /**
    Returns the elements of the internal array for non-modifying access.
  */
  inline const unsigned long* getElements() const noexcept
  {
    return elements->getElements();
  }
  
  /**
    Zero-extends the bit set to the specified size.
  */
  void zeroExtend(MemorySize size);
  
  /**
    Resets any unused bits.
  */
  inline void reinitialize() noexcept
  {
    unsigned long mask = getBitMask(size) - 1; // TAG: big endian problem
    if ((size > 0) && (mask != 0)) { // are some of the bits unused
      getElements()[getNumberOfElements(size) - 1] &= mask; // reset unused bits
    }
  }
  
  /**
    Sets the number of bits in the bit set.
  */
  inline void setSize(MemorySize size)
  {
    elements->setSize(getNumberOfElements(size));
    reinitialize();
  }
public:

  /**
    Initializes an empty bit set.
  */
  BitSet();
  
  /**
    Initializes array with the specified number of elements.

    @param size The initial number of bits.
    @param value The initial state of the bits.
  */
  BitSet(unsigned int size, bool value);
  
  /**
    Initializes bit set from other bit set.
  */
  inline BitSet(const BitSet& copy) noexcept
    : elements(copy.elements), size(copy.size)
  {
  }

  /**
    Assignment of bit set to bit set.
  */
  inline BitSet& operator=(const BitSet& assign) noexcept
  {
    elements = assign.elements;
    size = assign.size;
    return *this;
  }

  /**
    Returns the number of bit in the bit set.
  */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  /**
    Returns true if the bit set is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return size == 0;
  }

  /**
    Returns the bit state at the specified index. Raises OutOfRange if the
    index is invalid.
    
    @param index The index of the element.
  */
  bool getAt(MemorySize index) const;

  /**
    Sets the bit state at the specified index. Raises OutOfRange if the index
    is invalid.
    
    @param index The index of the element.
    @param value The desired value.
  */
  void setAt(MemorySize index, bool value);

  /**
    Sets (sets to true) all the states of the bit set.
  */
  BitSet& set() noexcept;

  /**
    Sets (sets to true) the state at the specified index. Raises OutOfRange if
    the index is invalid.
  */
  BitSet& set(unsigned int index);

  /**
    Resets (set to false) all the states of the bit set.
  */
  BitSet& reset() noexcept;

  /**
    Resets (sets to false) the state at the specified index. Raises OutOfRange
    if the index is invalid.
  */
  BitSet& reset(MemorySize index);

  /**
    Inverts all the states of the bit set.
  */
  BitSet& flip() noexcept;

  /**
    Inverts the state at the specified index. Raises OutOfRange if the index is
    invalid.
  */
  BitSet& flip(MemorySize index);

  /**
    Binary AND of the bit sets.
  */
  BitSet& operator&=(const BitSet& value) noexcept;

  /**
    Binary OR of the bit sets.
  */
  BitSet& operator|=(const BitSet& value) noexcept;

  /**
    Binary EXCLUSIVE OR of the bit sets.
  */
  BitSet& operator^=(const BitSet& value) noexcept;

  /**
    Binary left shift.

    @param count The number of bits to shift.
  */
  BitSet& operator<<=(unsigned int count) noexcept;

  /**
    Binary right shift.

    @param count The number of bits to shift.
  */
  BitSet& operator>>=(unsigned int count) noexcept;

  /**
    Returns an inverted bit set.
  */
  inline BitSet operator~() const noexcept
  {
    return BitSet(*this).flip();
  }

  /**
    Returns a left-shifted bit set.
  */
  inline BitSet operator<<(unsigned int count) const noexcept
  {
    return BitSet(*this) <<= count;
  }

  /**
    Returns a right-shifted bit set.
  */
  inline BitSet operator>>(unsigned int count) const noexcept
  {
    return BitSet(*this) >>= count;
  }

  /**
    Returns reference to the bit at the specified index. Raises OutOfRange if
    the index is invalid.

    @param index The index of the element.
  */
  inline Element operator[](MemorySize index)
  {
    return Element(*this, index);
  }

  /**
    Returns the bit at the specified index. Raises OutOfRange if the index is
    invalid.

    @param index The index of the element.
  */
  inline bool operator[](MemorySize index) const
  {
    return getAt(index);
  }

  /**
    Returns a modifying enumerator of the bit set. The elements are
    enumerated from most significant to the least significant.
  */
  inline Enumerator getEnumerator() noexcept
  {
    return Enumerator(
      elements->getElements() + size/(sizeof(unsigned long) * 8),
      size
    );
  }
  
  /**
    Returns a non-modifying enumerator of the bit set. The elements are
    enumerated from most significant to the least significant.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(
      elements->getElements() + size/(sizeof(unsigned long) * 8),
      size
    );
  }
  
  /**
    Removes all the bits.
  */
  void removeAll() noexcept;

  /** Returns true if bitset is non-empty. */
  inline operator bool() noexcept
  {
    return size != 0;
  }

  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const BitSet& value);
};

/**
  Writes a string representation of a bit set to a format stream.
  
  @relates BitSet
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const BitSet& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
