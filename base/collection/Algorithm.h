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

/** Compute task for profiler. */
class _COM_AZURE_DEV__BASE__API ComputeTask {
private:

  /** The ID of the task. */
  unsigned int taskId = static_cast<unsigned int>(-1);
public:
  
  /** Initializes compute task. */
  ComputeTask(const char* name) noexcept;
  
  ~ComputeTask() noexcept;
};

/** Bubble sort. O(n^2). Only forward iterator required. operator<= used for comparison of values. */
template<class ITERATOR>
void bubbleSort(const ITERATOR& _begin, const ITERATOR& _end)
{
  ComputeTask profiler("bubbleSort()");
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
  ComputeTask profiler("bubbleSort()");
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

template<class ITERATOR>
class ForwardIterable {
private:

  ITERATOR tbegin;
  ITERATOR tend;
public:
  
  inline ForwardIterable(const ITERATOR _begin, const ITERATOR _end)
    : tbegin(_begin), tend(_end)
  {
    const ForwardIterator* ensure = static_cast<const typename ITERATOR::Category*>(nullptr);
  }
  
  inline ITERATOR begin()
  {
    return tbegin;
  }

  inline ITERATOR end()
  {
    return tend;
  }
};

template<class FORWARD_ITERABLE>
inline void bubbleSort(FORWARD_ITERABLE& iterable)
{
  bubbleSort(iterable.begin(), iterable.end());
}

template<class FORWARD_ITERABLE, class PREDICATE>
inline void bubbleSort(FORWARD_ITERABLE& iterable, PREDICATE predicate)
{
  bubbleSort(iterable.begin(), iterable.end(), predicate);
}

#if 0
template<class ITERATOR>
inline void bubbleSort(ForwardIterable<ITERATOR>& iterable)
{
  bubbleSort(iterable.begin(), iterable.end());
}

template<class ITERATOR, class PREDICATE>
inline void bubbleSort(ForwardIterable<ITERATOR>& iterable, PREDICATE predicate)
{
  bubbleSort(iterable.begin(), iterable.end(), predicate);
}
#endif

template<class ITERATOR>
class RandomAccessIterable : public ForwardIterable<ITERATOR> {
public:
  
  inline RandomAccessIterable(const ITERATOR _begin, const ITERATOR _end)
    : ForwardIterable<ITERATOR>(_begin, _end)
  {
    const RandomAccessIterator* ensure = static_cast<const typename ITERATOR::Category*>(nullptr);
  }
};

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

/**
  Merge sort. O(n log(n)). Random access iterator required. operator<= used for comparison of values.

  It is recommended that the Value of the iterator supports move assignment.
*/
template<class ITERATOR>
void mergeSort(const ITERATOR& begin, const ITERATOR& end)
{
  ComputeTask profiler("mergeSort()");
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
  // TAG: for concurrency split in n equal blocks >= 8*1024
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
  ComputeTask profiler("mergeSort()");
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

template<class RANDOM_ACCESS_ITERABLE>
inline void mergeSort(RANDOM_ACCESS_ITERABLE& iterable)
{
  mergeSort(iterable.begin(), iterable.end());
}

template<class RANDOM_ACCESS_ITERABLE, class PREDICATE>
inline void mergeSort(RANDOM_ACCESS_ITERABLE& iterable, PREDICATE predicate)
{
  mergeSort(iterable.begin(), iterable.end(), predicate);
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

template<class RANDOM_ACCESS_ITERABLE, class TYPE>
inline typename RANDOM_ACCESS_ITERABLE::Iterator binarySearch(RANDOM_ACCESS_ITERABLE& iterable, const TYPE& find)
{
  return binarySearch(iterable.begin(), iterable.end(), find);
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

template<class RANDOM_ACCESS_ITERABLE>
inline void shuffle(RANDOM_ACCESS_ITERABLE& iterable)
{
  shuffle(iterable.begin(), iterable.end());
}

/** Returns true if equal. */
template<class ITERATOR>
bool equal(ITERATOR a, const ITERATOR& aEnd, ITERATOR b, const ITERATOR& bEnd)
{
  // TAG: if random iterator check sizes first
  for (; (a != aEnd) && (b != bEnd); ++a, ++b) {
    if (!(*a == *b)) {
      return false;
    }
  }
  return true;
}

/** Returns true if less than. */
template<class ITERATOR>
bool lessThan(ITERATOR a, const ITERATOR& aEnd, ITERATOR b, const ITERATOR& bEnd)
{
  for (; (a != aEnd) && (b != bEnd); ++a, ++b) {
    if (*a < *b) {
      return true;
    } else if (*a == *b) {
      continue;
    }
    return false;
  }
  return (b != bEnd);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
