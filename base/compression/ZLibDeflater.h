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
#include <base/compression/Deflater.h>
#include <base/mem/Allocator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  ZLIB deflater push stream.
  
  @short ZLIB deflater.
  @ingroup compression
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ZLibDeflater : public Object, public virtual Deflater {
public:
  
  /** The size of the compression buffer. */
  static const unsigned int BUFFER_SIZE = 64 * 1024;
  /** The default compression level. */
  static const unsigned int DEFAULT_COMPRESSION_LEVEL = 6;
private:
  
  enum State {
    RUNNING,
    FLUSHING,
    FINISHING,
    FINISHED,
    ENDED
  };
  
  /** Buffer holding compressed data. */
  Allocator<uint8> buffer;
  /** Specifies the number of compressed bytes currently available. */
  unsigned int availableBytes = 0;
  /** Opaque context. */
  void* context = nullptr;
  /** The current state. */
  State state = RUNNING;
public:

  /**
    Returns true if ZLib is supported by the runtime.
  */
  static bool isSupported() noexcept;

  /**
    Initializes the deflater with the default compression level.
  */
  ZLibDeflater();
  
  /**
    Initializes the deflater with the specified compression level.
    
    @param compressionLevel The compression level.
  */
  ZLibDeflater(unsigned int compressionLevel);
  
  /**
    Returns true if the end has been reached. This always returns false until
    pushEnd() has been invoked.
  */
  inline bool atEnd() const noexcept
  {
    return state == ENDED;
  }
  
  /**
    Flushes the stream. Invoke pull() one or more times to read the compressed
    data. The stream has been flushed when pull() returns 0.
  */
  void flush();
  
  /**
    Pushes uncompressed data onto the stream. This method may return 0 if the
    internal buffer is full. EndOfFile is raised if the end has been reached.
    IOException is raised unless the deflater is in the normal compression mode. 
    
    @param buffer The data to be pushed onto the stream.
    @param size The size of the buffer.
    
    @return The number of bytes pushed.
  */
  MemorySize push(const uint8* buffer, MemorySize size);
  
  /**
    This method should be invoked when all uncompressed data has been pushed
    onto the stream. Do not invoke flush(), push(), or pushEnd() hereafter.
  */
  void pushEnd();
  
  /**
    Pulls compressed bytes from the stream. EndOfFile is raised if atEnd()
    would return true. This method may return 0 in the normal deflation mode if
    the internal buffer is not full. Use flush() to force any buffered data
    through the pull() interface. Flush is completed when pull() returns 0.
    
    @param buffer The buffer to receive the compressed data.
    @param size The size of the buffer.
    
    @return The number of bytes pulled.
  */
  MemorySize pull(uint8* buffer, MemorySize size);
  
  /**
    Destroys the deflater.
  */
  virtual ~ZLibDeflater() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
