/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_OVERFLOW_H
#define _BASE_OVERFLOW_H

#include "Exception.h"

/**
  Thrown on value greater than maximum valid value.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class Overflow : public Exception {
public:
  Overflow();
  ~Overflow();
};

#endif
