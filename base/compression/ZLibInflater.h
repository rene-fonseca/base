/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COMPRESSION__ZLIB_INFLATER_H
#define _DK_SDU_MIP__BASE_COMPRESSION__ZLIB_INFLATER_H

#include <base/Object.h>
#include <base/compression/Inflater.h>
#include <base/mem/Allocator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  ZLIB inflater push stream.
  
  @short ZLIB inflater.
  @ingroup compression
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ZLibInflater : public Object, public virtual Inflater {
public:
  
  /** The size of the decompression buffer. */
  static const unsigned int BUFFER_SIZE = 64 * 1024;
private:
  
  enum State {
    RUNNING,
    FLUSHING,
    FINISHING,
    FINISHED,
    ENDED
  };
  
  /** Buffer holding decompressed data. */
  Allocator<uint8> buffer;
  /** Specifies the number of decompressed bytes currently available. */
  unsigned int availableBytes;
  /** Opaque context. */
  void* context;
  /** The current state. */
  State state;
public:
  
  /**
    Initializes the inflater.
  */
  ZLibInflater() throw(MemoryException);  
  
  /**
    Returns true if the end has been reached. This may return true before
    pushEnd() has been invoked if the compressed data has been padded with
    garbage.
  */
  inline bool atEnd() const throw() {
    return state == ENDED;
  }
  
  /**
    Pushes compressed data onto the stream. This method may return 0 if the
    internal buffer is full. EndOfFile is raised if the end has been reached.
    IOException is raised unless the inflater is in the normal decompression
    mode.
    
    @param buffer The data to be pushed onto the stream.
    @param size The size of the buffer.
    
    @return The number of bytes pushed.
  */
  unsigned int push(const uint8* buffer, unsigned int size) throw(IOException);
  
  /**
    This method should be invoked when all compressed data has been pushed onto
    the stream. Do not invoke push() or pushEnd() hereafter.
  */
  void pushEnd() throw(IOException);
  
  /**
    Pulls decompressed data from the stream. EndOfFile is raised if atEnd()
    would return true.
    
    @param buffer The buffer to receive the data.
    @param size The size of the buffer.
    
    @return The number of bytes pulled.
  */
  unsigned int pull(uint8* buffer, unsigned int size) throw(IOException);
  
  /**
    Destroys the inflater.
  */
  virtual ~ZLibInflater() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
