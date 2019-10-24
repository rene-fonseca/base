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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
