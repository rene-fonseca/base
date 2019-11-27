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

#include <base/Object.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Descriptor for the result of a remote invocation.
  
  @short Result of remote invocation.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API InvocationResult : public Object {
public:
  
  /** Invocation result flag. */
  enum Flags {
    /** Indicates that the invocation failed. */
    FAILED = 1,
    /** Specifies that an exception was raised at the server side. */
    EXCEPTION_RAISED = 2
  };
private:
  
  /** Flags. */
  unsigned int flags = 0;
  /** The cause of the invocation failure. */
  unsigned int cause = 0;
public:
  
  /**
    Initializes the invocation result.
    
    @param flags The flags.
    @param cause The cause.
  */
  inline InvocationResult(unsigned int _flags, unsigned int _cause) throw()
    : flags(_flags),
      cause(_cause)
  {
  }
  
  /**
    Initialization of object from other object.
  */
  inline InvocationResult(const InvocationResult& copy) throw()
    : flags(copy.flags),
      cause(copy.cause)
  {
  }
  
  /**
    Assignment of object to object.
  */
  inline InvocationResult& operator=(const InvocationResult& assign) throw()
  {
    flags = assign.flags;
    cause = assign.cause;
    return *this;
  }
  
  /**
    Returns the flags.
  */
  inline unsigned int getFlags() const throw()
  {
    return flags;
  }
  
  /**
    Returns the cause.
  */
  inline unsigned int getCause() const throw()
  {
    return cause;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
