/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__PIPE_H
#define _DK_SDU_MIP__BASE_IO__PIPE_H

#include <base/Object.h>
#include <base/io/TimedOut.h>
#include <base/io/FileDescriptor.h>
#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>
#include <base/io/PipeException.h>
#include <base/mem/Reference.h>
#include <base/collection/Pair.h>
#include <base/io/Handle.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class implements an anonymous pipe. MT-level is safe.
  
  @short Pipe.
  @ingroup io
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class Pipe : public virtual Object, public virtual InputStream, public virtual OutputStream {
public:

  class PipeHandle : public Handle {
    friend class Initialization;
    friend class Pipe;
  private:

    /** Invalid handle. */
    static Handle* invalid;

    /** Initializes pipe handle. */
    inline PipeHandle(OperatingSystem::Handle handle) throw() : Handle(handle) {
    }
    
    /** Releases the resources used by the pipe. */
    ~PipeHandle() throw(PipeException);
  };
protected:

  /** The internal pipe representation. */
  Reference<Handle> fd;
  /** Specifies that the end has been reached. */
  bool end;
public:

  /**
    Creates a new pair of pipes.
  */
  static Pair<Pipe, Pipe> make() throw(PipeException);

  /**
    Initializes an invalidated pipe object.
  */
  Pipe() throw();

  /**
    Initialization of pipe from other pipe.
  */
  inline Pipe(const Pipe& copy) throw() : fd(copy.fd) {
  }

  /**
    Assignment of pipe to pipe.
  */
  inline Pipe& operator=(const Pipe& eq) throw() {
    // no need to protect against self assignment
    fd = eq.fd;
    end = eq.end;
    return *this;
  }

  /**
    Closes this pipe.
  */
  void close() throw(PipeException);

  /**
    Returns the size of the internal buffer of the pipe. The data may be
    interleaved when writing to the pipe from multiple threads simultaneously.
    To avoid the data being interleaved the packet size must not exceed this
    buffer size.
  */
  unsigned int getBufferSize() const throw();

  /**
    Returns true if the end of the stream has been reached.
  */
  bool atEnd() const throw(PipeException);

  /**
    Returns the number of bytes that can be read or skipped over without
    blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(PipeException);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than
    currently available.
  */
  unsigned int skip(unsigned int count) throw(PipeException);

  /**
    Forces any buffered bytes to be written out.
  */
  void flush() throw(PipeException);

  unsigned int read(char* buffer, unsigned int size, bool nonblocking = false) throw(PipeException);

  unsigned int write(const char* buffer, unsigned int size, bool nonblocking = false) throw(PipeException);

  /**
    Blocking wait for input to become available.
  */
  void wait() const throw(PipeException);

  /**
    Blocking wait for input to become available.

    @param timeout The timeout periode in microseconds.
    @return True, if data is available. False, if the timeout periode expired.
  */
  bool wait(unsigned int timeout) const throw(PipeException);

  /**
    Destroys the pipe.
  */
  ~Pipe() throw(IOException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
