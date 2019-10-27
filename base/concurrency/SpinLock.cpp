/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/SpinLock.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

SpinLock::SpinLock() noexcept
  : value(LOCK_FREE)
{
}

bool SpinLock::invariant() const noexcept
{
  ExclusiveSynchronize<SpinLock> _guard(*this);

  MemoryDiff current = value;
  return (current == LOCK_FREE) || (current == LOCK_TAKEN);
}

void SpinLock::exclusiveLock() const noexcept
{
  while (true) {
    MemoryDiff expected = LOCK_FREE;
    if (value.compareAndExchangeWeak(expected, LOCK_TAKEN)) {
      break;
    }

#if defined(_DEBUG)
    MemoryDiff current = value;
    ASSERT((current == LOCK_FREE) || (current == LOCK_TAKEN));
#endif

    // yield
  }
}

bool SpinLock::tryExclusiveLock() const noexcept
{
  MemoryDiff expected = LOCK_FREE;
  return value.compareAndExchangeWeak(expected, LOCK_TAKEN);
}

void SpinLock::releaseLock() const noexcept {
  value = LOCK_FREE;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(SpinLock) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    // TAG: add Thread
    
    TEST_DECLARE_HERE(A);
    TEST_DECLARE_HERE(B);

    SpinLock lock;
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

TEST_REGISTER(SpinLock);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
