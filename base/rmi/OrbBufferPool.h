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

#include <base/mem/ReferenceCountedObject.h>
#include <base/collection/SingleLinkedNode.h>
#include <base/concurrency/Semaphore.h>
#include <base/concurrency/SpinLock.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/rmi/OrbBuffer.h>
#include <base/rmi/OrbException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker buffer pool.
  
  @short Object Request Broker buffer pool.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbBufferPool : public ReferenceCountedObject {
private:
  
  /** The guard. */
  typedef SpinLock Guard;
  
  /** The available buffers. */
  OrbBuffer* buffers = nullptr;
  /** The total number of buffers. */
  unsigned int numberOfBuffers = 0;
  /** The maximum number of buffers. */
  unsigned int maximumNumberOfBuffers = 0;
  /** Semaphore. */
  Semaphore semaphore;
  /** The guard. */
  Guard guard;
public:
  
  /** The size of the buffers. */
  static const unsigned int BUFFER_SIZE = 4 * 1024;
  /** The minimum buffer size. */
  static const unsigned int MINIMUM_BUFFER_SIZE = 1 * 1024;
  /** The maximum buffer size. */
  static const unsigned int MAXIMUM_BUFFER_SIZE = 1024 * 1024;
  /** The maximum number of available buffers in the queue. */
  static const unsigned int MAXIMUM_AVAILABLE_BUFFERS = 1024;
  
  /**
    Initializes the buffer pool.
  */
  OrbBufferPool() noexcept;
  
  /**
    Acquires a new buffer.
    
    @param holding The number of buffer currently held by requesting object.
  */
  OrbBuffer* acquire(unsigned int holding) throw(OrbException);
  
  /**
    Releases the specified buffers into the buffer pool.
  */
  inline void release(OrbBuffer* buffers) noexcept {
    if (buffers == 0) {
      return;
    }
    OrbBuffer* excess = nullptr;
    unsigned int numberOfExcess = 0;
    {
      ExclusiveSynchronize<Guard> _guard(guard);
      if (numberOfBuffers > MAXIMUM_AVAILABLE_BUFFERS) {
        excess = buffers;
        numberOfExcess = numberOfBuffers;
        while (buffers && (numberOfBuffers > MAXIMUM_AVAILABLE_BUFFERS)) {
          --numberOfBuffers;
          buffers = buffers->getNext();
        }
        numberOfExcess -= numberOfBuffers;
      }
      while (buffers) {
        OrbBuffer* node = buffers;
        buffers = buffers->getNext();
        node->setNext(this->buffers);
        this->buffers = node;
        semaphore.post();
      }
    }
    // destroy excess buffers without locking buffer pool
    while (numberOfExcess--) {
      OrbBuffer* node = excess;
      excess = excess->getNext();
      delete node;
    }
  }
};

#if 0
class _COM_AZURE_DEV__BASE__API BufferQueue : public Object {
private:
  
  /** The buffer pool. */
  Reference<OrbBufferPool> pool;
  /** The first buffer in the linked list. */
  OrbBuffer* first = nullptr;
  /** The last buffer in the linked list. */
  OrbBuffer* last = nullptr;
public:

  /**
    Initializes the buffer list.
  */
  inline BufferList(Reference<OrbBufferPool> _pool) throw(MemoryException)
    : pool(_pool) {
    first = pool->acquire();
    last = first;
  }
  
  /**
    Acquires a new buffer from the pool.
  */
  inline void acquire() throw(MemoryException) {
    OrbBuffer* buffer = pool->acquire();
    last->setNext(buffer);
    last = buffer;
  }
  
  /**
    Returns the elements of the current buffer for modifying access.
  */
  inline uint8* getElements() noexcept {
    return last->getElements();
  }
  
  /**
    Returns the elements of the current buffer for non-modifying access.
  */
  inline const uint8* getElements() const throw() {
    return last->getElements();
  }
  
  /**
    Returns the size of the current buffer.
  */
  inline unsigned int getSize() const throw() {
    return last->getSize();
  }
  
  inline OrbBuffer* pop() noexcept {
    OrbBuffer* node = first;
    if (first) {
      first = first->getNext();
    }
    return node;
  }
  
  /**
    Destroys the buffer list. The acquired buffers are brought back into the
    pool.
  */
  inline ~BufferList() noexcept {
    pool->release(first);
  }
};
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
