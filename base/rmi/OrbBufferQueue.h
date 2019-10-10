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
#include <base/concurrency/SpinLock.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/rmi/OrbBufferPool.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker buffer queue. The implementation is MT-safe.
  
  @short Object Request Broker buffer queue.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API BufferQueue : public Object {
private:

  typedef SpinLock Guard;
  /** The guard. */
  Guard guard;
  /** The buffer pool. */
  Reference<OrbBufferPool> pool;
  /** The first buffer in the linked list. */
  OrbBuffer* first = nullptr;
  /** The last buffer in the linked list. */
  OrbBuffer* last = nullptr;
public:
  
  /**
    Initializes the buffer queue.
  */
  inline BufferQueue(Reference<OrbBufferPool> _pool) throw()
    : pool(_pool) {
    first = 0;
    last = 0;
  }
  
  /**
    Pushes a buffer onto the queue.
  */
  inline void push(OrbBuffer* buffer) throw() {
    ExclusiveSynchronize<Guard> _guard(guard);
    if (last) {
      last->setNext(buffer);
      last = buffer;
    } else {
      first = buffer;
      last = first;
    }
  }
  
  /**
    Returns the next buffer in the queue.

    @return 0 if no buffer is available.
  */
  inline OrbBuffer* pop() throw() {
    ExclusiveSynchronize<Guard> _guard(guard);
    OrbBuffer* buffer = first;
    if (first) {
      first = first->getNext();
      if (!first) {
        last = 0;
      }
    }
    buffer->setNext(nullptr);
    return buffer;
  }
  
  /**
    Destroys the buffer queue. The acquired buffers are brought back into the
    pool.
  */
  inline ~BufferQueue() throw() {
    if (first) {
      pool->release(first);
    }
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
