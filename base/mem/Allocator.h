/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__ALLOCATOR_H
#define _DK_SDU_MIP__BASE_MEM__ALLOCATOR_H

#include <base/Functor.h>
#include <base/mem/Heap.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/iterator/SequenceIterator.h>
#include <new>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Allocator of resizeable memory block. The implementation is not MT-safe.

  @see ReferenceCountedAllocator CapacityAllocator
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.01
*/

template<class TYPE>
class Allocator {
private:

  /** The allocated memory block. */
  TYPE* elements;
  /** The number of elements in the block. */
  unsigned int size;
public:

  typedef SequenceIterator<IteratorTraits<TYPE> > Iterator;
  typedef SequenceIterator<ReadIteratorTraits<TYPE> > ReadIterator;

  typedef AllocatorEnumerator<EnumeratorTraits<TYPE> > Enumerator;
  typedef AllocatorEnumerator<ReadEnumeratorTraits<TYPE> > ReadEnumerator;

  /**
    Initializes the elements of the sequence using the default constructor.
    Relocatable objects are not initialized.
  */
  static inline void initialize(TYPE* element, unsigned int count) throw() {
    if (!Relocateable<TYPE>::IS_RELOCATEABLE) {
      const TYPE* end = element + count;
      --element;
      while (++element != end) {
        new(element) TYPE();
      }
    }
  }

  /**
    Initializes the elements of the sequence by copying elements from other
    sequence. The memory image is copied directly for relocatable objects.
  */
  static inline void initializeByCopy(TYPE* dest, const TYPE* src, unsigned int count) throw() {
    if (Relocateable<TYPE>::IS_RELOCATEABLE) {
      copy<TYPE>(dest, src, count); // blocks do not overlap
    } else {
      const TYPE* end = dest + count;
      while (dest != end) {
        new(dest) TYPE(*src); // copy object
        ++dest;
        ++src;
      }
    }
  }

  /**
    Initializes the elements of the sequence by moving elements from other
    sequence. This does nothing for relocateable object.
  */
  static inline void initializeByMove(TYPE* dest, const TYPE* src, unsigned int count) throw() {
    if (!Relocateable<TYPE>::IS_RELOCATEABLE) {
      const TYPE* end = dest + count;
      while (dest != end) {
        new(dest) TYPE(*src); // copy object
        src->~TYPE(); // destroy old object
        ++dest;
        ++src;
      }
    }
  }

  /**
    Destroys the elements of the sequence. Does nothing for relocateable objects.
  */
  static inline void destroy(TYPE* element, unsigned int count) throw() {
    if (!Relocateable<TYPE>::IS_RELOCATEABLE) { // must we destroy the elements
      const TYPE* end = element + count;
      --element;
      while (++element != end) {
        element->~TYPE();
      }
    }
  }
  
//  /**
//    Enumeration of all the elements of an Allocator.
//  */
//  class Enumerator : public AllocatorEnumerator<EnumeratorTraits<TYPE> > {
//  public:
//
//    /**
//      Initializes an enumeration of all the elements of the specified Allocator.
//
//      @param allocator The Allocator being enumerated.
//    */
//    Enumerator(Allocator& allocator) throw() :
//      AllocatorEnumerator<Traits>(allocator.getElements(), allocator.getElements() + allocator.getSize()) {}
//  };
//
//  /**
//    Non-modifying enumeration of all the elements of an Allocator.
//  */
//  class ReadEnumerator : public AllocatorEnumerator<ReadEnumeratorTraits<TYPE> > {
//  public:
//
//    /**
//      Initializes a non-modifying enumeration of all the elements of the specified Allocator.
//
//      @param allocator The Allocator being enumerated.
//    */
//    ReadEnumerator(const Allocator& allocator) throw() :
//      AllocatorEnumerator<Traits>(allocator.getElements(), allocator.getElements() + allocator.getSize()) {}
//  };
public:

  /**
    Initializes an empty allocator.
  */
  inline explicit Allocator() throw() : elements(0), size(0) {}

  /**
    Initializes an allocator of the specified size without initializing the
    elements. Throws 'MemoryException' if unable to allocate enough memory to
    hold the requested number of elements.

    @param size Specifies the initial size of the allocator.
  */
  explicit Allocator(unsigned int _size) throw(MemoryException)
    : elements(Heap::allocate<TYPE>(_size)), size(_size) {
    initialize(elements, size); // default initialization of elements
  }

  /**
    Initializes allocator from other allocator.
  */
  Allocator(const Allocator& copy) throw(MemoryException)
    : elements(Heap::allocate<TYPE>(copy.size)), size(copy.size) {
    initializeByCopy(elements, copy.elements, size); // initialization of elements by copying
  }

  /**
    Assignment of allocator by allocator.
  */
  Allocator& operator=(const Allocator& eq) throw(MemoryException) {
    if (&eq != this) { // protect against self assignment
      destroy(elements, size);
      size = eq.size;
      elements = Heap::resize(elements, size);
      initializeByCopy(elements, eq.elements, size); // initialization of elements by copying
    }
    return *this;
  }

  /**
    Returns the elements of the allocator for modifying access.
  */
  inline TYPE* getElements() throw() {
    return elements;
  }

  /**
    Returns the elements of the allocator for non-modifying access.
  */
  inline const TYPE* getElements() const throw() {
    return elements;
  }

  /**
    Returns the first element of the allocator as a modifying iterator.
  */
  inline Iterator getBeginIterator() throw() {
    return Iterator(getElements());
  }

  /**
    Returns the end of the allocator as a modifying iterator.
  */
  inline Iterator getEndIterator() throw() {
    return Iterator(getElements() + getSize());
  }

  /**
    Returns the first element of the allocator as a non-modifying iterator.
  */
  inline ReadIterator getBeginReadIterator() const throw() {
    return ReadIterator(getElements());
  }

  /**
    Returns the end of the allocator as a non-modifying iterator.
  */
  inline ReadIterator getEndReadIterator() const throw() {
    return ReadIterator(getElements() + getSize());
  }

  /**
    Returns a modifying enumerator of the allocator.
  */
  inline Enumerator getEnumerator() throw() {
    return Enumerator(getElements(), getElements() + getSize());
  }

  /**
    Returns a non-modifying enumerator of the allocator.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return ReadEnumerator(getElements(), getElements() + getSize());
  }

  /**
    Returns the number of elements of the allocator.
  */
  inline unsigned int getSize() const throw() {
    return size;
  }

  /**
    Returns the number of allocated bytes.
  */
  inline unsigned int getByteSize() const throw() {
    return size * sizeof(TYPE);
  }

  /**
    Returns true if no elements are allocated.
  */
  inline bool isEmpty() const throw() {
    return size == 0;
  }

  /**
    Sets the number of elements of the allocator. If the size is increased the
    original elements are not modified and the newly allocated elements are not
    initialized. Throws 'MemoryException' if unable to allocate additional
    memory (does not throw an exception when the size is decreased or
    unchanged). If the size is reduced the elements up to the new size are
    unchanged.

    @param size The desired size.
  */
  void setSize(unsigned int size) throw(MemoryException) {
    if (size != this->size) {
      if (Relocateable<TYPE>::IS_RELOCATEABLE) {
        // no need to destroy or initialize elements
        elements = Heap::resize(elements, size);
      } else {
        if (size < this->size) { // are we about to reduce the array
          destroy(elements + size, this->size - size);
          elements = Heap::resize(elements, size);
        } else { // array is to be expanded
          TYPE* temp = Heap::allocate<TYPE>(size); // new array
          initializeByMove(temp, elements, this->size);
          Heap::release(elements); // free previous array
          elements = temp;
          initialize(elements + this->size, size - this->size); // default initialization of new objects
          // could be optimized if we use Heap::tryResize - but currently only works for win32
        }
      }
      this->size = size;
    }
  }

  ~Allocator() {
    destroy(elements, size);
    Heap::release(elements);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
