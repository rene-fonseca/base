/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__TIMED_OUT_H
#define _DK_SDU_MIP__BASE_IO__TIMED_OUT_H

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception raised on timed out IO requests.

  @short Time out exception.
  @ingroup exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class TimedOut : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  TimedOut() throw();

  /**
    Initializes exception object as unspecified.

    @param message The message.
  */
  TimedOut(const char* message) throw();
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  TimedOut(Type type) throw() : IOException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  TimedOut(const char* message, Type type) throw() : IOException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
