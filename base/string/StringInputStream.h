/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__STRING_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_STRING__STRING_INPUT_STREAM_H

#include <base/io/InputStream.h>
#include <base/io/EndOfFile.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A string input stream is a input stream which receives its data from a string.

  @author René Møller Fonseca
  @version 1.0
*/

class StringInputStream : public InputStream, protected String<> {
protected:

  /** String providing the data stream. */
  String<>& str;
  /** The current position. */
  unsigned int index;
  /** Specifies that the end has been reached. */
  bool eof;
  /** Specifies that the stream has been closed. */
  bool closed;
public:

  /**
    Initializes the string input stream.

    @param s The string providing the data stream.
  */
  StringInputStream(String<>& s) throw(BindException);

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
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
