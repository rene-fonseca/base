/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__ALLOCATOR_H
#define _DK_SDU_MIP__BASE_MEM__ALLOCATOR_H

//#include <base/Object.h>
#include <base/MemoryException.h>

/**
  Allocator of memory.

  @author René Møller Fonseca
  @version 1.0
*/

class Allocator {
protected:

  /** The allocated memory block. */
  char* memory;
  /** The size of the allocated memory block. */
  unsigned int size;
public:

  /**
    Initializes an allocator.
  */
  Allocator(unsigned int size) throw(MemoryException);

  /**
    Initializes allocator from other allocator.
  */
  Allocator(const Allocator& copy) throw(MemoryException);

  /**
    Returns the allocated memory.
  */
  inline char* getMemory() const throw() {
    return memory;
  }

  /**
    Returns the size of the allocated memory.
  */
  inline unsigned int getSize() const throw() {
    return size;
  }

  /**
    Sets the size of the allocated memory.
  */
  void setSize(unsigned int size) throw(MemoryException);

  /**
    Destroys the allocator.
  */
  ~Allocator() throw();
};

#endif
