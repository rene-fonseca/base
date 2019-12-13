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
#include <base/io/TimedOut.h>
#include <base/io/FileDescriptor.h>
#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>
#include <base/io/PipeException.h>
#include <base/mem/Reference.h>
#include <base/collection/Pair.h>
#include <base/io/Handle.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class implements an anonymous pipe. MT-level is safe.
  
  @short Pipe.
  @ingroup io
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API Pipe : public virtual Object, public virtual InputStream, public virtual OutputStream {
public:

  class PipeHandle : public Handle {
    friend class Initialization;
    friend class Pipe;
  private:

    /** Invalid handle. */
    static Handle* invalid;

    /** Initializes pipe handle. */
    inline PipeHandle(OperatingSystem::Handle handle) noexcept : Handle(handle) {
    }
    
    /** Releases the resources used by the pipe. */
    ~PipeHandle();
  };
protected:

  /** The internal pipe representation. */
  Reference<Handle> fd;
  /** Specifies that the end has been reached. */
  bool end = false;
public:

  /**
    Creates a new pair of pipes.
  */
  static Pair<Pipe, Pipe> make() throw(PipeException);

  /**
    Initializes an invalidated pipe object.
  */
  Pipe() noexcept;

  /**
    Initialization of pipe from other pipe.
  */
  inline Pipe(const Pipe& copy) noexcept : fd(copy.fd) {
  }

  /**
    Assignment of pipe to pipe.
  */
  inline Pipe& operator=(const Pipe& assign) noexcept {
    // no need to protect against self assignment
    fd = assign.fd;
    end = assign.end;
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

  unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking = false) throw(PipeException);

  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false) throw(PipeException);

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
  ~Pipe();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
