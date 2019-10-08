/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/mem/Allocator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Fast buffer implementation intended for linked lists.
  
  @short ORB buffer.
  @ingroup rmi
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API OrbBuffer : public Object {
private:

  /** The next buffer in the linked list. */
  OrbBuffer* next = nullptr;
  /** The storage. */
  Allocator<uint8> buffer;
  /** The first element of the buffer. */
  uint8* first = nullptr;
  /** The end of the buffer. */
  const uint8* end = nullptr;
  /** The read head. */
  const uint8* readHead = nullptr;
  /** The write head. */
  uint8* writeHead = nullptr;
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
