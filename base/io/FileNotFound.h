/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_NOT_FOUND_H
#define _DK_SDU_MIP__BASE_IO__FILE_NOT_FOUND_H

#include <base/io/FileException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Specifies that the file doesn't exist or couldn't be opened.

  @short File not found exception.
  @ingroup exceptions io
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileNotFound : public FileException {
public:

  /**
    Initializes the exception object with no message.
  */
  FileNotFound() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  FileNotFound(const char* message) throw();

  /**
    Initializes the exception object without an associated message.

    @param type The identity of the type.
  */
  inline FileNotFound(Type type) throw() : FileException(type) {
  }

  /**
    Initializes the exception object.

    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline FileNotFound(const char* message, Type type) throw()
    : FileException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
