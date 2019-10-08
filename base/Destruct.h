/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Raised on object deconstruction errors. This exception is used to detect
  resource leaks.

  @short Destruction exception.
  @ingroup exceptions
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Destruct : public Exception {
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
