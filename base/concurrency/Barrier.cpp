/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/Barrier.h>
#include <base/concurrency/ExclusiveSynchronize.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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

unsigned int Barrier::wait(bool reset) throw(LockException) {
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
      ASSERT(waiting == 0);
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

Barrier::~Barrier() throw(LockException) {
  reset();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
