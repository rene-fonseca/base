/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_NET__NETWORK_EXCEPTION_H
#define _DK_SDU_MIP__BASE_NET__NETWORK_EXCEPTION_H

#include <base/io/IOException.h>

/**
  Indicates a network exception.

  @author Ren� M�ller Fonseca
  @version 1.01
*/

class NetworkException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  NetworkException() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  NetworkException(const char* message) throw();
};

#endif
