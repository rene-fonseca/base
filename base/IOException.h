/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_IO_EXCEPTION_H
#define _BASE_IO_EXCEPTION_H

#include "Exception.h"

/**
  Thrown on IO error.

  @author René Møller Fonseca
  @version 1.0
*/

class IOException : public Exception {
public:
  IOException();
  ~IOException();
};

#endif
