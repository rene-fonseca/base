/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__END_OF_ENUMERATION_H
#define _DK_SDU_MIP__BASE_COLLECTION__END_OF_ENUMERATION_H

#include "base/Exception.h"

/**

  @author René Møller Fonseca
  @version 1.0
*/

class EndOfEnumeration : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  EndOfEnumeration();

  /**
    Initializes the exception object.

    @param message The message.
  */
  EndOfEnumeration(const char* message);
};

#endif
