/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/OrbBufferPool.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

OrbBufferPool::OrbBufferPool() throw() {
  numberOfBuffers = 0;
  maximumNumberOfBuffers = 16384;
}

OrbBuffer* OrbBufferPool::acquire(unsigned int holding) throw(OrbException) {
  bool newBuffer = false;
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    if (buffers != 0) {
      OrbBuffer* node = buffers;
      buffers = buffers->getNext();
      semaphore.wait(); // will not block
      return node;
    }
    if (numberOfBuffers < maximumNumberOfBuffers) {
      ++numberOfBuffers;
      newBuffer = true;
    }
  }
  // allocate new buffer without blocking buffer pool
  if (newBuffer) {
    OrbBuffer* node = 0;
    try {
      node = new OrbBuffer(BUFFER_SIZE);
    } catch (MemoryException&) {
      ExclusiveSynchronize<Guard> _guard(guard);
      --numberOfBuffers; // undo
      if (numberOfBuffers == holding) {
        throw OrbException(this).setCause(OrbException::OUT_OF_MEMORY);
      }
    }
    if (node) {
      return node;
    }
  }
  while (true) { // retry until success
    semaphore.wait(); // blocking wait for buffer
    {
      ExclusiveSynchronize<Guard> _guard(guard);
      if (buffers) {
        OrbBuffer* node = buffers;
        buffers = buffers->getNext();
        return node;
      }
    }
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
