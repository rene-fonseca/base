/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_MEMORY_EXCEPTION_H
#define _BASE_MEMORY_EXCEPTION_H

#include "ResourceException.h"

/**
  Thrown on memory allocation/deallocation error.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class MemoryException : public ResourceException {
public:
  MemoryException();
  ~MemoryException();
};

#endif
