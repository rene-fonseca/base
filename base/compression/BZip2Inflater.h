/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/compression/Inflater.h>
#include <base/mem/Allocator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  BZip2 inflater push stream.
  
  @short BZip2 inflater.
  @ingroup compression
  @version 1.0
*/

class BZip2Inflater : public Object, public virtual Inflater {
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
  unsigned int availableBytes = 0;
  /** Opaque context. */
  void* context = nullptr;
  /** The current state. */
  State state = ENDED;
public:
  
  /**
    Initializes the inflater.
  */
  BZip2Inflater() throw(MemoryException);
  
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
  virtual ~BZip2Inflater() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
