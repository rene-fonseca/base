/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_OUT_OF_BOUNDS_H
#define _BASE_OUT_OF_BOUNDS_H

#include "Exception.h"

/**
  Out of bounds exception.

  @author René Møller Fonseca
  @version 1.0
*/

class OutOfBounds : public Exception {
public:
  OutOfBounds();
  ~OutOfBounds();
};

#endif
