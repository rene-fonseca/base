/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_UNDERFLOW_H
#define _BASE_UNDERFLOW_H

#include "Exception.h"

/**
  Thrown on value less than minimum valid value.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class Underflow : public Exception {
public:
  Underflow();
  ~Underflow();
};

#endif
