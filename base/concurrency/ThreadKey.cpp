/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/ThreadKey.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  ASSERTION(sizeof(unsigned long) == sizeof(DWORD));
#else // unix
  #include <pthread.h>
#endif // flavour

ThreadKeyImpl::ThreadKeyImpl() throw(ResourceException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if ((key = TlsAlloc()) == TLS_OUT_OF_INDEXES) {
    throw ResourceException();
  }
#else // unix
  if (pthread_key_create(&key, NULL)) {
    throw ResourceException();
  }
#endif // flavour
}

void* ThreadKeyImpl::getKey() const throw(ThreadKeyException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  void* result = TlsGetValue(key);
  if (!result && (GetLastError() != NO_ERROR)) {
    throw ThreadKeyException();
  }
  return result;
#else
  return pthread_getspecific(key); // no errors are returned
#endif // flavour
}

void ThreadKeyImpl::setKey(void* value) throw(ThreadKeyException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!TlsSetValue(key, value)) {
    throw ThreadKeyException();
  }
#else
  if (pthread_setspecific(key, value)) {
    throw ThreadKeyException();
  }
#endif // flavour
}

ThreadKeyImpl::~ThreadKeyImpl() throw(ThreadKeyException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!TlsFree(key)) {
    throw ThreadKeyException();
  }
#else
  if (pthread_key_delete(key)) { // key should always be valid at this point
    throw ThreadKeyException();
  }
#endif // flavour
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
