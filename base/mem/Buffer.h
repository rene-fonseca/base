/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__BUFFER_H
#define _DK_SDU_MIP__BASE_MEM__BUFFER_H

#include <base/Object.h>
#include <base/MemoryException.h>
#include <base/OutOfRange.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class implements a resizeable buffer and provides some low-level
  operations. The implementation is not MT-safe.

  @short Buffer.
  @ingroup memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Buffer : public Object {
public:

  /** Specifies the granularity of the buffer size. Guaranteed to be greater than 16 bytes. */
  static const unsigned int DEFAULT_GRANULARITY = 1024;
protected:

  /** The bytes of the buffer. */
  uint8* bytes;
  /** The size of the buffer. */
  unsigned int size;
  /** The granularity of the size. */
  unsigned int granularity;
private:

  Buffer& operator=(const Buffer& eq) throw();
public:

  /**
    Initializes the buffer of given size and does not clear the memory. Raises
    MemoryException if unable to allocate the required memory.

    @param size Specifies the initial size of the buffer. Default is 0.
    @param granularity Specifies the granularity of the size. Default is given
    by DEFAULT_GRANULARITY.
  */
  explicit Buffer(
    unsigned int size = 0,
    unsigned int granularity = DEFAULT_GRANULARITY) throw(MemoryException);
  
  /**
    Copy constructor. Raises MemoryException if unable to allocate the required
    memory.
  */
  Buffer(const Buffer& copy) throw(MemoryException);

  /**
    Returns the bytes of the buffer.
  */
  inline uint8* getBytes() throw() {
    return bytes;
  }

  /**
    Returns the bytes of the buffer.
  */
  inline const uint8* getBytes() const throw() {
    return bytes;
  }

  /**
    Returns the size of the buffer.
  */
  inline unsigned int getSize() const throw() {
    return size;
  }

  /**
    Sets the size of the buffer. The original bytes of the buffer are not
    changed. If the buffer is expanded the bytes are not initialized. Raises
    MemoryException if unable to adjust the size of the buffer or the buffer is
    externally managed.
    
    @param size The desired size.
  */
  void setSize(unsigned int size) throw(MemoryException);

  /**
    Destroys the buffer.
  */
  ~Buffer() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
