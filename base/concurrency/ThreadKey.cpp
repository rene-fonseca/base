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

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <pthread.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

ThreadKeyImpl::ThreadKeyImpl() throw(ResourceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD key;
  if ((key = ::TlsAlloc()) == TLS_OUT_OF_INDEXES) {
    throw ResourceException(this);
  }
  this->key = Cast::container<Key>(key);
#else // unix
  if (sizeof(pthread_key_t) <= sizeof(Key)) {
    pthread_key_t* internalKey = Cast::pointer<pthread_key_t*>(&key);
    if (pthread_key_create(internalKey, 0)) {
      throw ResourceException(this);
    }
  } else {
    pthread_key_t* internalKey = new pthread_key_t[1];
    if (pthread_key_create(internalKey, 0)) {
      delete[] internalKey;
      throw ResourceException(this);
    }
    key.pointer = internalKey;
  }
#endif // flavor
}

void* ThreadKeyImpl::getKey() const throw(ThreadKeyException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  void* result = ::TlsGetValue(Cast::extract<DWORD>(key));
  if (!result && (::GetLastError() != NO_ERROR)) {
    throw ThreadKeyException(this);
  }
  return result;
#else
  const pthread_key_t* internalKey;
  if (sizeof(pthread_key_t) <= sizeof(Key)) {
    internalKey = Cast::pointer<const pthread_key_t*>(&key);
  } else {
    internalKey = Cast::pointer<const pthread_key_t*>(key.pointer);
  }
  return pthread_getspecific(*internalKey); // no errors are returned
#endif // flavor
}

void ThreadKeyImpl::setKey(void* value) throw(ThreadKeyException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!::TlsSetValue(Cast::extract<DWORD>(key), value)) {
    throw ThreadKeyException(this);
  }
#else
  pthread_key_t* internalKey;
  if (sizeof(pthread_key_t) <= sizeof(Key)) {
    internalKey = Cast::pointer<pthread_key_t*>(&key);
  } else {
    internalKey = Cast::pointer<pthread_key_t*>(key.pointer);
  }
  if (pthread_setspecific(*internalKey, value)) {
    throw ThreadKeyException(this);
  }
#endif // flavor
}

ThreadKeyImpl::~ThreadKeyImpl() throw(ThreadKeyException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!::TlsFree(Cast::extract<DWORD>(key))) {
    throw ThreadKeyException(this);
  }
#else
  if (sizeof(pthread_key_t) <= sizeof(Key)) {
    pthread_key_t* internalKey = Cast::pointer<pthread_key_t*>(&key);
    if (pthread_key_delete(*internalKey)) { // key should always be valid at this point
      throw ThreadKeyException(this);
    }
  } else {
    pthread_key_t* internalKey = Cast::pointer<pthread_key_t*>(key.pointer);
    if (pthread_key_delete(*internalKey)) { // key should always be valid at this point
      delete[] internalKey;
      throw ThreadKeyException(this);
    }
  }
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
