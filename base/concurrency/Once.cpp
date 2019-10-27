/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/Once.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Once::Once() noexcept {
}

bool Once::getTokenImpl() throw(ResourceException) {
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    if (waiting++ == 0) {
      return true; // only true
    }
    if (!mask) {
      return false;
    }
    if (!event) {
      event = new Event();
    }
  }
  event->wait();
  {
    ExclusiveSynchronize<Guard> _guard(guard);
    waiting--;
    if (!waiting) {
      delete event;
      event = 0;
    }
  }
  return false;
}

void Once::relinquishToken() noexcept {
  ExclusiveSynchronize<Guard> _guard(guard);
  waiting--;
  mask = false;
  if (event) {
    event->signal();
  }
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Once) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    // TAG: add Thread
    
    TEST_DECLARE_HERE(A);
    TEST_DECLARE_NOT_HERE(B);

    static Once once;
    if (once.getToken()) {
      TEST_HERE(A);
      once.relinquishToken();
    }
    
    if (once.getToken()) {
      TEST_NOT_HERE(B);
      once.relinquishToken();
    }
  }
};

TEST_REGISTER(Once);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
