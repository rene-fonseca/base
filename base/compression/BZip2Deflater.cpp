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
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_BZIP2)
#  include <bzlib.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__USE_BZIP2)
namespace internal {
  
  class BZip2Deflater {
  public:
    
    static void* allocate(void*, int n, int m) noexcept
    {
      MemorySize size = static_cast<MemorySize>(n) * m;
      if ((size < 0) || (size > PrimitiveTraits<unsigned int>::MAXIMUM))  {
        return nullptr;
      }
      return Heap::allocateNoThrow<uint8>(size);
    }
    
    static void release(void*, void* memory) noexcept
    {
      Heap::release<uint8>(static_cast<uint8*>(memory));
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
};
#endif

BZip2Deflater::BZip2Deflater()
  : buffer(BUFFER_SIZE), availableBytes(0), state(RUNNING)
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
  bz_stream* context = new bz_stream;
  this->context = context;
  clear(*context);
  context->bzalloc = internal::BZip2Deflater::allocate;
  context->bzfree = internal::BZip2Deflater::release;
  context->opaque = nullptr;
  unsigned int compressionLevel = minimum(maximum(DEFAULT_COMPRESSION_LEVEL, 1U), 9U);
  int code = BZ2_bzCompressInit(context, compressionLevel, 0, 30);
  bassert(
    code == internal::BZip2Deflater::OK,
    MemoryException(this)
  );
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

BZip2Deflater::BZip2Deflater(unsigned int compressionLevel)
  : buffer(BUFFER_SIZE), availableBytes(0), state(RUNNING)
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
  bz_stream* context = new bz_stream;
  this->context = context;
  clear(*context);
  context->bzalloc = internal::BZip2Deflater::allocate;
  context->bzfree = internal::BZip2Deflater::release;
  context->opaque = nullptr;
  compressionLevel = minimum(maximum(compressionLevel, 1U), 9U);
  int code = BZ2_bzCompressInit(context, compressionLevel, 0, 30);
  bassert(
    code == internal::BZip2Deflater::OK,
    MemoryException(this)
  );
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

void BZip2Deflater::flush()
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
  bassert(state != ENDED, EndOfFile());
  bassert(state == RUNNING, IOException(this));
  state = FLUSHING;
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

MemorySize BZip2Deflater::push(const uint8* buffer, MemorySize size)
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
  bassert(state != ENDED, EndOfFile());
  bassert(state == RUNNING, IOException(this));
  if (availableBytes == this->buffer.getSize()) {
    return 0; // no storage available
  }
  bz_stream* context = Cast::pointer<bz_stream*>(this->context);
  context->next_in = (char*)static_cast<const uint8*>(buffer);
  context->avail_in = size;
  context->total_in_lo32 = 0;
  context->total_in_hi32 = 0;
  context->next_out = (char*)(this->buffer.getElements() + availableBytes);
  context->avail_out = this->buffer.getSize() - availableBytes;
  int code = BZ2_bzCompress(context, internal::BZip2Deflater::RUN);
  bassert(code == internal::BZip2Deflater::RUN_OK, IOException(this));
  availableBytes = this->buffer.getSize() - context->avail_out;
  return context->total_in_lo32;
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

void BZip2Deflater::pushEnd()
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
  bassert(state != ENDED, EndOfFile());
  bassert(state == RUNNING, IOException(this));
  state = FINISHING;
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

MemorySize BZip2Deflater::pull(uint8* buffer, MemorySize size)
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
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
      bz_stream* context = Cast::pointer<bz_stream*>(this->context);
      context->next_in = 0;
      context->avail_in = 0;
      context->total_in_lo32 = 0;
      // context->total_in_hi32 = 0; // TAG: not required
      context->next_out = (char*)static_cast<uint8*>(buffer);
      context->avail_out = size;
      int code = BZ2_bzCompress(context, internal::BZip2Deflater::FLUSH);
      if (code == internal::BZip2Deflater::FLUSH_OK) {
        // continue flushing
      } else if (code == internal::BZip2Deflater::RUN_OK) {
        state = RUNNING;
      } else {
        _throw IOException(this);
      }
      return bytesRead + size - context->avail_out;
    }
  case FINISHING:
    {
      bz_stream* context = Cast::pointer<bz_stream*>(this->context);
      context->next_in = 0;
      context->avail_in = 0;
      context->total_in_lo32 = 0;
      // context->total_in_hi32 = 0; // TAG: not required
      context->next_out = (char*)static_cast<uint8*>(buffer);
      context->avail_out = size;
      int code = BZ2_bzCompress(context, internal::BZip2Deflater::FINISH);
      if (code == internal::BZip2Deflater::FINISH_OK) {
        // continue finishing
      } else if (code == internal::BZip2Deflater::STREAM_END) {
        state = ENDED; // availableBytes = 0 (no need for FINISHED)
      } else {
        _throw IOException(this);
      }
      return bytesRead + size - context->avail_out;
    }
  case FINISHED: // not possible
    state = ENDED; // availableBytes = 0 and FINISHED => end
    break;
  case ENDED: // not possible
    break;
  }
  return bytesRead;
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

BZip2Deflater::~BZip2Deflater()
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
  bz_stream* context = Cast::pointer<bz_stream*>(this->context);
  BZ2_bzCompressEnd(context);
  delete context;
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
