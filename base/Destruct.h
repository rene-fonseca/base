/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__DESTRUCT_H
#define _DK_SDU_MIP__BASE__DESTRUCT_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Thrown on object deconstruction error. Used to detect resource leaks.

  @author René Møller Fonseca
  @version 1.0
*/

class Destruct : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  Destruct() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Destruct(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
