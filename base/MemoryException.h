/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_MEMORY_EXCEPTION_H
#define _BASE_MEMORY_EXCEPTION_H

#include "ResourceException.h"

/**
  Thrown on memory allocation/deallocation error.

  @author René Møller Fonseca
  @version 1.0
*/

class MemoryException : public ResourceException {
public:
  MemoryException();
  ~MemoryException();
};

#endif
