/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__DEVICE_EXCEPTION_H
#define _DK_SDU_MIP__BASE__DEVICE_EXCEPTION_H

#include <base/ResourceException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is raised on device access errors.

  @short Device access error
  @ingroup exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class DeviceException : public ResourceException {
public:

  /**
    Initializes the exception object with no message.
  */
  DeviceException() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  DeviceException(const char* message) throw();

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline DeviceException(Type type) throw() : ResourceException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline DeviceException(const char* message, Type type) throw()
    : ResourceException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
