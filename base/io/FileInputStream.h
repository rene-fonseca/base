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
#include <base/io/InputStream.h>
#include <base/io/File.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  File input stream.

  @short File input stream.
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API FileInputStream : public virtual Object, public InputStream {
protected:

  /** The file providing the stream. */
  File file;
  /** Specifies that the end of the file has been reached. */
  bool end = false;
public:

  /**
    Initializes the file input stream.

    @param path The path of the file.
    @param exclusive Specifies that the file should be opened in exclusive
    mode.
  */
  FileInputStream(
    const String& name, bool exclusive = true);

  inline bool atEnd() const {
    return end;
  }

  /**
    Returns the number of bytes that can be read or skipped over without
    blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const;

  void close();

  unsigned int skip(unsigned int count);

  inline unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking = false) {
    return file.read(buffer, size, nonblocking);
  }

  /**
    Blocking wait for input to become available. This method does nothing for a
    file.
  */
  void wait() const;

  /**
    Blocking wait for input to become available. This method does nothing for a
    file.

    @param timeout The timeout periode in microseconds.
    @return Always returns true.
  */
  bool wait(unsigned int timeout) const;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
