/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__PIPE_EXCEPTION_H
#define _DK_SDU_MIP__BASE_IO__PIPE_EXCEPTION_H

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown on pipe object exceptions.

  @author René Møller Fonseca
  @version 1.0
*/

class PipeException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  PipeException() throw() : IOException() {}

  /**
    Initializes the exception object.

    @param message The message.
  */
  PipeException(const char* message) throw() : IOException(message) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
