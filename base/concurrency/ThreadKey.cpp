/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/concurrency/ThreadKey.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <pthread.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

ThreadKeyImpl::ThreadKeyImpl() throw(ResourceException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if ((key = ::TlsAlloc()) == TLS_OUT_OF_INDEXES) {
    throw ResourceException(this);
  }
#else // unix
  if (pthread_key_create(&key, 0)) {
    throw ResourceException(this);
  }
#endif // flavor
}

void* ThreadKeyImpl::getKey() const throw(ThreadKeyException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  void* result = ::TlsGetValue(key);
  if (!result && (::GetLastError() != NO_ERROR)) {
    throw ThreadKeyException(this);
  }
  return result;
#else
  return pthread_getspecific(key); // no errors are returned
#endif // flavor
}

void ThreadKeyImpl::setKey(void* value) throw(ThreadKeyException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::TlsSetValue(key, value)) {
    throw ThreadKeyException(this);
  }
#else
  if (pthread_setspecific(key, value)) {
    throw ThreadKeyException(this);
  }
#endif // flavor
}

ThreadKeyImpl::~ThreadKeyImpl() throw(ThreadKeyException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::TlsFree(key)) {
    throw ThreadKeyException(this);
  }
#else
  if (pthread_key_delete(key)) { // key should always be valid at this point
    throw ThreadKeyException(this);
  }
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
