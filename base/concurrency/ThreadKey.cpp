/***************************************************************************
    begin       : Fri May 12 2000
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "ThreadKey.h"

template<class TYPE>
ThreadKey<TYPE>::ThreadKey(CleanUp cleanUp = NULL) throw(ResourceException) {
  if (pthread_key_create(&key, cleanUp)) {
    throw ResourceException();
  }
}

template<class TYPE>
TYPE* ThreadKey<TYPE>::getKey() const throw() {
  return (TYPE*)pthread_getspecific(key);
}

template<class TYPE>
void ThreadKey<TYPE>::setKey(TYPE* value) throw(ResourceException) {
  if (pthread_setspecific(key, (void*)value)) {
    throw ResourceException();
  }
}

template<class TYPE>
ThreadKey<TYPE>::~ThreadKey() throw() {
  pthread_key_delete(key); // key should always be valid at this point
}
