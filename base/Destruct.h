/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_DESTRUCT_H
#define _BASE_DESTRUCT_H

#include "Exception.h"

/**
  Thrown on object deconstruction error. Used to detect resource leaks.

  @author René Møller Fonseca
  @version 1.0
*/

class Destruct : public Exception {
public:
  Destruct();
  ~Destruct();
};

#endif
