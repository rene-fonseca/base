/***************************************************************************
    begin       : Fri May 19 2000
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _OUT_OF_DOMAIN_EXCEPTION_H
#define _OUT_OF_DOMAIN_EXCEPTION_H

#include "Exception.h"

/**
  Exception used to specify that a value is out of the domain.

  @author René Møller Fonseca
  @version 1.0
*/

class OutOfDomainException : public Exception {
public:

  /**
    Initializes the exception object.
  */
	OutOfDomainException();

  /**
    Destroys the exception object.
  */
	~OutOfDomainException();
};

#endif
