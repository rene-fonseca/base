/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_RESOURCE_EXCEPTION_H
#define _DK_SDU_MIP_BASE_RESOURCE_EXCEPTION_H

#include "Exception.h"

/**
  Thrown on resource allocation/deallocation error.

  @author René Møller Fonseca
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
