/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_ITERATOR__INTERLEAVED_ITERATOR_H
#define _DK_SDU_MIP__BASE_ITERATOR__INTERLEAVED_ITERATOR_H

#include <base/Iterator.h>
#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Iterator used to traverse interleaved elements within a sequence.

  @short Iterator of interleaved elements within a sequence.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
template<class TRAITS>
class InterleavedIterator : public Iterator<TRAITS> {
public:
  
  typedef typename Iterator<TRAITS>::Distance Distance;
  typedef typename Iterator<TRAITS>::Pointer Pointer;
  typedef typename Iterator<TRAITS>::Reference Reference;
protected:
  
  /** The position of the iterator. */
  Pointer element;
  /** The fixed distance between "consecutive" elements. */
  unsigned int step;
public:

  /**
    Initializes iterator.

    @param element The initial value of the iterator.
    @param step The number of elements to skip over per element.
  */
  InterleavedIterator(Pointer element, unsigned int step) throw();

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
    return element[index * step];
  }  
};

template<class TRAITS>
inline InterleavedIterator<TRAITS>::InterleavedIterator(Pointer _element, unsigned int _step) throw()
  : element(_element), step(_step) {
}

template<class TRAITS>
inline InterleavedIterator<TRAITS> operator+(const InterleavedIterator<TRAITS>& left, int right) throw() {
  InterleavedIterator<TRAITS> result(left);
  return result += right;
}

template<class TRAITS>
inline InterleavedIterator<TRAITS> operator-(const InterleavedIterator<TRAITS>& left, int right) throw() {
  InterleavedIterator<TRAITS> result(left);
  return result -= right;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
