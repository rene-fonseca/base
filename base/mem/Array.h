/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__ARRAY_H
#define _DK_SDU_MIP__BASE_MEM__ARRAY_H

#include <base/Object.h>
#include <base/collection/Enumeration.h>
#include <base/MemoryException.h>
#include <base/mem/Allocator.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/string/FormatOutputStream.h>
#include <string.h>

/**
  Copies elements from array to other array. The arrays may not overlap.
*/
template<class TYPE>
inline void copyArray(TYPE* dest, const TYPE* src, unsigned int count) throw() {
  ::memcpy(dest, src, count * sizeof(TYPE));
}

/**
  Copies elements from array to array. The arrays are allowed to overlap.
*/
template<class TYPE>
inline void moveArray(TYPE* dest, const TYPE* src, unsigned int count) throw() {
  ::memmove(dest, src, count * sizeof(TYPE));
};

/**
  Writes a string representation of an array to a format stream.
*/
template<class TYPE>
FormatOutputStream& toFormatStream(FormatOutputStream& stream, const TYPE* value, unsigned int size);



template<class TYPE> class ArrayEnumeration;

/**
  This class is a reference counted, low-level, and resizeable implementation
  of an array of elements. The implementation is not MT-safe.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class Array : public ReferenceCountedObject, private Allocator {
protected:

  /** The number of elements in the array. */
  unsigned int size;
private:

  Array& operator=(const Array& eq); // prohibit assignment
public:

  /**
    Initializes array of the specified size but does not initialize the
    elements. Throws 'MemoryException' if unable to allocate the required
    memory.

    @param size Specifies the initial size of the array. Default is 0.
  */
  inline explicit Array(unsigned int size = 0) throw(MemoryException) : Allocator(size * sizeof(TYPE)) {}

  /**
    Initializes the array by copying the from the specified array. Throws
    'MemoryException' if unable to allocate the required memory.
  */
  Array(const Array& copy) throw(MemoryException) : Allocator(copy), size(copy.size) {}

  /**
    Returns the elements of the array.
  */
  inline TYPE* getElements() throw() {
    return (TYPE*)Allocator::getMemory();
  }

  /**
    Returns the elements of the array.
  */
  inline const TYPE* getElements() const throw() {
    return (TYPE*)Allocator::getMemory();
  }

  /**
    Returns the number of elements in the array.
  */
  inline unsigned int getSize() const throw() {
    return size;
  }

  /**
    Sets the number of elements in the array. The original elements are not
    changed if the size is increased. Throws 'MemoryException' if unable to
    adjust the size of the array.

    @param size The desired size.
  */
  inline void setSize(unsigned int size) throw(MemoryException) {
    if (size != this->size) {
      this->size = size;
      Allocator::setSize(size * sizeof(TYPE));
    }
  }

  /**
    Returns an enumeration of all the elements in this array.
  */
  inline ArrayEnumeration<TYPE> getEnumeration() throw() {
    return ArrayEnumeration<TYPE>(*this);
  }

  /**
    Returns an enumeration of all the elements in this array.
  */
  inline ArrayEnumeration<const TYPE> getReadOnlyEnumeration() const throw() {
    return ArrayEnumeration<const TYPE>(*this);
  }
};



/**
  Enumerates the elements of an array in forward order.

  @short Enumeration of elements of array.
  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class ArrayEnumeration : public Object, public Enumeration<TYPE> {
protected:
  TYPE* current;
  const TYPE* end;
public:

  /**
    Initializes enumeration of all the elements of the specified array.

    @param array The array to enumerate.
  */
  inline ArrayEnumeration(Array<TYPE>& array) throw() {
    current = array.getElements();
    end = current + array.getSize();
  }

  /**
    Initializes enumeration of all the elements from the specified index of
    the specified array.

    @param array The array to enumerate.
    @param first The index of the first element to be enumerated.
  */
  inline ArrayEnumeration(Array<TYPE>& array, unsigned int first) throw() {
    current = array.getElements();
    end = current + array.getSize();
    current += first;
  }

  /**
    Initializes enumeration of all the elements in the specified subsection of
    the specified array.

    @param array The array to enumerate.
    @param first The index of the first element to be enumerated.
    @param end The index after the last element to be enumerated.
  */
  inline ArrayEnumeration(Array<TYPE>& array, unsigned int first, unsigned int end) throw() {
    current = array.getElements();
    end = current + min(array.getSize(), end);
    current += first;
  }

  /**
    Initializes enumeration from other enumeration.
  */
  inline ArrayEnumeration(const ArrayEnumeration& copy) throw() : current(copy.current), end(copy.end) {}

  /**
    Assignment of enumeration by enumeration.
  */
/*  ArrayEnumeration& operator=(const ArrayEnumeration& eq) throw() {
    current = eq.current;
    end = eq.end;
  }*/

  /**
    Returns true if not all the elements have been enumerated.
  */
  inline bool hasNext() const throw() {
    return current < end;
  }

  /**
    Returns the next element in the enumeration. Returns NULL if there is no
    more elements.
  */
  inline TYPE* const next() throw() {
    return (current < end) ? current++ : NULL;
  }
};



template<class TYPE>
class ArrayEnumeration<const TYPE> : public Object, public Enumeration<const TYPE> {
protected:
  const TYPE* current;
  const TYPE* end;
public:

  /**
    Initializes enumeration of all the elements of the specified array.

    @param array The array to enumerate.
  */
  inline ArrayEnumeration(const Array<TYPE>& array) throw() {
    current = array.getElements();
    end = current + array.getSize();
  }

  /**
    Initializes enumeration of all the elements from the specified index of
    the specified array.

    @param array The array to enumerate.
    @param first The index of the first element to be enumerated.
  */
  inline ArrayEnumeration(const Array<TYPE>& array, unsigned int first) throw() {
    current = array.getElements();
    end = current + array.getSize();
    current += first;
  }

  /**
    Initializes enumeration of all the elements in the specified subsection of
    the specified array.

    @param array The array to enumerate.
    @param first The index of the first element to be enumerated.
    @param end The index after the last element to be enumerated.
  */
  inline ArrayEnumeration(const Array<TYPE>& array, unsigned int first, unsigned int end) throw() {
    current = array.getElements();
    end = current + min(array.getSize(), end);
    current += first;
  }

  /**
    Initializes enumeration from other enumeration.
  */
  inline ArrayEnumeration(const ArrayEnumeration& copy) throw() : current(copy.current), end(copy.end) {}

//  ArrayEnumeration& operator=(const ArrayEnumeration& eq) throw();

  /**
    Returns true if not all the elements have been enumerated.
  */
  inline bool hasNext() const throw() {
    return current < end;
  }

  /**
    Returns the next element in the enumeration. Returns NULL if there is no
    more elements.
  */
  inline const TYPE* const next() throw() {
    return (current < end) ? current++ : NULL;
  }
};

#endif
