/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__PIPE_H
#define _DK_SDU_MIP__BASE_IO__PIPE_H

#include <base/Object.h>
#include <base/concurrency/Synchronize.h>
#include <base/io/TimedOut.h>
#include <base/io/FileDescriptor.h>
#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/collection/Pair.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class implements an anonymous pipe. MT-level is safe.

  @author René Møller Fonseca
  @version 1.2
*/

class Pipe : public virtual Object, public virtual InputStream, public virtual OutputStream, public virtual Synchronizeable<Unsafe> {
public:

  typedef Unsafe LOCK;

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
    ~PipeImpl() throw(IOException);
  };
protected:

  /** The internal pipe representation. */
  ReferenceCountedObjectPointer<PipeImpl> pipe;
public:

  /**
    Creates a new pair of pipes.
  */
  static Pair<Pipe, Pipe> make() throw(IOException);

  /**
    Initializes an invalidated pipe object.
  */
  Pipe() throw();

  /**
    Initialization of socket from other socket.
  */
  inline Pipe(const Pipe& copy) throw() : pipe(copy.pipe) {}

  /**
    Assignment of socket to socket.
  */
  inline Pipe& operator=(const Pipe& eq) throw() {
    if (&eq != this) { // protect against self assignment
      pipe = eq.pipe;
    }
    return *this;
  }

  /**
    Closes this pipe.
  */
  void close() throw(IOException);

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
  bool atEnd() const throw(IOException);

  /**
    Returns the number of bytes that can be read or skipped over without
    blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(IOException);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than available.
  */
  unsigned int skip(unsigned int count) throw(IOException);

  /**
    Forces any buffered bytes to be written out.
  */
  void flush() throw(IOException); 

  /**
    Fills the buffer with bytes from the pipe input stream. Blocks if asked
    to read more bytes than available.

    @param buffer The buffer.
    @param size The size of the buffer.
    @return The actual number of bytes read.
  */
  unsigned int read(char* buffer, unsigned int size) throw(IOException);

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @return The actual number of bytes written.
  */
  unsigned int write(const char* buffer, unsigned int size) throw(IOException);

  /**
    Blocking wait for input to become available.
  */
  void wait() const throw(IOException);

  /**
    Blocking wait for input to become available.

    @param timeout The timeout periode in microseconds.
    @return True, if data is available. False, if the timeout periode expired.
  */
  bool wait(unsigned int timeout) const throw(IOException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
