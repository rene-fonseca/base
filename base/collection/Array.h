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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Array. Ordered sequence of elements.

  @short Array
  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class Array : public Collection {
public:

  class Enumeration;
  friend class Enumeration;
  class ReadOnlyEnumeration;
  friend class ReadOnlyEnumeration;

  /**
    Enumeration of all the elements of an array.
  */
  class Enumeration : public AllocatorEnumeration<TYPE, TYPE&, TYPE*> {
  public:

    /**
      Initializes an enumeration of all the elements of the specified array.

      @param array The array being enumerated.
    */
    Enumeration(Array& array) throw() :
      AllocatorEnumeration<TYPE, TYPE&, TYPE*>(array.getElements(), array.getElements() + array.getSize()) {}
  };

  /**
    Non-modifying enumeration of all the elements of an array.
  */
  class ReadOnlyEnumeration : public AllocatorEnumeration<TYPE, const TYPE&, const TYPE*> {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the specified array.

      @param array The array being enumerated.
    */
    ReadOnlyEnumeration(const Array& array) throw() :
      AllocatorEnumeration<TYPE, const TYPE&, const TYPE*>(array.getElements(), array.getElements() + array.getSize()) {}
  };
private:

  /** The elements of the array. */
  ReferenceCountedObjectPointer<ReferenceCountedAllocator<TYPE> > elements;
  /** The number of elements in the array. */
  unsigned int size;
protected:

  /**
    Returns the elements of the array for modifying access.
  */
  inline TYPE* getElements() throw(MemoryException) {
    elements.copyOnWrite();
    return elements->getElements();
  }

  /**
    Returns the elements of the array for non-modifying access.
  */
  inline const TYPE* getElements() const throw() {
    return elements->getElements();
  }

  /**
    Sets the size of the array.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    if (size != this->size) {
      this->size = size;
      if (elements.isValid()) {
        elements->setSize(size);
      } else {
        elements = new ReferenceCountedAllocator<TYPE>(size);
      }
    }
  }
public:

  /**
    Initializes an empty array.
  */
  Array() throw() : elements(new ReferenceCountedAllocator<TYPE>()), size(0) {}

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
    Appends the value to this array.

    @param value The value to be appended.
  */
  void append(const TYPE& value) throw(MemoryException) {
    unsigned int size = getSize();
    setSize(size + 1);
    TYPE* elements = getElements(); // size must be set before
    elements[size] = value;
  }

  /**
    Prepends the value to this array.

    @param value The value to be prepended.
  */
  void prepend(const TYPE& value) throw(MemoryException) {
    setSize(getSize() + 1);
    TYPE* elements = getElements(); // size must be set before
    move(elements + 1, elements, getSize());
    elements[0] = value;
  }

  /**
    Inserts the value at the specified position. Throws 'OutOfRange' if the
    specified index is invalid.

    @param index Specifies the insert position.
    @param value The value to be inserted.
  */
  void insert(unsigned int index, const TYPE& value) throw(OutOfRange, MemoryException) {
    if (index > getSize()) {
      throw OutOfRange();
    }
    setSize(getSize() + 1);
    TYPE* elements = getElements(); // size must be set before
    move(elements + index + 1, elements + index, getSize() - index);
    elements[index] = value;
  }

  /**
    Remove the element specified by the index. Throws 'OutOfRange' if the index is invalid.

    @param index The index of the element to be removed.
  */
  void remove(unsigned int index) throw(OutOfRange) {
    if (index >= getSize()) {
      throw OutOfRange();
    }
    TYPE* elements = getElements(); // size must be set after
    move(elements + index, elements + index + 1, getSize() - index - 1);
    setSize(getSize() - 1);
  }

  /**
    Removes all the elements from this array.
  */
  void removeAll() throw() {
    elements = new ReferenceCountedAllocator<TYPE>(); // no need to copy
    size = 0;
  }
};



/**
  Writes a string representation of an array to a format stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Array<TYPE>& value) {
  Array<TYPE>::ReadOnlyEnumeration enu(value);
  stream << "{";
  while (enu.hasNext()) {
    stream << *enu.next();
    if (enu.hasNext()) {
      stream << ";";
    }
  }
  stream << "}";
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
