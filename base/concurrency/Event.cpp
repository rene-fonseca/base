/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Event.h"
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

Event::Event(unsigned int maximum) throw(Construct, ResourceException) {
  this->maximum = maximum;
  resetting = false;
  signaled = false;
  waitingThreads = 0;

  if (pthread_cond_init(&condition, NULL)) {
    throw ResourceException();
  }
}

void Event::reset() throw(MutualExclusion::MutualExclusionException) {
  MutualExclusion::lock();

    if (waitingThreads) {
      resetting = true;
      signaled = true;
      pthread_cond_broadcast(&condition); // make sure the waiting threads can continue

      while (resetting) { // wait until threads have been reset
        pthread_cond_wait(&condition, &mutex);
      }
    } else {
      signaled = false;
    }

  MutualExclusion::unlock();
}

void Event::signal() throw(MutualExclusion::MutualExclusionException) {
  MutualExclusion::lock();
    signaled = true;
    pthread_cond_broadcast(&condition); // make blocked threads unblocked
  MutualExclusion::unlock();
}

void Event::wait() throw(OutOfRange, MutualExclusion::MutualExclusionException) {
  MutualExclusion::lock();

    while (resetting) { // wait if resetting
      pthread_cond_wait(&condition, &mutex); // concurrent envocations of signal() does not matter
    }

    if (waitingThreads == maximum) {
      MutualExclusion::unlock();
      throw OutOfRange();
    }

    ++waitingThreads;
    while (!signaled) {
      pthread_cond_wait(&condition, &mutex); // concurrent envocations of signal() does not matter
    }
    --waitingThreads;

    if (!waitingThreads && resetting) { // can reset be completed
      signaled = false;
      resetting = false;
      pthread_cond_broadcast(&condition); // unblocked threads waiting for reset to complete
    }

  MutualExclusion::unlock();
}

bool Event::wait(unsigned int microseconds) throw(OutOfRange, MutualExclusion::MutualExclusionException) {
  int result = true; // no assignment gives warning

  MutualExclusion::lock();

    while (resetting) { // wait if resetting
      pthread_cond_wait(&condition, &mutex); // concurrent envocations of signal() does not matter
    }

    if (waitingThreads == maximum) {
      MutualExclusion::unlock();
      throw OutOfRange();
    }

    struct timespec absoluteTime;
    struct timeval now; // microsecond resolution
    gettimeofday(&now, NULL);
    long long nanoseconds = (now.tv_usec + microseconds) * 1000;
    absoluteTime.tv_sec = now.tv_sec + nanoseconds/1000000000;
    absoluteTime.tv_nsec = nanoseconds % 1000000000;

    ++waitingThreads;
    while (!signaled) { // wait for signal
      // concurrent envocations of signal() does not matter
      if (pthread_cond_timedwait(&condition, &mutex, &absoluteTime) == ETIMEDOUT) { // success, signal intr or timeout excepted
        break; // timed out
      }
    }
    --waitingThreads;

    if (!waitingThreads && resetting) { // can reset be completed
      signaled = false;
      resetting = false;
      pthread_cond_broadcast(&condition); // unblocked threads waiting for reset to complete
    }

  MutualExclusion::unlock();

  return result != ETIMEDOUT;
}

Event::~Event() throw() {
//  destroying = true;
//  waitingThreads -= maximum; // suspend if any waiting threads

//  signal();

  try {
    MutualExclusion::lock(); // possible exception
    MutualExclusion::unlock(); // possible exception

    pthread_cond_destroy(&condition); // should always succeed
  } catch(...) {
    // now what
    // cout << "Exception in Event::~Event()\n.";
  }
}
