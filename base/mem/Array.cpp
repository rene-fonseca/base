/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Array.h"
#include <stdlib.h>

template Array<char>;

template<class TYPE>
Array<TYPE>::Array(unsigned int size) throw(MemoryException) {
  elements = (TYPE*)malloc(size * sizeof(TYPE));
  if ((elements == NULL) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
}

template<class TYPE>
Array<TYPE>::Array(const Array& copy) throw(MemoryException) {
  size = copy.size;
  elements = (TYPE*)malloc(size * sizeof(TYPE));
  if ((elements == NULL) && (size != 0)) { // was memory allocated
    throw MemoryException();
  }
  copyArray(elements, copy.elements, size);
}

template<class TYPE>
void Array<TYPE>::setSize(unsigned int size) throw(MemoryException) {
  if (size != this->size) { // do we really have to
    this->size = size;
    TYPE* result = (TYPE*)realloc(elements, size * sizeof(TYPE));
    if ((result == NULL) && (size > 0)) { // was memory allocated
      throw MemoryException();
    }
    elements = result;
  }
}

template<class TYPE>
Array<TYPE>::~Array() throw() {
  ::free(elements);
}
