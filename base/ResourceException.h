/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__RESOURCE_EXCEPTION_H
#define _DK_SDU_MIP__BASE__RESOURCE_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Thrown on resource allocation/deallocation error.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class ResourceException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  ResourceException() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  ResourceException(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
