/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#define D_POSIX_PTHREAD_SEMANTICS

/*
  TODO

  1. Parent thread must wait for children threads?
  2. Which signals to block?
  3. Which signals to handle? SIGINT?
*/

#include "Thread.h"
#include "MutualExclusion.h"
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

using namespace ::std;
/*
#ifndef HAVE_PTHREAD_DELAY

struct timespec* getAbsoluteTime(struct timespec* absoluteTime, const struct timespec* interval) {
  if (!absoluteTime) {
    return NULL;
  }
  struct timeval now; // only microsecond resolution :-(
  gettimeofday(&now, NULL);
  long long nanoseconds = now.tv_usec * 1000 + interval->tv_nsec;
  absoluteTime->tv_sec = now.tv_sec + interval->tv_sec + nanoseconds/1000000000;
  absoluteTime->tv_nsec = nanoseconds % 1000000000;
  return absoluteTime;
}

#endif
*/


void* Thread::execute(Thread* thread) throw() {
  try {
    executingThread.setKey(thread);

    thread->mutex->lock(); // wait for start
    thread->mutex->unlock();

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); // should not fail
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); // should not fail

    try {
      thread->runnable->run();
      thread->termination = NORMAL;
    } catch(Thread::Exit) {
      thread->termination = EXIT; // valid exit - but may have created resource leaks on the heap
    } catch(...) {
      thread->termination = EXCEPTION; // uncaugth exception
    }

  } catch(...) {
    thread->termination = INTERNAL; // hopefully we will never end up here - critical error
  }

  return NULL;
}

void Thread::exited(Thread* thread) throw() {
// which thread runs this code?
  try {
    thread->mutex->lock(); // expected NOT to throw MutualExclusion::Lock here
    thread->threadID = 0;
    Thread* parent = thread->getParent();
    if (parent) {
      parent->onChildExit(thread); // signal cancellation to parent - warning - any exception could be thrown
    }
  } catch(...) {
  }
  thread->mutex->unlock(); // this should not throw MutualExclusion::Unlock unless Thread has been implemented with errors
}

/*
void Thread::signalHandler(int signal) throw() {
}

void Thread::cancelHandler(int signal) throw() {
  pthread_exit((void*)0);
}

void Thread::suspendHandler(int signal) throw() {
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGNAL_RESUME);
  sigwait(&set, NULL);
}
*/

void Thread::installSignalHandler(int signal, SignalHandler handler, int flags) throw(Construct) {
  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = flags;
  if (sigaction(signal, &action, NULL)) {
    throw Construct();
  }
}



void Thread::exit() throw(Thread::Exit) {
  throw Thread::Exit(); // make sure local objects are destructed
//  pthread_exit(0); // will properly create resource leaks
}

void Thread::nanosleep(unsigned int nanoseconds) throw() {
  struct timespec interval;
  interval.tv_sec = nanoseconds / 1000000000;
  interval.tv_nsec = nanoseconds % 1000000000;
  do {
    ::nanosleep(&interval, &interval);
  } while ((interval.tv_sec != 0) || (interval.tv_nsec != 0));
}

void Thread::microsleep(unsigned int microseconds) throw() {
  struct timespec interval;
  interval.tv_sec = microseconds / 1000000;
  interval.tv_nsec = (microseconds % 1000000) * 1000;
  do {
    ::nanosleep(&interval, &interval);
  } while ((interval.tv_sec != 0) || (interval.tv_nsec != 0));
}

void Thread::millisleep(unsigned int milliseconds) throw() {
  struct timespec interval;
  interval.tv_sec = milliseconds / 1000;
  interval.tv_nsec = (milliseconds % 1000) * 1000000;
  do {
    ::nanosleep(&interval, &interval);
  } while ((interval.tv_sec != 0) || (interval.tv_nsec != 0));
}

void Thread::sleep(unsigned int seconds) throw() {
  struct timespec interval;
  interval.tv_sec = seconds;
  interval.tv_nsec = 0;
  do {
    ::nanosleep(&interval, &interval);
  } while (interval.tv_sec != 0);
}

void Thread::yield() throw() {
#ifdef HAVE_PTHREAD_YIELD
  pthread_yield();
#else
  #ifdef HAVE_SCHED_YIELD
  sched_yield();
  #else
  sleep(0);
  #endif
#endif
}



void Thread::onChildExit(Thread* thread) {
  if (runnable) {
    runnable->onChild(thread);
  }
}



Thread::Thread() {
  runnable = NULL;
  stackSize = 0;
  threadID = pthread_self();
  terminated = false;
  parent = NULL;
  executingThread.setKey(this);
// not implemented
}

Thread::Thread(Runnable& runnable, SchedulingPolicy schedulingPolicy = INHERITED, size_t stackSize = DEFAULT_STACK_SIZE) throw(Construct, ResourceException, MutualExclusion::MutualExclusionException) {
  this->runnable = &runnable;
  this->stackSize = stackSize;
  threadID = 0;
  terminated = false;
  termination = ALIVE;
  parent = getThread(); // must never be NULL

  mutex->lock(); // make sure the new thread can't run - exception???

  pthread_attr_t attributes;
  pthread_attr_init(&attributes);
  pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setinheritsched(&attributes, PTHREAD_INHERIT_SCHED);
#ifdef PTHREAD_STACK_MIN
  if (pthread_attr_setstacksize(&attributes, stackSize <= PTHREAD_STACK_MIN ? PTHREAD_STACK_MIN : stackSize)) {
    pthread_attr_destroy(&attributes);
    throw Construct();
  }
#else
  if (pthread_attr_setstacksize(&attributes, stackSize)) {
    pthread_attr_destroy(&attributes);
    throw Construct();
  }
#endif
  if (pthread_create(&threadID, &attributes, (void*(*)(void*))&execute, (void*) this)) {
    pthread_attr_destroy(&attributes);
    throw Construct();
  }
  pthread_attr_destroy(&attributes);
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

void Thread::cancel() throw(Thread::Self, Thread::Child, Thread::ThreadException) {
  if (isSelf()) {
    throw Self();
  }
  if (isChild()) { // is executing thread child of this thread
    throw Child();
  }
  mutex->lock();
    if (isAlive()) {
      pthread_cancel(threadID); // ask thread to cancel - should only fail if threadID is not valid
      if (pthread_join(threadID, NULL)) { // wait for thread to cancel
        mutex->unlock();
        throw ThreadException();
      }
    }
  mutex->unlock();
}

void Thread::join() throw(Thread::Self, Thread::Child, Thread::ThreadException) {
  if (isSelf()) {
    throw Self();
  }
  if (isChild()) { // is executing thread child of this thread
    throw Child();
  }
  if (pthread_join(threadID, NULL)) {
    throw ThreadException();
  }
}

void Thread::start() throw(Thread::Self, Thread::Child, MutualExclusion::Unlock) {
  if (isSelf()) {
    throw Self();
  }
  if (isChild()) { // is executing thread child of this thread
    throw Child();
  }
  mutex->unlock();
}

void Thread::terminate() {
  if (!terminated) {
    terminated = true;
    runnable->onTermination(); // signal the runnable object that it should terminate as soon as possible
  }
}

void Thread::debug() const {
  cout << "CLASS/Thread\n";
  cout << "  this=" << this << "\n";
  cout << "  minimumStackSize=" << getMinimumStackSize() << "\n";
  cout << "  id=" << threadID << "\n";
  cout << "  alive=" << isAlive() << "\n";
  cout << "  terminated=" << isTerminated() << "\n";
  cout << "  termination=" << getTerminationState() << "\n";
//  cout << "  children=" << children << "\n";
}

Thread::~Thread() throw() {

/*  semaphore -= children;
  while (children()) { // have all the children been destroyed
    childEvent->wait(); // wait for signal from child
  }*/

  try {
    if (isAlive()) {
      cancel(); // also joins
    }
  } catch(...) {
    // now what?
    cout << "Exception in Thread::~Thread()\n";
  }

/*  if (parent) { // if thread has parent
    parent->children--;
  }*/
}


/*
MainThread::MainThread() : Thread(NULL, other) {
  runnable = NULL;
  suspended = false;
  terminated = false;
  parent = NULL;
  _executingThread.setKey(this);

  installSignalHandler(SIGUSR3, &signalHandler, 0);
  installSignalHandler(SIGUSR3, &cancelHandler, 0);
  installSignalHandler(SIGUSR3, &suspendHandler, 0);
}
*/
