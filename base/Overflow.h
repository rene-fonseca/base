/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_OVERFLOW_H
#define _BASE_OVERFLOW_H

#include "Exception.h"

/**
  Thrown on value greater than maximum valid value.

  @author René Møller Fonseca
  @version 1.0
*/

class Overflow : public Exception {
public:
  Overflow();
  ~Overflow();
};

#endif
