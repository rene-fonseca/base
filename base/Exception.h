/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__EXCEPTION_H
#define _DK_SDU_MIP__BASE__EXCEPTION_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The base class of all exceptions.

  @author René Møller Fonseca
  @version 1.01
*/

class Exception {
private:

  /**
    Message associated with the exception.
  */
  const char* message;
public:

  /**
    Initializes the exception object with no message.
  */
  Exception() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Exception(const char* message) throw();

  /**
    Copy constructor.

    @param exception The exception object to be copied.
  */
  Exception(const Exception& copy) throw();

  /**
    Returns the message associated with the exception.

    @return The message associated with the exception.
  */
  const char* getMessage() const throw();

  /**
    Destroys exception object.
  */
  virtual ~Exception() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
