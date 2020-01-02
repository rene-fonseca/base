/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/ThreadPool.h>
#include <base/collection/Functor.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/concurrency/SharedSynchronize.h>
#include <base/Profiler.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

//class QueuedJobProvider : public JobProvider, public Queue<Job*> {
//public:
//
//  QueueJobProvider() {
//  }
//};

void ThreadPool::Wrapper::run() noexcept
{
  pool->run();
}



void ThreadPool::run() noexcept
{
  Thread* context = Thread::getThread();
  Runnable* job = nullptr;

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

ThreadPool::ThreadPool(JobProvider* _provider) noexcept
  : runnable(this),
    provider(_provider),
    desiredThreads(0),
    terminated(false),
    terminateOne(false)
{
}

ThreadPool::ThreadPool(JobProvider* _provider, unsigned int threads) noexcept
  : runnable(this),
    provider(_provider),
    desiredThreads(0),
    terminated(false),
    terminateOne(false)
{
  setThreads(threads);
}

unsigned int ThreadPool::getThreads() const noexcept
{
  SharedSynchronize<Guard> _guard(guard);
  return desiredThreads;
}

void ThreadPool::setThreads(unsigned int value)
{
  ExclusiveSynchronize<Guard> _guard(guard);

  if (terminated) {
    _throw ThreadPoolException("Thread pool has been terminated.", this);
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

void ThreadPool::terminate() noexcept
{
  ExclusiveSynchronize<Guard> _guard(guard);
  terminated = true;
  forEach(pool, invokeMember(&Thread::terminate));
}

void ThreadPool::join() noexcept
{
  Profiler::WaitTask profile("ThreadPool::join()");

  // threads should not be signaled here
  ExclusiveSynchronize<Guard> _guard(guard);
  forEach(pool, invokeMember(&Thread::join));
}

void ThreadPool::post() noexcept
{
  semaphore.post();
}

ThreadPool::~ThreadPool() noexcept
{
  terminate();
  join();

  Set<Thread*>::Enumerator enu = pool.getEnumerator();
  while (enu.hasNext()) {
    delete *enu.next();
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
