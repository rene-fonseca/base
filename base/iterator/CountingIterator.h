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

#include <base/Iterator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Counting iterator. */
class CountingIterator {
private:

  MemorySize value = 0;
public:

  inline CountingIterator() noexcept {
  }

  inline CountingIterator(MemorySize _value) noexcept
    : value(_value) {
  }

  inline MemorySize getValue() const noexcept
  {
    return value;
  }

  inline CountingIterator& operator++() noexcept
  {
    ++value;
    return *this;
  }

  inline CountingIterator operator++(int) noexcept
  {
    CountingIterator result = *this;
    ++value;
    return result;
  }

  inline bool operator==(const CountingIterator& i) const noexcept
  {
    return value == i.value;
  }

  inline bool operator!=(const CountingIterator& i) const noexcept
  {
    return value != i.value;
  }

  inline bool operator<(const CountingIterator& i) const noexcept
  {
    return value < i.value;
  }

  inline bool operator<=(const CountingIterator& i) const noexcept
  {
    return value <= i.value;
  }

  inline operator MemorySize() const noexcept
  {
    return value;
  }
};

inline MemoryDiff operator-(const CountingIterator& a, const CountingIterator& b) noexcept
{
  return a.getValue() - b.getValue();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
