/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_THREAD__THREAD_KEY_H
#define _DK_SDU_MIP__BASE_THREAD__THREAD_KEY_H

#include "base/Exception.h"
#include "base/ResourceException.h"
#include <pthread.h>

/**
  This class allows global and static variables to hold thread-specific data.
*/

template<class TYPE>
class ThreadKey {
private:

  /** Internal data. */
  pthread_key_t key;
public:

  /** The type of the ThreadKey clean-up routine. */
  typedef void (*CleanUp)(void*);

  /**
    Initializes the key object.

    @param cleanUp The clean-up routine to be called when the thread is exited. Default is NULL.
  */
  ThreadKey(CleanUp cleanUp = NULL) throw(ResourceException);

  /**
    Returns the key.
  */
  TYPE* getKey() const throw();

  /**
    Sets the key.

    @param value The desired value of the key for the executing thread.
  */
  void setKey(TYPE* value) throw(ResourceException);

  /**
    Destroys the key object.
  */
  ~ThreadKey() throw();
};

#endif
