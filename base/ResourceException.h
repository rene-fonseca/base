/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_RESOURCE_EXCEPTION_H
#define _DK_SDU_MIP_BASE_RESOURCE_EXCEPTION_H

#include "Exception.h"

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
  ResourceException();

  /**
    Initializes the exception object.

    @param message The message.
  */
  ResourceException(const string& message);

  /**
    Destroys exception object.
  */
  ~ResourceException();
};

#endif
