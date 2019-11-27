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
#include <base/mem/Heap.h>
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
  HeapBlockOwned<uint8> heap;
  /** The size of the buffer. */
  MemorySize size = 0;
  /** The granularity of the size. */
  MemorySize granularity = DEFAULT_GRANULARITY;
private:

  Buffer& operator=(const Buffer& assign) noexcept;
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
    MemorySize granularity = DEFAULT_GRANULARITY);
  
  /**
    Copy constructor. Raises MemoryException if unable to allocate the required
    memory.
  */
  Buffer(const Buffer& copy);

  /**
    Returns the bytes of the buffer.
  */
  inline uint8* getBytes() noexcept
  {
    return static_cast<uint8*>(heap);
  }

  /**
    Returns the bytes of the buffer.
  */
  inline const uint8* getBytes() const noexcept
  {
    return static_cast<const uint8*>(heap);
  }

  /**
    Returns the size of the buffer.
  */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  /**
    Sets the size of the buffer. The original bytes of the buffer are not
    changed. If the buffer is expanded the bytes are not initialized. Raises
    MemoryException if unable to adjust the size of the buffer or the buffer is
    externally managed.
    
    @param size The desired size.
  */
  void setSize(MemorySize size);

  /**
    Returns the byte at the given index.
  */
  inline uint8 operator[](MemorySize i) const
  {
    if (i >= size) {
      throw OutOfRange();
    }
    return static_cast<const uint8*>(heap)[i];
  }

  /**
    Returns the byte at the given index.
  */
  inline uint8& operator[](MemorySize i)
  {
    if (i >= size) {
      throw OutOfRange();
    }
    return static_cast<uint8*>(heap)[i];
  }

  inline uint8* begin() noexcept
  {
    return static_cast<uint8*>(heap);
  }

  inline uint8* end() noexcept
  {
    return static_cast<uint8*>(heap) + size;
  }

  inline const uint8* begin() const noexcept
  {
    return static_cast<const uint8*>(heap);
  }

  inline const uint8* end() const noexcept
  {
    return static_cast<const uint8*>(heap) + size;
  }

  inline operator bool() const noexcept
  {
    return size != 0;
  }

  /**
    Destroys the buffer.
  */
  ~Buffer() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
