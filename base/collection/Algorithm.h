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

#include <base/ForwardIterator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Bubble sort. O(n^2). Only forward iterator required. operator<= used for comparison of values. */
template<class ITERATOR>
void bubbleSort(const ITERATOR& _begin, const ITERATOR& _end)
{
  // implementation with forward iterator only - bidirectional iterator not required
  // TAG: we could do a merge sort also - by splitting items in the middle and sorting independently

  // static_assert(std::is_same<ForwardIterator, typename ITERATOR::Category>::value, "Iterator must be ForwardIterator.");
  const ForwardIterator* ensureForwardIterator = static_cast<const typename ITERATOR::Category*>(nullptr);

  auto end = _end; // we lower end per loop
  while (true) { // loop until all items sorted
    auto current = _begin; // restart
    if (current == end) { // done - no more items to sort
      break;
    }

    // move max to end
    bool swapped = false;
    auto next = current;
    ++next;
    while (next != end) {
      if (!(*current <= *next)) { // dont swap on = but < is the primary comparison operator for algorithms
        swapper(*current, *next);
        swapped = true;
      }
      ++current;
      ++next;
    }
    if (!swapped) {
      return; // nothing to do
    }
    end = current; // max now at end so no need to look at this again
  }
}

// TAG: add merge sort

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
