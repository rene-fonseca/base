/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__ARRAY_H
#define _DK_SDU_MIP__BASE_COLLECTION__ARRAY_H

#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/collection/Collection.h>
#include <base/collection/Enumeration.h>
#include <base/collection/InvalidEnumeration.h>
#include <base/collection/InvalidNode.h>
#include <base/OutOfRange.h>
#include <base/MemoryException.h>
#include <base/string/FormatOutputStream.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Array. Ordered sequence of elements.

  @short Array
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Array : public Collection {
public:

  /** The type of the values. */
  typedef TYPE Value;

  typedef typename CapacityAllocator<TYPE>::Iterator Iterator;
  typedef typename CapacityAllocator<TYPE>::ReadIterator ReadIterator;
  typedef typename CapacityAllocator<TYPE>::Enumerator Enumerator;
  typedef typename CapacityAllocator<TYPE>::ReadEnumerator ReadEnumerator;

//  class Enumeration;
//  friend class Enumeration;
//  class ReadOnlyEnumeration;
//  friend class ReadOnlyEnumeration;

//  /**
//    Enumeration of all the elements of an array.
//  */
//  class Enumeration : public AllocatorEnumeration<Value, Value&, Value*> {
//  public:
//
//    /**
//      Initializes an enumeration of all the elements of the specified array.
//
//      @param array The array being enumerated.
//    */
//    Enumeration(Array& array) throw() :
//      AllocatorEnumeration<Value, Value&, Value*>(array.getElements(), array.getElements() + array.getSize()) {}
//  };
//
//  /**
//    Non-modifying enumeration of all the elements of an array.
//  */
//  class ReadOnlyEnumeration : public AllocatorEnumeration<Value, const Value&, const Value*> {
//  public:
//
//    /**
//      Initializes a non-modifying enumeration of all the elements of the specified array.
//
//      @param array The array being enumerated.
//    */
//    ReadOnlyEnumeration(const Array& array) throw() :
//      AllocatorEnumeration<Value, const Value&, const Value*>(array.getElements(), array.getElements() + array.getSize()) {}
//  };

  /**
    Reference to an element within an array.
  */
  class Reference {
  private:
    friend class Array;
    Array& array;
    unsigned int index;
    Reference(const Reference& copy); // prohibit default copy initialization
    Reference& operator=(const Reference& eq); // prohibit default assignment
    inline Reference(Array& a, unsigned int i) : array(a), index(i) {}
  public:
    inline Reference& operator=(Value value) throw(OutOfRange) {array.setAt(index, value); return *this;}
    inline operator Value() const throw(OutOfRange) {return array.getAt(index);}
  };
private:

  /** The elements of the array. */
  ReferenceCountedObjectPointer<ReferenceCountedCapacityAllocator<Value> > elements;
  /** The number of elements in the array. */
  unsigned int size;
protected:

  /**
    Sets the size of the array.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    if (size != this->size) {
      this->size = size;
      if (elements.isValid()) {
        elements->setSize(size);
      } else {
        elements = new ReferenceCountedCapacityAllocator<Value>(size);
      }
    }
  }
public:

  /**
    Returns the elements of the array for modifying access.
  */
  inline Value* getElements() throw(MemoryException) {
    elements.copyOnWrite();
    return elements->getElements();
  }

  /**
    Returns the elements of the array for non-modifying access.
  */
  inline const Value* getElements() const throw() {
    return elements->getElements();
  }

  /**
    Initializes an empty array.
  */
  Array() throw() : elements(new ReferenceCountedCapacityAllocator<Value>()), size(0) {}

  /**
    Initializes an empty array with the specified granularity.

    @param granularity The desired granularity.
  */
  explicit Array(unsigned int granularity) throw() : elements(new ReferenceCountedCapacityAllocator<Value>(granularity)), size(0) {}

  /**
    Initializes array with the specified number of elements.

    @param size The initial number of elements in the array.
    @param value The value used to initialize the elements.
    @param granularity The desired granularity. Default is ReferenceCountedCapacityAllocator<Value>::DEFAULT_GRANULARITY.
  */
  Array(unsigned int size, Value value, unsigned int granularity = ReferenceCountedCapacityAllocator<Value>::DEFAULT_GRANULARITY) throw() :
    elements(new ReferenceCountedCapacityAllocator<Value>(size, granularity)), size(size) {
    fill(getElements(), size, value);
  }

  /**
    Initializes array from other array.
  */
  Array(const Array& copy) throw(MemoryException) : elements(copy.elements), size(copy.size) {}

  /**
    Assignment of array to array.
  */
  Array& operator=(const Array& eq) throw(MemoryException) {
    if (&eq != this) { // protect against self assignment
      elements = eq.elements;
      size = eq.size;
    }
    return *this;
  }

  /**
    Returns the number fo elements in the array.
  */
  inline unsigned int getSize() const throw() {return size;}

  /**
    Returns true if the array is empty.
  */
  inline bool isEmpty() const throw() {return size == 0;}

  /**
    Returns the first element of the allocator as a modifying array.
  */
  inline Iterator getBeginIterator() throw() {
    return elements->getBeginIterator();
  }

  /**
    Returns the end of the allocator as a modifying array.
  */
  inline Iterator getEndIterator() throw() {
    return elements->getEndIterator();
  }

  /**
    Returns the first element of the allocator as a non-modifying array.
  */
  inline ReadIterator getBeginIterator() const throw() {
    return elements->getBeginIterator();
  }

  /**
    Returns the end of the allocator as a non-modifying array.
  */
  inline ReadIterator getEndIterator() const throw() {
    return elements->getEndIterator();
  }

  /**
    Returns a modifying enumerator of the array.
  */
  inline Enumerator getEnumerator() throw() {
    return elements->getEnumerator();
  }

  /**
    Returns a non-modifying enumerator of the array.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return elements->getReadEnumerator();
  }

  /**
    Appends the value to this array.

    @param value The value to be appended.
  */
  void append(const Value& value) throw(MemoryException) {
    unsigned int size = getSize();
    setSize(size + 1);
    Value* elements = getElements(); // size must be set before
    elements[size] = value;
  }

  /**
    Prepends the value to this array.

    @param value The value to be prepended.
  */
  void prepend(const Value& value) throw(MemoryException) {
    setSize(getSize() + 1);
    Value* elements = getElements(); // size must be set before
    move(elements + 1, elements, getSize());
    elements[0] = value;
  }

  /**
    Inserts the value at the specified position. Throws 'OutOfRange' if the
    specified index is invalid.

    @param index Specifies the insert position.
    @param value The value to be inserted.
  */
  void insert(unsigned int index, const Value& value) throw(OutOfRange, MemoryException) {
    assert(index <= getSize(), OutOfRange());
    setSize(getSize() + 1);
    Value* elements = getElements(); // size must be set before
    move(elements + index + 1, elements + index, getSize() - index);
    elements[index] = value;
  }

  /**
    Remove the element specified by the index. Throws 'OutOfRange' if the index is invalid.

    @param index The index of the element to be removed.
  */
  void remove(unsigned int index) throw(OutOfRange) {
    assert(index < getSize(), OutOfRange());
    Value* elements = getElements(); // size must be set after
    move(elements + index, elements + index + 1, getSize() - index - 1);
    setSize(getSize() - 1);
  }

  /**
    Removes all the elements from this array.
  */
  void removeAll() throw() {
    elements = new ReferenceCountedCapacityAllocator<Value>(); // no need to copy
    size = 0;
  }

  /**
    Returns the element at the specified index. Throws 'OutOfRange' if the index is invalid.

    @param index The index of the element.
  */
  Value getAt(unsigned int index) const throw(OutOfRange) {
    assert(index < getSize(), OutOfRange());
    return getElements()[index];
  }

  /**
    Sets the element at the specified index. Throws 'OutOfRange' if the index is invalid.

    @param index The index of the element.
    @param value The desired value.
  */
  void setAt(unsigned int index, const Value& value) throw(OutOfRange) {
    assert(index < getSize(), OutOfRange());
    getElements()[index] = value;
  }

  /**
    Returns a reference to the element at the specified index. Throws 'OutOfRange' if the index is invalid.

    @param index The index of the element.
  */
  inline Reference operator[](unsigned int index) throw(OutOfRange) {
    return Reference(*this, index);
  }

  /**
    Returns the element at the specified index. Throws 'OutOfRange' if the index is invalid.

    @param index The index of the element.
  */
  inline Value operator[](unsigned int index) const throw(OutOfRange) {
    return getAt(index);
  }
};



/**
  Writes a string representation of an array to a format stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Array<TYPE>& value) {
  Array<TYPE>::ReadEnumerator enu = value.getReadEnumerator();
  stream << '{';
  while (enu.hasNext()) {
    stream << *enu.next();
    if (enu.hasNext()) {
      stream << ';';
    }
  }
  stream << '}';
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
