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

/** Binary search. Compare operator must be <. Assume value is equal when !(a<b) && !(b<a). */
template<class ITERATOR, class TYPE, class COMPARE>
ITERATOR binarySearch(const ITERATOR& begin, const ITERATOR& end, const TYPE& find, COMPARE compare)
{
  const ForwardIterator* ensureForwardIterator = static_cast<const typename ITERATOR::Category*>(nullptr);
  // requires random access iterator
  if (begin == end) {
    return end; // not found
  }
  ITERATOR low = begin;
  ITERATOR high = end - 1; // last item // TAG: not desired for forward
  while (true) {
    const ITERATOR n = low + (high - low)/2; // mid - expensive for non-random iterator
    if (compare(find, *n)) { // find < *n - assumed expensive check
      if (n == low) { // assumed quick check
        return end; // not found
      }
      high = n - 1; // TAG: not desired for forward - we would need to cache from earlier distance iteration
    } else if (compare(*n, find)) { // *n < find - assumed expensive check
      if (n == high) { // assumed quick check
        return end; // not found
      }
      low = n + 1;
    } else {
      return n; // found
    }
  }
}

/** Binary search. Uses operator< only. Assume value is equal when !(a<b) && !(b<a). */
template<class ITERATOR, class TYPE>
ITERATOR binarySearch(const ITERATOR& begin, const ITERATOR& end, const TYPE& find)
{
  return binarySearch(begin, end, find, std::less<TYPE>());
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
