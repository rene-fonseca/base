/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_RESOURCE_EXCEPTION_H
#define _BASE_RESOURCE_EXCEPTION_H

#include "Exception.h"

/**
  Thrown on resource allocation/deallocation error.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class ResourceException : public Exception {
public:
  ResourceException();
  ~ResourceException();
};

#endif
