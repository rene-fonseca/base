/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_DESTRUCT_H
#define _BASE_DESTRUCT_H

#include "Exception.h"

/**
  Thrown on object deconstruction error. Used to detect resource leaks.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class Destruct : public Exception {
public:
  Destruct();
  ~Destruct();
};

#endif
