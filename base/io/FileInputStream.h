/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_INPUT_STREAM_H

#include "base/Object.h"
#include "InputStream.h"
#include "FileNotFound.h"
#include "FileDescriptor.h"

/**
  File input stream.

  @author René Møller Fonseca
  @version 1.0
*/

class FileInputStream : public Object, public InputStream {
protected:

  /** File descriptor. */
  FileDescriptor* fd;
public:

  /**
    Initializes the file input stream.

    @param name The name of the file.
  */
  FileInputStream(const char* name) throw(FileNotFound);

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available();

  /**
    Closes the input stream and releases any system resources associated with the stream.
  */
  void close() throw(IOException);

  /**
    Reads the next byte from the stream. Blocks if no bytes are available.

    @return The next byte from the stream.
  */
  int read() throw(IOException);

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
  void skip(unsigned int count) throw(IOException);

  /**
    Destroys the file input stream.
  */
  ~FileInputStream();
};

#endif
