/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__RMI_EXCEPTION_H
#define _DK_SDU_MIP__BASE_RMI__RMI_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Remote Method Invocation (RMI) exception.
  
  @short Remote Method Invocation (RMI) exception
  @ingroup exceptions rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class RMIException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  RMIException() throw();
  
  /**
    Initializes the exception object.

    @param message The message.
  */
  RMIException(const char* message) throw();
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  RMIException(Type type) throw();
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  RMIException(const char* message, Type type) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
