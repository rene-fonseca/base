/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_CONSTRUCT_H
#define _BASE_CONSTRUCT_H

#include "Exception.h"

/**
  Thrown on object construction error.

  @author René Møller Fonseca
  @version 1.0
*/

class Construct : public Exception {
public:
  Construct();
  ~Construct();
};

#endif
