/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/concurrency/ThreadKey.h>
#include <base/concurrency/Thread.h>
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  define __thread // TAG: temp. fix for s390-ibm-linux-gnu
#  include <pthread.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
namespace {

  void* global = nullptr;
}
#endif

ThreadKeyImpl::ThreadKeyImpl()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD key = 0;
  if ((key = ::TlsAlloc()) == TLS_OUT_OF_INDEXES) {
    throw ResourceException(this);
  }
  this->key.pointer = reinterpret_cast<void*>(static_cast<MemorySize>(key));
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
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

void* ThreadKeyImpl::getKey() const
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  void* result = ::TlsGetValue(Cast::extract<DWORD>(key));
  if (!result && (::GetLastError() != NO_ERROR)) {
    throw ThreadKeyException(this);
  }
  return result;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return global;
#else
  const pthread_key_t* internalKey = nullptr;
  if (sizeof(pthread_key_t) <= sizeof(Key)) {
    internalKey = Cast::pointer<const pthread_key_t*>(&key);
  } else {
    internalKey = Cast::pointer<const pthread_key_t*>(key.pointer);
  }
  return pthread_getspecific(*internalKey); // no errors are returned
#endif // flavor
}

void ThreadKeyImpl::setKey(void* value)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::TlsSetValue(Cast::extract<DWORD>(key), value)) {
    throw ThreadKeyException(this);
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  global = value;
#else
  pthread_key_t* internalKey = nullptr;
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

ThreadKeyImpl::~ThreadKeyImpl()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::TlsFree(Cast::extract<DWORD>(key))) {
    throw ThreadKeyException(this);
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
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


ThreadKeyImpl::ThreadKeyException::ThreadKeyException() noexcept
{
}

ThreadKeyImpl::ThreadKeyException::ThreadKeyException(const char* message) noexcept
  : Exception(message)
{
}

ThreadKeyImpl::ThreadKeyException::ThreadKeyException(const Type& type) noexcept
  : Exception(type)
{
}

ThreadKeyImpl::ThreadKeyException::ThreadKeyException(const char* message, const Type& type) noexcept
  : Exception(message, type)
{
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(ThreadKey);

class MyThreadKey : public Thread {
private:

  TEST_CLASS(ThreadKey)* parent = nullptr;
public:

  MyThreadKey(TEST_CLASS(ThreadKey)* _parent) : parent(_parent)
  {
  }

  void run() override;
};

class TEST_CLASS(ThreadKey) : public UnitTest {
public:

  TEST_PRIORITY(10);
  // test devel filter with this TEST_IN_DEVELOPMENT();
  TEST_PROJECT("base/concurrency");

  ThreadKey<String> key;

  void runFromThread()
  {
    String local;
    TEST_ASSERT(!key.getKey());
    key.setKey(&local);
    TEST_ASSERT(key.getKey() == &local);
    // keep bad pointer
  }

  void run() override
  {
    String local;
    TEST_ASSERT(!key.getKey());
    key.setKey(&local);
    TEST_ASSERT(key.getKey() == &local);

    MyThreadKey thread1(this);
    thread1.start();
    Thread::nanosleep(1000 * 1000);
    thread1.join();

    TEST_ASSERT(key.getKey() == &local);
    key.setKey(nullptr);
    TEST_ASSERT(!key.getKey());
  }
};

void MyThreadKey::run()
{
  setThreadName("ThreadKey / MyThread");
  parent->runFromThread();
}

TEST_REGISTER(ThreadKey);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
