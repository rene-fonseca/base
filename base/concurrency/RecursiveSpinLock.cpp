/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/RecursiveSpinLock.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

RecursiveSpinLock::RecursiveSpinLock() noexcept
{
}

void RecursiveSpinLock::exclusiveLock() const noexcept
{
  Thread::Identifier id = Thread::getIdentifier();
  {
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

bool RecursiveSpinLock::tryExclusiveLock() const noexcept
{
  Thread::Identifier id = Thread::getIdentifier();
  ExclusiveSynchronize<Guard> _guard(guard);
  if (owner == id) {
    ++numberOfLocks;
    return true;
  } else if (numberOfLocks == 0) {
    numberOfLocks = 1;
    owner = id;
    return true;
  } else {
    return false;
  }
}

void RecursiveSpinLock::releaseLock() const noexcept
{
  Profiler::pushSignal("RecursiveSpinLock::releaseLock()");
  // Thread::Identifier id = Thread::getIdentifier();
  ExclusiveSynchronize<Guard> _guard(guard);
  // bassert(owner == id, ConcurrencyException(this));
  if (--numberOfLocks == 0) {
    owner = 0; // relinquish ownership
    lock.releaseLock();
  }
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(RecursiveSpinLock) : public UnitTest {
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
    TEST_DECLARE_HERE(C);

    RecursiveSpinLock lock;
    if (lock.tryExclusiveLock()) {
      TEST_HERE(A);
      if (lock.tryExclusiveLock()) {
        TEST_HERE(B);
        lock.releaseLock();
      }
      lock.releaseLock();
    }
    if (lock.tryExclusiveLock()) {
      TEST_HERE(C);
      lock.releaseLock();
    }
    lock.exclusiveLock();
    lock.releaseLock();
  }
};

TEST_REGISTER(RecursiveSpinLock);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
