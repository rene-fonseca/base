/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/concurrency/ThreadKey.h>

ThreadKeyImpl::ThreadKeyImpl() throw(ResourceException) {
#if defined(__win32__)
  if ((key = TlsAlloc()) == TLS_OUT_OF_INDEXES) {
    throw ResourceException();
  }
#else
  if (pthread_key_create(&key, NULL)) {
    throw ResourceException();
  }
#endif
}

void* ThreadKeyImpl::getKey() const throw(ThreadKeyException) {
#if defined(__win32__)
  void* result = TlsGetValue(key);
  if (!result && (GetLastError() != NO_ERROR)) {
    throw ThreadKeyException(__func__);
  }
  return result;
#else
  return pthread_getspecific(key); // no errors are returned
#endif
}

void ThreadKeyImpl::setKey(void* value) throw(ThreadKeyException) {
#if defined(__win32__)
  if (!TlsSetValue(key, value)) {
    throw ThreadKeyException(__func__);
  }
#else
  if (pthread_setspecific(key, value)) {
    throw ThreadKeyException(__func__);
  }
#endif
}

ThreadKeyImpl::~ThreadKeyImpl() throw(ThreadKeyException) {
#if defined(__win32__)
  if (!TlsFree(key)) {
    throw ThreadKeyException(__func__);
  }
#else
  if (pthread_key_delete(key)) { // key should always be valid at this point
    throw ThreadKeyException(__func__);
  }
#endif
}
