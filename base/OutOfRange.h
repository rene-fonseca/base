/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_OUT_OF_RANGE_H
#define _BASE_OUT_OF_RANGE_H

#include "Exception.h"

/**
  Exception used to specify that an index is out of range.

  @author René Møller Fonseca
  @version 1.0
*/

class OutOfRange : public Exception {
public:
  OutOfRange();
  ~OutOfRange();
};

#endif
