/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_INPUT_STREAM_H

#include <base/io/FileDescriptorInputStream.h>
#include <base/io/FileNotFound.h>
#include <base/io/BindException.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  File input stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FileInputStream : public FileDescriptorInputStream {
public:

  /** The flags. */
  enum {NONBLOCK = 0x08};
protected:

  /** The path of the file. */
  String path;
public:

  /**
    Initializes the file input stream.

    @param path The path of the file.
    @param flags The desired flags.
  */
  FileInputStream(const String& name, unsigned int flags) throw(FileNotFound);

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(IOException);

  /**
    Returns the path of the file.
  */
  const String& getPath() const throw();

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

  /**
    Writes a string representation of a FileInputStream object to a stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const FileInputStream& value);
};

/**
  Writes a string representation of a FileInputStream object to a stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const FileInputStream& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
