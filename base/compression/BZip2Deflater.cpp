/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/compression/BZip2Deflater.h>
#include <base/io/EndOfFile.h>
#include <base/string/FormatOutputStream.h>
#include <base/NotSupported.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace internal {
  
  class BZip2Deflater {
  public:
    
    struct Context {
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
      } catch (MemoryException&) {
        return 0;
      }
    }
    
    static void release(void*, void* memory) throw() {
      delete[] static_cast<uint8*>(memory);
    }
    
    enum Action {
      RUN = 0,
      FLUSH = 1,
      FINISH = 2
    };
    
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
  extern "C" int BZ2_bzCompressInit(BZip2Deflater::Context* stream, int blockSize, int verbosity, int workFactor);
  extern "C" int BZ2_bzCompress(BZip2Deflater::Context* stream, int action);
  extern "C" int BZ2_bzCompressEnd(BZip2Deflater::Context* stream);
};

BZip2Deflater::BZip2Deflater() throw(MemoryException)
  : buffer(BUFFER_SIZE), availableBytes(0), state(RUNNING) {
#if (defined(_COM_AZURE_DEV__BASE__BZ2))
  internal::BZip2Deflater::Context* context = new internal::BZip2Deflater::Context;
  this->context = context;
  clear(*context);
  context->allocate = internal::BZip2Deflater::allocate;
  context->release = internal::BZip2Deflater::release;
  unsigned int compressionLevel = minimum(maximum(DEFAULT_COMPRESSION_LEVEL, 1U), 9U);
  int code = internal::BZ2_bzCompressInit(context, compressionLevel, 0, 30);
  bassert(
    code == internal::BZip2Deflater::OK,
    MemoryException(this)
  );
#else
  throw NotSupported(this);
#endif
}

BZip2Deflater::BZip2Deflater(unsigned int compressionLevel) throw(MemoryException)
  : buffer(BUFFER_SIZE), availableBytes(0), state(RUNNING) {
#if (defined(_COM_AZURE_DEV__BASE__BZ2))
  internal::BZip2Deflater::Context* context = new internal::BZip2Deflater::Context;
  this->context = context;
  clear(*context);
  context->allocate = internal::BZip2Deflater::allocate;
  context->release = internal::BZip2Deflater::release;
  compressionLevel = minimum(maximum(compressionLevel, 1U), 9U);
  int code = internal::BZ2_bzCompressInit(context, compressionLevel, 0, 30);
  bassert(
    code == internal::BZip2Deflater::OK,
    MemoryException(this)
  );
#else
  throw NotSupported(this);
#endif
}

void BZip2Deflater::flush() throw(IOException) {
#if (defined(_COM_AZURE_DEV__BASE__BZ2))
  bassert(state != ENDED, EndOfFile());
  bassert(state == RUNNING, IOException(this));
  state = FLUSHING;
#else
  throw IOException(this);
#endif
}

unsigned int BZip2Deflater::push(const uint8* buffer, unsigned int size) throw(IOException) {
#if (defined(_COM_AZURE_DEV__BASE__BZ2))
  bassert(state != ENDED, EndOfFile());
  bassert(state == RUNNING, IOException(this));
  if (availableBytes == this->buffer.getSize()) {
    return 0; // no storage available
  }
  internal::BZip2Deflater::Context* context =
    Cast::pointer<internal::BZip2Deflater::Context*>(this->context);
  context->nextInput = buffer;
  context->bytesToWrite = size;
  context->totalInputLow = 0;
  context->totalInputHigh = 0;
  context->nextOutput = this->buffer.getElements() + availableBytes;
  context->bytesToRead = this->buffer.getSize() - availableBytes;
  int code = internal::BZ2_bzCompress(context, internal::BZip2Deflater::RUN);
  bassert(code == internal::BZip2Deflater::RUN_OK, IOException(this));
  availableBytes = this->buffer.getSize() - context->bytesToRead;
  return context->totalInputLow;
#else
  throw IOException(this);
#endif
}

void BZip2Deflater::pushEnd() throw(IOException) {
#if (defined(_COM_AZURE_DEV__BASE__BZ2))
  bassert(state != ENDED, EndOfFile());
  bassert(state == RUNNING, IOException(this));
  state = FINISHING;
#else
  throw IOException(this);
#endif
}

unsigned int BZip2Deflater::pull(uint8* buffer, unsigned int size) throw(IOException) {
#if (defined(_COM_AZURE_DEV__BASE__BZ2))
  bassert(state != ENDED, EndOfFile());
  
  if ((state == RUNNING) &&
      (availableBytes != this->buffer.getSize())) {
    return 0; // wait for full buffer
  }
  
  if (size <= availableBytes) { // TAG: what if size = availableBytes = 0
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
      internal::BZip2Deflater::Context* context =
        Cast::pointer<internal::BZip2Deflater::Context*>(this->context);
      context->nextInput = 0;
      context->bytesToWrite = 0;
      context->totalInputLow = 0;
      // context->totalInputHigh = 0; // TAG: not required
      context->nextOutput = buffer;
      context->bytesToRead = size;
      int code = internal::BZ2_bzCompress(context, internal::BZip2Deflater::FLUSH);
      if (code == internal::BZip2Deflater::FLUSH_OK) {
        // continue flushing
      } else if (code == internal::BZip2Deflater::RUN_OK) {
        state = RUNNING;
      } else {
        throw IOException(this);
      }
      return bytesRead + size - context->bytesToRead;
    }
  case FINISHING:
    {
      internal::BZip2Deflater::Context* context =
        Cast::pointer<internal::BZip2Deflater::Context*>(this->context);
      context->nextInput = 0;
      context->bytesToWrite = 0;
      context->totalInputLow = 0;
      // context->totalInputHigh = 0; // TAG: not required
      context->nextOutput = buffer;
      context->bytesToRead = size;
      int code = internal::BZ2_bzCompress(context, internal::BZip2Deflater::FINISH);
      if (code == internal::BZip2Deflater::FINISH_OK) {
        // continue finishing
      } else if (code == internal::BZip2Deflater::STREAM_END) {
        state = ENDED; // availableBytes = 0 (no need for FINISHED)
      } else {
        throw IOException(this);
      }
      return bytesRead + size - context->bytesToRead;
    }
  case FINISHED: // not possible
    state = ENDED; // availableBytes = 0 and FINISHED => end
    break;
  case ENDED: // not possible
    break;
  }
  return bytesRead;
#else
  throw IOException(this);
#endif
}

BZip2Deflater::~BZip2Deflater() throw() {
#if (defined(_COM_AZURE_DEV__BASE__BZ2))
  internal::BZip2Deflater::Context* context =
    Cast::pointer<internal::BZip2Deflater::Context*>(this->context);
  internal::BZ2_bzCompressEnd(context);
  delete context;
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
