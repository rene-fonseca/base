/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_ITERATOR__MATRIX_ROW_ITERATOR_H
#define _DK_SDU_MIP__BASE_ITERATOR__MATRIX_ROW_ITERATOR_H

#include <base/iterator/InterleavedIterator.h>
#include <base/iterator/SequenceIterator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Row iterator for matrix.

  @short Matrix row iterator
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
template<class TRAITS>
class MatrixRowIterator : public InterleavedIterator<TRAITS> {
public:

  typedef SequenceIterator<TRAITS> ElementIterator;

  inline MatrixRowIterator(Pointer element, unsigned int columns) throw()
    : InterleavedIterator<TRAITS>(element, columns) {
  }

  /**
    Prefix increment.
  */
  inline MatrixRowIterator& operator++() throw() {
    element += step;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline MatrixRowIterator operator++(int) throw() {
    MatrixRowIterator result(*this);
    element += step;
    return result;
  }

  /**
    Prefix decrement.
  */
  inline MatrixRowIterator& operator--() throw() {
    element -= step;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline MatrixRowIterator operator--(int) throw() {
    MatrixRowIterator result(*this);
    element -= step;
    return result;
  }

  /**
    Move the specified distance forward.
  */
  inline MatrixRowIterator& operator+=(Distance distance) throw() {
    element += step * distance;
    return *this;
  }

  /**
    Move the specified distance backwards.
  */
  inline MatrixRowIterator& operator-=(Distance distance) throw() {
    element -= step * distance;
    return *this;
  }
  
  inline ElementIterator getFirst() const throw() {
    return ElementIterator(element);
  }

  inline ElementIterator getEnd() const throw() {
    return ElementIterator(element + step);
  }

  inline ElementIterator operator[](unsigned int index) const throw() {
    ASSERT(index < step);
    return ElementIterator(element + index);
  }
};

template<class TRAITS>
inline MatrixRowIterator<TRAITS> operator+(const MatrixRowIterator<TRAITS>& left, int right) throw() {
  MatrixRowIterator<TRAITS> result(left);
  return result += right;
}

template<class TRAITS>
inline MatrixRowIterator<TRAITS> operator-(const MatrixRowIterator<TRAITS>& left, int right) throw() {
  MatrixRowIterator<TRAITS> result(left);
  return result -= right;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
