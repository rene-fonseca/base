/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__SERVICE_NOT_FOUND_H
#define _DK_SDU_MIP__BASE_NET__SERVICE_NOT_FOUND_H

#include <base/net/NetworkException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception specifies that the service could not be resolved.

  @short Internet Protocol service exception.
  @ingroup exceptions net
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ServiceNotFound : public NetworkException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline ServiceNotFound() throw() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline ServiceNotFound(const char* message) throw()
    : NetworkException(message) {
  }
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline ServiceNotFound(Type type) throw() : NetworkException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline ServiceNotFound(const char* message, Type type) throw()
    : NetworkException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
