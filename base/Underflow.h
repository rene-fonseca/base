/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__UNDERFLOW_H
#define _DK_SDU_MIP__BASE__UNDERFLOW_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception indicates that a resulting value is less than the maximum
  negative representable value using a given format/type. An underflow
  condition will occur if the result of subtracting two 32 bit signed integers
  is less than -2^31.

  @see Overflow
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Underflow : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  Underflow() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Underflow(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
