/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__END_OF_ENUMERATION_H
#define _DK_SDU_MIP__BASE_COLLECTION__END_OF_ENUMERATION_H

#include <base/features.h>
#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**

  @author Ren� M�ller Fonseca
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
