/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__INVOCATION_RESULT_H
#define _DK_SDU_MIP__BASE_RMI__INVOCATION_RESULT_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Descriptor for the result of a remote invocation.
  
  @short Result of remote invocation.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class InvocationResult : public Object {
public:
    
  /** Invocation result flag. */
  enum Flags {
    /** Specifies that an exception was raised at the server side. */
    EXCEPTION_RAISED = 1
  };
private:
    
  /** Flags. */
  unsigned int flags;
  /** The error code of the invocation. */
  unsigned int error;
public:

  /**
    Initializes object.
    
    @param flags The flags.
    @param error The error code.
  */
  inline InvocationResult(unsigned int _flags, unsigned int _error) throw()
    : flags(_flags),
      error(_error) {
  }
    
  /**
    Initialization of object from other object.
  */
  inline InvocationResult(const InvocationResult& copy) throw()
    : flags(copy.flags),
      error(copy.error) {
  }
    
  /**
    Assignment of object to object.
  */
  inline InvocationResult& operator=(const InvocationResult& eq) throw() {
    flags = eq.flags;
    error = eq.error;
    return *this;
  }
    
  /**
    Returns the flags.
  */
  inline unsigned int getFlags() const throw() {
    return flags;
  }
    
  /**
    Returns the error code.
  */
  inline unsigned int getError() const throw() {
    return error;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
