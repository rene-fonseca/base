/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/RecursiveMutualExclusion.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: reimplement using OS mutex

RecursiveMutualExclusion::RecursiveMutualExclusion()
{
}

void RecursiveMutualExclusion::exclusiveLock() const
{
  Profiler::WaitTask profile("RecursiveMutualExclusion::exclusiveLock()");
  
  const Thread::Identifier id = Thread::getIdentifier();
  if (owner != 0) {
    ExclusiveSynchronize<Guard> _guard(guard);
    if (owner == id) {
      ++numberOfLocks;
      return;
    }
  }
  lock.exclusiveLock(); // block
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    numberOfLocks = 1;
    owner = id;
  }
}

bool RecursiveMutualExclusion::tryExclusiveLock() const
{
  Profiler::WaitTask profile("RecursiveMutualExclusion::tryExclusiveLock()");
  
  const Thread::Identifier id = Thread::getIdentifier();
  ExclusiveSynchronize<Guard> _guard(guard);
  if (owner == id) {
    ++numberOfLocks;
    return true;
  } else if (numberOfLocks == 0) {
    if (lock.tryExclusiveLock()) {
      numberOfLocks = 1;
      owner = id;
      return true;
    }
  }
  return false;
}

void RecursiveMutualExclusion::releaseLock() const
{
  Profiler::pushSignal("RecursiveMutualExclusion::releaseLock()");
  // Thread::Identifier id = Thread::getIdentifier();
  ExclusiveSynchronize<Guard> _guard(guard);
  // bassert(owner == id, ConcurrencyException(this));
  if (--numberOfLocks == 0) {
    owner = 0; // relinquish ownership
    lock.releaseLock();
  }
}

RecursiveMutualExclusion::~RecursiveMutualExclusion()
{
#if 0
  if (!guard.tryExclusiveLock() || !lock.tryExclusiveLock()) {
    _throw MutualExclusionException(this);
  }
#endif
}


#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(RecursiveMutualExclusion) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_PROJECT("base/concurrency");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    // TAG: add Thread
    
    TEST_DECLARE_HERE(A);
    TEST_DECLARE_HERE(B);

    RecursiveMutualExclusion lock;
    if (lock.tryExclusiveLock()) {
      TEST_HERE(A);
      lock.releaseLock();
    }
    if (lock.tryExclusiveLock()) {
      TEST_HERE(B);
      lock.releaseLock();
    }
    lock.exclusiveLock();
    lock.releaseLock();
  }
};

TEST_REGISTER(RecursiveMutualExclusion);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
