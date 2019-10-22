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
#include <base/Exception.h>
#include <base/ResourceException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/*
  @short Implementation used by the ThreadKey class.
  @ingroup concurrency
  @see ThreadKey
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API ThreadKeyImpl : public Object {
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
    @version 1.0
  */
  class ThreadKeyException : public Exception {
  public:
    
    inline ThreadKeyException() noexcept
    {
    }
    
    inline ThreadKeyException(const char* message) noexcept
      : Exception(message)
    {
    }
    
    inline ThreadKeyException(Type type) noexcept
      : Exception(type)
    {
    }
    
    inline ThreadKeyException(const char* message, Type type) noexcept
      : Exception(message, type)
    {
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
  ~ThreadKeyImpl();
};



/**
  This class allows global and static variables to hold thread-specific data.

  @short Thread local variable
  @ingroup concurrency
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
