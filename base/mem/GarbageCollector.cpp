/***************************************************************************
   The Base Framework
   A framework for developing platform independent applications

   See COPYRIGHT.txt for details.

   This framework is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   For the licensing terms refer to the file 'LICENSE'.
***************************************************************************/

#include <base/mem/GarbageCollector.h>
#include <base/concurrency/Thread.h>
#include <base/concurrency/MutualExclusion.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  class GarbageCollectorImpl : public Thread {
  public:
  
    bool stopped = true;
    Thread thread;
    MutualExclusion lock;
    Event signal;
    AnyReference references[4096];
    MemorySize count = 0;

    inline void release(const AnyReference& reference)
    {
      if (reference && !stopped) {
        MutualExclusion::Sync _sync(lock);
        if (count == getArraySize(references)) {
          // TAG: need to wait
        }
        references[count++] = reference;
        signal.signal();
      }
    }

    inline void release(AnyReference& reference)
    {
      // TAG: atomic allocate write position - swap atomic
      if (reference && !stopped) {
        MutualExclusion::Sync _sync(lock);
        if (count == getArraySize(references)) {
          // TAG: need to wait
        }
        references[count++] = reference;
        reference = nullptr;
        signal.signal();
      }
    }
    
    void onException(std::exception_ptr e)
    {
      // TAG: send to Application - get type of object
      ferr << "Error: Garbage collector exception." << ENDL;
    }

    void doRelease(MemorySize i)
    {
      auto& r = references[i];
      try {
        r = nullptr; // release
      } catch (...) {
        onException(std::current_exception());
      }
      if ((i + 1) < count) {
        r = moveObject(references[count - 1]);
      }
      --count;

      static MemorySize total = 0;
      ++total;
    }

    void releaseAll()
    {
      for (MemorySize i = count; i > 0; --i) {
        doRelease(i - 1);
      }
    }
    
    void run()
    {
      while (!stopped && signal.wait(10)) {
        signal.reset();
        MutualExclusion::Sync _sync(lock);
        for (MemorySize i = 0; i < count; ++i) {
          auto& r = references[i];
          if (!r.isMultiReferenced()) {
            doRelease(i);
          }
        }
      }
      
      releaseAll();
      BASSERT(count == 0);
    }
  };

  GarbageCollectorImpl collector;
}

void GarbageCollector::stop()
{
  collector.stopped = true;
}

bool GarbageCollector::isRunning()
{
  return collector.isAlive();
}

bool GarbageCollector::start()
{
  if (collector.isAlive()) {
    return false;
  }
  collector.thread.start();
  return true;
}

void GarbageCollector::release(const AnyReference& reference)
{
  if (reference) {
    collector.release(reference);
  }
}

void GarbageCollector::release(AnyReference& reference)
{
  if (reference) {
    auto r = moveObject(reference);
    collector.release(r);
  }
}

void garbageCollect(AnyReference& reference)
{
  if (reference) {
    auto r = moveObject(reference);
    collector.release(r);
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
