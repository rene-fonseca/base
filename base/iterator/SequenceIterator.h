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
  @version  1.0
*/
template<class TRAITS>
class SequenceIterator : public Iterator<TRAITS> {
protected:

  typedef typename Iterator<TRAITS>::Pointer Pointer;
  typedef typename Iterator<TRAITS>::Reference Reference;
  typedef typename Iterator<TRAITS>::Distance Distance;

  /** The position of the iterator. */
  Pointer element;
public:

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
  */
  explicit inline SequenceIterator(Pointer value) throw() : element(value) {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline SequenceIterator(const SequenceIterator& copy) throw()
    : element(copy.element) {
  }

  /**
    Initializes iterator from other iterator.
  */
  template<class POLY>
  inline SequenceIterator(const SequenceIterator<POLY>& copy) throw()
    : element(copy.getValue()) {
  }
  
  /**
    Initializes iterator from other iterator.
  */
  inline SequenceIterator& operator=(const SequenceIterator& eq) throw() {
    element = eq.element;
    return *this;
  }

  /**
    Initializes iterator from other iterator.
  */
  template<class POLY>
  inline SequenceIterator& operator=(
    const SequenceIterator<POLY>& eq) throw() {
    element = eq.getValue();
    return *this;
  }

  /**
    Prefix increment.
  */
  inline SequenceIterator& operator++() throw() {
    ++element;
    return *this;
  }

  /**
    Postfix increment.
  */
  inline SequenceIterator operator++(int) throw() {
    SequenceIterator result(*this);
    ++element;
    return result;
  }

  /**
    Prefix decrement.
  */
  inline SequenceIterator& operator--() throw() {
    --element;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline SequenceIterator operator--(int) throw() {
    SequenceIterator result(*this);
    --element;
    return result;
  }

  /**
    Moves the specified distance forward.
  */
  inline SequenceIterator& operator+=(Distance distance) throw() {
    element += distance;
    return *this;
  }

  /**
    Moves the specified distance backwards.
  */
  inline SequenceIterator& operator-=(Distance distance) throw() {
    element -= distance;
    return *this;
  }

  /**
    Returns true if the iterators are equal.
  */
  inline bool operator==(const SequenceIterator& eq) const throw() {
    return element == eq.element;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const SequenceIterator& eq) const throw() {
    return element != eq.element;
  }

  /**
    Returns true if this iterator is less than the specified iterator.
  */
  inline bool operator<(const SequenceIterator& eq) const throw() {
    return element < eq.element;
  }

  inline bool operator<=(const SequenceIterator& eq) const throw() {
    return element <= eq.element;
  }
  
  /**
    Returns true if this iterator is greater than or equal to the specified
    iterator.
  */
  inline bool operator>=(const SequenceIterator& eq) const throw() {
    return element >= eq.element;
  }

  inline bool operator>(const SequenceIterator& eq) const throw() {
    return element > eq.element;
  }
  
  /**
    Access the element.
  */
  inline Reference operator*() const throw() {
    return *element;
  }

  /**
    Access the element.
  */
  inline Pointer operator->() const throw() {
    return element;
  }

  /**
    Returns the pointer value of the iterator.
  */
  inline Pointer getValue() const throw() {
    return element;
  }

  /**
    Returns the element at the specified index from this element.
  */
  inline Reference operator[](int index) const throw() {
    return element[index];
  }
};

template<class LTRAITS, class RTRAITS>
inline MemoryDiff operator-(
  const SequenceIterator<LTRAITS>& left,
  const SequenceIterator<RTRAITS>& right) throw() {
  return left.getValue() - right.getValue();
}

template<class TYPE>
inline SequenceIterator<TYPE> operator-(
  const SequenceIterator<TYPE>& left, int right) throw() {
  SequenceIterator<TYPE> temp(left);
  return temp -= right;
}

template<class TYPE>
inline SequenceIterator<TYPE> operator+(
  const SequenceIterator<TYPE>& left, int right) throw() {
  SequenceIterator<TYPE> temp(left);
  return temp += right;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
