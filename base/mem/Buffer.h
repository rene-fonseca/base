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
#include <base/MemoryException.h>
#include <base/OutOfRange.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class implements a resizeable buffer and provides some low-level
  operations. The implementation is not MT-safe.

  @short Buffer.
  @ingroup memory
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Buffer : public Object {
public:

  /** Specifies the granularity of the buffer size. Guaranteed to be greater than 16 bytes. */
  static const MemorySize DEFAULT_GRANULARITY = 1024;
protected:

  /** The bytes of the buffer. */
  uint8* bytes = nullptr;
  /** The size of the buffer. */
  MemorySize size = 0;
  /** The granularity of the size. */
  MemorySize granularity = 0;
private:

  Buffer& operator=(const Buffer& assign) throw();
public:

  /**
    Initializes the buffer of given size and does not clear the memory. Raises
    MemoryException if unable to allocate the required memory.

    @param size Specifies the initial size of the buffer. Default is 0.
    @param granularity Specifies the granularity of the size. Default is given
    by DEFAULT_GRANULARITY.
  */
  explicit Buffer(
    MemorySize size = 0,
    MemorySize granularity = DEFAULT_GRANULARITY) throw(MemoryException);
  
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
  inline MemorySize getSize() const throw() {
    return size;
  }

  /**
    Sets the size of the buffer. The original bytes of the buffer are not
    changed. If the buffer is expanded the bytes are not initialized. Raises
    MemoryException if unable to adjust the size of the buffer or the buffer is
    externally managed.
    
    @param size The desired size.
  */
  void setSize(MemorySize size) throw(MemoryException);

  /**
    Returns the byte at the given index.
  */
  inline uint8 operator[](MemorySize i) const throw(OutOfRange) {
    if (i >= size) {
      throw OutOfRange();
    }
    return bytes[i];
  }

  /**
    Returns the byte at the given index.
  */
  inline uint8& operator[](MemorySize i) throw(OutOfRange) {
    if (i >= size) {
      throw OutOfRange();
    }
    return bytes[i];
  }

  inline uint8* begin() noexcept
  {
    return bytes;
  }

  inline uint8* end() noexcept
  {
    return bytes + size;
  }

  inline const uint8* begin() const noexcept
  {
    return bytes;
  }

  inline const uint8* end() const noexcept
  {
    return bytes + size;
  }

  /**
    Destroys the buffer.
  */
  ~Buffer() throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
