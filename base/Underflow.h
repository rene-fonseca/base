/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_UNDERFLOW_H
#define _BASE_UNDERFLOW_H

#include "Exception.h"

/**
  Thrown on value less than minimum valid value.

  @author René Møller Fonseca
  @version 1.0
*/

class Underflow : public Exception {
public:
  Underflow();
  ~Underflow();
};

#endif
