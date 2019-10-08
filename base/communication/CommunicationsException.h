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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception raised by communications ports.

  @short General communications exception
  @ingroup exceptions communications
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API CommunicationsException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  CommunicationsException() throw();

  /**
    Initializes exception object as unspecified.

    @param message The message.
  */
  CommunicationsException(const char* message) throw();

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  CommunicationsException(Type type) throw() : IOException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  CommunicationsException(const char* message, Type type) throw() : IOException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
