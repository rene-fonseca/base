/***************************************************************************
    begin       : Fri May 19 2000
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _OUT_OF_BOUNDS_EXCEPTION_H
#define _OUT_OF_BOUNDS_EXCEPTION_H

#include "Exception.h"

/**
  Out of bounds exception.

  @author Ren� M�ller Fonseca
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
