/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__END_OF_FILE_H
#define _DK_SDU_MIP__BASE_IO__END_OF_FILE_H

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Specifies that the end of stream has been reached unexpectedly during input.

  @short End of file exception
  @ingroup exceptions io
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class EndOfFile : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline EndOfFile() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  
  inline EndOfFile(const char* message) throw() : IOException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline EndOfFile(Type type) throw() : IOException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline EndOfFile(const char* message, Type type) throw()
    : IOException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
