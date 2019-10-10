/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Type.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API Exception {
private:

  /** The message associated with the exception (ASCII format). This may not be available. */
  const char* message = nullptr;
  /** The identity of the type which raised the exception (may not be available). */
  Type type;
  /** The associated cause (0 by default). */
  unsigned int cause = 0;
  /** The associated system error code. */
  unsigned int error = 0;
public:
  
  /**
    Returns true if the stack is currently being unwinded due to a raised exception.
    
    @see getExceptionType
    @return False if not supported.
  */
  static bool isUnwinding() throw();
  
  /**
    Returns the type of the caught exception.
    
    @return An uninitialized type is returned if no exception has been caught or this method isn't supported.
  */
  static Type getExceptionType() throw();
  
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

    @code
    void myMethod() throw(Exception) {
      if (!condition) {
        throw Exception("my short message", this);
      }
      ...
    }
    @endcode
    
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
    Returns the associated cause. 0 indicates an unspecified cause.
  */
  inline unsigned int getCause() const throw() {
    return cause;
  }

  /**
    Sets the cause. 0 indicates an unspecified cause.
  */
  inline void setCause(unsigned int cause) throw() {
    this->cause = cause;
  }
  
  /**
    Returns the associated native error code. 0 if no error.
  */
  inline unsigned int getError() const throw() {
    return error;
  }
  
  /**
    Sets the native error code. 0 indicates no error.
  */
  inline void setError(unsigned int error) throw() {
    this->error = error;
  }
  
  /**
    Returns the message associated with the exception.

    @return The message associated with the exception.
  */
  inline const char* getMessage() const throw() {
    return message;
  }
  
  /**
    Associates the exception with the specified message.
  */
  inline void setMessage(const char* message) throw() {
    this->message = message;
  }
  
  /**
    Returns the identity of the type which raised the exception.
  */
  inline Type getType() const throw() {
    return type;
  }
  
  /**
    Sets the identity of the type which raised the exception.
  */
  inline void setType(const Type& type) throw() {
    this->type = type;
  }
  
  /**
    Destroys exception object.
  */
  virtual ~Exception() throw();
};

/**
  Associates the exception with the given cause.
*/
template<class EXCEPTION>
inline EXCEPTION bindCause(EXCEPTION e, unsigned int cause) throw() {
  e.setCause(cause);
  return e;
}

/**
  Associates the exception with the given native error code.
*/
template<class EXCEPTION>
inline EXCEPTION bindError(EXCEPTION e, unsigned int error) throw() {
  e.setError(error);
  return e;
}

/**
  Associates the exception with the given message.
*/
template<class EXCEPTION>
inline EXCEPTION bindMessage(EXCEPTION e, const char* message) throw() {
  e.setMessage(message);
  return e;
}

/**
  Associates the exception with the given type.
*/
template<class EXCEPTION>
inline EXCEPTION bindType(EXCEPTION e, const Type& type) throw() {
  e.setType(type);
  return e;
}

template<class EXCEPTION>
inline void raise(EXCEPTION e) throw(EXCEPTION) {
  throw e;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
