/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__THREAD_KEY_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__THREAD_KEY_H

#include <base/Object.h>
#include <base/Exception.h>
#include <base/ResourceException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/*
  @short Implementation used by the ThreadKey class.
  @ingroup concurrency
  @see ThreadKey
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class ThreadKeyImpl : public Object {
private:

  union Key {
    uint64 integer;
    void* pointer;
  };
  
  /** Internal data. */
  Key key;
public:

  /**
    Exception raised directly by the ThreadKey class.
    
    @short Thread key exception.
    @ingroup concurrency exceptions
    @author Rene Moeller Fonseca
    @version 1.0
  */
  class ThreadKeyException : public Exception {
  public:
    
    inline ThreadKeyException() throw() {
    }
    
    inline ThreadKeyException(const char* message) throw() : Exception(message) {
    }
    
    inline ThreadKeyException(Type type) throw() : Exception(type) {
    }
    
    inline ThreadKeyException(const char* message, Type type) throw()
      : Exception(message, type) {
    }
  };

  /**
    Initializes the key object.
  */
  ThreadKeyImpl() throw(ResourceException);

  /**
    Returns the key.
  */
  void* getKey() const throw(ThreadKeyException);

  /**
    Sets the key.

    @param value The desired value of the key for the executing thread.
  */
  void setKey(void* value) throw(ThreadKeyException);

  /**
    Destroys the key object.
  */
  ~ThreadKeyImpl() throw(ThreadKeyException);
};



/**
  This class allows global and static variables to hold thread-specific data.

  @short Thread local variable
  @ingroup concurrency
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

template<class TYPE>
class ThreadKey : private ThreadKeyImpl {
public:

  /**
    Initializes the key object.
  */
  inline ThreadKey() throw(ResourceException) {
  }

  /**
    Returns the key.
  */
  inline TYPE* getKey() const throw(ThreadKeyException) {
    return static_cast<TYPE*>(ThreadKeyImpl::getKey());
  }

  /**
    Sets the key.

    @param value The desired value of the key for the executing thread.
  */
  inline void setKey(TYPE* value) throw(ThreadKeyException) {
    ThreadKeyImpl::setKey(value);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
