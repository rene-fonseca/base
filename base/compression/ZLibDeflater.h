/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COMPRESSION__ZLIB_DEFLATER_H
#define _DK_SDU_MIP__BASE_COMPRESSION__ZLIB_DEFLATER_H

#include <base/Object.h>
#include <base/compression/Deflater.h>
#include <base/mem/Allocator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  ZLIB deflater push stream.
  
  @short ZLIB deflater.
  @ingroup compression
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ZLibDeflater : public Object, public virtual Deflater {
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
    COMPLETED
  };
  
  /** Buffer holding compressed data. */
  Allocator<uint8> buffer;
  /** Specifies the number of compressed bytes currently available. */
  unsigned int availableBytes;
  /** Opaque context. */
  void* context;
  /** The current state. */
  State state;
public:
  
  /**
    Initializes the deflater.
  */
  ZLibDeflater() throw(MemoryException);
  
  /**
    Initializes the deflater with the specified compression level.
    
    @param compressionLevel The compression level.
  */
  ZLibDeflater(unsigned int compressionLevel) throw(MemoryException);
  
  /**
    Returns true if the end has been reached. This always returns false until
    pushEnd() has been invoked.
  */
  inline bool atEnd() const throw() {
    return (availableBytes == 0) && (state == COMPLETED);
  }
  
  /**
    Flushes the stream. The stream has been flushed when pull() returns 0.
  */
  void flush() throw(IOException);
  
  /**
    Pushes the bytes of the buffer onto the stream.
    
    @param buffer The data to be pushed onto the stream.
    @param size The size of the buffer.
    
    @return The number of bytes pushed.
  */
  unsigned int push(const uint8* buffer, unsigned int size) throw(IOException);
  
  /**
    This method should be invoked when the end has been reached. Do not invoke
    flush(), push(), or pushEnd() hereafter.
  */
  void pushEnd() throw(IOException);
  
  /**
    Pulls bytes from the stream. EndOfFile is raised if atEnd() would return
    true.
    
    @param buffer The buffer to receive the data.
    @param size The size of the buffer.
    
    @return The number of bytes pulled.
  */
  unsigned int pull(uint8* buffer, unsigned int size) throw(IOException);
  
  /**
    Destroys the deflater.
  */
  virtual ~ZLibDeflater() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
