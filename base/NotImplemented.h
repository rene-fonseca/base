/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__NOT_IMPLEMENTED_H
#define _DK_SDU_MIP__BASE__NOT_IMPLEMENTED_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception used to specify that some context has not been fully implemented.

  @see NotSupported
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class NotImplemented : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  NotImplemented() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  NotImplemented(const char* message) throw();

  NotImplemented(Type type) throw() : Exception(type) {}
  
  NotImplemented(const char* message, Type type) throw() : Exception(message, type) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
