/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_DL__LINKER_EXCEPTION_H
#define _DK_SDU_MIP__BASE_DL__LINKER_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Exception thrown by DynamicLinker.
  @author René Møller Fonseca
  @version 1.0
*/

class LinkerException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  LinkerException() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  LinkerException(const char* message) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
