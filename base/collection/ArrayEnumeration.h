/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__ARRAY_H
#define _DK_SDU_MIP__BASE_COLLECTION__ARRAY_H

#include <base/mem/ReferenceCountedAllocator.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/collection/Collection.h>
#include <base/collection/Enumeration.h>
#include <base/collection/InvalidEnumeration.h>
#include <base/collection/InvalidNode.h>
#include <base/OutOfRange.h>
#include <base/MemoryException.h>
#include <base/string/FormatOutputStream.h>
#include <base/Functor.h>

template<class TYPE, class REF, class PTR> class ArrayEnumeration;

/**
  Array. Ordered sequence of elements.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class Array : public Collection {
public:

  /** . */
  typedef AllocatorEnumeration<TYPE, TYPE&, TYPE*> Enumeration;
  /** . */
  typedef AllocatorEnumeration<TYPE, const TYPE&, const TYPE*> ReadOnlyEnumeration;

  friend Enumeration;
  friend ReadOnlyEnumeration;
protected:

  /** The elements of the array. */
  ReferenceCountedObjectPointer<ReferenceCountedAllocator<TYPE> > elements;

  /**
    Returns the elements of the array for modifying access.
  */
  inline TYPE* getElements() throw(MemoryException) {
    if (elements.isMultiReferenced()) { // do we have the elements for our self
      elements = new ReferenceCountedAllocator<TYPE>(*elements); // make copy of the elements
    }
    return elements->getElements();
  }

  /**
    Returns the elements of the array for non-modifying access.
  */
  inline const TYPE* getElements() const throw() {
    return elements->getElements();
  }

  /**
    Sets the size of the array. Not fully implemented.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    if (size != this->size) {
      this->size = size;
      if (elements.isNULL()) {
        elements = new ReferenceCountedAllocator<TYPE>(size);
      } else {
        elements->setSize(size);
      }
    }
  }
public:

  /**
    Initializes an empty array.
  */
  Array() throw() {}

  /**
    Initializes array from other array.
  */
  Array(const Array& copy) throw(MemoryException) : elements(copy.elements) {}

  /**
    Appends the value to this array.
  */
  void append(const TYPE& value) throw() {
    TYPE* elements = getElements();
    unsigned int size = getSize();
    setSize(size + 1);
    elements[size] = value;
  }

  /**
    Prepends the value to this array.
  */
  void prepend(const TYPE& value) throw() {
    TYPE* elements = getElements();
    setSize(getSize() + 1);
    copy(elements + 1, elements, getSize());
    elements[0] = value;
  }

  /**
    Inserts the value at the specified position. Throws 'OutOfRange' if the
    specified index is invalid.

    @param index Specifies the insert position.
    @param value The value to be inserted.
  */
  void insert(unsigned int index, const TYPE& value) throw(OutOfRange) {
    if (index > getSize()) {
      throw OutOfRange();
    }
    TYPE* elements = getElements();
    setSize(getSize() + 1);
    copy(elements + index + 1, elements + index, getSize() - index);
    elements[index] = value;
  }

  /**
    Remove the element specified by the index.

    @param index The index of the element to be removed.
  */
  void remove(unsigned int index) throw(OutOfRange) {
    if (index >= getSize()) {
      throw OutOfRange();
    }
    TYPE* elements = getElements();
    copy(elements + index, elements + index + 1, getSize() - index - 1);
    setSize(getSize() - 1);
  }

  /**
    Removes all the elements from this array.
  */
  void removeAll() throw() {
    elements = 0;
  }

  // Friend section

  /**
    Writes a string representation of an array to a format stream.
  */
  friend FormatOutputStream& operator<< <>(FormatOutputStream& stream, const Array<TYPE>& value);
};



  /**
    Enumeration of elements in an array.
  */
  template<class TYPE, class REF, class PTR>
  class ArrayEnumeration : public Enumeration<TYPE, REF, PTR> {
  public:

    typedef TYPE Value;
    typedef REF Reference;
    typedef PTR Pointer;
  protected:

    /** The current position of the enumeration. */
    Pointer current;
    /** The end of the enumeration. */
    Pointer end;
  public:

    /**
      Initializes an enumeration of the specified array.
    */
    inline ArrayEnumeration(Array<TYPE>& array) throw() {
      current = array.getElements();
      end = current + array.getSize();
    }

    /**
      Initializes an enumeration from another enumeration.
    */
    inline ArrayEnumeration(const ArrayEnumeration& copy) throw() : current(copy.current), end(copy.end) {}

    /**
      Returns true if there is more elements in this enumeration.
    */
    inline bool hasNext() const throw() {
      return current < end;
    }

    /**
      Returns the current value and increments the position. Throws
      'EndOfEnumeration' if the end has been reached.
    */
    inline Pointer next() throw(EndOfEnumeration) {
      if (current >= end) {
        throw EndOfEnumeration();
      }
      return current++;
    }

    /**
      Returns true if the enumerations are pointing to the same position.
    */
    inline bool operator==(const ArrayEnumeration& eq) const throw() {
      return current == eq.current;
    }

    /**
      Returns true if the enumerations aren't pointing to the same position.
    */
    inline bool operator!=(const ArrayEnumeration& eq) const throw() {
      return current != eq.current;
    }
  };

/**
  Writes a string representation of an array to a format stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Array<TYPE>& value) {
  Array<TYPE>::ReadOnlyEnumeration enu = value.getReadOnlyEnumeration();
/*  stream.list.begin();
  while (enu.hasNext()) {
    stream << enu.next();
    if (enu.hasNext()) {
      stream.list.separate();
    }
  }
  stream.list.end();*/
  return stream;
}

#endif
