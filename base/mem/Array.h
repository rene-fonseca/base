/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__ARRAY_H
#define _DK_SDU_MIP__BASE_MEM__ARRAY_H

#include "base/MemoryException.h"
#include "ReferenceCountedObject.h"
#include <string.h>

/** Copy elements from array to other array. The arrays may not overlap. */
template<class TYPE>
inline void copyArray(TYPE* dest, const TYPE* src, unsigned int count) throw() {
  ::memcpy(dest, src, count * sizeof(TYPE));
}

/** Copy elements from array to array. The arrays are allowed to overlap. */
template<class TYPE>
inline void moveArray(TYPE* dest, const TYPE* src, unsigned int count) throw() {
  ::memmove(dest, src, count * sizeof(TYPE));
};

/**
  This class implements a reference counted, low-level, and resizeable array
  of elements. The implementation is not MT-safe.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class Array : public ReferenceCountedObject {
protected:

  /** The elements of the array. */
  TYPE* elements;
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
  explicit Array(unsigned int size = 0) throw(MemoryException);

  /**
    Initializes the array by copying the from the specified array. Throws
    'MemoryException' if unable to allocate the required memory.
  */
  Array(const Array& copy) throw(MemoryException);

  /**
    Returns the elements of the array.
  */
  inline TYPE* getElements() throw() {return elements;};

  /**
    Returns the elements of the array.
  */
  inline const TYPE* getElements() const throw() {return elements;};

  /**
    Returns the number of elements in the array.
  */
  inline unsigned int getSize() const throw() {return size;};

  /**
    Sets the number of elements in the array. The original elements are not
    changed if the size is increased. Throws 'MemoryException' if unable to
    adjust the size of the array.

    @param size The desired size.
  */
  void setSize(unsigned int size) throw(MemoryException);

  /**
    Destroys the array.
  */
  ~Array() throw();
};

#endif
