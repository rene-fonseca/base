/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/concurrency/ThreadPool.h>
#include <base/collection/Functor.h>
#include <base/Trace.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

//class QueuedJobProvider : public JobProvider, public Queue<Job*> {
//public:
//
//  QueueJobProvider() {}
//};

void ThreadPool::Wrapper::run() throw() {
  pool->run();
}



void ThreadPool::run() throw() {
  Thread* context = Thread::getThread();
  Runnable* job;

  while (!context->isTerminated()) {
    semaphore.wait(); // wait for job or termination event

    threadLock.exclusiveLock(); // synchronize threads in the pool

    if (context->isTerminated()) { // has thread already been terminated
      threadLock.releaseLock();
      break; // leave
    }

    if (terminateOne) { // should the thread be terminated
      context->terminate();
      terminatedThread = context;
      termination.post(); // signal the thread that requested the termination
      terminateOne = false;
      threadLock.releaseLock();
      break; // leave
    }

    if (provider->isEmpty()) { // is job available
      threadLock.releaseLock();
      continue; // try again
    }

    job = provider->pop();
    threadLock.releaseLock();

    job->run();

//    threadLock.exclusiveLock();
//    if (!terminatedOne && !terminate && context->isTerminated()) { // has thread been terminated by job
//      // what should we do if the job terminates the thread
//    }
//    threadLock.releaseLock();
  }
}

ThreadPool::ThreadPool(JobProvider* provider) throw() :
  runnable(this), provider(provider), desiredThreads(0), terminated(false), terminateOne(false) {
}

ThreadPool::ThreadPool(JobProvider* provider, unsigned int threads) throw() :
  runnable(this), provider(provider), desiredThreads(0), terminated(false), terminateOne(false) {
  setThreads(threads);
}

unsigned int ThreadPool::getThreads() const throw() {
  SynchronizeShared();
  return desiredThreads;
}

void ThreadPool::setThreads(unsigned int value) throw(ThreadPoolException) {
  SynchronizeExclusively();

  if (terminated) {
    ThreadPoolException("Thread pool has been terminated");
  }

  if (value != desiredThreads) {

    if (value > desiredThreads) { // should we added thread to the pool

      threadLock.exclusiveLock();
      unsigned int count = value - desiredThreads; // number of threads to add
      while (count--) {
        Thread* thread = new Thread(&runnable);
        pool.add(thread);
        thread->start();
      }
      desiredThreads = value;
      threadLock.releaseLock();

    } else if (value < desiredThreads) { // should we remove threads
      threadLock.exclusiveLock();

      unsigned int count = desiredThreads - value; // number of threads to terminate
      desiredThreads = value;

      while (count--) { // threads are terminated one by one
        terminateOne = true; // tell random thread to terminate
        semaphore.post(); // wake one thread

        threadLock.releaseLock();
        termination.wait(); // wait for a thread to be terminated
        threadLock.exclusiveLock();

        terminatedThread->join(); // wait for thread to finalize
        delete terminatedThread;
        pool.remove(terminatedThread);
      }

      threadLock.releaseLock();
    }
  }
}

void ThreadPool::terminate() throw() {
  SynchronizeExclusively();
  terminated = true;
  forEach(pool, invokeMember(&Thread::terminate));
}

void ThreadPool::join() throw() {
  // threads should not be signaled here
  SynchronizeExclusively();
  forEach(pool, invokeMember(&Thread::join));
}

void ThreadPool::post() throw() {
  semaphore.post();
}

ThreadPool::~ThreadPool() throw() {
  TRACE_MEMBER();
  terminate();
  join();

  Set<Thread*>::Enumeration enu(pool);
  while (enu.hasNext()) {
    delete *enu.next();
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
