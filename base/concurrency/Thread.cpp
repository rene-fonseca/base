/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/concurrency/Thread.h>
#include <base/concurrency/MutualExclusion.h>

#if defined(__win32__)
  #include <stdlib.h>
#else
  #include <pthread.h>
  #include <signal.h>
  #include <time.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <stdlib.h>
#endif

// Pointer to thread object of executing thread
ThreadKey<Thread> executingThread;
// Pointer to thread local storage
ThreadKey<char> threadLocalStorage;
// The main thread object.
//Thread mainThread;

#if defined(__win32__)
DWORD WINAPI Thread::execute(Thread* thread) throw() {
#else
void* Thread::execute(Thread* thread) throw() {
#endif
  try {
    executingThread.setKey(thread);
    threadLocalStorage.setKey((char*)malloc(THREAD_LOCAL_STORAGE));
    thread->event.wait(); // wait until signaled
 
    try {
      thread->runnable->run();
      thread->termination = NORMAL;
      Thread* parent = thread->getParent();
      if (parent) {
        parent->onChildTermination(thread);
      }
    } catch(...) {
      thread->termination = EXCEPTION; // uncaugth exception
    }

    free(getLocalStorage());
  } catch(...) {
    thread->termination = INTERNAL; // hopefully we will never end up here
  }
  return 0;
}

void Thread::exit() throw() {
#if defined(__win32__)
  ExitThread(0); // will properly create resource leaks
#else
  pthread_exit(0); // will properly create resource leaks
#endif
}

Thread* Thread::getThread() throw() {
  return (Thread*)executingThread.getKey();
}

char* Thread::getLocalStorage() throw() {
  return (char*)threadLocalStorage.getKey();
}

void Thread::nanosleep(unsigned int nanoseconds) throw(OutOfDomain) {
  if (nanoseconds >= 1000000000) {
    throw OutOfDomain();
  }
#if defined(__win32__)
  Sleep(nanoseconds/1000);
#else
  struct timespec interval;
  interval.tv_sec = nanoseconds / 1000000000;
  interval.tv_nsec = nanoseconds % 1000000000;
  do {
    ::nanosleep(&interval, &interval);
  } while ((interval.tv_sec != 0) || (interval.tv_nsec != 0));
#endif
}

void Thread::microsleep(unsigned int microseconds) throw(OutOfDomain) {
  if (microseconds >= 1000000000) {
    throw OutOfDomain();
  }
#if defined(__win32__)
  Sleep(microseconds/1000);
#else
  struct timespec interval;
  interval.tv_sec = microseconds / 1000000;
  interval.tv_nsec = (microseconds % 1000000) * 1000;
  do {
    ::nanosleep(&interval, &interval);
  } while ((interval.tv_sec != 0) || (interval.tv_nsec != 0));
#endif
}

void Thread::millisleep(unsigned int milliseconds) throw(OutOfDomain) {
  if (milliseconds >= 1000000000) {
    throw OutOfDomain();
  }
#if defined(__win32__)
  Sleep(milliseconds);
#else
  struct timespec interval;
  interval.tv_sec = milliseconds / 1000;
  interval.tv_nsec = (milliseconds % 1000) * 1000000;
  do {
    ::nanosleep(&interval, &interval);
  } while ((interval.tv_sec != 0) || (interval.tv_nsec != 0));
#endif
}

void Thread::sleep(unsigned int seconds) throw(OutOfDomain) {
  if (seconds >= 1000000) {
    throw OutOfDomain();
  }
#if defined(__win32__)
  Sleep(seconds * 1000);
#else
  struct timespec interval;
  interval.tv_sec = seconds;
  interval.tv_nsec = 0;
  do {
    ::nanosleep(&interval, &interval);
  } while (interval.tv_sec != 0);
#endif
}

void Thread::yield() throw() {
#if defined(__win32__)
  Sleep(0);
#elif defined(HAVE_PTHREAD_YIELD)
  pthread_yield(); // ignore errors
#else
  sched_yield(); // ignore errors
#endif
}



void Thread::onChildTermination(Thread* thread) {
  if (runnable) {
    runnable->onChild(thread);
  }
}



Thread::Thread() throw() {
  this->runnable = NULL;
#if defined(__win32__)
  threadHandle = GetCurrentThread();
  threadID = GetCurrentThreadId();
#else
  threadID = pthread_self();
#endif
  terminated = false;
  termination = ALIVE;
  parent = NULL;
  executingThread.setKey(this);
  threadLocalStorage.setKey((char*)malloc(THREAD_LOCAL_STORAGE));
}

Thread::Thread(Runnable& runnable) throw(ResourceException) {
  this->runnable = &runnable;
  threadID = 0;
  terminated = false;
  termination = ALIVE;
  parent = getThread(); // must never be NULL

#if defined(__win32__)
  if (threadHandle = CreateThread(NULL, 0, execute, this, 0, &threadID)) {
    throw ResourceException(__func__);
  }
#else
  pthread_attr_t attributes;
  pthread_attr_init(&attributes);
  pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setinheritsched(&attributes, PTHREAD_INHERIT_SCHED);
  if (pthread_create(&threadID, &attributes, (void*(*)(void*))&execute, (void*) this)) {
    pthread_attr_destroy(&attributes);
    throw ResourceException();
  }
  pthread_attr_destroy(&attributes);
#endif
}

bool Thread::isAlive() const throw() {
  return termination == ALIVE;
}

bool Thread::isAncestor() const throw() {
  const Thread* executing = getThread();
  const Thread* current = this;
  if (current == executing) {
    return false;
  }
  while (!current) { // stop if main thread
    if (current == executing) { // could have checked thread IDs
      return true;
    }
    current = current->getParent();
  }
  return false;
}

bool Thread::isChild() const throw() {
  const Thread* current = getThread();
  if (current == this) {
    return false;
  }
  while (!current) { // stop if main thread
    if (current == this) { // could have checked thread IDs
      return true;
    }
    current = current->getParent();
  }
  return false;
}

bool Thread::isParent() const throw() {
  return getThread() == getParent();
}

bool Thread::isSelf() const throw() {
#if defined(__win32__)
  return GetCurrentThreadId() == threadID;
#else
  return pthread_self() == threadID;
#endif
}

void Thread::join() const throw(ThreadException) {
  if (isSelf()) { // is thread trying to wait for itself to exit
    throw Self();
  }
#if defined(__win32__)
  WaitForSingleObject(threadHandle, INFINITE);
#else
  if (pthread_join(threadID, NULL)) {
    throw ThreadException(__func__);
  }
#endif
}

void Thread::start() throw() {
  event.signal();
}

void Thread::terminate() throw() {
  if (!terminated) {
    terminated = true;
    if (runnable) {
      runnable->onTermination();
    }
  }
}

Thread::~Thread() throw(ThreadException) {
  if (isAlive()) {
    throw ThreadException(__func__);
  }

#if defined(__win32__)
  if (!CloseHandle(threadHandle)) {
    ThreadException(__func__);
  }
#endif
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Thread& value) {
  stream << "Thread{"
         << "alive=" << value.isAlive()
         << ",terminated=" << value.isTerminated()
         << "}";
  return stream;
}
