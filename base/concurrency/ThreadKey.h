/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__THREAD_KEY_H
#define _DK_SDU_MIP__BASE_THREAD__THREAD_KEY_H

#include <base/features.h>
#include <base/Object.h>
#include <base/Exception.h>
#include <base/ResourceException.h>

#if defined(__win32__)
  #include <windows.h>
#else
  #include <pthread.h>
#endif

/**
  Implementation used by the ThreadKey class.

  @see ThreadKey
  @author René Møller Fonseca
  @version 1.0
*/

class ThreadKeyImpl : public Object {
private:

#if defined(__win32__)
  /** Internal data. */
  DWORD key;
#else
  /** Internal data. */
  pthread_key_t key;
#endif
public:

  /** Exception thrown directly by the ThreadKey class. */
  class ThreadKeyException : public Exception {
  public:
    ThreadKeyException(const char* message) throw() : Exception(message) {}
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

  @author René Møller Fonseca
  @version 1.2
*/

template<class TYPE>
class ThreadKey : private ThreadKeyImpl {
public:

  /**
    Initializes the key object.
  */
  inline ThreadKey() throw(ResourceException) {}

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

#endif
