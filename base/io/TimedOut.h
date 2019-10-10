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
  Exception raised on timed out IO requests.

  @short Time out exception.
  @ingroup exceptions io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API TimedOut : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline TimedOut() throw() {
  }

  /**
    Initializes exception object as unspecified.

    @param message The message.
  */
  inline TimedOut(const char* message) throw() : IOException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline TimedOut(Type type) throw() : IOException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline TimedOut(const char* message, Type type) throw()
    : IOException(message, type) {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
