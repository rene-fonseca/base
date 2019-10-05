/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__ORB_BUFFER_H
#define _DK_SDU_MIP__BASE_RMI__ORB_BUFFER_H

#include <base/Object.h>
#include <base/mem/Allocator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Fast buffer implementation intended for linked lists.
  
  @short ORB buffer.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbBuffer : public Object {
private:

  /** The next buffer in the linked list. */
  OrbBuffer* next;
  /** The storage. */
  Allocator<uint8> buffer;
  /** The first element of the buffer. */
  uint8* first;
  /** The end of the buffer. */
  const uint8* end;
  /** The read head. */
  const uint8* readHead;
  /** The write head. */
  uint8* writeHead;
public:
  
  /**
    Initializes the buffer with the specified size.
  */
  OrbBuffer(unsigned int size) throw(MemoryException)
    : buffer(size) {
    next = 0;
    first = buffer.getElements();
    end = first + buffer.getSize();
    readHead = first;
    writeHead = first;
  }

  /**
    Returns the next buffer.
  */
  inline OrbBuffer* getNext() throw() {
    return next;
  }
  
  /**
    Sets the next buffer.
  */
  inline void setNext(OrbBuffer* next) throw() {
    this->next = next;
  }
  
  /**
    Resets the buffer.
  */
  inline void reset() throw() {
    readHead = first;
    writeHead = first;
  }
  
  /**
    Returns the number of readable bytes.
  */
  inline unsigned int getReadable() const throw() {
    return writeHead - readHead;
  }

  /**
    Returns the number of writeable bytes.
  */
  inline unsigned int getWriteable() const throw() {
    return end - writeHead;
  }

  /**
    Returns true if no bytes are available for reading.
  */
  inline bool isEmpty() const throw() {
    return readHead == writeHead;
  }
  
  /**
    Returns true if no bytes may be written.
  */
  inline bool isFull() const throw() {
    return writeHead == end;
  }
  
  /**
    Allocates the specified number of bytes.
    
    @param size The number of bytes to write.
    @return 0 if invalid request.
  */
  inline uint8* push(unsigned int size) throw() {
    if (end < (writeHead + size)) {
      return 0;
    }
    uint8* result = writeHead;
    writeHead += size;
    return result;
  }
  
  /**
    Reads the specified number of bytes.

    @param size The number of bytes to read.
    @return 0 if the buffer doesn't contain the requested number of bytes.
  */
  inline const uint8* pull(unsigned int size) throw() {
    if (writeHead < (readHead + size)) {
      return 0;
    }
    const uint8* result = readHead;
    readHead += size;
    return result;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
