/***************************************************************************
    begin       : Fri May 19 2000
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _OUT_OF_BOUNDS_EXCEPTION_H
#define _OUT_OF_BOUNDS_EXCEPTION_H

#include "Exception.h"

/**
  Out of bounds exception.

  @author René Møller Fonseca
  @version 1.0
*/

class OutOfBoundsException : public Exception {
public: 

  /**
    Initializes the exception.
  */
	OutOfBoundsException();

  /**
    Destroys the exception.
  */
	~OutOfBoundsException();
};

#endif
