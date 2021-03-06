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
#include <base/io/FileDescriptor.h>
#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>
#include <base/mem/Reference.h>
#include <base/collection/Pair.h>
#include <base/Resource.h>
#include <base/io/Handle.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class implements an anonymous pipe. MT-level is safe.
  
  @short Pipe.
  @ingroup io
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API Pipe : public Resource, public virtual InputStream, public virtual OutputStream {
public:

  /**
    Creates a new pair of pipes.
  */
  static Pair<Pipe, Pipe> make();

  /**
    Initializes an invalidated pipe object.
  */
  Pipe() noexcept;

  /**
    Closes this pipe.
  */
  void close();

  /**
    Returns the size of the internal buffer of the pipe. The data may be
    interleaved when writing to the pipe from multiple threads simultaneously.
    To avoid the data being interleaved the packet size must not exceed this
    buffer size.
  */
  unsigned int getBufferSize() const;

  /**
    Returns true if the end of the stream has been reached.
  */
  bool atEnd() const;

  /**
    Returns the number of bytes that can be read or skipped over without
    blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const;

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than
    currently available.
  */
  unsigned int skip(unsigned int count);

  /**
    Forces any buffered bytes to be written out.
  */
  void flush();

  unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  /**
    Blocking wait for input to become available.
  */
  void wait() const;

  /**
    Blocking wait for input to become available.

    @param timeout The timeout periode in microseconds.
    @return True, if data is available. False, if the timeout periode expired.
  */
  bool wait(unsigned int timeout) const;

  /**
    Destroys the pipe.
  */
  ~Pipe();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
