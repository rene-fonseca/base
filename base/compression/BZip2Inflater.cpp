/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/compression/BZip2Inflater.h>
#include <base/io/EndOfFile.h>
#include <base/string/FormatOutputStream.h>
#include <base/NotSupported.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace internal {
  
  class BZip2Inflater {
  public:
    
    struct Context { // TAG: what if 64 bit architecture
      const uint8* nextInput;
      unsigned int bytesToWrite;
      unsigned int totalInputLow;
      unsigned int totalInputHigh;
      uint8* nextOutput;
      unsigned int bytesToRead;
      unsigned int totalOutputLow;
      unsigned int totalOutputHigh;
      void* state;
      void* (*allocate)(void*, int, int) throw();
      void (*release)(void*, void*) throw();
      void* opaque;
    };
    
    static void* allocate(void*, int n, int m) throw() {
      long long size = static_cast<long long>(n) * m;
      if ((size < 0) || (size > PrimitiveTraits<unsigned int>::MAXIMUM))  {
        return 0;
      }
      try {
        return new uint8[size];
      } catch (MemoryException& e) {
        return 0;
      }
    }
    
    static void release(void*, void* memory) throw() {
      delete[] static_cast<uint8*>(memory);
    }
    
    enum Code {
      OK = 0,
      RUN_OK = 1,
      FLUSH_OK = 2,
      FINISH_OK = 3,
      STREAM_END = 4,
      ERROR_SEQUENCE = -1,
      ERROR_PARAM = -2
    };
  };

  // TAG: dll support
  extern "C" int BZ2_bzDecompressInit(BZip2Inflater::Context* stream, int verbosity, int small);
  extern "C" int BZ2_bzDecompress(BZip2Inflater::Context* stream);
  extern "C" int BZ2_bzDecompressEnd(BZip2Inflater::Context* stream);
};

BZip2Inflater::BZip2Inflater() throw(MemoryException)
  : buffer(BUFFER_SIZE), availableBytes(0), state(RUNNING) {
#if (defined(_DK_SDU_MIP__BASE__BZ2))
  internal::BZip2Inflater::Context* context = new internal::BZip2Inflater::Context;
  this->context = context;
  clear(*context);
  context->allocate = internal::BZip2Inflater::allocate;
  context->release = internal::BZip2Inflater::release;
  assert(
    internal::BZ2_bzDecompressInit(context, 0, 0) == internal::BZip2Inflater::OK,
    MemoryException(this)
  );
#else
  throw NotSupported(this);
#endif
}

unsigned int BZip2Inflater::push(const uint8* buffer, unsigned int size) throw(IOException) {
#if (defined(_DK_SDU_MIP__BASE__BZ2))
  assert(state != ENDED, EndOfFile());
  assert(state == RUNNING, IOException(this));
  if (availableBytes == this->buffer.getSize()) {
    return 0; // no storage available
  }
  internal::BZip2Inflater::Context* context =
    Cast::pointer<internal::BZip2Inflater::Context*>(this->context);
  context->nextInput = buffer;
  context->bytesToWrite = size;
  context->totalInputLow = 0;
  context->totalInputHigh = 0;
  context->nextOutput = this->buffer.getElements() + availableBytes;
  context->bytesToRead = this->buffer.getSize() - availableBytes;
  int code = internal::BZ2_bzDecompress(context);
  if (code == internal::BZip2Inflater::OK) {
    // continue pushing
  } else if (code == internal::BZip2Inflater::STREAM_END) {
    state = FINISHED; // stream may containg garbage data after eof (e.g. padding)
  } else {
    throw IOException(this);
  }
  availableBytes = this->buffer.getSize() - context->bytesToRead;
  return context->totalInputLow;
#else
  throw IOException(this);
#endif
}

void BZip2Inflater::pushEnd() throw(IOException) {
#if (defined(_DK_SDU_MIP__BASE__BZ2))
  if (state != ENDED) {
    assert(state == RUNNING, IOException(this));
    state = FINISHING;
  }
#else
  throw IOException(this);
#endif
}

unsigned int BZip2Inflater::pull(uint8* buffer, unsigned int size) throw(IOException) {
#if (defined(_DK_SDU_MIP__BASE__BZ2))
  assert(state != ENDED, EndOfFile());
  
  if ((state == RUNNING) &&
      (availableBytes != this->buffer.getSize())) {
    return 0; // wait for full buffer
  }
  
  if (size <= availableBytes) {
    copy(buffer, this->buffer.getElements(), size);
    move(this->buffer.getElements(), this->buffer.getElements() + size, availableBytes - size);
    availableBytes -= size;
    return size;
  }
  
  unsigned int bytesRead = 0;
  if (availableBytes) {
    copy(buffer, this->buffer.getElements(), availableBytes);
    buffer += availableBytes;
    size -= availableBytes;
    bytesRead += availableBytes;
    availableBytes = 0;
  }
  
  switch (state) {
  case RUNNING:
    break;
  case FLUSHING: // not possible (not supported)
    break;
  case FINISHING:
    {
      internal::BZip2Inflater::Context* context =
        Cast::pointer<internal::BZip2Inflater::Context*>(this->context);
      context->nextInput = 0;
      context->bytesToWrite = 0;
      context->totalInputLow = 0;
      context->totalInputHigh = 0;
      context->nextOutput = buffer;
      context->bytesToRead = size;
      int code = internal::BZ2_bzDecompress(context);
      if (code == internal::BZip2Inflater::OK) {
        // continue finishing
      } else if (code == internal::BZip2Inflater::STREAM_END) {
        state = ENDED; // availableBytes = 0 thus we skip FINISHED
      } else {
        throw IOException(this);
      }
      return bytesRead + size - context->bytesToRead;
    }
  case FINISHED:
    state = ENDED; // availableBytes == 0 and FINISHED => end
    break;
  case ENDED: // not possible
    break;
  }
  return bytesRead;
#else
  throw IOException(this);
#endif
}

BZip2Inflater::~BZip2Inflater() throw() {
#if (defined(_DK_SDU_MIP__BASE__BZ2))
  internal::BZip2Inflater::Context* context =
    Cast::pointer<internal::BZip2Inflater::Context*>(this->context);
  internal::BZ2_bzDecompressEnd(context);
  delete context;
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
