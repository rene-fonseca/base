/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__ORB_EXCEPTION_H
#define _DK_SDU_MIP__BASE_RMI__ORB_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker (ORB) exception.
  
  @short Object Request Broker (ORB) exception.
  @ingroup exceptions rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  OrbException() throw();
  
  /**
    Initializes the exception object.

    @param message The message.
  */
  OrbException(const char* message) throw();
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  OrbException(Type type) throw();
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  OrbException(const char* message, Type type) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
