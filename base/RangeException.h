/***************************************************************************
    begin       : Fri May 19 2000
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _RANGE_EXCEPTION_H
#define _RANGE_EXCEPTION_H

#include "Exception.h"

/**
  Exception used to specify that an index is out of range.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class RangeException : public Exception  {
public: 

  /**
    Initializes the exception object.
  */
	RangeException();

  /**
    Destroys the exception object.
  */
	~RangeException();
};

#endif
