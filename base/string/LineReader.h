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
#include <base/string/String.h>
#include <base/collection/Array.h>

// TAG: add support for encodings
// TAG: validate/repair UTF8 support

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Line reader. */
class LineReader {
private:

  BufferedInputStream bis;
  char skipCode = 0;
public:

  /** Initializes line reader. */
  LineReader() noexcept
  {
  }

  /** Initializes line reader. */
  LineReader(InputStream* is) noexcept
    : bis(*is)
  {
  }

  /** Returns true if eof has not been reached. */
  inline bool hasMore() const noexcept
  {
    return bis.available();
  }

  /** Reads the next line. */
  String readLine();

  /** Reads all lines. */
  Array<String> readLines();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
