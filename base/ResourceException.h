/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_RESOURCE_EXCEPTION_H
#define _BASE_RESOURCE_EXCEPTION_H

#include "Exception.h"

/**
  Thrown on resource allocation/deallocation error.

  @author René Møller Fonseca
  @version 1.0
*/

class ResourceException : public Exception {
public:
  ResourceException();
  ~ResourceException();
};

#endif
