/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_INPUT_STREAM_H
#define _DK_SDU_MIP__BASE_IO__FILE_INPUT_STREAM_H

#include "FileDescriptorInputStream.h"
#include "FileNotFound.h"
#include "BindException.h"
#include "base/string/String.h"

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
  String<> path;
public:

  /**
    Initializes the file input stream.

    @param path The path of the file.
    @param flags The desired flags.
  */
  FileInputStream(const String<>& name, unsigned int flags) throw(FileNotFound);

  /**
    Returns the path of the file.
  */
  const String<>& getPath() const throw();

  /**
    Writes a string representation of a FileInputStream object to a stream.
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const FileInputStream& value);
};

/**
  Writes a string representation of a FileInputStream object to a stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const FileInputStream& value);

#endif
