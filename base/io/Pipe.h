/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__PIPE_H
#define _DK_SDU_MIP__BASE_IO__PIPE_H

#include <base/Object.h>
#include <base/io/TimedOut.h>
#include <base/io/FileDescriptor.h>
#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>
#include <base/io/PipeException.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/collection/Pair.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class implements an anonymous pipe. MT-level is safe.

  @author René Møller Fonseca
  @version 1.2
*/

class Pipe : public virtual Object, public virtual InputStream, public virtual OutputStream {
public:

  class PipeImpl : public virtual ReferenceCountedObject {
  private:

    /** Handle to the pipe. */
    int handle;
  public:

    /** Initializes invalid pipe. */
    inline PipeImpl() throw() : handle(-1) {}
    /** Initializes pipe by handle. */
    PipeImpl(int handle) throw() : handle(handle) {}
    /** Returns the handle. */
    inline int getHandle() const throw() {return handle;}
    /** Releases the resources use by the pipe. */
    ~PipeImpl() throw(PipeException);
  };
protected:

  /** The internal pipe representation. */
  ReferenceCountedObjectPointer<PipeImpl> fd;
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
    Initialization of socket from other socket.
  */
  inline Pipe(const Pipe& copy) throw() : fd(copy.fd) {}

  /**
    Assignment of socket to socket.
  */
  inline Pipe& operator=(const Pipe& eq) throw() {
    if (&eq != this) { // protect against self assignment
      fd = eq.fd;
    }
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
