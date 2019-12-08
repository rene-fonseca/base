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
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Barrier::Barrier()
{
}

unsigned int Barrier::getCount() const
{
  ExclusiveSynchronize<Guard> _guard(guard);
  return count;
}

void Barrier::setCount(unsigned int count)
{
  ExclusiveSynchronize<Guard> _guard(guard);
  this->count = count;
  if (waiting >= count) {
    event.signal();
  }
}

unsigned int Barrier::getWaiting() const
{
  ExclusiveSynchronize<Guard> _guard(guard);
  return waiting;
}

unsigned int Barrier::wait(bool reset)
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

void Barrier::reset()
{
  wait(true);
}

Barrier::~Barrier()
{
  reset();
}

#if 0 && defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Barrier) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_PROJECT("base/concurrency");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    Barrier b;
    TEST_ASSERT(b.getCount() == 0);
    TEST_ASSERT(b.getWaiting() == 0);
    b.setCount(10);
    TEST_ASSERT(b.getCount() == 10);
    b.setCount(0);
    // b.wait();
    // b.reset();
  }
};

TEST_REGISTER(Barrier);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
