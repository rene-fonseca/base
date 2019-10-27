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

#include <base/io/IOException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Exception raised on file object exceptions.
  
  @short File exception.
  @ingroup exceptions io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API FileException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline FileException() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline FileException(const char* message) throw() : IOException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline FileException(Type type) throw() : IOException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline FileException(const char* message, Type type) throw()
    : IOException(message, type) {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
