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

#include <base/io/OutputStream.h>
#include <base/mem/Allocator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Memory output stream.

  @ingroup io
*/

class _COM_AZURE_DEV__BASE__API MemoryOutputStream : public OutputStream {
private:

  // list of buffers is better unless a single buffer is requested
  Allocator<uint8> buffer;
  MemorySize bytesRead = 0;
  bool closed = false;
public:

  MemoryOutputStream(MemorySize capacity = 16 * 1024)
    : buffer(capacity)
  {
  }
  
  void swap(Allocator<uint8>& buffer)
  {
    swapper(this->buffer, buffer);
  }

  void flush() override
  {
    if (closed) {
      _throw IOException("Stream is closed.");
    }
  }

  unsigned int write(const uint8* src, unsigned int size, bool nonblocking) override;

  inline void close() override
  {
    closed = true;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
