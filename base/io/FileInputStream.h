/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_INPUT_STREAM_H

#include <base/io/InputStream.h>
#include <base/io/File.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  File input stream.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class FileInputStream : public InputStream {
protected:

  /** The file providing the stream. */
  File file;
  /** Specifies that the end of the file has been reached. */
  bool end;
public:

  /**
    Initializes the file input stream.

    @param path The path of the file.
    @param exclusive Specifies that the file should be opened in exclusive mode.
  */
  FileInputStream(const String& name, bool exclusive = true) throw(FileNotFound);

  inline bool atEnd() const throw(FileException) {return end;}

  /**
    Returns the number of bytes that can be read or skipped over without blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(FileException);

  void close() throw(FileException);

  unsigned int skip(unsigned int count) throw(FileException);

  inline unsigned int read(char* buffer, unsigned int size, bool nonblocking = false) throw(FileException) {
    return file.read(buffer, size, nonblocking);
  }

  /**
    Blocking wait for input to become available. This method does nothing for a
    file.
  */
  void wait() const throw(FileException);

  /**
    Blocking wait for input to become available. This method does nothing for a
    file.

    @param timeout The timeout periode in microseconds.
    @return Always returns true.
  */
  bool wait(unsigned int timeout) const throw(FileException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
