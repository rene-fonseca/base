/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/concurrency/AtomicCounter.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Performance helpers. */

class _COM_AZURE_DEV__BASE__API Performance {
public:

  class Cache {
  public:

    MemorySize hits = 0;
    MemorySize misses = 0;
  };

  class _COM_AZURE_DEV__BASE__API Counter {
  public:

    PreferredAtomicCounter value;
    const char* id = nullptr;

    inline Counter(const char* _id = "") noexcept
      : id(_id)
    {
    }

    inline Counter& operator++() noexcept
    {
      ++value;
      return *this;
    }

    inline Counter& operator+=(MemorySize count) noexcept
    {
      value += count;
      return *this;
    }

    inline operator MemorySize() const noexcept
    {
      return value;
    }

    ~Counter();
  };
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
