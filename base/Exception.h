/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__EXCEPTION_H
#define _DK_SDU_MIP__BASE__EXCEPTION_H

#include <base/Type.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @defgroup exceptions Exceptions
*/

/**
  This is the base class of all exceptions supported by the framework. Normally
  exceptions do not raise exceptions themselves. It is highly advised against
  raising objects which are not specializations of Exception. Exceptions allow
  the developer to handle 'rare' conditions at runtime in a safe manner. An
  exception doesn't have to be indicating an error but normally does. Uncaught
  exceptions will terminate the executing context with an error (in the case of
  the main context the entire application is terminated).

  @short The general exception class
  @ingroup exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.02
*/

class Exception {
private:

  /**
    The message associated with the exception (ASCII format). This may not be available.
  */
  const char* message;
  
  /**
    The identity of the type which raised the exception (may not be available).
  */
  const Type type;
public:

  /**
    Initializes the exception object without an associated message and type identity.
  */
  Exception() throw();

  /**
    Initializes the exception object.

    @param message An NULL-terminated string (ASCII).
  */
  Exception(const char* message) throw();

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  Exception(Type type) throw();
  
  /**
    Initializes the exception object.

    <pre>
    void myMethod() throw(Exception) {
      if (!condition) {
        throw Exception("my short message", this);
      }
      ...
    }
    </pre>
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  Exception(const char* message, Type type) throw();

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
    Returns the identity of the type which raised the exception.
  */
  Type getType() const throw();
  
  /**
    Destroys exception object.
  */
  virtual ~Exception() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
