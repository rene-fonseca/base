/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__THREAD_KEY_H
#define _DK_SDU_MIP__BASE_THREAD__THREAD_KEY_H

#include <config.h>
#include "base/Object.h"
#include "base/Exception.h"
#include "base/ResourceException.h"

#ifdef __win32__
  #include <windows.h>
#else
  #include <pthread.h>
#endif

/**
  This class allows global and static variables to hold thread-specific data.

  @author René Møller Fonseca
  @version 1.1
*/

template<class TYPE>
class ThreadKey : public Object {
private:

#ifdef __win32__
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
  ThreadKey() throw(ResourceException);

  /**
    Returns the key.
  */
  TYPE* getKey() const throw(ThreadKeyException);

  /**
    Sets the key.

    @param value The desired value of the key for the executing thread.
  */
  void setKey(TYPE* value) throw(ThreadKeyException);

  /**
    Destroys the key object.
  */
  ~ThreadKey() throw(ThreadKeyException);
};

#endif
