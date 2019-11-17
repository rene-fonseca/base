/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/Barrier.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/Profiler.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Barrier::Barrier() throw(ResourceException)
  : count(0), waiting(0) {
}

unsigned int Barrier::getCount() const throw() {
  ExclusiveSynchronize<Guard> _guard(guard);
  return count;
}

void Barrier::setCount(unsigned int count) throw() {
  ExclusiveSynchronize<Guard> _guard(guard);
  this->count = count;
  if (waiting >= count) {
    event.signal();
  }
}

unsigned int Barrier::getWaiting() const throw() {
  ExclusiveSynchronize<Guard> _guard(guard);
  return waiting;
}

unsigned int Barrier::wait(bool reset) throw(LockException)
{
  Profiler::WaitTask profile("Barrier::wait()");
  {
    ExclusiveSynchronize<MutualExclusion> _guard(mutex); // blocking
    bool waitReset = false;
    {
      ExclusiveSynchronize<Guard> _guard(guard);
      waiting++;
      if ((waiting >= count) || reset) {
        waitReset = true;
        resetEvent.reset();
      }
    }
    if (waitReset) {
      event.signal(); // wake up all waiting threads
      resetEvent.wait();
      event.reset();
      waiting--;
      BASSERT(waiting == 0);
      return waiting;
    }
  }
  event.wait();
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    waiting--;
    if (waiting == 1) {
      resetEvent.signal();
    }
    return waiting;
  }
}

void Barrier::reset() throw(LockException) {
  wait(true);
}

Barrier::~Barrier() {
  reset();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
