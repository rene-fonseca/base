/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "ThreadKey.h"

class Thread;
template ThreadKey<Thread>;

template<class TYPE>
ThreadKey<TYPE>::ThreadKey() throw(ResourceException) {
#ifdef __win32__
  if ((key = TlsAlloc()) == TLS_OUT_OF_INDEXES) {
    throw ResourceException();
  }
#else
  if (pthread_key_create(&key, NULL)) {
    throw ResourceException();
  }
#endif
}

template<class TYPE>
TYPE* ThreadKey<TYPE>::getKey() const throw(ThreadKeyException) {
#ifdef __win32__
  TYPE* result = (TYPE*)TlsGetValue(key);
  if (!result && (GetLastError() != NO_ERROR)) {
    throw ThreadKeyException(__func__);
  }
  return result;
#else
  return (TYPE*)pthread_getspecific(key); // no errors are returned
#endif
}

template<class TYPE>
void ThreadKey<TYPE>::setKey(TYPE* value) throw(ThreadKeyException) {
#ifdef __win32__
  if (!TlsSetValue(key, (void*)value)) {
    throw ThreadKeyException(__func__);
  }
#else
  if (pthread_setspecific(key, (void*)value)) {
    throw ThreadKeyException(__func__);
  }
#endif
}

template<class TYPE>
ThreadKey<TYPE>::~ThreadKey() throw(ThreadKeyException) {
#ifdef __win32__
  if (!TlsFree(key)) {
    throw ThreadKeyException(__func__);
  }
#else
  if (pthread_key_delete(key)) { // key should always be valid at this point
    throw ThreadKeyException(__func__);
  }
#endif
}
