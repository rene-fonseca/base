/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__FORMAT_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_STRING__FORMAT_INPUT_STREAM_H

#include <base/io/BufferedInputStream.h>
#include <base/io/BindException.h>
#include <base/io/EndOfFile.h>
#include <base/string/InvalidFormat.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Format input stream. This class is not MT-safe.

  @see FormatOutputStream
  @short Reader of formatted input.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FormatInputStream : public FilterInputStream {
public:

  /** Specifies the default line granularity. */
  static const unsigned int LINE_GRANULARITY = 1024;
  /** Specifies the window size. */
  static const unsigned int WINDOW_SIZE = 4096;
protected:

  typedef Allocator<uint8>::Iterator Iterator;
  typedef Allocator<uint8>::ReadIterator ReadIterator;

  /** The internal buffer. */
  Allocator<uint8> buffer;
  /** The current write position. */
  Iterator head;
  /** The current read position. */
  ReadIterator tail;
  /** Preallocated line. */
  String line;
  
  /**
    Fills the internal buffer with bytes from the input stream. Removes any
    characters already in the buffer.
  */
  bool overwriteFromSource() throw(IOException);

  /**
    Fills the internal buffer with bytes from the input stream without removing
    any characters already in the buffer.
  */
  bool appendFromSource() throw(IOException);
public:

  /**
    Initializes the format input stream.

    @param in The input stream.
  */
  FormatInputStream(InputStream& in) throw(BindException);

  /**
    Returns the number of bytes that can be read or skipped over without
    blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const throw(IOException);

  /**
    Reads one character from the stream.
  */
  inline char getCharacter() throw(IOException) {
    if (tail == head) {
      assert(overwriteFromSource(), EndOfFile());
    }
    return *tail++;
  }

  /**
    Reads one word from the stream.
  */
  String getWord() throw(IOException);

  /**
    Reads one line from the stream.
  */
  String getLine() throw(IOException);

  /**
    Fills the specified buffer with bytes from the stream. Blocks if asked to
    read more bytes than available.

    @param buffer The buffer to receive the bytes.
    @param size The size of the buffer.
    @param nonblocking Specifies that the method may not block. Default is
    false.
    @return The actual number of bytes read from the stream.
  */
  unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking = false) throw(IOException);

  /**
    Destroys the format input stream.
  */
  ~FormatInputStream();
};

/**
  Format input stream linked to the standard input stream. This variable
  corresponds to 'cin' from the Standard Template Library.
*/
extern FormatInputStream fin;

/**
  Reads one character from the format input stream.
*/
FormatInputStream& operator>>(
  FormatInputStream& stream, char& value) throw(IOException);

/**
  Reads one line from the format input stream.
*/
FormatInputStream& operator>>(
  FormatInputStream& stream, String& value) throw(IOException);

/**
  Reads unsigned int from the format input stream.
*/
FormatInputStream& operator>>(
  FormatInputStream& stream,
  unsigned int& value) throw(InvalidFormat, IOException);

//FormatInputStream& operator>>(FormatInputStream& stream, short int& value);
//FormatInputStream& operator>>(FormatInputStream& stream, unsigned short int& value);
//FormatInputStream& operator>>(FormatInputStream& stream, int& value);
//FormatInputStream& operator>>(FormatInputStream& stream, long long int& value);
//FormatInputStream& operator>>(FormatInputStream& stream, unsigned long long int& value);
//FormatInputStream& operator>>(FormatInputStream& stream, float& value);
//FormatInputStream& operator>>(FormatInputStream& stream, double& value);
//FormatInputStream& operator>>(FormatInputStream& stream, long double& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
