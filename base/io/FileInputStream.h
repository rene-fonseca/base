/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_INPUT_STREAM_H

#include "InputStream.h"
#include "FileDescriptor.h"
#include "FileNotFound.h"
#include "BindException.h"
#include "base/string/String.h"

/**
  File input stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FileInputStream : public InputStream, protected FileDescriptor {
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
    Initializes the file input stream with file descriptor as source of stream.

    @param handle The file descriptor.
  */
  FileInputStream(int handle) throw(BindException);

  bool atEnd() const throw();

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() throw(IOException);

  /**
    Closes the input stream and releases any system resources associated with the stream.
  */
  void close() throw(IOException);

  /**
    Fills the buffer with bytes from the stream. Blocks if asked to read more bytes than available.

    @param buffer The buffer.
    @param size The size of the buffer.
  */
  unsigned int read(char* buffer, unsigned int size) throw(IOException);

  /**
    Skips a specified number of bytes. Blocks if asked to skip more bytes than available.

    @param count The number of bytes to skip.
  */
  unsigned int skip(unsigned int count) throw(IOException);

  /**
    Destroys the file input stream.
  */
  ~FileInputStream();

  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const FileInputStream& value);
};

/**
  Writes a string representation of a file input stream to a stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const FileInputStream& value);

#endif
