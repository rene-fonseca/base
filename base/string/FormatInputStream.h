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

#include <base/io/BufferedInputStream.h>
#include <base/io/BindException.h>
#include <base/io/EndOfFile.h>
#include <base/string/InvalidFormat.h>
#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Format input stream. This class is not MT-safe.

  @see FormatOutputStream
  @short Reader of formatted input.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API FormatInputStream : public FilterInputStream {
public:

  /** Specifies the default line granularity. */
  static constexpr unsigned int LINE_GRANULARITY = 1024;
  /** Specifies the window size. */
  static constexpr unsigned int WINDOW_SIZE = 4096;
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
  bool overwriteFromSource();

  /**
    Fills the internal buffer with bytes from the input stream without removing
    any characters already in the buffer.
  */
  bool appendFromSource();
public:

  /**
    Initializes the format input stream.

    @param in The input stream.
  */
  FormatInputStream(InputStream& in);

  /**
    Returns the number of bytes that can be read or skipped over without
    blocking.

    @return Available number of bytes in stream.
  */
  unsigned int available() const;

  /**
    Reads one character from the stream.
  */
  inline char getCharacter()
  {
    if (tail == head) {
      bassert(overwriteFromSource(), EndOfFile());
    }
    return *tail++;
  }

  /**
    Reads one word from the stream.
  */
  String getWord();

  /**
    Reads one line from the stream.
  */
  String getLine();

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
    bool nonblocking = false);

  /**
    Destroys the format input stream.
  */
  ~FormatInputStream();
};

/**
  Format input stream linked to the standard input stream. This variable
  corresponds to 'cin' from the Standard C++ Library.
*/
extern _COM_AZURE_DEV__BASE__API FormatInputStream fin;

/**
  Reads one character from the format input stream.
*/
_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(
  FormatInputStream& stream, char& value);

/**
  Reads one line from the format input stream.
*/
_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(
  FormatInputStream& stream, String& value);

/**
  Reads unsigned int from the format input stream.
*/
_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(
  FormatInputStream& stream,
  unsigned int& value);

//_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(FormatInputStream& stream, short int& value);
//_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(FormatInputStream& stream, unsigned short int& value);
//_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(FormatInputStream& stream, int& value);
//_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(FormatInputStream& stream, long long int& value);
//_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(FormatInputStream& stream, unsigned long long int& value);
//_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(FormatInputStream& stream, float& value);
//_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(FormatInputStream& stream, double& value);
//_COM_AZURE_DEV__BASE__API FormatInputStream& operator>>(FormatInputStream& stream, long double& value);

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
