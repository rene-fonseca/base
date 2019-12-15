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
#include <base/Random.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Bubble sort. O(n^2). Only forward iterator required. operator<= used for comparison of values. */
template<class ITERATOR>
void bubbleSort(const ITERATOR& _begin, const ITERATOR& _end)
{
  // implementation with forward iterator only - bidirectional iterator not required

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

template<class ITERATOR, class PREDICATE>
void bubbleSort(const ITERATOR& _begin, const ITERATOR& _end, PREDICATE predicate)
{
  // implementation with forward iterator only - bidirectional iterator not required

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
      if (!predicate(*current, *next)) {
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

template<class TYPE>
inline void mergeSortTiny(TYPE* begin, const MemorySize size)
{
  auto end = begin + size; // we lower end per loop
  while (true) { // loop until all items sorted
    auto current = begin; // restart
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

template<class TYPE, class PREDICATE>
inline void mergeSortTiny(TYPE* begin, const MemorySize size, PREDICATE predicate)
{
  auto end = begin + size; // we lower end per loop
  while (true) { // loop until all items sorted
    auto current = begin; // restart
    if (current == end) { // done - no more items to sort
      break;
    }

    // move max to end
    bool swapped = false;
    auto next = current;
    ++next;
    while (next != end) {
      if (!predicate(*current, *next)) {
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

template<class TYPE>
inline void mergeSortMerge(TYPE* a, const TYPE* aEnd, TYPE* b, const TYPE* bEnd, TYPE* dest)
{
  while ((a != aEnd) && (b != bEnd)) {
    if (*a <= *b) {
      *dest++ = moveObject(*a++);
    } else {
      *dest++ = moveObject(*b++);
    }
  }
  // either a or b remaining
  while (a != aEnd) {
    *dest++ = moveObject(*a++);
  }
  while (b != bEnd) {
    *dest++ = moveObject(*b++);
  }
}

template<class TYPE, class PREDICATE>
inline void mergeSortMerge(TYPE* a, const TYPE* aEnd, TYPE* b, const TYPE* bEnd, TYPE* dest, PREDICATE predicate)
{
  while ((a != aEnd) && (b != bEnd)) {
    if (predicate(*a, *b)) {
      *dest++ = moveObject(*a++);
    } else {
      *dest++ = moveObject(*b++);
    }
  }
  // either a or b remaining
  while (a != aEnd) {
    *dest++ = moveObject(*a++);
  }
  while (b != bEnd) {
    *dest++ = moveObject(*b++);
  }
}

#if 0
template<class TYPE>
void mergeSort(TYPE* begin, const MemorySize size, TYPE* dest)
{
  if (size <= 8) { // TAG: what is the proper limit?
    mergeSortTiny(dest, size);
    return; // sorted
  }
  const auto m = size / 2;
  mergeSort(dest, m, begin);
  mergeSort(dest + m, size - m, begin + m);
  mergeSortMerge(begin, begin + m, begin + m, begin + size, dest);
}
#endif

/**
  Merge sort. O(n log(n)). Random access iterator required. operator<= used for comparison of values.

  It is recommended that the Value of the iterator supports move assignment.
*/
template<class ITERATOR>
void mergeSort(const ITERATOR& begin, const ITERATOR& end)
{
  // static_assert(std::is_same<RandomIterator, typename ITERATOR::Category>::value, "Iterator must be RandomAccessIterator.");
  const RandomAccessIterator* ensureRandomAccessIterator =
    static_cast<const typename ITERATOR::Category*>(nullptr);
  
  constexpr MemorySize TINY = 8;

  const MemorySize size = end - begin;
  if (size <= TINY) {
    bubbleSort(begin, end);
    return; // nothing to do
  }

  // TAG: not optimized - 2 buffers used - need to use indirection for fast move
  // TAG: do not use indirection if TYPE is primitive/sizeof(TYPE) <= 8
  typedef typename ITERATOR::Value TYPE;
  Allocator<TYPE> temp(size);
  Allocator<TYPE> sorted(size);
  {
    TYPE* dest = sorted.getElements();
    for (ITERATOR src = begin; src != end; ++src, ++dest) {
      *dest = moveObject(*src);
    }
  }
  // TAG: for concurrency split in n equal blocks
  TYPE* src = sorted.getElements();
  TYPE* dest = temp.getElements();

  // presort tiny blocks
  for (MemorySize i = 0; i < size; i += TINY) {
    mergeSortTiny(src + i, minimum(size - i, TINY));
  }

  for (MemorySize width = TINY; width < size; width *= 2) {
    for (MemorySize i = 0; i < size; ) { // merge pair of blocks
      const MemorySize m = minimum(i + width, size);
      const MemorySize end = minimum(i + 2 * width, size);
      mergeSortMerge(src + i, src + m, src + m, src + end, dest + i);
      i = end;
    }
    swapper(dest, src);
  }
  for (ITERATOR dest = begin; dest != end; ++src, ++dest) {
    *dest = moveObject(*src);
  }
}

template<class ITERATOR, class PREDICATE>
void mergeSort(const ITERATOR& begin, const ITERATOR& end, PREDICATE predicate)
{
  // static_assert(std::is_same<RandomIterator, typename ITERATOR::Category>::value, "Iterator must be RandomAccessIterator.");
  const RandomAccessIterator* ensureRandomAccessIterator =
    static_cast<const typename ITERATOR::Category*>(nullptr);
  
  constexpr MemorySize TINY = 8;

  const MemorySize size = end - begin;
  if (size <= TINY) {
    bubbleSort(begin, end, predicate);
    return; // nothing to do
  }

  // TAG: not optimized - 2 buffers used - need to use indirection for fast move
  // TAG: do not use indirection if TYPE is primitive/sizeof(TYPE) <= 8
  typedef typename ITERATOR::Value TYPE;
  Allocator<TYPE> temp(size);
  Allocator<TYPE> sorted(size);
  {
    TYPE* dest = sorted.getElements();
    for (ITERATOR src = begin; src != end; ++src, ++dest) {
      *dest = moveObject(*src);
    }
  }
  // TAG: for concurrency split in n equal blocks
  TYPE* src = sorted.getElements();
  TYPE* dest = temp.getElements();

  // presort tiny blocks
  for (MemorySize i = 0; i < size; i += TINY) {
    mergeSortTiny(src + i, minimum(size - i, TINY), predicate);
  }

  for (MemorySize width = TINY; width < size; width *= 2) {
    for (MemorySize i = 0; i < size; ) { // merge pair of blocks
      const MemorySize m = minimum(i + width, size);
      const MemorySize end = minimum(i + 2 * width, size);
      mergeSortMerge(src + i, src + m, src + m, src + end, dest + i, predicate);
      i = end;
    }
    swapper(dest, src);
  }
  for (ITERATOR dest = begin; dest != end; ++src, ++dest) {
    *dest = moveObject(*src);
  }
}

/**
  Binary search.

  Compare operator must be <. Assume value is equal when !(a<b) && !(b<a).

  Random access iterator is recommended. Although forward iterator will still work.
*/
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

/**
  Binary search.

  Compare operator must be <. Assume value is equal when !(a<b) && !(b<a).

  Random access iterator is recommended. Although forward iterator will still work.
*/
template<class ITERATOR, class TYPE>
ITERATOR binarySearch(const ITERATOR& begin, const ITERATOR& end, const TYPE& find)
{
  return binarySearch(begin, end, find, std::less<TYPE>());
}

/**
  Shuffles elements for the given iterators.
  See https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#Modern_method.
*/
template<class ITERATOR>
void shuffle(const ITERATOR& begin, const ITERATOR& end)
{
  const RandomAccessIterator* ensureIterator = static_cast<const typename ITERATOR::Category*>(nullptr);
  auto n = end - begin;
  while (n > 1) {
    MemorySize i = Random::random<MemorySize>() % n;
    --n;
    swapper(begin[i], begin[n]); // move to last
  }
}

/**
  Shuffles elements for the given iterators.
  See https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#Modern_method.
*/
template<class TYPE>
void shuffle(TYPE* begin, TYPE* end)
{
  auto n = end - begin;
  while (n > 1) {
    MemorySize i = Random::random<MemorySize>() % n;
    --n;
    swapper(begin[i], begin[n]); // move to last
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
