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
#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Iterator used to traverse interleaved elements within a sequence.

  @short Iterator of interleaved elements within a sequence.
*/
template<class TRAITS>
class InterleavedIterator : public Iterator<TRAITS> {
public:
  
  typedef typename Iterator<TRAITS>::Value Value;
  typedef typename Iterator<TRAITS>::Reference Reference;
  typedef typename Iterator<TRAITS>::Pointer Pointer;
  typedef typename Iterator<TRAITS>::Distance Distance;
  typedef typename Iterator<TRAITS>::Category Category;
protected:
  
  /** The position of the iterator. */
  Pointer element;
  /** The fixed distance between "consecutive" elements. */
  MemoryDiff step = 0;
public:

  /**
    Initializes iterator.

    @param element The initial value of the iterator.
    @param step The number of elements to skip over per element.
  */
  InterleavedIterator(Pointer element, MemoryDiff step);

  /**
    Prefix increment.
  */
  inline InterleavedIterator& operator++()
  {
    element += step;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline InterleavedIterator operator++(int)
  {
    InterleavedIterator result(*this);
    element += step;
    return result;
  }

  /**
    Prefix decrement.
  */
  inline InterleavedIterator& operator--()
  {
    element -= step;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline InterleavedIterator operator--(int)
  {
    InterleavedIterator result(*this);
    element -= step;
    return result;
  }

  /**
    Move the specified distance forward.
  */
  inline InterleavedIterator& operator+=(Distance distance)
  {
    element += step * distance;
    return *this;
  }

  /**
    Move the specified distance backwards.
  */
  inline InterleavedIterator& operator-=(Distance distance)
  {
    element -= step * distance;
    return *this;
  }

  /**
    Returns true if the iterators are equal.
  */
  inline bool operator!=(const InterleavedIterator& compare) const
  {
    return element != compare.element;
  }

  /**
    Returns true if this iterator is less than the specified iterator.
  */
  inline bool operator<(const InterleavedIterator& compare) const
  {
    return element < compare.element;
  }

  /**
    Access the element.
  */
  inline Reference operator*() const
  {
    return *element;
  }

  /**
    Access the element.
  */
  inline Pointer operator->() const
  {
    return element;
  }

  /**
    Returns the pointer value of the iterator.
  */
  inline Pointer getValue() const
  {
    return element;
  }

  /**
    Returns the element at the specified index from this element.
  */
  inline Reference operator[](MemoryDiff index) const
  {
    return element[index * step];
  }  
};

template<class TRAITS>
inline InterleavedIterator<TRAITS>::InterleavedIterator(Pointer _element, MemoryDiff _step)
  : element(_element),
    step(_step)
{
}

template<class TRAITS>
inline InterleavedIterator<TRAITS> operator+(const InterleavedIterator<TRAITS>& left, MemoryDiff right)
{
  InterleavedIterator<TRAITS> result(left);
  return result += right;
}

template<class TRAITS>
inline InterleavedIterator<TRAITS> operator-(const InterleavedIterator<TRAITS>& left, MemoryDiff right)
{
  InterleavedIterator<TRAITS> result(left);
  return result -= right;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

_COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(base::InterleavedIterator);
