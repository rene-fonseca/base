/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_IO_EXCEPTION_H
#define _BASE_IO_EXCEPTION_H

#include "Exception.h"

/**
  Thrown on IO error.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class IOException : public Exception {
public:
  IOException();
  ~IOException();
};

#endif
