/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_ITERATOR__INTERLEAVED_ITERATOR_H
#define _DK_SDU_MIP__BASE_ITERATOR__INTERLEAVED_ITERATOR_H

#include <base/Iterator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Iterator used to traverse interleaved elements within a sequence.

  @short Iterator of interleaved elements within a sequence.
  @author Ren� M�ller Fonseca
  @version 1.0
*/
template<class TRAITS>
class InterleavedIterator : public Iterator<TRAITS> {
protected:

  /** The position of the iterator. */
  Pointer element;
  /** The fixed distance between "consecutive" elements. */
  const unsigned int step;
public:

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
    @param step The number of elements to skip over per element.
  */
  InterleavedIterator(Pointer value, unsigned int step) throw();

  /**
    Prefix increment.
  */
  inline InterleavedIterator& operator++() throw() {
    element += step;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline InterleavedIterator operator++(int) throw() {
    InterleavedIterator result(*this);
    element += step;
    return result;
  }

  /**
    Prefix decrement.
  */
  inline InterleavedIterator& operator--() throw() {
    element -= step;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline InterleavedIterator operator--(int) throw() {
    InterleavedIterator result(*this);
    element -= step;
    return result;
  }

  /**
    Move the specified distance forward.
  */
  inline InterleavedIterator& operator+=(Distance distance) throw() {
    element += step * distance;
    return *this;
  }

  /**
    Move the specified distance backwards.
  */
  inline InterleavedIterator& operator-=(Distance distance) throw() {
    element -= step * distance;
    return *this;
  }

  /**
    Returns true if the iterators are equal.
  */
  inline bool operator!=(const InterleavedIterator& eq) const throw() {
    return element != eq.element;
  }

  /**
    Returns true if this iterator is less than the specified iterator.
  */
  inline bool operator<(const InterleavedIterator& eq) const throw() {
    return element < eq.element;
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
};

template<class TRAITS>
inline InterleavedIterator<TRAITS>::InterleavedIterator(Pointer value, unsigned int s) throw()
  : element(value), step(s) {}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
