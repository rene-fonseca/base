/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/compression/ZLibDeflater.h>
#include <base/io/EndOfFile.h>
#include <base/string/FormatOutputStream.h>
#include <base/NotSupported.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace internal {
  
  class ZLibDeflater {
  public:
    
    struct Context {
      const uint8* nextInput;
      unsigned int bytesToWrite;
      unsigned long totalInput;
      uint8* nextOutput;
      unsigned int bytesToRead;
      unsigned long totalOutput;
      char* message;
      void* state;
      void* (*allocate)(void*, int, int) throw();
      void (*release)(void*, void*) throw();
      void* opaque;
      int dataType;
      unsigned long adler;
      unsigned long reserved;
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
    
    enum Action {
      NO_FLUSH = 0,
      SYNC_FLUSH = 2,
      FULL_FLUSH = 3,
      FINISH = 4
    };
    
    enum Code {
      OK = 0,
      STREAM_END = 1,
      NEED_DICT = 2,
      VERSION_ERROR = -6
    };
  };
  
  // TAG: dll support
  extern "C" int deflateInit_(ZLibDeflater::Context* stream, int level, const char* version, int size);
  extern "C" int deflate(ZLibDeflater::Context* stream, int flush);
  extern "C" int deflateEnd(ZLibDeflater::Context* stream);
};

ZLibDeflater::ZLibDeflater() throw(MemoryException)
  : buffer(BUFFER_SIZE), availableBytes(0), state(RUNNING) {
#if (defined(_DK_SDU_MIP__BASE__ZLIB))
  internal::ZLibDeflater::Context* context = new internal::ZLibDeflater::Context;
  this->context = context;
  clear(*context);
  context->allocate = internal::ZLibDeflater::allocate;
  context->release = internal::ZLibDeflater::release;
  unsigned int compressionLevel = minimum(maximum(DEFAULT_COMPRESSION_LEVEL, 1U), 9U);
  int code = internal::deflateInit_(context, compressionLevel, "1.1.4", sizeof(*context));
  switch (code) {
  case internal::ZLibDeflater::OK:
    break;
  case internal::ZLibDeflater::VERSION_ERROR:
    throw NotSupported(this);
  default:
    throw MemoryException(this);
  }
#else
  throw NotSupported(this);
#endif
}

ZLibDeflater::ZLibDeflater(unsigned int compressionLevel) throw(MemoryException)
  : buffer(BUFFER_SIZE), availableBytes(0), state(RUNNING) {
#if (defined(_DK_SDU_MIP__BASE__ZLIB))
  internal::ZLibDeflater::Context* context = new internal::ZLibDeflater::Context;
  this->context = context;
  clear(*context);
  context->allocate = internal::ZLibDeflater::allocate;
  context->release = internal::ZLibDeflater::release;
  compressionLevel = minimum(maximum(compressionLevel, 1U), 9U);
  int code = internal::deflateInit_(context, compressionLevel, "1.1.4", sizeof(*context));
  switch (code) {
  case internal::ZLibDeflater::OK:
    break;
  case internal::ZLibDeflater::VERSION_ERROR:
    throw NotSupported(this);
  default:
    throw MemoryException(this);
  }
#else
  throw NotSupported(this);
#endif
}

void ZLibDeflater::flush() throw(IOException) {
#if (defined(_DK_SDU_MIP__BASE__ZLIB))
  assert(!atEnd(), EndOfFile());
  assert(state == RUNNING, IOException(this)); // TAG: should we accept FLUSHING
  state = FLUSHING;
#else
  throw IOException(this);
#endif
}

unsigned int ZLibDeflater::push(const uint8* buffer, unsigned int size) throw(IOException) {
#if (defined(_DK_SDU_MIP__BASE__ZLIB))
  assert(!atEnd(), EndOfFile());
  assert(state == RUNNING, IOException(this));
  if (availableBytes == this->buffer.getSize()) {
    return 0; // no storage available
  }
  internal::ZLibDeflater::Context* context =
    Cast::pointer<internal::ZLibDeflater::Context*>(this->context);
  context->nextInput = buffer;
  context->bytesToWrite = size;
  context->totalInput = 0;
  context->nextOutput = this->buffer.getElements() + availableBytes;
  context->bytesToRead = this->buffer.getSize() - availableBytes;
  int code = internal::deflate(context, internal::ZLibDeflater::NO_FLUSH);
  assert(code == internal::ZLibDeflater::OK, IOException(this));
  availableBytes += this->buffer.getSize() - context->bytesToRead;
  return context->totalInput;
#else
  throw IOException(this);
#endif
}

void ZLibDeflater::pushEnd() throw(IOException) {
#if (defined(_DK_SDU_MIP__BASE__ZLIB))
  assert(!atEnd(), EndOfFile());
  assert(state == RUNNING, IOException(this));
  state = FINISHING;
#else
  throw IOException(this);
#endif
}

unsigned int ZLibDeflater::pull(uint8* buffer, unsigned int size) throw(IOException) {
#if (defined(_DK_SDU_MIP__BASE__ZLIB))
  assert(!atEnd(), EndOfFile());
  
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
  case FLUSHING:
    {
      internal::ZLibDeflater::Context* context =
        Cast::pointer<internal::ZLibDeflater::Context*>(this->context);
      context->nextInput = 0;
      context->bytesToWrite = 0;
      context->totalInput = 0;
      context->nextOutput = buffer;
      context->bytesToRead = size;
      int code = internal::deflate(context, internal::ZLibDeflater::SYNC_FLUSH);
      if (code == internal::ZLibDeflater::OK) {
        if (context->bytesToRead != 0) { // done flushing
          state = RUNNING;
        }
      } else {
        throw IOException(this);
      }
      return bytesRead + size - context->bytesToRead;
    }
  case FINISHING:
    {
      internal::ZLibDeflater::Context* context =
        Cast::pointer<internal::ZLibDeflater::Context*>(this->context);
      context->nextInput = 0;
      context->bytesToWrite = 0;
      context->totalInput = 0;
      context->nextOutput = buffer;
      context->bytesToRead = size;
      int code = internal::deflate(context, internal::ZLibDeflater::FINISH);
      if (code == internal::ZLibDeflater::OK) {
        // continue finishing
      } else if (code == internal::ZLibDeflater::STREAM_END) {
        state = COMPLETED;
      } else {
        throw IOException(this);
      }
      return bytesRead + size - context->bytesToRead;
    }
  case COMPLETED: // not possible
    break;
  }
  return bytesRead;
#else
  throw IOException(this);
#endif
}

ZLibDeflater::~ZLibDeflater() throw() {
#if (defined(_DK_SDU_MIP__BASE__ZLIB))
  internal::ZLibDeflater::Context* context =
    Cast::pointer<internal::ZLibDeflater::Context*>(this->context);
  internal::deflateEnd(context);
  delete context;
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
