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
#include <base/compression/BZip2Inflater.h>
#include <base/io/EndOfFile.h>
#include <base/string/FormatOutputStream.h>
#include <base/NotSupported.h>
#include <base/Module.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_BZIP2)
#  include <bzlib.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace internal {
  
  class BZip2Inflater {
  public:
    
    static void* allocate(void*, int n, int m) noexcept
    {
      MemorySize size = static_cast<MemorySize>(n) * m;
      if ((size < 0) || (size > PrimitiveTraits<unsigned int>::MAXIMUM))  {
        return 0;
      }
      return Heap::allocateNoThrow<uint8>(size);
    }
    
    static void release(void*, void* memory) noexcept
    {
      Heap::release<uint8>(static_cast<uint8*>(memory));
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
};

#if defined(_COM_AZURE_DEV__BASE__USE_BZIP2)
MODULE_REGISTER_EXPLICIT(_COM_AZURE_DEV__BASE__THIS_MODULE, "org.sourceware.bzip2", "libbzip2", "", "https://www.sourceware.org/bzip2/");
#endif

BZip2Inflater::BZip2Inflater()
  : buffer(BUFFER_SIZE), availableBytes(0), state(RUNNING)
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
  bz_stream* context = new bz_stream;
  this->context = context;
  clear(*context);
  context->bzalloc = internal::BZip2Inflater::allocate;
  context->bzfree = internal::BZip2Inflater::release;
  context->opaque = nullptr;
  bassert(
    BZ2_bzDecompressInit(context, 0, 0) == internal::BZip2Inflater::OK,
    MemoryException(this)
  );
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

MemorySize BZip2Inflater::push(const uint8* buffer, MemorySize size)
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
  int code = BZ2_bzDecompress(context);
  if (code == internal::BZip2Inflater::OK) {
    // continue pushing
  } else if (code == internal::BZip2Inflater::STREAM_END) {
    state = FINISHED; // stream may containg garbage data after eof (e.g. padding)
  } else {
    _throw IOException(this);
  }
  availableBytes = this->buffer.getSize() - context->avail_out;
  return context->total_in_lo32;
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

void BZip2Inflater::pushEnd()
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
  if (state != ENDED) {
    bassert(state == RUNNING, IOException(this));
    state = FINISHING;
  }
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

MemorySize BZip2Inflater::pull(uint8* buffer, MemorySize size)
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
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
  case FLUSHING: // not possible (not supported)
    break;
  case FINISHING:
    {
      bz_stream* context = Cast::pointer<bz_stream*>(this->context);
      context->next_in = 0;
      context->avail_in = 0;
      context->total_in_lo32 = 0;
      context->total_in_hi32 = 0;
      context->next_out = (char*)static_cast<uint8*>(buffer);
      context->avail_out = size;
      int code = BZ2_bzDecompress(context);
      if (code == internal::BZip2Inflater::OK) {
        // continue finishing
      } else if (code == internal::BZip2Inflater::STREAM_END) {
        state = ENDED; // availableBytes = 0 thus we skip FINISHED
      } else {
        _throw IOException(this);
      }
      return bytesRead + size - context->avail_out;
    }
  case FINISHED:
    state = ENDED; // availableBytes == 0 and FINISHED => end
    break;
  case ENDED: // not possible
    break;
  }
  return bytesRead;
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

BZip2Inflater::~BZip2Inflater() noexcept
{
#if (defined(_COM_AZURE_DEV__BASE__USE_BZIP2))
  bz_stream* context = Cast::pointer<bz_stream*>(this->context);
  BZ2_bzDecompressEnd(context);
  delete context;
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
