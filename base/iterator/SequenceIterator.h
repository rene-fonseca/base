/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_ITERATOR__SEQUENCE_ITERATOR_H
#define _DK_SDU_MIP__BASE_ITERATOR__SEQUENCE_ITERATOR_H

#include <base/Iterator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Iterator used to traverse elements of a sequence (not to be confused with the
  Array collection).

  @short Iterator of elements of a sequence.
  @author René Møller Fonseca
  @version  1.0
*/
template<class TRAITS>
class SequenceIterator : public Iterator<TRAITS> {
protected:

  /** The position of the iterator. */
  Pointer element;
public:

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
  */
  explicit inline SequenceIterator(Pointer value) throw() : element(value) {}

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

  inline SequenceIterator operator+(Distance distance) throw() {
    SequenceIterator result(*this);
    result += distance;
    return result;
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

  /**
    Returns true if this iterator is greater than or equal to the specified iterator.
  */
  inline bool operator>=(const SequenceIterator& eq) const throw() {
    return element >= eq.element;
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

  friend long operator-<>(SequenceIterator left, SequenceIterator right) throw();
};

template<class TYPE>
inline long operator-(SequenceIterator<TYPE> left, SequenceIterator<TYPE> right) throw() {
  return left.element - right.element;
}

template<class TYPE>
inline SequenceIterator<TYPE> operator-(SequenceIterator<TYPE> left, int right) throw() {
  SequenceIterator<TYPE> temp = left;
  temp -= right;
  return temp;
}

template<class TYPE>
inline SequenceIterator<TYPE> operator+(SequenceIterator<TYPE> left, int right) throw() {
  SequenceIterator<TYPE> temp = left;
  temp += right;
  return temp;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
