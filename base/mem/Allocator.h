/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__ALLOCATOR_H
#define _DK_SDU_MIP__BASE_MEM__ALLOCATOR_H

#include <base/mem/Heap.h>
#include <base/mem/AllocatorEnumeration.h>
#include <base/Functor.h>
#include <base/iterator/SequenceIterator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/** Initializes the elements of the sequence using the default constructor. */
template<class TYPE>
inline void initialize(TYPE* element, unsigned int count) throw() {
  if (!isRelocateable<TYPE>()) {
    const TYPE* end = element + count;
    --element;
    while (++element != end) {
      new(element) TYPE();
    }
  }
}

/** Initializes the elements of the sequence by copying elements from other sequence. */
template<class TYPE>
inline void initializeByCopy(TYPE* dest, const TYPE* src, unsigned int count) throw() {
  if (isRelocateable<TYPE>()) {
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

/** Initializes the elements of the sequence by moving elements from other sequence. */
template<class TYPE>
inline void initializeByMove(TYPE* dest, const TYPE* src, unsigned int count) throw() {
  if (!isRelocateable<TYPE>()) {
    const TYPE* end = dest + count;
    while (dest != end) {
      new(dest) TYPE(*src); // copy object
      src->~TYPE(); // destroy old object
      ++dest;
      ++src;
    }
  }
}

/** Destroys the elements of the sequence. */
template<class TYPE>
inline void destroy(TYPE* element, unsigned int count) throw() {
  if (!isRelocateable<TYPE>()) { // must we destroy the elements
    const TYPE* end = element + count;
    --element;
    while (++element != end) {
      element->~TYPE();
    }
  }
}

/**
  Allocator of resizeable memory block. The implementation is not MT-safe.

  @see ReferenceCountedAllocator CapacityAllocator
  @author René Møller Fonseca
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

  /**
    Enumeration of all the elements of an Allocator.
  */
  class Enumeration : public AllocatorEnumeration<TYPE, TYPE&, TYPE*> {
  public:

    /**
      Initializes an enumeration of all the elements of the specified Allocator.

      @param allocator The Allocator being enumerated.
    */
    Enumeration(Allocator& allocator) throw() :
      AllocatorEnumeration<TYPE, TYPE&, TYPE*>(allocator.getElements(), allocator.getElements() + allocator.getSize()) {}
  };

  /**
    Non-modifying enumeration of all the elements of an Allocator.
  */
  class ReadOnlyEnumeration : public AllocatorEnumeration<TYPE, const TYPE&, const TYPE*> {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the specified Allocator.

      @param allocator The Allocator being enumerated.
    */
    ReadOnlyEnumeration(const Allocator& allocator) throw() :
      AllocatorEnumeration<TYPE, const TYPE&, const TYPE*>(allocator.getElements(), allocator.getElements() + allocator.getSize()) {}
  };
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
  explicit Allocator(unsigned int size) throw(MemoryException) :
    elements(Heap::allocate<TYPE>(size)), size(size) {
    initialize(elements, size); // default initialization of elements
  }

  /**
    Initializes allocator from other allocator.
  */
  Allocator(const Allocator& cpy) throw(MemoryException) :
    elements(Heap::allocate<TYPE>(size)), size(cpy.size) {
    initializeByCopy(elements, cpy.elements, size); // initialization of elements by copying
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
    Returns the first element of the allocator is a modifying iterator.
  */
  inline Iterator getBeginIterator() throw() {
    return Iterator(elements);
  }

  /**
    Returns the end of the allocator is a modifying iterator.
  */
  inline Iterator getEndIterator() throw() {
    return Iterator(elements + size);
  }

  /**
    Returns the first element of the allocator is a non-modifying iterator.
  */
  inline ReadIterator getBeginIterator() const throw() {
    return ReadIterator(elements);
  }

  /**
    Returns the end of the allocator is a non-modifying iterator.
  */
  inline ReadIterator getEndIterator() const throw() {
    return ReadIterator(elements + size);
  }

  /**
    Returns the number of elements of the allocator.
  */
  inline unsigned int getSize() const throw() {
    return size;
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
      if (isRelocateable<TYPE>()) {
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
