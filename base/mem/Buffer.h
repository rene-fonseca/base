/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__BUFFER_H
#define _DK_SDU_MIP__BASE_MEM__BUFFER_H

#include "../Object.h"
#include "../MemoryException.h"
#include "../OutOfRange.h"

/**
  This class implements a resizeable buffer and provides some low-level operations. The implementation is not MT-safe.

  @author René Møller Fonseca
  @version 1.0
*/

class Buffer : public Object {
protected:

  /** The size of the buffer. */
  unsigned int size;
  /** The bytes of the buffer. */
  char* bytes;
  /** Specifies that the memory is managed internally. */
  bool managed;
public:

  /**
    Initializes an empty buffer.
  */
  explicit Buffer() throw();

  /**
    Initializes the buffer of given size and does not clear the memory. Throws 'MemoryException' if unable to allocate the required memory.

    @param size The desired size of the buffer.
  */
  explicit Buffer(unsigned int size) throw(MemoryException);

  /**
    Initializes the buffer of given size and fills the buffer with a constant. Throws 'MemoryException' if unable to allocate the required memory.

    @param size The desired size of the buffer.
    @param value The value used to fill the buffer.
  */
  Buffer(unsigned int size, char value) throw(MemoryException);

  /**
    Initializes the buffer with an external buffer of a fixed size. Throws 'MemoryException' if bytes and size conflict with each other.

    @param bytes The external buffer.
    @param size The fixed size of the external buffer.
  */
  Buffer(char* bytes, unsigned int size) throw(MemoryException);

  /**
    Copy constructor. Throws 'MemoryException' if unable to allocate the required memory.
  */
  Buffer(const Buffer& obj) throw(MemoryException);

  /**
    Returns the bytes of the buffer.
  */
  inline char* getBytes() throw() {return bytes;};

  /**
    Returns the bytes of the buffer.
  */
  inline const char* getBytes() const throw() {return bytes;};

  /**
    Returns the size of the buffer.
  */
  inline unsigned int getSize() const throw() {return size;};

  /**
    Returns true if the buffer is managed by this buffer object.
  */
  inline bool isManaged() const throw() {return managed;};

  /**
    Sets the size of the buffer. The original bytes of the buffer are not changed. If the buffer is expanded the bytes are not initialized. Throws 'MemoryException' if unable to adjust the size of the buffer or the buffer is externally managed.

    @param size The desired size.
  */
  void setSize(unsigned int size) throw(MemoryException);

  /**
    Fills the buffer with a constant. Throws 'OutOfRange' if offset and count conflict with the size of the buffer.

    @param value The constant used to fill the buffer.
    @param offset The offset of the first byte to set.
    @param count The number of bytes to fill.
  */
  void fill(char value, unsigned int offset, unsigned int count) throw(OutOfRange);

  /**
    Copies bytes to buffer. Throws 'OutOfRange' if offset and count conflict with the size of the buffer.

    @param src Buffer holding the source bytes.
    @param offset The offset of the first byte to set.
    @param count The number of bytes to copy.
  */
  void copy(const char* src, unsigned int offset, unsigned int count) throw(OutOfRange);

  /**
    Copies bytes to buffer. Throws 'OutOfRange' if the size of the buffers, the offset, and the count conflict.

    @param src Buffer holding the source bytes.
    @param offset The offset of the first byte to set.
    @param count The number of bytes to copy.
  */
  inline void copy(const Buffer& src, unsigned int offset, unsigned int count) throw(OutOfRange) {copy(src.getBytes(), offset, count);};

  /**
    Destroys the buffer.
  */
  ~Buffer() throw();
};

#endif
