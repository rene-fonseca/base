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
#include <base/Module.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_ZLIB)
#  include <zlib.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace internal {
  
  class ZLibInflater {
  public:
    
    static void* allocate(void* opaque, uInt n, uInt m) noexcept
    {
      MemorySize size = static_cast<MemorySize>(n) * m;
      if ((size < 0) || (size > PrimitiveTraits<unsigned int>::MAXIMUM))  {
        return 0;
      }
      return Heap::allocateNoThrow<uint8>(size);
    }
    
    static void release(void* opaque, void* memory) noexcept
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
};

#if defined(_COM_AZURE_DEV__BASE__USE_ZLIB)
MODULE_REGISTER_EXPLICIT(_COM_AZURE_DEV__BASE__THIS_MODULE, "net.zlib", "libzlib", ZLIB_VERSION, "https://zlib.net/");
#endif

ZLibInflater::ZLibInflater()
  : buffer(BUFFER_SIZE), availableBytes(0), state(RUNNING)
{
#if (defined(_COM_AZURE_DEV__BASE__USE_ZLIB))
  z_stream* context = new z_stream;
  this->context = context;
  clear(*context);
  context->zalloc = internal::ZLibInflater::allocate;
  context->zfree = internal::ZLibInflater::release;
  context->opaque = nullptr;
  int code = inflateInit_(context, ZLIB_VERSION, sizeof(*context));
  switch (code) {
  case internal::ZLibInflater::OK:
    break;
  case internal::ZLibInflater::VERSION_ERROR:
    _throw NotSupported(this);
  default:
    _throw MemoryException(this);
  }
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

MemorySize ZLibInflater::push(const uint8* buffer, MemorySize _size)
{
  if (_size > 0xffffffff) {
    _throw IOException(this);
  }
  unsigned int size = static_cast<unsigned int>(_size);
  
#if (defined(_COM_AZURE_DEV__BASE__USE_ZLIB))
  bassert(state != ENDED, EndOfFile());
  bassert(state == RUNNING, IOException(this));
  if (availableBytes == this->buffer.getSize()) {
    return 0; // no storage available
  }
  z_stream* context = Cast::pointer<z_stream*>(this->context);
  context->next_in = (Bytef*)static_cast<const uint8*>(buffer);
  context->avail_in = size;
  context->total_in = 0;
  context->next_out = this->buffer.getElements() + availableBytes;
  context->avail_out = static_cast<unsigned int>(this->buffer.getSize()) - availableBytes;
  int code = inflate(context, internal::ZLibInflater::NO_FLUSH);
  if (code == internal::ZLibInflater::OK) {
    // continue pushing
  } else if (code == internal::ZLibInflater::STREAM_END) {
    state = FINISHED; // stream may containg garbage data after eof (e.g. padding)
  } else {
    _throw IOException(this);
  }
  availableBytes = static_cast<unsigned int>(this->buffer.getSize()) - context->avail_out;
  return context->total_in;
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

void ZLibInflater::pushEnd()
{
#if (defined(_COM_AZURE_DEV__BASE__USE_ZLIB))
  if (state != ENDED) {
    bassert(state == RUNNING, IOException(this));
    state = FINISHING;
  }
#else
  _COM_AZURE_DEV__BASE__NOT_SUPPORTED();
#endif
}

MemorySize ZLibInflater::pull(uint8* buffer, MemorySize _size)
{
  if (_size > 0xffffffff) {
    _throw IOException(this);
  }
  unsigned int size = static_cast<unsigned int>(_size);
  
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
  
  MemorySize bytesRead = 0;
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
#if 0
    {
      z_stream* context = Cast::pointer<z_stream*>(this->context);
      context->next_in = 0;
      context->avail_in = 0;
      context->total_in = 0;
      context->next_out = buffer;
      context->avail_out = size;
      int code = inflate(context, internal::ZLibInflater::SYNC_FLUSH);
      if (code == internal::ZLibInflater::OK) {
        if (context->avail_out != 0) { // done flushing
          state = RUNNING;
        }
      } else if (code == internal::ZLibInflater::STREAM_END) { // TAG: is this possible
        state = FINISHED; // done flushing
      } else {
        _throw IOException(this);
      }
      return bytesRead + size - context->avail_out;
    }
#endif
  case FINISHING:
    {
      z_stream* context = Cast::pointer<z_stream*>(this->context);
      context->next_in = 0;
      context->avail_in = 0;
      context->total_in = 0;
      context->next_out = buffer;
      context->avail_out = size;
      int code = inflate(context, internal::ZLibInflater::SYNC_FLUSH); // TAG: do not use FINISH here
      if (code == internal::ZLibInflater::OK) {
        // continue finishing
      } else if (code == internal::ZLibInflater::STREAM_END) {
        state = ENDED; // availableBytes = 0 thus we skip FINISHED
      } else {
        _throw IOException(this);
      }
      return bytesRead + size - context->avail_out;
    }
  case FINISHED:
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

ZLibInflater::~ZLibInflater() noexcept
{
#if (defined(_COM_AZURE_DEV__BASE__USE_ZLIB))
  z_stream* context = Cast::pointer<z_stream*>(this->context);
  inflateEnd(context);
  delete context;
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
