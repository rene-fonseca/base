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
  Iterator used to traverse elements of a sequence (not to be confused with the
  Array collection).

  @short Iterator of elements of a sequence
*/
template<class TRAITS>
class SequenceIterator : public Iterator<TRAITS> {
public:

  typedef typename Iterator<TRAITS>::Value Value;
  typedef typename Iterator<TRAITS>::Reference Reference;
  typedef typename Iterator<TRAITS>::Pointer Pointer;
  typedef typename Iterator<TRAITS>::Distance Distance;
  typedef typename Iterator<TRAITS>::Category Category;
protected:

  /** The position of the iterator. */
  Pointer element = nullptr;
public:

  /**
    Initializes iterator.
  */
  inline SequenceIterator() noexcept
  {
  }

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
  */
  explicit inline SequenceIterator(const Pointer value) noexcept
   : element(value)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline SequenceIterator(const SequenceIterator& copy) noexcept
    : element(copy.element)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline SequenceIterator(SequenceIterator&& move) noexcept
    : element(moveObject(move.element))
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  template<class POLY>
  inline SequenceIterator(const SequenceIterator<POLY>& copy) noexcept
    : element(copy.getValue())
  {
  }
  
  /**
    Initializes iterator from other iterator.
  */
  inline SequenceIterator& operator=(const SequenceIterator& assign) noexcept
  {
    element = assign.element;
    return *this;
  }

  /**
    Initializes iterator from other iterator.
  */
  inline SequenceIterator& operator=(SequenceIterator&& assign) noexcept
  {
    element = moveObject(assign.element);
    return *this;
  }

  /**
    Initializes iterator from other iterator.
  */
  template<class POLY>
  inline SequenceIterator& operator=(const SequenceIterator<POLY>& assign) noexcept
  {
    element = assign.getValue();
    return *this;
  }

  /**
    Prefix increment.
  */
  inline SequenceIterator& operator++() noexcept
  {
    ++element;
    return *this;
  }

  /**
    Postfix increment.
  */
  inline SequenceIterator operator++(int) noexcept
  {
    SequenceIterator result(*this);
    ++element;
    return result;
  }

  /**
    Prefix decrement.
  */
  inline SequenceIterator& operator--() noexcept
  {
    --element;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline SequenceIterator operator--(int) noexcept
  {
    SequenceIterator result(*this);
    --element;
    return result;
  }

  /**
    Moves the specified distance forward.
  */
  inline SequenceIterator& operator+=(Distance distance) noexcept
  {
    element += distance;
    return *this;
  }

  /**
    Moves the specified distance backwards.
  */
  inline SequenceIterator& operator-=(Distance distance) noexcept
  {
    element -= distance;
    return *this;
  }

  /**
    Returns true if the iterators are equal.
  */
  inline bool operator==(const SequenceIterator& compare) const noexcept
  {
    return element == compare.element;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const SequenceIterator& compare) const noexcept
  {
    return !(element == compare.element);
  }

  /**
    Returns true if this iterator is less than the specified iterator.
  */
  inline bool operator<(const SequenceIterator& compare) const noexcept
  {
    return element < compare.element;
  }

  inline bool operator>(const SequenceIterator& compare) const noexcept
  {
    return element > compare.element;
  }
  
  /**
    Returns true if this iterator is greater than or equal to the specified
    iterator.
  */
  inline bool operator>=(const SequenceIterator& compare) const noexcept
  {
    return !(element < compare.element);
  }

  inline bool operator<=(const SequenceIterator& compare) const noexcept
  {
    return !(element > compare.element);
  }

  /**
    Access the element.
  */
  inline Reference operator*() const noexcept
  {
    return *element;
  }

  /**
    Access the element.
  */
  inline Pointer operator->() const noexcept
  {
    return element;
  }

  /**
    Returns the pointer value of the iterator.
  */
  inline Pointer getValue() const noexcept
  {
    return element;
  }

  /**
    Returns the element at the specified index from this element.
  */
  inline Reference operator[](MemoryDiff index) const noexcept
  {
    return element[index];
  }
};

template<class LTRAITS, class RTRAITS>
inline MemoryDiff operator-(
  const SequenceIterator<LTRAITS>& left,
  const SequenceIterator<RTRAITS>& right) noexcept
{
  return left.getValue() - right.getValue();
}

template<class TYPE>
inline SequenceIterator<TYPE> operator-(const SequenceIterator<TYPE>& left, MemoryDiff right) noexcept
{
  SequenceIterator<TYPE> temp(left);
  return temp -= right;
}

template<class TYPE>
inline SequenceIterator<TYPE> operator+(const SequenceIterator<TYPE>& left, MemoryDiff right) noexcept
{
  SequenceIterator<TYPE> temp(left);
  return temp += right;
}

template<class TYPE>
inline SequenceIterator<TYPE> operator+(MemoryDiff left, const SequenceIterator<TYPE>& right) noexcept
{
  SequenceIterator<TYPE> temp(right);
  return temp += left;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

_COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(base::SequenceIterator);
