/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OVERFLOW_H
#define _DK_SDU_MIP__BASE__OVERFLOW_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception indicates that a resulting value is greater than the maximum
  positive representable value using a given format/type. An overflow condition
  will occur if the result of adding two 32 bit signed integers is greater than
  2^31-1.

  @see Underflow
  @author René Møller Fonseca
  @version 1.0
*/

class Overflow : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  Overflow() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Overflow(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
