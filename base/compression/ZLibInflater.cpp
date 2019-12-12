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
#include <base/compression/ZLibInflater.h>
#include <base/io/EndOfFile.h>
#include <base/string/FormatOutputStream.h>
#include <base/NotSupported.h>
#include <base/build.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace internal {
  
  class ZLibInflater {
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
    
    static void* allocate(void*, int n, int m) throw()
    {
      MemorySize size = static_cast<MemorySize>(n) * m;
      if ((size < 0) || (size > PrimitiveTraits<unsigned int>::MAXIMUM))  {
        return 0;
      }
      return Heap::allocateNoThrow<uint8>(size);
    }
    
    static void release(void*, void* memory) throw()
    {
      Heap::release<uint8>(static_cast<uint8*>(memory));
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
  extern "C" int inflateInit_(ZLibInflater::Context* stream, const char* version, int size);
  extern "C" int inflate(ZLibInflater::Context* stream, int flush);
  extern "C" int inflateEnd(ZLibInflater::Context* stream);
};

ZLibInflater::ZLibInflater() throw(MemoryException)
  : buffer(BUFFER_SIZE), availableBytes(0), state(RUNNING)
{
#if (defined(_COM_AZURE_DEV__BASE__USE_ZLIB))
  internal::ZLibInflater::Context* context = new internal::ZLibInflater::Context;
  this->context = context;
  clear(*context);
  context->allocate = internal::ZLibInflater::allocate;
  context->release = internal::ZLibInflater::release;
  int code = internal::inflateInit_(context, "1.1.4", sizeof(*context));
  switch (code) {
  case internal::ZLibInflater::OK:
    break;
  case internal::ZLibInflater::VERSION_ERROR:
    throw NotSupported(this);
  default:
    throw MemoryException(this);
  }
#else
  throw NotSupported(this);
#endif
}

unsigned int ZLibInflater::push(const uint8* buffer, unsigned int size) throw(IOException) {
#if (defined(_COM_AZURE_DEV__BASE__USE_ZLIB))
  bassert(state != ENDED, EndOfFile());
  bassert(state == RUNNING, IOException(this));
  if (availableBytes == this->buffer.getSize()) {
    return 0; // no storage available
  }
  internal::ZLibInflater::Context* context =
    Cast::pointer<internal::ZLibInflater::Context*>(this->context);
  context->nextInput = buffer;
  context->bytesToWrite = size;
  context->totalInput = 0;
  context->nextOutput = this->buffer.getElements() + availableBytes;
  context->bytesToRead = this->buffer.getSize() - availableBytes;
  int code = internal::inflate(context, internal::ZLibInflater::NO_FLUSH);
  if (code == internal::ZLibInflater::OK) {
    // continue pushing
  } else if (code == internal::ZLibInflater::STREAM_END) {
    state = FINISHED; // stream may containg garbage data after eof (e.g. padding)
  } else {
    throw IOException(this);
  }
  availableBytes = this->buffer.getSize() - context->bytesToRead;
  return context->totalInput;
#else
  throw IOException(this);
#endif
}

void ZLibInflater::pushEnd() throw(IOException) {
#if (defined(_COM_AZURE_DEV__BASE__USE_ZLIB))
  if (state != ENDED) {
    bassert(state == RUNNING, IOException(this));
    state = FINISHING;
  }
#else
  throw IOException(this);
#endif
}

unsigned int ZLibInflater::pull(uint8* buffer, unsigned int size) throw(IOException) {
#if (defined(_COM_AZURE_DEV__BASE__USE_ZLIB))
  bassert(state != ENDED, EndOfFile());
  
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
  case FLUSHING: // not possible
    break;
    {
      internal::ZLibInflater::Context* context =
        Cast::pointer<internal::ZLibInflater::Context*>(this->context);
      context->nextInput = 0;
      context->bytesToWrite = 0;
      context->totalInput = 0;
      context->nextOutput = buffer;
      context->bytesToRead = size;
      int code = internal::inflate(context, internal::ZLibInflater::SYNC_FLUSH);
      if (code == internal::ZLibInflater::OK) {
        if (context->bytesToRead != 0) { // done flushing
          state = RUNNING;
        }
      } else if (code == internal::ZLibInflater::STREAM_END) { // TAG: is this possible
        state = FINISHED; // done flushing
      } else {
        throw IOException(this);
      }
      return bytesRead + size - context->bytesToRead;
    }
  case FINISHING:
    {
      internal::ZLibInflater::Context* context =
        Cast::pointer<internal::ZLibInflater::Context*>(this->context);
      context->nextInput = 0;
      context->bytesToWrite = 0;
      context->totalInput = 0;
      context->nextOutput = buffer;
      context->bytesToRead = size;
      int code = internal::inflate(context, internal::ZLibInflater::SYNC_FLUSH); // TAG: do not use FINISH here
      if (code == internal::ZLibInflater::OK) {
        // continue finishing
      } else if (code == internal::ZLibInflater::STREAM_END) {
        state = ENDED; // availableBytes = 0 thus we skip FINISHED
      } else {
        throw IOException(this);
      }
      return bytesRead + size - context->bytesToRead;
    }
  case FINISHED:
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

ZLibInflater::~ZLibInflater() throw() {
#if (defined(_COM_AZURE_DEV__BASE__USE_ZLIB))
  internal::ZLibInflater::Context* context =
    Cast::pointer<internal::ZLibInflater::Context*>(this->context);
  internal::inflateEnd(context);
  delete context;
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
