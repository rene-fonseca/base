/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Asynchronous exception.

  @short Asynchronous exception
  @ingroup exceptions
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API AsynchronousException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  AsynchronousException() throw();

  /**
    Initializes exception object as unspecified.

    @param message The message.
  */
  AsynchronousException(const char* message) throw();

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline AsynchronousException(Type type) throw() : IOException(type) {
  }

  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */  
  inline AsynchronousException(const char* message, Type type) throw()
    : IOException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
