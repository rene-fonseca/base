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

#include <base/Base.h>
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
public:

  /** Callback for exception. */
  typedef void (*ExceptionHandler)(Exception* exception);
private:

  /** When enabled, all exceptions are printed to stderr. */
  static bool dumpExceptions;
  static ExceptionHandler exceptionHandler;

  /** The message associated with the exception (ASCII format). This may not be available. */
  const char* message = nullptr;
  /** The identity of the type which raised the exception (may not be available). */
  Type type;
  /** The associated cause (0 by default). */
  unsigned int cause = 0;
  /** The associated system error code. */
  unsigned int error = 0;
#if defined(_COM_AZURE_DEV__BASE__DEBUG)
  /**
    The number of times the exception got copy constructed. This can indicate catch-statement without reference.
    Try to keep to 0.
  */
  unsigned int copies = 0;
#endif
public:

  class Hook {};

  /** Called on throw. */
  static void onThrow(Exception& exception) noexcept;

  /** Rethrows exception. Throws Exception is no current exception. */
  static void rethrow();

  /** Returns true if stack traces for new exceptions are printed. */
  static inline bool getDumpExceptions() noexcept
  {
    return dumpExceptions;
  }

  /** Enables printing of stack traces when exceptions are constructed. */
  static inline void setDumpExceptions(bool _dumpExceptions) noexcept
  {
    dumpExceptions = _dumpExceptions;
  }

  /** Sets exception handler. */
  static inline ExceptionHandler setExceptionHandler(ExceptionHandler _exceptionHandler) noexcept
  {
    auto result = exceptionHandler;
    exceptionHandler = _exceptionHandler;
    return result;
  }

  /**
    Returns true if an exception is pending.
  */
  static bool hasPendingException() noexcept;

  /**
    Returns true if the stack is currently being unwinded due to a raised exception.
    
    @see getExceptionType
    @return False if not supported.
  */
  static bool isUnwinding() noexcept;

  /**
    Returns the number of currently pending/unhandled exceptions.
  */
  static unsigned int getPendingExceptions() noexcept;

  /**
    Returns description for well-known Std C++ exceptions.
  */
  static const char* getStdExceptionName(const std::exception& e) noexcept;

  /**
    Returns the type of the caught exception.
    
    @return An uninitialized type is returned if no exception has been caught or this method isn't supported.
  */
  static Type getExceptionType() noexcept;
  
  /**
    Initializes the exception object without an associated message and type identity.
  */
  Exception() noexcept;
  
  /**
    Initializes the exception object.

    @param message An NULL-terminated string (ASCII).
  */
  Exception(const char* message) noexcept;
  
  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  Exception(const Type& type) noexcept;
  
  /**
    Initializes the exception object.

    @code
    void myMethod()
    {
      if (!condition) {
        throw Exception("My short message.", this);
      }
      ...
    }
    @endcode
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  Exception(const char* message, const Type& type) noexcept;
  
  /**
    Copy constructor.

    @param exception The exception object to be copied.
  */
  Exception(const Exception& copy) noexcept;

  /**
    Move constructor.

    @param exception The exception object to be moved.
  */
  Exception(Exception&& move) noexcept;

  /**
    Returns the associated cause. 0 indicates an unspecified cause.
  */
  inline unsigned int getCause() const noexcept
  {
    return cause;
  }

  /**
    Sets the cause. 0 indicates an unspecified cause.
  */
  inline void setCause(unsigned int cause) noexcept
  {
    this->cause = cause;
  }
  
  /**
    Returns the associated native error code. 0 if no error.
  */
  inline unsigned int getError() const noexcept
  {
    return error;
  }
  
  /**
    Sets the native error code. 0 indicates no error.
  */
  inline void setError(unsigned int error) noexcept
  {
    this->error = error;
  }
  
  /**
    Returns the message associated with the exception.

    @return The message associated with the exception.
  */
  inline const char* getMessage() const noexcept
  {
    return message;
  }
  
  /**
    Associates the exception with the specified message.
  */
  inline void setMessage(const char* message) noexcept
  {
    this->message = message;
  }

  /** Returns the type of the exception. */
  virtual Type getThisType() const noexcept;
  
  /**
    Returns the identity of the type which raised the exception.
  */
  inline const Type& getType() const noexcept
  {
    return type;
  }
  
  /**
    Sets the identity of the type which raised the exception.
  */
  inline void setType(const Type& type) noexcept
  {
    this->type = type;
  }
  
  /**
    Returns true if exception is generally an externally triggered exception that can be suppressed and handled
    directly. Internal exceptions like NullPointer should not be handled which is the default behavior. E.g. out of
    resource, EOF, parsing issues, and similar.
  */
  virtual bool isCommonException() const noexcept;

  class StackTrace {
  public:

    const void* const* begin = nullptr; // beginning of trace
    const void* const* end = nullptr; // end of trace
  };

  /**
    Returns the stack trace for the last constructed exception. Avoid constructing exception that are not thrown as
    these will hide the true exception.

    @return Can return nullptr if exception is unknown. Do NOT keep the pointers outside scope.
  */
  static StackTrace getStackTrace();

  /**
    Destroys exception object.
  */
  virtual ~Exception() noexcept;
};

/**
  Associates the exception with the given cause.
*/
template<class EXCEPTION>
inline EXCEPTION&& bindCause(EXCEPTION&& e, unsigned int cause) noexcept
{
  e.setCause(cause);
  return moveObject(e);
}

/**
  Associates the exception with the given native error code.
*/
template<class EXCEPTION>
inline EXCEPTION&& bindError(EXCEPTION&& e, unsigned int error) noexcept
{
  e.setError(error);
  return moveObject(e);
}

/**
  Associates the exception with the given message.
*/
template<class EXCEPTION>
inline EXCEPTION&& bindMessage(EXCEPTION&& e, const char* message) noexcept
{
  e.setMessage(message);
  return moveObject(e);
}

/**
  Associates the exception with the given type.
*/
template<class EXCEPTION>
inline EXCEPTION&& bindType(EXCEPTION&& e, const Type& type) noexcept
{
  e.setType(type);
  return moveObject(e);
}

template<class EXCEPTION>
inline EXCEPTION&& bindException(EXCEPTION&& e, const char* message, const Type& type) noexcept
{
  e.setMessage(message);
  e.setType(type);
  return moveObject(e);
}

template<class EXCEPTION>
inline EXCEPTION&& bindException(EXCEPTION&& e, const char* message, unsigned int cause) noexcept
{
  e.setMessage(message);
  e.setCause(cause);
  return moveObject(e);
}

/** Helper to hook throw. */
template<class TYPE>
inline TYPE&& operator*(const Exception::Hook&& hook, TYPE&& exception)
{
  static_assert(std::is_base_of<Exception, TYPE>(), "Only Exception derived types may be thrown.");
  Exception::onThrow(exception);
  return moveObject(exception);
}

/** Throws the given exception but requires function call style. */
template<class EXCEPTION>
inline void throwthis(EXCEPTION&& e)
{
  static_assert(std::is_base_of<Exception, EXCEPTION>(), "Only Exception derived types may be thrown.");
  Exception::onThrow(e); // does not throw
  throw moveObject(e); // throw std::forward(e);
}

/** Throws exception. throwit MyException(). */
#define throwit Exception::Hook() * // fake keyword

/** Rethrows current exception if any. */
inline void rethrow()
{
  Exception::rethrow(); // does throw
  // throw; // throwing in handler
}

#define rethrowit Exception::rethrow() // fake keyword

#define _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE() \
  Type getThisType() const noexcept override \
  { \
    return Type::getType(*this); \
  }

/**
  Helper class for detecting unwinding during destruction.
  See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4152.pdf.
*/
class NewException {
private:

  const unsigned int pendingExceptions = Exception::getPendingExceptions();
public:

  /** Returns true if new exception have been . */
  inline bool isNewException() const noexcept
  {
    return pendingExceptions != Exception::getPendingExceptions();
  }
};

/** Causes stack dump on exception during destruction. */
class DumpStackOnException {
public:

  ~DumpStackOnException() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
